local m_Mod;
local m_uiBack = nil; --UnityEngine.GameObject
local m_uiShape = nil; --UnityEngine.GameObject
local m_uiActivityTime = nil; --UnityEngine.GameObject
local m_uiTitle = nil; --UnityEngine.GameObject
local m_uiItemInfo = nil; --UnityEngine.GameObject
local m_uiShopList = nil; --UnityEngine.GameObject
local m_uiWarnList = nil; --UnityEngine.GameObject
local m_uiAwardDescLayer = nil; --UnityEngine.GameObject
local m_uiBack1 = nil; --UnityEngine.GameObject
local m_recvValue = nil
local m_AwardDescLayerShow = false
----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function Activity5ModOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
		--print( "Button Clicked, nControlID:" .. nControlID );
		-- 招募武将
		if nControlID == -2 then
			Activity5ModAwardDescLayer();
		elseif nControlID == 1 then
			Activity5ModUpdate()
		-- 刷新
		elseif nControlID == 2 then
			Activity5ModCallHero()
		-- 购买	
		elseif nControlID >= 1000 and nControlID < 2000 then
			Activity5ModBuy( nControlID-1000 )
		elseif nControlID >= 2000 and nControlID < 3000 then
			Activity5ModClickItem( nControlID-2000, value )
		end
	elseif nType == UI_EVENT_PRESS then
		if value == 0 then
			--print( "Button Pressed Down, nControlID:" .. nControlID );
		elseif value == 1 then
			--print( "Button Pressed UP, nControlID:" .. nControlID);
		end
	elseif nType == UI_EVENT_SCROLLDRAG then
		if nControlID == 0 then
			if m_AwardDescLayerShow == true then
				Activity5ModAwardDescLayer()
			end
		end
	elseif nType == UI_EVENT_TIMECOUNTEND then
		if nControlID == 0 then	
			system_askinfo( ASKINFO_ACTIVITY, "", ACTIVITY_5, 0 )
		end
	end
end

-- 载入时调用
function Activity5ModOnAwake( gameObject )
	m_Mod = gameObject;
	local objs = gameObject:GetComponent( "UIMod" ).relatedGameObject;
	m_uiBack = objs[0];
	m_uiShape = objs[1];
	m_uiActivityTime = objs[2];
	m_uiTitle = objs[3];
	m_uiItemInfo = objs[4];
	m_uiShopList = objs[5];
	m_uiWarnList = objs[6];
	m_uiAwardDescLayer = objs[7];
	m_uiBack1 = objs[8];
end

-- 界面初始化时调用
function Activity5ModOnStart( gameObject )
	system_askinfo( ASKINFO_ACTIVITY, "", ACTIVITY_5, 0 )
	ResourceManager.LoadAssetBundle( "_ab_activity_pic_6" )
	ResourceManager.LoadAssetBundle( "_ab_activity_back_4" )
	
	SetImage( m_uiBack, LoadSprite("activity_pic_6") )
	SetImage( m_uiBack1, LoadSprite("activity_back_4") )
	SetImage( m_uiShape, LoadSprite("heroface_49") )
	m_AwardDescLayerShow = false
end

-- 界面显示时调用
function Activity5ModOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function Activity5ModOnDisable( gameObject )
	
end

-- 界面删除时调用
function Activity5ModOnDestroy( gameObject )
	m_Mod = nil
	m_recvValue = nil
	ResourceManager.UnloadAssetBundle( "_ab_activity_pic_6" )
	ResourceManager.UnloadAssetBundle( "_ab_activity_back_4" )
end

-- 每帧调用
function Activity5ModOnLogic( gameObject )
	
end

function Activity5ModGet()
	return m_Mod;
end
----------------------------------------
-- 自定
----------------------------------------
-- m_count=0,m_list={m_awardkind=0,m_awardnum=0,m_token=0,m_isbuy=0,[m_count]},m_buynum=0,m_updatenum=0,m_updatestamp=0,m_myxw=0
function Activity5ModRecv( recvValue )
	if m_Mod == nil then
		return
	end
	local ActivityList = ActivityDlgGetRecvValue()
	local info = nil;
	for i=1, #ActivityList, 1 do
		if ActivityList[i].m_activityid == ACTIVITY_5 then
			info = ActivityList[i];
			break
		end
	end
	if info == nil then
		return
	end
	m_recvValue = recvValue
	-- 活动时间
	local starttime = getMonthDayHourMinStringByInterval_zh( info.m_starttime )
	local endtime = getMonthDayHourMinStringByInterval_zh( info.m_endtime )
	SetText( m_uiActivityTime.transform:Find("Text"), F( 2475, starttime, endtime ) )
	
	-- 信物个数
	SetText( m_uiItemInfo.transform:Find("Name"), F(2473,recvValue.m_myxw,7) )
	
	-- 刷新次数
	SetText( m_uiWarnList.transform:Find("Num"), F(2471,recvValue.m_updatenum,4) )
	
	-- 刷新时间
	if recvValue.m_updatenum >= 4 then
		SetFalse( m_uiWarnList.transform:Find("Timer") )
	else
		SetTrue( m_uiWarnList.transform:Find("Timer") )
		SetTimer( m_uiWarnList.transform:Find("Timer"), recvValue.m_updatestamp-1, recvValue.m_updatestamp, 0, T(2472) )
	end
	
	local sale = 0;
	if recvValue.m_buynum == 1 then
		sale = 90
	elseif recvValue.m_buynum == 2 then
		sale = 80
	elseif recvValue.m_buynum == 3 then
		sale = 70
	elseif recvValue.m_buynum == 4 then
		sale = 60
	elseif recvValue.m_buynum >= 5 then
		sale = 50
	end
	-- 商品数量
	for i=1, recvValue.m_count, 1 do
		local uiObj = m_uiShopList.transform:GetChild(i-1);
		local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
		local uiShape = objs[0]
		local uiColor = objs[1]
		local uiNum = objs[2]
		local uiHasBuy = objs[3]
		local uiBuyButton = objs[4]
		local uiToken = objs[5]
		local uiSale = objs[6]
		local uiSaleText = objs[7]
		
		local sprite, color, name, c, desc = AwardInfo( recvValue.m_list[i].m_awardkind )
		SetImage( uiShape, sprite );
		SetImage( uiColor, color );
		SetText( uiToken, recvValue.m_list[i].m_token )	
		SetControlID( uiObj, 2000+i )
		SetControlID( uiBuyButton, 1000+i )
		SetText( uiNum, recvValue.m_list[i].m_awardnum )
		
		if recvValue.m_list[i].m_isbuy == 1 then
			SetFalse( uiBuyButton )
			SetTrue( uiHasBuy )
		else
			SetTrue( uiBuyButton )
			SetFalse( uiHasBuy )
		end
		if sale > 0 and recvValue.m_list[i].m_isbuy == 0 then
			SetTrue( uiSale )
			SetText( uiSaleText, sale.."%")
		else
			SetFalse( uiSale )
		end
	end
end

-- 招募武将
function Activity5ModCallHero()
	if m_recvValue.m_myxw < 7 then
		AlertMsg( T(2476) )
		return
	end
	system_askinfo( ASKINFO_ACTIVITY, "", ACTIVITY_5, 3 )
end

-- 刷新
function Activity5ModUpdate()
	if m_recvValue.m_updatenum <= 0 then
		MsgBox( F(2477,global.activity05_update_token), function()
			system_askinfo( ASKINFO_ACTIVITY, "", ACTIVITY_5, 2 )
		end )
		return
	end
	system_askinfo( ASKINFO_ACTIVITY, "", ACTIVITY_5, 2 )
end

-- 购买	
function Activity5ModBuy( index )
	local info = m_recvValue.m_list[index];
	if info == nil then
		return
	end
	if GetPlayer().m_token < info.m_token then
		JumpToken()
		return
	end
	local sprite, color, name, c, desc = AwardInfo( info.m_awardkind )
	MsgBox( F(1778, info.m_token, name, info.m_awardnum ), function()
		system_askinfo( ASKINFO_ACTIVITY, "", ACTIVITY_5, 1, index-1 )
	end )
end

-- 奖励描述
function Activity5ModAwardDescLayer()
	SetFalse( m_uiAwardDescLayer )
	m_AwardDescLayerShow = false
end
function Activity5ModClickItem( index, uiObj )	
	Activity5ModAwardDescLayer()
	local info = m_recvValue.m_list[index];
	if info == nil then
		return
	end
	local awardkind = info.m_awardkind
	if awardkind > AWARDKIND_HEROBASE and awardkind < AWARDKIND_BUILDINGBASE then
		local herokind = awardkind - AWARDKIND_HEROBASE
		local color = hero_getnormalcolor( herokind )
		HeroConfigDlgShow( g_heroinfo[herokind][color], 0 );
	else
		m_uiAwardDescLayer.transform:SetParent( uiObj.transform )
		m_uiAwardDescLayer.transform.anchoredPosition = Vector2( 0, 200 )
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
