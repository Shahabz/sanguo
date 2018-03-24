using UnityEngine;
using System.Collections;
using System;
using LuaInterface;

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

#else
#endif
    }

	// 充值回调
	void onPay( string jsonResult )
	{
		LuaFun.Call( "SDK.onPay", jsonResult );
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

#else
#endif
	}

}
