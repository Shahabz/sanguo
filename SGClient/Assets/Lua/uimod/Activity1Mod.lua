local m_Mod;
local m_uiBack = nil; --UnityEngine.GameObject
local m_uiShape = nil; --UnityEngine.GameObject
local m_uiActivityTime = nil; --UnityEngine.GameObject
local m_uiTitle = nil; --UnityEngine.GameObject
local m_uiAwardList = nil; --UnityEngine.GameObject
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
function Activity1ModOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
		--print( "Button Clicked, nControlID:" .. nControlID );
		if nControlID == -2 then
			Activity1ModAwardDescLayer();
		elseif nControlID == 1 then
			PayDlgShow()
		elseif nControlID == 2 then
			Activity1ModGet()
		elseif nControlID >= 1000000 and nControlID < 2000000 then
			Activity1ModClickItem( nControlID-1000000, value )
		end
	elseif nType == UI_EVENT_PRESS then
		if value == 0 then
			--print( "Button Pressed Down, nControlID:" .. nControlID );
		elseif value == 1 then
			--print( "Button Pressed UP, nControlID:" .. nControlID);
		end
	elseif nType == UI_EVENT_TIMECOUNTEND then
		if nControlID == 0 then	
			system_askinfo( ASKINFO_ACTIVITY, "", ACTIVITY_25, 0 )
		end
	end
end

-- 载入时调用
function Activity1ModOnAwake( gameObject )
	m_Mod = gameObject;
	local objs = gameObject:GetComponent( "UIMod" ).relatedGameObject;
	m_uiBack = objs[0];
	m_uiShape = objs[1];
	m_uiActivityTime = objs[2];
	m_uiTitle = objs[3];
	m_uiAwardList = objs[4];
	m_uiPayButton = objs[5];
	m_uiGetButton = objs[6];
	m_uiAwardDescLayer = objs[7];
	m_uiHasGet = objs[8];
end

-- 界面初始化时调用
function Activity1ModOnStart( gameObject )
	system_askinfo( ASKINFO_ACTIVITY, "", ACTIVITY_1, 0 )
	ResourceManager.LoadAssetBundle( "_ab_activity_pic_1" )
	ResourceManager.LoadAssetBundle( "_ab_ui_static_npc2" )
	
	SetImage( m_uiBack, LoadSprite("activity_pic_1") )
	SetImage( m_uiShape, LoadSprite("ui_static_npc2") )
	m_AwardDescLayerShow = false
end

-- 界面显示时调用
function Activity1ModOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function Activity1ModOnDisable( gameObject )
	
end

-- 界面删除时调用
function Activity1ModOnDestroy( gameObject )
	m_Mod = nil
	ResourceManager.UnloadAssetBundle( "_ab_activity_pic_1" )
	ResourceManager.UnloadAssetBundle( "_ab_ui_static_npc2" )
end

-- 每帧调用
function Activity1ModOnLogic( gameObject )
	
end

function Activity1ModGet()
	return m_Mod;
end
----------------------------------------
-- 自定
----------------------------------------
-- m_count=0,m_list={m_kind=0,m_num=0,[m_count]},m_fristpay=0,m_fristpay_award=0,
function Activity1ModRecv( recvValue )
	if m_Mod == nil then
		return
	end
	m_red = 0;
	local ActivityList = ActivityDlgGetRecvValue()
	if ActivityList == nil then
		return
	end
	local info = nil;
	for i=1, #ActivityList, 1 do
		if ActivityList[i].m_activityid == ACTIVITY_1 then
			info = ActivityList[i];
			break
		end
	end
	if info == nil then
		return
	end
	-- 活动时间
	--local left = info.m_endtime-info.m_starttime
	--SetTimer( m_uiActivityTime.transform:Find("Text"), info.m_endtime-GetServerTime(), left, 0, T(2483) )
	--SetText( m_uiActivityTime.transform:Find("Text"), T(2451) )

	-- 活动状态
	if recvValue.m_fristpay == 0 then
		SetTrue( m_uiPayButton )
		SetFalse( m_uiGetButton )
		SetFalse( m_uiHasGet )
	else
		if recvValue.m_fristpay_award == 1 then -- 已领取
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
	
	-- 奖励
	SetTrue( m_uiAwardList )
	for i=1, 4, 1 do
		local awardObj = m_uiAwardList.transform:GetChild(i-1);
		if recvValue.m_list[i].m_kind > 0 then
			local sprite, color, name = AwardInfo( recvValue.m_list[i].m_kind )
			SetTrue( awardObj )
			SetControlID( awardObj, 1000000+recvValue.m_list[i].m_kind )
			SetImage( awardObj.transform:Find("Shape"), sprite );
			if recvValue.m_list[i].m_num > 1 then
				SetText( awardObj.transform:Find("Num"), "x"..knum(recvValue.m_list[i].m_num) );
			else
				SetText( awardObj.transform:Find("Num"), "" );
			end
		else
			SetFalse( awardObj )
		end
	end 
	
	-- 红点
	ActivityDlgChangeRed( ACTIVITY_1, m_red )
end

function Activity1ModGet()
	system_askinfo( ASKINFO_ACTIVITY, "", ACTIVITY_1, 1 )
end

-- 奖励描述
function Activity1ModAwardDescLayer()
	SetFalse( m_uiAwardDescLayer )
	m_AwardDescLayerShow = false
end
function Activity1ModClickItem( awardkind, uiObj )	
	Activity1ModAwardDescLayer()
	if awardkind > AWARDKIND_HEROBASE and awardkind < AWARDKIND_BUILDINGBASE then
		local herokind = awardkind - AWARDKIND_HEROBASE
		local color = hero_getnormalcolor( herokind )
		HeroConfigDlgShow( g_heroinfo[herokind][color], 0 );
	else
		m_uiAwardDescLayer.transform:SetParent( uiObj.transform )
		m_uiAwardDescLayer.transform.anchoredPosition = Vector2( -100, 100 )
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
