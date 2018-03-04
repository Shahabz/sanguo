local m_Mod;

local m_uiPayBagScroll = nil; --UnityEngine.GameObject
local m_uiPayBagContent = nil; --UnityEngine.GameObject
local m_uiUIP_PayBagItem = nil; --UnityEngine.GameObject
local m_uiPayBagScrollDot = nil; --UnityEngine.GameObject
local m_uiPayBagScrollDotToggle = nil; --UnityEngine.GameObject
local m_uiTitleText = nil; --UnityEngine.GameObject
local m_uiBack = nil; --UnityEngine.GameObject
local m_uiTitleBack = nil; --UnityEngine.GameObject
local m_uiLeftTimer = nil; --UnityEngine.GameObject
local m_uiLeftCount = nil; --UnityEngine.GameObject
local m_uiAwardScroll = nil; --UnityEngine.GameObject
local m_uiAwardContent = nil; --UnityEngine.GameObject
local m_uiUIP_Award = nil; --UnityEngine.GameObject
local m_uiPrice = nil; --UnityEngine.GameObject
local m_uiRedLine = nil; --UnityEngine.GameObject
local m_uiBuyText = nil; --UnityEngine.GameObject

local m_ObjectPool = nil
local m_PayBagRecvValue = nil;
local m_selectPage = -1;
----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function Activity10ModOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
		if nControlID == 2 then
			Activity10ModBuy( nControlID )
		end
	elseif nType == UI_EVENT_PRESS then
		if value == 0 then
			--print( "Button Pressed Down, nControlID:" .. nControlID );
		elseif value == 1 then
			--print( "Button Pressed UP, nControlID:" .. nControlID);
		end
	elseif nType == UI_EVENT_SCROLLPAGE then
		if nControlID == 0 then
			Activity10ModSelect( value )
		end
	end
end

-- 载入时调用
function Activity10ModOnAwake( gameObject )
	m_Mod = gameObject;
	local objs = gameObject:GetComponent( "UIMod" ).relatedGameObject;
	m_uiPayBagScroll = objs[0];
	m_uiPayBagContent = objs[1];
	m_uiUIP_PayBagItem = objs[2];
	m_uiPayBagScrollDot = objs[3];
	m_uiPayBagScrollDotToggle = objs[4];
	m_uiTitleText = objs[5];
	m_uiBack = objs[6];
	m_uiTitleBack = objs[7];
	m_uiLeftTimer = objs[8];
	m_uiLeftCount = objs[9];
	m_uiAwardScroll = objs[10];
	m_uiAwardContent = objs[11];
	m_uiUIP_Award = objs[12];
	m_uiPrice = objs[13];
	m_uiRedLine = objs[14];
	m_uiBuyText = objs[15];
	
	-- 对象池
	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );
	m_ObjectPool:CreatePool("UIP_Award", 4, 4, m_uiUIP_Award);
end

-- 界面初始化时调用
function Activity10ModOnStart( gameObject )
	system_askinfo( ASKINFO_PAY, "", 1, 1 )
	ResourceManager.LoadAssetBundle( "_ab_paybag" );
	ResourceManager.LoadAssetBundle( "_ab_activity_back_6" )
	ResourceManager.LoadAssetBundle( "_ab_activity_pic_8" )
	SetImage( m_uiBack, LoadSprite("activity_pic_8") )
	SetImage( m_uiTitleBack, LoadSprite("activity_back_6") )
end

-- 界面显示时调用
function Activity10ModOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function Activity10ModOnDisable( gameObject )
	
end

-- 界面删除时调用
function Activity10ModOnDestroy( gameObject )
	m_Mod = nil
	m_PayBagRecvValue = nil
	Invoke( function() 
		ResourceManager.UnloadAssetBundleImmediately( "_ab_activity_back_6" )
		ResourceManager.UnloadAssetBundleImmediately( "_ab_activity_pic_8" )
		ResourceManager.UnloadAssetBundleImmediately( "_ab_paybag" );
	end, 0.3 );
end

-- 每帧调用
function Activity10ModOnLogic( gameObject )
	
end

function Activity10ModGet()
	return m_Mod;
end
----------------------------------------
-- 自定
----------------------------------------
-- m_count=0,m_list={m_goodsid=0,m_price=0,m_nameid=0,m_descid=0,m_icon=0,m_sale=0,m_worth=0,m_bag_time=0,m_bag_num=0,m_awardcount=0,m_award={m_kind=0,m_num=0,[m_awardcount]},[m_count]},
function Activity10ModRecv( recvValue )
	if m_Mod == nil then
		return
	end
	local scrollPage = m_uiPayBagScroll.transform:GetComponent( typeof(UIScrollPage) )
	scrollPage:ClearPage()
	m_PayBagRecvValue = recvValue
	for i=1, recvValue.m_count, 1 do
		Activity10ModCreateItem( i, scrollPage, recvValue.m_list[i] )
	end
	if recvValue.m_count > 0 then
		scrollPage:PageCountChanged()
		scrollPage:Play()
		SetTrue( m_uiPayBagScroll )
	else
		SetFalse( m_uiPayBagScroll )
	end
	Activity10ModSelect( 0 )
end
function Activity10ModCreateItem( index, scrollPage, info )
	local uiObj = GameObject.Instantiate( m_uiUIP_PayBagItem );
	SetTrue( uiObj )
	scrollPage:AddPage( uiObj )
	
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiPic = objs[0];
	local uiSale = objs[1];
	local uiSaleText = objs[2];
	local uiName = objs[3];
	
	if info.m_sale > 0 then
		SetTrue( uiSale )
		SetText( uiSaleText, "-"..info.m_sale.."%" )
	else
		SetFalse( uiSale )
	end
	SetText( uiName, T(info.m_nameid) )
end

-- 当前显示的礼包
function Activity10ModSelect( page )
	local info = m_PayBagRecvValue.m_list[page+1]
	if info == nil then
		return
	end
	m_selectPage = page;
	-- 礼包标题
	SetText( m_uiTitleText, T(info.m_nameid) )
	-- 结束时间
	SetText( m_uiLeftTimer, F(2332, getDateStringByInterval_zh(info.m_bag_time) ) )
	-- 剩余次数
	SetText( m_uiLeftCount, F( 2333, info.m_bag_num ) )
	-- 价格
	SetText( m_uiBuyText, PayDlgGetMoneySymbol()..info.m_price )
	-- 原价
	if info.m_sale > 0 then
		SetText( m_uiPrice, PayDlgGetMoneySymbol()..math.floor(info.m_price*(1+info.m_sale/100)) )
		SetTrue( m_uiRedLine )
	else
		SetText( m_uiPrice, "" )
		SetFalse( m_uiRedLine )
	end
	-- 奖励
	Activity10ModDlgClear()
	for i=1, info.m_awardcount, 1 do
		Activity10ModCreateAward( info.m_award[i] )
	end
end

-- 创建奖励
function Activity10ModCreateAward( awardinfo )
	local uiObj = m_ObjectPool:Get("UIP_Award");
	uiObj.transform:SetParent( m_uiAwardContent.transform );
	uiObj.transform.localScale = Vector3.one;
	SetTrue( uiObj )
	
	local sprite, color, name, c, desc = AwardInfo( awardinfo.m_kind )
	SetImage( uiObj.transform:Find("Shape"), sprite )
	SetImage( uiObj.transform:Find("Color"), color )
	SetText( uiObj.transform:Find("Name"), name );
	SetText( uiObj.transform:Find("Num"), "x"..knum(awardinfo.m_num) );
end

--清空
function Activity10ModDlgClear()
	local objs = {};
	for i=0,m_uiAwardContent.transform.childCount-1 do
		table.insert(objs,m_uiAwardContent.transform:GetChild(i).gameObject);
	end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "UIP_Award(Clone)" then
			m_ObjectPool:Release( "UIP_Award", obj );
		end
	end
end

-- 礼包点击购买
function Activity10ModBuy()
	local index = m_selectPage+1
	if m_PayBagRecvValue.m_list[index] == nil then
		return
	end
	PayDlgBuy( m_PayBagRecvValue.m_list[index].m_goodsid )
end