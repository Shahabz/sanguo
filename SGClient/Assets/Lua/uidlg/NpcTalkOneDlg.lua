-- 界面
local m_Dlg = nil;
local m_uiTalk = nil; --UnityEngine.GameObject

local m_callback = nil;
local m_clickstep = 0

-- 打开界面
function NpcTalkOneDlgOpen()
	m_Dlg = eye.uiManager:Open( "NpcTalkOneDlg" );
end

-- 隐藏界面
function NpcTalkOneDlgClose()
	if m_Dlg == nil then
		return;
	end
	if m_callback then
		m_callback();
	end
	eye.uiManager:Close( "NpcTalkOneDlg" );
end

-- 删除界面
function NpcTalkOneDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function NpcTalkOneDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
			if m_clickstep == 0 then
				-- 文字快速出现
				m_uiTalk:GetComponent( typeof(TypeWriter) ):OnFinish()
				m_clickstep = 1;
			else
				NpcTalkOneDlgClose();	
			end
        end
    elseif nType == UI_EVENT_TWEENFINISH then
		if nControlID == 0 then
			NpcTalkOneDlgClose()
		end
	end
end

-- 载入时调用
function NpcTalkOneDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;
	m_uiTalk = objs[0];	
end

-- 界面初始化时调用
function NpcTalkOneDlgOnStart( gameObject )

end

-- 界面显示时调用
function NpcTalkOneDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function NpcTalkOneDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function NpcTalkOneDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function NpcTalkOneDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function NpcTalkOne( text, callback )
	NpcTalkOneDlgOpen();
	m_clickstep = 0;
	SetTextWriter( m_uiTalk, text, function()
		m_clickstep=1
	end )
	m_callback = callback;
end
