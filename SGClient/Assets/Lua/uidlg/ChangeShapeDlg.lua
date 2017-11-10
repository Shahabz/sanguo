-- 界面
local m_Dlg = nil;

--------------------------------------------------------------
local m_uiShape = nil; --UnityEngine.GameObject
local m_uiNameInput = nil; --UnityEngine.GameObject
local m_uiChangeNameItem = nil; --UnityEngine.GameObject
local m_uiChangeNameUseBtn = nil; --UnityEngine.GameObject
local m_uiChangeNameBuyBtn = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_SelectShape = -1;

-- 打开界面
function ChangeShapeDlgOpen()
	m_Dlg = eye.uiManager:Open( "ChangeShapeDlg" );
end

-- 隐藏界面
function ChangeShapeDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "ChangeShapeDlg" );
end

-- 删除界面
function ChangeShapeDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function ChangeShapeDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            ChangeShapeDlgClose();
			
		-- 选择形象
        elseif nControlID >= 0 and nControlID <= 40 then
        	ChangeShapeDlgSelect( nControlID );
			
		-- 确定改形象
       	elseif nControlID == 100 then
       		ChangeShapeDlgChangeShape();
			
		-- 确定改名
       	elseif nControlID == 101 then
       		ChangeShapeDlgChangeName();
			
		-- 随机名称
		elseif nControlID == 102 then
			ChangeShapeDlgRandom();
        end
	end
end

-- 载入时调用
function ChangeShapeDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;
	
	m_uiShape = objs[0];
	m_uiNameInput = objs[1];
	m_uiChangeNameItem = objs[2];
	m_uiChangeNameUseBtn = objs[3];
	m_uiChangeNameBuyBtn = objs[4];
	m_uiContent = objs[5];


end

-- 界面初始化时调用
function ChangeShapeDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function ChangeShapeDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function ChangeShapeDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function ChangeShapeDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function ChangeShapeDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function ChangeShapeDlgShow()
	ChangeShapeDlgOpen()
	ChangeShapeDlgSelect( GetPlayer().m_shape );
	m_uiNameInput:GetComponent( "UIInputField" ).text = GetPlayer().m_name
	
	local itemkind = 172
	if GetItem():GetCount(itemkind) > 0 then
		SetTrue( m_uiChangeNameUseBtn )
		SetFalse( m_uiChangeNameBuyBtn )
	else
		SetFalse( m_uiChangeNameUseBtn )
		SetTrue( m_uiChangeNameBuyBtn )
	end
	SetImage( m_uiChangeNameItem.transform:Find("Shape"), ItemSprite(itemkind) )
	SetImage( m_uiChangeNameItem.transform:Find("Color"), ItemColorSprite(item_getcolor(itemkind)) )
	SetText( m_uiChangeNameItem.transform:Find("Name"), item_getname(itemkind), NameColor(item_getcolor(itemkind)) )
end

-- 选择形象
function ChangeShapeDlgSelect( shape )
	m_SelectShape = shape;
	m_uiShape:GetComponent( "Image" ).sprite = PlayerFaceSprite( shape );
	for i = 0 ,m_uiContent.transform.childCount - 1 do
		local obj = m_uiContent.transform:GetChild(i).gameObject;
		if obj.transform:GetComponent( typeof(UIButton) ).controlID == shape then
			SetTrue( obj.transform:Find("Select") )
		else
			SetFalse( obj.transform:Find("Select") )
		end
    end
end

-- 改形象
function ChangeShapeDlgChangeShape()
	if m_SelectShape < 0 or m_SelectShape == GetPlayer().m_shape then
		return;
	end
	system_askinfo( ASKINFO_CHANGESHAPE, "", m_SelectShape );
	ChangeShapeDlgClose();
end

-- 改名
function ChangeShapeDlgChangeName()
	local name = m_uiNameInput:GetComponent( "UIInputField" ).text;
	if name == GetPlayer().m_name then
		pop(T(786))
		return;
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
	if GetItem():GetCount(itemkind) <= 0 then
		MsgBox( F(785, item_gettoken( itemkind ) ), function() 
			if GetPlayer().m_token < item_gettoken( itemkind ) then
				JumpToken();
				return
			else
				system_askinfo( ASKINFO_CHANGENAME, name, 1 );
				ChangeShapeDlgClose();
			end
		end )
	else
		system_askinfo( ASKINFO_CHANGENAME, name, 1 );
		ChangeShapeDlgClose();
	end
end

-- 随机名称
function ChangeShapeDlgRandom()
	m_uiNameInput:GetComponent( "UIInputField" ).text = random_name()
end
