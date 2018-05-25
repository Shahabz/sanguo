local m_Mod;
local m_uiBack = nil; --UnityEngine.GameObject
local m_uiBack2 = nil; --UnityEngine.GameObject
local m_uiScrollView = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiUIP_Mission = nil; --UnityEngine.GameObject
local m_uiLeftBtn = nil; --UnityEngine.GameObject
local m_uiRightBtn = nil; --UnityEngine.GameObject
local m_uiAwardList = nil; --UnityEngine.GameObject
local m_uiUnLockState = nil; --UnityEngine.GameObject
local m_uiGetState = nil; --UnityEngine.GameObject
local m_uiGetButton = nil; --UnityEngine.GameObject
local m_uiAwardDescLayer = nil; --UnityEngine.GameObject
local m_AwardDescLayerShow = false
local m_state = 0
local m_red = 0;
----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function Activity2ModOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
		--print( "Button Clicked, nControlID:" .. nControlID );
		if nControlID == -2 then
			Activity2ModAwardDescLayer();
		elseif nControlID == 1 then
			Activity2ModScrollPagePre()
		elseif nControlID == 2 then
			Activity2ModScrollPageNext()
		elseif nControlID >= 1000 and nControlID <= 2000 then
			Activity2ModGoto( nControlID-1000 )
		elseif nControlID >= 2000 and nControlID <= 3000 then
			Activity2ModSelect( nControlID-2000 )
		elseif nControlID >= 1000000 and nControlID < 2000000 then
			Activity2ModClickItem( nControlID-1000000, value )
		end
	elseif nType == UI_EVENT_PRESS then
		if value == 0 then
			--print( "Button Pressed Down, nControlID:" .. nControlID );
		elseif value == 1 then
			--print( "Button Pressed UP, nControlID:" .. nControlID);
		end
	elseif nType == UI_EVENT_SCROLLPAGE then
		if nControlID == 0 then
			Activity2ModScrollPage( value )
		end
	end
end

-- 载入时调用
function Activity2ModOnAwake( gameObject )
	m_Mod = gameObject;
	local objs = gameObject:GetComponent( "UIMod" ).relatedGameObject;
	m_uiBack = objs[0];
	m_uiBack2 = objs[1];
	m_uiScrollView = objs[2];
	m_uiContent = objs[3];
	m_uiUIP_Mission = objs[4];
	m_uiLeftBtn = objs[5];
	m_uiRightBtn = objs[6];
	m_uiAwardList = objs[7];
	m_uiUnLockState = objs[8];
	m_uiGetState = objs[9];
	m_uiGetButton = objs[10];
	m_uiAwardDescLayer = objs[11];
end

-- 界面初始化时调用
function Activity2ModOnStart( gameObject )
	system_askinfo( ASKINFO_ACTIVITY, "", ACTIVITY_2, 0 )
	ResourceManager.LoadAssetBundle( "_ab_activity_pic_3" )
	ResourceManager.LoadAssetBundle( "_ab_activity_back_0" )
	ResourceManager.LoadAssetBundle( "_ab_activity_back_3" )
	
	SetImage( m_uiBack, LoadSprite("activity_pic_3") )
	SetImage( m_uiBack2, LoadSprite("activity_back_3") )
	m_AwardDescLayerShow = false
end

-- 界面显示时调用
function Activity2ModOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function Activity2ModOnDisable( gameObject )
	
end

-- 界面删除时调用
function Activity2ModOnDestroy( gameObject )
	m_Mod = nil
	ResourceManager.UnloadAssetBundle( "_ab_activity_pic_3" )
	ResourceManager.UnloadAssetBundle( "_ab_activity_back_0" )
	ResourceManager.UnloadAssetBundle( "_ab_activity_back_3" )
	m_state = 0
end

-- 每帧调用
function Activity2ModOnLogic( gameObject )
	
end

function Activity2ModGet()
	return m_Mod;
end
----------------------------------------
-- 自定
----------------------------------------
function Activity2ModRecv( state )
	if m_Mod == nil then
		return
	end
	m_red = 0;
	local scrollPage = m_uiScrollView.transform:GetComponent( typeof(UIScrollPage) )
	scrollPage:ClearPage()
	m_state = state
	for i=1, #g_activity_02, 1 do
		local sflag = Utils.get_int_sflag( state, i )
		Activity2ModCreate( g_activity_02[i], sflag, scrollPage )
	end
	scrollPage:PageCountChanged()
	
	-- 默认选择一个
	local isselect = false;
	for i=1, #g_activity_02, 1 do
		local sflag = Utils.get_int_sflag( state, i )
		if sflag == 0 and GetPlayer():CityLevel() >= g_activity_02[i].level then
			scrollPage:Go( i-1 )
			isselect = true;
			break;
		end
	end
	if isselect == false then
		for i=1, #g_activity_02, 1 do
			local sflag = Utils.get_int_sflag( state, i )
			if GetPlayer():CityLevel() < g_activity_02[i].level then
				scrollPage:Go( i-1 )
				break;
			end
		end
	end
	-- 红点
	ActivityDlgChangeRed( ACTIVITY_2, m_red )
end
function Activity2ModCreate( info, state, scrollPage )
	local uiObj = GameObject.Instantiate( m_uiUIP_Mission );
	SetTrue( uiObj )
	scrollPage:AddPage( uiObj )
	
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiBack = objs[0];
	local uiLevel = objs[1];
	local uiIcon = objs[2];
	local uiBuilding = objs[3];
	local uiName = objs[4];
	local uiDesc = objs[5];
	local uiGotoButton = objs[6];
	local uiWarn = objs[7];
		
	SetControlID( uiGotoButton, 1000 + info.id  )
	SetImage( uiBack, LoadSprite("activity_back_0") )
	SetText( uiLevel, F(2462,info.level) )
	SetText( uiName, T(info.unlock_nameid) )
	SetText( uiDesc, T(info.unlock_descid) )
	
	if info.sprite_buildingkind > 0 then
		SetImage( uiBuilding, BuildingSprite( info.sprite_buildingkind ) )
		SetTrue( uiBuilding )
		SetFalse( uiIcon )
	elseif info.sprite_techkind > 0 then
		SetImage( uiIcon.transform:Find("Shape"), TechSprite( info.sprite_techkind ) )
		SetFalse( uiIcon.transform:Find("Color") )
		SetTrue( uiIcon )
		SetFalse( uiBuilding )
	elseif info.sprite_icon ~= "" then
		SetImage( uiIcon.transform:Find("Shape"), LoadSprite( info.sprite_icon ) )
		SetImage( uiIcon.transform:Find("Color"), ItemColorSprite( info.sprite_color ) )
		SetTrue( uiIcon.transform:Find("Color") )
		SetTrue( uiIcon )
		SetFalse( uiBuilding )
	end

	
	if GetPlayer():CityLevel() >= info.level then
		SetTrue( uiGotoButton )
		SetFalse( uiWarn )
	else
		SetFalse( uiGotoButton )
		SetTrue( uiWarn )
		local msg = ""
		if info.cond_kind == 1 then
			msg = msg .. F( 2465, info.cond_level )
		elseif info.cond_kind == 4 then
			msg = msg .. F( 2467, info.cond_level )
		end
		if info.actor_level > 0 then
			msg = msg .." ".. F( 2466, info.actor_level )
		end
		SetText( uiWarn, msg )
	end 
	
end

function Activity2ModScrollPagePre()
	local scrollPage = m_uiScrollView.transform:GetComponent( typeof(UIScrollPage) )
	scrollPage:Pre()
end

function Activity2ModScrollPageNext()
	local scrollPage = m_uiScrollView.transform:GetComponent( typeof(UIScrollPage) )
	scrollPage:Next()
end

function Activity2ModScrollPage( page )
	local id = page+1
	if page == 0 then
		SetFalse( m_uiLeftBtn )
	else
		SetTrue( m_uiLeftBtn )
	end
	
	if page == #g_activity_02-1 then
		SetFalse( m_uiRightBtn )
	else
		SetTrue( m_uiRightBtn )
	end
	
	-- 奖励
	local info = g_activity_02[id]
	local awardkind = { info.awardkind0, info.awardkind1, info.awardkind2, info.awardkind3, info.awardkind4 }
	local awardnum = { info.awardnum0, info.awardnum1, info.awardnum2, info.awardnum3, info.awardnum4 }
	for i=1, 3, 1 do
		local awardObj = m_uiAwardList.transform:GetChild(i-1);
		if awardkind[i] > 0 then
			local sprite, color, name = AwardInfo( awardkind[i] )
			SetTrue( awardObj )
			SetControlID( awardObj, 1000000+awardkind[i] )
			SetImage( awardObj.transform:Find("Shape"), sprite );
			if awardnum[i] > 1 then
				SetTrue( awardObj.transform:Find("NumBack") )
				SetText( awardObj.transform:Find("Num"), knum(awardnum[i]) );
			else
				SetFalse( awardObj.transform:Find("NumBack") )
				SetText( awardObj.transform:Find("Num"), "" );
			end
		else
			SetFalse( awardObj )
		end
	end
	
	-- 已经领取
	if Utils.get_int_sflag( m_state, id ) == 1 then
		SetFalse( m_uiGetButton )
		SetFalse( m_uiUnLockState )
		SetTrue( m_uiGetState )
	else
		if GetPlayer():CityLevel() >= g_activity_02[id].level then
			SetTrue( m_uiGetButton )
			SetFalse( m_uiUnLockState )
			SetFalse( m_uiGetState )
			SetControlID( m_uiGetButton, 2000 + id )
			m_red = 1
		else
			SetTrue( m_uiUnLockState )
			SetText( m_uiUnLockState, F(2464,g_activity_02[id].level) )
			SetFalse( m_uiGetButton )
			SetFalse( m_uiGetState )
		end
	end
end

-- 领取奖励
function Activity2ModSelect( id )
	if GetPlayer():CityLevel() < g_activity_02[id].level then
		return	
	end
	if Utils.get_int_sflag( m_state, id ) == 1 then
		return
	end
	system_askinfo( ASKINFO_ACTIVITY, "", ACTIVITY_2, 1, id )
end

-- 前往
function Activity2ModGoto( id )
	ActivityDlgClose()
	if id == 1 then
		City.Move( BUILDING_Shop, -1, true );
	elseif id == 2 then
		City.Move( BUILDING_Smithy, -1, true );
	elseif id == 3 then
		City.Move( BUILDING_Cavalry, -1, true );
	elseif id == 4 then
		City.Move( BUILDING_Tech, -1, true );
	elseif id == 5 then
		City.Move( BUILDING_Tech, -1, true );
	elseif id == 6 then
		City.Move( BUILDING_Tech, -1, true );
	elseif id == 7 then
		City.Move( BUILDING_Wash, -1, true );
	elseif id == 8 then
		NationEquipDlgShow()
	elseif id == 9 then
		City.Move( BUILDING_Craftsman, -1, true );
	elseif id == 10 then
		City.Move( BUILDING_Main, -1, true );
	elseif id == 11 then
		City.Move( BUILDING_Tech, -1, true );
	elseif id == 12 then
		City.Move( BUILDING_Craftsman, -1, true );
	elseif id == 13 then
		City.Move( BUILDING_Tech, -1, true );
	elseif id == 14 then
		City.Move( BUILDING_Cabinet, -1, true );
	elseif id == 15 then
		City.Move( BUILDING_Hero, -1, true );
	elseif id == 16 then
		City.Move( BUILDING_Craftsman, -1, true );
	elseif id == 17 then
		City.Move( BUILDING_Cabinet, -1, true );
	elseif id == 18 then
		City.Move( BUILDING_Militiaman_Infantry, -1, true );
	elseif id == 19 then
		City.Move( BUILDING_Cabinet, -1, true );
	elseif id == 20 then
		City.Move( BUILDING_Fangshi, -1, true );
	elseif id == 21 then
		City.Move( BUILDING_Cabinet, -1, true );
	elseif id == 22 then
		City.Move( BUILDING_Tech, -1, true );
	elseif id == 23 then
		City.Move( BUILDING_Tech, -1, true );
	elseif id == 24 then
		City.Move( BUILDING_Tech, -1, true );
	elseif id == 25 then
		City.Move( BUILDING_Tech, -1, true );
	elseif id == 26 then
		City.Move( BUILDING_Tech, -1, true );
	elseif id == 27 then
		City.Move( BUILDING_Tech, -1, true );
	elseif id == 28 then
		City.Move( BUILDING_Tech, -1, true );
	end
end

-- 奖励描述
function Activity2ModAwardDescLayer()
	SetFalse( m_uiAwardDescLayer )
	m_AwardDescLayerShow = false
end
function Activity2ModClickItem( awardkind, uiObj )
	Activity2ModAwardDescLayer()
	if awardkind > AWARDKIND_HEROBASE and awardkind < AWARDKIND_BUILDINGBASE then
		local herokind = awardkind - AWARDKIND_HEROBASE
		local color = hero_getnormalcolor( herokind )
		HeroConfigDlgShow( g_heroinfo[herokind][color], 0 );
	else
		m_uiAwardDescLayer.transform:SetParent( uiObj.transform )
		m_uiAwardDescLayer.transform.anchoredPosition = Vector2( 0, 100 )
		m_uiAwardDescLayer.transform:SetParent( m_Mod.transform )
		local _, _, name, c, desc = AwardInfo( awardkind )
		SetText( m_uiAwardDescLayer.transform:Find("Name"), name, NameColor(c) )	
		local _desc = string.split( desc, "\n")
		if _desc ~= nil and _desc[1] ~= nil then
			SetText( m_uiAwardDescLayer.transform:Find("Desc"), _desc[1] )
		else
			SetText( m_uiAwardDescLayer.transform:Find("Desc"), desc )
		end
		SetText( m_uiAwardDescLayer.transform:Find("Warn"), "" )
		SetTrue( m_uiAwardDescLayer )
		m_AwardDescLayerShow = true
	end
end