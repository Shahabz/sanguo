using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;
using LuaInterface;
using System.Reflection;
using System.IO;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

public class PackageManager : MonoBehaviour
{
    class DownLoadFile
    {
        public DownLoadFile( int size, string md5 )
        {
            this.size = size;
            this.md5 = md5;
            this.BytesReceived = 0;
        }
        public int size = 0;
        public string md5 = string.Empty;
        public long BytesReceived = 0;
    }

    // 全部内容
    public GameObject m_uiContent;
    // 解压过程中的文字描述
    public UIText m_uiText;
    // 下载过程中的文字描述
    public UIText m_uiProgressText;
    public UIText m_uiSpeedText;
    public GameObject m_uiWarningBox;
    public GameObject m_uiNetworkBox;
    public GameObject m_uiForceUpdateBox;
    public GameObject m_uiAdvice;
    private string m_szFilePrefix = "";
    // 文件列表名称
    private string m_szFilesName = "files.txt";
    // 程序配置文件名称
    private string m_szAppTxt = "app.txt";
    // 程序配置文件
    private IniUtil m_pAppIni = new IniUtil();
    // 资源版本文件名称
    private string m_szResverTxt = "resver.txt";
    // 解压到本地的资源版本文件
    private IniUtil m_pLocalResverIni = new IniUtil();
    // 包里的资源版本文件
    private IniUtil m_pPackageResverIni = new IniUtil();
    // 资源版本号
    private int m_local_resource_version = 0;
    private int m_package_resource_version = 0;
    // 需要下载的文件
    Dictionary<string, DownLoadFile> m_szNeedUpdateFileDict;
    private List<string> m_downloadFiles = new List<string>();
    private string m_downloadProgress = "";
    private string m_downloadspeed = "";
    private long m_downloadsize_total = 0;
    private bool m_AskSureUpdate = false;
    //异步对象
    AsyncOperation async = null;

    // 初始化
    void Awake()
    {
#if UNITY_IOS // 不需要iCloud备份
        UnityEngine.iOS.Device.SetNoBackupFlag( PathUtil.LocalPath() );
        UnityEngine.iOS.Device.SetNoBackupFlag( PathUtil.ImagePath()() );
        UnityEngine.iOS.Device.SetNoBackupFlag( PathUtil.ConfigPath() );
#endif
        // 调整安卓分辨率
        //GameManager.setDesignContentScale();
        StartCoroutine( ShowUIOnNextFrame() );

        // 禁止黑屏
        Screen.sleepTimeout = SleepTimeout.NeverSleep;
        // 后台运行
        Application.runInBackground = true;

        // 不同平台不同的文件前缀
        if ( Application.platform == RuntimePlatform.Android )
            m_szFilePrefix = "android";
        else if ( Application.platform == RuntimePlatform.IPhonePlayer )
            m_szFilePrefix = "ios";
        else if ( Application.platform == RuntimePlatform.WindowsEditor || Application.platform == RuntimePlatform.WindowsPlayer )
            m_szFilePrefix = "windows";
        else if ( Application.platform == RuntimePlatform.OSXEditor || Application.platform == RuntimePlatform.OSXPlayer )
            m_szFilePrefix = "osx";

        // 文件列表名称
        m_szFilesName = m_szFilePrefix + "_" + "files.txt";

        // 创建启动日志
        LogUtil.GetInstance().Create( PathUtil.LogPath() );
        LogUtil.GetInstance().WriteGame( "------------------ Launcher Start ------------------" );
        Debug.Log( "StreamingAssetsPath:" + PathUtil.StreamingAssetsPath() );
        Debug.Log( "PackagePath:" + PathUtil.PackagePath() );
        Debug.Log( "LocalPath:" + PathUtil.LocalPath() );
        Debug.Log( "LogPath:" + PathUtil.LogPath() );

        LogUtil.GetInstance().WriteGame( "StreamingAssetsPath:" + PathUtil.StreamingAssetsPath() );
        LogUtil.GetInstance().WriteGame( "PackagePath:" + PathUtil.PackagePath() );
        LogUtil.GetInstance().WriteGame( "LocalPath:" + PathUtil.LocalPath() );

        // 本地化系统初始化
        if ( Localization.init() < 0 )
        {
            LogUtil.GetInstance().WriteGame( "Localization Init Failed" );
            return;
        }

        // 根据地区是否显示健康游戏忠告
        if ( Localization.currentCountry == "cn" )
            m_uiAdvice.SetActive( true );

        // 开始启动逻辑
        Begin();
    }

    IEnumerator ShowUIOnNextFrame()
    {
        yield return new WaitForEndOfFrame();
        yield return new WaitForEndOfFrame();
        m_uiContent.SetActive( true );
    }

    void Update()
    {
        if ( m_downloadProgress != "" )
        {
            m_uiProgressText.text = m_downloadProgress;
        }
        if ( m_downloadspeed != "" )
        {
            m_uiSpeedText.text = m_downloadspeed;
        }

        // 进度
        if ( async != null )
        {
            //float space = 0.2f - 0.04f;
            //float proc = 0.04f + async.progress / 1 * space;
            //m_uiProgress.value = proc;
        }
    }

    public void Begin()
    {
        // 检测网络状态
        if ( Application.internetReachability == NetworkReachability.NotReachable )
        {
            // 开启网络状态提示框
            m_uiNetworkBox.SetActive( true );
            return;
        }
        else
        {
            // 关闭网络状态提示框
            m_uiNetworkBox.SetActive( false );
        }

        // 读取包里的资源版本文件
        m_pPackageResverIni.OpenFromStreamingAssets( m_szResverTxt );

        // 读取本地的资源版本文件
        m_pLocalResverIni.OpenFromTXT( PathUtil.ConfigPath() + m_szResverTxt );

        if ( Const.ResourceMode == "assetbundle" )
        {
            // 其它模式先检查源
            CheckResource();
        }
        else
        {
            // 非assetbundle模式直接从Resource目录读取资源
            OnInited();
        }
    }

    /// <summary>
    /// 第一步，检查是否是老资源，有则删除
    /// </summary>
    public void CheckResource()
    {
        // 比较资源版本号
        m_package_resource_version = Convert.ToInt32( m_pPackageResverIni.ReadValue( "RESOURCE_VERSION", "0" ) );
        m_local_resource_version = Convert.ToInt32( m_pLocalResverIni.ReadValue( "RESOURCE_VERSION", "0" ) );
        LogUtil.GetInstance().WriteGame( "m_local_resource_version:" + m_local_resource_version );
        LogUtil.GetInstance().WriteGame( "m_package_resource_version:" + m_package_resource_version );

        // 包里的资源版本号比解压出来的大
        if ( m_package_resource_version > m_local_resource_version )
        {  // 将文件列表和版本文件解压，并清除旧文件
            StartCoroutine( OnExtractResource() );
        }
        else
        {   // 启动检查更新逻辑
            StartCoroutine( OnUpdateResource() );
        }
    }

    /// <summary>
    /// 第二步，开始释放资源
    /// </summary>
    IEnumerator OnExtractResource()
    {
        LogUtil.GetInstance().WriteGame( "Start ExtractResource!" );
        string sourcePath = PathUtil.StreamingAssetsPath(); // 游戏安装包目录
        string targetPath = PathUtil.LocalPath();   // 本地目录

        // 有数据目录，先删除，再创建，保持干净
        if ( Directory.Exists( targetPath ) )
        {
            Directory.Delete( targetPath, true );
        }
        Directory.CreateDirectory( targetPath );

#if UNITY_IOS
        UnityEngine.iOS.Device.SetNoBackupFlag( targetPath );
#endif

        // ------------------------------ 文件列表 -------------------------------------
        // 有文件列表，先删除，再创建，保持干净
        string sourceFile = sourcePath + m_szFilesName;
        string targetFile = targetPath + m_szFilesName;

        // 拷贝文件
        if ( Application.platform == RuntimePlatform.Android )
        {
            WWW www = new WWW( sourceFile );
            yield return www;

            if ( www.isDone )
            {
                File.WriteAllBytes( targetFile, www.bytes );
            }
        }
        else File.Copy( sourceFile, targetFile, true );
        LogUtil.GetInstance().WriteGame( "FileList Extract Finish!" );

        // ------------------------------ 更新程序资源版本文件 ----------------------------
        string resverSourceFile = PathUtil.StreamingAssetsPath() + m_szResverTxt;
        string resverTargetFile = PathUtil.ConfigPath() + m_szResverTxt;

        // 如果文件有子目录，创建子目录
        string resverDir = Path.GetDirectoryName( resverTargetFile );
        if ( !Directory.Exists( resverDir ) )
        {
            Directory.CreateDirectory( resverDir );
#if UNITY_IOS
            UnityEngine.iOS.Device.SetNoBackupFlag( resverDir );
#endif
        }

        // 有文件，先删除，再创建，保持干净
        if ( File.Exists( resverTargetFile ) )
            File.Delete( resverTargetFile );
        if ( Application.platform == RuntimePlatform.Android )
        {
            WWW resverWWW = new WWW( resverSourceFile );
            yield return resverWWW;
            if ( resverWWW.isDone )
            {
                File.WriteAllBytes( resverTargetFile, resverWWW.bytes );
            }
        }
        else File.Copy( resverSourceFile, resverTargetFile, true );
        LogUtil.GetInstance().WriteGame( "Resver Extract Finish!" );

        // 重读本地资源版本文件
        m_pLocalResverIni.OpenFromTXT( resverTargetFile );
        LogUtil.GetInstance().WriteGame( "Resver Reload Finish!" );
        // ------------------------------------------------------------------------------------

        // 释放完成，开始启动更新资源逻辑
        StartCoroutine( OnUpdateResource() );
    }

    /// <summary>
    /// 第四步，启动web更新逻辑
    /// </summary>
    IEnumerator OnUpdateResource()
    {
        LogUtil.GetInstance().WriteGame( "Start UpdateResource!" );
        // 更新过程中，每秒都需要进行网络检查
        StartCoroutine( OnCheckNetwork() );

        // 本地平台
        IniUtil plat = new IniUtil();
        plat.OpenFromData( "plat.txt" );
        Const.platid = Convert.ToInt32( plat.ReadValue( "PLATID", "" ) );
        LogUtil.GetInstance().WriteGame( "PlatID:" + Const.platid );

        // 本地资源版本号
        m_local_resource_version = Convert.ToInt32( m_pLocalResverIni.ReadValue( "RESOURCE_VERSION", "0" ) );
        LogUtil.GetInstance().WriteGame( "UpdateResource Start local_version:" + m_local_resource_version );

        // 本地资源文件列表，生成字典结构，文件名当索引，以便查找
        Dictionary<string, string> szlocalFileMd5Dict = new Dictionary<string, string>();
        Dictionary<string, int> szlocalFileSizeDict = new Dictionary<string, int>();

        // 读取本地文件列表
        string[] szlocalFileList = File.ReadAllLines( PathUtil.LocalPath() + m_szFilesName );
        LogUtil.GetInstance().WriteGame( "FileList Readed!" );

        // 解析文件列表
        foreach ( var localFile in szlocalFileList )
        {
            string[] values = localFile.Split( '|' );
            string fileName = values[0];
            string fileMd5 = values[1];
            int fileSize = Convert.ToInt32( values[2] );
            szlocalFileMd5Dict.Add( fileName, fileMd5 );
            szlocalFileSizeDict.Add( fileName, fileSize );
        }

        // 更新地址
        bool appSucc = m_pAppIni.OpenFromData( m_szAppTxt );
        LogUtil.GetInstance().WriteGame( "Read App.txt:" + appSucc.ToString() );
        string url = Convert.ToString( m_pAppIni.ReadValue( "UPDATE_URL", "" ) );

        // 获取服务器的更新文件地址
        // 先取cfg文件，没有文件才取php
        LogUtil.GetInstance().WriteGame( url + "/update_" + m_szFilePrefix + ".cfg" );
        WWW www = new WWW( url + "/update_" + m_szFilePrefix + ".cfg" );
        yield return www;
        if ( www.error != null )
        {
            // 取php
            LogUtil.GetInstance().WriteGame( url + "/update_" + m_szFilePrefix + ".php" );
            www = new WWW( url + "/update_" + m_szFilePrefix + ".php" );
            yield return www;
            if ( www.error != null )
            {
                // 更新地址错误也进游戏
                //m_uiText.text = "update url error";
                OnInited();
                yield break;
            }
        }

        LogUtil.GetInstance().WriteGame( "www.text:" + www.text );
        string[] result = www.text.Split( '|' );
        if ( result.Length < 2 )
        {
            OnInited();
            yield break;
        }
        // 最新版本
        int latest_version = Convert.ToInt32( result[0] );
        // 更新地址
        string updateResUrl = result[1];

        // 强制更新版本号(程序版本)，低于这个版本都需要强制更新
        if ( result.Length == 3 )
        {
            int forceVersion = Convert.ToInt32( result[2] );

            // 本地程序版本号
            string[] appVersionArray = Application.version.Split( '.' );
            int appVersion = Convert.ToInt32( appVersionArray[2] );
            LogUtil.GetInstance().WriteGame( "appVersion:" + appVersion );

            if ( appVersion < forceVersion )
            {
                // 请前去下载最新版本游戏
                m_uiForceUpdateBox.transform.Find( "Text" ).GetComponent<UIText>().text = Localization.text( 425 );
                m_uiForceUpdateBox.SetActive( true );
                yield break;
            }
        }

        LogUtil.GetInstance().WriteGame( "web_latest_version:" + latest_version );

        if ( latest_version < m_local_resource_version )
        {
            // 本地版本大于远端版本，不更新直接进游戏
            //m_uiText.text = Localization.text( 403 );
            LogUtil.GetInstance().WriteGame( "Has been the latest version" );
            OnInited();
            yield break;
        }

        // 先下载文件列表到本地
        www = new WWW( updateResUrl + "/" + m_szFilesName );
        yield return www;
        if ( www.error != null )
        {
            //m_uiText.text = "files url error";
            LogUtil.GetInstance().WriteGame( "files url error:" + www.error );
            OnInited();
            yield break;
        }

        // 如果存在，那么先删除在创建
        if ( Directory.Exists( PathUtil.PatchDownloadPath() ) )
        {
            Directory.Delete( PathUtil.PatchDownloadPath(), true );
        }
        Directory.CreateDirectory( PathUtil.PatchDownloadPath() );
        File.WriteAllBytes( PathUtil.PatchDownloadPath() + m_szFilesName, www.bytes );

        // 比较web下载下来的文件列表和本地的两个文件列表的差异数据
        m_downloadsize_total = 0;
        m_szNeedUpdateFileDict = new Dictionary<string, DownLoadFile>();
        string[] szUpdateFileList = File.ReadAllLines( PathUtil.PatchDownloadPath() + m_szFilesName );
        foreach ( var updateFile in szUpdateFileList )
        {
            string[] values = updateFile.Split( '|' );
            string updateFileName = values[0];
            string updateFileMd5 = values[1];
            int updateFileSize = Convert.ToInt32( values[2] );
            string updateFilePath = ( PathUtil.PatchDownloadPath() + updateFileName ).Trim();
            string updateFileDirectory = Path.GetDirectoryName( updateFilePath );
            if ( !Directory.Exists( updateFileDirectory ) )
            {
                Directory.CreateDirectory( updateFileDirectory );
#if UNITY_IOS
                UnityEngine.iOS.Device.SetNoBackupFlag( updateFileDirectory );
#endif
            }

            // 1.先找这个文件本地是否存在，不存在直接下载
            bool isUpdate = false;
            // 2.如果存在，比较文件大小，一样大就比较MD5，不一样大就直接下载
            if ( szlocalFileSizeDict.ContainsKey( updateFileName ) == false )
            {
                isUpdate = true;
                LogUtil.GetInstance().WriteGame( "[" + updateFilePath + "]Not Exists，Download！" );
            }
            else
            {
                if ( szlocalFileSizeDict[updateFileName] != updateFileSize )
                {
                    isUpdate = true;
                    LogUtil.GetInstance().WriteGame( "[" + updateFilePath + "] FileSize，Download！" );
                }
                else
                {
                    if ( !szlocalFileMd5Dict[updateFileName].Equals( updateFileMd5 ) )
                    {
                        isUpdate = true;
                        LogUtil.GetInstance().WriteGame( "[" + updateFilePath + "] MD5，Download！" );
                    }
                }
            }

            // 如果需要更新
            if ( isUpdate )
            {
                m_szNeedUpdateFileDict.Add( updateFileName, new DownLoadFile( updateFileSize, updateFileMd5 ) );
                m_downloadsize_total += updateFileSize;
            }
        }

        // 这里要询问玩家是否进行更新
        if ( m_downloadsize_total > 1024 * 1024 )
        {
            // 大于1mb提示
            m_AskSureUpdate = true;
            long nTotalSize = m_downloadsize_total;
            if ( nTotalSize < 1024 )
                nTotalSize = 1024;
            m_uiWarningBox.transform.Find( "Text" ).GetComponent<UIText>().text = string.Format( Localization.text( 407 ), nTotalSize / 1024.0 / 1024.0 );
            m_uiWarningBox.SetActive( true );
        }
        else
        {
            // 小于1mb不提示
            m_AskSureUpdate = false;
            m_uiWarningBox.SetActive( false );
        }
        // 等待按钮改变这个值
        while ( m_AskSureUpdate )
        {
            yield return new WaitForEndOfFrame();
        }


        // 初始化线程管理器
        Utils.Add<ThreadManager>( gameObject );

        // 开始更新
        bool isNeedUpdate = false;
        foreach ( var updateInfo in m_szNeedUpdateFileDict )
        {
            isNeedUpdate = true;
            m_uiText.text = Localization.text( 404 );
            LogUtil.GetInstance().WriteGame( "<Download>" + "[name]:" + updateInfo.Key + "|[size]:" + updateInfo.Value.size );

            bool downloadFinish = false;
            while ( !downloadFinish )
            {
                // 启动下载文件
                DownloadFile( updateResUrl + "/", updateInfo.Key );
                // 此处轮询检查这个文件时候已经下载
                while ( !m_downloadFiles.Contains( updateInfo.Key ) )
                {
                    yield return new WaitForEndOfFrame();
                }
                // 刚刚下载完成的文件
                string curr_updateFile = PathUtil.PatchDownloadPath() + updateInfo.Key;
                // 没这个文件
                if ( !File.Exists( curr_updateFile ) )
                {
                    LogUtil.GetInstance().WriteGame( "<Download Miss>" + "[path]:" + curr_updateFile );
                    m_downloadFiles.Remove( updateInfo.Key );
                    updateInfo.Value.BytesReceived = 0;
                    continue;
                }

                // 检查下载成功的文件
                LogUtil.GetInstance().WriteGame( "<Download MD5 Check>" + "[path]:" + curr_updateFile );
                string curr_md5 = Utils.md5file( curr_updateFile );
                if ( updateInfo.Value.md5.Equals( curr_md5 ) )
                {
                    LogUtil.GetInstance().WriteGame( "<Download MD5 Succesed>" + "[path]:" + curr_updateFile );
                    downloadFinish = true;
                }
                else
                {
                    // 下载失败，重新下载               
                    LogUtil.GetInstance().WriteGame( "<Download Failed>" + "[path]:" + curr_updateFile + "[web]:" + updateInfo.Value.md5 + "[local]:" + curr_md5 );
                    m_downloadFiles.Remove( updateInfo.Key );
                    updateInfo.Value.BytesReceived = 0;
                }
            }
        }

        // 更新完毕之后将临时补丁文件夹内容拷贝到正式本地目录
        if ( isNeedUpdate )
        {
            m_uiText.text = Localization.text( 405 );
            LogUtil.GetInstance().WriteGame( "update success" );

            // 拷贝所有文件
            FileUtil.CopyDir( PathUtil.PatchDownloadPath(), PathUtil.LocalPath() );
            Directory.Delete( PathUtil.PatchDownloadPath(), true );

            //  更新程序资源版本文件
            File.Copy( PathUtil.LocalPath() + m_szResverTxt, PathUtil.ConfigPath() + m_szResverTxt, true );

            // 重读本地资源版本文件
            m_pLocalResverIni.OpenFromTXT( PathUtil.ConfigPath() + m_szResverTxt );

        }
        else
        {
            //m_uiText.text = Localization.text( 403 );
            m_uiText.text = "";
            LogUtil.GetInstance().WriteGame( "Has been the latest version" );
        }

        OnInited();
        yield break;
    }

    /// <summary>
    /// 下载文件
    /// </summary>
    void DownloadFile( string url, string file )
    {
        object[] param = new object[2] { url, file };
        ThreadEvent ev = new ThreadEvent();
        ev.Key = NotifyConst.UPDATE_DOWNLOAD;
        ev.evParams.AddRange( param );
        GetComponent<ThreadManager>().AddEvent( ev, OnThreadDownLoadProc );
    }

    /// <summary>
    /// 已经下载文件大小
    /// </summary>
    public long GetDownloadSize()
    {
        long v = 0;
        foreach ( var updateInfo in m_szNeedUpdateFileDict )
        {
            v += updateInfo.Value.BytesReceived;
        }
        return v;
    }

    /// <summary>
    /// 下载过程，线程调用
    /// </summary>
    /// <param name="data"></param>
    void OnThreadDownLoadProc( NotifyData data )
    {
        switch ( data.evName )
        {
            case NotifyConst.UPDATE_PROGRESS:
                // 下载过程
                string file = data.evParam[0].ToString();
                m_downloadspeed = data.evParam[1].ToString();
                m_szNeedUpdateFileDict[file].BytesReceived = Convert.ToInt64( data.evParam[2] );
                m_downloadProgress = String.Format( "{0:F}", GetDownloadSize() / 1024.0 / 1024.0 ) + "MB" + " / " + String.Format( "{0:F}", m_downloadsize_total / 1024.0 / 1024.0 ) + "MB";
                break;
            case NotifyConst.UPDATE_DOWNLOAD:
                // 下载完成之后添加到已下载列表
                LogUtil.GetInstance().WriteGame( "<DownloadCompleted>" + "[name]:" + data.evParam[0].ToString() );
                m_downloadFiles.Add( data.evParam[0].ToString() );
                break;
        }
    }

    // 网络检查
    IEnumerator OnCheckNetwork()
    {
        while ( true )
        {
            // 启动检测网络
            yield return new WaitForSeconds( 2 );
            //if ( Application.internetReachability == NetworkReachability.NotReachable )
            //{
            //    // 开启网络不通提示框
            //    m_uiNetworkBox.SetActive( true );
            //}
        }
    }

    // 按钮事件-退出
    public void ExitClicked()
    {
        Application.Quit();
    }

    // 按钮事件-立即更新
    public void UpdateClicked()
    {
        m_AskSureUpdate = false;
        m_uiWarningBox.SetActive( false );
    }

    // 按钮事件-网络重试
    public void NetworkRetry()
    {
        m_uiNetworkBox.SetActive( false );
        Begin();
    }

    // 按钮事件-去市场
    public void MarketClicked()
    {
        Utils.GotoMarket();
    }

    /// <summary>
    /// 包初始化完毕-启动游戏主场景
    /// </summary>
    public void OnInited()
    {
        // 进游戏之前再进行一次检测网络
        if ( Application.internetReachability == NetworkReachability.NotReachable )
        {
            // 开启网络状态提示框
            m_uiNetworkBox.SetActive( true );
            return;
        }
        LogUtil.GetInstance().WriteGame( "------------------ Launcher OnInited ------------------" );
        StartCoroutine( loadScene() );
    }

    IEnumerator loadScene()
    {
        // 异步读取场景
        async = SceneManager.LoadSceneAsync( "game" );
        yield return async;
    }
}
