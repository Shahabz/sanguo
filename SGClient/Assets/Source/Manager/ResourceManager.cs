using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;
using LuaInterface;
using System.Reflection;
using System.IO;

// 添加引用计数的结构
public class AssetBundleInfo
{
    public AssetBundle m_AssetBundle;
    public int m_ReferencedCount;
    public AssetBundleInfo( AssetBundle assetBundle )
    {
        m_AssetBundle = assetBundle;
        m_ReferencedCount = 1;
    }
}

// 异步读取结构
public class LoadAssetRequest
{
    public Type assetType;
    public string assetNames;
    public LuaFunction luaFunc;
    public Action<UnityEngine.Object> sharpFunc;
}

// 数据管理器 
public class ResourceManager : MonoBehaviour
{
    static Dictionary<string, string[]> m_resmap_sprite;    // 资源映射表
    static Dictionary<string, string[]> m_resmap_prefab;    // 资源映射表
    static Dictionary<string, string[]> m_resmap_object;    // 资源映射表

    // AssetBundle名称列表
    static string[] m_AssetBundleNameList = null;

    // Manifest文件缓存
    static AssetBundleManifest m_AssetBundleManifest = null;

    // 每个AssetBundle的依赖信息
    static Dictionary<string, string[]> m_Dependencies = new Dictionary<string, string[]>();

    // 正在请求异步加载的资源
    static Dictionary<string, List<LoadAssetRequest>> m_LoadRequests = new Dictionary<string, List<LoadAssetRequest>>();

    // 已经加载的AssetBundle
    static Dictionary<string, AssetBundleInfo> m_LoadedAssetBundles = new Dictionary<string, AssetBundleInfo>();

    void Awake()
    {
        InitResMap();
    }

    void OnDestroy()
    {
        LogUtil.GetInstance().WriteGame( "ResourceManager.OnDestroy" );
        m_LoadRequests.Clear();
        if ( Const.ResourceMode == "assetbundle" )
        {
            for ( int tmpi = 0; tmpi < m_AssetBundleNameList.Length; tmpi++ )
            {
                UnloadAssetBundleImmediately( m_AssetBundleNameList[tmpi] );
                LogUtil.GetInstance().WriteGame( "UnloadAssetBundle:" + m_AssetBundleNameList[tmpi] );
            }
        }
        else
        {
            Resources.UnloadUnusedAssets();
        }
        m_LoadedAssetBundles.Clear();
        m_Dependencies.Clear();
        m_AssetBundleNameList = null;
        m_resmap_sprite.Clear();
        m_resmap_sprite = null;
        m_resmap_prefab.Clear();
        m_resmap_prefab = null;
        m_resmap_object.Clear();
        m_resmap_object = null;
    }

    void InitResMap()
    {
        // sprite资源映射表
        m_resmap_sprite = new Dictionary<string, string[]>();
        TableUtil spriteTable = new TableUtil();
        spriteTable.OpenFromData( "resmap_sprite.txt" );
        for ( int row = 0; row < spriteTable.GetRecordsNum(); row++ )
        {
            string[] s = new string[3];
            s[0] = spriteTable.GetValue( row, 1 );
            s[1] = spriteTable.GetValue( row, 2 );
            s[2] = spriteTable.GetValue( row, 3 );
            m_resmap_sprite.Add( spriteTable.GetValue( row, 0 ), s );
        }

        // prefab资源映射表
        m_resmap_prefab = new Dictionary<string, string[]>();
        TableUtil prefabTable = new TableUtil();
        prefabTable.OpenFromData( "resmap_prefab.txt" );
        for ( int row = 0; row < prefabTable.GetRecordsNum(); row++ )
        {
            string[] s = new string[3];
            s[0] = prefabTable.GetValue( row, 1 );
            s[1] = prefabTable.GetValue( row, 2 );
            s[2] = prefabTable.GetValue( row, 3 );
            m_resmap_prefab.Add( prefabTable.GetValue( row, 0 ), s );
        }

        // object资源映射表
        m_resmap_object = new Dictionary<string, string[]>();
        TableUtil objectTable = new TableUtil();
        objectTable.OpenFromData( "resmap_object.txt" );
        for ( int row = 0; row < objectTable.GetRecordsNum(); row++ )
        {
            string[] s = new string[3];
            s[0] = objectTable.GetValue( row, 1 );
            s[1] = objectTable.GetValue( row, 2 );
            s[2] = objectTable.GetValue( row, 3 );
            m_resmap_object.Add( objectTable.GetValue( row, 0 ), s );
        }
    }

    public void Initialize( Action callback )
    {
        // 读取所有assetbundle名和依赖
        if ( Const.ResourceMode == "assetbundle" )
        {
            LoadAssetAsyn<AssetBundleManifest>( "StreamingAssets", "AssetBundleManifest", delegate ( UnityEngine.Object objs )
            {
                if ( objs )
                {
                    m_AssetBundleManifest = objs as AssetBundleManifest;
                    m_AssetBundleNameList = m_AssetBundleManifest.GetAllAssetBundles();
                    if ( callback != null )
                        callback();
                }
            } );
        }
        else
        {
            if ( callback != null )
                callback();
        }
    }

    /// <summary>
    /// 载入AssetBundle
    /// </summary>
    static public AssetBundleInfo LoadAssetBundle( string assetBundleName )
    {
        if ( Const.ResourceMode == "assetbundle" )
        {
            AssetBundleInfo bundle = GetLoadedAssetBundle( assetBundleName );
            if ( bundle == null )
            {
                // 先读取它的依赖
                LoadDependencies( assetBundleName );

                // 优先查询本地资源，本地没有就找包里资源
                string file = PathUtil.LocalPath() + assetBundleName;
                if ( File.Exists( file ) == false )
                {
                    file = PathUtil.PackagePath() + assetBundleName;
                }
                LogUtil.GetInstance().WriteGame( "LoadAssetBundle Path:" + file );

                // 非异步方法最快
                AssetBundle assetBundle = AssetBundle.LoadFromFile( file );
                if ( assetBundle == null )
                {
                    // 没读到暂时什么也不做
                    LogUtil.GetInstance().WriteGame( "LoadAssetBundle is null path:[" + file + "]" );
                }
                else
                {
                    // 读到了就添加缓存
                    m_LoadedAssetBundles.Add( assetBundleName, new AssetBundleInfo( assetBundle ) );
                }
            }
            return bundle;
        }
        return null;
    }

    /// <summary>
    /// 载入依赖
    /// </summary>
    static void LoadDependencies( string assetBundleName )
    {
        string[] dependencies = m_AssetBundleManifest.GetAllDependencies( assetBundleName );
        if ( dependencies.Length == 0 )
            return;

        for ( int i = 0; i < dependencies.Length; i++ )
        {
            dependencies[i] = RemapVariantName( dependencies[i] );
        }

        for ( int i = 0; i < dependencies.Length; i++ )
        {
            LoadAssetBundle( dependencies[i] );
        }
    }

    static string RemapVariantName( string assetBundleName )
    {
        string[] variants = { };
        string[] bundlesWithVariant = m_AssetBundleManifest.GetAllAssetBundlesWithVariant();
        if ( System.Array.IndexOf( bundlesWithVariant, assetBundleName ) < 0 )
            return assetBundleName;

        string[] split = assetBundleName.Split( '.' );

        int bestFit = int.MaxValue;
        int bestFitIndex = -1;

        for ( int i = 0; i < bundlesWithVariant.Length; i++ )
        {
            string[] curSplit = bundlesWithVariant[i].Split( '.' );
            if ( curSplit[0] != split[0] )
                continue;

            int found = System.Array.IndexOf( variants, curSplit[1] );
            if ( found != -1 && found < bestFit )
            {
                bestFit = found;
                bestFitIndex = i;
            }
        }
        if ( bestFitIndex != -1 )
            return bundlesWithVariant[bestFitIndex];
        else
            return assetBundleName;
    }

    /// <summary>
    /// 读取所有AssetBundle
    /// </summary>
    public void LoadAllAssetBundle()
    {
        if ( Const.ResourceMode == "assetbundle" )
        {
            for ( int tmpi = 0; tmpi < m_AssetBundleNameList.Length; tmpi++ )
            {
                string assetBundleName = m_AssetBundleNameList[tmpi];
                int progress = tmpi + 1;
                int totalProgress = m_AssetBundleNameList.Length;

                // 加载AssetBundle
                LoadAssetBundle( assetBundleName );

                // 设置进度
                object[] param = { assetBundleName, progress, totalProgress };
                LuaFun.loadAssetBundleProc.Call( param );
            }
        }
        else
        {
            object[] param = { "", 0, 0 };
            LuaFun.loadAssetBundleProc.Call( param );
        }
    }

    /// <summary>
    /// 检查已经读到的资源里是否有这个AssetBundle，有就直接拿
    /// </summary>
    static public AssetBundleInfo GetLoadedAssetBundle( string assetBundleName )
    {
        // 已经缓存的
        AssetBundleInfo bundle = null;
        m_LoadedAssetBundles.TryGetValue( assetBundleName, out bundle );
        if ( bundle == null )
            return null;
        bundle.m_ReferencedCount++;
        return bundle;
    }

    /// <summary>
    /// 读AssetBundle里的资源
    /// </summary>
    static private UnityEngine.Object Load( string assetBundleName, string assetName, System.Type type )
    {
        // 先检查缓存里有没有，有就拿缓存，没有读一遍assetbundle
        AssetBundleInfo bundleInfo = GetLoadedAssetBundle( assetBundleName );
        if ( bundleInfo != null && bundleInfo.m_AssetBundle != null )
        {
            UnityEngine.Object loadedobj = bundleInfo.m_AssetBundle.LoadAsset( assetName, type );
            return loadedobj;
        }

        // 加载AssetBundle
        bundleInfo = LoadAssetBundle( assetBundleName );
        if ( bundleInfo == null || bundleInfo.m_AssetBundle == null )
        {
            return null;
        }

        // 读取assetbundle里的这个文件
        UnityEngine.Object loadobj = bundleInfo.m_AssetBundle.LoadAsset( assetName, type );
        return loadobj;
    }

    /// <summary>
    /// 释放AssetBundle
    /// </summary>
    static public void UnloadAssetBundle( string assetBundleName )
    {
        AssetBundleInfo bundle = GetLoadedAssetBundle( assetBundleName );
        if ( bundle == null )
            return;

        if ( --bundle.m_ReferencedCount <= 0 )
        {
            if ( m_LoadRequests.ContainsKey( assetBundleName ) )
            {
                return;
            }
            bundle.m_AssetBundle.Unload( false );
            m_LoadedAssetBundles.Remove( assetBundleName );
        }
    }

    /// <summary>
    /// 立即释放AssetBundle
    /// </summary>
    static public void UnloadAssetBundleImmediately( string assetBundleName )
    {
        AssetBundleInfo bundle = GetLoadedAssetBundle( assetBundleName );
        if ( bundle == null )
            return;

        while ( true )
        {
            if ( m_LoadRequests.ContainsKey( assetBundleName ) )
            {
                // 如果当前AssetBundle处于Async Loading过程中，卸载会崩溃，所以等待loading完毕
                continue;
            }
            bundle.m_AssetBundle.Unload( false );
            m_LoadedAssetBundles.Remove( assetBundleName );
            break;
        }
    }

    /// <summary>
    /// 读取Sprite资源
    /// </summary>
    static public Sprite LoadSprite( string reskeyname )
    {
        if ( m_resmap_sprite == null || m_resmap_sprite.ContainsKey( reskeyname ) == false )
        {
            Debug.Log( reskeyname + " is not found" );
            LogUtil.GetInstance().WriteGame( reskeyname + " is not found" );
            return null;
        }

        if ( Const.ResourceMode == "assetbundle" )
        {
            return Load( m_resmap_sprite[reskeyname][1], m_resmap_sprite[reskeyname][2], typeof( Sprite ) ) as Sprite;
        }
        else
        {
#if UNITY_EDITOR
            return UnityEditor.AssetDatabase.LoadAssetAtPath<Sprite>( m_resmap_sprite[reskeyname][0] );
#else
            return Resources.Load<Sprite>( m_resmap_sprite[reskeyname][0] );
#endif
        }
    }

    /// <summary>
    /// 读取Prefab资源
    /// </summary>
    static public GameObject LoadPrefab( string reskeyname )
    {
        if ( m_resmap_prefab == null || m_resmap_prefab.ContainsKey( reskeyname ) == false )
        {
            Debug.Log( reskeyname + " is not found" );
            LogUtil.GetInstance().WriteGame( reskeyname + " is not found" );
            return null;
        }
        if ( Const.ResourceMode == "assetbundle" )
        {
            return Load( m_resmap_prefab[reskeyname][1], m_resmap_prefab[reskeyname][2], typeof( GameObject ) ) as GameObject;
        }
        else
        {
            return Resources.Load<GameObject>( m_resmap_prefab[reskeyname][0] );
        }
    }

    /// <summary>
    /// 读取Material资源
    /// </summary>
    static public Material LoadMaterial( string reskeyname )
    {
        return LoadObject( reskeyname ) as Material;
    }

    /// <summary>
    /// 读取Object
    /// </summary>
    static public UnityEngine.Object LoadObject( string reskeyname )
    {
        if ( m_resmap_object == null || m_resmap_object.ContainsKey( reskeyname ) == false )
        {
            Debug.Log( reskeyname + " is not found" );
            LogUtil.GetInstance().WriteGame( reskeyname + " is not found" );
            return null;
        }

        if ( Const.ResourceMode == "assetbundle" )
        {
            return Load( m_resmap_object[reskeyname][1], m_resmap_object[reskeyname][2], typeof( UnityEngine.Object ) ) as UnityEngine.Object;
        }
        else
        {
            return Resources.Load( m_resmap_object[reskeyname][0] );
        }
    }

    static public T LoadAny<T>( String reskeyname ) where T : UnityEngine.Object
    {
#if UNITY_EDITOR
        if ( m_resmap_object == null )
        {
            // object资源映射表
            m_resmap_object = new Dictionary<string, string[]>();
            TableUtil objectTable = new TableUtil();
            objectTable.OpenFromData( "resmap_object.txt" );
            for ( int row = 0; row < objectTable.GetRecordsNum(); row++ )
            {
                string[] s = new string[3];
                s[0] = objectTable.GetValue( row, 1 );
                s[1] = objectTable.GetValue( row, 2 );
                s[2] = objectTable.GetValue( row, 3 );
                m_resmap_object.Add( objectTable.GetValue( row, 0 ), s );
            }
        }
#endif
        if ( m_resmap_object == null || m_resmap_object.ContainsKey( reskeyname ) == false )
        {
            Debug.Log( reskeyname + " is not found" );
            LogUtil.GetInstance().WriteGame( reskeyname + " is not found" );
            return default( T );
        }

        if ( Const.ResourceMode == "assetbundle" )
        {
            return (T)Load( m_resmap_object[reskeyname][1], m_resmap_object[reskeyname][2], typeof( T ) ) as T;
        }
        else
        {
            return Resources.Load<T>( m_resmap_object[reskeyname][0] );
        }
    }

    /// <summary>
    /// 释放Sprite
    /// </summary>
    static public void UnloadSprite( string reskeyname )
    {
        if ( m_resmap_sprite == null || m_resmap_sprite[reskeyname] == null )
        {
            Debug.Log( reskeyname + " is not UnloadSprite" );
            LogUtil.GetInstance().WriteGame( reskeyname + " is not UnloadSprite" );
            return;
        }

        if ( Const.ResourceMode == "assetbundle" )
        {
            UnloadAssetBundle( m_resmap_sprite[reskeyname][1] );
        }
        else
        {

        }
    }

    /// <summary>
    /// 释放Prefab
    /// </summary>
    static public void UnloadPrefab( string reskeyname )
    {
        if ( m_resmap_prefab == null || m_resmap_prefab[reskeyname] == null )
        {
            Debug.Log( reskeyname + " is not UnloadPrefab" );
            LogUtil.GetInstance().WriteGame( reskeyname + " is not UnloadPrefab" );
            return;
        }

        if ( Const.ResourceMode == "assetbundle" )
        {
            UnloadAssetBundle( m_resmap_prefab[reskeyname][1] );
        }
        else
        {

        }
    }


    static public void UnloadObject( string reskeyname )
    {
        if ( m_resmap_object == null || m_resmap_object[reskeyname] == null )
        {
            Debug.Log( reskeyname + " is not UnloadSprite" );
            LogUtil.GetInstance().WriteGame( reskeyname + " is not UnloadSprite" );
            return;
        }

        if ( Const.ResourceMode == "assetbundle" )
        {
            UnloadAssetBundle( m_resmap_object[reskeyname][1] );
        }
        else
        {

        }
    }

    /// <summary>
    /// 异步载入素材
    /// </summary>
    void LoadAssetAsyn<T>( string abName, string assetNames, Action<UnityEngine.Object> action = null, LuaFunction func = null ) where T : UnityEngine.Object
    {
        abName = GetRealAssetPath( abName );

        LoadAssetRequest request = new LoadAssetRequest();
        request.assetType = typeof( T );
        request.assetNames = assetNames;
        request.luaFunc = func;
        request.sharpFunc = action;

        List<LoadAssetRequest> request_list = null;
        if ( !m_LoadRequests.TryGetValue( abName, out request_list ) )
        {
            request_list = new List<LoadAssetRequest>();
            request_list.Add( request );
            m_LoadRequests.Add( abName, request_list );
            StartCoroutine( OnLoadAsset<T>( abName ) );
        }
        else
        {
            request_list.Add( request );
        }
    }

    /// <summary>
    /// 异步载入素材协程
    /// </summary>
    IEnumerator OnLoadAsset<T>( string abName ) where T : UnityEngine.Object
    {
        AssetBundleInfo bundleInfo = GetLoadedAssetBundle( abName );
        if ( bundleInfo == null )
        {
            yield return StartCoroutine( OnLoadAssetBundle( abName, typeof( T ) ) );

            bundleInfo = GetLoadedAssetBundle( abName );
            if ( bundleInfo == null )
            {
                m_LoadRequests.Remove( abName );
                Debug.LogError( "OnLoadAsset--->>>" + abName );
                yield break;
            }
        }
        List<LoadAssetRequest> list = null;
        if ( !m_LoadRequests.TryGetValue( abName, out list ) )
        {
            m_LoadRequests.Remove( abName );
            yield break;
        }
        for ( int i = 0; i < list.Count; i++ )
        {
            string assetNames = list[i].assetNames;
            AssetBundleRequest request = bundleInfo.m_AssetBundle.LoadAssetAsync( assetNames, list[i].assetType );
            yield return request;

            if ( list[i].sharpFunc != null )
            {
                list[i].sharpFunc( request.asset );
                list[i].sharpFunc = null;
            }
            if ( list[i].luaFunc != null )
            {
                list[i].luaFunc.Call( request.asset );
                list[i].luaFunc.Dispose();
                list[i].luaFunc = null;
            }
            bundleInfo.m_ReferencedCount++;
        }
        m_LoadRequests.Remove( abName );
    }

    IEnumerator OnLoadAssetBundle( string assetBundleName, Type type )
    {
        // 优先查询本地资源，本地没有就找包里资源
        string file = PathUtil.LocalPath() + assetBundleName;
        if ( File.Exists( file ) == false )
        {
            file = PathUtil.PackagePath() + assetBundleName;
        }
        // 异步读取
        AssetBundleCreateRequest request;

        if ( type == typeof( AssetBundleManifest ) )
        {
            request = AssetBundle.LoadFromFileAsync( file );
        }
        else
        {
            string[] dependencies = m_AssetBundleManifest.GetAllDependencies( assetBundleName );
            if ( dependencies.Length > 0 )
            {
                m_Dependencies.Add( assetBundleName, dependencies );
                for ( int i = 0; i < dependencies.Length; i++ )
                {
                    string depName = dependencies[i];
                    AssetBundleInfo bundleInfo = null;
                    if ( m_LoadedAssetBundles.TryGetValue( depName, out bundleInfo ) )
                    {
                        bundleInfo.m_ReferencedCount++;
                    }
                    else if ( !m_LoadRequests.ContainsKey( depName ) )
                    {
                        yield return StartCoroutine( OnLoadAssetBundle( depName, type ) );
                    }
                }
            }
            request = AssetBundle.LoadFromFileAsync( assetBundleName );
        }
        yield return request;

        AssetBundle assetObj = request.assetBundle;
        if ( assetObj != null )
        {
            m_LoadedAssetBundles.Add( assetBundleName, new AssetBundleInfo( assetObj ) );
        }
    }

    string GetRealAssetPath( string abName )
    {
        if ( abName.Equals( "StreamingAssets" ) )
        {
            return abName;
        }
        abName = abName.ToLower();
        if ( abName.Contains( "/" ) )
        {
            return abName;
        }
        for ( int i = 0; i < m_AssetBundleNameList.Length; i++ )
        {
            int index = m_AssetBundleNameList[i].LastIndexOf( '/' );
            string path = m_AssetBundleNameList[i].Remove( 0, index + 1 );
            if ( path.Equals( abName ) )
            {
                return m_AssetBundleNameList[i];
            }
        }
        Debug.LogError( "GetRealAssetPath Error:>>" + abName );
        return null;
    }

    /// <summary>
    /// 非AssetBundle模式使用
    /// </summary>
    IEnumerator OnResourceLoad<T>( string path, Action<UnityEngine.Object> action = null, LuaFunction func = null ) where T : UnityEngine.Object
    {
        ResourceRequest resourceRequest = Resources.LoadAsync<GameObject>( path );
        while ( !resourceRequest.isDone )
        {
            yield return 0;
        }

        if ( action != null )
        {
            action( resourceRequest.asset );
        }
        if ( func != null )
        {
            func.Call( resourceRequest.asset );
            func.Dispose();
        }
    }

    /// <summary>
    /// 异步读取Sprite
    /// </summary>
    public void LoadSpriteAsyn( string reskeyname, Action<UnityEngine.Object> func )
    {
        if ( m_resmap_sprite == null || m_resmap_sprite.ContainsKey( reskeyname ) == false )
        {
            Debug.Log( reskeyname + " is not found" );
            LogUtil.GetInstance().WriteGame( reskeyname + " is not found" );
            return;
        }

        if ( Const.ResourceMode == "assetbundle" )
        {
            LoadAssetAsyn<Sprite>( m_resmap_sprite[reskeyname][1], m_resmap_sprite[reskeyname][2], func );
        }
        else
        {
            StartCoroutine( OnResourceLoad<Sprite>( m_resmap_sprite[reskeyname][0] ) );
        }
    }

    /// <summary>
    /// 异步读取Sprite
    /// </summary>
    public void LoadSpriteAsyn( string reskeyname, LuaFunction func )
    {
        if ( m_resmap_sprite == null || m_resmap_sprite.ContainsKey( reskeyname ) == false )
        {
            Debug.Log( reskeyname + " is not found" );
            LogUtil.GetInstance().WriteGame( reskeyname + " is not found" );
            return;
        }

        if ( Const.ResourceMode == "assetbundle" )
        {
            LoadAssetAsyn<Sprite>( m_resmap_sprite[reskeyname][1], m_resmap_sprite[reskeyname][2], null, func );
        }
        else
        {
            StartCoroutine( OnResourceLoad<Sprite>( m_resmap_sprite[reskeyname][0], null, func ) );
        }
    }

    /// <summary>
    /// 异步读取Prefab
    /// </summary>
    public void LoadPrefabAsyn( string reskeyname, Action<UnityEngine.Object> func )
    {
        if ( m_resmap_prefab == null || m_resmap_prefab.ContainsKey( reskeyname ) == false )
        {
            Debug.Log( reskeyname + " is not found" );
            LogUtil.GetInstance().WriteGame( reskeyname + " is not found" );
            return;
        }

        if ( Const.ResourceMode == "assetbundle" )
        {
            LoadAssetAsyn<GameObject>( m_resmap_prefab[reskeyname][1], m_resmap_prefab[reskeyname][2], func );
        }
        else
        {
            StartCoroutine( OnResourceLoad<GameObject>( m_resmap_prefab[reskeyname][0] ) );
        }
    }

    /// <summary>
    /// 异步读取Prefab
    /// </summary>
    public void LoadPrefabAsyn( string reskeyname, LuaFunction func )
    {
        if ( m_resmap_prefab == null || m_resmap_prefab.ContainsKey( reskeyname ) == false )
        {
            Debug.Log( reskeyname + " is not found" );
            LogUtil.GetInstance().WriteGame( reskeyname + " is not found" );
            return;
        }

        if ( Const.ResourceMode == "assetbundle" )
        {
            LoadAssetAsyn<GameObject>( m_resmap_prefab[reskeyname][1], m_resmap_prefab[reskeyname][2], null, func );
        }
        else
        {
            StartCoroutine( OnResourceLoad<GameObject>( m_resmap_prefab[reskeyname][0], null, func ) );
        }
    }
}
