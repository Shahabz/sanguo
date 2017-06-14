using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;

// 应用程序全局变量
public class Const {   
    public static bool DebugMode = true;                        // 调试模式-用于内部测试
	static string _ResourceMode = "";
	public static string ResourceMode
	{
		set{
			_ResourceMode = "";
		}
		get{
#if UNITY_EDITOR
			string signfilePath = Application.dataPath + "/" + ".develop.mode";
			if (File.Exists (signfilePath))
			{
				_ResourceMode = "resource";
			}
			else
			{
				_ResourceMode = "assetbundle";
			}
#elif UNITY_STANDALONE
			_ResourceMode = "resource";
#else
			_ResourceMode = "assetbundle";
#endif
			return _ResourceMode;
		}
	}

    public static int TimerInterval = 1;
    public static int GameFrameRate = 30;                       // 游戏帧频

    public static TextAsset[] luaScripts;                       // Lua公共脚本
    public static bool LuaStop = false;                         // 暂停所有lua调用

    public static string SelectSocketAddress = string.Empty;    // 选择的Socket服务器地址
	public static int SocketPort = 0;                           // Socket服务器端口
    public static string SocketAddress = string.Empty;          // Socket服务器地址
    public static uint RecvKey = 0;
    public static uint SendKey = 0;
    public static int NetStatus = 0;
    public static bool fcmInit = false;                         // google云推送是否初始化

    public static int platid = 0;                               // 平台ID
    public static string userid = string.Empty;                 // 游戏用户ID
    public static string serverid = string.Empty;               // 服务器ID
    public static string actorid = string.Empty;                // 角色ID
    public static bool trackover = false;
    public static int quality = 0;                              // 0 高配， 1, 中配， 2， 低配

    // SDK使用
    public static bool sdk_isinit           = false;    // SDK是否初始化成功
    public static bool sdk_isEnterUserCenter = false;	// 是否有 个人中心功能
	public static bool sdk_isEnterBBS 		= false;	// 是否有 论坛功能
	public static bool sdk_isExitGame 		= false; 	// 是否有 退出游戏功能
	public static bool sdk_isLogout 		= false; 	// 是否有 注销功能
	public static bool sdk_isSwitchAccount	= false; 	// 是否有 切换账号功能
	public static bool sdk_isShareSdk 		= false; 	// 是否有 分享功能
    public static int sdk_payprice          = 0;
    public static string sdk_paytype        = string.Empty;
    public static int sdk_productid          = 0;
#if UNITY_ANDROID
	public static string sdk_sysType 		= "2"; 	    // 系统类型 1：ios 2：android
#elif UNITY_IOS
    public static string sdk_sysType 		= "1";
#else
    public static string sdk_sysType 		= "0";
#endif
    public static string sdk_fbuid          = string.Empty; // FB
    public static string sdk_gpuid          = string.Empty; // GOOGLE
    public static string sdk_puid           = string.Empty;
    public static string sdk_uid            = string.Empty;
    public static string sdk_sign           = string.Empty;
    public static string sdk_timestamp      = string.Empty;
    public static string sdk_channelId      = "0";
    public static string sdk_channelLable   = string.Empty;
    public static string sdk_isnewuser      = "0";

    //public static void setnetkey()
    //{
    //    fruit.networkManager.SendNetKey( Const.SendKey, Const.RecvKey );
    //}
}
