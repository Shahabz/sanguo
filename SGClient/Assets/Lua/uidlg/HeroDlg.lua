-- 界面
local m_Dlg = nil;
local m_DialogFrameMod = nil;
local m_uiUIP_Hero = {nil,nil,nil,nil}; --UnityEngine.GameObject
local m_uiInfantryNum = nil; --UnityEngine.GameObject
local m_uiCavalryNum = nil; --UnityEngine.GameObject
local m_uiArcherNum = nil; --UnityEngine.GameObject
local m_uiBuffLayer = nil; --UnityEngine.GameObject

-- 打开界面
function HeroDlgOpen()
	m_Dlg = eye.uiManager:Open( "HeroDlg" );
	m_DialogFrameMod = DialogFrameModOpen( m_Dlg, T(550), HELP_HeroDlg, HeroDlgClose );
end

-- 隐藏界面
function HeroDlgClose()
	if m_Dlg == nil then
		return;
	end
	DialogFrameModClose( m_DialogFrameMod );
	m_DialogFrameMod = nil;
	eye.uiManager:Close( "HeroDlg" );
end

-- 删除界面
function HeroDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function HeroDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            HeroDlgClose();
		elseif nControlID >= 1 and nControlID <= 4 then
			HeroDlgSelect( nControlID )
		elseif nControlID >= 100 and nControlID <= 200 then
			HeroDlgSoldiers( nControlID-100 )
        end
		
	end
end

-- 载入时调用
function HeroDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;
	m_uiUIP_Hero[1] = objs[0];
	m_uiUIP_Hero[2] = objs[1];
	m_uiUIP_Hero[3] = objs[2];
	m_uiUIP_Hero[4] = objs[3];	
	m_uiInfantryNum = objs[4];
	m_uiCavalryNum = objs[5];
	m_uiArcherNum = objs[6];
	m_uiBuffLayer = objs[7];
end

-- 界面初始化时调用
function HeroDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function HeroDlgOnEnable( gameObject )
	HeroDlgSetInfantry()
	HeroDlgSetCavalry()
	HeroDlgSetArcher()
	HeroDlgSetBuff()
	HeroDlgUpdate();
end

-- 界面隐藏时调用
function HeroDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function HeroDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function HeroDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function HeroDlgShow()
	HeroDlgOpen();
	HeroDlgSetInfantry()
	HeroDlgSetCavalry()
	HeroDlgSetArcher()
	HeroDlgSetBuff()
	HeroDlgUpdate();
end

function HeroDlgUpdate()
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	local index = 1;
	for i=0,3,1 do -- 战斗武将位置0-3
		HeroDlgSetHero( index, GetHero().m_CityHero[i] );
		index = index + 1;
	end
end

function HeroDlgSetHero( index, pHero )
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
		if index == 3 then
			if GetPlayer().m_attr.m_hero_up_num < 1 then -- 科技增加
				SetTrue( uiUnLockText )
				SetText( uiUnLockText, T(610) )
				SetImage( uiShapeBack1, LoadSprite("ui_icon_back_2") )
			else
				SetFalse( uiUnLockText )
				SetImage( uiShapeBack1, LoadSprite("ui_icon_back_4") )
			end

		elseif index == 4 then
			if GetPlayer().m_attr.m_hero_up_num < 2 then -- 科技增加
				SetTrue( uiUnLockText )
				SetText( uiUnLockText, T(611) )
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
	
	SetImage( uiShape, HeroHeadSprite( pHero.m_kind )  );
	SetImage( uiColor,  ItemColorSprite( pHero.m_color )  );
	SetImage( uiCorps,  CorpsSprite( pHero.m_corps )  );
	SetText( uiName, HeroNameLv( pHero.m_kind, pHero.m_level ), NameColor( pHero.m_color ) );
	SetProgress( uiSoldiersProgress, pHero.m_soldiers/pHero.m_troops );
	SetText( uiSoldiersText, pHero.m_soldiers.."/"..pHero.m_troops );
	if pHero.m_soldiers < pHero.m_troops then
		SetTrue( uiSoldiersBtn );
		SetFalse( uiSoldiersFull )
	else
		SetFalse( uiSoldiersBtn );
		SetTrue( uiSoldiersFull )
	end
	SetControlID( uiSoldiersBtn, 100 + index )
	SetText( uiState, HeroState( pHero.m_state ) );
end

-- 步兵
function HeroDlgSetInfantry()
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	SetText( m_uiInfantryNum, GetPlayer().m_infantry_num );
end

-- 骑兵
function HeroDlgSetCavalry()
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	SetText( m_uiCavalryNum, GetPlayer().m_cavalry_num );
end

-- 弓兵
function HeroDlgSetArcher()
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	SetText( m_uiArcherNum, GetPlayer().m_archer_num );
end

-- 选择英雄
function HeroDlgSelect( index )
	if GetHero().m_CityHero[index-1] == nil or GetHero().m_CityHero[index-1].m_kind <= 0 then
		if index == 3 then
			if GetPlayer().m_attr.m_hero_up_num < 1 then -- 科技增加
			else
				HeroListDlgShow( HEROLIST_PATH_HERO )
				HeroDlgClose()
			end
		elseif index == 4 then
			if GetPlayer().m_attr.m_hero_up_num < 2 then -- 科技增加
			else
				HeroListDlgShow( HEROLIST_PATH_HERO )
				HeroDlgClose()
			end
		end
		return;
	end
	HeroInfoDlgShow( HEROLIST_PATH_HERO, GetHero().m_CityHero[index-1], true );
end

-- 补兵
function HeroDlgSoldiers( index )
	if GetHero().m_CityHero[index-1] == nil then
		return;
	end
	local pHero = GetHero().m_CityHero[index-1]
	local soldiers = pHero.m_troops - pHero.m_soldiers;
	if soldiers <= 0 then
		return
	end

	if pHero.m_corps == 0 then
		if GetPlayer().m_infantry_num <= 0 then
			pop( T(1995) )
			return
		end
	elseif pHero.m_corps == 1 then
		if GetPlayer().m_cavalry_num <= 0 then
			pop( T(1995) )
			return
		end
	elseif pHero.m_corps == 2 then
		if GetPlayer().m_archer_num <= 0 then
			pop( T(1995) )
			return
		end
	end
	system_askinfo( ASKINFO_HERO, "", 1, pHero.m_kind );
end

-- 显示字令buff
function HeroDlgSetBuff()
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	local nowtime = GetServerTime()
	for i=2,5,1 do
		local uiObj = m_uiBuffLayer.transform:GetChild(i-2)
		local lefttime = GetPlayer().m_buff_endtime[i] - nowtime
		if i == 3 or i == 4 and GetPlayer().m_buff_endtime[i] > 0 and lefttime > 0 then
			SetTrue( uiObj )
			SetImage( uiObj.transform:Find("Shape"), LoadSprite("item_icon_"..(199+i)) )
			SetTimer( uiObj.transform:Find("Timer"), lefttime-1, lefttime, 0 );
		else
			SetFalse( uiObj )
		end
	end
end
