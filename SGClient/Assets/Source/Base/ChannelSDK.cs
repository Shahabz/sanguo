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
		if (Application.platform == RuntimePlatform.Android) {
			using (AndroidJavaClass jc = new AndroidJavaClass (DeviceHelper.AndroidPackageName+".SdkFun"))
			{
				jc.CallStatic ("init", jsonString);
			}
		}
		else if ( Application.platform == RuntimePlatform.IPhonePlayer )
		{
		}
		else 
		{

		}
	}

	// 初始化回调
	public void onInit( string jsonResult )
	{
		LuaFun.Call( "SDK.onInit", jsonResult );
	}

	// 登录
	public void login( string jsonString )
	{

        if ( Application.platform == RuntimePlatform.Android )
        {
			using ( AndroidJavaClass jc = new AndroidJavaClass( DeviceHelper.AndroidPackageName + ".SdkFun" ) )
            {
				jc.CallStatic( "login", jsonString );
            }
        }
        else if ( Application.platform == RuntimePlatform.IPhonePlayer )
        {

        }
        else 
        {
         
        }
	}

	// 登陆回调
	void onLogin( string jsonResult )
	{
		LuaFun.Call( "SDK.onLogin", jsonResult );
	}

	// 登出
	public void logout( string jsonString )
	{

		if ( Application.platform == RuntimePlatform.Android )
		{
			using ( AndroidJavaClass jc = new AndroidJavaClass( DeviceHelper.AndroidPackageName + ".SdkFun" ) )
			{
				jc.CallStatic( "logout", jsonString );
			}
		}
		else if ( Application.platform == RuntimePlatform.IPhonePlayer )
		{

		}
		else 
		{

		}
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
        if ( Application.platform == RuntimePlatform.Android )
        {
			using ( AndroidJavaClass jc = new AndroidJavaClass( DeviceHelper.AndroidPackageName + ".SdkFun" ) )
            {
				jc.CallStatic( "pay", jsonString );
            }
        }
        else if ( Application.platform == RuntimePlatform.IPhonePlayer )
        {

        }
        else
        {

        }
    }

	// 充值回调
	void onPay( string jsonResult )
	{
		LuaFun.Call( "SDK.onPay", jsonResult );
	}

	// 反馈
	public void gmbug( string jsonString )
	{
		if ( Application.platform == RuntimePlatform.Android )
		{
			using ( AndroidJavaClass jc = new AndroidJavaClass( DeviceHelper.AndroidPackageName + ".SdkFun" ) )
			{
				jc.CallStatic( "gmbug", jsonString );
			}
		}
		else if ( Application.platform == RuntimePlatform.IPhonePlayer )
		{

		}
		else
		{

		}
	}

	// 传送扩展数据
	public void setExtendData( string jsonString )
	{
		if ( Application.platform == RuntimePlatform.Android )
		{
			using ( AndroidJavaClass jc = new AndroidJavaClass( DeviceHelper.AndroidPackageName + ".SdkFun" ) )
			{
				jc.CallStatic( "setExtendData", jsonString );
			}
		}
		else if ( Application.platform == RuntimePlatform.IPhonePlayer )
		{

		}
		else
		{

		}
	}

}
