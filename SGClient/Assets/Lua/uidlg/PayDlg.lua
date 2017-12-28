-- 界面
local m_Dlg = nil;
local m_uiVipLevel = nil; --UnityEngine.GameObject
local m_uiVipNext = nil; --UnityEngine.GameObject
local m_uiVipProgress = nil; --UnityEngine.GameObject
local m_uiMCard = nil; --UnityEngine.GameObject
local m_uiGoodsScroll = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiUIP_Goods = nil; --UnityEngine.GameObject

local m_ObjectPool = nil;
-- 打开界面
function PayDlgOpen()
	m_Dlg = eye.uiManager:Open( "PayDlg" );
end

-- 隐藏界面
function PayDlgClose()
	if m_Dlg == nil then
		return;
	end
	PayDlgClear();
	eye.uiManager:Close( "PayDlg" );
end

-- 删除界面
function PayDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function PayDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            PayDlgClose();
		
		-- 查看特权
		elseif nControlID == 1 then
			VipDlgShow()	
			
		-- 购买
		elseif nControlID >= 1000 and nControlID <= 10000 then
			PayDlgBuy( nControlID-1000 )
        end
	end
end

-- 载入时调用
function PayDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;
	m_uiVipLevel = objs[0];
	m_uiVipNext = objs[1];
	m_uiVipProgress = objs[2];
	m_uiMCard = objs[3];
	m_uiGoodsScroll = objs[4];
	m_uiContent = objs[5];
	m_uiUIP_Goods = objs[6];
	-- 对象池
	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );
	m_ObjectPool:CreatePool("UIP_Goods", 6, 6, m_uiUIP_Goods);
end

-- 界面初始化时调用
function PayDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function PayDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function PayDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function PayDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function PayDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function PayDlgShow()
	PayDlgOpen()
	system_askinfo( ASKINFO_PAY, "", 0 )
	PayDlgUpdateVip()
end

-- m_count=0,m_list={m_goodsid=0,m_price=0,m_token=0,m_day=0,m_nameid=0,m_icon=0,[m_count]},
function PayDlgRecv( recvValue )
	PayDlgClear()
	for i=1, recvValue.m_count, 1 do
		PayDlgCreateGoods( recvValue.m_list[i] )
	end
end

-- 创建一个商品
function PayDlgCreateGoods( info )
	local uiObj = m_ObjectPool:Get( "UIP_Goods" );
	uiObj.transform:SetParent( m_uiContent.transform );
	uiObj.transform.localScale = Vector3.one;
	uiObj.gameObject:SetActive( true );
	SetControlID( uiObj.transform:Find("BuyBtn"), 1000+info.m_goodsid )
	SetText( uiObj.transform:Find("Name"), T(info.m_nameid) )
	SetText( uiObj.transform:Find("Price"), PayDlgGetMoneySymbol()..info.m_price )
	if info.m_goodsid == 1 then
		SetTrue( uiObj.transform:Find("Flag") )
		SetText( uiObj.transform:Find("Flag/Text"), T(2099) )
	else
		SetFalse( uiObj.transform:Find("Flag") )
	end
end

--清空
function PayDlgClear()
	local objs = {};
	for i=0,m_uiContent.transform.childCount-1 do
		table.insert(objs,m_uiContent.transform:GetChild(i).gameObject);
	end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "UIP_Goods(Clone)" then
			m_ObjectPool:Release( "UIP_Goods", obj );
		end
	end
end

-- 更新VIP
function PayDlgUpdateVip()
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	local viplevel = GetPlayer().m_viplevel
	SetText( m_uiVipLevel, "vip"..viplevel )
	if viplevel >= 14 then
		-- 已经满级
		SetFalse( m_uiVipNext )
		SetProgress( m_uiVipProgress, 1 );
		SetText( m_uiVipProgress.transform:Find("Text"), T(2098) );
	else
		local needexp = g_vip[viplevel].exp
		SetTrue( m_uiVipNext )
		SetText( m_uiVipNext, F(2096, needexp-GetPlayer().m_vipexp, viplevel+1 ) )
		SetProgress( m_uiVipProgress, GetPlayer().m_vipexp/needexp );
		SetText( m_uiVipProgress.transform:Find("Text"), GetPlayer().m_vipexp.."/"..needexp );
	end
end

-- 购买
function PayDlgBuy( goodsid )
	if Const.platid == 1 then
		system_askinfo( ASKINFO_PAY, "", 3, goodsid )
	else
		system_askinfo( ASKINFO_PAY, "", 2, goodsid )
	end
end

-- 获取国家货币符号
function PayDlgGetMoneySymbol()
    if Const.platid < 100 then
        return "￥";
    else
        return "$";
    end
    return "";
end