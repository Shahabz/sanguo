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
NOTIFY_LOSTREBUILD	=	15	-- 高级重建次数
NOTIFY_MAPZONEGOZC	=	16	-- 前往州城的显示和隐藏
NOTIFY_MSGBOX		=	17	-- 弹出消息选择框
NOTIFY_BOX			=	18	-- 弹出消息，带框
NOTIFY_POP			=	19	-- 弹出消息，无框
NOTIFY_BUILDINGFINISH = 20  -- 建筑完成
NOTIFY_CITYGUARDNUM	  = 21	-- 城防军数量
NOTIFY_HERO_VISIT	  =	22	-- 武将寻访
NOTIFY_MAPCALL		  =	23	-- 地图召唤
NOTIFY_HEROBACK		  = 24	-- 武将回城
NOTIFY_HEROGOD		  =	25	-- 神突成功
NOTIFY_ACTIVITY		  =	26	-- 活动
NOTIFY_GUIDE		  = 27  -- 指引
NOTIFY_INVITECODE	  =	28	-- 邀请码

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
			local kind = value[2]
			local num = value[3]
			local color = item_getcolor(kind)
			local itemname = NameColorText( item_getname(kind), color )
			local msg = T(753)..": "..itemname.."x"..num
			pop( msg )
		elseif value[1] == 1 then
			-- 道具加速
			if value[2] > 0 then
				NotifyMiddle( T(759)..NameColorText(zhtime(value[2]),1), nil )
				--pop( T(759)..zhtime(value[2]) )
				QuickItemDlgSet( value[2], 1 )
			else
				QuickItemDlgClose();
				TrainDlgClose();
			end
		end
	
	-- 头像改变
	elseif msgid == NOTIFY_CHANGESHAPE then
		if value[1] == 0 then
			GetPlayer().m_shape = value[2];
			MainDlgSetHead();
			PlayerDlgSet();
		elseif value[1] == 1 then
			GetPlayer().m_shape_bag	= value[2];
		end
	
	-- NPC对话	
	elseif msgid == NOTIFY_NPCTALK then
		if BuildingGetDlgIsShow() == true then
			BuildingGetDlgWait( NpcTalk, {5002, Localization.text_quest(value[1]), nil, nil} )
		else
			NpcTalk( {5002, Localization.text_quest(value[1]), nil, nil} )
		end
	
	-- 装备	
	elseif msgid == NOTIFY_EQUIP then
		if value[1] == 0 then
			GetPlayer().m_equipext = value[2];
			EquipDlgEquipExtSet();
		end
	
	-- 角色	
	elseif msgid == NOTIFY_ACTOR then
		-- 购买体力询问
		if value[1] == 0 then
			MsgBox( F( 783, value[3], value[4], value[2] ), function()
				system_askinfo( ASKINFO_ACTOR, "", 0, 0 );
			end )
		
		-- 次数用尽
		elseif value[1] == 1 then
			AlertMsg( T(784) )
		
		-- 购买资源
		elseif value[1] == 2 then
			JumpDlgMsgBox( value[2], value[3], value[4] )
		end
	
	-- 角色标志位	
	elseif msgid == NOTIFY_ACTORSFLAG then
		GetPlayer().m_actor_sflag = value[1]
	
	-- 世界地图	
	elseif msgid == NOTIFY_WORLDMAP then
		 -- 迁城完毕
        if value[1] == 1 then
            WorldMap.OnCityMoved(value[2], value[3], value[4]);
			GetPlayer().m_zone = value[7]
			
		-- 更新击杀级别
		elseif value[1] == 2 then
			local oldlevel = GetPlayer().m_mokilllv;
			GetPlayer().m_mokilllv = value[2];
			-- 更新周边野怪
			if oldlevel ~= GetPlayer().m_mokilllv then
				if GameManager.currentScence == "worldmap" then
					MapUnit.RefreshEnemyLevel( GetPlayer().m_mokilllv )
				end
			end
		end
	
	-- 值改变	
	elseif msgid == NOTIFY_VALUECHANGE then
		-- 洗髓次数
		if value[1] == 1 then
			--pop( T(120)..": "..T(1100).."x"..value[2] ) 
			AwardNotify( AWARDKIND_HERO_WASH, value[2] )
			
		-- 洗练次数
		elseif value[1] == 2 then
			--pop( T(120)..": "..T(1101).."x"..value[2] ) 
			AwardNotify( AWARDKIND_EQUIP_WASH, value[2] )
			
		-- 爵位变化
		elseif value[1] == 3 then
			GetPlayer().m_place = value[2]
			NationDlgChangePlace();
			NationPlaceDlgRecv()
		-- 官职变化
		elseif value[1] == 4 then
			GetPlayer().m_official = value[2]
			NationDlgChangeOfficial()
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
		-- 
		elseif value[1] == 3 then
			GetMail().m_bLoadNew = false
		end
		
	-- 消息确定框
	elseif msgid == NOTIFY_ALERT then
		if recvValue.m_msg_length > 1 then
			local json = require "cjson"
			local msgjson = json.decode( recvValue.m_msg );
			local msg = "";
			if msgjson["v1"] ~= nil and msgjson["v2"] ~= nil then
				local v1_str = GetMail():GetString( msgjson["v1"] );
				local v2_str = GetMail():GetString( msgjson["v2"] );
				msg = F( value[1], v1_str, v2_str )
			
			elseif msgjson["v1"] ~= nil then
				local v1_str = GetMail():GetString( msgjson["v1"] );
				msg = F( value[1], v1_str )
				
			else
				msg = T(value[1])
			end
			AlertMsg( msg )
		else
			AlertMsg( T(value[1]) )
		end
	
	-- 重建家园次数	
	elseif msgid == NOTIFY_LOSTREBUILD then
		if value[1] == 0 then
			StoreDlgRecvValue( value[2] )
		end
	
	-- 前往州城的显示和隐藏	
	elseif msgid == NOTIFY_MAPZONEGOZC then
		if value[1] == 0 then
			MapMainDlgHideGotoZone()
		elseif value[1] == 1 then	
			MapMainDlgShowGotoZone()
		end
	
	-- 弹出消息选择框
	elseif msgid == NOTIFY_MSGBOX then
		local json = require "cjson"
		local msgjson = json.decode( recvValue.m_msg );
		local msg = "";
		if msgjson["v1"] ~= nil and msgjson["v2"] ~= nil then
			local text = GetMail():GetString( msgjson["textid"] );
			local v1_str = GetMail():GetString( msgjson["v1"] );
			local v2_str = GetMail():GetString( msgjson["v2"] );
			msg = FF( text, v1_str, v2_str )
		
		elseif msgjson["v1"] ~= nil then
			local text = GetMail():GetString( msgjson["textid"] );
			local v1_str = GetMail():GetString( msgjson["v1"] );
			msg = FF( text, v1_str )
			
		else
			local text = GetMail():GetString( msgjson["textid"] );
			msg = text;
		end
		MsgBox( msg, function() 
			if value[1] > 0 then
				system_askinfo( ASKINFO_MSGBOX_CALLBACK, "", value[1], value[2], value[3] )
			end
		end )
	
	-- 弹出消息，带框	
	elseif msgid == NOTIFY_BOX then
		if recvValue.m_msg_length > 1 then
			local json = require "cjson"
			local msgjson = json.decode( recvValue.m_msg );
			local msg = "";
			if msgjson["v1"] ~= nil and msgjson["v2"] ~= nil then
				local v1_str = GetMail():GetString( msgjson["v1"] );
				local v2_str = GetMail():GetString( msgjson["v2"] );
				msg = F( value[1], v1_str, v2_str )
			
			elseif msgjson["v1"] ~= nil then
				local v1_str = GetMail():GetString( msgjson["v1"] );
				msg = F( value[1], v1_str )
				
			else
				msg = T(value[1])
			end
			NotifyMiddle( msg, nil )
		else
			NotifyMiddle( msg )
		end
		
	-- 弹出消息，无框
	elseif msgid == NOTIFY_POP then
		if recvValue.m_msg_length > 1 then
			local json = require "cjson"
			local msgjson = json.decode( recvValue.m_msg );
			local msg = "";
			if msgjson["v1"] ~= nil and msgjson["v2"] ~= nil then
				local v1_str = GetMail():GetString( msgjson["v1"] );
				local v2_str = GetMail():GetString( msgjson["v2"] );
				msg = F( value[1], v1_str, v2_str )
			
			elseif msgjson["v1"] ~= nil then
				local v1_str = GetMail():GetString( msgjson["v1"] );
				msg = F( value[1], v1_str )
				
			else
				msg = T(value[1])
			end
			pop( msg )
		else
			pop( T(value[1]) )
		end
		
	
	-- 建筑完成	
	elseif msgid == NOTIFY_BUILDINGFINISH then
		local op = value[1]
		local kind = value[2]
		local offset = value[3];
		local level = value[4]
		if kind >= BUILDING_Silver and kind <= BUILDING_Iron then
			NotifyMiddle( F(1481,level,offset+1,BuildingName(kind)), {back=LoadSprite("ui_icon_back_8"), shape=BuildingSprite(kind)} )
		else
			NotifyMiddle( F(1480,level,BuildingName(kind)), {back=LoadSprite("ui_icon_back_8"), shape=BuildingSprite(kind)} )
		end
	
	-- 城防军数量	
	elseif msgid == NOTIFY_CITYGUARDNUM then
		GetPlayer().m_guardnum = value[1];
		City.GuardCallMod( nil, true )
	
	-- 武将寻访	
	elseif msgid == NOTIFY_HERO_VISIT then
		if value[1] == 0 then
			if value[2] == 0 then
				City.HeroVisitMod( nil, false, 0 )
			elseif value[2] == 1 then
				City.HeroVisitMod( nil, true, 1 )
			elseif value[2] == 2 then
				City.HeroVisitMod( nil, true, 2 )
			end
		end
		
	-- 地图召唤	
	elseif msgid == NOTIFY_MAPCALL then
		if value[1] == 0 then
			MapCallDlgRecv( value[2], value[3], value[4] )
		end
		
	-- 武将回城
	elseif msgid == NOTIFY_HEROBACK then
		local str = ""
		if value[1] > 0 then
			str = str..HeroName( value[1] )
		end
		if value[2] > 0 then
			str = str.."/"..HeroName( value[2] )
		end
		if value[3] > 0 then
			str = str.."/"..HeroName( value[3] )
		end
		if value[4] > 0 then
			str = str.."/"..HeroName( value[4] )
		end
		pop( F( 983, str ) )
	
	-- 神突成功
	elseif msgid == NOTIFY_HEROGOD then
		if value[1] == 0 then
			
		end
	
	-- 活动	
	elseif msgid == NOTIFY_ACTIVITY then
		if value[1] == ACTIVITY_2 then
			Activity2ModRecv( value[3] )
		elseif value[1] == ACTIVITY_3 then
			Activity3ModRecv( value[3], value[4], value[5], value[6] )
		elseif value[1] == ACTIVITY_6 then
			Activity6ModRecv( value[3], {{n=1,v=value[4]}, {n=2,v=value[5]}, {n=3,v=value[6]}}, value[7] )
		elseif value[1] == ACTIVITY_8 then
			Activity8ModRecv( value[3], value[4] )
		elseif value[1] == ACTIVITY_11 then
			Activity11ModRecv( value[3], value[4] )
		end
	
	-- 触发指引	
	elseif msgid == NOTIFY_GUIDE then
		Guide( value[1], 1, false );
	
	-- 邀请码	
	elseif msgid == NOTIFY_INVITECODE then
		InviteCodeDlgOpRecv( value[1], value[2] )
    end
end


-- UI更新
UI_UPDATE_NATIONFIGHT = 1 -- 国战相关
UI_UPDATE_FIGHTINFO	  = 2 -- 战斗信息变化
UI_UPDATE_NATIONOFFICIAL = 3 -- 官员系统状态变化
UI_UPDATE_NATIONHEROPOS	 = 4 -- 名将位置变化
function dialogupdate_recv( ui )
	if ui == UI_UPDATE_NATIONFIGHT then
		MapMainDlgNationLayerUpdate()
		MapNationFightDlgUpdate()
		NationWarDlgNationUpdate()
		NationWarDlgCityUpdate()
		
	elseif ui == UI_UPDATE_FIGHTINFO then
		MapNationFightDlgUpdate()
		MapArmyGroupDlgUpdate()
		NationWarDlgNationUpdate()
		NationWarDlgCityUpdate()
		
	elseif ui == UI_UPDATE_NATIONOFFICIAL then
		NationOfficialDlgUpdate()
		
	elseif ui == UI_UPDATE_NATIONHEROPOS then
		NationHeroDlgUpdate()
		
	end
end