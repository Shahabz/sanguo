-- 脚本网络部分
Network = {};
local this = Network;

-- 本地协议
Connect           = -101;	--连接服务器
ConnectFail		  = -102;	--连接服务器失败
Exception         = -103;	--异常掉线
Disconnect        = -104;	--正常断线

-- 当前网络场景
-- 0未联网状态 
-- 1已经与服务器建立连接
-- 2已接收加密秘钥等信息
-- 3已经进入游戏
Const.NetStatus = 0;

-- 登陆排队人数
Network.LoginUserNum = 0;

-- 第一次连接返回的数据
local m_recv_key = 0;
local m_send_key = 0;
local m_code_len = 0;
local m_value1 = 0;
local m_value2 = 0;
local m_value3 = 0;
local m_value4 = 0;

-- 处理逻辑消息总入口 --
function Network.OnSocket(cmd, buffer)
	netlog("calllua cmd="..cmd);
	if buffer == nil then
		netlog("buffer == nil Network.OnSocket("..cmd..")");
		return;
	end
	-- 本地发过来的连接成功,断开,异常的消息
	if cmd == Connect then 
		this.OnConnect();
		return;
	
	-- 连接失败
	elseif cmd == ConnectFail then
		this.OnConnectFail( buffer );
		Const.NetStatus = 0;
		return;
	
	-- 连接异常关闭
	elseif cmd == Exception then
		local err = buffer:ReadShort();
		local msg = buffer:ReadString();
		netlog("[OnException] error:"..err.." msg:"..msg);
		
		if Const.NetStatus == 3 or GameManager.restart == true then
			this.OnException();
		end
		Const.NetStatus = 0;
		return;
		
	-- 失去连接
	elseif cmd == Disconnect then
		local err = buffer:ReadShort();
		local msg = buffer:ReadString();
		netlog("[OnDisconnect] error:"..err.." msg:"..msg);
		if Const.NetStatus == 3 then
			this.OnDisconnect();
		end
		Const.NetStatus = 0;
		return;
	end
	
	-- 连接开始接受服务器发过来的版本秘钥等的信息
	if Const.NetStatus == 1 then
		if cmd == 2015 then
			buffer:ReadShort();
			local err_code = buffer:ReadInt();
			local m_send_key = buffer:ReadUInt();
			local m_recv_key = buffer:ReadUInt();
			local m_code_len = buffer:ReadInt();
			local m_value1 = buffer:ReadInt();
			local m_value2 = buffer:ReadInt();
			local m_value3 = buffer:ReadInt();
			local m_value4 = buffer:ReadInt();

			-- 已接收加密秘钥等信息
			Const.NetStatus = 2;

			-- 扔到全局变量
			Const.RecvKey = m_recv_key;
			Const.SendKey = m_send_key;
			
			-- 请求登陆验证
			if Const.platid > 10 then
				LoginModSDKLoginProc();
			else
				LoginModTestLoginProc()
			end
		end
		netlog("Const.NetStatus="..Const.NetStatus);
		return;
	end
	
	-- 普通游戏数据包
	local datasize = buffer:ReadShort();
	if in_proc_command_C( cmd, buffer ) == 0 then
		netlog("cmd not found:"..cmd);
	end

	-- 检查一下后面是否还有小数据包
	local buffersize = buffer:Len() - 4 - datasize;
	while buffersize > 0 do
		local cmd = buffer:ReadShort();
		local datasize = buffer:ReadShort();
		if in_proc_command_C( cmd, buffer ) == 0 then
			netlog("cmd not found:"..cmd);
		end
		buffersize = buffersize - 4 - datasize;
	end
end

-- 当连接建立时 --
function Network.OnConnect()
	gamelog( "Network.OnConnect()" );
    if Const.NetStatus == 0 then
		-- 已建立连接
		Const.NetStatus = 1;
		-- 建立连接后，发送版本信息
		local buffer = ByteBuffer.New();
		local str = Global.GetValue( "GetDeviceDesc" );
		if str == "" then
			--str = DeviceHelper.GetDeviceDesc();
			str = "android"
			Global.AddValue( "GetDeviceDesc", str );
		end
		local str = "";
		local ver = string.split(Application.version, ".");
		if ver[1] == nil then
			buffer:WriteInt(1);
		else
			buffer:WriteInt(tonumber(ver[1]));
		end
		
		if ver[2] == nil then
			buffer:WriteInt(0);
		else
			buffer:WriteInt(tonumber(ver[2]));
		end
		
		if ver[3] == nil then
			buffer:WriteInt(1);
		else
			buffer:WriteInt(tonumber(ver[3]));
		end
		
		-- 系统类型
		buffer:WriteInt( tonumber(Const.sdk_sysType) );
		-- 平台
		buffer:WriteShort( Const.platid );
		-- 国籍
		buffer:WriteShort( Localization.countryID( Localization.currentCountry ) );
		-- 语言
		buffer:WriteShort( Localization.languageCustomID( Localization.currentLanguage ) );
		-- 无用
		buffer:WriteInt(0);
		-- 渠道信息
		if Const.sdk_channelId == "" then
			Const.sdk_channelId = 0;
		end
		buffer:WriteInt( tonumber(Const.sdk_channelId) );
		-- 详细设备信息
		buffer:WriteString( str.."|"..Const.sdk_channelId );
		-- 发送首包
		eye.networkManager:SendMessage(buffer);
    end
end

-- 连接服务器失败 -- 
function Network.OnConnectFail( buffer )
	local err = buffer:ReadShort();
	local msg = buffer:ReadString();
	netlog("[OnConnectFail] error:"..err.." msg:"..msg);
	
	if Const.NetStatus < 2 then
--[[		if Const.platid > 10 then
			LoginModOpenSDKLoginActive( true );
		else
			LoginModOpenTestLogin();
		end--]]
		LoginModOpenTestLogin()
	end

	Const.NetStatus = 0;	
	LoginModLoginQueue( false );
	LoginModWarning( T(501) );
end

-- 异常断线，需重连 
-- 向socket写入数据异常
function Network.OnException() 
	if Const.NetStatus > 2 or GameManager.restart == true then
		eye.uiManager:Clear();
		LoginModDestroy();
		SceneManager.LoadScene( "launcher" );
	end
	Const.NetStatus = 0;
	
--[[    Alert( GetLocalizeText(502), GetLocalizeText(503), function()
		fruit.networkManager:SendConnect();
		WaitDlgOpen();
	end )--]]
end

-- 连接中断，或者被踢掉 --
-- 服务器发过来的包大小不对
function Network.OnDisconnect()
	if Const.NetStatus > 2 then
		eye.uiManager:Clear();
		LoginModDestroy();
		SceneManager.LoadScene( "launcher" );
	end
	Const.NetStatus = 0;
	
--[[	Alert( GetLocalizeText(502), GetLocalizeText(503), function()
		fruit.networkManager:SendConnect();
		WaitDlgOpen();
	end )--]]
end

-- 连接服务器
function Network.SDKConnectServer( host, port )
	Const.SocketAddress = host;
	Const.SocketPort = port;
	eye.networkManager:SendConnect();
end

-- 检测收发包时间
function Network.Ping( state )
	-- 正常状态发ping包
	if state == 0 then
		--[[if Const.NetStatus < 3 then
			if Novice_IsIn() == true then
				if MainDlgPing() then
					MainDlgPing().transform:GetComponent( "ShowPing" ):Response();
				end
			end
			return -1;
		end
		if fruit.scenceManager.cityScence == nil then
			return -1;
		end
		local sendValue = {};
		sendValue.m_time = 0;
		netsend_pingrequest_C( sendValue );--]]

	-- 关闭网络不好提醒	
	elseif state == 1 then
		--WaitDlgClose();
		
	-- 显示网络不好提醒	
	elseif state == 2 then
		--WaitDlgOpen();
	end
	return 0;
end

