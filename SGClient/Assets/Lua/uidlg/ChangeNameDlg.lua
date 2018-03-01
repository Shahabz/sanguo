-- 界面
local m_Dlg = nil;
local m_uiShape = nil; --UnityEngine.GameObject
local m_uiNameInput = nil; --UnityEngine.GameObject
local m_uiChangeNameBtn = nil; --UnityEngine.GameObject
-- 打开界面
function ChangeNameDlgOpen()
	ResourceManager.LoadAssetBundle( "_ab_ui_static_npc1" );
	m_Dlg = eye.uiManager:Open( "ChangeNameDlg" );
end

-- 隐藏界面
function ChangeNameDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	if IsGuiding() and GetCurrentGuideType() == GUIDE_CREATENAME then GuideNext() end;
	eye.uiManager:Close( "ChangeNameDlg" );
end

-- 删除界面
function ChangeNameDlgDestroy()
	GameObject.Destroy( m_Dlg );
	ResourceManager.UnloadAssetBundle( "_ab_ui_static_npc1" )
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function ChangeNameDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            ChangeNameDlgClose();
		elseif nControlID == 1 then
			ChangeNameDlgChange();
		elseif nControlID == 2 then
			ChangeNameDlgRandom();
        end
	end
end

-- 载入时调用
function ChangeNameDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiShape = objs[0];
	m_uiNameInput = objs[1];
	m_uiChangeNameBtn = objs[2];

end

-- 界面初始化时调用
function ChangeNameDlgOnStart( gameObject )
	if IsGuiding() and GetCurrentGuideType() == GUIDE_CREATENAME then FindCmdTpye(m_uiChangeNameBtn.transform.position) end;
end

-- 界面显示时调用
function ChangeNameDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function ChangeNameDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function ChangeNameDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function ChangeNameDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function ChangeNameDlgShow()
	math.randomseed(tostring(math.sin(os.time())):sub(4, 12))
	ChangeNameDlgOpen();
	ChangeNameDlgRandom();
end

function ChangeNameDlgRandom()
	m_uiNameInput:GetComponent( "UIInputField" ).text = random_name()
end

function ChangeNameDlgChange()
	local name = m_uiNameInput:GetComponent( "UIInputField" ).text;
	
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
				
	system_askinfo( ASKINFO_CHANGENAME, name, 0 );
	
end
