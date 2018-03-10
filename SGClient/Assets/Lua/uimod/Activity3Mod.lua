local m_Mod;
local m_uiBack = nil; --UnityEngine.GameObject
local m_uiShape = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiUIP_Mission = nil; --UnityEngine.GameObject
local m_uiAwardDescLayer = nil; --UnityEngine.GameObject
local m_AwardDescLayerShow = false
local m_ObjectPool = nil
local m_recvValue = nil;
local m_red = 0;
----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function Activity3ModOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
		--print( "Button Clicked, nControlID:" .. nControlID );
		if nControlID == -2 then
			Activity3ModAwardDescLayer();
		elseif nControlID >= 1000 and nControlID <= 2000 then
			Activity3ModSelect( nControlID-1000 )
		elseif nControlID >= 1000000 and nControlID < 2000000 then
			Activity3ModClickItem( nControlID-1000000, value )
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
				Activity3ModAwardDescLayer()
			end
		end
	end
end

-- 载入时调用
function Activity3ModOnAwake( gameObject )
	m_Mod = gameObject;
	local objs = gameObject:GetComponent( "UIMod" ).relatedGameObject;
	m_uiBack = objs[0];
	m_uiShape = objs[1];
	m_uiContent = objs[2];
	m_uiUIP_Mission = objs[3];
	m_uiAwardDescLayer = objs[4];
	-- 对象池
	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );
	m_ObjectPool:CreatePool("UIP_Mission", 7, 7, m_uiUIP_Mission);
end

-- 界面初始化时调用
function Activity3ModOnStart( gameObject )
	system_askinfo( ASKINFO_ACTIVITY, "", ACTIVITY_3, 0 )
	ResourceManager.LoadAssetBundle( "_ab_activity_pic_4" )
	ResourceManager.LoadAssetBundle( "_ab_activity_back_5" )
	
	SetImage( m_uiBack, LoadSprite("activity_pic_4") )
	SetImage( m_uiShape, LoadSprite("heroface_119") )
	m_AwardDescLayerShow = false
end

-- 界面显示时调用
function Activity3ModOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function Activity3ModOnDisable( gameObject )
	
end

-- 界面删除时调用
function Activity3ModOnDestroy( gameObject )
	m_Mod = nil
	m_recvValue = nil
	ResourceManager.UnloadAssetBundle( "_ab_activity_pic_4" )
	ResourceManager.UnloadAssetBundle( "_ab_activity_back_5" )
end

-- 每帧调用
function Activity3ModOnLogic( gameObject )
	
end

function Activity3ModGet()
	return m_Mod;
end
----------------------------------------
-- 自定
----------------------------------------
-- m_count=0,m_list={m_value=0,m_state=0,[m_count]},
function Activity3ModRecv( recvValue )
	if m_Mod == nil then
		return
	end
	m_red = 0;
	Activity3ModClear()
	m_recvValue = recvValue
	
	-- 已完成的排序后面
	local tmptable = {}
	for i=1, #g_activity_03, 1 do
		local state = recvValue.m_list[i].m_state
		local value = recvValue.m_list[i].m_value
		table.insert( tmptable, {state=state, value=value, rank=-1000*state-g_activity_03[i].id, info=g_activity_03[i]} )
	end
	table.sort( tmptable, function(a,b) 
		if a.rank > b.rank then
			return true
		else
			return false
		end
	end )
	
	for i=1, #tmptable, 1 do
		Activity3ModCreate( tmptable[i].info, tmptable[i].value, tmptable[i].state )
	end
	
	-- 红点
	ActivityDlgChangeRed( ACTIVITY_3, m_red )
end

-- 创建
function Activity3ModCreate( info, value, state )
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
	SetControlID( uiStateBtn, 1000 + info.id )
	SetImage( uiBack, LoadSprite("activity_back_5") )
	SetText( uiName, F(info.nameid, info.needvalue) )
	SetText( uiValue, value.."/"..info.needvalue )
	
	if value >= info.needvalue then
		if state == 1 then-- 完成已经领取
			SetText( uiStateBtn.transform:Find("Back/Text"), T(1109) )
			SetFalse( uiStateBtn )
			SetTrue( uiState )
		else -- 完成没有领取
			SetText( uiStateBtn.transform:Find("Back/Text"), T(1351) )
			SetTrue( uiStateBtn )
			SetButtonTrue( uiStateBtn )
			SetFalse( uiState )
			m_red = 1
		end
	else
		SetText( uiStateBtn.transform:Find("Back/Text"), T(2452) )
		SetTrue( uiStateBtn )
		SetButtonFalse( uiStateBtn )
		SetFalse( uiState )
	end
	
	-- 奖励
	local awardkind = { info.awardkind0, info.awardkind1, info.awardkind2, info.awardkind3, info.awardkind4 }
	local awardnum = { info.awardnum0, info.awardnum1, info.awardnum2, info.awardnum3, info.awardnum4 }
	for i=1, 4, 1 do
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

--清空
function Activity3ModClear()
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
end

-- 领取奖励
function Activity3ModSelect( id )
	if m_recvValue.m_list[id].m_value < g_activity_03[id].needvalue then
		return	
	end
	if m_recvValue.m_list[id].m_state == 1 then
		return
	end
	system_askinfo( ASKINFO_ACTIVITY, "", ACTIVITY_3, 1, id )
end

-- 奖励描述
function Activity3ModAwardDescLayer()
	SetFalse( m_uiAwardDescLayer )
	m_AwardDescLayerShow = false
end
function Activity3ModClickItem( awardkind, uiObj )
	Activity3ModAwardDescLayer()
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