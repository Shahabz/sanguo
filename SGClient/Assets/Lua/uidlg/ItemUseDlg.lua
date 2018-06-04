-- 界面
local m_Dlg = nil;
local m_uiGrid = nil; --UnityEngine.GameObject
local m_callback = nil;
local m_autoclose = true;
local m_useitem_toggle = false;
-- 打开界面
function ItemUseDlgOpen()
	m_Dlg = eye.uiManager:Open( "ItemUseDlg" );
end

-- 隐藏界面
function ItemUseDlgClose()
	if m_Dlg == nil then
		return;
	end
	m_callback = nil
	m_useitem_toggle = false;
	eye.uiManager:Close( "ItemUseDlg" );
end

-- 删除界面
function ItemUseDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
	m_type = 0;
	m_army_index = -1;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function ItemUseDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            ItemUseDlgClose();
		elseif nControlID > 10000 and nControlID < 20000 then
			ItemUseDlgUse( nControlID-10000 )
		elseif nControlID > 20000 and nControlID < 30000 then
			ItemUseDlgBuy( nControlID-20000 )
        end
	end
end

-- 载入时调用
function ItemUseDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiGrid = objs[0];
end

-- 界面初始化时调用
function ItemUseDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function ItemUseDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function ItemUseDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function ItemUseDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function ItemUseDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function ItemUseDlgShow( itemList, itemDesc, autoclose, callback )
	ItemUseDlgOpen()
	m_callback = callback;
	m_autoclose = autoclose;
	for i=0,2,1 do
		SetFalse( m_uiGrid.transform:GetChild(i).gameObject )
	end
	
	for i=1, #itemList, 1 do
		ItemUseDlgItemSet( i, itemList[i], itemDesc[i] )
	end
end

function ItemUseDlgItemSet( index, kind, desc )
	local UIP_Item = m_uiGrid.transform:GetChild(index-1).gameObject
	SetTrue( UIP_Item )
	local objs = UIP_Item.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiShape = objs[0];
	local uiColor = objs[1];
	local uiNum = objs[2];
	local uiToken = objs[3];
	local uiName = objs[4];
	local uiDesc = objs[5];
	local uiUse = objs[6];

	SetImage( uiShape, ItemSprite(kind) )
	SetImage( uiColor, ItemColorSprite( item_getcolor(kind) ) )
	SetText( uiName, item_getname(kind), NameColor( item_getcolor( kind ) ) );
	SetText( uiDesc, desc );
	local num = GetItem():GetCount( kind );
	if GetItem():GetCount( kind ) > 0 then
		SetFalse( uiToken );
		SetTrue( uiNum );
		SetText( uiNum, num );
		SetText( uiUse.transform:Find("Back/Text"), T(679) )
		SetControlID( uiUse, 10000 + kind ); 
	else
		SetFalse( uiNum );
		SetTrue( uiToken );
		SetRichText( uiToken, "<icon=token>x"..item_gettoken(kind) );
		SetText( uiUse.transform:Find("Back/Text"), T(753) )
		SetControlID( uiUse, 20000 + kind ); 
	end
end

function ItemUseDlgUse( itemkind )
	if m_callback then
		m_callback( itemkind )
	end
	ItemUseDlgClose();
end

function ItemUseDlgBuy( itemkind )
	local token = item_gettoken(itemkind)
	if GetPlayer().m_token < token then
		JumpToken()
		return
	end
	
	MsgBoxEx( F(754, token, item_getname(itemkind)), function( sure, toggle )
		if sure == 1 then
			if m_callback then
				m_callback( itemkind )
			end
			if m_autoclose == true then
				ItemUseDlgClose();
			end
		end
		m_useitem_toggle = toggle;
	end, m_useitem_toggle )
end