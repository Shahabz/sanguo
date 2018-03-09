local m_Mod;
local m_uiBack = nil; --UnityEngine.GameObject
local m_uiShape = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiUIP_Mission = nil; --UnityEngine.GameObject
local m_uiTypeButtons = nil; --UnityEngine.GameObject
local m_uiDay = nil; --UnityEngine.GameObject
local m_uiAwardDescLayer = nil; --UnityEngine.GameObject
local m_uiScrollView = nil; --UnityEngine.GameObject
local m_recvValue = nil;
local m_selectDay = 0;
local m_selectKind = 0;
local m_AwardDescLayerShow = false
local m_Activity4Title = { 
[1] = { 2681, 2682, 2683 },
[2] = { 2681, 2684, 2685 },
[3] = { 2681, 2686, 2687 },
[4] = { 2681, 2688, 2689 },
[5] = { 2681, 2690, 2691 },
[6] = { 2681, 2692, 2693 },
[7] = { 2681, 2694, 2695 },
}
----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function Activity4ModOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
		--print( "Button Clicked, nControlID:" .. nControlID );
		if nControlID == -2 then
			Activity4ModAwardDescLayer();
		elseif nControlID >= 1 and nControlID <= 7 then
			Activity4ModSelectDay( nControlID )
		elseif nControlID >= 100 and nControlID < 110 then
			Activity4ModSelectKind( nControlID-100 )
		elseif nControlID >= 1000 and nControlID < 2000 then
			Activity4ModSelect( nControlID-1000 )
		elseif nControlID >= 2000 and nControlID < 3000 then
			Activity4ModGoto( nControlID-2000 )
		elseif nControlID >= 1000000 and nControlID < 2000000 then
			Activity4ModClickItem( nControlID-1000000, value )
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
				Activity4ModAwardDescLayer()
			end
		end
	end
end

-- 载入时调用
function Activity4ModOnAwake( gameObject )
	m_Mod = gameObject;
	local objs = gameObject:GetComponent( "UIMod" ).relatedGameObject;
	m_uiBack = objs[0];
	m_uiShape = objs[1];
	m_uiContent = objs[2];
	m_uiUIP_Mission = objs[3];
	m_uiTypeButtons = objs[4];
	m_uiDay = objs[5];
	m_uiAwardDescLayer = objs[6];
	m_uiScrollView = objs[7];
	-- 对象池
	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );
	m_ObjectPool:CreatePool("UIP_Mission", 5, 5, m_uiUIP_Mission);
end

-- 界面初始化时调用
function Activity4ModOnStart( gameObject )
	system_askinfo( ASKINFO_ACTIVITY, "", ACTIVITY_4, 0 )
	ResourceManager.LoadAssetBundle( "_ab_activity_pic_5" )
	ResourceManager.LoadAssetBundle( "_ab_activity_back_1" )
	ResourceManager.LoadAssetBundle( "_ab_activity_back_2" )
	ResourceManager.LoadAssetBundle( "_ab_activity_back_5" )
	
	SetImage( m_uiBack, LoadSprite("activity_pic_5") )
	SetImage( m_uiShape, LoadSprite("heroface_46") )
	m_selectDay = 0;
	m_selectKind = 0;
	m_AwardDescLayerShow = false
end

-- 界面显示时调用
function Activity4ModOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function Activity4ModOnDisable( gameObject )
	
end

-- 界面删除时调用
function Activity4ModOnDestroy( gameObject )
	m_Mod = nil
	m_recvValue = nil;
	ResourceManager.UnloadAssetBundle( "_ab_activity_pic_5" )
	ResourceManager.UnloadAssetBundle( "_ab_activity_back_1" )
	ResourceManager.UnloadAssetBundle( "_ab_activity_back_2" )
	ResourceManager.UnloadAssetBundle( "_ab_activity_back_5" )
end

-- 每帧调用
function Activity4ModOnLogic( gameObject )
	
end

function Activity4ModGet()
	return m_Mod;
end
----------------------------------------
-- 自定
----------------------------------------
-- m_count=0,m_list={m_value=0,m_state=0,[m_count]},m_myday=0,
function Activity4ModRecv( recvValue )
	if m_Mod == nil then
		return
	end
	m_recvValue = recvValue
	
	if m_selectDay > 0 then
		Activity4ModSetList( m_selectDay, m_selectKind )
	else
		Activity4ModSelectDay( recvValue.m_myday )
	end
end

function Activity4ModSelectDay( day )
	if day > m_recvValue.m_myday then
		pop(F(2487,day))
		return
	end
	if m_selectDay == day then
		return
	end
	m_selectDay = day;
	m_selectKind = 1;
	Activity4ModSetList( day, m_selectKind )
end

function Activity4ModSelectKind( kind )
	if m_selectKind == kind then
		return
	end
	m_selectKind = kind;
	Activity4ModSetList( m_selectDay, kind )
end

-- 设置列表
function Activity4ModSetList( day, kind )
	Activity4ModClear()
	-- 默认选择天
	for i=1, 7, 1 do
		local uiButton = m_uiDay.transform:GetChild(i+1)
		SetText( uiButton.transform:Find("Back/Text"), T(2780+i) )
		if day == i then
			SetImage( uiButton.transform:Find("Back"), LoadSprite("activity_back_2") )
		else
			SetImage( uiButton.transform:Find("Back"), LoadSprite("activity_back_1") )
		end
		if i <= m_recvValue.m_myday then
			SetFalse( uiButton.transform:Find("Back/Lock") )
		else
			SetTrue( uiButton.transform:Find("Back/Lock") )
		end
	end
	-- 分类名称
	for i=1, 3, 1 do
		local uiButton = m_uiTypeButtons.transform:GetChild(i-1)
		SetText( uiButton.transform:Find("Back/Text"), T(m_Activity4Title[day][i]) )
		SetControlID( uiButton, 100+i )
		if kind == i then
			SetImage( uiButton.transform:Find("Back"), LoadSprite("ui_button_page1") )
		else
			SetImage( uiButton.transform:Find("Back"), LoadSprite("ui_button_page2") )
		end
	end
	
	local tmptable = {}
	for i=1, #g_activity_04, 1 do
		local state = m_recvValue.m_list[i].m_state
		local value = m_recvValue.m_list[i].m_value
		local rank = 0;
		if value >= g_activity_04[i].value or ( g_activity_04[i].type == 2 and value >= 1 ) then
			if state == 1 then-- 完成已经领取
				rank = -1000 - g_activity_04[i].id
			else -- 完成未领取
				rank = 0
			end
		else
			rank = - g_activity_04[i].id
		end
		table.insert( tmptable, {state=state, value=value, rank=rank, info=g_activity_04[i]} )
	end
	table.sort( tmptable, function(a,b) 
		if a.rank > b.rank then
			return true
		else
			return false
		end
	end )
	
	for id=1, #tmptable, 1 do
		if tmptable[id].info.day == day and tmptable[id].info.title == m_Activity4Title[day][kind] then
			Activity4ModCreate( tmptable[id].info, tmptable[id].value, tmptable[id].state )
		end
	end
end

-- 创建
function Activity4ModCreate( info, value, state )
	local uiObj = m_ObjectPool:Get("UIP_Mission");
	uiObj.transform:SetParent( m_uiContent.transform );
	uiObj.transform.localScale = Vector3.one;
	
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiName = objs[0]
	local uiValue = objs[1]
	local uiAwardList = objs[2]
	local uiStateBtn = objs[3]
	local uiBack = objs[4]
	local uiState = objs[5]
	local uiGotoBtn = objs[6]
	SetControlID( uiStateBtn, 1000 + info.id )
	SetImage( uiBack, LoadSprite("activity_back_5") )
	local name = ""
	if info.type == 2 then -- 科技
		name = F(2700+info.type, TechName(info.value))
	elseif info.type == 14 then -- 副本
		name = F(2700+info.type, g_story[info.value].chapter)
	else
		name = F(2700+info.type, info.value)
	end
	if info.guideid > 0 then
		name = name..T(info.guideid)
	end
	SetText( uiName, name )
	
	if info.type == 2 or info.type == 14 then
		SetText( uiValue, "" )
	elseif info.type == 7 or info.type == 39 then
		if value > 1 then
			SetText( uiValue, "1/1" )
		else
			SetText( uiValue, "0/1" )
		end
	else
		SetText( uiValue, value.."/"..info.value )
	end
	
	if value >= info.value or ( info.type == 2 and value >= 1 ) then
		if state == 1 then-- 完成已经领取
			SetText( uiStateBtn.transform:Find("Back/Text"), T(1109) )
			SetFalse( uiStateBtn )
			SetTrue( uiState )
		else -- 完成没有领取
			SetText( uiStateBtn.transform:Find("Back/Text"), T(1351) )
			SetTrue( uiStateBtn )
			SetButtonTrue( uiStateBtn )
			SetFalse( uiState )
		end
		SetFalse( uiGotoBtn )
	else
		if info.gototype > 0 then
			SetTrue( uiGotoBtn )
			SetControlID( uiGotoBtn, 2000+info.gototype )
		else
			SetText( uiStateBtn.transform:Find("Back/Text"), T(2452) )
			SetTrue( uiStateBtn )
			SetButtonFalse( uiStateBtn )
			SetFalse( uiState )
			SetFalse( uiGotoBtn )
		end
	end
	
	-- 奖励
	local awardkind = { info.awardkind0, info.awardkind1, info.awardkind2 }
	local awardnum = { info.awardnum0, info.awardnum1, info.awardnum2 }
	for i=1, 3, 1 do
		local awardObj = uiAwardList.transform:GetChild(i-1);
		if awardkind[i] > 0 then
			local sprite, color, name = AwardInfo( awardkind[i] )
			SetTrue( awardObj )
			SetControlID( awardObj, 1000000+awardkind[i] )
			SetImage( awardObj.transform:Find("Shape"), sprite );
			if awardnum[i] > 1 then
				SetText( awardObj.transform:Find("Num"), "x"..knum(awardnum[i]) );
			else
				SetText( awardObj.transform:Find("Num"), "" );
			end
		else
			SetFalse( awardObj )
		end
	end
end

-- 清空
function Activity4ModClear()
	local objs = {};
	for i=0,m_uiContent.transform.childCount-1 do
		table.insert(objs,m_uiContent.transform:GetChild(i).gameObject);
	end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "UIP_Mission(Clone)" then
			m_ObjectPool:Release( "UIP_Mission", obj );
		end
	end
	m_uiScrollView:GetComponent("UIScrollRect").normalizedPosition = Vector2.zero;
	m_uiScrollView:GetComponent("UIScrollRect"):ResetScroll();
end

-- 领取奖励
function Activity4ModSelect( id )
	system_askinfo( ASKINFO_ACTIVITY, "", ACTIVITY_4, 1, id )
end

function Activity4ModGoto( gototype )
	-- 打开科技界面
	if gototype == 1 then
		
	-- 打开官府界面
	elseif gototype == 2 then
	
	-- 打开副本界面
	elseif gototype == 3 then
	
	-- 前往世界
	elseif gototype == 4 then
	
	-- 打开充值界面
	elseif gototype == 5 then
	
	-- 打开铁匠铺
	elseif gototype == 6 then
	
	-- 打开武将界面
	elseif gototype == 7 then
	
	-- 打开特价礼包界面
	elseif gototype == 8 then
	
	-- 国器
	elseif gototype == 9 then
	end

end
	
-- 奖励描述
function Activity4ModAwardDescLayer()
	SetFalse( m_uiAwardDescLayer )
	m_AwardDescLayerShow = false
end
function Activity4ModClickItem( awardkind, uiObj )
	Activity4ModAwardDescLayer()
	if awardkind > AWARDKIND_HEROBASE and awardkind < AWARDKIND_BUILDINGBASE then
		local herokind = awardkind - AWARDKIND_HEROBASE
		local color = hero_getnormalcolor( herokind )
		HeroConfigDlgShow( g_heroinfo[herokind][color], 0 );
	else
		m_uiAwardDescLayer.transform:SetParent( uiObj.transform )
		m_uiAwardDescLayer.transform.anchoredPosition = Vector2( 0, 80 )
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
