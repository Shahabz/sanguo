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
local m_uiUIP_Hero1 = nil; --UnityEngine.GameObject
local m_uiUIP_Hero2 = nil; --UnityEngine.GameObject
local m_uiUIP_Hero3 = nil; --UnityEngine.GameObject
local m_uiUIP_Hero4 = nil; --UnityEngine.GameObject

local m_activityinfo = nil;
local m_recvValue = nil;
local m_HeroRecvValue = nil;

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
		
		-- 血战排行榜
		elseif nControlID == 2 then
		
		-- 加入血战
		elseif nControlID == 10 then
			BloodyBattleDlgBattle()
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
	m_uiUIP_Hero1 = objs[15];
	m_uiUIP_Hero2 = objs[16];
	m_uiUIP_Hero3 = objs[17];
	m_uiUIP_Hero4 = objs[18];
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
end

-- 接收据点信息
-- m_id=0,m_attack_total=0,m_defense_total=0,m_nation=0,
function BloodyBattleDlgRecv( recvValue )
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	m_recvValue = recvValue;
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
end

-- 接收武将信息
-- m_count=0,m_list={m_army_index=0,m_unit_index=0,m_state=0,m_statetime=0,m_stateduration=0,m_action=0,m_to_posx=0,m_to_posy=0,m_herokind={[4]},[m_count]},m_unit_index=0,
function BloodyBattleDlgHeroRecv( recvValue )
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	m_HeroRecvValue = recvValue;
	
	local index = 0;
	for i=1, m_recvValue.m_count, 1 do
		
		index = index + 1;
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
--m_state=0,m_beginstamp=0,m_endstamp=0,m_nation=0,
function BloodyBattleDlgSetActivityInfo( info )
	m_activityinfo = info;
end
