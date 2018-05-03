-- 界面
local m_Dlg = nil;

local m_uiShape = nil; --UnityEngine.GameObject
local m_uiTalk = nil; --UnityEngine.GameObject
local m_uiButton = nil; --UnityEngine.GameObject
local m_uiButtonText = nil; --UnityEngine.GameObject

local m_callback = nil;
local m_clickstep = 0

-- 打开界面
function NpcTalkOneDlgOpen()
	ResourceManager.LoadAssetBundle( "_ab_ui_static_npc1" );
	m_Dlg = eye.uiManager:Open( "NpcTalkOneDlg" );
end

-- 隐藏界面
function NpcTalkOneDlgClose()
	if m_Dlg == nil then
		return;
	end
	eye.uiManager:Close( "NpcTalkOneDlg" );
end

-- 删除界面
function NpcTalkOneDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
	ResourceManager.UnloadAssetBundle( "_ab_ui_static_npc1" )
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
			end
		elseif nControlID == 1 then
			if m_callback then
				m_callback();
			end
			NpcTalkOneDlgClose();
        end
	end
end

-- 载入时调用
function NpcTalkOneDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;
	m_uiShape = objs[0];
	m_uiTalk = objs[1];
	m_uiButton = objs[2];
	m_uiButtonText = objs[3];	
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
function NpcTalkOne( text, buttonText, callback )
	NpcTalkOneDlgOpen();
	SetFalse( m_uiButton )
	m_clickstep = 0;
	SetImage( m_uiShape, LoadSprite("ui_static_npc1") );
	SetTextWriter( m_uiTalk, text, function() 
		m_clickstep=1
		SetTrue( m_uiButton )
	end )
	SetText( m_uiButtonText, buttonText );
	m_callback = callback;
end
