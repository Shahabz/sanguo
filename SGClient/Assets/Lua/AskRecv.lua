-- 通用短消息接收
NOTIFY_NORMAL = 0	-- 缺省文字提示，服务端发文字

-- 处理接收到的消息
function RecvActorNotify(recvValue)
    local msgid = recvValue.m_msgid;
    local value = recvValue.m_value;
    local msg = recvValue.m_msg;
    
    if msgid == NOTIFY_NORMAL then
		
    end
end
