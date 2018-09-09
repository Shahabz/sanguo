-- 界面
local m_Dlg = nil;
local m_uiShape = nil; --UnityEngine.GameObject
local m_uiMaxRank = nil; --UnityEngine.GameObject
local m_uiMyRank = nil; --UnityEngine.GameObject
local m_uiTodayNum = nil; --UnityEngine.GameObject
local m_uiMatchLayer = nil; --UnityEngine.GameObject
local m_uiMatchContent = nil; --UnityEngine.GameObject
local m_uiMyPower = nil; --UnityEngine.GameObject
local m_uiMyTeamList = nil; --UnityEngine.GameObject
local m_uiUpdateCD = nil; --UnityEngine.GameObject
local m_uiUpdateBtn = nil; --UnityEngine.GameObject
local m_uiLogLayer = nil; --UnityEngine.GameObject
local m_uiLogScroll = nil; --UnityEngine.GameObject
local m_uiLogContent = nil; --UnityEngine.GameObject
local m_uiUIP_Log = nil; --UnityEngine.GameObject
local m_uiWaiting = nil; --UnityEngine.GameObject
local m_uiReplaceBtn = nil; --UnityEngine.GameObject
local m_uiReplaceCD = nil; --UnityEngine.GameObject
local m_uiTodayResetBtn = nil; --UnityEngine.GameObject

local m_ObjectPool = nil
local m_MatchRecvValue = nil;
local m_LogRecvValue = nil;
local m_myteam = nil;
local m_mypower = 0;

-- 打开界面
function ColiseumDlgOpen()
	m_Dlg = eye.uiManager:Open( "ColiseumDlg" );
end

-- 隐藏界面
function ColiseumDlgClose()
	if m_Dlg == nil then
		return;
	end

	eye.uiManager:Close( "ColiseumDlg" );
	m_MatchRecvValue = nil;
	m_LogRecvValue = nil;
	m_myteam = nil;
end

-- 删除界面
function ColiseumDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
	m_MatchRecvValue = nil;
	m_LogRecvValue = nil;
	m_myteam = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function ColiseumDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            ColiseumDlgClose();
			
		elseif nControlID == 1 then
			ColiseumDlgShowMatchLayer()
			
		elseif nControlID == 2 then
			ColiseumDlgShowLogLayer()
		
		-- 替换阵容	
		elseif nControlID == 3 then
			ColiseumDlgReplace()
		
		-- 换一批
		elseif nControlID == 4 then
			ColiseumDlgUpdate()
		
		-- 重置今日次数
		elseif nControlID == 5 then
			ColiseumDlgTodayReset()
					
		-- 挑战	
		elseif nControlID >= 100 and nControlID <= 102 then
			ColiseumDlgPk( nControlID-100 )
			
		-- 观战
		elseif nControlID >= 10000 then
			ColiseumDlgLogView( nControlID-10000 )
        end
	elseif nType == UI_EVENT_TIMECOUNTEND then
		if nControlID == 0 then
			ColiseumDlgRecvUpdateCD( 0 )
		elseif nControlID == 1 then
			ColiseumDlgTeamReplaceCD( 0 )
		end
	end
end

-- 载入时调用
function ColiseumDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiShape = objs[0];
	m_uiMaxRank = objs[1];
	m_uiMyRank = objs[2];
	m_uiTodayNum = objs[3];
	m_uiMatchLayer = objs[4];
	m_uiMatchContent = objs[5];
	m_uiMyPower = objs[6];
	m_uiMyTeamList = objs[7];
	m_uiUpdateCD = objs[8];
	m_uiUpdateBtn = objs[9];
	m_uiLogLayer = objs[10];
	m_uiLogScroll = objs[11];
	m_uiLogContent = objs[12];
	m_uiUIP_Log = objs[13];
	m_uiWaiting = objs[14];
	m_uiReplaceBtn = objs[15];
	m_uiReplaceCD = objs[16];
	m_uiTodayResetBtn = objs[17];

	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );
	m_ObjectPool:CreatePool("UIP_Log", 30, 30, m_uiUIP_Log);
end

-- 界面初始化时调用
function ColiseumDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function ColiseumDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function ColiseumDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function ColiseumDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function ColiseumDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function ColiseumDlgShow()
	ColiseumDlgOpen()
	ColiseumDlgShowMatchLayer()
end

function ColiseumDlgScale( scale )
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	m_Dlg.transform.localScale = Vector3.New(scale,scale,scale)
	if scale == 1 then
		SetFalse( m_uiWaiting )
	end
end

-- 对手
function ColiseumDlgShowMatchLayer()
	SetTrue(m_uiMatchLayer)
	SetFalse(m_uiLogLayer)
	if m_MatchRecvValue == nil then
		SetTrue( m_uiWaiting )
		system_askinfo( ASKINFO_COLISEUM, "", 0 )
	else
		ColiseumDlgMatchRecv( m_MatchRecvValue )
	end
end

-- 收到对手列表
function ColiseumDlgMatchRecv( recvValue )
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	SetFalse( m_uiWaiting )
	m_MatchRecvValue = recvValue
	
	-- 基本信息
	SetText( m_uiMaxRank, F(4295,m_MatchRecvValue.m_maxrank) )
	SetText( m_uiMyRank, F(4288,m_MatchRecvValue.m_myrank) )
	SetText( m_uiTodayNum, F(4289,m_MatchRecvValue.m_todaynum,m_MatchRecvValue.m_maxtodaynum) )
	ColiseumDlgRecvUpdateCD( m_MatchRecvValue.m_updatecd )
	ColiseumDlgTeamReplaceCD( m_MatchRecvValue.m_replacecd )
	if m_MatchRecvValue.m_todaynum <= 0 then
		SetTrue( m_uiTodayResetBtn )
	else
		SetFalse( m_uiTodayResetBtn )
	end
	
	-- 对手列表
	for i=1, 3, 1 do	
		local uiObj = m_uiMatchContent.transform:GetChild(i-1);
		local info = m_MatchRecvValue.m_list[i]
		ColiseumDlgCreateMatch( i-1, uiObj, info )
	end
	
	-- 我的队伍
	ColiseumDlgTeamRecv(m_MatchRecvValue.m_myteam)
end

-- 我的队伍信息
function ColiseumDlgTeamRecv( myteam )
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	SetFalse( m_uiWaiting )
	m_myteam = myteam;
	-- 我的阵容
	m_mypower = 0;
	for i=1, 3, 1 do
		local uiObj = m_uiMyTeamList.transform:GetChild(i-1);
		local kind = m_myteam[i]
		ColiseumDlgCreateMyTeamHero( uiObj, kind )
	end
	SetText( m_uiMyPower, F(4293, m_mypower ) )
end

-- 替换队伍CD
function ColiseumDlgTeamReplaceCD( cd )
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	if cd > 0 then
		SetTimer( m_uiReplaceCD, cd, cd, 1 )
		SetTrue( m_uiReplaceCD )
		SetFalse( m_uiReplaceBtn )
	else
		SetFalse( m_uiReplaceCD )
		SetTrue( m_uiReplaceBtn )
	end
end

-- 换一批CD
function ColiseumDlgRecvUpdateCD( cd )
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	if cd > 0 then
		SetTimer( m_uiUpdateCD, cd, cd, 0, T(4299) )
		SetTrue( m_uiUpdateCD )
		SetFalse( m_uiUpdateBtn )
	else
		SetFalse( m_uiUpdateCD )
		SetTrue( m_uiUpdateBtn )
	end
end

-- 创建匹配对手
function ColiseumDlgCreateMatch( index, uiObj, info )
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiName = objs[0]
	local uiPower = objs[1]
	local uiRank = objs[2]
	local uiHeroList = objs[3]
	local uiPkBtn = objs[4]
	SetControlID( uiPkBtn, 100+index )
	SetText( uiName, info.m_name )
	SetText( uiPower, F( 4290, info.m_bpower ) )
	SetText( uiRank, F( 4291, info.m_rank ) )
	for i=1, 3, 1 do
		local uiHeroObj = uiHeroList.transform:GetChild(i-1);
		local heroinfo = info.m_hero[i]
		ColiseumDlgCreateHero( uiHeroObj, heroinfo )
	end
end

-- 创建武将
function ColiseumDlgCreateHero( uiHeroObj, hero )
	if hero.m_kind > 0 then
		SetTrue( uiHeroObj )
		SetImage( uiHeroObj.transform:Find("Color"), HeroColorSprite(hero.m_color) );
		SetImage( uiHeroObj.transform:Find("Shape"), HeroFaceSprite(hero.m_kind) );
		SetImage( uiHeroObj.transform:Find("NameColor"), HeroNameColorSprite(hero.m_color) );
		SetText( uiHeroObj.transform:Find("Name"), HeroName( hero.m_kind ) );
	else
		SetFalse( uiHeroObj )
	end
end

-- 创建我的武将
function ColiseumDlgCreateMyTeamHero( uiHeroObj, kind )
	local pHero = GetHero():GetPtr( kind )
	if pHero then
		SetImage( uiHeroObj.transform:Find("Color"), ItemColorSprite(pHero.m_color) );
		SetImage( uiHeroObj.transform:Find("Shape"), HeroHeadSprite(pHero.m_kind) );
		SetText( uiHeroObj.transform:Find("Name"), HeroName(pHero.m_kind) );
		SetTrue( uiHeroObj.transform:Find("NameBack") )
		m_mypower = m_mypower + pHero.m_bpower
	else
		SetImage( uiHeroObj.transform:Find("Color"), HeroColorSprite(0) );
		SetImage( uiHeroObj.transform:Find("Shape"), HeroFaceSprite(0) );
		SetText( uiHeroObj.transform:Find("Name"), "" );
		SetFalse( uiHeroObj.transform:Find("NameBack") )
	end
end

-- 挑战
function ColiseumDlgPk( index )
	if m_MatchRecvValue.m_todaynum <= 0 then
		ColiseumDlgTodayReset()
		return
	end
	SetTrue( m_uiWaiting )
	system_askinfo( ASKINFO_COLISEUM, "", 2, index )
end

-- 替换阵容
function ColiseumDlgReplace()
	ColiseumTeamDlgShow( m_myteam )
end

-- 换一批
function ColiseumDlgUpdate()
	SetTrue( m_uiWaiting )
	system_askinfo( ASKINFO_COLISEUM, "", 3 )
end

-- 战报
function ColiseumDlgShowLogLayer()
	SetFalse(m_uiMatchLayer)
	SetTrue(m_uiLogLayer)
	if m_LogRecvValue == nil then
		SetTrue( m_uiWaiting )
		system_askinfo( ASKINFO_COLISEUM, "", 5 )
	else
		ColiseumDlgLogRecv( m_LogRecvValue )
	end
end

-- 收到战报列表
function ColiseumDlgLogRecv( recvValue )
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	ColiseumDlgLogClear()
	SetFalse( m_uiWaiting )
	m_LogRecvValue = recvValue
	for i=1, recvValue.m_count, 1 do
		local info = recvValue.m_list[i]
		local uiObj = m_ObjectPool:Get( "UIP_Log" );
		uiObj.transform:SetParent( m_uiLogContent.transform );
		uiObj.transform.localScale = Vector3.one;
		uiObj.gameObject:SetActive( true );
		ColiseumDlgLogCreate( uiObj, info )
	end
end

-- 重置战报列表
function ColiseumDlgLogClear()
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
function ColiseumDlgLogCreate( uiObj, info )
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiANation = objs[0]
	local uiAName = objs[1]
	local uiAPower = objs[2]
	local uiARank = objs[3]
	local uiAHeroList = objs[4]
	local uiDNation = objs[5]
	local uiDName = objs[6]
	local uiDPower = objs[7]
	local uiDRank = objs[8]
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
	
	SetText( uiAPower, F(4290,info.m_attack.m_bpower) );
	SetText( uiDPower, F(4290,info.m_defense.m_bpower) );
	
	SetText( uiARank, F(4291,info.m_attack.m_rank) );
	SetText( uiDRank, F(4291,info.m_defense.m_rank) );
	
	if info.m_win == 1 then
		SetFalse( uiALose )
		SetTrue( uiDLose )
	else
		SetTrue( uiALose )
		SetFalse( uiDLose )
	end
	
	for i=1, 3, 1 do
		local uiHeroObj = uiAHeroList.transform:GetChild(i-1);
		local heroinfo = info.m_attack.m_hero[i]
		ColiseumDlgCreateLogHero( uiHeroObj, heroinfo )
	end
	for i=1, 3, 1 do
		local uiHeroObj = uiDHeroList.transform:GetChild(i-1);
		local heroinfo = info.m_defense.m_hero[i]
		ColiseumDlgCreateLogHero( uiHeroObj, heroinfo )
	end	
end

-- 创建我的武将
function ColiseumDlgCreateLogHero( uiHeroObj, pHero )
	if pHero then
		SetTrue( uiHeroObj )
		SetImage( uiHeroObj.transform:Find("Color"), ItemColorSprite(pHero.m_color) );
		SetImage( uiHeroObj.transform:Find("Shape"), HeroHeadSprite(pHero.m_kind) );
		--SetText( uiHeroObj.transform:Find("Name"), HeroName(pHero.m_kind) );
	else
		SetFalse( uiHeroObj )
	end
end

-- 观战
function ColiseumDlgLogView( fightid )
	system_askinfo( ASKINFO_COLISEUM, "", 6, fightid )
end

function ColiseumDlgTodayReset()
	MsgBox( F(4309,80), function() 
	 	system_askinfo( ASKINFO_COLISEUM, "", 8 )
	end )
end

function ColiseumDlgTodayUpdate( todaynum )
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	if m_MatchRecvValue == nil then
		return
	end
	m_MatchRecvValue.m_todaynum = todaynum;
	SetText( m_uiTodayNum, F(4289,m_MatchRecvValue.m_todaynum,m_MatchRecvValue.m_maxtodaynum) )
	if m_MatchRecvValue.m_todaynum <= 0 then
		SetTrue( m_uiTodayResetBtn )
	else
		SetFalse( m_uiTodayResetBtn )
	end
end