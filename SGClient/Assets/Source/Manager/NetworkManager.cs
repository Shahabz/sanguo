using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;
using LuaInterface;

public class NetworkManager : MonoBehaviour {
    private static Queue<KeyValuePair<int, ByteBuffer>> sEvents = new Queue<KeyValuePair<int, ByteBuffer>>();
		
	/// <summary>
    /// 启动
    /// </summary>
    void Start()
    {
    }
    
    /// <summary>
    /// 析构函数
    /// </summary>
    void OnDestroy() 
    {
    }
    
    /// <summary>
    /// 向逻辑层消息队列添加消息
    /// </summary>
    public static void AddEvent(int _event, ByteBuffer _data)
    {
        sEvents.Enqueue(new KeyValuePair<int, ByteBuffer>(_event, _data));
		LogUtil.GetInstance().WriteNet("cmd=" + _event + " len=" + _data.Len().ToString() );
        //RecUtil.GetInstance().WriteNetData(_event, _data);
    }
		
	/// <summary>
    /// 每帧遍历逻辑层消息队列
    /// </summary>
    void Update()
    {
        if (sEvents.Count > 0) 
        {
            while (sEvents.Count > 0) 
            {
                KeyValuePair<int, ByteBuffer> _event = sEvents.Dequeue();
                switch (_event.Key) 
                {
                    default:
                        LuaFun.OnSocket.Call( _event.Key, _event.Value );
                    	//CallMethod("OnSocket", _event.Key, _event.Value);
                    break;
                }
            }
        }
    }

    /// <summary>
    /// 发送链接请求
    /// </summary>
    public void SendConnect()
    {
        SocketClient.SendConnect();
    }

    /// <summary>
    /// 发送登出
    /// </summary>
    public void Logout()
    {
        SocketClient.Logout();
    }

    /// <summary>
    /// 发送SOCKET消息
    /// </summary>
    public void SendMessage(ByteBuffer buffer)
    {
        SocketClient.SendMessage(buffer);
    }

    public void SendNetKey( uint SendKey, uint RecvKey )
    {

    }
}
