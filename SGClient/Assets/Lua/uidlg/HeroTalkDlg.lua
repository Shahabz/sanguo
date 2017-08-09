-- 界面
local m_Dlg = nil;
local m_uiShape = nil; --UnityEngine.GameObject
local m_uiName = nil; --UnityEngine.GameObject
local m_uiTalk = nil; --UnityEngine.GameObject
local m_herokind = 0;

-- 打开界面
function HeroTalkDlgOpen()
	m_Dlg = eye.uiManager:Open( "HeroTalkDlg" );
end

-- 隐藏界面
function HeroTalkDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "HeroTalkDlg" );
end

-- 删除界面
function HeroTalkDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function HeroTalkDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            HeroTalkDlgClose();
			if m_herokind == 1 then
				NpcTalkID( 10004 );
			end
        end
	end
end

-- 载入时调用
function HeroTalkDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;
	m_uiShape = objs[0];
	m_uiName = objs[1];
	m_uiTalk = objs[2];	
end

-- 界面初始化时调用
function HeroTalkDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function HeroTalkDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function HeroTalkDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function HeroTalkDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function HeroTalkDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function HeroTalk( shape, name, talk )
	HeroTalkDlgOpen();
	m_herokind = 0;
	m_uiShape:GetComponent( "Image" ).sprite = shape;
	m_uiName:GetComponent( "UIText" ).text = name;
	m_uiTalk:GetComponent( "UIText" ).text = talk;
end

function HeroTalkKind( herokind, text )
	HeroTalkDlgOpen();
	m_herokind = herokind
	m_uiShape:GetComponent( "Image" ).sprite = HeroFaceSprite( herokind );
	m_uiName:GetComponent( "UIText" ).text = HeroName(herokind);
	m_uiTalk:GetComponent( "UIText" ).text = text;
end
