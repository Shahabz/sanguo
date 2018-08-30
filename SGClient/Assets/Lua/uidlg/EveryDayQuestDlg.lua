-- 界面
local m_Dlg = nil;
--local m_DialogFrameMod = nil;

-- 打开界面
function EveryDayQuestDlgOpen()
	m_Dlg = eye.uiManager:Open( "EveryDayQuestDlg" );
	--m_DialogFrameMod = DialogFrameModOpen( m_Dlg, T(0), nil, EveryDayQuestDlgClose );
end

-- 隐藏界面
function EveryDayQuestDlgClose()
	if m_Dlg == nil then
		return;
	end
	--DialogFrameModClose( m_DialogFrameMod );
	--m_DialogFrameMod = nil;
	eye.uiManager:Close( "EveryDayQuestDlg" );
end

-- 删除界面
function EveryDayQuestDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function EveryDayQuestDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            EveryDayQuestDlgClose();
        end
	end
end

-- 载入时调用
function EveryDayQuestDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
end

-- 界面初始化时调用
function EveryDayQuestDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function EveryDayQuestDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function EveryDayQuestDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function EveryDayQuestDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function EveryDayQuestDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function EveryDayQuestDlgShow()
	EveryDayQuestDlgOpen()
end
