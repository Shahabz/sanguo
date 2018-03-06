local m_Mod;
local m_uiBack = nil; --UnityEngine.GameObject
local m_uiShape = nil; --UnityEngine.GameObject
local m_uiBack2 = nil; --UnityEngine.GameObject
local m_uiGetButton = nil; --UnityEngine.GameObject
----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function Activity11ModOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
		--print( "Button Clicked, nControlID:" .. nControlID );
		if nControlID == 1 then
			system_askinfo( ASKINFO_ACTIVITY, "", ACTIVITY_11, 1 )
		end
	elseif nType == UI_EVENT_PRESS then
		if value == 0 then
			--print( "Button Pressed Down, nControlID:" .. nControlID );
		elseif value == 1 then
			--print( "Button Pressed UP, nControlID:" .. nControlID);
		end
	end
end

-- 载入时调用
function Activity11ModOnAwake( gameObject )
	m_Mod = gameObject;
	local objs = gameObject:GetComponent( "UIMod" ).relatedGameObject;
	m_uiBack = objs[0];
	m_uiShape = objs[1];
	m_uiBack2 = objs[2];
	m_uiGetButton = objs[3];
end

-- 界面初始化时调用
function Activity11ModOnStart( gameObject )
	system_askinfo( ASKINFO_ACTIVITY, "", ACTIVITY_11, 0 )
	ResourceManager.LoadAssetBundle( "_ab_ui_static_npc3" )
	ResourceManager.LoadAssetBundle( "_ab_activity_pic_15" )
	ResourceManager.LoadAssetBundle( "_ab_activity_back_3" )
	
	SetImage( m_uiBack, LoadSprite("activity_pic_15") )
	SetImage( m_uiShape, LoadSprite("ui_static_npc3") )
	SetImage( m_uiBack2, LoadSprite("activity_back_3") )
end

-- 界面显示时调用
function Activity11ModOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function Activity11ModOnDisable( gameObject )
	
end

-- 界面删除时调用
function Activity11ModOnDestroy( gameObject )
	m_Mod = nil
	ResourceManager.UnloadAssetBundle( "_ab_ui_static_npc3" )
	ResourceManager.UnloadAssetBundle( "_ab_activity_pic_15" )
	ResourceManager.UnloadAssetBundle( "_ab_activity_back_3" );
end

-- 每帧调用
function Activity11ModOnLogic( gameObject )
	
end

function Activity11ModGet()
	return m_Mod;
end
----------------------------------------
-- 自定
----------------------------------------
function Activity11ModRecv( isget1, isget2 )
	if m_Mod == nil then
		return
	end
	local textid = 0
	local nowtime = GetServerTime()
	local t = os.date( "*t", nowtime )
	if t.hour >= 0 and t.hour < 11 then
		textid = 2436
		SetButtonFalse( m_uiGetButton )
	elseif t.hour >= 11 and t.hour < 12 then
		textid = 2437
		SetButtonFalse( m_uiGetButton )
	elseif t.hour >= 12 and t.hour < 15 then
		if isget1 == 1 then
			textid = 1109
			SetButtonFalse( m_uiGetButton )
		else
			textid = 2439
			SetButtonTrue( m_uiGetButton )
		end
	elseif t.hour >= 15 and t.hour < 17 then
		textid = 2436
		SetButtonFalse( m_uiGetButton )
	elseif t.hour >= 17 and t.hour < 18 then
		textid = 2438
		SetButtonFalse( m_uiGetButton )
	elseif t.hour >= 18 and t.hour < 21 then
		if isget2 == 1 then
			textid = 1109
			SetButtonFalse( m_uiGetButton )
		else
			textid = 2439
			SetButtonTrue( m_uiGetButton )
		end
	else
		textid = 2436
		SetButtonFalse( m_uiGetButton )
	end
	SetText( m_uiGetButton.transform:Find("Back/Text"), T(textid) )
end
