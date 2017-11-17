-- 界面
local m_Dlg = nil;
local m_uiWeiGroup = nil; --UnityEngine.GameObject
local m_uiShuGroup = nil; --UnityEngine.GameObject
local m_uiWuGroup = nil; --UnityEngine.GameObject
local m_uiPingJin = nil; --UnityEngine.GameObject
local m_uiHanGu = nil; --UnityEngine.GameObject
local m_uiHuLao = nil; --UnityEngine.GameObject
local m_uiLuoYang = nil; --UnityEngine.GameObject
local m_uiLog = nil; --UnityEngine.GameObject
local m_uiInfantryNum = nil; --UnityEngine.GameObject
local m_uiCavalryNum = nil; --UnityEngine.GameObject
local m_uiArcherNum = nil; --UnityEngine.GameObject
local m_uiActivitySec = nil; --UnityEngine.GameObject
local m_uiTitleText = nil; --UnityEngine.GameObject
local m_uiToken = nil; --UnityEngine.GameObject
local m_uiMaskLayer = nil; --UnityEngine.GameObject
local m_uiUIP_Hero = { nil, nil, nil, nil }; --UnityEngine.GameObject
local m_uiWindow = nil; --UnityEngine.GameObject
local m_uiTownButtons = {}; --UnityEngine.GameObject
local m_uiHanGuButtons = nil; --UnityEngine.GameObject
local m_uiHuLaoButtons = nil; --UnityEngine.GameObject
local m_uiLuoYangButtons = nil; --UnityEngine.GameObject
local m_uiToken = nil; --UnityEngine.GameObject
local m_uiChatText = nil; --UnityEngine.GameObject

local m_HeroArmyInfo = { nil, nil, nil, nil }
local m_uiHeroFindIndex = { 1, 1, 1, 1, 1, 1, 1 }
local m_SelectHeroOffset = -1;
local m_LastSelectArmyIndex = -1;
local m_activityinfo = nil;
local m_TownRecvValue = { nil, nil, nil, nil };
local m_HeroRecvValue = nil;
local m_ChatRecvValue = nil;
-- 打开界面
function BloodyBattleDlgOpen()
	m_Dlg = eye.uiManager:Open( "BloodyBattleDlg" );
end

-- 隐藏界面
function BloodyBattleDlgClose()
	if m_Dlg == nil then
		return;
	end
	system_askinfo( ASKINFO_KINGWAR, "", -1 )
	BloodyBattleDlgSelectHero( -1 )
	eye.uiManager:Close( "BloodyBattleDlg" );
end

-- 删除界面
function BloodyBattleDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function BloodyBattleDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            BloodyBattleDlgClose();
		elseif nControlID == -2 then
			BloodyBattleDlgSelectHero( -1 )
			
		-- 血战排行榜
		elseif nControlID == 2 then
		
		-- 聊天
		elseif nControlID == 3 then
			ChatDlgShow();
		
		-- 加入血战
		elseif nControlID == 10 then
			BloodyBattleDlgBattle()
		
		-- 选择英雄
		elseif nControlID >= 100 and nControlID < 200 then
			BloodyBattleDlgSelectHero( nControlID-100 )
		
		-- 前往
		elseif nControlID >= 1001 and nControlID <= 1004 then
			BloodyBattleDlgGoto( nControlID - 1000 )
			
		-- 偷袭
		elseif nControlID >= 1011 and nControlID <= 1014 then
			BloodyBattleDlgSneak( nControlID - 1010 )
			
		-- 单挑
		elseif nControlID >= 1021 and nControlID <= 1024 then
			BloodyBattleDlgPK( nControlID - 1020 )
			
		-- 进攻
		elseif nControlID >= 1031 and nControlID <= 1034 then
			BloodyBattleDlgAttack( nControlID - 1030 )
			
		-- 回防
		elseif nControlID >= 1041 and nControlID <= 1044 then
			BloodyBattleDlgDefense( nControlID - 1040 )
			
        end
	elseif nType == UI_EVENT_TWEENFINISH then
		if nControlID >= 1 and nControlID <= 4 then	
			SetFalse( m_uiUIP_Hero[nControlID].transform:Find("Chat") )
		end
	end
end

-- 载入时调用
function BloodyBattleDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiWeiGroup = objs[0];
	m_uiShuGroup = objs[1];
	m_uiWuGroup = objs[2];
	m_uiPingJin = objs[3];
	m_uiHanGu = objs[4];
	m_uiHuLao = objs[5];
	m_uiLuoYang = objs[6];
	m_uiLog = objs[7];
	m_uiInfantryNum = objs[8];
	m_uiCavalryNum = objs[9];
	m_uiArcherNum = objs[10];
	m_uiActivitySec = objs[11];
	m_uiTitleText = objs[12];
	m_uiToken = objs[13];
	m_uiMaskLayer = objs[14];
	m_uiUIP_Hero[1] = objs[15];
	m_uiUIP_Hero[2] = objs[16];
	m_uiUIP_Hero[3] = objs[17];
	m_uiUIP_Hero[4] = objs[18];
	m_uiWindow = objs[19];
	m_uiTownButtons[4] = objs[20];
	m_uiTownButtons[5] = objs[21];
	m_uiTownButtons[6] = objs[22];
	m_uiTownButtons[7] = objs[23];
	m_uiToken = objs[24];
	m_uiChatText = objs[25];
end

-- 界面初始化时调用
function BloodyBattleDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function BloodyBattleDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function BloodyBattleDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function BloodyBattleDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function BloodyBattleDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function BloodyBattleDlgShow()
	if m_activityinfo == nil then
		return
	end
	if m_activityinfo.m_state ~= 1 then
		AlertMsg( T(1396) )
		return
	end
	BloodyBattleDlgOpen()
	BloodyBattleDlgChangeToken()
	BloodyBattleDlgSetInfantry()
	BloodyBattleDlgSetCavalry()
	BloodyBattleDlgSetArcher()
	BloodyBattleDlgSetChat( m_ChatRecvValue )
	m_SelectHeroOffset = -1;
	if GetPlayer().m_nation == 1 then
		m_uiWeiGroup:GetComponent("UITweenScale"):Play(true);
		SetControlID( m_uiWeiGroup, 10 )
	elseif GetPlayer().m_nation == 2 then
		m_uiShuGroup:GetComponent("UITweenScale"):Play(true);
		SetControlID( m_uiShuGroup, 10 )
	elseif GetPlayer().m_nation == 3 then
		m_uiWuGroup:GetComponent("UITweenScale"):Play(true);
		SetControlID( m_uiWuGroup, 10 )
	end
	system_askinfo( ASKINFO_KINGWAR, "", 0 )
	BloodyBattleDlgHeroRecv( MapMainDlgGetRecvValue() )
end

-- 接收据点信息
-- m_id=0,m_attack_total=0,m_defense_total=0,m_nation=0,
function BloodyBattleDlgTownRecv( recvValue )
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	m_TownRecvValue[recvValue.m_id] = recvValue;
	local obj = 0;
	if recvValue.m_id == 4 then
		obj = m_uiPingJin
	elseif recvValue.m_id == 5 then
		obj = m_uiHanGu
	elseif recvValue.m_id == 6 then
		obj = m_uiHuLao
	elseif recvValue.m_id == 7 then
		obj = m_uiLuoYang
	end
	if recvValue.m_nation > 0 then
		SetImage( obj.transform:Find("Back"), LoadSprite("ui_bloodybattle_back_"..recvValue.m_nation) )
		SetTrue( obj.transform:Find("Back") )
	else
		SetFalse( obj.transform:Find("Back") )
	end
	SetText( obj.transform:Find("AttackHp"), recvValue.m_attack_total );
	SetText( obj.transform:Find("DefenseHp"), recvValue.m_defense_total );
	if recvValue.m_attack_total > 0 and recvValue.m_defense_total > 0 then
		SetTrue( obj.transform:Find("State") )
	else
		SetFalse( obj.transform:Find("State") )
	end
end

-- 接收活动剩余时间
function BloodyBattleDlgRecvLeftStamp( leftstamp )
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	SetTimer( m_uiActivitySec, leftstamp, leftstamp )
end

-- 总损失兵力
function BloodyBattleDlgRecvLostHP( losthp )
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	-- 根据损失兵力计算血战等级
	local level = 1;
	for i=1,#g_kingwar_config-1,1 do
		if losthp < g_kingwar_config[i].exp then
			level = i;
			break;
		end
	end
	SetText( m_uiTitleText, F(1423, level, losthp, g_kingwar_config[level].exp ) );
end

-- 钻石
function BloodyBattleDlgChangeToken()
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	SetRichText( m_uiToken.transform:Find("Text"), "<icon=token>"..GetPlayer().m_token );
end

-- 步兵
function BloodyBattleDlgSetInfantry()
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	SetText( m_uiInfantryNum, GetPlayer().m_infantry_num );
end

-- 骑兵
function BloodyBattleDlgSetCavalry()
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	SetText( m_uiCavalryNum, GetPlayer().m_cavalry_num );
end

-- 弓兵
function BloodyBattleDlgSetArcher()
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	SetText( m_uiArcherNum, GetPlayer().m_archer_num );
end

-- 接收武将信息
-- m_count=0,m_list={m_army_index=0,m_unit_index=0,m_state=0,m_statetime=0,m_stateduration=0,m_action=0,m_to_posx=0,m_to_posy=0,m_herokind={[4]},[m_count]},m_unit_index=0,
function BloodyBattleDlgHeroRecv( recvValue )
	m_HeroRecvValue = recvValue;
	if m_Dlg == nil or IsActive( m_Dlg ) == false or recvValue == nil then
		return;
	end
	for i=1, 4, 1 do
		SetFalse( m_uiUIP_Hero[i] )
	end
	m_uiHeroFindIndex = { 1, 1, 1, 1, 1, 1, 1 }
	for i=1, recvValue.m_count, 1 do
		local _, offset = GetHero():GetIndex( recvValue.m_list[i].m_herokind[1] );
		if offset < 4 then
			BloodyBattleDlgHeroSet( i, offset, recvValue.m_list[i] )
		end
	end
end

-- 更新
function BloodyBattleDlgHeroUpdate( recvValue )
	for i=1, m_HeroRecvValue.m_count, 1 do
		if m_HeroRecvValue.m_list[i].m_army_index == recvValue.m_army_index then
			m_HeroRecvValue.m_list[i] = recvValue;
		end
	end
	BloodyBattleDlgHeroRecv( m_HeroRecvValue )
end

-- 创建一个武将
function BloodyBattleDlgHeroSet( index, offset, info )
	local uiHero = m_uiUIP_Hero[offset+1];
	SetTrue( uiHero )
	SetImage( uiHero.transform:Find("Shape"), HeroHeadSprite( info.m_herokind[1] ) )
	m_HeroArmyInfo[offset+1] = info;
	
	-- 集结点
	if info.m_state == ARMY_STATE_KINGWAR_READY then
		local id = GetPlayer().m_nation
		local findindex = m_uiHeroFindIndex[id]
		local uiBasePos = m_uiWindow.transform:Find( "hero_"..id.."_"..findindex );
		m_uiHeroFindIndex[id] = m_uiHeroFindIndex[id] + 1;
		uiHero.transform.localPosition = Vector3.New( uiBasePos.localPosition.x, uiBasePos.localPosition.y, uiBasePos.localPosition.z );
		SetControlID( uiHero, 100+offset )
		SetText( uiHero.transform:Find("Name"), T(1388) )
		SetTrue( uiHero.transform:Find("Name") )
		SetFalse( uiHero.transform:Find("Timer") )
		SetGray( uiHero.transform:Find("Shape"), false )
		
		-- 整装待发
		--SetTrue( uiHero.transform:Find("Chat") );
		--SetText( uiHero.transform:Find("Chat/Text"), T( 1413 ) );
		
	-- 待战	
	elseif info.m_state == ARMY_STATE_KINGWAR_FIGHT then
		local id = info.m_to_posx
		local findindex = m_uiHeroFindIndex[id]
		local uiBasePos = m_uiWindow.transform:Find( "hero_"..id.."_"..findindex );
		m_uiHeroFindIndex[id] = m_uiHeroFindIndex[id] + 1;
		uiHero.transform.localPosition = Vector3.New( uiBasePos.localPosition.x, uiBasePos.localPosition.y, uiBasePos.localPosition.z );
		SetControlID( uiHero, 100+offset )
		SetText( uiHero.transform:Find("Name"), F(1401, info.m_unit_index) )
		SetTrue( uiHero.transform:Find("Name") )
		SetFalse( uiHero.transform:Find("Timer") )
		SetGray( uiHero.transform:Find("Shape"), false )
		
		-- 显示第{0}位上阵
		if m_LastSelectArmyIndex == info.m_army_index then
			SetTrue( uiHero.transform:Find("Chat") );
			SetText( uiHero.transform:Find("Chat/Text"), F( 1412, info.m_to_posy ) );
			m_LastSelectArmyIndex = -1;
		end
		
	-- 等待救援	
	elseif info.m_state == ARMY_STATE_KINGWAR_WAITSOS then
		local id = info.m_to_posx
		local findindex = m_uiHeroFindIndex[id]
		local uiBasePos = m_uiWindow.transform:Find( "hero_"..id.."_"..findindex );
		m_uiHeroFindIndex[id] = m_uiHeroFindIndex[id] + 1;
		uiHero.transform.localPosition = Vector3.New( uiBasePos.localPosition.x, uiBasePos.localPosition.y, uiBasePos.localPosition.z );
		SetControlID( uiHero, 100+offset )
		SetText( uiHero.transform:Find("Name"), "" )
		SetFalse( uiHero.transform:Find("Name") )
		SetTrue( uiHero.transform:Find("Timer") )
		SetTimer( uiHero.transform:Find("Timer"), info.m_stateduration-info.m_statetime, info.m_stateduration, 0, T(1419) )
		SetGray( uiHero.transform:Find("Shape"), true )
	end
end

-- 选择部队
function BloodyBattleDlgSelectHero( offset )
	if offset == -1 or m_SelectHeroOffset >= 0 then
		m_SelectHeroOffset = -1;
		SetFalse( m_uiMaskLayer )
		for i=1, 4, 1 do
			m_uiUIP_Hero[i].transform:SetSiblingIndex( 100 )
			SetFalse( m_uiUIP_Hero[i].transform:Find("Chat") )
		end
		for id = 4, 7, 1 do
			m_uiTownButtons[id].transform:SetSiblingIndex( 100 )
			for i = 0 ,m_uiTownButtons[id].transform.childCount - 1 do
				SetFalse( m_uiTownButtons[id].transform:GetChild(i).gameObject )
			end
		end
		return;
	end
	
	m_SelectHeroOffset = offset;
	local uiHero = m_uiUIP_Hero[offset+1];
	local heroArmy = m_HeroArmyInfo[offset+1];
	
	-- 改变层级遮罩
	if heroArmy.m_state ~= ARMY_STATE_KINGWAR_WAITSOS then
		SetTrue( m_uiMaskLayer )
		m_uiMaskLayer.transform:SetSiblingIndex( 200 )
		uiHero.transform:SetSiblingIndex( 201 )
	end
	
	-- 准备状态，显示前往
	if heroArmy.m_state == ARMY_STATE_KINGWAR_READY then
		-- 整装待发
		SetTrue( uiHero.transform:Find("Chat") );
		SetText( uiHero.transform:Find("Chat/Text"), T( 1413 ) );
		
		-- 魏国前往平津关，蜀国前往函谷关，吴国虎牢关
		local townid = heroArmy.m_to_posx -- 当前所在集结点
		m_uiTownButtons[townid+3].transform:SetSiblingIndex( 201 )
		if m_TownRecvValue[townid+3].m_nation == GetPlayer().m_nation then
			SetTrue( m_uiTownButtons[townid+3].transform:Find("Goto") );
		else
			SetTrue( m_uiTownButtons[townid+3].transform:Find("Attack") );
		end
	
	-- 战斗排队中	
	elseif heroArmy.m_state == ARMY_STATE_KINGWAR_FIGHT then
		-- 显示第{0}位上阵
		SetTrue( uiHero.transform:Find("Chat") );
		SetText( uiHero.transform:Find("Chat/Text"), F( 1412, heroArmy.m_to_posy ) );
	
		local townid = heroArmy.m_to_posx -- 当前所在据点
		for id = 4, 7, 1 do
			m_uiTownButtons[id].transform:SetSiblingIndex( 201 )
			if id == townid then
				if m_TownRecvValue[id].m_nation ~= GetPlayer().m_nation then
					SetTrue( m_uiTownButtons[id].transform:Find("PK") );
					SetText( m_uiTownButtons[id].transform:Find("PK/Token/Text"), global.kingwar_token_pk )
				end
			else
				if m_TownRecvValue[id].m_nation == GetPlayer().m_nation then
					if m_TownRecvValue[townid].m_attack_total > 0 then -- 战斗状态显示回防
						SetTrue( m_uiTownButtons[id].transform:Find("Defense") );
						SetText( m_uiTownButtons[id].transform:Find("Defense/Token/Text"), global.kingwar_token_def )
					else
						SetTrue( m_uiTownButtons[id].transform:Find("Goto") );
					end
				else
					if m_TownRecvValue[townid].m_attack_total > 0 then -- 战斗状态显示偷袭
						SetTrue( m_uiTownButtons[id].transform:Find("Sneak") );
						SetText( m_uiTownButtons[id].transform:Find("Sneak/Token/Text"), global.kingwar_token_sneak )
					else
						SetTrue( m_uiTownButtons[id].transform:Find("Attack") );
					end
				end
			end
		end
	
	-- 等待救援	
	elseif heroArmy.m_state == ARMY_STATE_KINGWAR_WAITSOS then
		BloodyBattleDlgRebirth()
	end
end

-- 集合
function BloodyBattleDlgBattle()
	local recvValue = {};	
	recvValue.m_unit_index = -1
	recvValue.m_posx = 0;
	recvValue.m_posy = 0;
	recvValue.m_nation = m_activityinfo.m_nation;
	recvValue.m_type = MAPUNIT_TYPE_KINGWAR_TOWN;
	MapBattleDlgShow( recvValue, ARMY_ACTION_KINGWAR, -1 )
end

-- 前往
function BloodyBattleDlgGoto( id )
	if m_SelectHeroOffset < 0 then
		return
	end
	local heroArmy = m_HeroArmyInfo[m_SelectHeroOffset+1];
	system_askinfo( ASKINFO_KINGWAR, "", 1, heroArmy.m_army_index, id+3 )
	BloodyBattleDlgSelectHero( -1 )
	m_LastSelectArmyIndex = heroArmy.m_army_index
end

-- 偷袭
function BloodyBattleDlgSneak( id )
	if m_SelectHeroOffset < 0 then
		return
	end
	local heroArmy = m_HeroArmyInfo[m_SelectHeroOffset+1];
	system_askinfo( ASKINFO_KINGWAR, "", 2, heroArmy.m_army_index, id+3 )
	BloodyBattleDlgSelectHero( -1 )
	m_LastSelectArmyIndex = heroArmy.m_army_index
end

-- 单挑
function BloodyBattleDlgPK( id )
	if m_SelectHeroOffset < 0 then
		return
	end
	local heroArmy = m_HeroArmyInfo[m_SelectHeroOffset+1];
	system_askinfo( ASKINFO_KINGWAR, "", 3, heroArmy.m_army_index, id+3 )
	BloodyBattleDlgSelectHero( -1 )
	m_LastSelectArmyIndex = heroArmy.m_army_index
end

-- 进攻
function BloodyBattleDlgAttack( id )
	if m_SelectHeroOffset < 0 then
		return
	end
	local heroArmy = m_HeroArmyInfo[m_SelectHeroOffset+1];
	system_askinfo( ASKINFO_KINGWAR, "", 4, heroArmy.m_army_index, id+3 )
	BloodyBattleDlgSelectHero( -1 )
	m_LastSelectArmyIndex = heroArmy.m_army_index
end
	
-- 回防
function BloodyBattleDlgDefense( id )
	if m_SelectHeroOffset < 0 then
		return
	end
	local heroArmy = m_HeroArmyInfo[m_SelectHeroOffset+1];
	system_askinfo( ASKINFO_KINGWAR, "", 5, heroArmy.m_army_index, id+3 )
	BloodyBattleDlgSelectHero( -1 )
	m_LastSelectArmyIndex = heroArmy.m_army_index
end

-- 救援
function BloodyBattleDlgRebirth()
	if m_SelectHeroOffset < 0 then
		return
	end
	local heroArmy = m_HeroArmyInfo[m_SelectHeroOffset+1];
	
	local costtoken = 0;
	local rebirthNum = heroArmy.m_to_type
	if rebirthNum == 0 then
		costtoken = 10;
	elseif rebirthNum == 1 then
		costtoken = 100;
	elseif rebirthNum == 2 then
		costtoken = 200;
	else
		costtoken = 400;
	end
	MsgBox( F(1416, costtoken, HeroName( heroArmy.m_herokind[1] ) ), function() 
		system_askinfo( ASKINFO_KINGWAR, "", 6, heroArmy.m_army_index )
		BloodyBattleDlgSelectHero( -1 )
		m_LastSelectArmyIndex = heroArmy.m_army_index
	end )
end


--m_state=0,m_beginstamp=0,m_endstamp=0,m_nation=0,
function BloodyBattleDlgSetActivityInfo( info )
	m_activityinfo = info;
end

-- 聊天
function BloodyBattleDlgSetChat( recvValue )
	m_ChatRecvValue = recvValue
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	if recvValue.m_actorid < 0 then
		-- 系统
		local name = "<color=FF0000FF>["..recvValue.m_name.."]：</color>"
		local msg = recvValue.m_msg
		SetRichText( m_uiChatText, name..msg )
	else
		if recvValue.m_channel == 2 then
		else
			local nation = "<color=4F57FFFF>【"..Nation( recvValue.m_nation ).."】</color>"
			local name = "<color=FFB900FF>["..recvValue.m_name.."]：</color>"
			local msg = ChatDlgMakeMsg( recvValue )
			if recvValue.m_msgtype == CHAT_MSGTYPE_VS or recvValue.m_msgtype == CHAT_MSGTYPE_SPY then
				SetRichText( m_uiChatText, nation..name..msg )
			else
				SetRichText( m_uiChatText, nation..name..msg )
			end
		end
	end
end

-- 设置log
-- m_a_heroid=0,m_a_color=0,m_a_name_len=0,m_a_name="[m_a_name_len]",m_a_losthp=0,m_a_nation=0,m_d_heroid=0,m_d_color=0,m_d_name_len=0,m_d_name="[m_d_name_len]",m_d_losthp=0,m_d_nation=0,m_result=0,m_id=0,
function BloodyBattleDlgSetFightLog( recvValue )
	SetText( m_uiLog.transform:Find("Timer"), "<"..getHourStringByInterval(os.time())..">("..KingWarTownName(recvValue.m_id)..")" );
	local losthp
	local heroname
	local actorname
	
	losthp = "<color=#e80017>-"..recvValue.m_a_losthp.."</color> ";
	heroname = "<color="..NameColorStr(recvValue.m_a_color)..">"..HeroName(recvValue.m_a_heroid).."</color>";
	if recvValue.m_result == 1 then
		actorname = "<color=#F7F3BB>["..recvValue.m_a_name.."]</color>"
	else
		actorname = "<color=#333333>["..recvValue.m_a_name.."]</color>"
	end
	SetText( m_uiLog.transform:Find("AttackName"), losthp..heroname..actorname );
	
	losthp = "<color=#e80017>-"..recvValue.m_d_losthp.."</color>";
	heroname = "<color="..NameColorStr(recvValue.m_d_color)..">"..HeroName(recvValue.m_d_heroid).."</color> ";
	if recvValue.m_result == 2 then
		actorname = "<color=#F7F3BB>["..recvValue.m_d_name.."]</color>"
	else
		actorname = "<color=#333333>["..recvValue.m_d_name.."]</color>"
	end
	SetText( m_uiLog.transform:Find("DefenseName"), actorname..heroname..losthp );
end

