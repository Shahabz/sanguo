-- 界面
local m_Dlg = nil;
local m_uiShape = nil; --UnityEngine.GameObject
local m_uiName = nil; --UnityEngine.GameObject
local m_uiTalk = nil; --UnityEngine.GameObject

-- 打开界面
function NpcTalkDlgOpen()
	m_Dlg = eye.uiManager:Open( "NpcTalkDlg" );
end

-- 隐藏界面
function NpcTalkDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "NpcTalkDlg" );
end

-- 删除界面
function NpcTalkDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function NpcTalkDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            NpcTalkDlgClose();
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
function NpcTalk( talk )
	NpcTalkDlgOpen();
	--m_uiShape:GetComponent( "Image" ).sprite = shape;
	--m_uiName:GetComponent( "UIText" ).text = name;
	m_uiTalk:GetComponent( "UIText" ).text = talk;
end