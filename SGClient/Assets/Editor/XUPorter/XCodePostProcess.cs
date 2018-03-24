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

		//TODO implement generic settings as a module option
		Debug.Log(projectName);
		if(projectName== "zaya")
		{
			//project.overwriteBuildSetting ("CODE_SIGN_IDENTITY[sdk=iphoneos*]", "iPhone Developer: JiaJia Wen (WT35G7885B)", "Release");
			project.overwriteBuildSetting( "DEVELOPMENT_TEAM", "9C3LHQ7ULB", "Release");
			project.overwriteBuildSetting( "PROVISIONING_PROFILE_SPECIFIER", "dist_zysg", "Release");
			project.overwriteBuildSetting( "CODE_SIGN_IDENTITY[sdk=iphoneos*]", "iPhone Distribution: JiaJia Wen (9C3LHQ7ULB)", "Release");
			string path = Path.GetFullPath(pathToBuiltProject);
			EditorPlist(path);
			EditorCode(path);
		}
		else if(projectName== "sdk_zaya")
		{
			project.overwriteBuildSetting ("CODE_SIGN_IDENTITY[sdk=iphoneos*]", "iPhone Distribution: JiaJia Wen (9C3LHQ7ULB)", "Release");
			string path = Path.GetFullPath(pathToBuiltProject);
			EditorPlist(path);
			EditorCode(path);
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
		
	private static void EditorPlist(string filePath)
	{

		XCPlist2 list = new XCPlist2(filePath);

		string Plist_url_types = @"";
		list.AddKey(Plist_url_types); 

		//在plist里面替换一行
		//        list.ReplaceKey("<string>com.yusong.${PRODUCT_NAME}</string>","<string>"+bundle+"</string>");
		//保存
		list.Save();

	}

	private static void EditorCode(string filePath)
    {
        // UnityAppController.mm
        //XClass UnityAppController = new XClass(filePath + "/Classes/UnityAppController.mm");

        //
        //        UnityAppController.WriteBelow("#include \"PluginBase/AppDelegateListener.h\"","#import <ShareSDK/ShareSDK.h>");
        //		  UnityAppController.WriteBelow("#include \"PluginBase/AppDelegateListener.h\"", "#include <SplusLibrary/SplusInterfaceKit.h>");

        //
        //        UnityAppController.Replace("return YES;","return [ShareSDK handleOpenURL:url sourceApplication:sourceApplication annotation:annotation wxDelegate:nil];");

        //
        //        UnityAppController.WriteBelow("UnityCleanup();\n}","- (BOOL)application:(UIApplication *)application handleOpenURL:(NSURL *)url\r{\r    return [ShareSDK handleOpenURL:url wxDelegate:nil];\r}");
        //UnityAppController.WriteBelow("[self preStartUnity];\n", "\t[[SplusInterfaceKit sharedInstance] initSplus:_rootView];");

		//UnityAppController.WriteBelow("[self preStartUnity];\n","\t    [[SplusInterfaceKit sharedInstance] initSplus];");

		//UnityAppController.WriteBelow("AppController_SendNotificationWithArg(kUnityOnOpenURL, notifData);\n", "\t[[SplusInterfaceKit sharedInstance] application:application openURL:url sourceApplication:sourceApplication annotation:annotation];");
		
		//UnityAppController.WriteBelow("_didResignActive = false;\n","\t    [[SplusInterfaceKit sharedInstance] applicationDidBecomeActive:application];");
		
		//UnityAppController.WriteBelow("[GetAppController().unityView recreateGLESSurfaceIfNeeded];\n\t}\n","\t[[SplusInterfaceKit sharedInstance] applicationWillEnterForeground:application];");
    }

	public static void Log(string message)
	{
		UnityEngine.Debug.Log("PostProcess: "+message);
	}
}
