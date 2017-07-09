-- 界面
local m_Dlg = nil;
local m_uiQuestName = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiUIP_Award = nil; --UnityEngine.GameObject

-- 打开界面
function QuestAwardDlgOpen()
	m_Dlg = eye.uiManager:Open( "QuestAwardDlg" );
end

-- 隐藏界面
function QuestAwardDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "QuestAwardDlg" );
end

-- 删除界面
function QuestAwardDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function QuestAwardDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == 1 then
			QuestAwardDlgGet();
        end
	end
end

-- 载入时调用
function QuestAwardDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;
	m_uiQuestName = objs[0];
	m_uiContent = objs[1];
	m_uiUIP_Award = objs[2];
end

-- 界面初始化时调用
function QuestAwardDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function QuestAwardDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function QuestAwardDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function QuestAwardDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function QuestAwardDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function QuestAwardDlgShow( recvValue )
	QuestAwardDlgOpen()
	clearChild( m_uiContent );
	
end

function QuestAwardDlgGet()
	
	QuestAwardDlgClose();
end


