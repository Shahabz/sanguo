-- 界面
local m_Dlg = nil;
local m_uiVipShopBtn = nil; --UnityEngine.GameObject
local m_uiVipBagBtn = nil; --UnityEngine.GameObject
local m_uiFightBtn = nil; --UnityEngine.GameObject
local m_uiOtherBtn = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiVipShopScroll = nil; --UnityEngine.GameObject
local m_uiVipBagScroll = nil; --UnityEngine.GameObject
local m_uiFightScroll = nil; --UnityEngine.GameObject
local m_uiOtherScroll = nil; --UnityEngine.GameObject
local m_uiUIP_VipShopItem = nil; --UnityEngine.GameObject
local m_uiUIP_VipBagItem = nil; --UnityEngine.GameObject
local m_uiUIP_ShopItem = nil; --UnityEngine.GameObject
local m_uiVipShopContent = nil; --UnityEngine.GameObject
local m_uiVipBagContent = nil; --UnityEngine.GameObject
local m_uiFightContent = nil; --UnityEngine.GameObject
local m_uiOtherContent = nil; --UnityEngine.GameObject
local m_uiAwardDescLayer = nil; --UnityEngine.GameObject
local m_uiBuyInfo = nil; --UnityEngine.GameObject
local m_uiVipShopUseItem = nil; --UnityEngine.GameObject
local m_uiSwitchOnBtn = nil; --UnityEngine.GameObject
local m_uiSwitchOffBtn = nil; --UnityEngine.GameObject
local m_uiPayBagScroll = nil; --UnityEngine.GameObject
local m_uiPayBagContent = nil; --UnityEngine.GameObject
local m_uiUIP_PayBagItem = nil; --UnityEngine.GameObject
local m_uiPayBagScrollDot = nil; --UnityEngine.GameObject

local m_uiCostToken = nil
local m_uiUseNum = nil
local m_buytoken = 0;
local m_SelectItemNum = 0;
local m_ObjectPool = nil;
local m_selectType = 0;
local m_selectShopType = 0;
local m_Cache = nil;
local m_CacheItem = nil
local m_PayBagRecvValue = nil;
local m_AwardDescLayerShow = false

-- 打开界面
function ShopDlgOpen()
	ResourceManager.LoadAssetBundle( "_ab_ui_bagdlg" );
	ResourceManager.LoadAssetBundle( "_ab_paybag" );
	m_Dlg = eye.uiManager:Open( "ShopDlg" );
	m_DialogFrameMod = DialogFrameModOpen( m_Dlg, T(34), HELP_BagDlg, ShopDlgClose );
end

-- 隐藏界面
function ShopDlgClose()
	if m_Dlg == nil then
		return;
	end
	DialogFrameModClose( m_DialogFrameMod );
	m_DialogFrameMod = nil;
	eye.uiManager:Close( "ShopDlg" );
	
	m_Cache = nil;
	m_CacheItem = nil
	m_PayBagRecvValue = nil;
end

-- 删除界面
function ShopDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
	ResourceManager.UnloadAssetBundle( "_ab_ui_bagdlg" );
	ResourceManager.UnloadAssetBundle( "_ab_paybag" );
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function ShopDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            ShopDlgClose();
		elseif nControlID == -2 then
			ShopDlgHideAwardDescLayer();
		elseif nControlID == -3 then
			ShopDlgHideBuyInfoLayer()
			
		elseif nControlID >= 1 and nControlID <= 4 then
			ShopDlgSelectType( nControlID )
		
		-- 减10
		elseif nControlID == 11 then
			ShopDlgBuyItemSetNum( -10 )
					
		-- 减1
		elseif nControlID == 12 then
			ShopDlgBuyItemSetNum( -1 )
			
		-- 加1
		elseif nControlID == 13 then
			ShopDlgBuyItemSetNum( 1 )
		
		-- 加10
		elseif nControlID == 14 then
			ShopDlgBuyItemSetNum( 10 )
			
		-- 购买
		elseif nControlID == 15 then
			ShopDlgItemBuy( 0 )
			
		-- 购买并使用
		elseif nControlID == 16 then
			ShopDlgItemBuy( 1 )
		
		-- 点击优先使用道具	
		elseif nControlID == 21 then
			system_askinfo( ASKINFO_VIPSHOP, "", 2, 1 );
			
		-- 点击优先使用道具
		elseif nControlID == 22 then
			system_askinfo( ASKINFO_VIPSHOP, "", 2, 0 );
		
		-- 军事其他商店-点击购买
		elseif nControlID >= 1000 and nControlID < 2000 then
			ShopDlgBuyAsk( nControlID-1000 )
		
		-- VIP礼包点击购买	
		elseif nControlID >= 2000 and nControlID < 3000 then
			ShopDlgVipBagBuy( nControlID-2000 )
		
		-- 礼包点击购买	
		elseif nControlID >= 3000 and nControlID < 4000 then
			ShopDlgPayBagBuy( nControlID-3000 )
					
		-- 点击道具	
		elseif nControlID >= 1000000 then
			ShopDlgClickItem( nControlID-1000000, value )
        end
	elseif nType == UI_EVENT_TIMECOUNTEND then
		if nControlID == 0 then
			ShopDlgClose()
			ShopDlgShowByType( m_selectType )
		end
	elseif nType == UI_EVENT_SCROLLDRAG then
		if nControlID == 0 then
			if m_AwardDescLayerShow == true then
				ShopDlgHideAwardDescLayer()
			end
		end
	elseif nType == UI_EVENT_SCROLLPAGE then
		if nControlID == 0 then
			ShopDlgPayBagScrollSelect( value )
		end
	end
end

-- 载入时调用
function ShopDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiVipShopBtn = objs[0];
	m_uiVipBagBtn = objs[1];
	m_uiFightBtn = objs[2];
	m_uiOtherBtn = objs[3];
	m_uiContent = objs[4];
	m_uiVipShopScroll = objs[5];
	m_uiVipBagScroll = objs[6];
	m_uiFightScroll = objs[7];
	m_uiOtherScroll = objs[8];
	m_uiUIP_VipShopItem = objs[9];
	m_uiUIP_VipBagItem = objs[10];
	m_uiUIP_ShopItem = objs[11];
	m_uiVipShopContent = objs[12];
	m_uiVipBagContent = objs[13];
	m_uiFightContent = objs[14];
	m_uiOtherContent = objs[15];
	m_uiAwardDescLayer = objs[16];
	m_uiBuyInfo = objs[17];
	m_uiVipShopUseItem = objs[18];
	m_uiSwitchOnBtn = objs[19];
	m_uiSwitchOffBtn = objs[20];
	m_uiPayBagScroll = objs[21];
	m_uiPayBagContent = objs[22];
	m_uiUIP_PayBagItem = objs[23];
	m_uiPayBagScrollDot = objs[24];

	-- 对象池
	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );
	m_ObjectPool:CreatePool("UIP_VipShopItem", 8, 8, m_uiUIP_VipShopItem);
	m_ObjectPool:CreatePool("UIP_VipBagItem", 4, 4, m_uiUIP_VipBagItem);
	m_ObjectPool:CreatePool("UIP_ShopItem", 8, 8, m_uiUIP_ShopItem);
end

-- 界面初始化时调用
function ShopDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function ShopDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function ShopDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function ShopDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function ShopDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function ShopDlgShow()
	ShopDlgOpen()
	ShopDlgSelectType( 1 )
	system_askinfo( ASKINFO_PAY, "", 1, 0 )
end

function ShopDlgShowByType( type )
	ShopDlgOpen()
	ShopDlgSelectType( type )
	system_askinfo( ASKINFO_PAY, "", 1, 0 )
end

-- 选择种类
function ShopDlgSelectType( type )
	-- VIP特价商店
	if type == 1 then
		SetTrue( m_uiVipShopUseItem )
		SetTrue( m_uiVipShopScroll )
		SetFalse( m_uiVipBagScroll )
		SetFalse( m_uiFightScroll )
		SetFalse( m_uiOtherScroll )
		SetImage( m_uiVipShopBtn.transform:Find("Back"), LoadSprite("ui_button_page1") )
		SetImage( m_uiVipBagBtn.transform:Find("Back"), LoadSprite("ui_button_page2") )
		SetImage( m_uiFightBtn.transform:Find("Back"), LoadSprite("ui_button_page2") )
		SetImage( m_uiOtherBtn.transform:Find("Back"), LoadSprite("ui_button_page2") )
		system_askinfo( ASKINFO_VIPSHOP, "", 0 );
		
	-- VIP 礼包
	elseif type == 2 then
		SetFalse( m_uiVipShopUseItem )
		SetFalse( m_uiVipShopScroll )
		SetTrue( m_uiVipBagScroll )
		SetFalse( m_uiFightScroll )
		SetFalse( m_uiOtherScroll )
		SetImage( m_uiVipShopBtn.transform:Find("Back"), LoadSprite("ui_button_page2") )
		SetImage( m_uiVipBagBtn.transform:Find("Back"), LoadSprite("ui_button_page1") )
		SetImage( m_uiFightBtn.transform:Find("Back"), LoadSprite("ui_button_page2") )
		SetImage( m_uiOtherBtn.transform:Find("Back"), LoadSprite("ui_button_page2") )
		system_askinfo( ASKINFO_VIPBAG, "", 0 );
		
	-- 普通商店-军事
	elseif type == 3 then
		SetFalse( m_uiVipShopUseItem )
		SetFalse( m_uiVipShopScroll )
		SetFalse( m_uiVipBagScroll )
		SetTrue( m_uiFightScroll )
		SetFalse( m_uiOtherScroll )
		SetImage( m_uiVipShopBtn.transform:Find("Back"), LoadSprite("ui_button_page2") )
		SetImage( m_uiVipBagBtn.transform:Find("Back"), LoadSprite("ui_button_page2") )
		SetImage( m_uiFightBtn.transform:Find("Back"), LoadSprite("ui_button_page1") )
		SetImage( m_uiOtherBtn.transform:Find("Back"), LoadSprite("ui_button_page2") )
		system_askinfo( ASKINFO_SHOP, "", 0, 1 );
	
	-- 普通商店-其他
	elseif type == 4 then
		SetFalse( m_uiVipShopUseItem )
		SetFalse( m_uiVipShopScroll )
		SetFalse( m_uiVipBagScroll )
		SetFalse( m_uiFightScroll )
		SetTrue( m_uiOtherScroll )
		SetImage( m_uiVipShopBtn.transform:Find("Back"), LoadSprite("ui_button_page2") )
		SetImage( m_uiVipBagBtn.transform:Find("Back"), LoadSprite("ui_button_page2") )
		SetImage( m_uiFightBtn.transform:Find("Back"), LoadSprite("ui_button_page2") )
		SetImage( m_uiOtherBtn.transform:Find("Back"), LoadSprite("ui_button_page1") )
		system_askinfo( ASKINFO_SHOP, "", 0, 2 );
	end
	ShopDlgHideAwardDescLayer()
end

-- 返回VIP特价商店信息
-- m_count=0,m_list={m_id=0,m_awardkind=0,m_awardnum=0,m_token=0,m_itemkind=0,m_vip_token=0,m_vip_buynum=0,m_vip_buynum_max=0,[m_count]},m_useitem=0,
function ShopDlgRecvVipShop( recvValue )
	m_selectType = 1
	ShopDlgVipShopSwitchUseItem( recvValue.m_useitem )
	ShopDlgClearVipShopItem()
	m_Cache = recvValue;
	for i = 1, recvValue.m_count, 1 do
		ShopDlgCreateVipShopItem( i, recvValue.m_list[i], recvValue.m_useitem )
	end
end
function ShopDlgCreateVipShopItem( index, info, useitem )
	local uiObj = m_ObjectPool:Get( "UIP_VipShopItem" );
	uiObj.transform:SetParent( m_uiVipShopContent.transform );
	uiObj.transform.localScale = Vector3.one;
	uiObj.gameObject:SetActive( true );
	SetControlID( uiObj, 1000000 + info.m_awardkind )
	
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiShape = objs[0]
	local uiColor = objs[1]
	local uiNum = objs[2]
	local uiName = objs[3]
	local uiDesc1 = objs[4]
	local uiDesc2 = objs[5]
	local uiPrice = objs[6]
	local uiRedLine = objs[7]
	local uiBuyButton = objs[8]
	local uiIcon = objs[9]
	local uiFree = objs[10]
	local uiToken = objs[11]
	local uiTimer = objs[12]
	SetFalse( uiTimer )
	
	local sprite, color, name, c, desc = AwardInfo( info.m_awardkind )
	SetImage( uiShape, sprite );
	SetImage( uiColor, color );
	if info.m_awardnum > 1 then
		if info.m_id == 15 or info.m_id == 16 then
			SetText( uiName, name..info.m_awardnum.."%", NameColor(c) )
			SetText( uiNum, "" )
		else
			SetText( uiName, name.."x"..info.m_awardnum, NameColor(c) )
			SetText( uiNum, "x"..info.m_awardnum )
		end
	else
		SetText( uiName, name, NameColor(c) )
		SetText( uiNum, "" )
	end	
	SetText( uiDesc1, F( 2280+info.m_id, info.m_awardnum ) );
	
	-- 可以道具购买并且已经优选选择道具购买
	local isuse = 0
	if info.m_itemkind > 0 and useitem == 0 then
		local itemnum = GetItem():GetCount( info.m_itemkind )
		if itemnum > 0 then
			isuse = 1;
			SetTrue( uiFree )
			SetText( uiFree, T(2277) )
			SetFalse( uiDesc2 )
			SetFalse( uiRedLine )
			SetTrue( uiPrice )
			SetText( uiPrice, item_getname(info.m_itemkind).."("..itemnum..")" )
			SetFalse( uiIcon )
			SetText( uiToken, "" )
		end
	end
	
	if isuse == 0 then		
		if info.m_id == 15 or info.m_id == 16 then
			SetFalse( uiDesc2 )
			SetFalse( uiRedLine )
			SetTrue( uiPrice )
			SetText( uiPrice, T(2276) )
			SetText( uiToken, info.m_token )
			if info.m_id == 15  then
				local lefttime = GetPlayer().m_buff_endtime[CITY_BUFF_MARCH]-GetServerTime()
				if lefttime > 0 then
					SetTrue( uiTimer )
					SetTimer( uiTimer, lefttime-1, lefttime, 0, T(702) )
					SetText( uiName, name..info.m_awardnum.."%".."("..T(2324)..")", NameColor(c) )
					SetText( uiPrice, T(2325) )
				end
			elseif info.m_id == 16 then
				local lefttime = GetPlayer().m_buff_endtime[CITY_BUFF_TRAIN]-GetServerTime()
				if lefttime > 0 then
					SetTrue( uiTimer )
					SetTimer( uiTimer, lefttime-1, lefttime, 0, T(702) )
					SetText( uiName, name..info.m_awardnum.."%".."("..T(2324)..")", NameColor(c) )
					SetText( uiPrice, T(2325) )
				end
			end
		else
			local leftnum =  info.m_vip_buynum_max-info.m_vip_buynum
			if leftnum > 0 then
				SetTrue( uiDesc2 )
				SetText( uiDesc2, F( 2089, info.m_vip_token, leftnum) )
				SetTrue( uiRedLine )
				SetTrue( uiPrice )
				SetText( uiPrice, F( 2088, info.m_token ) )
				SetText( uiToken, info.m_vip_token )
			else
				SetFalse( uiDesc2 )
				SetFalse( uiRedLine )
				SetFalse( uiPrice )
				SetText( uiToken, info.m_token )
			end
		end
		SetTrue( uiIcon )
		SetFalse( uiFree )
	end
	SetControlID( uiBuyButton, 1000+index )
end
-- VIP商店清空对象
function ShopDlgClearVipShopItem()
	local objs = {};
	for i=0,m_uiVipShopContent.transform.childCount-1 do
		table.insert(objs,m_uiVipShopContent.transform:GetChild(i).gameObject);
	end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "UIP_VipShopItem(Clone)" then
			m_ObjectPool:Release( "UIP_VipShopItem", obj );
		end
	end
end
function ShopDlgVipShopSwitchUseItem( useitem )
	if useitem == 0 then
		SetTrue( m_uiSwitchOnBtn )
		SetFalse( m_uiSwitchOffBtn )
	else
		SetFalse( m_uiSwitchOnBtn )
		SetTrue( m_uiSwitchOffBtn )
	end
end

-- 返回VIP礼包信息
-- m_vipbag=0,m_vipbag_count=0
function ShopDlgRecvVipBag( recvValue )
	m_selectType = 2
	local maxlevel = GetPlayer().m_viplevel + 2
	if maxlevel > recvValue.m_vipbag_count then
		maxlevel = recvValue.m_vipbag_count
	end
	ShopDlgClearVipBagItem()
	for level = 0, maxlevel, 1 do
		if Utils.get_int_sflag( recvValue.m_vipbag, level ) == 0 then
			ShopDlgCreateVipBagItem( level, 0 )
		end
	end	
end
function ShopDlgCreateVipBagItem( level, hasbuy )
	local uiObj = m_ObjectPool:Get( "UIP_VipBagItem" );
	uiObj.transform:SetParent( m_uiVipBagContent.transform );
	uiObj.transform.localScale = Vector3.one;
	uiObj.gameObject:SetActive( true );
	
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiTitleText = objs[0]
	local uiPrice = objs[1]
	local uiRedLine = objs[2]
	local uiNewPrice = objs[3]
	local uiBuyButton = objs[4]
	local uiAwardList = objs[5]
	local uiHasBuy = objs[6]
	
	SetControlID( uiBuyButton, 2000+level )
	SetText( uiTitleText, F(2207, level) )
	SetRichText( uiPrice, F(2271, g_vipbag[level].original_token) )
	SetRichText( uiNewPrice, F(2272, g_vipbag[level].token) )
	
	if GetPlayer().m_viplevel < level or hasbuy == 1 then
		if hasbuy == 1 then
			SetText( uiHasBuy, T(2270) )
		else
			SetText( uiHasBuy, F( 2275, level ) )
		end
		SetTrue( uiHasBuy )
		SetFalse( uiBuyButton )
	else
		SetFalse( uiHasBuy )
		SetTrue( uiBuyButton )
	end
	
	-- 礼包
	local p = g_vipbag[level]
	local awardkind = { p.awardkind0,p.awardkind1,p.awardkind2,p.awardkind3,p.awardkind4,p.awardkind5 }
	local awardnum = { p.awardnum0,p.awardnum1,p.awardnum2,p.awardnum3,p.awardnum4,p.awardnum5 }
	for i=0, 4, 1 do
		local awardObj = uiAwardList.transform:GetChild(i);
		if awardkind[i+1] > 0 then
			SetControlID( awardObj, 1000000 + awardkind[i+1] );
			local sprite, color, name, c, desc = AwardInfo( awardkind[i+1] )
			SetTrue( awardObj )
			SetImage( awardObj.transform:Find("Shape"), sprite );
			SetImage( awardObj.transform:Find("Color"), color );
			SetText( awardObj.transform:Find("Name"), name, NameColor(c) )
			if awardnum[i+1] > 1 then
				SetText( awardObj.transform:Find("Num"), awardnum[i+1] );
			else
				SetText( awardObj.transform:Find("Num"), "" );
			end
			if awardkind[i+1] > AWARDKIND_HEROBASE and awardkind[i+1] < AWARDKIND_BUILDINGBASE then
				SetTrue( awardObj.transform:Find("Corps") )
				local herokind = awardkind[i+1] - AWARDKIND_HEROBASE
				local color = hero_getnormalcolor( herokind )
				SetImage( awardObj.transform:Find("Corps"), CorpsSprite( g_heroinfo[herokind][color].corps ) )
			else
				SetFalse( awardObj.transform:Find("Corps") )
			end
		else
			SetFalse( awardObj )
		end
	end
end
-- VIP礼包清空对象
function ShopDlgClearVipBagItem()
	local objs = {};
	for i=0,m_uiVipBagContent.transform.childCount-1 do
		table.insert(objs,m_uiVipBagContent.transform:GetChild(i).gameObject);
	end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "UIP_VipBagItem(Clone)" then
			m_ObjectPool:Release( "UIP_VipBagItem", obj );
		end
	end
end
-- VIP礼包购买
function ShopDlgVipBagBuy( level )
	if g_vipbag[level] == nil then
		return
	end
	if GetPlayer().m_token < g_vipbag[level].token then
		JumpToken()
		return
	end
	system_askinfo( ASKINFO_VIPBAG, "", 1, level );
end

-- 返回普通商店信息
-- m_type=0,m_count=0,m_list={m_offset=0,m_awardkind=0,m_awardnum=0,m_token=0,m_sale=0,m_buyuse=0 [m_count]},
function ShopDlgRecvShop( recvValue )
	m_selectShopType = recvValue.m_type	
	if m_selectShopType == 1 then
		m_selectType = 3
		ShopDlgClearShopItem( m_uiFightContent )
		ShopDlgClearShopItem( m_uiOtherContent )
		table.sort( recvValue.m_list, function(a,b)
			if a.m_sale > b.m_sale then
				return true;
			end
			return false
		end )
		m_Cache = recvValue;
		for i = 1, recvValue.m_count, 1 do
			ShopDlgCreateShopItem( m_uiFightContent, i, recvValue.m_list[i] )
		end
	elseif m_selectShopType == 2 then
		m_selectType = 4
		ShopDlgClearShopItem( m_uiFightContent )
		ShopDlgClearShopItem( m_uiOtherContent )
		table.sort( recvValue.m_list, function(a,b)
			if a.m_sale > b.m_sale then
				return true;
			end
			return false
		end )
		m_Cache = recvValue;
		for i = 1, recvValue.m_count, 1 do
			ShopDlgCreateShopItem( m_uiOtherContent, i, recvValue.m_list[i] )
		end
	end
end
function ShopDlgCreateShopItem( uiContent, index, info )
	local uiObj = m_ObjectPool:Get( "UIP_ShopItem" );
	uiObj.transform:SetParent( uiContent.transform );
	uiObj.transform.localScale = Vector3.one;
	uiObj.gameObject:SetActive( true );
	SetControlID( uiObj, 1000000 + info.m_awardkind )
	
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiShape = objs[0]
	local uiColor = objs[1]
	local uiNum = objs[2]
	local uiName = objs[3]
	local uiBuyButton = objs[4]
	local uiToken = objs[5]
	local uiSale = objs[6]
	local uiSaleText = objs[7]
	
	local sprite, color, name, c, desc = AwardInfo( info.m_awardkind )
	SetImage( uiShape, sprite );
	SetImage( uiColor, color );
	SetText( uiName, name, NameColor(c) )
	SetText( uiToken, info.m_token )	
	SetControlID( uiBuyButton, 1000+index )
	
	if info.m_awardnum > 1 then
		SetText( uiNum, "x"..info.m_awardnum )
	else
		SetText( uiNum, "" )	
	end
	
	if info.m_sale > 0 then
		SetTrue( uiSale )
		SetText( uiSaleText, -info.m_sale.."%")
	else
		SetFalse( uiSale )
	end
end

function ShopDlgClearShopItem( uiContent )
	local objs = {};
	for i=0,uiContent.transform.childCount-1 do
		table.insert(objs,uiContent.transform:GetChild(i).gameObject);
	end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "UIP_ShopItem(Clone)" then
			m_ObjectPool:Release( "UIP_ShopItem", obj );
		end
	end
end

-- 点击道具图标
function ShopDlgClickItem( awardkind, uiObj )	
	ShopDlgHideAwardDescLayer()
	if awardkind > AWARDKIND_HEROBASE and awardkind < AWARDKIND_BUILDINGBASE then
		local herokind = awardkind - AWARDKIND_HEROBASE
		local color = hero_getnormalcolor( herokind )
		HeroConfigDlgShow( g_heroinfo[herokind][color], 0 );
	else
		m_uiAwardDescLayer.transform:SetParent( uiObj.transform )
		m_uiAwardDescLayer.transform.anchoredPosition = Vector2( 0, 200 )
		m_uiAwardDescLayer.transform:SetParent( m_Dlg.transform:Find("Window").transform )
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

-- 隐藏道具描述页
function ShopDlgHideAwardDescLayer()
	SetFalse( m_uiAwardDescLayer )
	m_AwardDescLayerShow = false
end

-- 购买确定框
function ShopDlgBuyAsk( index )	
	if m_Cache == nil then
		return
	end
	m_CacheItem = m_Cache.m_list[index]
	if m_CacheItem == nil then
		return
	end		
	SetTrue( m_uiBuyInfo )
	local objs = m_uiBuyInfo.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local ItemObj = objs[0];
	local ItemName = objs[1];
	local ItemDesc = objs[2];
	local UseNum = objs[3];
	local BuyButton = objs[4];
	local UseButton = objs[5];
	local NumSelect = objs[6];
	local CostToken = objs[7];
	m_uiCostToken = CostToken;
	m_uiUseNum = UseNum;
	m_SelectItemNum = 1;
		
	-- vip特价商店购买框		
	if m_selectType == 1 then
		-- m_id=0,m_awardkind=0,m_awardnum=0,m_token=0,m_itemkind=0,m_vip_token=0,m_vip_buynum=0,m_vip_buynum_max=0
		if m_CacheItem.m_itemkind > 0 and m_Cache.m_useitem == 0 then
			local itemnum = GetItem():GetCount( m_CacheItem.m_itemkind )
			if itemnum > 0 then
				-- 直接道具购买
				SetFalse( m_uiBuyInfo )
				system_askinfo( ASKINFO_VIPSHOP, "", 3, m_CacheItem.m_id, m_CacheItem.m_awardkind, m_SelectItemNum );
				return
			end
		end
		local sprite, color, name, c, desc = AwardInfo( m_CacheItem.m_awardkind )
		SetImage( ItemObj.transform:Find("Shape"), sprite )
		SetImage( ItemObj.transform:Find("Color"), color )
		SetText( ItemName, name, NameColor(c) );
		SetText( ItemDesc, desc )
		SetText( m_uiUseNum, m_SelectItemNum );
		
		local leftnum =  m_CacheItem.m_vip_buynum_max-m_CacheItem.m_vip_buynum
		if leftnum > 0 then
			m_buytoken = m_CacheItem.m_vip_token;
		else
			m_buytoken = m_CacheItem.m_token;
		end
		local cost = m_SelectItemNum*m_buytoken
		if GetPlayer().m_token < cost then
			SetRichText( CostToken, F( 2274, cost ) )
		else
			SetRichText( CostToken, F( 2273, cost ) )
		end
		SetFalse( UseButton )
	
	-- 军事其他商店购买框	
	elseif m_selectType == 3 or m_selectType == 4 then
		-- {m_offset=0,m_awardkind=0,m_awardnum=0,m_token=0,m_sale=0,m_buyuse=0 [m_count]},
		local sprite, color, name, c, desc = AwardInfo( m_CacheItem.m_awardkind )
		SetImage( ItemObj.transform:Find("Shape"), sprite )
		SetImage( ItemObj.transform:Find("Color"), color )
		if m_CacheItem.m_awardnum > 1 then
			SetText( ItemObj.transform:Find("Num"), "x"..m_CacheItem.m_awardnum );
		else
			SetText( ItemObj.transform:Find("Num"), "" );
		end
		SetText( ItemName, name, NameColor(c) );
		SetText( ItemDesc, desc )
		SetText( m_uiUseNum, m_SelectItemNum );
		
		m_buytoken = m_CacheItem.m_token;
		local cost = m_SelectItemNum*m_buytoken
		if GetPlayer().m_token < cost then
			SetRichText( CostToken, F( 2274, cost ) )
		else
			SetRichText( CostToken, F( 2273, cost ) )
		end
		
		if m_CacheItem.m_buyuse == 1 then
			SetTrue( UseButton )
		else
			SetFalse( UseButton )
		end
	end
end

-- 隐藏购买确定页
function ShopDlgHideBuyInfoLayer()
	SetFalse( m_uiBuyInfo )
end

-- 设置数量
function ShopDlgBuyItemSetNum( num )
	m_SelectItemNum = m_SelectItemNum + num
	if m_SelectItemNum <= 0 then
		m_SelectItemNum = 1;
	end
	
	-- VIP购买次数检查
	if m_selectType == 1 then
		local leftnum =  m_CacheItem.m_vip_buynum_max-m_CacheItem.m_vip_buynum
		if leftnum > 0 and m_SelectItemNum > leftnum then
			m_SelectItemNum = leftnum;
			pop( F(2089, m_CacheItem.m_vip_token,leftnum) )
		end
	end		
	
	SetText( m_uiUseNum, m_SelectItemNum );
	local cost = m_SelectItemNum*m_buytoken
	if GetPlayer().m_token < cost then
		SetRichText( m_uiCostToken, F( 2274, cost ) )
	else
		SetRichText( m_uiCostToken, F( 2273, cost ) )
	end
end
		
-- 购买
function ShopDlgItemBuy( buyuse )
	local cost = m_SelectItemNum*m_buytoken
	if GetPlayer().m_token < cost then
		JumpToken()
		return
	end
	if m_CacheItem == nil then
		return
	end
		
	if m_selectType == 1 then
		-- 自动建造次数询问
		if m_CacheItem.m_awardkind == AWARDKIND_AUTOBUILD then
			if GetPlayer().m_autobuild >= global.autobuild_max then
				AlertMsg( T(2328) )
				return
			end
			if GetPlayer().m_autobuild + m_CacheItem.m_awardnum > global.autobuild_max then
				MsgBox( F( 2326, GetPlayer().m_autobuild, global.autobuild_max ), function()
					system_askinfo( ASKINFO_VIPSHOP, "", 1, m_CacheItem.m_id, m_CacheItem.m_awardkind, m_SelectItemNum );
				end )
			else
				system_askinfo( ASKINFO_VIPSHOP, "", 1, m_CacheItem.m_id, m_CacheItem.m_awardkind, m_SelectItemNum );
			end
		
		-- 城防补充次数询问
		elseif m_CacheItem.m_awardkind == AWARDKIND_CITYGUARDNUM then
			if GetPlayer().m_autoguard >= global.autoguard_max then
				AlertMsg( T(2329) )
				return
			end
			if GetPlayer().m_autoguard + m_CacheItem.m_awardnum > global.autoguard_max then
				MsgBox( F( 2327, GetPlayer().m_autoguard, global.autoguard_max ), function()
					system_askinfo( ASKINFO_VIPSHOP, "", 1, m_CacheItem.m_id, m_CacheItem.m_awardkind, m_SelectItemNum );
				end )
			else
				system_askinfo( ASKINFO_VIPSHOP, "", 1, m_CacheItem.m_id, m_CacheItem.m_awardkind, m_SelectItemNum );
			end
		else
			system_askinfo( ASKINFO_VIPSHOP, "", 1, m_CacheItem.m_id, m_CacheItem.m_awardkind, m_SelectItemNum );
		end
			
	elseif m_selectType == 3 or m_selectType == 4 then
	
		if m_selectType == 3 then
			system_askinfo( ASKINFO_SHOP, "", 1, 1, m_CacheItem.m_offset, m_CacheItem.m_awardkind, m_SelectItemNum, buyuse );
		elseif m_selectType == 4 then
			system_askinfo( ASKINFO_SHOP, "", 1, 2, m_CacheItem.m_offset, m_CacheItem.m_awardkind, m_SelectItemNum, buyuse  );
		end
	end
	ShopDlgHideBuyInfoLayer()
end	


-- 礼包列表
-- m_count=0,m_list={m_goodsid=0,m_price=0,m_nameid=0,m_descid=0,m_icon=0,m_sale=0,m_worth=0,m_bag_time=0,m_bag_num=0,m_awardcount=0,m_award={m_kind=0,m_num=0,[m_awardcount]},[m_count]},
function ShopDlgPayBagRecv( recvValue )
	local scrollPage = m_uiPayBagScroll.transform:GetComponent( typeof(UIScrollPage) )
	scrollPage:ClearPage()
	m_PayBagRecvValue = recvValue
	for i=1, recvValue.m_count, 1 do
		ShopDlgPayBagCreateItem( i, scrollPage, recvValue.m_list[i] )
	end
	if recvValue.m_count > 0 then
		scrollPage:PageCountChanged()
		scrollPage:Play()
		SetTrue( m_uiPayBagScroll )
	else
		SetFalse( m_uiPayBagScroll )
	end
end
function ShopDlgPayBagCreateItem( index, scrollPage, info )
	local uiObj = GameObject.Instantiate( m_uiUIP_PayBagItem );
	SetTrue( uiObj )
	scrollPage:AddPage( uiObj )
	
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiPic = objs[0];
	local uiSale = objs[1];
	local uiSaleText = objs[2];
	local uiName = objs[3];
	local uiWorth = objs[4];
	local uiTimer = objs[5];
	local uiBuyButton = objs[6];
	
	if info.m_sale > 0 then
		SetTrue( uiSale )
		SetText( uiSaleText, "-"..info.m_sale.."%" )
	else
		SetFalse( uiSale )
	end
	SetText( uiName, T(info.m_nameid) )
	SetRichText( uiWorth, F(2278, info.m_worth) )
	if info.m_bag_time > 0 then
		SetTrue( uiTimer )
		local lefttime = info.m_bag_time-GetServerTime()
		SetTimer( uiTimer, lefttime-1, lefttime )
	else
		SetFalse( uiTimer )
	end
	SetText( uiBuyButton.transform:Find("Back/Text"), PayDlgGetMoneySymbol()..info.m_price )
	SetControlID( uiBuyButton, 3000 + index  )
end

-- 当前显示的礼包
function ShopDlgPayBagScrollSelect( page )
	
end

-- 礼包点击购买
function ShopDlgPayBagBuy( index )
	if m_PayBagRecvValue.m_list[index] == nil then
		return
	end
	ShopDlgClose()
	ActivityDlgShowByID( ACTIVITY_PAYBAG ); 
end
