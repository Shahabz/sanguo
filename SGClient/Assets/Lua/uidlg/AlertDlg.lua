-- 界面
local m_Dlg = nil;
local m_uiAlertText = nil; --UnityEngine.GameObject
local m_uiButtonText = nil; --UnityEngine.GameObject
local m_pCallBack = nil;
-- 打开界面
function AlertDlgOpen()
	m_Dlg = eye.uiManager:Open( "AlertDlg" );
end

-- 隐藏界面
function AlertDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "AlertDlg" );
end

-- 删除界面
function AlertDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function AlertDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            AlertDlgClose();
		elseif nControlID == 1 then
			AlertDlgClose()
			if m_pCallBack then
				m_pCallBack();
			end
        end
	end
end

-- 载入时调用
function AlertDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiAlertText = objs[0];
	m_uiButtonText = objs[1];
end

-- 界面初始化时调用
function AlertDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function AlertDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function AlertDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function AlertDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function AlertDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function AlertDlgShow( szMsg, szBtnName, pCallBack )
	AlertDlgOpen()
	m_uiAlertText:GetComponent( "UIText" ).text = szMsg;
	m_uiButtonText:GetComponent( "UIText" ).text = szBtnName;
	m_pCallBack = pCallBack;
end

-- 弹出确定提示框
function AlertMsg( szMsg )
    AlertDlgShow( szMsg, T(602), nil );
end

function Alert( szMsg, szBtnName, pCallBack )
	AlertDlgShow( szMsg, szBtnName, pCallBack );
end
