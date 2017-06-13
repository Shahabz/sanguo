using UnityEngine;
using System;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Collections;
using System.Collections.Generic;
using LuaInterface;

public enum NetActionType {
    Connect,
    Message,
    Logout
}

public enum Protocal {
    Connect     = -101,     //连接服务器
    ConnectFail = -102,     //服务器连接失败
    Exception   = -103,     //异常掉线
    Disconnect  = -104,     //正常断线   
}

public class SocketClient : MonoBehaviour {
    private TcpClient client = null;
    private NetworkStream outStream = null;
    private MemoryStream memStream;
    private BinaryReader reader;
    private const int MAX_READ = 8192;
    private byte[] byteBuffer = new byte[MAX_READ];
    private int recvTotalBytes = 0;
    public static bool loggedIn = false;
    private static Queue<KeyValuePair<NetActionType, ByteBuffer>> _events = new Queue<KeyValuePair<NetActionType, ByteBuffer>>();
	private bool firstconnect = true;
	private bool firstkey = false;
    // Use this for initialization
    void Awake() {
        memStream = new MemoryStream();
        reader = new BinaryReader(memStream);
    }
    
    void OnDestroy() {
    	Debug.Log("~socket OnDestroy");
    	Close();
    }

    /// <summary>
    /// 消息循环
    /// </summary>
    void Update() 
    {
        while (_events.Count > 0) 
        {
            KeyValuePair<NetActionType, ByteBuffer> _event = _events.Dequeue();
            switch (_event.Key) 
            {
                case NetActionType.Connect:
                    LogUtil.GetInstance().WriteNet("Connect:" + Const.SocketAddress + " " + Const.SocketPort);
                    ConnectServer(Const.SocketAddress, Const.SocketPort);
                	break;
                case NetActionType.Message: 
                    SessionSend(_event.Value.ToBytes());
                	break;
                case NetActionType.Logout: 
                	Close();
					LogUtil.GetInstance().WriteNet("Logout");

                	break;
            }
            if (_event.Value != null) 
            	_event.Value.Close();
        }
    }

    /// <summary>
    /// 连接服务器
    /// </summary>
    void ConnectServer( string host, int port ) 
    {
        //if( RecUtil.GetInstance().IsRec() )
        //{
            String newServerIp = "";
            AddressFamily newAddressFamily = AddressFamily.InterNetwork;
            IPv6SupportMidleware.getIPType( host, port.ToString(), out newServerIp, out newAddressFamily );
            if ( !string.IsNullOrEmpty( newServerIp ) ) 
            { 
                host = newServerIp; 
            }
            client = null;
            client = new TcpClient( newAddressFamily );
            client.SendTimeout = 60000;
            client.ReceiveTimeout = 60000;
            client.NoDelay = true;

            try
            {
                client.BeginConnect( host, port, new AsyncCallback(OnConnect), null );
            }
            catch (Exception e)
            {
                OnDisconnected(Protocal.ConnectFail, -1000, e.Message);
            }
        //}
    }

    /// <summary>
    /// 连接上服务器
    /// </summary>
    void OnConnect(IAsyncResult asr) 
    {
        try 
        {
            Const.RecvKey = 0;
            Const.SendKey = 0;
			firstconnect = true;
			firstkey = false;
			outStream = client.GetStream();
            outStream.BeginRead(byteBuffer, 0, MAX_READ, new AsyncCallback(OnRead), null);
            NetworkManager.AddEvent((int)Protocal.Connect, new ByteBuffer());
        }
        catch ( Exception e )
        {
            OnDisconnected(Protocal.ConnectFail, -1001, e.Message); 
        }
   
    }

    /// <summary>
    /// 写数据
    /// </summary>
    void WriteMessage(byte[] message) 
    {
        MemoryStream ms = null;
        using (ms = new MemoryStream()) 
        {
            ms.Position = 0;
            BinaryWriter writer = new BinaryWriter( ms );
            ushort msglen = (ushort)message.Length;
            writer.Write( msglen );
            writer.Write( message );
            writer.Flush();
            if (client != null && client.Connected) 
            {
                //NetworkStream stream = client.GetStream(); 
                byte[] payload = ms.ToArray(); 
				#if true
				if( firstconnect )
				{
					outStream.BeginWrite(payload, 0, payload.Length, new AsyncCallback(OnWrite), null);
					firstconnect = false;
				}
				else
				{
					if( !firstkey )
					{
						LuaDLL.netcode_setkey( Const.SendKey, Const.RecvKey );
						firstkey = true;
					}
					byte[] truemsg = new byte[payload.Length];
					LuaDLL.netcode_encode( truemsg, payload, payload.Length );
					outStream.BeginWrite(truemsg, 0, payload.Length, new AsyncCallback(OnWrite), null);

				}
				#else
				outStream.BeginWrite(payload, 0, payload.Length, new AsyncCallback(OnWrite), null);
				#endif
            } 
            else 
            {
				LogUtil.GetInstance().WriteNet("WriteMessage Fail");
                OnDisconnected(Protocal.Exception, -2003, "WriteMessage Fail"); 
            }
        }
    }

    /// <summary>
    /// 读取消息
    /// </summary>
    void OnRead(IAsyncResult asr) 
    {
        int bytesRead = 0;
        try 
        {
            lock (client.GetStream()) 
            {	//读取字节流到缓冲区
                bytesRead = client.GetStream().EndRead(asr);
            }
            if (bytesRead < 1) 
            {	//包尺寸有问题，断线处理
                OnDisconnected( Protocal.Disconnect, -2000, "bytesRead < 1" ); 
                return;
            }
            //分析数据包内容，抛给逻辑层
            OnReceive(byteBuffer, bytesRead);
            
            lock (client.GetStream()) 
            {	//分析完，再次监听服务器发过来的新消息
                Array.Clear(byteBuffer, 0, byteBuffer.Length);   //清空数组
                client.GetStream().BeginRead(byteBuffer, 0, MAX_READ, new AsyncCallback(OnRead), null);
            }
        } 
        catch (Exception e) 
        {
            //PrintBytes();
            OnDisconnected(Protocal.Exception, -2001, e.Message); 
        }
    }

    /// <summary>
    /// 丢失链接
    /// </summary>
    void OnDisconnected( Protocal protocal, short error, string msg )
    {
        Close();
        ByteBuffer buffer = new ByteBuffer();
        buffer.WriteShort(error);
        buffer.WriteString(msg);
        NetworkManager.AddEvent((int)protocal, new ByteBuffer(buffer.ToBytes()));
        buffer.Close();
    }

    /// <summary>
    /// 打印字节
    /// </summary>
    /// <param name="bytes"></param>
    void PrintBytes() 
    { 
        string returnStr = string.Empty; 
        for (int i = 0; i < byteBuffer.Length; i++) 
        {
            returnStr += byteBuffer[i].ToString("X2"); 
        }
        Debug.LogError(returnStr);
    }

    /// <summary>
    /// 向链接写入数据流
    /// </summary>
    void OnWrite(IAsyncResult r) 
    {
        try 
        {
            outStream.EndWrite(r);
        } 
        catch (Exception e) 
        {
            OnDisconnected(Protocal.Exception, -2002, e.Message); 
        }
    }
    /// <summary>
    /// 接收到消息
    /// </summary>
    void OnReceive(byte[] bytes, int length) 
    {
        memStream.Seek(0, SeekOrigin.End);
        memStream.Write(bytes, 0, length);
        memStream.Seek(0, SeekOrigin.Begin);

		//Debug.Log("xxx:"+Convert.ToString(length)+" yyy:"+Convert.ToString( Const.NetStatus ) );
        
        //PrintBytes();
        while (RemainingBytes() > 2)
        {
            ushort messageLen = 0;
			#if true
			if ( Const.NetStatus <= 1 )
			{
				messageLen = reader.ReadUInt16();
			}
			else
			{
				if( !firstkey )
				{
					LuaDLL.netcode_setkey( Const.SendKey,  Const.RecvKey );
					firstkey = true;
				}
				messageLen = (ushort) LuaDLL.netcode_decode_size( reader.ReadInt16() );
			}
			#else
			{
				messageLen = reader.ReadUInt16();
                Debug.Log( "messageLen:" + messageLen );
			}
			#endif

            if (RemainingBytes() >= messageLen)
            {
                MemoryStream ms = new MemoryStream();
                BinaryWriter writer = new BinaryWriter(ms);
				#if true
				if ( Const.NetStatus <= 1 )
				{
					writer.Write(reader.ReadBytes(messageLen));
				}
				else
				{
                    LuaDLL.netcode_getrecvkey();
					byte[] truemsg = new byte[messageLen];
					LuaDLL.netcode_decode( truemsg, reader.ReadBytes(messageLen), messageLen );
					writer.Write(truemsg);
				}
				#else
				writer.Write(reader.ReadBytes(messageLen));
				#endif
                ms.Seek(0, SeekOrigin.Begin);
                OnReceivedMessage(ms);
                //recvTotalBytes += messageLen;
                //Debug.Log( "recvTotalBytes:" + (recvTotalBytes/1024.0).ToString( "0.00" ) + "KB" );
            } 
            else 
            {
                //Back up the position two bytes
                memStream.Position = memStream.Position - 2;
                break;
            }
        }
        
        //Create a new stream with any leftover bytes
        byte[] leftover = reader.ReadBytes((int)RemainingBytes());
        memStream.SetLength(0);     //Clear
        memStream.Write(leftover, 0, leftover.Length);
    }

    /// <summary>
    /// 剩余的字节
    /// </summary>
    private long RemainingBytes() 
    {
        return memStream.Length - memStream.Position;
    }

    /// <summary>
    /// 接收到消息
    /// </summary>
    /// <param name="ms"></param>
    void OnReceivedMessage(MemoryStream ms) 
    {
        BinaryReader r = new BinaryReader(ms);
        byte[] message = r.ReadBytes((int)(ms.Length - ms.Position));

        ByteBuffer buffer = new ByteBuffer(message);
        int commandId = buffer.ReadShort();
        NetworkManager.AddEvent(commandId, buffer);
    }


    /// <summary>
    /// 会话发送
    /// </summary>
    void SessionSend(byte[] bytes) 
    {
		//if( RecUtil.GetInstance().IsRec())
		//{
			WriteMessage(bytes);
		//}
    }

    /// <summary>
    /// 关闭链接
    /// </summary>
    void Close()
    { 
        if (client != null) 
        {
            if (client.Connected) 
            {
            	Debug.Log("client.Close");
            	client.Close();
            }
            client = null;
            OnDisconnected( Protocal.Disconnect, -2005, "client.Close" ); 
        }
        loggedIn = false;
        Const.RecvKey = 0;
        Const.SendKey = 0;
    }

    /// <summary>
    /// 登出
    /// </summary>
    public static void Logout() 
    { 
        _events.Enqueue(new KeyValuePair<NetActionType, ByteBuffer>(NetActionType.Logout, null));
    }

    /// <summary>
    /// 发送连接请求
    /// </summary>
    public static void SendConnect() 
    {
        _events.Enqueue(new KeyValuePair<NetActionType, ByteBuffer>(NetActionType.Connect, null));
    }

    /// <summary>
    /// 发送消息
    /// </summary>
    public static void SendMessage(ByteBuffer buffer) 
    {
        _events.Enqueue(new KeyValuePair<NetActionType, ByteBuffer>(NetActionType.Message, buffer));
    }

    public static string GetLocalIPv4()
    {
        try
        {
            string HostName = Dns.GetHostName(); //得到主机名  
            IPHostEntry IpEntry = Dns.GetHostEntry( HostName );
            for ( int i = 0; i < IpEntry.AddressList.Length; i++ )
            {
                //从IP地址列表中筛选出IPv4类型的IP地址  
                //AddressFamily.InterNetwork表示此IP为IPv4,  
                //AddressFamily.InterNetworkV6表示此地址为IPv6类型  
                if ( IpEntry.AddressList[i].AddressFamily == AddressFamily.InterNetwork )
                {
                    return IpEntry.AddressList[i].ToString();
                }
            }
            return "";
        }
        catch ( Exception ex )
        {
            return "";
        }
    }  
}
