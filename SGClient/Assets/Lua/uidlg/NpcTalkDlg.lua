-- 界面
local m_Dlg = nil;
local m_uiShape = nil; --UnityEngine.GameObject
local m_uiName = nil; --UnityEngine.GameObject
local m_uiTalk = nil; --UnityEngine.GameObject
local m_WaitCallback = nil;
local m_WaitValue = nil;
local m_clickstep = 0
-- 打开界面
function NpcTalkDlgOpen()
	ResourceManager.LoadAssetBundle( "_ab_ui_static_npc1" );
	m_Dlg = eye.uiManager:Open( "NpcTalkDlg" );
end

-- 隐藏界面
function NpcTalkDlgClose()
	if m_Dlg == nil then
		return;
	end
	if m_WaitCallback then
		m_WaitCallback( m_WaitValue );
	end
	m_WaitCallback = nil;
	m_WaitValue = nil;
	eye.uiManager:Close( "NpcTalkDlg" );
end

-- 删除界面
function NpcTalkDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
	ResourceManager.UnloadAssetBundle( "_ab_ui_static_npc1" )
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function NpcTalkDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
			if m_clickstep == 0 then
				-- 文字快速出现
				m_uiTalk:GetComponent( typeof(TypeWriter) ):OnFinish()
				m_clickstep = 1;
			else
				NpcTalkDlgClose();
			end
        end
	end
end

-- 载入时调用
function NpcTalkDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiShape = objs[0];
	m_uiName = objs[1];
	m_uiTalk = objs[2];
end

-- 界面初始化时调用
function NpcTalkDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function NpcTalkDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function NpcTalkDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function NpcTalkDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function NpcTalkDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function NpcTalk( text, callback )
	NpcTalkDlgOpen();
	m_clickstep = 0;
	SetImage( m_uiShape, LoadSprite("ui_static_npc1") );
	--SetText( m_uiName, name );
	SetTextWriter( m_uiTalk, text, function() 
		m_clickstep=1
	end )
	m_WaitCallback = callback;
end

function NpcTalkIsShow()
	if m_Dlg ~= nil then
		return IsActive( m_Dlg )
	end
	return false;
end
-- 设置等待数据
function NpcTalkWait( callback, value )
	m_WaitCallback = callback;
	m_WaitValue = value;
end
