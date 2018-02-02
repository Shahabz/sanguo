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
local m_uiUIP_Grid = nil;
local m_uiUIP_AwardGrid = nil;
local m_uiBuyBtn = nil;
local m_uiTimer = nil;

local m_ObjectPool = nil;
local m_LastOpenStamp = nil;
local m_SelectType = nil;
local m_recvValue = nil;
local m_IsCanOpen = nil;

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
	m_recvValue = nil
end

-- 删除界面
function WishingDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
	m_recvValue = nil
	HeroColorSpriteUnload( 0 )
	HeroColorSpriteUnload( 1 )
	HeroColorSpriteUnload( 2 )
	HeroColorSpriteUnload( 3 )
	HeroColorSpriteUnload( 4 )
	HeroColorSpriteUnload( 5 )
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
            WishingDlgCheckGrid(-1);
		elseif nControlID >= 1 and nControlID <= 4 then
			WishingDlgSelectType( nControlID );
		elseif nControlID > 100 and nControlID < 200 then			-- 获取物品
			WishingDlgGetGrid( nControlID  - 100 );
		elseif nControlID > 200 and nControlID < 300 then			-- 打开物品
			WishingDlgOpenGrid( nControlID - 200 );
		elseif nControlID > 300 and nControlID < 400 then			-- 直接购买物品
			WishingDlgBuyGrid( nControlID - 300 );
        end
	elseif nType == UI_EVENT_TIMECOUNTEND then
		if nControlID == 1 then
			SetFalse(m_uiTimer);
			SetTrue(m_uiTitle);
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
	m_uiGridContent = objs[9];
	m_uiUIP_AwardGrid = objs[10];
	m_uiAwardContent = objs[11];
	m_uiBuyBtn = objs[12];
	m_uiTimer = objs[13];
	
	-- 对象池
	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );		
	m_ObjectPool:CreatePool("UIP_AwardGrid", 9, 9, m_uiUIP_AwardGrid);	


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
	WishingDlgOpen();
	WishingDlgSelectType( 1 );
end

-- 接收聚宝盆数据
-- m_count=0,m_list={m_id=0,m_color=0,m_awardkind=0,m_awardnum=0,m_costkind=0,m_costnum=0,m_open=0,[m_count]},m_openstamp=0,
-- system_askinfo( ASKINFO_WISHING, "", 0 ); -- 获取宝物信息
-- system_askinfo( ASKINFO_WISHING, "", 1, id ); -- 宝物打开
-- system_askinfo( ASKINFO_WISHING, "", 2, id ); -- 宝物购买
function WishingDlgRecv( recvValue )
	m_recvValue = recvValue;		
	--设置数据
	for i = 1, 9, 1 do
		local index = WishingDlgGetRealIndex(i);
		WishingDlgSetGrid( i, m_recvValue.m_list[index]);
	end
	--设置时间
	m_LastOpenStamp = m_recvValue.m_openstamp;
	WishingDlgSetTitle(m_LastOpenStamp);
end
--设置标题或时间
function WishingDlgSetTitle(EndTime)
	local time = EndTime - GetServerTime();
	if time > 0 then 
		SetTimer( m_uiTimer, time , EndTime, m_SelectType);
		SetTrue(m_uiTimer);
		SetFalse(m_uiTitle);
	else
		SetFalse(m_uiTimer);
		SetTrue(m_uiTitle);
	end
end
--获取真实位置
function WishingDlgGetRealIndex(index)
	local RealIndex = 0;
	if index == 5 then 
		RealIndex = 1;
	elseif index < 5 then
		RealIndex = index + 1;
	else
		RealIndex = index;
	end
	return RealIndex;
end

--获取是否可开启下一个宝箱
function WishingDlgIsCanOpenNext()
	if m_LastOpenStamp - GetServerTime() <= 0 then 
		return true ;
	end
	return false;
end

-- 设置聚宝盆物品数据
function WishingDlgSetGrid(index, data)
	local uiObj = m_uiGridContent.transform:GetChild( index - 1 );
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiBack = objs[0];
	local uiNoOpen = objs[1];
	local uiOpen= objs[2];
	local uiOpenColor = objs[3];
	local uiOpenItemIcon = objs[4];
	local uiOpenName = objs[5];
	local uiOpenItemNum = objs[6];
	local uiOpenCostIcon = objs[7];	
	local uiOpenCostNum = objs[8];
	local uiIndex = objs[9];
	
	if data.m_open == 1 then 
		SetTrue(uiOpen);		
		SetFalse(uiNoOpen);
		
		SetImage(uiBack, HeroColorSprite(data.m_color)); 		
		local sprite, color, name, c, desc = AwardInfo( data.m_awardkind );			
		SetImage( uiOpenItemIcon, sprite );
		SetImage( uiOpenColor, color );
		SetText( uiOpenName, name );		
		SetText(uiOpenItemNum,"x"..data.m_awardnum);		
		SetImage( uiOpenCostIcon, ResIcon(data.m_costkind - 50000) );		
		SetText( uiOpenCostNum, data.m_costnum);
		SetControlID( uiObj, 100 + index );
	else
		SetTrue(uiNoOpen);		
		SetFalse(uiOpen);	
		SetControlID( uiObj, 200 + index );	
	end
	SetText(uiIndex,index);
end

-- 打开某一个聚宝盆物品
function WishingDlgOpenGrid( index )
	-- 时间不足打开提示框
	if WishingDlgIsCanOpenNext() == false then 
		WishingDlgCheckGrid(index);
		return ;
	end
	system_askinfo( ASKINFO_WISHING, "", 1, m_recvValue.m_list[WishingDlgGetRealIndex(index)].m_id ); -- 宝物打开	
end

-- 点击获取某一个聚宝盆物品
function WishingDlgGetGrid( index )
	--今天已购买，提示
	if m_recvValue.m_todaybuy > 0 then 
		pop(T(3007));
		return ;
	end 
	system_askinfo( ASKINFO_WISHING, "", 2, m_recvValue.m_list[WishingDlgGetRealIndex(index)].m_id ); -- 宝物获取
end

-- 获取聚宝物品
function WishingDlgGetShow(bShow,index)
	if bShow == false then 			
		SetFalse(m_uiWishingGet);
		return	
	end
	SetTrue(m_uiWishingGet);
	SetFalse(m_uiWishingInfo);
end

-- 选择宝箱
function WishingDlgCheckGrid( index )
	if index <= 0 then 
		SetFalse(m_uiWishingInfo);
	else
		SetTrue(m_uiWishingInfo);	
		WishingDlgInfoView(index);
	end
end

-- 查看宝箱信息
function WishingDlgInfoView(index)
	WishingDlgClearInfoGrid();
	local tab = table.shuffle(1,9,9)
	for i = 1, 9, 1 do 	
		if m_recvValue.m_list[tab[i]].m_open ~= 1 then
			WishingDlgCreateInfoGrid(m_recvValue.m_list[tab[i]]);
		end
	end
	SetControlID( m_uiBuyBtn, 300 + index );
end

--直接购买宝箱
function WishingDlgBuyGrid(index)
	SetFalse(m_uiWishingInfo);
	system_askinfo( ASKINFO_WISHING, "", 2, m_recvValue.m_list[WishingDlgGetRealIndex(index)].m_id ); -- 宝物获取
end
-- 创建查看物品数据
function WishingDlgCreateInfoGrid(data)
	local uiObj = m_ObjectPool:Get( "UIP_AwardGrid" );
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiItem = objs[0];	
	local uiColor = objs[1];
	local uiNum = objs[2];
	
	local sprite, color, name, c, desc = AwardInfo( data.m_awardkind );		
	SetImage( uiItem, sprite );
	SetImage( uiColor, color );
	SetText(uiNum,"x"..data.m_awardnum);
	
	uiObj.transform:SetParent( m_uiAwardContent.transform );
	uiObj.transform.localScale = Vector3.one;
	uiObj.gameObject:SetActive( true );
end
-- 清空查看物品数据
function WishingDlgClearInfoGrid()
	local objs = {};
	for i=0,m_uiAwardContent.transform.childCount-1 do
		table.insert(objs,m_uiAwardContent.transform:GetChild(i).gameObject);
	end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "UIP_AwardGrid(Clone)" then
			m_ObjectPool:Release( "UIP_AwardGrid", obj );
		end
	end
end
-- 选择分页
function WishingDlgSelectType( type )
	if type == m_SelectType then return end 
	-- 聚宝盆
	if type == 1 then
		SetTrue(m_uiWishingScroll);
		SetFalse(m_uiChangeresScroll); 		
		SetImage( m_uiWishingBtn.transform:Find("Back"), LoadSprite("ui_button_page1") );
		SetImage( m_uiChangeresBtn.transform:Find("Back"), LoadSprite("ui_button_page2") );
		SetImage( m_uiPackresBtn.transform:Find("Back"), LoadSprite("ui_button_page2") );
		SetText( m_uiTitle, T(3004));
		system_askinfo( ASKINFO_WISHING, "", 0 );
		
	-- 换宝
	elseif type == 2 then
		SetFalse(m_uiWishingScroll);
		SetTrue(m_uiChangeresScroll);
		SetImage( m_uiWishingBtn.transform:Find("Back"), LoadSprite("ui_button_page2") );
		SetImage( m_uiChangeresBtn.transform:Find("Back"), LoadSprite("ui_button_page1") );
		SetImage( m_uiPackresBtn.transform:Find("Back"), LoadSprite("ui_button_page2") );
		SetText( m_uiTitle, T(3005));

		--system_askinfo( ASKINFO_VIPBAG, "", 0 );
		
	-- 打包
	elseif type == 3 then
		SetFalse(m_uiWishingScroll);
		SetFalse(m_uiChangeresScroll); 
		SetImage( m_uiWishingBtn.transform:Find("Back"), LoadSprite("ui_button_page2") );
		SetImage( m_uiChangeresBtn.transform:Find("Back"), LoadSprite("ui_button_page2") );
		SetImage( m_uiPackresBtn.transform:Find("Back"), LoadSprite("ui_button_page1") );
		SetText( m_uiTitle, T(3006));
		--system_askinfo( ASKINFO_SHOP, "", 0, 1 );
	end
	m_SelectType = type;
end

