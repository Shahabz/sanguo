using System.Collections;
using LuaInterface;

// 除UIMod的lua函数，集中在此
public class LuaFun
{	
    public static object[] Call( string funname, params object[] args )
    { 
        return eye.luaManager.GetFunction(funname).Call( args );
    }

	/// <summary>
	/// 资源数字变换
	/// </summary>
	private static LuaFunction _numFormat = null;
	public static LuaFunction numFormat
	{
		get
		{
			if ( _numFormat == null )
				_numFormat = eye.luaManager.GetFunction( "string.formatnumberthousands" );
			
			return _numFormat;
		}
	}	
	
	/// <summary>
	/// 时间数字变换
	/// </summary>
	private static LuaFunction _timeFormat = null;
	public static LuaFunction timeFormat
	{
		get
		{
			if ( _timeFormat == null )
				_timeFormat = eye.luaManager.GetFunction( "GetTimeFomat" );
			
			return _timeFormat;
		}
	}
	
    /// <summary>
    /// 逻辑包分发
    /// </summary>
    private static LuaFunction _onsocket = null;
    public static LuaFunction OnSocket
    {
        get
        {
            if ( _onsocket == null )
                _onsocket = eye.luaManager.GetFunction( "Network.OnSocket" );

            return _onsocket;
        }
    }

    /// <summary>
    /// 游戏开始
    /// </summary>
    private static LuaFunction _gameStart = null;
    public static LuaFunction gameStart
    {
        get
        {
            if ( _gameStart == null )
                _gameStart = eye.luaManager.GetFunction( "GameManager.OnStart" );

            return _gameStart;
        }
    }

    /// <summary>
    /// 游戏逻辑
    /// </summary>
    private static LuaFunction _gameLogic = null;
	public static LuaFunction gameLogic
	{
		get
		{
			if ( _gameLogic == null )
				_gameLogic = eye.luaManager.GetFunction( "GameManager.GameLogic" );
			
			return _gameLogic;
		}
	}

	/// <summary>
	/// 游戏暂停
	/// </summary>
	private static LuaFunction _gamePause = null;
	public static LuaFunction gamePause
	{
		get
		{
			if ( _gamePause == null )
				_gamePause = eye.luaManager.GetFunction( "GameManager.OnApplicationPause" );
			
			return _gamePause;
		}
	}

	/// <summary>
	/// 摇一摇
	/// </summary>
	private static LuaFunction _gameWave = null;
	public static LuaFunction gameWave
	{
		get
		{
			if ( _gameWave == null )
				_gameWave = eye.luaManager.GetFunction( "GameManager.GameWave" );
			
			return _gameWave;
		}
	}

    /// <summary>
    /// 所有UI关闭
    /// </summary>
    private static LuaFunction _allUIClose = null;
    public static LuaFunction allUIClose
    {
        get
        {
            if ( _allUIClose == null )
                _allUIClose = eye.luaManager.GetFunction( "GameManager.AllUIClose" );

            return _allUIClose;
        }
    }

    /// <summary>
    /// 登出
    /// </summary>
    private static LuaFunction _gameLogout = null;
    public static LuaFunction gameLogout
    {
        get
        {
            if ( _gameLogout == null )
                _gameLogout = eye.luaManager.GetFunction( "GameManager.Logout" );

            return _gameLogout;
        }
    }

    /// <summary>
    /// 海外推送标示
    /// </summary>
    private static LuaFunction _pushKey = null;
    public static LuaFunction pushKey
    {
        get
        {
            if ( _pushKey == null )
                _pushKey = eye.luaManager.GetFunction( "GameManager.PushKey" );

            return _pushKey;
        }
    }

    /// <summary>
    /// 选择一个建筑
    /// </summary>
    private static LuaFunction _buildingselect = null;
    public static LuaFunction buildingselect
    {
        get
        {
            if ( _buildingselect == null )
                _buildingselect = eye.luaManager.GetFunction( "MainCity.BuildingSelect" );

            return _buildingselect;
        }
    }

    /// <summary>
	/// 加载资源
	/// </summary>
	private static LuaFunction _loadAssetBundleProc = null;
	public static LuaFunction loadAssetBundleProc
	{
		get
		{
			if ( _loadAssetBundleProc == null )
				_loadAssetBundleProc = eye.luaManager.GetFunction( "LoginModOnLoadAssetBundleProc" );
			
			return _loadAssetBundleProc;
		}
	}
    
    /// <summary>
    /// 上传头像回调
    /// </summary>
    private static LuaFunction _photoCustomHead = null;
    public static LuaFunction photoCustomHead
    {
        get
        {
            if ( _photoCustomHead == null )
                _photoCustomHead = eye.luaManager.GetFunction( "photoCustomHead" );

            return _photoCustomHead;
        }
    }

    /// <summary>
    /// 战斗帧逻辑
    /// </summary>
    private static LuaFunction _fightFrameTurn = null;
    public static LuaFunction fightFrameTurn
    {
        get
        {
            if ( _fightFrameTurn == null )
                _fightFrameTurn = eye.luaManager.GetFunction( "FightFrameTurn" );

            return _fightFrameTurn;
        }
    }

    public static void Destroy()
    {
        _numFormat = null;
        _timeFormat = null;
        _onsocket = null;
        _loadAssetBundleProc = null;

        _gameStart = null;
        _gameLogic = null;
        _gamePause = null;
        _gameWave = null;
        _gameLogout = null;
        _pushKey = null;

        _photoCustomHead = null;

        _allUIClose = null;
        _buildingselect = null;
        _fightFrameTurn = null;
    }
}
