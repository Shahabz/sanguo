-- 通用短消息发送
ASKINFO_NORMAL		= 	0;
ASKINFO_FIGHTMATCH		= 	10;
ASKINFO_FIGHTROOMQUIT 	= 11;

function system_askinfo( msgid, msg, ... )
	-- 未进入游戏不允许发送通用短协议
	if Const.NetStatus < 3 then
		return
	end
	-- m_msgid=0,m_valuenum=0,m_value={}[m_valuenum],m_msg_length=0,m_msg="[m_msg_length]",
	local arg={...}
	local sendValue = {};
	sendValue.m_msgid = msgid;
	sendValue.m_msg = msg;
	sendValue.m_msg_length = string.len( sendValue.m_msg );
	sendValue.m_valuenum = #arg;
	sendValue.m_value={};
	for i,v in ipairs(arg) do
		sendValue.m_value[i] = v;
   	end	
	netsend_askinfo_C( sendValue );
end



-- 订阅协议类型
-- 订阅一个消息，服务器只给订阅的用户发送消息
function SubscribeCmd( cmd )
	system_askinfo( ASKINFO_SUBSCRIBE, "", 1, cmd );
end

-- 取消订阅一个消息
function SubscribeCmdCancel( cmd )
	system_askinfo( ASKINFO_SUBSCRIBE, "", 2, cmd );
end
