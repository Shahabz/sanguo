-- 界面
local m_Dlg = nil;
--local m_DialogFrameMod = nil;
local m_uiLeft = nil; --UnityEngine.GameObject
local m_uiUpgradeLayer = nil; --UnityEngine.GameObject

-- 打开界面
function TeacherHelpDlgOpen()
	m_Dlg = eye.uiManager:Open( "TeacherHelpDlg" );
	--m_DialogFrameMod = DialogFrameModOpen( m_Dlg, T(0), nil, TeacherHelpDlgClose );
end

-- 隐藏界面
function TeacherHelpDlgClose()
	if m_Dlg == nil then
		return;
	end
	--DialogFrameModClose( m_DialogFrameMod );
	--m_DialogFrameMod = nil;
	eye.uiManager:Close( "TeacherHelpDlg" );
end

-- 删除界面
function TeacherHelpDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function TeacherHelpDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            TeacherHelpDlgClose();
        end
	end
end

-- 载入时调用
function TeacherHelpDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiLeft = objs[0];
	m_uiUpgradeLayer = objs[1];
end

-- 界面初始化时调用
function TeacherHelpDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function TeacherHelpDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function TeacherHelpDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function TeacherHelpDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function TeacherHelpDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function TeacherHelpDlgShow()
	TeacherHelpDlgOpen()
	local sprite, name, left = NpcTalkShapeInfo( 0 );
	SetTrue( m_uiLeft )
	SetImage( m_uiLeft.transform:Find("Shape"), sprite );
	SetText( m_uiLeft.transform:Find("Name"), name );
end

function TeacherHelpDlgShowByUpgrade()
	TeacherHelpDlgShow()
	SetTrue( m_uiUpgradeLayer )
end
