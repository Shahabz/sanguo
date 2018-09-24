------------------ 界面
local m_Dlg = nil;
local m_uiIcon = nil; --UnityEngine.GameObject
local m_uiTitle = nil; --UnityEngine.GameObject
local m_uiUIP_Item = nil; --UnityEngine.GameObject
local m_uiBack = nil; --UnityEngine.GameObject

local m_Cache = {};
-- 队列起始
local m_FirstIndex = 1;
-- 队列结束
local m_LastIndex = 0;
-- 移动速度
local m_MoveSpeed = 60;
-- 界面初始宽度
local m_StartPosX = 600;
------------------

-- 打开界面
function RollingMsgTopDlgOpen()
	m_Dlg = eye.uiManager:Open( "RollingMsgTopDlg" );

    if Localization.currentLanguage == "ar" or Localization.currentLanguage == "fa" then
        m_StartPosX = -m_Dlg.transform.rect.width / 2;
    else
        m_StartPosX = m_Dlg.transform.rect.width / 2;
    end


    RollingMsgTopDlgPrepare();
end

-- 隐藏界面
function RollingMsgTopDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "RollingMsgTopDlg" );
end

-- 删除界面
function RollingMsgTopDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function RollingMsgTopDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            RollingMsgTopDlgClose();
        end
    elseif nType == UI_EVENT_TWEENFINISH then
        if nControlID == 1 then
            RollingMsgTopDlgPrepare();
        elseif nControlID == 2 then
            RollingMsgTopDlgUseMsg();
        end
	end
end

-- 载入时调用
function RollingMsgTopDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;
	m_uiIcon = objs[0];
	m_uiTitle = objs[1];
	m_uiUIP_Item = objs[2];
	m_uiBack = objs[3];
end

-- 界面初始化时调用
function RollingMsgTopDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function RollingMsgTopDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function RollingMsgTopDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function RollingMsgTopDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function RollingMsgTopDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
-- 显示内容
function RollingMsgTopDlgShowMsg( title, text )
    m_LastIndex = m_LastIndex + 1;
    m_Cache[m_LastIndex] = {};
    m_Cache[m_LastIndex].title = title;
    m_Cache[m_LastIndex].text = text;
    if m_Dlg == nil or m_Dlg.activeInHierarchy == false then
        RollingMsgTopDlgOpen();
    end
end

-- 使用队列里面的内容
function RollingMsgTopDlgUseMsg()
    if m_FirstIndex > m_LastIndex then
        -- 内容不足时关闭
        RollingMsgTopDlgClose();
        return nil;
    else
        if m_Dlg ~= nil and m_Dlg.activeInHierarchy == true then
            local trans = m_uiUIP_Item.transform;
            local width = trans:GetComponent("HorizontalLayoutGroup").preferredWidth;
            local tween = m_uiUIP_Item:GetComponent("UITweenLocalPosition");
            tween.from = Vector3.New(m_StartPosX,0,0);
            if Localization.currentLanguage == "ar" or Localization.currentLanguage == "fa" then
                tween.to = Vector3.New(width,0,0);
            else
                tween.to = Vector3.New(-width,0,0);
            end
            tween.duration = ( width + m_StartPosX ) / m_MoveSpeed;
            tween.delay = 0;
            tween:Play(true);
            m_Cache[m_FirstIndex] = nil;
            m_FirstIndex = m_FirstIndex + 1;
        end
    end
end

-- 准备显示队列里面的内容
function RollingMsgTopDlgPrepare()
    if m_FirstIndex > m_LastIndex then
        -- 内容不足时关闭
        RollingMsgTopDlgClose();
        return nil;
    else
        if m_Dlg ~= nil and m_Dlg.activeInHierarchy == true then
            local cache = m_Cache[m_FirstIndex];
            SetText(m_uiTitle, cache.title.." ");
            local trans = m_uiUIP_Item.transform;
			SetRichText( trans:Find("Text"), cache.text, ChatDlgOnLinkClickPos )
            trans.localPosition = Vector3.New(m_StartPosX,0,0);
            trans:GetComponent("UITweenFade"):Play(true);
        end
    end
end
