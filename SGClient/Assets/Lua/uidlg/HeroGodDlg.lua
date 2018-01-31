-- 界面
local m_Dlg = nil;
local m_uiShape = nil; --UnityEngine.GameObject
local m_uiCorps = nil; --UnityEngine.GameObject
local m_uiType = nil; --UnityEngine.GameObject
local m_uiGrid = nil; --UnityEngine.GameObject
local m_uiEquipGrid = nil; --UnityEngine.GameObject
local m_uiBack = nil; --UnityEngine.GameObject

local m_pHero = nil
local m_AddEquip = nil
local m_NeedEquip = nil
local m_AttrName = nil
local m_AttrWashid = nil

-- 打开界面
function HeroGodDlgOpen()
	m_Dlg = eye.uiManager:Open( "HeroGodDlg" );
	
end

-- 隐藏界面
function HeroGodDlgClose()
	if m_Dlg == nil then
		return;
	end
	eye.uiManager:Close( "HeroGodDlg" );
	m_pHero = nil
	m_AddEquip = nil
	m_NeedEquip = nil
	m_AttrName = nil
	m_AttrWashid = nil
end

-- 删除界面
function HeroGodDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
	m_pHero = nil
	m_AddEquip = nil
	m_NeedEquip = nil
	m_AttrName = nil
	m_AttrWashid = nil
	HeroColorSpriteUnload( 0 )
	HeroColorSpriteUnload( 1 )
	HeroColorSpriteUnload( 2 )
	HeroColorSpriteUnload( 3 )
	HeroColorSpriteUnload( 4 )
	HeroColorSpriteUnload( 5 )
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function HeroGodDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            HeroGodDlgClose();
			
		-- 一键添加
		elseif nControlID == 1 then
			HeroGodDlgAllAdd()
			
		-- 开始突破
		elseif nControlID == 2 then
			HeroGodDlgUp()
		
		-- 添加装备	
		elseif nControlID > 100 and nControlID < 110 then
			HeroGodDlgAdd( nControlID - 100 )
        end
	end
end

-- 载入时调用
function HeroGodDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiShape = objs[0];
	m_uiCorps = objs[1];
	m_uiType = objs[2];
	m_uiGrid = objs[3];
	m_uiEquipGrid = objs[4];
	m_uiBack = objs[5];
end

-- 界面初始化时调用
function HeroGodDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function HeroGodDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function HeroGodDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function HeroGodDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function HeroGodDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function HeroGodDlgShow( pHero )
	HeroGodDlgOpen()
	m_pHero = pHero
	SetImage( m_uiBack, HeroColorSprite( pHero.m_color ) )
	SetImage( m_uiShape, HeroHeadSprite( pHero.m_kind ) );
	SetImage( m_uiCorps, CorpsSprite( pHero.m_corps ) );
	
	local only = GetHero():IsNationHeroOnly( pHero.m_kind )
	if only == true then
		SetText( m_uiType, T(2359) )
	else
		SetText( m_uiType, T(2358) )
	end
	
	SetText( m_uiGrid.transform:GetChild(0).transform:Find("Text"),  F( 2381, pHero.m_troops, global.hero_god_troops ) );
	SetText( m_uiGrid.transform:GetChild(1).transform:Find("Text"),  F( 2382, pHero.m_attack, global.hero_god_attack ) )
	SetText( m_uiGrid.transform:GetChild(2).transform:Find("Text"),  F( 2383, pHero.m_defense, global.hero_god_defense ) )
	
	m_AttrWashid = { 15, 15, 25, 25, 75, 75 } 
	m_AttrName = { 2377, 2377, 2378, 2378, 2379, 2379 } 
	m_NeedEquip = { 5, 15, 25, 35, 45, 55 } 
	m_AddEquip = { 0, 0, 0, 0, 0, 0 }
	for i=0,5,1 do
		HeroGodDlgSetEquip( i+1, m_uiEquipGrid.transform:GetChild( i ) )
	end
end

function HeroGodDlgSetEquip( index, uiObj )
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiShape = objs[0];
	local uiWash = objs[1];
	local uiAttrName = objs[2];
	local uiMask = objs[3];
	local uiAdd = objs[4];
	SetControlID( uiObj, 100+index )
	
	local addequip = m_AddEquip[index];
	if addequip == 1 then
		SetFalse( uiAdd )
		SetFalse( uiMask )
	else
		SetTrue( uiAdd )
		SetTrue( uiMask )
	end
	SetImage( uiShape, EquipSprite(m_NeedEquip[index]) )
	SetText( uiAttrName, T(m_AttrName[index]) );
end

-- 添加单件
function HeroGodDlgAdd( index )
	if m_AddEquip[index] == 1 then
		return
	end
	local kind = m_NeedEquip[index]
	local washid = m_AttrWashid[index]
	local has = 0;
	for tmpi=0, MAX_EQUIPNUM-1, 1 do
		local pEquip = GetEquip():GetAnyEquip( tmpi );
		if pEquip ~= nil and pEquip.m_kind == kind then
			if pEquip.m_washid[1] == washid and 
			   pEquip.m_washid[2] == washid and 
			   pEquip.m_washid[3] == washid and 
			   pEquip.m_washid[4] == washid then
				has = 1;
			end
		end
	end
	
	if has == 0 then
		pop( T(2380) )
	else
		m_AddEquip[index] = 1;
		HeroGodDlgSetEquip( index, m_uiEquipGrid.transform:GetChild( index - 1 ) )
	end
end

-- 一键添加
function HeroGodDlgAllAdd()
	for i=1,6,1 do
		HeroGodDlgAdd( i )
	end
end

-- 开始突破
function HeroGodDlgUp()
	
end
