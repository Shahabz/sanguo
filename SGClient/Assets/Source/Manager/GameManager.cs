using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;
using LuaInterface;
using System.Reflection;
using System.IO;
using System.Diagnostics;
using System.Net;
using System.Text;

public class GameManager : MonoBehaviour {
    // 配置文件名称
    private string m_szAppTxt = "app.txt";
    // 资源版本文件
    private string m_szResverTxt = "resver.txt";
    // 解压到本地的配置文件
    private IniUtil m_pLocalAppIni = new IniUtil();
    private IniUtil m_pLocalResverIni = new IniUtil();
    public static bool isInBackground = false;

    /// <summary>
    /// 初始化游戏管理器
    /// </summary>
    void Awake () 
    {
        Screen.sleepTimeout = SleepTimeout.NeverSleep;
        Application.targetFrameRate = Const.GameFrameRate;

        // 创建日志系统
        LogUtil.GetInstance().Create( PathUtil.LogPath() );
        LogUtil.GetInstance().WriteGame( "------------------ Game Log Begin ------------------" );
        LogUtil.GetInstance().WriteNet( "------------------ Net Log Begin ------------------" );

        // 本地化系统初始化，再读一遍，因为文字表有更新
        if ( Localization.init() < 0 )
        {
            LogUtil.GetInstance().WriteGame( "Localization Init Failed" );
            return;
        }
        LogUtil.GetInstance().WriteGame( "STEP 1" );
        // 再次读取本地程序配置文件,因为编辑器模式下，启动场景不使用
        if ( m_pLocalAppIni.OpenFromData( m_szAppTxt ) == false )
            LogUtil.GetInstance().WriteGame( m_szAppTxt + " Not Found!!!" );
        else
            m_pLocalAppIni.SetGlobal();
        LogUtil.GetInstance().WriteGame( "STEP 2" );

        // 再次读取本地资源版本文件,因为编辑器模式下，启动场景不使用
        if ( m_pLocalResverIni.OpenFromTXT( PathUtil.ConfigPath() + m_szResverTxt ) == false )
            LogUtil.GetInstance().WriteGame( PathUtil.ConfigPath() + m_szResverTxt + " Not Found" );
        else
            m_pLocalResverIni.SetGlobal();
        LogUtil.GetInstance().WriteGame( "STEP 3" );

        Utils.Add<ObjectPoolManager>( gameObject );	// 添加对象池管理器
        LogUtil.GetInstance().WriteGame( "Add ObjectPoolManager" );
        Utils.Add<ResourceManager>( gameObject );	// 添加资源管理器组件
        LogUtil.GetInstance().WriteGame( "Add ResourceManager" );
        Utils.Add<SocketClient>( gameObject );	// 添加socket组件
        LogUtil.GetInstance().WriteGame( "Add SocketClient" );
        Utils.Add<NetworkManager>( gameObject );	// 添加网络消息管理器组件
        LogUtil.GetInstance().WriteGame( "Add NetworkManager" );
        Utils.Add<UIManager>( gameObject );		    // 添加UI管理器
        LogUtil.GetInstance().WriteGame( "Add UIManager" );
        Utils.Add<LuaManager>( gameObject );		// 添加LUA管理器
        LogUtil.GetInstance().WriteGame( "Add LuaManager" );

        // 初始化LUA
        LuaInited();
        LogUtil.GetInstance().WriteGame( "LuaInited" );

        // 初始化资源管理
        eye.resourceManager.Initialize( delegate ()
        {
            // 先读取部分AssetBundle
            ResourceManager.LoadAssetBundle( "ui_dialog_login" );

            // 游戏开始
            LuaFun.gameStart.Call();

            // 启动游戏主逻辑, 每分钟调用
            StartCoroutine( GameLogic() );
        } );
    }

    /// <summary>
    /// 初始化Lua
    /// </summary>
    void LuaInited() 
    {
        eye.luaManager.InitStart();
        eye.luaManager.DoFile( "Include.lua" );
    }

    /// <summary>
    /// 析构函数
    /// </summary>
    void OnDestroy() 
    {
        // uluaMgr.lua.GetFunction( "GameManager.OnDestroy" ).Call();
        LuaFun.Destroy();
		LogUtil.GetInstance().WriteGame( "------------------ Game Log End ------------------" );
	    LogUtil.GetInstance().WriteNet( "------------------ Net Log End ------------------" );
    }
    /// <summary>
    /// 每帧遍历逻辑层消息队列
    /// </summary>
    void Update()
    {
        //RecUtil.GetInstance().FrameUpdate();
        if ( Input.GetKeyDown(KeyCode.Escape) )
        {
            //uluaMgr.lua.GetFunction( "GameManager.OnAskQuit" ).Call();
        }
    }

    IEnumerator GameLogic()
    {
        while( true )
        {
            yield return new WaitForSeconds( 60 );
            LuaFun.gameLogic.Call();
        }
    }

    void OnApplicationPause( bool paused )
    {
        //if ( paused )
        //{
        //    GameManager.isInBackground = true;
        //}
        //else
        //{
        //    GameManager.isInBackground = false;
        //    setDesignContentScale();
        //}
        //object[] param = { paused };
        //LuaFun.gamePause.Call( param );
    }

    private static int scaleWidth = 0;
    private static int scaleHeight = 0;
    public static void setDesignContentScale()
    {
#if UNITY_ANDROID
        if ( scaleWidth == 0 && scaleHeight == 0 )
        {
            int width = Screen.currentResolution.width;
            int height = Screen.currentResolution.height;
            int designWidth = 1334;
            int designHeight = 750;
            float s1 = (float)designWidth / (float)designHeight;
            float s2 = (float)width / (float)height;
            if ( s1 < s2 )
            {
                designWidth = (int)Mathf.FloorToInt( designHeight * s2 );
            }
            else if ( s1 > s2 )
            {
                designHeight = (int)Mathf.FloorToInt( designWidth / s2 );
            }
            float contentScale = (float)designWidth / (float)width;
            if ( contentScale < 1.0f )
            {
                scaleWidth = designWidth;
                scaleHeight = designHeight;
            }
        }
        if ( scaleWidth > 0 && scaleHeight > 0 )
        {
            if ( scaleWidth % 2 == 0 )
            {
                scaleWidth += 1;
            }
            else
            {
                scaleWidth -= 1;
            }
            Screen.SetResolution( scaleWidth, scaleHeight, true );
        }
#endif
    }

    /*********************************************************/
    // 延时执行
    /*********************************************************/
    public delegate void LuaExecute( object param );
    public LuaExecute delayExecute = null;
    Dictionary<string, Coroutine> _InvokeList = new Dictionary<string, Coroutine>();
    public void GameInvoke( float delay, object param, string name )
    {
        if ( delayExecute == null )
            return;

        if ( name != "" )
        {
            if ( _InvokeList.ContainsKey( name ) )
            {
                StopCoroutine( _InvokeList[name] );
                _InvokeList.Remove( name );
            }
            _InvokeList.Add( name, StartCoroutine( Execute( delayExecute, delay, param, name ) ) );
        }
        else
            StartCoroutine( Execute( delayExecute, delay, param, name ) );

        delayExecute = null;
    }
    public void GameInvoke_Stop( string name )
    {
        if ( !_InvokeList.ContainsKey( name ) )
            return;

        StopCoroutine( _InvokeList[name] );
        _InvokeList.Remove( name );
        delayExecute = null;
    }
    IEnumerator Execute( LuaExecute func, float delay, object param, string name )
    {
        yield return new WaitForSeconds( delay );
        if ( name != "" )
            _InvokeList.Remove( name );
        func( param );
        yield return null;
    }
}
