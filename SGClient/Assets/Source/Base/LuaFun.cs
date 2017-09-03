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
                _buildingselect = eye.luaManager.GetFunction( "City.BuildingSelect" );

            return _buildingselect;
        }
    }

	/// <summary>
	/// 选择一个建筑地块
	/// </summary>
	private static LuaFunction _buildinglandselect = null;
	public static LuaFunction buildinglandselect
	{
		get
		{
			if ( _buildinglandselect == null )
				_buildinglandselect = eye.luaManager.GetFunction( "City.BuildingLandSelect" );

			return _buildinglandselect;
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

	/// <summary>
	/// 世界地图初始化
	/// </summary>
	private static LuaFunction _worldMapStart = null;
	public static LuaFunction worldMapStart
	{
		get
		{
			if ( _worldMapStart == null )
				_worldMapStart = eye.luaManager.GetFunction( "WorldMap.Start" );

			return _worldMapStart;
		}
	}

	/// <summary>
	/// 世界地图帧逻辑
	/// </summary>
	private static LuaFunction _worldMapUpdate = null;
	public static LuaFunction worldMapUpdate
	{
		get
		{
			if ( _worldMapUpdate == null )
				_worldMapUpdate = eye.luaManager.GetFunction( "WorldMap.Update" );

			return _worldMapUpdate;
		}
	}

	/// <summary>
	/// 世界地图
	/// </summary>
	private static LuaFunction _worldMapViewChangeSec = null;
	public static LuaFunction worldMapViewChangeSec
	{
		get
		{
			if ( _worldMapViewChangeSec == null )
				_worldMapViewChangeSec = eye.luaManager.GetFunction( "WorldMap.ViewChangeSec" );

			return _worldMapViewChangeSec;
		}
	}

	/// <summary>
	/// 世界地图
	/// </summary>
	private static LuaFunction _worldMapViewChangeFrame = null;
	public static LuaFunction worldMapViewChangeFrame
	{
		get
		{
			if ( _worldMapViewChangeFrame == null )
				_worldMapViewChangeFrame = eye.luaManager.GetFunction( "WorldMap.ViewChangeFrame" );

			return _worldMapViewChangeFrame;
		}
	}

	/// <summary>
	/// 世界地图
	/// </summary>
	private static LuaFunction _worldMapOnClick = null;
	public static LuaFunction worldMapOnClick
	{
		get
		{
			if ( _worldMapOnClick == null )
				_worldMapOnClick = eye.luaManager.GetFunction( "WorldMap.OnClick" );

			return _worldMapOnClick;
		}
	}

	/// <summary>
	/// 世界地图
	/// </summary>
	private static LuaFunction _worldMapOnDrag = null;
	public static LuaFunction worldMapOnDrag
	{
		get
		{
			if ( _worldMapOnDrag == null )
				_worldMapOnDrag = eye.luaManager.GetFunction( "WorldMap.OnDrag" );

			return _worldMapOnDrag;
		}
	}

	/// <summary>
	/// 世界地图缩略图
	/// </summary>
	private static LuaFunction _worldMapThumbStart = null;
	public static LuaFunction worldMapThumbStart
	{
		get
		{
			if ( _worldMapThumbStart == null )
				_worldMapThumbStart = eye.luaManager.GetFunction( "WorldMapThumb.Start" );

			return _worldMapThumbStart;
		}
	}

	/// <summary>
	/// 世界地图缩略图
	/// </summary>
	private static LuaFunction _worldMapThumbOnClick = null;
	public static LuaFunction worldMapThumbOnClick
	{
		get
		{
			if ( _worldMapThumbOnClick == null )
				_worldMapThumbOnClick = eye.luaManager.GetFunction( "WorldMapThumb.OnClick" );

			return _worldMapThumbOnClick;
		}
	}

	/// <summary>
	/// 世界地图缩略图
	/// </summary>
	private static LuaFunction _worldMapThumbOnDrag = null;
	public static LuaFunction worldMapThumbOnDrag
	{
		get
		{
			if ( _worldMapThumbOnDrag == null )
				_worldMapThumbOnDrag = eye.luaManager.GetFunction( "WorldMapThumb.OnDrag" );

			return _worldMapThumbOnDrag;
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
		_buildinglandselect = null;
        _fightFrameTurn = null;

		_worldMapStart = null;
		_worldMapUpdate = null;
		_worldMapViewChangeSec = null;
		_worldMapViewChangeFrame = null;
		_worldMapOnClick = null;
		_worldMapOnDrag = null;
		_worldMapThumbStart = null;
		_worldMapThumbOnClick = null;
		_worldMapThumbOnDrag = null;
    }
}
