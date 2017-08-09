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
	HeroExpDlgSetHero()
	HeroExpDlgSelectHero( kind );
	HeroExpDlgSetItem()
	HeroExpDlgSelectItem( 161 );
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
			SetTrue( uiSelect )
		else
			SetFalse( uiSelect )
		end
	end
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
			else
				SetTrue( uiItem.transform:Find("BuyBtn") )
				SetFalse( uiItem.transform:Find("UseBtn") )
			end
		else
			SetFalse( uiItem.transform:Find("Select") )
			SetFalse( uiItem.transform:Find("UseBtn") )
			SetFalse( uiItem.transform:Find("BuyBtn") )
		end
		
	end
end

