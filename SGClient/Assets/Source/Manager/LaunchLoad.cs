using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using System.IO;

public class LaunchLoad : MonoBehaviour {

	// Use this for initialization
	void Start () {
        LogUtil.GetInstance().Create( PathUtil.LogPath() );
        LogUtil.GetInstance().WriteGame( "------------------ Launcher Start ------------------" );
        Debug.Log( "StreamingAssetsPath:" + PathUtil.StreamingAssetsPath() );
        Debug.Log( "PackagePath:" + PathUtil.PackagePath() );
        Debug.Log( "LocalPath:" + PathUtil.LocalPath() );
        Debug.Log( "LogPath:" + PathUtil.LogPath() );
        LogUtil.GetInstance().WriteGame( "StreamingAssetsPath:" + PathUtil.StreamingAssetsPath() );
        LogUtil.GetInstance().WriteGame( "PackagePath:" + PathUtil.PackagePath() );
        LogUtil.GetInstance().WriteGame( "LocalPath:" + PathUtil.LocalPath() );

        if ( Const.ResourceMode == "assetbundle" )
        {
            AssetBundleInfo abinfo = ResourceManager.GetLoadedAssetBundle( "launchload" );
            if ( abinfo == null )
            {
                // 读取开始界面必需资源
                string file = PathUtil.LocalPath() + "StreamingAssets";
                if ( File.Exists( file ) == false )
                {
                    file = PathUtil.PackagePath() + "StreamingAssets";
                }

                AssetBundle ab = AssetBundle.LoadFromFile( file );
                ResourceManager.m_AssetBundleManifest = ab.LoadAsset( "AssetBundleManifest" ) as AssetBundleManifest;
                ResourceManager.m_AssetBundleNameList = ResourceManager.m_AssetBundleManifest.GetAllAssetBundles();
                ResourceManager.LoadAssetBundle( "launchload" );
                ab.Unload( false );
            }
        }
        SceneManager.LoadScene( "game" );
	}
	
	// Update is called once per frame
	void Update () {
		
	}
}
