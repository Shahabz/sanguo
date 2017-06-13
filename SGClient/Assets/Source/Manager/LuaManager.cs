using UnityEngine;
using System.Collections;
using System.IO;
using LuaInterface;

/// <summary>
/// 集成自LuaFileUtils，重写里面的ReadFile，
/// </summary>
public class LuaLoader : LuaFileUtils
{
    // Use this for initialization
    public LuaLoader()
    {
        instance = this;
    }

    /// <summary>
    /// 当LuaVM加载Lua文件的时候，这里就会被调用，
    /// 用户可以自定义加载行为，只要返回byte[]即可。
    /// </summary>
    /// <param name="fileName"></param>
    /// <returns></returns>
    public override byte[] ReadFile( string fileName )
    {
        return base.ReadFile( fileName );
    }
}


public class LuaManager : MonoBehaviour
{
    private LuaState lua;
    private LuaLoader loader;
    private LuaLooper loop = null;

    // Use this for initialization
    void Awake()
    {
        loader = new LuaLoader();
        lua = new LuaState();
        this.OpenLibs();
        lua.LuaSetTop( 0 );

        LuaBinder.Bind( lua );
        LuaCoroutine.Register( lua, this );
    }

    public void InitStart()
    {
        InitLuaPath();
        this.lua.Start();    //启动LUAVM
        this.StartMain();
        this.StartLooper();
    }

    void StartLooper()
    {
        loop = gameObject.AddComponent<LuaLooper>();
        loop.luaState = lua;
    }

    //cjson 比较特殊，只new了一个table，没有注册库，这里注册一下
    protected void OpenCJson()
    {
        lua.LuaGetField( LuaIndexes.LUA_REGISTRYINDEX, "_LOADED" );
        lua.OpenLibs( LuaDLL.luaopen_cjson );
        lua.LuaSetField( -2, "cjson" );

        lua.OpenLibs( LuaDLL.luaopen_cjson_safe );
        lua.LuaSetField( -2, "cjson.safe" );
    }

    void StartMain()
    {
        lua.DoFile( "Main.lua" );

        LuaFunction main = lua.GetFunction( "Main" );
        main.Call();
        main.Dispose();
        main = null;
    }

    /// <summary>
    /// 初始化加载第三方库
    /// </summary>
    void OpenLibs()
    {
        lua.OpenLibs( LuaDLL.luaopen_pb );
        lua.OpenLibs( LuaDLL.luaopen_lpeg );
        lua.OpenLibs( LuaDLL.luaopen_bit );
        lua.OpenLibs( LuaDLL.luaopen_socket_core );

        this.OpenCJson();
    }

    /// <summary>
    /// 初始化Lua代码加载路径
    /// </summary>
    void InitLuaPath()
    {
        //lua.AddSearchPath( PathUtil.LocalLuaPath() );
        //lua.AddSearchPath( PathUtil.LocalLuaPath()+"tolua/" );
    }

    public object[] DoFile( string filename )
    {
        return lua.DoFile( filename );
    }

    // Update is called once per frame
    public object[] CallFunction( string funcName, params object[] args )
    {
        LuaFunction func = lua.GetFunction( funcName );
        if ( func != null )
        {
            return func.Call( args );
        }
        return null;
    }

    public LuaFunction GetFunction( string funcName )
    {
        return lua.GetFunction( funcName );
    }

    public void LuaGC()
    {
        lua.LuaGC( LuaGCOptions.LUA_GCCOLLECT );
    }

    public void Close()
    {
        loop.Destroy();
        loop = null;

        lua.Dispose();
        lua = null;
        loader = null;
    }
}
