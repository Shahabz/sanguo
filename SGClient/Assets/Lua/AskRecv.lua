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
NOTIFY_ACTOR		=	9	-- 角色
NOTIFY_ACTORSFLAG	=	10	-- 角色标志位
NOTIFY_WORLDMAP		=	11	-- 世界地图
NOTIFY_VALUECHANGE	=	12	-- 值改变
NOTIFY_MAIL			=	13	-- 邮件
NOTIFY_ALERT		=	14	-- 消息确定框

-- 处理接收到的消息
function RecvActorNotify(recvValue)
    local msgid = recvValue.m_msgid;
    local value = recvValue.m_value;
    local msg = recvValue.m_msg;
    
    if msgid == NOTIFY_NORMAL then
		pop(T(value[1]))
		
	-- 购买并使用道具
	elseif msgid == NOTIFY_ITEM then
		if value[1] == 0 then
			pop( T(753)..": "..item_getname(value[2]).."x"..value[3] )
		elseif value[1] == 1 then
			-- 道具加速
			if value[2] > 0 then
				pop( T(759)..zhtime(value[2]) )
				QuickItemDlgSet( value[2], 1 )
			else
				QuickItemDlgClose();
				TrainDlgClose();
			end
		end
	
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
	
	-- 角色	
	elseif msgid == NOTIFY_ACTOR then
		-- 购买体力询问
		if value[1] == 0 then
			MsgBox( F( 783, value[2] ), function()
				system_askinfo( ASKINFO_ACTOR, "", 0, 0 );
			end )
		
		-- 次数用尽
		elseif value[1] == 1 then
			AlertMsg( T(784) )
	
		end
	
	-- 角色标志位	
	elseif msgid == NOTIFY_ACTORSFLAG then
		GetPlayer().m_actor_sflag = value[1]
	
	-- 世界地图	
	elseif msgid == NOTIFY_WORLDMAP then
		 -- 迁城完毕
        if value[1] == 1 then
            WorldMap.OnCityMoved(value[2], value[3], value[4]);
		-- 更新击杀级别
		elseif value[1] == 2 then
			GetPlayer().m_mokilllv = value[2];
		end
	
	-- 值改变	
	elseif msgid == NOTIFY_VALUECHANGE then
		-- 洗髓次数
		if value[1] == 1 then
			pop( T(120)..": "..T(1100).."x"..value[2] ) 
		-- 洗练次数
		elseif value[1] == 2 then
			pop( T(120)..": "..T(1101).."x"..value[2] ) 
		end
	
	-- 邮件	
	elseif msgid == NOTIFY_MAIL then
		-- 邮件数量
		if value[1] == 1 then
			MainDlgSetMailNum( value[2] )
		-- 操作返回
		elseif value[1] == 2 then
			if value[2] == 0 or value[2] == 1 then
				MailDlgMailRecvOver( value[2], value[3] );
			end
		end
		
	-- 消息确定框
	elseif msgid == NOTIFY_ALERT then
		AlertMsg( T(value[1]) )
    end
end
