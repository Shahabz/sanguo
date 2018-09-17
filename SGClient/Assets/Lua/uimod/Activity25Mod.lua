local m_Mod;
local m_uiBack = nil; --UnityEngine.GameObject
local m_uiShape = nil; --UnityEngine.GameObject
local m_uiActivityTime = nil; --UnityEngine.GameObject
local m_uiTitle = nil; --UnityEngine.GameObject
local m_uiPayButton = nil; --UnityEngine.GameObject
local m_uiGetButton = nil; --UnityEngine.GameObject
local m_uiAwardDescLayer = nil; --UnityEngine.GameObject
local m_uiHasGet = nil; --UnityEngine.GameObject
local m_AwardDescLayerShow = false
local m_red = 0;
----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function Activity25ModOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
		if nControlID == 1 then
			PayDlgShow()
		elseif nControlID == 2 then
			Activity25ModGet()
		end
		--print( "Button Clicked, nControlID:" .. nControlID );
	elseif nType == UI_EVENT_PRESS then
		if value == 0 then
			--print( "Button Pressed Down, nControlID:" .. nControlID );
		elseif value == 1 then
			--print( "Button Pressed UP, nControlID:" .. nControlID);
		end
	end
end

-- 载入时调用
function Activity25ModOnAwake( gameObject )
	m_Mod = gameObject;
	local objs = gameObject:GetComponent( "UIMod" ).relatedGameObject;
	m_uiBack = objs[0];
	m_uiShape = objs[1];
	m_uiActivityTime = objs[2];
	m_uiTitle = objs[3];
	m_uiPayButton = objs[5];
	m_uiGetButton = objs[6];
	m_uiAwardDescLayer = objs[7];
	m_uiHasGet = objs[8];
end

-- 界面初始化时调用
function Activity25ModOnStart( gameObject )
	ResourceManager.LoadAssetBundle( "_ab_activity_pic_16" )
	ResourceManager.LoadAssetBundle( "_ab_ui_static_npc3" )
	
	SetImage( m_uiBack, LoadSprite("activity_pic_16") )
	SetImage( m_uiShape, LoadSprite("ui_static_npc3") )
	
	system_askinfo( ASKINFO_ACTIVITY, "", ACTIVITY_25, 0 )
end

-- 界面显示时调用
function Activity25ModOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function Activity25ModOnDisable( gameObject )
	
end

-- 界面删除时调用
function Activity25ModOnDestroy( gameObject )
	m_Mod = nil
	ResourceManager.UnloadAssetBundle( "_ab_activity_pic_16" )
	ResourceManager.UnloadAssetBundle( "_ab_ui_static_npc3" )
end

-- 每帧调用
function Activity25ModOnLogic( gameObject )
	
end

function Activity25ModGet()
	return m_Mod;
end
----------------------------------------
-- 自定
----------------------------------------
function Activity25ModRecv( sec, point, token, isget )
	if m_Mod == nil then
		return
	end
	m_red = 0;	
	
	local moneySymbol, divider = PayDlgGetMoneySymbol()
	if divider == 1 then	
		SetText( m_uiActivityTime.transform:Find("FristDayPay"), F(4245,moneySymbol,point) )
	else
		SetText( m_uiActivityTime.transform:Find("FristDayPay"), F(4245,moneySymbol,string.format("%.2f",point/divider)) )
	end
	SetRichText( m_uiActivityTime.transform:Find("Token"), "<icon=token>x"..token )
	
	-- 活动状态
	if sec > 0 then
		SetTrue( m_uiPayButton )
		SetFalse( m_uiGetButton )
		SetFalse( m_uiHasGet )
		SetTrue( m_uiActivityTime.transform:Find("Text") )
		SetTimer( m_uiActivityTime.transform:Find("Text"), sec, sec, 0, T(4246) )
	else
		SetFalse( m_uiActivityTime.transform:Find("Text") )
		if isget == 1 then -- 已领取
			SetFalse( m_uiPayButton )
			SetFalse( m_uiGetButton )
			SetTrue( m_uiHasGet )
		else
			SetFalse( m_uiPayButton )
			SetTrue( m_uiGetButton )
			SetFalse( m_uiHasGet )
			m_red = 1;
		end
	end
	
	-- 红点
	ActivityDlgChangeRed( ACTIVITY_25, m_red )
end

function Activity25ModGet()
	system_askinfo( ASKINFO_ACTIVITY, "", ACTIVITY_25, 1 )
end