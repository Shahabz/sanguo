using UnityEngine;
using System.Collections;
using System;
using LuaInterface;
#if UNITY_IPHONE || UNITY_IOS
using System.Runtime.InteropServices;
#endif
public class ChannelSDK : MonoBehaviour
{
	private static ChannelSDK _instance;
	public static ChannelSDK Instance
	{
		get
		{
			if ( _instance == null )
			{
				_instance = eye.gameManager.transform.Find( "SdkAndroidCallbackEntry" ).GetComponent<ChannelSDK>();
            }
			return _instance;
		}
	}
#if UNITY_IOS
	[DllImport("__Internal")]
	private static extern void sdkInit( string jsonString );//初始化
	[DllImport("__Internal")]
	private static extern void sdkLogin( string jsonString );//登录
	[DllImport("__Internal")]
	private static extern void sdkLogout( string jsonString );// 登出
	[DllImport("__Internal")]
	private static extern void sdkPay( string jsonString );// 支付
	[DllImport("__Internal")]
	private static extern void sdkUserCenter( string jsonString );// 用户中心
	[DllImport("__Internal")]
	private static extern void sdkGmBug( string jsonString );// 反馈
	[DllImport("__Internal")]
	private static extern void sdkSetExtendData( string jsonString );// 数据统计
#endif
    // 初始化
    public void init( string jsonString )
	{
#if UNITY_EDITOR
#elif UNITY_ANDROID
		using (AndroidJavaClass jc = new AndroidJavaClass (DeviceHelper.AndroidPackageName+".SdkFun"))
		{
			jc.CallStatic ("init", jsonString);
		}
#elif  UNITY_IPHONE || UNITY_IOS
		sdkInit( jsonString );
#else
#endif
	}

	// 初始化回调
	public void onInit( string jsonResult )
	{
		LuaFun.Call( "SDK.onInit", jsonResult );
	}

	// 登录
	public void login( string jsonString )
	{
#if UNITY_EDITOR
#elif UNITY_ANDROID
		using ( AndroidJavaClass jc = new AndroidJavaClass( DeviceHelper.AndroidPackageName + ".SdkFun" ) )
		{
			jc.CallStatic( "login", jsonString );
		}
#elif  UNITY_IPHONE || UNITY_IOS
		sdkLogin( jsonString );
#else
#endif
	}

	// 登陆回调
	void onLogin( string jsonResult )
	{
		LuaFun.Call( "SDK.onLogin", jsonResult );
	}

	// 登出
	public void logout( string jsonString )
	{
#if UNITY_EDITOR
#elif UNITY_ANDROID
		using ( AndroidJavaClass jc = new AndroidJavaClass( DeviceHelper.AndroidPackageName + ".SdkFun" ) )
		{
			jc.CallStatic( "logout", jsonString );
		}
#elif  UNITY_IPHONE || UNITY_IOS
		sdkLogout( jsonString );
#else
#endif
	}

	// 登出回调
	void onLogout( string jsonResult )
	{
		LuaFun.Call( "SDK.onLogout", jsonResult );
	}

	// 切换账号回调
	void onSwitchAccount( string jsonResult )
	{
		LuaFun.Call( "SDK.onSwitchAccount", jsonResult );
	}

	// 充值
	public void pay( string jsonString )
	{
#if UNITY_EDITOR
#elif UNITY_ANDROID
		using ( AndroidJavaClass jc = new AndroidJavaClass( DeviceHelper.AndroidPackageName + ".SdkFun" ) )
		{
			jc.CallStatic( "pay", jsonString );
		}
#elif  UNITY_IPHONE  || UNITY_IOS
		sdkPay( jsonString );
#else
#endif
    }

	// 充值回调
	void onPay( string jsonResult )
	{
		LuaFun.Call( "SDK.onPay", jsonResult );
	}

	// 用户中心
	public void user_center( string jsonString )
	{
#if UNITY_EDITOR
#elif UNITY_ANDROID
		using ( AndroidJavaClass jc = new AndroidJavaClass( DeviceHelper.AndroidPackageName + ".SdkFun" ) )
		{
		jc.CallStatic( "user_center", jsonString );
		}
#elif  UNITY_IPHONE || UNITY_IOS
		sdkUserCenter( jsonString );
#else
#endif
	}

	// 反馈
	public void gmbug( string jsonString )
	{
#if UNITY_EDITOR
#elif UNITY_ANDROID
		using ( AndroidJavaClass jc = new AndroidJavaClass( DeviceHelper.AndroidPackageName + ".SdkFun" ) )
		{
			jc.CallStatic( "gmbug", jsonString );
		}
#elif  UNITY_IPHONE || UNITY_IOS
		sdkGmBug( jsonString );
#else
#endif
	}

	// 传送扩展数据
	public void setExtendData( string jsonString )
	{
#if UNITY_EDITOR
#elif UNITY_ANDROID
		using ( AndroidJavaClass jc = new AndroidJavaClass( DeviceHelper.AndroidPackageName + ".SdkFun" ) )
		{
			jc.CallStatic( "setExtendData", jsonString );
		}
#elif  UNITY_IPHONE || UNITY_IOS
		sdkSetExtendData( jsonString );
#else
#endif
	}

}
