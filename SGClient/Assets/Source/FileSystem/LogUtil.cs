using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

/// <summary>
/// 日志系统
/// </summary>
public class LogUtil
{
    private FileStream m_FileStreamGame = null;
    private FileStream m_FileStreamNet = null;
    private Dictionary<string, DateTime> m_MarkTimes = new Dictionary<string, DateTime>();

    /// <summary>
    /// 单例
    /// </summary>
    private static LogUtil s_LogLogUtil;

    public static LogUtil GetInstance()
    {
        if (s_LogLogUtil == null)
        {
            s_LogLogUtil = new LogUtil();
        }
        return s_LogLogUtil;
    }

    /// <summary>
    /// 创建日志系统
    /// </summary>
    public bool Create( string path )
    {
#if true
        if ( !Directory.Exists( path ) )
        {
            Directory.CreateDirectory( path );
        }

        DateTime now = DateTime.Now;
        string time = string.Format("{0:D4}_{1:D2}_{2:D2}-{3:D2}_{4:D2}_{5:D2}",
                                    now.Year, now.Month, now.Day, now.Hour, now.Minute, now.Second);
        if ( m_FileStreamGame == null )
             m_FileStreamGame = new FileStream(path + "/" + time + ".game.log", FileMode.Create);
        if ( m_FileStreamNet == null )
             m_FileStreamNet = new FileStream(path + "/" + time + ".net.log", FileMode.Create);
#endif
        return true;
    }

    /// <summary>
    /// 关闭日志系统
    /// </summary>
    public void Close()
    {
        s_LogLogUtil = null;
        if ( m_FileStreamGame != null )
        {
            m_FileStreamGame.Close();
            m_FileStreamGame = null;
        }
        if ( m_FileStreamNet != null )
        {
            m_FileStreamNet.Close();
            m_FileStreamNet = null;
        }
    }

    /// <summary>
    /// 写入游戏逻辑日志
    /// </summary>
    public void WriteGame(string msg)
    {
        if ( m_FileStreamGame == null )
            return;
        DateTime now = DateTime.Now;
        string time = string.Format("[{0:D2}:{1:D2}:{2:D2}]", now.Hour, now.Minute, now.Second);
        byte[] memory = StringUtil.StringToByte(time + msg + "\r\n");
        m_FileStreamGame.Write(memory, 0, memory.Length);
        m_FileStreamGame.Flush();
    }

    /// <summary>
    /// 写入游戏网络日志
    /// </summary>
    public void WriteNet(string msg)
    {
        if ( m_FileStreamNet == null )
            return;
        DateTime now = DateTime.Now;
        string time = string.Format("[{0:D2}:{1:D2}:{2:D2}]", now.Hour, now.Minute, now.Second);
        byte[] memory = StringUtil.StringToByte(time + msg + "\r\n");
        m_FileStreamNet.Write(memory, 0, memory.Length);
        m_FileStreamNet.Flush();
    }

    public void MarkStart(string tag)
    {
        m_MarkTimes.Add(tag, DateTime.Now);

    }

    public void MarkEnd(string tag)
    {
        DateTime start = m_MarkTimes[tag];
        WriteGame(tag + " usetime:" + (DateTime.Now - start).TotalMilliseconds);
        m_MarkTimes.Remove(tag);
    }

};
