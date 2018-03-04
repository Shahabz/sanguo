local m_Mod;
local m_uiBack = nil; --UnityEngine.GameObject
local m_uiShape = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiUIP_Mission = nil; --UnityEngine.GameObject
local m_ObjectPool = nil
local m_recvValue = nil;
----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function Activity3ModOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
		--print( "Button Clicked, nControlID:" .. nControlID );
		if nControlID >= 1000 and nControlID <= 2000 then
			Activity3ModSelect( nControlID-1000 )
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
function Activity3ModOnAwake( gameObject )
	m_Mod = gameObject;
	local objs = gameObject:GetComponent( "UIMod" ).relatedGameObject;
	m_uiBack = objs[0];
	m_uiShape = objs[1];
	m_uiContent = objs[2];
	m_uiUIP_Mission = objs[3];
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
	Invoke( function() 
		ResourceManager.UnloadAssetBundleImmediately( "_ab_activity_pic_4" )
		ResourceManager.UnloadAssetBundleImmediately( "_ab_activity_back_5" )
	end, 0.3 );
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
	Activity3ModClear()
	m_recvValue = recvValue
	for i=1, #g_activity_03, 1 do
		Activity3ModCreate( g_activity_03[i], recvValue.m_list[i].m_value, recvValue.m_list[i].m_state )
	end
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
			SetFalse( uiState )
		end
	else
		SetText( uiStateBtn.transform:Find("Back/Text"), T(2452) )
		SetTrue( uiStateBtn )
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