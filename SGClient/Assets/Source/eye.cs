using UnityEngine;
using System.Collections;
using System.Text;
using LuaInterface;

/// <summary>
/// 可通过该类取到所有管理器
/// </summary>
public class eye {
    /// <summary>
    /// 游戏管理器对象，包含了所有管理器
    /// </summary>
    private static GameObject _manager = null;
    public static GameObject manager {
        get { 
            if (_manager == null) 
                _manager = GameObject.FindWithTag("GameManager");
            return _manager;
        }
    }

    /// <summary>
    /// 游戏管理器
    /// </summary>
    private static GameManager _gameManager = null;
    public static GameManager gameManager {
        get { 
            if (_gameManager == null)
                _gameManager = manager.GetComponent<GameManager>();
            return _gameManager;
        }
    }

    /// <summary>
    /// 脚本管理器
    /// </summary>
    private static LuaManager _luaManager = null;
    public static LuaManager luaManager
    {
        get
        {
            if ( _luaManager == null )
                _luaManager = manager.GetComponent<LuaManager>();
            return _luaManager;
        }
    }

    /// <summary>
    /// 网络管理器
    /// </summary>
    private static NetworkManager _networkManager = null;
    public static NetworkManager networkManager
    {
        get
        {
            if ( _networkManager == null )
                _networkManager = manager.GetComponent<NetworkManager>();
            return _networkManager;
        }
    }

    /// <summary>
    /// UI管理器
    /// </summary>
    private static UIManager _uiManager = null;
    public static UIManager uiManager
    {
        get
        {
            if ( _uiManager == null && manager != null )
                _uiManager = manager.GetComponent<UIManager>();
            return _uiManager;
        }
    }

    ///// <summary>
    ///// 音效管理器
    ///// </summary>
    //private static AudioManager _audioManager = null;
    //public static AudioManager audioManager {
    //	get {
    //		if ( _audioManager == null && manager != null )
    //			_audioManager = manager.GetComponent<AudioManager>();
    //		return _audioManager;
    //	}
    //}


    /// <summary>
    /// 资源管理器
    /// </summary>
    private static ResourceManager _resourceManager = null;
    public static ResourceManager resourceManager
    {
        get
        {
            if ( _resourceManager == null )
                _resourceManager = manager.GetComponent<ResourceManager>();
            return _resourceManager;
        }
    }

    /// <summary>
    /// 对象池管理器
    /// </summary>
    private static ObjectPoolManager _objectPoolManager = null;
    public static ObjectPoolManager objectPoolManager
    {
        get
        {
            if ( _objectPoolManager == null )
                _objectPoolManager = manager.GetComponent<ObjectPoolManager>();
            return _objectPoolManager;
        }
    }

    ///// <summary>
    ///// 本地推送管理器
    ///// </summary>
    //private static NativeNotificationManager _nativeNotificationManager = null;
    //public static NativeNotificationManager nativeNotificationManager
    //{
    //    get
    //    {
    //        if ( _nativeNotificationManager == null )
    //            _nativeNotificationManager = manager.GetComponent<NativeNotificationManager>();
    //        return _nativeNotificationManager;
    //    }
    //}

    /// <summary>
    /// 服务器时间
    /// </summary>
    private static LuaFunction _getServerTime = null;
	public static int serverTime
	{
		get
		{
			if ( _getServerTime == null )
				_getServerTime = eye.luaManager.GetFunction( "GetServerTime" );
			object[] info = _getServerTime.Call( "" );
			return System.Convert.ToInt32( info[0] );
		}
	}

    public static void Destroy()
    {
        _manager = null;
        _gameManager = null;
        _luaManager = null;
        _networkManager = null;
        _uiManager = null;
        _resourceManager = null;
        _objectPoolManager = null;
        //_scenceManager = null;
        //_nativeNotificationManager = null;
        //_voiceManager = null;
        _getServerTime = null;
    }
}

