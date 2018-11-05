using UnityEngine;

#if UNITY_EDITOR
using UnityEditor;
using UnityEditor.Callbacks;
using UnityEditor.XCodeEditor;
using System.Xml;
#endif
using System.IO;

public static class XCodePostProcess
{

#if UNITY_EDITOR
	[PostProcessBuild(999)]
	public static void OnPostProcessBuild( BuildTarget target, string pathToBuiltProject )
	{
		if (target != BuildTarget.iOS) {
			Debug.LogWarning("Target is not iPhone. XCodePostProcess will not run");
			return;
		}

		// Create a new project object from build target
		XCProject project = new XCProject( pathToBuiltProject );

		// Find and run through all projmods files to patch the project.
		// Please pay attention that ALL projmods files in your project folder will be excuted!
		string[] files = Directory.GetFiles( Application.dataPath, "*.projmods", SearchOption.AllDirectories );
		foreach( string file in files ) {
			UnityEngine.Debug.Log("ProjMod File: "+file);
			project.ApplyMod( file );
		}
		string path = Path.GetFullPath(pathToBuiltProject);
		// ipx适配
		EditIphoneXCode(path);
		// www url 缓存
		EditWWWCacheCode(path); 
		// 编辑plist
		EditorPlist_all(path);
		//TODO implement generic settings as a module option
		Debug.Log(projectName);
        if (projectName == "test")
        {// 测试版
         //project.overwriteBuildSetting ("CODE_SIGN_IDENTITY[sdk=iphoneos*]", "iPhone Developer: JiaJia Wen (WT35G7885B)", "Release");
         //project.overwriteBuildSetting( "DEVELOPMENT_TEAM", "9C3LHQ7ULB", "Release");
         //project.overwriteBuildSetting( "PROVISIONING_PROFILE_SPECIFIER", "dist_zysg", "Release");
         //project.overwriteBuildSetting( "CODE_SIGN_IDENTITY[sdk=iphoneos*]", "iPhone Distribution: JiaJia Wen (9C3LHQ7ULB)", "Release");
        }
        else if (projectName == "lvke")
        { // 绿壳
          //project.overwriteBuildSetting( "DEVELOPMENT_TEAM", "9C3LHQ7ULB", "Release");
          //project.overwriteBuildSetting( "PROVISIONING_PROFILE_SPECIFIER", "dist_zysg", "Release");
          //project.overwriteBuildSetting ("CODE_SIGN_IDENTITY[sdk=iphoneos*]", "iPhone Distribution: JiaJia Wen (9C3LHQ7ULB)", "Release");
            EditorPlistLvke(path);
            EditorCodeLvke(path);
        }
        else if (projectName == "yaya")
        { // 呀呀游戏
          //project.overwriteBuildSetting( "DEVELOPMENT_TEAM", "9C3LHQ7ULB", "Release");
          //project.overwriteBuildSetting( "PROVISIONING_PROFILE_SPECIFIER", "dist_zysg", "Release");
          //project.overwriteBuildSetting ("CODE_SIGN_IDENTITY[sdk=iphoneos*]", "iPhone Distribution: JiaJia Wen (9C3LHQ7ULB)", "Release");
            EditorPlistYaya(path);
            EditorCodeYaya(path);
        }
        else if (projectName == "lsqxz")
        { // 繁体海外
            EditorPlistLsqxz(path);
            EditorCodeLsqxz(path);
        }
        else if (projectName == "xtby")
        {   // 海外独代
            EditorPlistXtby(path);
            EditorCodeXtby(path);
        }
        // Finally save the xcode project
        project.Save();

	}
#endif
	public static string projectName
	{
		get
		{
			foreach(string arg in System.Environment.GetCommandLineArgs()) {
				if(arg.StartsWith("project"))
				{
					return arg.Split("-"[0])[1];
				}
			}
			return "";
		}
	}

	private static void EditorPlist_all(string filePath)
	{

		XCPlist2 list = new XCPlist2(filePath);

		string Plist_url_types = @"
		<key>NSPhotoLibraryAddUsageDescription</key>
		<string>This app requires access to the photo library.</string>
		<key>NSPhotoLibraryUsageDescription</key>
		<string>This app requires access to the photo library.</string>
		<key>NSCameraUsageDescription</key>
		<string>This app requires access to the camera.</string>
		";
		list.AddKey(Plist_url_types);
		list.Save();
	}


	private static void EditorPlist(string filePath)
	{
		XCPlist2 list = new XCPlist2(filePath);
		string Plist_url_types = @"";
		list.AddKey(Plist_url_types); 
		//在plist里面替换一行
		// list.ReplaceKey("<string>com.yusong.${PRODUCT_NAME}</string>","<string>"+bundle+"</string>");
		//保存
		list.Save();
	}
	private static void EditorCode(string filePath)
    {
        // UnityAppController.mm
        //XClass UnityAppController = new XClass(filePath + "/Classes/UnityAppController.mm");
        //UnityAppController.WriteBelow("#include \"PluginBase/AppDelegateListener.h\"","#import <ShareSDK/ShareSDK.h>");
        //UnityAppController.WriteBelow("#include \"PluginBase/AppDelegateListener.h\"", "#include <SplusLibrary/SplusInterfaceKit.h>");
        //UnityAppController.Replace("return YES;","return [ShareSDK handleOpenURL:url sourceApplication:sourceApplication annotation:annotation wxDelegate:nil];");
        //UnityAppController.WriteBelow("UnityCleanup();\n}","- (BOOL)application:(UIApplication *)application handleOpenURL:(NSURL *)url\r{\r    return [ShareSDK handleOpenURL:url wxDelegate:nil];\r}");
        //UnityAppController.WriteBelow("[self preStartUnity];\n", "\t[[SplusInterfaceKit sharedInstance] initSplus:_rootView];");
		//UnityAppController.WriteBelow("[self preStartUnity];\n","\t    [[SplusInterfaceKit sharedInstance] initSplus];");
		//UnityAppController.WriteBelow("AppController_SendNotificationWithArg(kUnityOnOpenURL, notifData);\n", "\t[[SplusInterfaceKit sharedInstance] application:application openURL:url sourceApplication:sourceApplication annotation:annotation];");
		//UnityAppController.WriteBelow("_didResignActive = false;\n","\t    [[SplusInterfaceKit sharedInstance] applicationDidBecomeActive:application];");
		//UnityAppController.WriteBelow("[GetAppController().unityView recreateGLESSurfaceIfNeeded];\n\t}\n","\t[[SplusInterfaceKit sharedInstance] applicationWillEnterForeground:application];");
    }

	// 传娱sdk
	private static void EditorPlistLvke(string filePath)
	{
		XCPlist2 list = new XCPlist2(filePath);

		string PhotoInfolist = @"
		<key>NSPhotoLibraryAddUsageDescription</key>
		<string>以照片的形式为您保存账号密码</string>
		<key>NSPhotoLibraryUsageDescription</key>
		<string>以照片的形式为您保存账号密码</string>";
		list.AddKey(PhotoInfolist);

		string Infolist = @"
		<key>CFBundleURLTypes</key>
		<array>
		<dict>
		<key>CFBundleTypeRole</key>
		<string>Editor</string>
		<key>CFBundleURLIconFile</key>
		<string></string>
		<key>CFBundleURLName</key>
		<string></string>
		<key>CFBundleURLSchemes</key>
		<array>
		<string>hchuleg.com1435853907</string>
		</array>
		</dict>
		</array>";
		list.AddKey(Infolist);

		//保存
		list.Save();
	}
	private static void EditorCodeLvke(string filePath)
	{
		// UnityAppController.mm
		XClass UnityAppController = new XClass(filePath + "/Classes/UnityAppController.mm");
		UnityAppController.WriteBelow("#include \"PluginBase/AppDelegateListener.h\"","#import <GameFramework/GameFramework.h>");
		UnityAppController.WriteBelow("AppController_SendNotificationWithArg(kUnityOnOpenURL, notifData);","[Game_Api Game_application:application openURL:url sourceApplication:sourceApplication annotation:annotation];");
	}

	// 呀呀游戏
	private static void EditorPlistYaya(string filePath)
	{
		XCPlist2 list = new XCPlist2(filePath);

		string PhotoInfolist = @"
		<key>NSPhotoLibraryAddUsageDescription</key>
		<string>以照片的形式为您保存账号密码</string>
		<key>NSPhotoLibraryUsageDescription</key>
		<string>以照片的形式为您保存账号密码</string>";
		list.AddKey(PhotoInfolist);

		string Infolist = @"
		<key>CFBundleURLTypes</key>
		<array>
		<dict>
		<key>CFBundleTypeRole</key>
		<string>Editor</string>
		<key>CFBundleURLIconFile</key>
		<string></string>
		<key>CFBundleURLName</key>
		<string></string>
		<key>CFBundleURLSchemes</key>
		<array>
		<string>com.xxsg.zhangm1438496089</string>
		</array>
		</dict>
		</array>";
		list.AddKey(Infolist);

		//保存
		list.Save();
	}
	private static void EditorCodeYaya(string filePath)
	{
		// UnityAppController.mm
		XClass UnityAppController = new XClass(filePath + "/Classes/UnityAppController.mm");
		UnityAppController.WriteBelow("#include \"PluginBase/AppDelegateListener.h\"","#import <GameUser_framwork/YvGameUserAPIManage.h>\n#define SDK_APPID    @\"1438496089\"");
		UnityAppController.WriteBelow("[KeyboardDelegate Initialize];","[[YvGameUserAPIManage shareInstance] initWithAppId:SDK_APPID channelId:@\"xxsg001\" urlScheme:@\"com.xxsg.zhangm1438496089\" isTest:NO];");
	}

	// lsqxz
	private static void EditorPlistLsqxz(string filePath)
	{
		XCPlist2 list = new XCPlist2(filePath);

		string FacebookDialogsInfolist = @"
		<key>LSApplicationQueriesSchemes</key>
		<array>
		  <string>fbapi</string>
		  <string>fb-messenger-api</string>
		  <string>fbauth2</string>
		  <string>fbshareextension</string>
		</array>";
		list.AddKey(FacebookDialogsInfolist);

		string Infolist = @"
		<key>CFBundleURLTypes</key>
		<array>
		  <dict>
		  <key>CFBundleURLSchemes</key>
		  <array>
		    <string>fb1093672920808279</string>
		  </array>
		  </dict>
		</array>
		<key>FacebookAppID</key>
		<string>1093672920808279</string>
		<key>FacebookDisplayName</key>
		<string>乱世群雄转</string>";
		list.AddKey(Infolist);

		//保存
		list.Save();
	}
	private static void EditorCodeLsqxz(string filePath)
	{
		// UnityAppController.mm
		XClass UnityAppController = new XClass(filePath + "/Classes/UnityAppController.mm");
		UnityAppController.WriteBelow("#include \"PluginBase/AppDelegateListener.h\"","#import <FBSDKCoreKit/FBSDKCoreKit.h>");
		UnityAppController.WriteBelow("_didResignActive = false;","[FBSDKAppEvents activateApp];");
		UnityAppController.WriteBelow("[KeyboardDelegate Initialize];","[[FBSDKApplicationDelegate sharedInstance] application:application\n                           didFinishLaunchingWithOptions:launchOptions];");
		UnityAppController.Replace("AppController_SendNotificationWithArg(kUnityOnOpenURL, notifData);\n    return YES;","AppController_SendNotificationWithArg(kUnityOnOpenURL, notifData);\n    return [[FBSDKApplicationDelegate sharedInstance] application:application\n                                                         openURL:url\n                                               sourceApplication:sourceApplication\n                                                      annotation:annotation];;");
	}

    // xtby
    private static void EditorPlistXtby(string filePath)
    {
        XCPlist2 list = new XCPlist2(filePath);

        string FacebookDialogsInfolist = @"
        <key>LSApplicationQueriesSchemes</key>
        <array>
          <string>fbapi</string>
          <string>fb-messenger-api</string>
          <string>fbauth2</string>
          <string>fbshareextension</string>
        </array>";
        list.AddKey(FacebookDialogsInfolist);

        string Infolist = @"
        <key>CFBundleURLTypes</key>
        <array>
          <dict>
          <key>CFBundleURLSchemes</key>
          <array>
            <string>fb951132541753415</string>
          </array>
          </dict>
        </array>
        <key>FacebookAppID</key>
        <string>951132541753415</string>
        <key>FacebookDisplayName</key>
        <string>雄圖霸業</string>";
        list.AddKey(Infolist);

        //保存
        list.Save();
    }
    private static void EditorCodeXtby(string filePath)
    {
        // UnityAppController.mm
        XClass UnityAppController = new XClass(filePath + "/Classes/UnityAppController.mm");
        UnityAppController.WriteBelow("#include \"PluginBase/AppDelegateListener.h\"", "#import <FBSDKCoreKit/FBSDKCoreKit.h>");
        UnityAppController.WriteBelow("_didResignActive = false;", "[FBSDKAppEvents activateApp];");
        UnityAppController.WriteBelow("[KeyboardDelegate Initialize];", "[[FBSDKApplicationDelegate sharedInstance] application:application\n                           didFinishLaunchingWithOptions:launchOptions];");
        UnityAppController.Replace("AppController_SendNotificationWithArg(kUnityOnOpenURL, notifData);\n    return YES;", "AppController_SendNotificationWithArg(kUnityOnOpenURL, notifData);\n    return [[FBSDKApplicationDelegate sharedInstance] application:application\n                                                         openURL:url\n                                               sourceApplication:sourceApplication\n                                                      annotation:annotation];;");
    }

    // ipx 齐刘海适配
    public static void EditIphoneXCode(string path) 
	{
		//插入代码
		//读取UnityAppController.mm文件
		string src = @"_window         = [[UIWindow alloc] initWithFrame: [UIScreen mainScreen].bounds];";
		string dst = @"//    _window         = [[UIWindow alloc] initWithFrame: [UIScreen mainScreen].bounds];

   CGRect winSize = [UIScreen mainScreen].bounds;
   if (winSize.size.height / winSize.size.width > 2) {
       winSize.size.height -= 150;
       winSize.origin.y = 75;
       ::printf(""-> is iphonex aaa hello world\n"");
   } else {
       ::printf(""-> is not iphonex aaa hello world\n"");
   }
   _window = [[UIWindow alloc] initWithFrame: winSize];

   ";
		XClass UnityAppController = new XClass(path + "/Classes/UnityAppController.mm");
		UnityAppController.Replace(src, dst);
	}

	// www url 缓存
	public static void EditWWWCacheCode(string path) 
	{
		//插入代码
		XClass UnityAppController = new XClass(path + "/Classes/Unity/WWWConnection.mm");
		UnityAppController.WriteBelow("[request setAllHTTPHeaderFields: headers];","request.cachePolicy = NSURLRequestReloadIgnoringLocalCacheData;\n    [request setHTTPShouldHandleCookies:NO];");
	}

	public static void Log(string message)
	{
		UnityEngine.Debug.Log("PostProcess: "+message);
	}
}
