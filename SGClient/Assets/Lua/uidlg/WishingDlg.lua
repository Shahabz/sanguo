-- 界面
local m_Dlg = nil;
local m_DialogFrameMod = nil;
local m_uiWishingBtn = nil; 
local m_uiChangeresBtn = nil;
local m_uiPackresBtn = nil; 
local m_uiTitle = nil;
local m_uiWishingScroll = nil;
local m_uiChangeresScroll = nil;
local m_uiWishingGet = nil;
local m_uiWishingInfo = nil;
local m_uiUIP_Noopen = nil;
local m_uiUIP_Open = nil;
local m_uiWishingContent = nil;
local m_uiBuyBtn = nil;
local m_ObjectPool = nil;
local m_IsGet = nil;

-- 打开界面
function WishingDlgOpen()
	m_Dlg = eye.uiManager:Open( "WishingDlg" );
	m_DialogFrameMod = DialogFrameModOpen( m_Dlg, BuildingName(36), HELP_WishingDlg, WishingDlgClose );
	
end

-- 隐藏界面
function WishingDlgClose()
	if m_Dlg == nil then
		return;
	end
	DialogFrameModClose( m_DialogFrameMod );
	m_DialogFrameMod = nil;
	eye.uiManager:Close( "WishingDlg" );
end

-- 删除界面
function WishingDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function WishingDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            WishingDlgClose();
		elseif nControlID == -2 then
            WishingDlgGetShow(false);
		elseif nControlID == -3 then
            WishingDlgCheckGird(-1);
		elseif nControlID >= 1 and nControlID <= 4 then
			WishingDlgSelectType( nControlID )
		elseif nControlID > 100 and nControlID < 200 then			-- 获取物品
			WishingDlgGetGird( nControlID  - 100 )
		elseif nControlID > 200 and nControlID < 300 then			-- 打开物品
			WishingDlgOpenGird( nControlID - 200 )
		elseif nControlID > 1000 and nControlID < 2000 then			-- 直接购买物品
			WishingDlgGetGird( nControlID - 1000 )
		elseif nControlID > 10000 and nControlID < 20000 then
			WishingDlgCheckGird( nControlID - 1000);
        end
	end
end

-- 载入时调用
function WishingDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiWishingBtn = objs[0];
	m_uiChangeresBtn = objs[1];
	m_uiPackresBtn = objs[2];
	m_uiTitle = objs[3];
	m_uiWishingScroll = objs[4];
	m_uiChangeresScroll = objs[5];
	m_uiWishingGet = objs[6];
	m_uiWishingInfo = objs[7];
	m_uiUIP_Grid = objs[8];
	m_uiWishingContent = objs[9];
	m_uiBuyBtn = objs[10];
	
	-- 对象池
	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );
	m_ObjectPool:CreatePool("UIP_Grid", 5, 5, m_uiUIP_Grid);	

end

-- 界面初始化时调用
function WishingDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function WishingDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function WishingDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function WishingDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function WishingDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function WishingDlgShow()
	WishingDlgOpen()
	WishingDlgSelectType( 1 )
end

-- 接收聚宝盆数据
-- m_count=0,m_list={m_id=0,m_color=0,m_awardkind=0,m_awardnum=0,m_costkind=0,m_costnum=0,m_open=0,[m_count]},m_openstamp=0,
-- system_askinfo( ASKINFO_WISHING, "", 0 ); -- 获取宝物信息
-- system_askinfo( ASKINFO_WISHING, "", 1, id ); -- 宝物打开
-- system_askinfo( ASKINFO_WISHING, "", 2, id ); -- 宝物购买
function WishinggDlgRecv( recvValue )
	m_IsGet = false;
	WishingDlgClearGird();
	for i = 1, 9, 1 do
		WishingDlgCreateOpenItem( i, recvValue.m_list[i], false )
	end
end

-- 创建聚宝盆物品数据
function WishingDlgCreateOpenItem(index, data, bOpen)
	local uiObj = m_ObjectPool:Get( "UIP_Grid" );
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiBack = objs[0]	
	local uiNoOpen = objs[1]
	local uiOpen= objs[2]
	local uiWishingBtn = objs[3]
	local uiOpenColor = objs[4]
	local uiOpenItemIcon = objs[5]	
	local uiOpenName = objs[6]
	local uiOpenCost = objs[7]
	local uiOpenCostBack = objs[8]	
	local uiOpenCostText = objs[9]
	local uiOpenItemBtn = objs[10]
	
	if bOpen == true then 
		SetTrue(uiOpen)		
		SetFalse(uiNoOpen)
		SetControlID( uiOpenItemBtn, 100+index )
	else
		SetTrue(uiNoOpen)		
		SetFalse(uiOpen)		
		SetControlID( uiWishingBtn, 200+index )
	end
	uiObj.transform:SetParent( m_uiWishingContent.transform );
	uiObj.transform.localScale = Vector3.one
	uiObj.gameObject:SetActive( true )
end

-- 清空聚宝盆物品数据
function WishingDlgClearGird()
	local objs = {};
	for i=0,m_uiWishingContent.transform.childCount-1 do
		table.insert(objs,m_uiWishingContent.transform:GetChild(i).gameObject);
	end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "UIP_Grid(Clone)" then
			m_ObjectPool:Release( "UIP_Grid", obj );
		end
	end
end

-- 打开某一个聚宝盆物品
function WishingDlgOpenGird( index )
	--时间不足打开提示框
--	if m_IsGet == false then 
--		WishingDlgCheckGird(index)
--		return ;
--	end
	--时间充足直接打开
	local uiObj = m_uiWishingContent.transform:GetChild(index-1);
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiBack = objs[0]	
	local uiNoOpen = objs[1]
	local uiOpen= objs[2]
	local uiWishingBtn = objs[3]
	local uiOpenColor = objs[4]
	local uiOpenItemIcon = objs[5]	
	local uiOpenName = objs[6]
	local uiOpenCost = objs[7]
	local uiOpenCostBack = objs[8]	
	local uiOpenCostText = objs[9]	
	local uiOpenItemBtn = objs[10]

	SetTrue(uiOpen)		
	SetFalse(uiNoOpen)
	SetControlID( uiOpenItemBtn, 100+index )
end

-- 点击获取某一个聚宝盆物品
function WishingDlgGetGird( index )
	if m_IsGet == true then return end       
	WishingDlgGetShow(true,index)
	for i = 1, 9, 1 do
		WishingDlgOpenGird( i )
	end
end

-- 获取聚宝物品
function WishingDlgGetShow(bShow,index)
	if bShow == false then 			
		SetFalse(m_uiWishingGet)
		return	
	end
	SetTrue(m_uiWishingGet)
	SetFalse(m_uiWishingInfo)
	m_IsGet = true;
end

-- 选择未开启的宝箱
function WishingDlgCheckGird( index )
	if index <= 0 then 
		SetFalse(m_uiWishingInfo)
	else
		SetTrue(m_uiWishingInfo)
		SetControlID( m_uiBuyBtn, 1000 + index )
		--local objs = m_uiWishingInfo.transform:GetComponent( typeof(Reference) ).relatedGameObject;		
	end
end

-- 选择分页
function WishingDlgSelectType( type )
	-- 聚宝盆
	if type == 1 then
		SetTrue(m_uiWishingScroll)
		SetFalse(m_uiChangeresScroll) 		
		SetImage( m_uiWishingBtn.transform:Find("Back"), LoadSprite("ui_button_page1") )
		SetImage( m_uiChangeresBtn.transform:Find("Back"), LoadSprite("ui_button_page2") )
		SetImage( m_uiPackresBtn.transform:Find("Back"), LoadSprite("ui_button_page2") )
		SetText( m_uiTitle, T(3004))
		system_askinfo( ASKINFO_WISHING, "", 0 );
		
	-- 换宝
	elseif type == 2 then
		SetFalse(m_uiWishingScroll)
		SetTrue(m_uiChangeresScroll) 
		SetImage( m_uiWishingBtn.transform:Find("Back"), LoadSprite("ui_button_page2") )
		SetImage( m_uiChangeresBtn.transform:Find("Back"), LoadSprite("ui_button_page1") )
		SetImage( m_uiPackresBtn.transform:Find("Back"), LoadSprite("ui_button_page2") )
		SetText( m_uiTitle, T(3005))

		--system_askinfo( ASKINFO_VIPBAG, "", 0 );
		
	-- 打包
	elseif type == 3 then
		SetFalse(m_uiWishingScroll)
		SetFalse(m_uiChangeresScroll) 
		SetImage( m_uiWishingBtn.transform:Find("Back"), LoadSprite("ui_button_page2") )
		SetImage( m_uiChangeresBtn.transform:Find("Back"), LoadSprite("ui_button_page2") )
		SetImage( m_uiPackresBtn.transform:Find("Back"), LoadSprite("ui_button_page1") )
		SetText( m_uiTitle, T(3006))
		--system_askinfo( ASKINFO_SHOP, "", 0, 1 );
	end
end

