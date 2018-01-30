-- 界面
local m_Dlg = nil;
local m_DialogFrameMod = nil;
local m_uiUIP_Hero = {nil,nil,nil,nil}; --UnityEngine.GameObject
local m_uiReplaceBtn = nil; --UnityEngine.GameObject
local m_uiFood = nil; --UnityEngine.GameObject

local m_baseoffset = 8;
-- 打开界面
function HeroGuardDlgOpen()
	m_Dlg = eye.uiManager:Open( "HeroGuardDlg" );
	m_DialogFrameMod = DialogFrameModOpen( m_Dlg, T(857), HELP_CabinetDlg, HeroGuardDlgClose );
end

-- 隐藏界面
function HeroGuardDlgClose()
	if m_Dlg == nil then
		return;
	end
	DialogFrameModClose( m_DialogFrameMod );
	m_DialogFrameMod = nil;
	eye.uiManager:Close( "HeroGuardDlg" );
end

-- 删除界面
function HeroGuardDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function HeroGuardDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            HeroGuardDlgClose();
		elseif nControlID >= 1 and nControlID <= 4 then
			HeroGuardDlgSelect( nControlID )
		elseif nControlID >= 101 and nControlID <= 104 then
			HeroGuardDlgSoldiers( nControlID-100 )
		elseif nControlID == 1000 then
			HeroListDlgShow( HEROLIST_PATH_HERO_GUARD )
        end
	end
end

-- 载入时调用
function HeroGuardDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiUIP_Hero[1] = objs[0];
	m_uiUIP_Hero[2] = objs[1];
	m_uiUIP_Hero[3] = objs[2];
	m_uiUIP_Hero[4] = objs[3];	
	m_uiReplaceBtn = objs[4];
	m_uiFood = objs[5];
end

-- 界面初始化时调用
function HeroGuardDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function HeroGuardDlgOnEnable( gameObject )
	--HeroGuardDlgUpdate();
end

-- 界面隐藏时调用
function HeroGuardDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function HeroGuardDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function HeroGuardDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function HeroGuardDlgShow()
	CabinetDlgClose();
	HeroGuardDlgOpen()
	m_uiReplaceBtn.transform:SetSiblingIndex(1000);
	HeroGuardDlgSetFood()
	HeroGuardDlgUpdate();
end

function HeroGuardDlgUpdate()
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	local index = 1;
	for i=8,11,1 do -- 羽林卫武将位置8-11
		HeroGuardDlgSetHero( index, GetHero().m_CityHero[i] );
		index = index + 1;
	end
end

function HeroGuardDlgSetHero( index, pHero )
	local objs = m_uiUIP_Hero[index].transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiShape = objs[0];
	local uiColor = objs[1];
	local uiCorps = objs[2];
	local uiName = objs[3];
	local uiSoldiersProgress = objs[4];
	local uiSoldiersText = objs[5];
	local uiSoldiersBtn = objs[6];
	local uiState = objs[7];
	local uiSoldiersFull = objs[8];
	local uiUnLockText = objs[9];
	local uiStateBack = objs[10];
	local uiAdd = objs[11];
	local uiShapeBack1 = objs[12];
	local uiType = objs[13];
	SetControlID( m_uiUIP_Hero[index], index )
	
	if pHero == nil or pHero.m_kind <= 0 then
		SetFalse( uiShape )
		SetFalse( uiColor )
		SetFalse( uiCorps )
		SetFalse( uiName )
		SetFalse( uiSoldiersProgress )
		SetFalse( uiSoldiersText )
		SetFalse( uiSoldiersBtn )
		SetFalse( uiSoldiersFull )
		SetFalse( uiState )
		SetFalse( uiStateBack )
		SetFalse( uiAdd )
		if index == 1 then
			if GetPlayer().m_level < global.hero_cabinet_level1 then -- 等级0开放
				SetTrue( uiUnLockText )
				SetText( uiUnLockText, F(1486,global.hero_cabinet_level1) )
				SetImage( uiShapeBack1, LoadSprite("ui_icon_back_2") )
			else
				SetFalse( uiUnLockText )
				SetImage( uiShapeBack1, LoadSprite("ui_icon_back_4") )
			end
		elseif index == 2 then
			if GetPlayer().m_level < global.hero_cabinet_level2 then -- 等级100开放
				SetTrue( uiUnLockText )
				SetText( uiUnLockText, F(1486,global.hero_cabinet_level2) )
				SetImage( uiShapeBack1, LoadSprite("ui_icon_back_2") )
			else
				SetFalse( uiUnLockText )
				SetImage( uiShapeBack1, LoadSprite("ui_icon_back_4") )
			end
		elseif index == 3 then
			if GetPlayer().m_level < global.hero_cabinet_level3 then -- 等级105开放
				SetTrue( uiUnLockText )
				SetText( uiUnLockText, F(1486,global.hero_cabinet_level3) )
				SetImage( uiShapeBack1, LoadSprite("ui_icon_back_2") )
			else
				SetFalse( uiUnLockText )
				SetImage( uiShapeBack1, LoadSprite("ui_icon_back_4") )
			end

		elseif index == 4 then
			if GetPlayer().m_level < global.hero_cabinet_level4 then -- 等级110开放
				SetTrue( uiUnLockText )
				SetText( uiUnLockText, F(1486,global.hero_cabinet_level4) )
				SetImage( uiShapeBack1, LoadSprite("ui_icon_back_2") )
			else
				SetFalse( uiUnLockText )
				SetImage( uiShapeBack1, LoadSprite("ui_icon_back_4") )
			end
		end
		return;
	end
	
	SetFalse( uiUnLockText )
	SetTrue( uiShape )
	SetTrue( uiColor )
	SetTrue( uiCorps )
	SetTrue( uiName )
	SetTrue( uiSoldiersProgress )
	SetTrue( uiSoldiersText )
	SetTrue( uiState )
	SetTrue( uiStateBack )
	SetFalse( uiAdd )
	
	SetImage( uiShape, HeroHeadSprite( pHero.m_kind )  );
	SetImage( uiColor,  ItemColorSprite( pHero.m_color )  );
	SetImage( uiCorps,  CorpsSprite( pHero.m_corps )  );
	SetText( uiName, HeroNameLv( pHero.m_kind, pHero.m_level ), NameColor( pHero.m_color ) );
	
	local only = GetHero():IsNationHeroOnly( pHero.m_kind )
	if only == true and pHero.m_god == 1 then
		SetTrue( uiType )
		SetText( uiType, T(2359) )
	elseif only == true then
		SetTrue( uiType )
		SetText( uiType, T(2357) )
	elseif pHero.m_god == 1 then
		SetTrue( uiType )
		SetText( uiType, T(2358) )
	else
		SetFalse( uiType )
	end
	
	SetProgress( uiSoldiersProgress, pHero.m_soldiers/pHero.m_troops );
	SetText( uiSoldiersText, pHero.m_soldiers.."/"..pHero.m_troops, NameColor(0) );
	if pHero.m_soldiers < pHero.m_troops then
		SetTrue( uiSoldiersBtn );
		SetFalse( uiSoldiersFull )
		SetText( uiSoldiersText, T(1493), NameColor(2) )
	else
		SetFalse( uiSoldiersBtn );
		SetTrue( uiSoldiersFull )
	end
	SetControlID( uiSoldiersBtn, 100 + index )
	SetText( uiState, HeroState( pHero.m_state ) );
end

-- 选择英雄
function HeroGuardDlgSelect( index )
	local offset = index + m_baseoffset
	if GetHero().m_CityHero[offset-1] == nil or GetHero().m_CityHero[offset-1].m_kind <= 0 then
		if index == 1 then
			if GetPlayer().m_level < global.hero_cabinet_level1 then -- 等级0开放
			else
				HeroListDlgShow( HEROLIST_PATH_HERO_GUARD )
				HeroGuardDlgClose()
			end
		elseif index == 2 then
			if GetPlayer().m_level < global.hero_cabinet_level2 then -- 等级100开放
			else
				HeroListDlgShow( HEROLIST_PATH_HERO_GUARD )
				HeroGuardDlgClose()
			end
		elseif index == 3 then
			if GetPlayer().m_level < global.hero_cabinet_level3 then -- 等级105开放
			else
				HeroListDlgShow( HEROLIST_PATH_HERO_GUARD )
				HeroGuardDlgClose()
			end
		elseif index == 4 then
			if GetPlayer().m_level < global.hero_cabinet_level4 then -- 等级110开放
			else
				HeroListDlgShow( HEROLIST_PATH_HERO_GUARD )
				HeroGuardDlgClose()
			end
		end
		return;
	end
	HeroInfoDlgShow( HEROLIST_PATH_HERO_GUARD, GetHero().m_CityHero[offset-1], true );
end

-- 设置粮食
function HeroGuardDlgSetFood()
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	SetText( m_uiFood, T(123)..":"..knum(GetPlayer().m_food) )
end

-- 补兵
function HeroGuardDlgSoldiers( index )
	local offset = index + m_baseoffset
	if GetHero().m_CityHero[offset-1] == nil then
		return;
	end
	local pHero = GetHero().m_CityHero[offset-1]
	local soldiers = pHero.m_troops - pHero.m_soldiers;
	local progress = math.ceil( soldiers / pHero.m_troops * 100.0 );
	local token = math.ceil( progress * global.hero_cabinet_guard_token );
	MsgBox( F(1494, token), function() 
		if GetPlayer().m_token < token then
			JumpToken()
			return
		end
		system_askinfo( ASKINFO_HERO, "", 10, pHero.m_kind );
	end )
end
