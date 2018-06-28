-- 界面
local m_Dlg = nil;

local m_uiVipLevel = nil; --UnityEngine.GameObject
local m_uiVipNext = nil; --UnityEngine.GameObject
local m_uiVipProgress = nil; --UnityEngine.GameObject
local m_uiMCard = nil; --UnityEngine.GameObject
local m_uiWebView = nil; --UnityEngine.GameObject

-- 打开界面
function PayWebDlgOpen()
	m_Dlg = eye.uiManager:Open( "PayWebDlg" );
end

-- 隐藏界面
function PayWebDlgClose()
	if m_Dlg == nil then
		return;
	end
	eye.uiManager:Close( "PayWebDlg" );
end

-- 删除界面
function PayWebDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function PayWebDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            PayWebDlgClose();
        end
	end
end

-- 载入时调用
function PayWebDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiVipLevel = objs[0];
	m_uiVipNext = objs[1];
	m_uiVipProgress = objs[2];
	m_uiMCard = objs[3];
	m_uiWebView = objs[4];
end

-- 界面初始化时调用
function PayWebDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function PayWebDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function PayWebDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function PayWebDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function PayWebDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function PayWebDlgShow( recvValue )
	PayWebDlgOpen()
	PayWebDlgUpdateVip()
	
	local url = Global.GetValue("PAY_URL");
	local uniWebView = m_uiWebView.transform:GetComponent( "UniWebView" )
	uniWebView:Load( "http://uniwebview.onevcat.com/demo/index.html", false );
	uniWebView:Show( false, UniWebViewTransitionEdge.None, 0.4, nil );
	uniWebView.OnPageFinished = uniWebView.OnPageFinished + function( view, statusCode, url ) 
		
	end
end

-- 更新VIP
function PayWebDlgUpdateVip()
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	local viplevel = GetPlayer().m_viplevel
	SetText( m_uiVipLevel, "vip"..viplevel )
	if viplevel >= 14 then
		-- 已经满级
		SetFalse( m_uiVipNext )
		SetProgress( m_uiVipProgress, 1 );
		SetText( m_uiVipProgress.transform:Find("Text"), T(2098) );
	else
		local needexp = g_vip[viplevel].exp
		SetTrue( m_uiVipNext )
		SetText( m_uiVipNext, F(2096, needexp-GetPlayer().m_vipexp, viplevel+1 ) )
		SetProgress( m_uiVipProgress, GetPlayer().m_vipexp/needexp );
		SetText( m_uiVipProgress.transform:Find("Text"), GetPlayer().m_vipexp.."/"..needexp );
	end
end
