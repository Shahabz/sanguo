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
local m_uiRankLayer = nil; --UnityEngine.GameObject
local m_uiRankScroll = nil; --UnityEngine.GameObject
local m_uiRankContent = nil; --UnityEngine.GameObject
local m_uiUIP_Rank = nil; --UnityEngine.GameObject
local m_uiWaiting = nil; --UnityEngine.GameObject
local m_ObjectPool = nil
local m_MatchRecvValue = nil;
local m_RankRecvValue = nil;
local m_myteam = nil;
local m_mypower = 0;

-- 打开界面
function ColiseumDlgOpen()
	m_Dlg = eye.uiManager:Open( "ColiseumDlg" );
	--m_DialogFrameMod = DialogFrameModOpen( m_Dlg, T(0), nil, ColiseumDlgClose );
end

-- 隐藏界面
function ColiseumDlgClose()
	if m_Dlg == nil then
		return;
	end
	--DialogFrameModClose( m_DialogFrameMod );
	--m_DialogFrameMod = nil;
	eye.uiManager:Close( "ColiseumDlg" );
	m_MatchRecvValue = nil;
	m_RankRecvValue = nil;
	m_myteam = nil;
end

-- 删除界面
function ColiseumDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
	m_MatchRecvValue = nil;
	m_RankRecvValue = nil;
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
			ColiseumDlgShowRankLayer()
		
		-- 替换阵容	
		elseif nControlID == 3 then
			ColiseumDlgReplace()
		
		-- 换一批
		elseif nControlID == 3 then
			ColiseumDlgUpdate()
				
		-- 挑战	
		elseif nControlID >= 100 and nControlID <= 102 then
			ColiseumDlgPk( nControlID-100 )
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
	m_uiRankLayer = objs[10];
	m_uiRankScroll = objs[11];
	m_uiRankContent = objs[12];
	m_uiUIP_Rank = objs[13];
	m_uiWaiting = objs[14];
	
	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );
	m_ObjectPool:CreatePool("UIP_Rank", 30, 30, m_uiUIP_Rank);
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

-- 对手
function ColiseumDlgShowMatchLayer()
	SetTrue(m_uiMatchLayer)
	SetFalse(m_uiRankLayer)
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
	SetControlID( uiObj, 100+index )
	SetText( uiName, info.m_name )
	SetText( uiPower, F( 4290, info.m_bpower ) )
	SetText( uiRank, F( 4291, info.m_rank ) )
	for i=1, 3, 1 do
		local uiHeroObj = uiHeroList.transform:GetChild(i-1);
		local heroinfo = info.m_hero[i]
		ColiseumDlgCreateHero( uiHeroObj, hero )
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
		SetImage( uiHeroObj.transform:Find("Color"), HeroColorSprite(pHero.m_color) );
		SetImage( uiHeroObj.transform:Find("Shape"), HeroFaceSprite(pHero.m_kind) );
		SetText( uiHeroObj.transform:Find("Name"), "lv."..pHero.m_level );
		SetTrue( uiHeroObj.transform:Find("NameBack") )
		m_mypower = m_mypower + pHero.m_bpower
	else
		SetImage( uiHeroObj.transform:Find("Color"), HeroColorSprite(0) );
		SetImage( uiHeroObj.transform:Find("Shape"), HeroFaceSprite(0) );
		SetText( uiHeroObj.transform:Find("Name"), "lv."..pHero.m_level );
		SetFalse( uiHeroObj.transform:Find("NameBack") )
	end
end

-- 挑战
function ColiseumDlgPk( index )
	system_askinfo( ASKINFO_COLISEUM, "", 2, index )
end

-- 替换阵容
function ColiseumDlgReplace()
	ColiseumTeamDlgShow( m_myteam )
end

-- 换一批
function ColiseumDlgUpdate()
	system_askinfo( ASKINFO_COLISEUM, "", 3 )
end

-- 排行
function ColiseumDlgShowRankLayer()
	SetFalse(m_uiMatchLayer)
	SetTrue(m_uiRankLayer)
	if m_RankRecvValue == nil then
		SetTrue( m_uiWaiting )
		system_askinfo( ASKINFO_COLISEUM, "", 1 )
	else
		ColiseumDlgRankRecv( m_RankRecvValue )
	end
end

-- 收到排行列表
function ColiseumDlgRankRecv( recvValue )
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	ColiseumDlgRankClear()
	SetFalse( m_uiWaiting )
	m_RankRecvValue = recvValue
	for i=1, recvValue.m_count, 1 do
		local info = recvValue.m_list[i]
		local uiObj = m_ObjectPool:Get( "UIP_Rank" );
		uiObj.transform:SetParent( m_uiRankContent.transform );
		uiObj.transform.localScale = Vector3.one;
		uiObj.gameObject:SetActive( true );
		ColiseumDlgCreateRank( uiObj, info )
	end
end

-- 重置排名列表
function ColiseumDlgRankClear()
	local objs = {};
	for i=0,m_uiRankContent.transform.childCount-1 do
		table.insert(objs,m_uiRankContent.transform:GetChild(i).gameObject);
	end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "UIP_Rank(Clone)" then
			m_ObjectPool:Release( "UIP_Rank", obj );
		end
	end
end

-- 创建排名
function ColiseumDlgRankCreate( uiObj, info )
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiRank = objs[0]
	local uiRankImage = objs[1]
	local uiNation = objs[2]
	local uiName = objs[3]
	local uiLevel = objs[4]
	local uiPower = objs[5]
	
	if info.m_rank == 1 then
		SetTrue( uiRankImage )
		SetImage( uiRankImage, LoadSprite( "ui_icon_rank1" ) )
	elseif info.m_rank == 2 then
		SetTrue( uiRankImage )
		SetImage( uiRankImage, LoadSprite( "ui_icon_rank2" ) )
	elseif info.m_rank == 3 then
		SetTrue( uiRankImage )
		SetImage( uiRankImage, LoadSprite( "ui_icon_rank3" ) )
	else
		SetFalse( uiRankImage )
	end
	SetText( uiRank, info.m_rank )
	SetImage( uiNation, NationSprite(info.m_nation) );
	SetText( uiName, info.m_name );
	SetTrue( uiLevel, info.m_level )
	SetTrue( uiPower, info.m_bpower )
end


