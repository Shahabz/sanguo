-- 通用短消息接收
NOTIFY_NORMAL 		= 	0	-- 缺省文字提示，服务端发文字
NOTIFY_TEXTTABLE	=	1	-- 
NOTIFY_ITEM			=	2
NOTIFY_ITEMNUM		=	3
NOTIFY_LOGIN_QUEUE	=	4
NOTIFY_CHANGESHAPE	=	5	-- 玩家头像修改
NOTIFY_CHANGENAME	=	6	-- 玩家修改名称
NOTIFY_NPCTALK		=	7	-- NPC对话
NOTIFY_EQUIP		=	8	-- 装备

-- 处理接收到的消息
function RecvActorNotify(recvValue)
    local msgid = recvValue.m_msgid;
    local value = recvValue.m_value;
    local msg = recvValue.m_msg;
    
    if msgid == NOTIFY_NORMAL then
		
	-- 头像改变
	elseif msgid == NOTIFY_CHANGESHAPE then
		GetPlayer().m_shape = value[2];
		MainDlgSetHead();
		PlayerDlgSet();
	
	-- NPC对话	
	elseif msgid == NOTIFY_NPCTALK then
		if BuildingGetDlgIsShow() == true then
			BuildingGetDlgWait( NpcTalkID, value[1] )
		else
			NpcTalkID( value[1] )
		end
	
	-- 装备	
	elseif msgid == NOTIFY_EQUIP then
		if value[1] == 0 then
			GetPlayer().m_equipext = value[2];
			BagDlgEquipExtSet();
		end
    end
end
