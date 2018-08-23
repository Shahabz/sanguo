local m_Mod;
local m_uiBack = nil; --UnityEngine.GameObject
local m_uiActivityTime = nil; --UnityEngine.GameObject
local m_uiViewListBtn = nil; --UnityEngine.GameObject
local m_uiViewAwardBtn = nil; --UnityEngine.GameObject
local m_uiActorList = nil; --UnityEngine.GameObject
local m_uiAwardScrollView = nil; --UnityEngine.GameObject
local m_uiAwardContent = nil; --UnityEngine.GameObject
local m_uiAwardDescLayer = nil; --UnityEngine.GameObject
local m_uiMyRank = nil; --UnityEngine.GameObject
local m_uiMyPay = nil; --UnityEngine.GameObject
local m_recvValue = nil
local m_AwardDescLayerShow = false
----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function Activity33ModOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
		--print( "Button Clicked, nControlID:" .. nControlID );
		if nControlID == 1 then
			Activity33ModShowRankList()
		elseif nControlID == 2 then
			Activity33ModShowAwardList()
		elseif nControlID >= 1000000 and nControlID < 2000000 then
			Activity33ModClickItem( nControlID-1000000, value )
		end
	elseif nType == UI_EVENT_PRESS then
		if value == 0 then
			--print( "Button Pressed Down, nControlID:" .. nControlID );
		elseif value == 1 then
			--print( "Button Pressed UP, nControlID:" .. nControlID);
		end
	elseif nType == UI_EVENT_SCROLLDRAG then
		if nControlID == 1 then
			if m_AwardDescLayerShow == true then
				Activity33ModAwardDescLayer()
			end
		end
	end
end

-- 载入时调用
function Activity33ModOnAwake( gameObject )
	m_Mod = gameObject;
	local objs = gameObject:GetComponent( "UIMod" ).relatedGameObject;
	m_uiBack = objs[0];
	m_uiActivityTime = objs[1];
	m_uiViewListBtn = objs[2];
	m_uiViewAwardBtn = objs[3];
	m_uiActorList = objs[4];
	m_uiAwardScrollView = objs[5];
	m_uiAwardContent = objs[6];
	m_uiAwardDescLayer = objs[7];
	m_uiMyRank = objs[8];
	m_uiMyPay = objs[9];
end

-- 界面初始化时调用
function Activity33ModOnStart( gameObject )
	ResourceManager.LoadAssetBundle( "_ab_activity_pic_7" )
	ResourceManager.LoadAssetBundle( "_ab_activity_back_5" )
	SetImage( m_uiBack, LoadSprite("activity_pic_7") )
	SetFalse( m_uiActorList )
	SetFalse( m_uiAwardScrollView )
	SetFalse( m_uiMyRank )
	SetFalse( m_uiMyPay )
	system_askinfo( ASKINFO_ACTIVITY, "", ACTIVITY_33, 0 )
	m_AwardDescLayerShow = false
end

-- 界面显示时调用
function Activity33ModOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function Activity33ModOnDisable( gameObject )
	
end

-- 界面删除时调用
function Activity33ModOnDestroy( gameObject )
	m_Mod = nil
	ResourceManager.UnloadAssetBundle( "_ab_activity_pic_7" )
	ResourceManager.UnloadAssetBundle( "_ab_activity_back_5" )
end

-- 每帧调用
function Activity33ModOnLogic( gameObject )
	
end

function Activity33ModGet()
	return m_Mod;
end
----------------------------------------
-- 自定
----------------------------------------
-- m_count=0,m_list={m_nation=0,m_namelen=0,m_name="[m_namelen]",m_pay=0,[m_count]},m_myrank=0,m_awardcount=0,m_awardlist={m_awardkind={[5]},m_awardnum={[5]},[m_awardcount]},m_mypay=0,
function Activity33ModRecv( recvValue )
	if m_Mod == nil then
		return
	end
	m_recvValue = recvValue
	local ActivityList = ActivityDlgGetRecvValue()
	if ActivityList == nil then
		return
	end
	local info = nil;
	for i=1, #ActivityList, 1 do
		if ActivityList[i].m_activityid == ACTIVITY_33 then
			info = ActivityList[i];
			break
		end
	end
	if info == nil then
		return
	end
	-- 活动时间
	local starttime = getMonthDayHourMinStringByInterval_zh( info.m_starttime )
	local endtime = getMonthDayHourMinStringByInterval_zh( info.m_endtime )
	SetText( m_uiActivityTime.transform:Find("Text"), F( 2475, starttime, endtime ) )
	
	SetTrue( m_uiMyRank )
	SetTrue( m_uiMyPay )
	if recvValue.m_myrank == -1 then
		SetText( m_uiMyRank, F(4242,T(240)) )
	else
		SetText( m_uiMyRank, F(4242,recvValue.m_myrank+1) )
	end
	SetText( m_uiMyPay, F(4243,recvValue.m_mypay) )
	
	Activity33ModShowRankList()
	--Activity33ModShowAwardList()
	--Activity33ModShowRankList()
end

-- 显示排名
function Activity33ModShowRankList()
	Activity33ModAwardDescLayer()
	SetTrue( m_uiViewAwardBtn )
	SetFalse( m_uiViewListBtn )
	SetTrue( m_uiActorList )
	SetFalse( m_uiAwardScrollView )
	local listcount = m_uiActorList.transform.childCount
	for i=0,listcount-1,1 do
		local obj = m_uiActorList.transform:GetChild(i).gameObject
		SetTrue( obj )
		SetFalse( obj.transform:Find("Nation") )
		SetText( obj.transform:Find("Name"), "-" )
		SetText( obj.transform:Find("Pay"), "0" )
	end
	for i=1,m_recvValue.m_count,1 do
		local obj = m_uiActorList.transform:GetChild(i-1).gameObject
		SetTrue( obj.transform:Find("Nation") )
		SetImage( obj.transform:Find("Nation"), NationSprite( m_recvValue.m_list[i].m_nation ) )
		SetText( obj.transform:Find("Name"), m_recvValue.m_list[i].m_name )
		SetText( obj.transform:Find("Pay"), m_recvValue.m_list[i].m_pay )
	end
end

-- 显示奖励
function Activity33ModShowAwardList()
	Activity33ModAwardDescLayer()
	SetFalse( m_uiViewAwardBtn )
	SetTrue( m_uiViewListBtn )
	SetFalse( m_uiActorList )
	SetTrue( m_uiAwardScrollView )
	
	for i=1,m_recvValue.m_awardcount,1 do
		local obj = m_uiAwardContent.transform:GetChild(i-1).gameObject
		SetImage( obj.transform:Find("Back"), LoadSprite("activity_back_5") )
		local awardObj = obj.transform:Find("AwardList")
		for j=1,4,1 do
			local awardkind = m_recvValue.m_awardlist[i].m_awardkind[j]
			local awardnum = m_recvValue.m_awardlist[i].m_awardnum[j]
			local sprite, color, name = AwardInfo( awardkind )
			
			local awardChildObj = awardObj.transform:GetChild(j-1).gameObject
			SetControlID( awardChildObj, 1000000+awardkind )
			SetImage( awardChildObj.transform:Find("Shape"), sprite );
			if awardnum > 1 then
				SetText( awardChildObj.transform:Find("Num"), knum(awardnum) );
				SetTrue( awardChildObj.transform:Find("NumBack") )
			else
				SetFalse( awardChildObj.transform:Find("NumBack") )
				SetText( awardChildObj.transform:Find("Num"), "" );
			end
		end
	end
end

-- 奖励描述
function Activity33ModAwardDescLayer()
	SetFalse( m_uiAwardDescLayer )
	m_AwardDescLayerShow = false
end
function Activity33ModClickItem( awardkind, uiObj )
	Activity33ModAwardDescLayer()
	if awardkind > AWARDKIND_HEROBASE and awardkind < AWARDKIND_BUILDINGBASE then
		local herokind = awardkind - AWARDKIND_HEROBASE
		local color = hero_getnormalcolor( herokind )
		HeroConfigDlgShow( g_heroinfo[herokind][color], 0 );
	else
		m_uiAwardDescLayer.transform:SetParent( uiObj.transform )
		m_uiAwardDescLayer.transform.anchoredPosition = Vector2( -60, 80 )
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