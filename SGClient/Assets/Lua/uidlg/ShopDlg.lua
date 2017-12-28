-- 界面
local m_Dlg = nil;
local m_DialogFrameMod = nil;
local m_uiSaleBtn = nil; --UnityEngine.GameObject
local m_uiVipBagBtn = nil; --UnityEngine.GameObject
local m_uiFightBtn = nil; --UnityEngine.GameObject
local m_uiOtherBtn = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiSaleScroll = nil; --UnityEngine.GameObject
local m_uiVipBagScroll = nil; --UnityEngine.GameObject
local m_uiFightScroll = nil; --UnityEngine.GameObject
local m_uiOtherScroll = nil; --UnityEngine.GameObject
local m_uiUIP_SaleItem = nil; --UnityEngine.GameObject

local m_ObjectPool = nil;
local m_selectType = 0;
local m_selectShopType = 0;

-- 打开界面
function ShopDlgOpen()
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
end

-- 删除界面
function ShopDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function ShopDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            ShopDlgClose();
        end
	end
end

-- 载入时调用
function ShopDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiSaleBtn = objs[0];
	m_uiVipBagBtn = objs[1];
	m_uiFightBtn = objs[2];
	m_uiOtherBtn = objs[3];
	m_uiContent = objs[4];
	m_uiSaleScroll = objs[5];
	m_uiVipBagScroll = objs[6];
	m_uiFightScroll = objs[7];
	m_uiOtherScroll = objs[8];
	m_uiUIP_SaleItem = objs[9];
	-- 对象池
	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );
	m_ObjectPool:CreatePool("UIP_SaleItem", 8, 8, m_uiUIP_SaleItem);
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
end

function ShopDlgShowByType( type )
	ShopDlgOpen()
	ShopDlgSelectType( type )
end

-- 选择种类
function ShopDlgSelectType( type )
	-- VIP特价商店
	if type == 1 then
		system_askinfo( ASKINFO_VIPSHOP, "", 0 );
		
	-- VIP 礼包
	elseif type == 2 then
		system_askinfo( ASKINFO_VIPBAG, "", 0 );
		
	-- 普通商店-军事
	elseif type == 3 then
		system_askinfo( ASKINFO_SHOP, "", 0, 1 );
	
	-- 普通商店-其他
	elseif type == 4 then
		system_askinfo( ASKINFO_SHOP, "", 0, 2 );
	end
end

-- 返回VIP特价商店信息
-- m_count=0,m_list={m_awardkind=0,m_awardnum=0,m_token=0,m_itemkind=0,m_vip_token=0,m_vip_buynum=0,m_vip_buynum_max=0,[m_count]},m_useitem=0,
function ShopDlgRecvVipShop( recvValue )
	m_selectType = 1
	for i = 1, recvValue.m_count, 1 do
		
	end
end
function ShopDlgCreateVipShopItem(  )
	
end

-- 返回VIP礼包信息
function ShopDlgRecvVipBag( recvValue )
	m_selectType = 2
	for i = 1, recvValue.m_count, 1 do
		
	end
end
function ShopDlgCreateVipBagItem()
	
end

-- 返回普通商店信息
-- m_type=0,m_count=0,m_list={m_offset=0,m_awardkind=0,m_awardnum=0,m_token=0,m_sale=0 [m_count]},
function ShopDlgRecvShop( recvValue )
	m_selectShopType = recvValue.m_type
	if m_selectShopType == 1 then
		m_selectType = 3
	elseif m_selectShopType == 2 then
		m_selectType = 4
	end
	for i = 1, recvValue.m_count, 1 do
		
	end
end
function ShopDlgCreateShopItem(  )
	
end


