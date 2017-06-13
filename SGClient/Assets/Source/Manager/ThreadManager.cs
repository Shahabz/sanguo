using System.Collections;
using System.Threading;
using System.Collections.Generic;
using System.IO;
using System.Diagnostics;
using System.Net;
using System.ComponentModel;
using System;
using UnityEngine;
public class NotifyConst
{
    public const string UPDATE_MESSAGE = "UpdateMessage";           //更新消息
    public const string UPDATE_DOWNLOAD = "UpdateDownload";         //更新下载
    public const string UPDATE_PROGRESS = "UpdateProgress";         //更新进度

    public const string UPLOAD = "UpLoad";                          //上传
    public const string UPLOAD_PROGRESS = "UpLoadProgress";         //上传进度
}

// 线程事件
public class ThreadEvent
{
    public string Key;
    public List<object> evParams = new List<object>();
}

// 线程通知的数据结构
public class NotifyData
{
    public string evName;
    public object[] evParam;
    public NotifyData( string name, object[] param )
    {
        this.evName = name;
        this.evParam = param;
    }
}

/// <summary>
/// 当前线程管理器，同时只能做一个任务
/// </summary>
public class ThreadManager : MonoBehaviour
{
    // 线程
    private Thread m_thread;

    // 通知函数
    private Action<NotifyData> m_func;

    // 记录响应时间
    private Stopwatch m_stopwatch = new Stopwatch();

    // 当前正在下载的文件
    private string m_currDownFile = string.Empty;

    // 下载完之后写入路径
    private string m_DownLoadPath = "";

    // 当前正在上传的文件
    private string m_currUploadFile = string.Empty;

    // 事件队列
    static readonly object m_lockObj = new object();
    static Queue<ThreadEvent> m_events = new Queue<ThreadEvent>();

    // 代理
    delegate void ThreadSyncEvent( NotifyData data );
    private ThreadSyncEvent m_SyncEvent;

    public void Awake()
    {
        m_DownLoadPath = PathUtil.PatchDownloadPath();
        m_SyncEvent = OnSyncEvent;
        m_thread = new Thread( OnUpdate );
    }

    // Use this for initialization
    public void Start()
    {
        m_thread.Start();
    }

	/// <summary>
    /// 添加到事件队列
    /// </summary>
    public void AddEvent( ThreadEvent ev, Action<NotifyData> func )
    {
        lock ( m_lockObj )
        {
            this.m_func = func;
            m_events.Enqueue( ev );
        }
    }

    /// <summary>
    /// 通知事件
    /// </summary>
    /// <param name="state"></param>
    private void OnSyncEvent( NotifyData data )
    {
        if ( this.m_func != null )
           this. m_func( data );  // 回调逻辑层
    }

    // Update is called once per frame
    void OnUpdate()
    {
        while ( true )
        {
            lock ( m_lockObj )
            {
                if ( m_events.Count > 0 )
                {
                    ThreadEvent e = m_events.Dequeue();
                    try
                    {
                        switch ( e.Key )
                        {
                            case NotifyConst.UPDATE_DOWNLOAD:
                                {  //下载文件
                                    OnDownloadFile( e.evParams );
                                }
                                break;
                            case NotifyConst.UPLOAD:
                                { // 上传文件
                                    OnUploadFile( e.evParams );
                                }
                            break;
                        }
                    }
                    catch ( System.Exception ex )
                    {
                        UnityEngine.Debug.LogError( ex.Message );
                    }
                }
            }
            Thread.Sleep( 1 );
        }
    }

    /// <summary>
    /// 下载文件
    /// </summary>
    void OnDownloadFile( List<object> evParams )
    {
        string url = evParams[0].ToString();
        m_currDownFile = evParams[1].ToString();
        LogUtil.GetInstance().WriteGame( "<OnDownloadFile>"+"[url]:"+url + "[file]:"+m_currDownFile );
        using ( WebClient client = new WebClient() )
        {
            m_stopwatch.Start();
            client.DownloadProgressChanged += new DownloadProgressChangedEventHandler( ProgressChanged );
            client.DownloadFileCompleted += new AsyncCompletedEventHandler( DownloadFileCompleted );
            client.DownloadFileAsync( new System.Uri( url+m_currDownFile ), m_DownLoadPath+m_currDownFile );
        }
    }

    /// <summary>
    /// 更新进度
    /// </summary>
    private void ProgressChanged( object sender, DownloadProgressChangedEventArgs e )
    {
        //UnityEngine.Debug.Log(e.ProgressPercentage);

        //UnityEngine.Debug.Log(string.Format("{0} MB's / {1} MB's",
        //    (e.BytesReceived / 1024d / 1024d).ToString("0.00"),
        //    (e.TotalBytesToReceive / 1024d / 1024d).ToString("0.00")));

        //float value = (float)e.ProgressPercentage / 100f;
        string speed = string.Format( "{0} kb/s", ( e.BytesReceived / 1024d / m_stopwatch.Elapsed.TotalSeconds ).ToString( "0.00" ) );
        object[] evParam = { m_currDownFile, speed, e.BytesReceived };
        NotifyData data = new NotifyData( NotifyConst.UPDATE_PROGRESS, evParam );
        if ( m_SyncEvent != null ) 
            m_SyncEvent( data );

        //if ( e.ProgressPercentage == 100 && e.BytesReceived == e.TotalBytesToReceive )
        //{
        //    m_stopwatch.Reset();
        //    data = new NotifyData( NotifyConst.UPDATE_DOWNLOAD, m_currDownFile );
        //    if ( m_SyncEvent != null ) 
        //        m_SyncEvent( data );
        //}
    }

    /// <summary>
    /// 下载完成
    /// </summary>
    private void DownloadFileCompleted( object sender, AsyncCompletedEventArgs e )
    {
        m_stopwatch.Reset();
        // 通知逻辑层
        object[] evParam = { m_currDownFile };
        NotifyData data = new NotifyData( NotifyConst.UPDATE_DOWNLOAD, evParam );
        if ( m_SyncEvent != null )
            m_SyncEvent( data );
    }

     /// <summary>
    /// 上传文件
    /// </summary>
    void OnUploadFile( List<object> evParams )
    {
        string url = evParams[0].ToString();
        m_currUploadFile = evParams[1].ToString();
        LogUtil.GetInstance().WriteGame( "<OnUploadFile>"+"[url]:"+url + "[file]:"+m_currUploadFile );
        using ( WebClient client = new WebClient() )
        {
            m_stopwatch.Start();
            client.UploadProgressChanged += new UploadProgressChangedEventHandler( UploadProgressChanged );
            client.UploadFileCompleted += new UploadFileCompletedEventHandler( UploadFileCompleted );

            //string boundary = DateTime.Now.Ticks.ToString( "X" ); // 随机分隔线
            int pos = m_currUploadFile.LastIndexOf( "\\" );
            string fileName = m_currUploadFile.Substring( pos + 1 );
            //Content - Disposition: form - data; name = " file1 " ; filename = " G:/homepage.txt "
            //client.Headers.Add( "Content-Type", "multipart/form-data;charset=utf-8;boundary=" + boundary);

            client.Headers.Add( "Content-Disposition", string.Format( "form-data;name=\"thefile\";filename=\"{0}\"\r\nContent-Type:application/x-www-form-urlencoded\r\n\r\n", fileName ) );    
            //client.UploadFile( new System.Uri( url ), "POST", m_currUploadFile );
        }
    }

    /// <summary>
    /// 上传进度
    /// </summary>
    private void UploadProgressChanged( object sender, UploadProgressChangedEventArgs e )
    {
        string value = string.Format( "{0} kb/s", ( e.BytesSent / 1024d / m_stopwatch.Elapsed.TotalSeconds ).ToString( "0.00" ) );
        object[] evParam = { value, e.BytesSent };
        NotifyData data = new NotifyData( NotifyConst.UPLOAD_PROGRESS, evParam );
        if ( m_SyncEvent != null ) 
            m_SyncEvent( data );
    }

    /// <summary>
    /// 上传完成
    /// </summary>
    private void UploadFileCompleted( object sender, AsyncCompletedEventArgs e )
    {
        m_stopwatch.Reset();
        // 通知逻辑层
        object[] evParam = { m_currUploadFile };
        NotifyData data = new NotifyData( NotifyConst.UPLOAD, evParam );
        if ( m_SyncEvent != null )
            m_SyncEvent( data );
    }

    /// <summary>
    /// 应用程序退出
    /// </summary>
    void OnDestroy()
    {
        m_events.Clear();
        m_thread.Abort();
    }
}
