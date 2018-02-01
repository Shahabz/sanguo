-- 界面
local m_Dlg = nil;
local m_DialogFrameMod = nil;
local m_uiReelOpen = nil; --UnityEngine.GameObject
local m_uiDesc = nil; --UnityEngine.GameObject
local m_uiDesc1 = nil; --UnityEngine.GameObject
local m_uiDesc2 = nil; --UnityEngine.GameObject
local m_uiDesc3 = nil; --UnityEngine.GameObject
local m_uiOpenTip = nil; --UnityEngine.GameObject
local m_uiTitleTip = nil; --UnityEngine.GameObject
local m_uiProTip = nil; --UnityEngine.GameObject
local m_uiFinishBack = nil; --UnityEngine.GameObject
local m_uiProContent = nil; --UnityEngine.GameObject
local m_uiButtons = nil; --UnityEngine.GameObject
local m_uiGoButton = nil; --UnityEngine.GameObject
local m_uiNextButton = nil; --UnityEngine.GameObject

-- 打开界面
function HeroLevyDlgOpen()
	m_Dlg = eye.uiManager:Open( "HeroLevyDlg" );
	m_DialogFrameMod = DialogFrameModOpen( m_Dlg, T(3101), nil, HeroLevyDlgClose );
end

-- 隐藏界面
function HeroLevyDlgClose()
	if m_Dlg == nil then
		return;
	end
	DialogFrameModClose( m_DialogFrameMod );
	m_DialogFrameMod = nil;
	eye.uiManager:Close( "HeroLevyDlg" );
end

-- 删除界面
function HeroLevyDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function HeroLevyDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            HeroLevyDlgClose();
        end
	end
end

-- 载入时调用
function HeroLevyDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiInfoLayer = objs[0];
	m_uiLeftBtn = objs[1];
	m_uiRightBtn = objs[2];
	m_uiTipLayer = objs[3];
	m_uiReelClose = objs[4];
	m_uiReelOpen = objs[5];
	m_uiDesc = objs[6];
	m_uiDesc1 = objs[7];
	m_uiDesc2 = objs[8];
	m_uiDesc3 = objs[9];
	m_uiOpenTip = objs[10];
	m_uiTitleTip = objs[11];
	m_uiProTip = objs[12];
	m_uiFinishBack = objs[13];
	m_uiProContent = objs[14];
	m_uiButtons = objs[15];
	m_uiGoButton = objs[16];
	m_uiNextButton = objs[17];
	
end

-- 界面初始化时调用
function HeroLevyDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function HeroLevyDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function HeroLevyDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function HeroLevyDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function HeroLevyDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function HeroLevyDlgShow()
	HeroLevyDlgOpen()
end
