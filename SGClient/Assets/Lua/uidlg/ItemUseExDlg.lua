-- 界面
local m_Dlg = nil;
local m_uiTitle = nil; --UnityEngine.GameObject
local m_uiGrid = nil; --UnityEngine.GameObject
local m_uiDesc = nil; --UnityEngine.GameObject
local m_uiUse = nil; --UnityEngine.GameObject

local m_ItemList = {}
local m_type = 0;
local m_nSelectKind = 0;

-- 打开界面
function ItemUseExDlgOpen()
	m_Dlg = eye.uiManager:Open( "ItemUseExDlg" );
end

-- 隐藏界面
function ItemUseExDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "ItemUseExDlg" );
end

-- 删除界面
function ItemUseExDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function ItemUseExDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            ItemUseExDlgClose();
		
		-- 使用
		elseif nControlID == 1 then
			ItemUseExDlgUse()
			
		-- 购买并使用
		elseif nControlID == 2 then
			ItemUseExDlgBuy()
			
		elseif nControlID > 100 and nControlID <= 200 then
			ItemUseExDlgSelect( nControlID-100 )
        end
	end
end

-- 载入时调用
function ItemUseExDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiTitle = objs[0];
	m_uiGrid = objs[1];
	m_uiDesc = objs[2];
	m_uiUse = objs[3];
end

-- 界面初始化时调用
function ItemUseExDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function ItemUseExDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function ItemUseExDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function ItemUseExDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function ItemUseExDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function ItemUseExDlgShow( type )
	ItemUseExDlgOpen()
	SetText( m_uiDesc.transform:Find("Text"), "" )
	m_type = type;
	m_nSelectKind = 0;
	m_ItemList = {}
	-- 迁城
	if m_type == 1 then
		m_ItemList = { 132, 133 }
		SetText( m_uiTitle.transform:Find("Text"), T(994) )
	end
	for i=1, 4, 1 do
		if i <= #m_ItemList then
			ItemUseExDlgItemSet( i, m_ItemList[i] )
		else
			ItemUseExDlgItemSet( i, 0 )
		end
	end
end

function ItemUseExDlgItemSet( index, kind )
	local UIP_Item = m_uiGrid.transform:GetChild(index-1).gameObject
	SetTrue( UIP_Item )
	local objs = UIP_Item.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiShape = objs[0];
	local uiColor = objs[1];
	local uiNum = objs[2];
	local uiToken = objs[3];
	local uiBack = objs[4];
	local uiNumBack = objs[5];
	local uiSelect = objs[6];
	SetFalse( uiSelect )
	
	if kind <= 0 then
		SetFalse( uiShape )
		SetFalse( uiColor )
		SetFalse( uiNum )
		SetFalse( uiToken )
		SetFalse( uiBack )
		SetFalse( uiNumBack )
		SetControlID( UIP_Item, 0 ); 
		return	
	end
	
	SetControlID( UIP_Item, 100 + index ); 
	SetTrue( uiShape )
	SetTrue( uiColor )
	SetTrue( uiBack )
	SetTrue( uiNumBack )
	SetImage( uiShape, ItemSprite(kind) )
	SetImage( uiColor, ItemColorSprite( item_getcolor(kind) ) )
	
	local num = GetItem():GetCount( kind );
	if GetItem():GetCount( kind ) > 0 then
		SetFalse( uiToken );
		SetTrue( uiNum );
		SetText( uiNum, "x"..num );
	else
		SetFalse( uiNum );
		SetTrue( uiToken );
		SetText( uiToken, T(125)..":"..item_gettoken(kind) );
	end
end

-- 选择
function ItemUseExDlgSelect( index )
	local kind = m_ItemList[index]
	if kind == nil then
		return
	end
	m_nSelectKind = kind;
	-- 选择框
	for i=0, 3, 1 do
		local UIP_Item = m_uiGrid.transform:GetChild(i).gameObject
		local objs = UIP_Item.transform:GetComponent( typeof(Reference) ).relatedGameObject;
		local uiSelect = objs[6];
		if i == index - 1 then
			SetTrue( uiSelect )
		else
			SetFalse( uiSelect )
		end
	end
	
	-- 描述
	local desc = item_getdesc(kind);
	desc = string.split(desc, "\n")
	SetText( m_uiDesc.transform:Find("Text"), item_getname(kind)..": "..desc[1] )
	
	local num = GetItem():GetCount( kind );
	if GetItem():GetCount( kind ) > 0 then
		SetText( m_uiUse.transform:Find("Back/Text"), T(679) )
		SetControlID( m_uiUse, 1 ); 
	else
		SetText( m_uiUse.transform:Find("Back/Text"), T(753) )
		SetControlID( m_uiUse, 2 ); 
	end
end

-- 使用
function ItemUseExDlgUse()
	if m_nSelectKind <= 0 then
		return
	end
	if m_type == 1 then
		system_askinfo( ASKINFO_WORLDMAP, "", 8, WorldMap.m_nLastTouchGameCoorX, WorldMap.m_nLastTouchGameCoorY, m_nSelectKind );
	else
		GetItem():UseWithToken( m_nSelectKind, 1  )
	end
	ItemUseExDlgClose();
end

-- 购买
function ItemUseExDlgBuy()
	if m_nSelectKind <= 0 then
		return
	end
	MsgBox( F(754, item_gettoken(m_nSelectKind), item_getname(m_nSelectKind) ), function()
		if m_type == 1 then
			system_askinfo( ASKINFO_WORLDMAP, "", 8, WorldMap.m_nLastTouchGameCoorX, WorldMap.m_nLastTouchGameCoorY, m_nSelectKind );
		else
			GetItem():UseWithToken( m_nSelectKind, 1  )
		end
		ItemUseExDlgClose();
	end )
end
