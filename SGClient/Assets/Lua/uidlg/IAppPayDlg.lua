-- 界面
local m_Dlg = nil;
local m_uiWebView = nil; --UnityEngine.GameObject
local m_uiWaiting = nil; --UnityEngine.GameObject

-- 打开界面
function IAppPayDlgOpen()
	m_Dlg = eye.uiManager:Open( "IAppPayDlg" );
end

-- 隐藏界面
function IAppPayDlgClose()
	if m_Dlg == nil then
		return;
	end
	eye.uiManager:Close( "IAppPayDlg" );
end

-- 删除界面
function IAppPayDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function IAppPayDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            IAppPayDlgClose();
        end
	end
end

-- 载入时调用
function IAppPayDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;
	m_uiWebView = objs[0];
	m_uiWaiting = objs[1];
end

-- 界面初始化时调用
function IAppPayDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function IAppPayDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function IAppPayDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function IAppPayDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function IAppPayDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
-- m_orderid_len=0,m_orderid="[m_orderid_len]",m_ext_len=0,m_ext="[m_ext_len]",m_goodsid=0,m_productid=0,m_nameid=0,m_descid=0,m_price=0,
function IAppPayDlgShow( recvValue )
	IAppPayDlgOpen()
	SetTrue( m_uiWaiting )
	local info = {}
	info["waresid"] = tonumber(recvValue.m_productid)-- 商品编号
	--info["waresname"] = -- 商品名称
	info["cporderid"] = recvValue.m_orderid -- 商户订单号
	info["price"] = recvValue.m_price -- 支付金额
	info["appuserid"] = Const.serverid.."#"..Const.actorid -- 用户在商户应用的唯一标识
	info["cpprivateinfo"] = recvValue.m_ext -- 商户私有信息会透传给商户
	local transdata_json = json.encode(info)
	local transdata = WWW.EscapeURL(transdata_json)
	local uniWebView = m_uiWebView.transform:GetComponent( "UniWebView" )
	uniWebView:CleanCache()
	uniWebView:Load( "http://39.105.38.19/sg/iapppay/trade.php?transdata="..transdata, false );
	
	uniWebView.OnPageStarted = uniWebView.OnPageStarted + function( view, url )
		print( "OnPageStarted:"..url )
		gamelog( "OnPageStarted:"..url )
		local prefixHttp = string.sub( url, 4 )
		local prefixHttps = string.sub( url, 5 )
		if prefixHttp ~= "http" and prefixHttps ~= "https" then
			gamelog( "loadInnerApp" )
			DeviceHelper.loadInnerApp( url )
			uniWebView:Hide( false, UniWebViewTransitionEdge.None, 0.4, nil );
			IAppPayDlgClose()
		end
	end
	
	uniWebView.OnPageFinished = uniWebView.OnPageFinished + function( view, statusCode, url )
		print( "OnPageFinished:"..statusCode..","..url )
		gamelog( "OnPageFinished:"..statusCode..","..url )
		SetFalse( m_uiWaiting )
		--uniWebView:Show( false, UniWebViewTransitionEdge.None, 0.4, nil );
	end
	
	uniWebView.OnPageErrorReceived = uniWebView.OnPageErrorReceived + function( view, errorCode, errorMessage )
		print( "OnPageErrorReceived:"..errorCode..","..errorMessage )
		gamelog( "OnPageErrorReceived:"..errorCode..","..errorMessage )
		SetFalse( m_uiWaiting )
		--uniWebView:Hide( false, UniWebViewTransitionEdge.None, 0.4, nil );
	end
	
	uniWebView.OnMessageReceived = uniWebView.OnMessageReceived + function( view, message ) 
		gamelog( "OnMessageReceived:"..message )
		print( "OnMessageReceived:"..message )
	end
end
