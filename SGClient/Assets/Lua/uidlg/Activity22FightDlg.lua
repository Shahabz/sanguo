-- 界面
local m_Dlg = nil;
local m_uiFightLayer = nil; --UnityEngine.GameObject
local m_uiDefenseScroll = nil; --UnityEngine.GameObject
local m_uiDefenseContent = nil; --UnityEngine.GameObject
local m_uiUIP_DefenseArmy = nil; --UnityEngine.GameObject
local m_uiAttackScroll = nil; --UnityEngine.GameObject
local m_uiAttackContent = nil; --UnityEngine.GameObject
local m_uiUIP_AttackArmy = nil; --UnityEngine.GameObject
local m_uiLogLayer = nil; --UnityEngine.GameObject
local m_uiLogScroll = nil; --UnityEngine.GameObject
local m_uiLogContent = nil; --UnityEngine.GameObject
local m_uiUIP_Log = nil; --UnityEngine.GameObject
local m_uiDefenseInfo = nil; --UnityEngine.GameObject
local m_uiAttackInfo = nil; --UnityEngine.GameObject
local m_uiEnterBtn = nil; --UnityEngine.GameObject
local m_uiWaiting = nil; --UnityEngine.GameObject

local m_ObjectPool = nil;
local m_FightRecvValueD = nil
local m_FightRecvValueA = nil
local m_LogRecvValue = nil
local m_AttackTotals = 0;
local m_DefenseTotals = 0;
local m_recvValue = nil

-- 打开界面
function Activity22FightDlgOpen()
	ResourceManager.LoadAssetBundle( "_ab_ui_static_pic_5" )
	m_Dlg = eye.uiManager:Open( "Activity22FightDlg" );
end

-- 隐藏界面
function Activity22FightDlgClose()
	if m_Dlg == nil then
		return;
	end
	eye.uiManager:Close( "Activity22FightDlg" );
	m_FightRecvValueD = nil
	m_FightRecvValueA = nil
	m_LogRecvValue = nil
end

-- 删除界面
function Activity22FightDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
	ResourceManager.UnloadAssetBundle( "_ab_ui_static_pic_5" )
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function Activity22FightDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            Activity22FightDlgClose();
			
		-- 对战	
		elseif nControlID == 1 then
			Activity22FightDlgShowFightLayer()
		
		-- 排行榜	
		elseif nControlID == 2 then
			Activity22FightDlgShowLogLayer()
		
		elseif nControlID == 10 then
			Activity22FightDlgBattle()
			
		elseif nControlID == 11 then
			Activity22RankDlgShow()
        end
	end
end

-- 载入时调用
function Activity22FightDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiFightLayer = objs[0];
	m_uiDefenseScroll = objs[1];
	m_uiDefenseContent = objs[2];
	m_uiUIP_DefenseArmy = objs[3];
	m_uiAttackScroll = objs[4];
	m_uiAttackContent = objs[5];
	m_uiUIP_AttackArmy = objs[6];
	m_uiLogLayer = objs[7];
	m_uiLogScroll = objs[8];
	m_uiLogContent = objs[9];
	m_uiUIP_Log = objs[10];
	m_uiDefenseInfo = objs[11];
	m_uiAttackInfo = objs[12];
	m_uiEnterBtn = objs[13];
	m_uiWaiting = objs[14];
	
	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );
	m_ObjectPool:CreatePool("UIP_DefenseArmy", 3, 3, m_uiUIP_DefenseArmy);
	m_ObjectPool:CreatePool("UIP_AttackArmy", 3, 3, m_uiUIP_AttackArmy);
	m_ObjectPool:CreatePool("UIP_Log", 10, 10, m_uiUIP_Log);
end

-- 界面初始化时调用
function Activity22FightDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function Activity22FightDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function Activity22FightDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function Activity22FightDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function Activity22FightDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function Activity22FightDlgShow( recvValue )
	Activity22FightDlgOpen()
	Activity22FightDlgShowFightLayer()
	m_recvValue = recvValue
end

-- 对战
function Activity22FightDlgShowFightLayer()
	SetTrue(m_uiFightLayer)
	SetFalse(m_uiLogLayer)
	if m_FightRecvValueA == nil then
		SetTrue( m_uiWaiting )
		system_askinfo( ASKINFO_ACTIVITY22, "", 1 )
	else
		Activity22FightDlgFightRecvA( m_FightRecvValueA )
	end
	
	if m_FightRecvValueD == nil then
		SetTrue( m_uiWaiting )
		system_askinfo( ASKINFO_ACTIVITY22, "", 2 )
	else
		Activity22FightDlgFightRecvD( m_FightRecvValueD )
	end
end
-- 收到对战列表
function Activity22FightDlgFightRecvA( recvValue )
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	m_AttackTotals = 0;
	Activity22FightDlgFightClearA()
	SetFalse( m_uiWaiting )
	m_FightRecvValueA = recvValue
	for i=1, recvValue.m_count, 1 do
		local info = recvValue.m_list[i]
		local uiObj = m_ObjectPool:Get( "UIP_AttackArmy" );
		uiObj.transform:SetParent( m_uiAttackContent.transform );
		uiObj.transform.localScale = Vector3.one;
		uiObj.gameObject:SetActive( true );
		ColiseumInfoDlgFightCreate( uiObj, info )
		m_AttackTotals = m_AttackTotals + info.m_totals
	end
	SetText( m_uiAttackInfo.transform:Find("Totals"),F(4337,m_AttackTotals ) )
end
function Activity22FightDlgFightRecvD( recvValue )
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	m_DefenseTotals = 0;
	Activity22FightDlgFightClearD()
	SetFalse( m_uiWaiting )
	m_FightRecvValueD = recvValue
	local nation = 0;
	for i=1, recvValue.m_count, 1 do
		local info = recvValue.m_list[i]
		local uiObj = m_ObjectPool:Get( "UIP_DefenseArmy" );
		uiObj.transform:SetParent( m_uiDefenseContent.transform );
		uiObj.transform.localScale = Vector3.one;
		uiObj.gameObject:SetActive( true );
		ColiseumInfoDlgFightCreate( uiObj, info )
		m_DefenseTotals = m_DefenseTotals + info.m_totals
		nation = info.m_nation
	end
	SetImage( m_uiDefenseInfo.transform:Find("Nation"), NationSprite(nation) )
	SetText( m_uiDefenseInfo.transform:Find("Totals"),F(4336,m_DefenseTotals ) )
end
-- 重置对战列表
function Activity22FightDlgFightClearA()
	local objs = {};
	for i=0,m_uiAttackContent.transform.childCount-1 do
		table.insert(objs,m_uiAttackContent.transform:GetChild(i).gameObject);
	end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "UIP_AttackArmy(Clone)" then
			m_ObjectPool:Release( "UIP_AttackArmy", obj );
		end
	end
end
function Activity22FightDlgFightClearD()
	local objs = {};
	for i=0,m_uiDefenseContent.transform.childCount-1 do
		table.insert(objs,m_uiDefenseContent.transform:GetChild(i).gameObject);
	end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "UIP_DefenseArmy(Clone)" then
			m_ObjectPool:Release( "UIP_DefenseArmy", obj );
		end
	end
end
-- 创建
function ColiseumInfoDlgFightCreate( uiObj, info )
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiNation = objs[0]
	local uiName = objs[1]
	local uiTotals = objs[2]
	local uiHeroList = objs[3]	
	local uiTimer = objs[4]
	local uiReady = objs[5]
	SetImage( uiNation, NationSprite(info.m_nation) );
	SetText( uiName, info.m_name );
	SetText( uiTotals, F(4339,info.m_totals) );
	if info.m_state == ARMY_STATE_ACTIVITY22_READY then
		SetTrue( uiReady )
		SetFalse( uiTimer )
	else
		SetFalse( uiReady )
		SetTrue( uiTimer )
		SetTimerOver( uiTimer, T(4342) )
		SetTimer( uiTimer,info.m_stateduration-info.m_statetime, info.m_stateduration, 0, T(4341) )
	end
	for i=1, 4, 1 do
		local uiHeroObj = uiHeroList.transform:GetChild(i-1);
		local herokind = info.m_herokind[i]
		ColiseumInfoDlgCreateFightHero( uiHeroObj, herokind )
	end	
end
-- 创建我的武将
function ColiseumInfoDlgCreateFightHero( uiHeroObj, herokind )
	if herokind > 0 then
		SetTrue( uiHeroObj )
		SetImage( uiHeroObj.transform:Find("Shape"), HeroHeadSprite(herokind) );
	else
		SetFalse( uiHeroObj )
	end
end
-- 更新
function ColiseumInfoDlgFightUpdate( attack )
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	system_askinfo( ASKINFO_ACTIVITY22, "", attack )
end


-- 战报
function Activity22FightDlgShowLogLayer()
	SetFalse(m_uiFightLayer)
	SetTrue(m_uiLogLayer)
	m_LogRecvValue = nil
	if m_LogRecvValue == nil then
		SetTrue( m_uiWaiting )
		system_askinfo( ASKINFO_ACTIVITY22, "", 4 )
	else
		Activity22FightDlgLogRecv( m_LogRecvValue )
	end
end
-- 收到战报列表
function Activity22FightDlgLogRecv( recvValue )
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	Activity22FightDlgLogClear()
	SetFalse( m_uiWaiting )
	m_LogRecvValue = recvValue
	for i=1, recvValue.m_count, 1 do
		local info = recvValue.m_list[i]
		local uiObj = m_ObjectPool:Get( "UIP_Log" );
		uiObj.transform:SetParent( m_uiLogContent.transform );
		uiObj.transform.localScale = Vector3.one;
		uiObj.gameObject:SetActive( true );
		Activity22FightDlgLogCreate( uiObj, info )
	end
end
-- 重置战报列表
function Activity22FightDlgLogClear()
	local objs = {};
	for i=0,m_uiLogContent.transform.childCount-1 do
		table.insert(objs,m_uiLogContent.transform:GetChild(i).gameObject);
	end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "UIP_Log(Clone)" then
			m_ObjectPool:Release( "UIP_Log", obj );
		end
	end
end
-- 创建战报
function Activity22FightDlgLogCreate( uiObj, info )
local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiANation = objs[0]
	local uiAName = objs[1]
	local uiATotal = objs[2]
	local uiALost = objs[3]
	local uiAHeroList = objs[4]
	local uiDNation = objs[5]
	local uiDName = objs[6]
	local uiDTotal = objs[7]
	local uiDlost = objs[8]
	local uiDHeroList = objs[9]
	local uiViewBtn = objs[10]
	local uiOptime = objs[11]
	local uiALose = objs[12]
	local uiDLose = objs[13]
	
	SetControlID( uiViewBtn, 10000+info.m_fightid )
	SetText( uiOptime, os.date( "%m-%d %H:%M", info.m_optime ) )
	
	SetImage( uiANation, NationSprite(info.m_attack.m_nation) );
	SetImage( uiDNation, NationSprite(info.m_defense.m_nation) );
	
	SetText( uiAName, info.m_attack.m_name );
	SetText( uiDName, info.m_defense.m_name );
	
	SetText( uiAPower, T(1117)..":"..info.m_attack.m_total );
	SetText( uiDPower, T(1117)..":"..info.m_defense.m_total );
	
	SetText( uiARank, T(1118)..":"..info.m_attack.m_lost );
	SetText( uiDRank, T(1118)..":"..info.m_defense.m_lost );
	
	if info.m_win == 1 then
		SetFalse( uiALose )
		SetTrue( uiDLose )
	else
		SetTrue( uiALose )
		SetFalse( uiDLose )
	end
	
	for i=1, 4, 1 do
		local uiHeroObj = uiAHeroList.transform:GetChild(i-1);
		local heroinfo = info.m_attack.m_hero[i]
		Activity22FightDlgCreateLogHero( uiHeroObj, heroinfo )
	end
	for i=1, 4, 1 do
		local uiHeroObj = uiDHeroList.transform:GetChild(i-1);
		local heroinfo = info.m_defense.m_hero[i]
		Activity22FightDlgCreateLogHero( uiHeroObj, heroinfo )
	end	
end

-- 创建我的武将
function Activity22FightDlgCreateLogHero( uiHeroObj, pHero )
	if pHero then
		SetTrue( uiHeroObj )
		SetImage( uiHeroObj.transform:Find("Shape"), HeroHeadSprite(pHero.m_kind) );
	else
		SetFalse( uiHeroObj )
	end
end

-- 集合
function Activity22FightDlgBattle()
	MapBattleDlgShow( m_recvValue, ARMY_ACTION_ACTIVITY22, -1 )
end