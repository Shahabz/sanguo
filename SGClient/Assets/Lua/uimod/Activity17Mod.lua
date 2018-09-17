-- 界面
local m_Mod;
local m_uiScrollView = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiUIP_Mission = nil; --UnityEngine.GameObject
local m_uiAwardDescLayer = nil; --UnityEngine.GameObject
local m_uiBack = nil; --UnityEngine.GameObject
local m_uiMyPay = nil; --UnityEngine.GameObject
local m_recvValue = nil;
local m_AwardDescLayerShow = false
local m_ObjectPool = nil
local m_red = 0;

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function Activity17ModOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
		if nControlID == -2 then
			Activity17ModAwardDescLayer();
		elseif nControlID >= 1000 and nControlID <= 2000 then
			Activity17ModSelect( nControlID-1000 )
		elseif nControlID >= 1000000 and nControlID < 2000000 then
			Activity17ModClickItem( nControlID-1000000, value )
        end
	elseif nType == UI_EVENT_SCROLLDRAG then
		if nControlID == 1 then
			if m_AwardDescLayerShow == true then
				Activity17ModAwardDescLayer()
			end
		end
	end
end

-- 载入时调用
function Activity17ModOnAwake( gameObject )
	m_Mod = gameObject;
	local objs = gameObject:GetComponent( "UIMod" ).relatedGameObject;
	-- 控件赋值	
	m_uiScrollView = objs[0];
	m_uiContent = objs[1];
	m_uiUIP_Mission = objs[2];
	m_uiAwardDescLayer = objs[3];
	m_uiBack = objs[4];
	m_uiMyPay = objs[5];

	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );
	m_ObjectPool:CreatePool("UIP_Mission", 8, 8, m_uiUIP_Mission);
end

-- 界面初始化时调用
function Activity17ModOnStart( gameObject )
	system_askinfo( ASKINFO_ACTIVITY, "", ACTIVITY_17, 0 )
	ResourceManager.LoadAssetBundle( "_ab_activity_pic_1" )
	ResourceManager.LoadAssetBundle( "_ab_activity_back_4" )
	
	SetImage( m_uiBack, LoadSprite("activity_pic_1") )
	m_AwardDescLayerShow = false
end

-- 界面显示时调用
function Activity17ModOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function Activity17ModOnDisable( gameObject )
	
end

-- 界面删除时调用
function Activity17ModOnDestroy( gameObject )
	m_Dlg = nil;
	ResourceManager.UnloadAssetBundle( "_ab_activity_pic_1" )
	ResourceManager.UnloadAssetBundle( "_ab_activity_back_4" )
end

-- 每帧调用
function Activity17ModOnLogic( gameObject )
	
end

function Activity17ModGet()
	return m_Mod;
end
----------------------------------------
-- 自定
----------------------------------------
-- m_count=0,m_list={m_id=0,m_pay=0,m_awardkind={[12]},m_awardnum={[12]},[m_count]},m_mypay=0,
function Activity17ModRecv( recvValue )
	if m_Mod == nil then
		return
	end
	m_recvValue = recvValue
	Activity17ModClear()
	m_red = 0;
	
	local moneySymbol, divider = PayDlgGetMoneySymbol()
	if divider == 1 then	
		SetText( m_uiMyPay, F(4259,moneySymbol,recvValue.m_mypay) )
	else
		SetText( m_uiMyPay, F(4259,moneySymbol,string.format("%.2f",recvValue.m_mypay/divider)) )
	end
	
	-- 已完成的排序后面
	local tmptable = {}
	for i=1, recvValue.m_count, 1 do
		local sflag = recvValue.m_list[i].m_isget
		table.insert( tmptable, {sflag=sflag, rank=-1000*sflag-recvValue.m_list[i].m_id, info=recvValue.m_list[i]} )
	end
	table.sort( tmptable, function(a,b) 
		if a.rank > b.rank then
			return true
		else
			return false
		end
	end )
	
	for i=1, #tmptable, 1 do
		Activity17ModCreate( tmptable[i].info )
	end
	
	-- 红点
	ActivityDlgChangeRed( ACTIVITY_17, m_red )
end

-- 创建
function Activity17ModCreate( info )
	local uiObj = m_ObjectPool:Get("UIP_Mission");
	uiObj.transform:SetParent( m_uiContent.transform );
	uiObj.transform.localScale = Vector3.one;
	
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiName = objs[0]
	local uiAwardList = objs[2]
	local uiStateBtn = objs[3]
	local uiBack = objs[4]
	local uiState = objs[5]
	
	SetControlID( uiStateBtn, 1000 + info.m_id )
	SetImage( uiBack, LoadSprite("activity_back_4") )
	
	local moneySymbol, divider = PayDlgGetMoneySymbol()
	if divider == 1 then	
		SetText( uiName, F(4260,moneySymbol,info.m_pay) )
	else
		SetText( uiName, F(4260,moneySymbol,string.format("%.2f",info.m_pay/divider)) )
	end
	
	if m_recvValue.m_mypay >= info.m_pay then
		if info.m_isget == 1 then-- 完成已经领取
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
	for i=1, 16, 1 do
		local awardObj = uiAwardList.transform:GetChild(i-1);
		if info.m_awardkind[i] > 0 then
			local sprite, color, name = AwardInfo( info.m_awardkind[i] )
			SetTrue( awardObj )
			SetControlID( awardObj, 1000000+info.m_awardkind[i] )
			SetImage( awardObj.transform:Find("Shape"), sprite );
			SetImage( awardObj.transform:Find("Color"), color );
			if info.m_awardnum[i] > 1 then
				SetTrue( awardObj.transform:Find("NumBack") )
				SetText( awardObj.transform:Find("Num"), info.m_awardnum[i] );
			else
				SetFalse( awardObj.transform:Find("NumBack") )
				SetText( awardObj.transform:Find("Num"), "" );
			end
		else
			SetFalse( awardObj )
		end
	end
end

--清空
function Activity17ModClear()
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
function Activity17ModSelect( id )
	system_askinfo( ASKINFO_ACTIVITY, "", ACTIVITY_17, 1, id )
end

-- 奖励描述
function Activity17ModAwardDescLayer()
	SetFalse( m_uiAwardDescLayer )
	m_AwardDescLayerShow = false
end
function Activity17ModClickItem( awardkind, uiObj )
	Activity17ModAwardDescLayer()
	if awardkind > AWARDKIND_HEROBASE and awardkind < AWARDKIND_BUILDINGBASE then
		local herokind = awardkind - AWARDKIND_HEROBASE
		local color = hero_getnormalcolor( herokind )
		HeroConfigDlgShow( g_heroinfo[herokind][color], 0 );
		
	elseif awardkind > AWARDKIND_GIRLBASE and awardkind < AWARDKIND_GIRLSOULBASE then
		local grilkind = awardkind - AWARDKIND_GIRLBASE
		GirlDlgShow( grilkind )
		
	else
		m_uiAwardDescLayer.transform:SetParent( uiObj.transform )
		m_uiAwardDescLayer.transform.anchoredPosition = Vector2( 0, 80 )
		m_uiAwardDescLayer.transform:SetParent( m_Mod.transform )
		local _, _, name, c, desc = AwardInfo( awardkind )
		SetText( m_uiAwardDescLayer.transform:Find("Name"), name, NameColor(c) )	
		SetText( m_uiAwardDescLayer.transform:Find("Desc"), desc )
		SetText( m_uiAwardDescLayer.transform:Find("Warn"), "" )
		SetTrue( m_uiAwardDescLayer )
		m_AwardDescLayerShow = true
	end
end
