-- 界面
local m_Dlg = nil;
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiUIP_HeroHead = nil; --UnityEngine.GameObject
local m_uiHeroName = nil; --UnityEngine.GameObject
local m_uiExpPer = nil; --UnityEngine.GameObject
local m_uiProgress1 = nil; --UnityEngine.GameObject
local m_uiProgress2 = nil; --UnityEngine.GameObject
local m_uiItemGrid = nil; --UnityEngine.GameObject

local m_ItemList = { 161, 162, 163 }
local m_herokind = 0;
local m_itemkind = 0;
local m_pHero = nil
local m_guardup_toggle = false;

-- 打开界面
function HeroExpDlgOpen()
	m_Dlg = eye.uiManager:Open( "HeroExpDlg" );
end

-- 隐藏界面
function HeroExpDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "HeroExpDlg" );
end

-- 删除界面
function HeroExpDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function HeroExpDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            HeroExpDlgClose();
		
		-- 使用
		elseif nControlID == 1 then
			HeroExpDlgUse()
			
		-- 购买并使用
		elseif nControlID == 2 then
			HeroExpDlgBuy()
		
		elseif nControlID > 1000 and nControlID < 2000 then
			HeroExpDlgSelectHero( nControlID - 1000 )
		elseif nControlID > 2000 and nControlID < 3000 then
			HeroExpDlgSelectItem( nControlID - 2000 )
        end
	end
end

-- 载入时调用
function HeroExpDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiContent = objs[0];
	m_uiUIP_HeroHead = objs[1];
	m_uiHeroName = objs[2];
	m_uiExpPer = objs[3];
	m_uiProgress1 = objs[4];
	m_uiProgress2 = objs[5];
	m_uiItemGrid = objs[6];
end

-- 界面初始化时调用
function HeroExpDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function HeroExpDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function HeroExpDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function HeroExpDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function HeroExpDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function HeroExpDlgShow( kind )
	HeroExpDlgOpen()
	HeroExpDlgSetItem()
	HeroExpDlgSelectItem( 161 );
	HeroExpDlgSetHero()
	HeroExpDlgSelectHero( kind );
end

-- 英雄
function HeroExpDlgSetHero()
	for i=0,3,1 do
		local uiHero = m_uiContent.transform:GetChild(i).gameObject;
		local pHero = GetHero().m_CityHero[i]
		if pHero.m_kind > 0 then
			SetTrue( uiHero )
			SetControlID( uiHero, 1000+pHero.m_kind )
			local objs = uiHero.transform:GetComponent( typeof(Reference) ).relatedGameObject;
			local uiShape = objs[0];
			local uiColor = objs[1];
			local uiCorps = objs[2];
			local uiName = objs[3];
			local uiSelect = objs[4];
			SetTrue( uiShape )
			SetTrue( uiColor )
			SetTrue( uiCorps )
			SetTrue( uiName )
			SetFalse( uiSelect )
			
			SetImage( uiShape, HeroHeadSprite( pHero.m_kind )  );
			SetImage( uiColor,  ItemColorSprite( pHero.m_color )  );
			SetImage( uiCorps,  CorpsSprite( pHero.m_corps )  );
			SetText( uiName, HeroNameLv( pHero.m_kind, pHero.m_level ) );
	
		else
			SetFalse( uiHero )
		end
	end
end

-- 选择英雄
function HeroExpDlgSelectHero( kind )
	m_herokind = kind
	for i=0,3,1 do
		local uiHero = m_uiContent.transform:GetChild(i).gameObject;
		local objs = uiHero.transform:GetComponent( typeof(Reference) ).relatedGameObject;
		local uiSelect = objs[4];
		
		local pHero = GetHero().m_CityHero[i]
		if pHero.m_kind > 0 and pHero.m_kind == kind then
			m_pHero = pHero;
			SetTrue( uiSelect )
			SetText( m_uiHeroName, HeroNameLv( pHero.m_kind, pHero.m_level ) )
		else
			SetFalse( uiSelect )
		end
	end
	HeroExpDlgUpdateDesc( m_itemkind )
end

-- 道具
function HeroExpDlgSetItem()
	for i=1, #m_ItemList, 1 do
		local kind = m_ItemList[i];
		local uiItem = m_uiItemGrid.transform:GetChild(i-1).gameObject;
		SetControlID( uiItem, 2000 + kind )
		SetImage( uiItem.transform:Find("Shape"), ItemSprite(kind) );
		SetImage( uiItem.transform:Find("Color"), ItemColorSprite(item_getcolor(kind)) );
		SetText( uiItem.transform:Find("Num"), GetItem():GetCount(kind) );
	end
end

-- 选择道具
function HeroExpDlgSelectItem( itemkind )
	m_itemkind = itemkind
	for i=1, #m_ItemList, 1 do
		local kind = m_ItemList[i];
		local uiItem = m_uiItemGrid.transform:GetChild(i-1).gameObject;
		if kind == itemkind then
			SetTrue( uiItem.transform:Find("Select") )
	
			if GetItem():GetCount(kind) > 0 then
				SetTrue( uiItem.transform:Find("UseBtn") )
				SetFalse( uiItem.transform:Find("BuyBtn") )
				SetFalse( uiItem.transform:Find("Token") )
			else
				SetTrue( uiItem.transform:Find("BuyBtn") )
				SetFalse( uiItem.transform:Find("UseBtn") )
				SetTrue( uiItem.transform:Find("Token") )
				SetText( uiItem.transform:Find("Token/Num"), "x"..item_gettoken(kind) )
			end
		else
			SetFalse( uiItem.transform:Find("Select") )
			SetFalse( uiItem.transform:Find("UseBtn") )
			SetFalse( uiItem.transform:Find("BuyBtn") )
			SetFalse( uiItem.transform:Find("Token") )
		end
	end
	HeroExpDlgUpdateDesc( m_itemkind )
end

function HeroExpDlgUpdateDesc( itemkind )
	if m_pHero == nil then
		return
	end
	SetText( m_uiExpPer, T(128)..": "..math.floor(m_pHero.m_exp*100/m_pHero.m_exp_max).."%" )
	SetProgress( m_uiProgress1, (m_pHero.m_exp+g_itemkind[m_itemkind].base_value0)/m_pHero.m_exp_max );
	SetProgress( m_uiProgress2, m_pHero.m_exp/m_pHero.m_exp_max );
end

function HeroExpDlgUpdate()
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	HeroExpDlgSetItem()
	HeroExpDlgSelectItem( m_itemkind );
	HeroExpDlgSetHero()
	HeroExpDlgSelectHero( m_herokind );
end

-- 使用
function HeroExpDlgUse()
	if m_herokind <= 0 or m_itemkind <= 0 then
		return
	end
	if m_pHero.m_level >= GetPlayer().m_level then
		pop(T(834))
		return
	end
	system_askinfo( ASKINFO_HERO, "", 2, m_herokind, m_itemkind );
end

-- 购买并使用
function HeroExpDlgBuy()
	if m_herokind <= 0 or m_itemkind <= 0 then
		return
	end
	if m_pHero.m_level >= GetPlayer().m_level then
		pop(T(834))
		return
	end
	local heroname = HeroName( m_herokind )
	local token = item_gettoken( m_itemkind )
	MsgBoxEx( F( 833, token, heroname ), function( sure, toggle ) 
		if sure == 1 then
			if GetPlayer().m_token < token then
				JumpToken()
			else
				system_askinfo( ASKINFO_HERO, "", 2, m_herokind, m_itemkind );
			end
		end
		m_guardup_toggle = toggle;
	end, m_guardup_toggle ) 
end


