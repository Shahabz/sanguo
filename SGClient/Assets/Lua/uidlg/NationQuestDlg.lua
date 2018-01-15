-- 界面
local m_Dlg = nil;

-- 打开界面
function NationQuestDlgOpen()
	m_Dlg = eye.uiManager:Open( "NationQuestDlg" );
end

-- 隐藏界面
function NationQuestDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "NationQuestDlg" );
end

-- 删除界面
function NationQuestDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function NationQuestDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            NationQuestDlgClose();
        end
	end
end

-- 载入时调用
function NationQuestDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
end

-- 界面初始化时调用
function NationQuestDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function NationQuestDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function NationQuestDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function NationQuestDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function NationQuestDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function NationQuestDlgShow()
	NationQuestDlgOpen()
end
