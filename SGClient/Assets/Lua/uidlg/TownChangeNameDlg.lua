-- 界面
local m_Dlg = nil;
local m_uiChangeNameItem = nil; --UnityEngine.GameObject
local m_uiNameInput = nil; --UnityEngine.GameObject

local m_townid = 0;
local m_oldname = ""

-- 打开界面
function TownChangeNameDlgOpen()
	m_Dlg = eye.uiManager:Open( "TownChangeNameDlg" );
end

-- 隐藏界面
function TownChangeNameDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "TownChangeNameDlg" );
end

-- 删除界面
function TownChangeNameDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function TownChangeNameDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            TownChangeNameDlgClose();
		elseif nControlID == 1 then
			TownChangeNameDlgChange();
        end
	end
end

-- 载入时调用
function TownChangeNameDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiChangeNameItem = objs[0];
	m_uiNameInput = objs[1];
end

-- 界面初始化时调用
function TownChangeNameDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function TownChangeNameDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function TownChangeNameDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function TownChangeNameDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function TownChangeNameDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function TownChangeNameDlgShow( townid, custom_name )
	TownChangeNameDlgOpen()
	m_townid = townid;
	
	if custom_name ~= "" then
		m_oldname = custom_name;
	else
		m_oldname = MapTownName( townid )
	end
	m_uiNameInput:GetComponent( "UIInputField" ).text = m_oldname;
	
	local itemkind = 172
	SetImage( m_uiChangeNameItem.transform:Find("Shape"), ItemSprite(itemkind) )
	SetImage( m_uiChangeNameItem.transform:Find("Color"), ItemColorSprite(item_getcolor(itemkind)) )
	SetText( m_uiChangeNameItem.transform:Find("Name"), item_getname(itemkind).."x2", NameColor(item_getcolor(itemkind)) )
	SetText( m_uiChangeNameItem.transform:Find("Num"), GetItem():GetCount(itemkind) )
end

function TownChangeNameDlgChange()
	local name = m_uiNameInput:GetComponent( "UIInputField" ).text;
	if name == m_oldname then
		pop(T(1341))
		return
	end
	
	-- 非法检查
	local len = string.len( name );
	if len < 4 or len > 18 then
		pop(T(790))
		return
	end
	if string.checksign( name ) == false then
		pop(T(788))
		return
	end
	if Utils.MaskWordCheck( name ) == false then
		pop(T(789))
		return;
	end
	
	local itemkind = 172;
	local itemnum = GetItem():GetCount(itemkind);
	local neednum = 2
	if itemnum < 2 then
		local leftnum = neednum - itemnum;
		local costtoken = item_gettoken( itemkind )*leftnum;
		MsgBox( F(1342, costtoken, leftnum, item_getname(itemkind) ), function() 
			if GetPlayer().m_token < costtoken then
				JumpToken();
				return
			else
				system_askinfo( ASKINFO_MAPTOWN, name, 10, m_townid );
				TownChangeNameDlgClose()
				MapTownExDlgClose();
			end
		end )
	else
		system_askinfo( ASKINFO_MAPTOWN, name, 10, m_townid );
		TownChangeNameDlgClose()
		MapTownExDlgClose();
	end
end