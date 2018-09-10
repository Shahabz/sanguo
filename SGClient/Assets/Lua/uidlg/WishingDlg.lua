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
local m_uiGridContent = nil;
local m_uiUIP_Grid = nil;
local m_uiUIP_AwardGrid = nil;
local m_uiBuyBtn = nil;
local m_uiTimer = nil;
local m_uiChangeRes = nil;
local m_uiCostText = nil; 
local m_uiRankCostBtn = nil; 
local m_uiToglGroup = nil; 
local m_uiRankCostName = nil;
local m_uiChangeContent = nil;
local m_uiPackScroll = nil; 
local m_uiPackChange = nil;
local m_uiPackInfo = nil;
local m_uiPackContent = nil;
local m_uiPackBtn = nil;
local m_uiPackGrid = nil; 
local m_uiPackToGrid = nil; 
local m_uiAwardDescLayer = nil; --UnityEngine.GameObject
local m_uiPackItemDesc = nil; --UnityEngine.GameObject

local m_ObjectPool = nil;
local m_LastOpenStamp = nil;
local m_ChangeCD = nil;
local m_SelectType = nil;
local m_ChangeType = 1;
local m_PackType = nil;
local m_ChangeTab = {};
local m_recvWish = nil;
local m_recvPack = nil;
local m_recvChange = nil;


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
	m_recvWish = nil;
	m_recvPack = nil;
	m_recvChange = nil;
end

-- 删除界面
function WishingDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
	m_recvWish = nil;
	m_recvPack = nil;
	m_recvChange = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function WishingDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            WishingDlgClose();
			WishingDlgSelectCloseToggle();
		elseif nControlID == -2 then
            WishingDlgCheckGrid(-1);
		elseif nControlID >= 1 and nControlID <= 3 then			--标签页
			WishingDlgSelectType( nControlID );
			WishingDlgSelectCloseToggle();
		elseif nControlID > 10 and nControlID < 20 then			-- 点击物品
			WishingDlgClickGrid( nControlID  - 10 );
		elseif nControlID > 20 and nControlID < 30 then			-- 直接购买物品
			WishingDlgBuyGrid( nControlID - 20 );
		elseif nControlID == 101 then							-- 切换兑换资源
			WishingDlgSelectRankCost( nControlID - 100 );
		elseif nControlID == 102 then
			WishingDlgSelectRankCost( nControlID - 100 );
		elseif nControlID == 103 then
			WishingDlgSelectRankCost( nControlID - 100 );
		elseif nControlID == 104 then
			WishingDlgSelectRankCost( nControlID - 100 );
		elseif nControlID == 105 then
			WishingDlgSelectRankCost( nControlID - 100 );
		elseif nControlID == 106 then
			WishingDlgSelectRankCost( nControlID - 100 );
		elseif nControlID > 110 and nControlID < 120 then		-- 点击兑换资源
			WishingDlgOnBtnChange( nControlID - 110 );		
		elseif nControlID >= 201 and nControlID <= 203 then		-- 选择打包类型资源
			WishingDlgPackInfoView( nControlID - 200 )
		elseif nControlID == 204 then							-- 重新选择打包资源
			WishingDlgBackPackChange();
		elseif nControlID == 205 then
			WishingDlgOnBtnPack();
        end
	elseif nType == UI_EVENT_PRESS then
		if nControlID >= 10000 then
			WishingDlgShowAwardLayer( nControlID-10000, value );
		end
	elseif nType == UI_EVENT_TIMECOUNTEND then
		if nControlID == 1 then
			SetFalse(m_uiTimer);
			SetTrue(m_uiTitle);
		elseif nControlID == 2 then
			SetFalse(m_uiTimer);
			SetTrue(m_uiTitle);
		end
	elseif nType == UI_EVENT_TOGGLEVALUECHANGE then
		if nControlID == 11 then
			WishingDlgSetTogGroup();
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
	m_uiWishingInfo = objs[6];
	m_uiUIP_Grid = objs[7];
	m_uiGridContent = objs[8];
	m_uiUIP_AwardGrid = objs[9];
	m_uiAwardContent = objs[10];
	m_uiBuyBtn = objs[11];
	m_uiTimer = objs[12];
	m_uiChangeRes = objs[13];
	m_uiCostText = objs[14];
	m_uiRankCostBtn = objs[15];
	m_uiToglGroup = objs[16];
	m_uiRankCostName = objs[17];
	m_uiChangeContent = objs[18];
	m_uiPackScroll = objs[19];
	m_uiPackChange = objs[20];
	m_uiPackInfo = objs[21];
	m_uiPackContent = objs[22];
	m_uiPackBtn = objs[23];
	m_uiPackGrid = objs[24];
	m_uiPackToGrid = objs[25];
	m_uiAwardDescLayer = objs[26];
	m_uiPackItemDesc = objs[27];
	
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

-- 选择分页
function WishingDlgSelectType( type )
	if type == m_SelectType then return end 
	-- 聚宝盆
	if type == 1 then
		SetTrue(m_uiWishingScroll);
		SetFalse(m_uiChangeresScroll); 	
		SetFalse(m_uiPackScroll);
		SetImage( m_uiWishingBtn.transform:Find("Back"), LoadSprite("ui_button_18") );
		SetImage( m_uiChangeresBtn.transform:Find("Back"), LoadSprite("ui_button_17") );
		SetImage( m_uiPackresBtn.transform:Find("Back"), LoadSprite("ui_button_17") );
		SetText( m_uiTitle, T(3004));
		system_askinfo( ASKINFO_WISHING, "", 0 );
		
	-- 换宝
	elseif type == 2 then
		SetFalse(m_uiWishingScroll);
		SetTrue(m_uiChangeresScroll);
		SetFalse(m_uiPackScroll);
		SetImage( m_uiWishingBtn.transform:Find("Back"), LoadSprite("ui_button_17") );
		SetImage( m_uiChangeresBtn.transform:Find("Back"), LoadSprite("ui_button_18") );
		SetImage( m_uiPackresBtn.transform:Find("Back"), LoadSprite("ui_button_17") );
		SetText( m_uiTitle, T(3005));
		m_ChangeType = 1;
		system_askinfo( ASKINFO_WISHING, "", 3 ); 
		
	-- 打包
	elseif type == 3 then
		SetFalse(m_uiWishingScroll);
		SetFalse(m_uiChangeresScroll); 
		SetTrue(m_uiPackScroll);
		SetImage( m_uiWishingBtn.transform:Find("Back"), LoadSprite("ui_button_17") );
		SetImage( m_uiChangeresBtn.transform:Find("Back"), LoadSprite("ui_button_17") );
		SetImage( m_uiPackresBtn.transform:Find("Back"), LoadSprite("ui_button_18") );
		SetText( m_uiTitle, T(3006));
		m_PackType = nil;
		system_askinfo( ASKINFO_WISHING, "", 5 ); 		
	end
	m_SelectType = type;
end

-- m_count=0,m_list={m_id=0,m_color=0,m_awardkind=0,m_awardnum=0,m_costkind=0,m_costnum=0,m_open=0,[m_count]},m_openstamp=0,
-- system_askinfo( ASKINFO_WISHING, "", 0 ); -- 获取宝物信息
-- system_askinfo( ASKINFO_WISHING, "", 1, id ); -- 宝物打开
-- system_askinfo( ASKINFO_WISHING, "", 2, id ); -- 宝物购买
-- system_askinfo( ASKINFO_WISHING, "", 3 ); -- 兑换信息
-- system_askinfo( ASKINFO_WISHING, "", 4, kind, tokind ); -- 兑换
-- system_askinfo( ASKINFO_WISHING, "", 5 ); — 打包信息
-- system_askinfo( ASKINFO_WISHING, "", 6, id ); — 打包

-- 接收聚宝盆数据
function WishingDlgWishingRecv( recvValue )					
	m_recvWish = recvValue;		                            
	--设置数据                                              
	for i = 1, m_recvWish.m_count, 1 do
		local index = WishingDlgGetRealIndex(i);
		WishingDlgSetGrid( i, m_recvWish.m_list[index]);
	end
	--设置时间
	m_LastOpenStamp = m_recvWish.m_openstamp;
	WishingDlgSetWishingTitle(m_LastOpenStamp);
end
--设置标题或时间
function WishingDlgSetWishingTitle(EndTime)
	local time = EndTime - GetServerTime();
	if time > 0 then 
		SetTimer( m_uiTimer, time , EndTime, m_SelectType,T(3012));
		SetTrue(m_uiTimer);
		SetFalse(m_uiTitle);
	else
		SetFalse(m_uiTimer);
		SetTrue(m_uiTitle);
	end
	-- 聚宝盆冷却完成通知
	notification_set( "SETTING_PUSH_WISHING", T(5911), time )
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

--获取是否是已经打开宝箱
function WishingDlgIsOpenGrid(index)
	local RealIndex = WishingDlgGetRealIndex(index);
	if m_recvWish.m_list[RealIndex].m_open == 1 then 
		return true ;
	end
	return false;
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
	
	if data.m_open == 1 then 
		SetTrue(uiOpen);		
		SetFalse(uiNoOpen);		
		SetImage(uiBack, HeroColorSprite(data.m_color)); 		
		local sprite, color, name, c, desc = AwardInfo( data.m_awardkind );			
		SetImage( uiOpenItemIcon, sprite );
		SetImage( uiOpenColor, color );
		SetText( uiOpenName, name );		
		SetText(uiOpenItemNum,"x"..knum(data.m_awardnum));		
		SetImage( uiOpenCostIcon, ResIcon(data.m_costkind - 50000) );		
		SetText( uiOpenCostNum, knum(data.m_costnum));
	else
		SetTrue(uiNoOpen);		
		SetFalse(uiOpen);	
	end
	SetControlID( uiObj, 10 + index );
end

-- 点击某一个聚宝盆物品
function WishingDlgClickGrid( index )
	if m_recvWish == nil then
		WishingDlgSelectType( 1 )
		return
	end
	-- 今天已购买，提示
	if m_recvWish.m_todaybuy > 0 then 
		pop(T(3007));
		return ;
	end 
	
	-- 是否是已打开的物品
	if WishingDlgIsOpenGrid(index) == true then 
		WishingDlgBuyWishing(index,true);
		return 
	end
	
	-- 开启时间不足打开查看框
	if WishingDlgIsCanOpenNext() == false then 
		WishingDlgCheckGrid(index);
		return ;
	end	
	
	-- 打开某一个聚宝盆物品
	system_askinfo( ASKINFO_WISHING, "", 1, m_recvWish.m_list[WishingDlgGetRealIndex(index)].m_id ); -- 宝物打开		
end

-- 查看宝箱
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
		if m_recvWish.m_list[tab[i]].m_open ~= 1 then
			WishingDlgCreateInfoGrid(m_recvWish.m_list[tab[i]]);
		end
	end
	SetControlID( m_uiBuyBtn, 20 + index );
end

--直接购买宝箱
function WishingDlgBuyGrid(index)
	SetFalse(m_uiWishingInfo);
	WishingDlgBuyWishing(index,false);
end

--购买宝箱
function WishingDlgBuyWishing(index,IsOpen)
	local RealIndex = WishingDlgGetRealIndex(index);
	local CostNum = m_recvWish.m_list[RealIndex].m_costnum;
	local CostName = ResName(m_recvWish.m_list[RealIndex].m_costkind - 50000);
	local AwardNum = "x"..m_recvWish.m_list[RealIndex].m_awardnum;		
	if IsOpen == true then 
		local sprite, color, name, c, desc = AwardInfo( m_recvWish.m_list[RealIndex].m_awardkind );	
		local AwardNum = "x"..m_recvWish.m_list[RealIndex].m_awardnum;	
		MsgBox( F(3008, knum(CostNum),CostName,name,knum(AwardNum) ), function()
			system_askinfo( ASKINFO_WISHING, "", 2, m_recvWish.m_list[RealIndex].m_id ); -- 宝物购买
		end)
	else
		MsgBox( F(3009, knum(CostNum),CostName ), function()
			system_askinfo( ASKINFO_WISHING, "", 2, m_recvWish.m_list[RealIndex].m_id ); -- 直接购买
		end)
	end
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
	
	SetControlID( uiObj, 10000+data.m_awardkind )
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


--接收兑换信息
function WishingDlgChangeRecv( recvValue )
	m_recvChange = recvValue;		
	WishingDlgChangeView();	
	m_ChangeCD = m_recvChange.m_cd;
	WishingDlgSetChangeTitle()
end

--设置兑换标题或时间
function WishingDlgSetChangeTitle()
	if m_ChangeCD > 0 then 
		if m_ChangeCD > global.wishing_change_cdmax then 
			SetTimer( m_uiTimer, m_ChangeCD , GetServerTime() + m_ChangeCD, m_SelectType,T(3010));
		else
			SetTimer( m_uiTimer, m_ChangeCD , GetServerTime() + m_ChangeCD, m_SelectType,T(3011));
		end
		SetTrue(m_uiTimer);
		SetFalse(m_uiTitle);
	else
		SetFalse(m_uiTimer);
		SetTrue(m_uiTitle);
	end
end

-- 兑换界面View
function WishingDlgChangeView()	
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	if m_recvChange == nil then
		return
	end
	m_ChangeTab = {};	
	if m_ChangeType == 1 then 			--银币兑换
		SetImage(m_uiChangeRes,ItemSprite(120));
		SetText( m_uiCostText, ""..knum(m_recvChange.m_silver).."/"..knum(GetPlayer().m_silver));
		SetText(m_uiRankCostName,ResName(m_ChangeType));
		table.insert(m_ChangeTab,{m_kind = AWARDKIND_WOOD, m_Sprite = 121, m_Num = m_recvChange.m_silver_to_wood});	
		table.insert(m_ChangeTab,{m_kind = AWARDKIND_FOOD, m_Sprite = 122, m_Num = m_recvChange.m_silver_to_food});
		m_ChangeTab.Count = 2;
	elseif m_ChangeType == 2 then 		--木材兑换
		SetImage(m_uiChangeRes,ItemSprite(121))
		SetText( m_uiCostText, ""..knum(m_recvChange.m_wood).."/"..knum(GetPlayer().m_wood));
		SetText(m_uiRankCostName,ResName(m_ChangeType));
		table.insert(m_ChangeTab,{m_kind = AWARDKIND_SILVER, m_Sprite = 120, m_Num = m_recvChange.m_wood_to_silver});	
		table.insert(m_ChangeTab,{m_kind = AWARDKIND_FOOD, m_Sprite = 122, m_Num = m_recvChange.m_wood_to_food});
		m_ChangeTab.Count = 2;
	elseif m_ChangeType == 3 then 		--粮草兑换
		SetImage(m_uiChangeRes,ItemSprite(122))
		SetText( m_uiCostText, ""..knum(m_recvChange.m_food).."/"..knum(GetPlayer().m_food));
		SetText(m_uiRankCostName,ResName(m_ChangeType));
		table.insert(m_ChangeTab,{m_kind = AWARDKIND_SILVER, m_Sprite = 120, m_Num = m_recvChange.m_food_to_silver});	
		table.insert(m_ChangeTab,{m_kind = AWARDKIND_WOOD, m_Sprite = 121, m_Num = m_recvChange.m_food_to_wood});
		m_ChangeTab.Count = 2;
	elseif m_ChangeType == 4 then 		--牛皮兑换
		SetImage(m_uiChangeRes,ItemSprite(1));
		SetText( m_uiCostText, ""..global.wishing_green_to_draw);
		SetText(m_uiRankCostName,item_getname(1));
		table.insert(m_ChangeTab,{m_kind = 42, m_Sprite = 42, m_Num = 1});	
		m_ChangeTab.Count = 1;
	elseif m_ChangeType == 5 then 		--碎石兑换	
		SetImage(m_uiChangeRes,ItemSprite(11));
		SetText( m_uiCostText, ""..global.wishing_green_to_draw);
		SetText(m_uiRankCostName,item_getname(11));
		table.insert(m_ChangeTab,{m_kind = 42, m_Sprite = 42, m_Num = 1});	
		m_ChangeTab.Count = 1;	
	elseif m_ChangeType == 6 then 		--朽木兑换
		SetImage(m_uiChangeRes,ItemSprite(21));
		SetText( m_uiCostText, ""..global.wishing_green_to_draw);
		SetText(m_uiRankCostName,item_getname(21));
		table.insert(m_ChangeTab,{m_kind = 42, m_Sprite = 42, m_Num = 1});	
		m_ChangeTab.Count = 1;
	end
	--设置可兑换资源的数量颜色
	if WishingDlgIsCanChangeByStuff() == false then 
		SetTextColor( m_uiCostText, Hex2Color(0xE80017FF))
	else
		SetTextColor( m_uiCostText, Hex2Color(0xF7F3BBFF))
	end
	--设置可兑换资源
	for i = 1 ,m_uiChangeContent.transform.childCount do
		local bShow = true;
		if i > m_ChangeTab.Count then bShow = false; end;
		WishingDlgSetChangeRes(i,m_ChangeTab[i],bShow);
	end
end

--设置可兑换资源
function WishingDlgSetChangeRes(index,data,bShow)
	local uiObj = m_uiChangeContent.transform:GetChild( index - 1 );
	if bShow == false then SetFalse(uiObj); return ; end ;
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiColor = objs[0];
	local uiShape = objs[1];
	local uiName= objs[2];	
	local uiNum = objs[3];
	local uiBtnChang = objs[4];
	
	local sprite, color, name, c, desc = AwardInfo( data.m_Sprite );		
	SetImage(uiShape,sprite);
	SetImage(uiColor,color);
	if m_ChangeType >= 1 and m_ChangeType <= 3 then 
		SetText(uiName,ResName(data.m_kind-50000));	
		SetText(uiNum,"+"..knum(data.m_Num));
		SetImage(uiShape,sprite)
		SetImage(uiColor,color)
	elseif m_ChangeType >= 4 and m_ChangeType <= 6 then 
		SetText(uiName,item_getname(data.m_kind));	
		SetText(uiNum,"+1");
	end	
	SetTrue(uiObj);
	SetControlID( uiBtnChang, 110 + index );
end

-- 选择消耗的兑换资源
function WishingDlgSelectRankCost(number)	
	m_ChangeType = number;
	WishingDlgRankCostView();
	WishingDlgChangeView();
	WishingDlgSelectCloseToggle();
end

-- 设置选择兑换的资源选中状态
function WishingDlgRankCostView()
	for i = 1,m_uiToglGroup.transform.childCount,1 do
		local obj = m_uiToglGroup.transform:GetChild(i-1):Find("TogGround");
		if i == m_ChangeType then
			SetTrue(obj);
		else
			SetFalse(obj);
		end
	end	
end

--设置ToggleGroup的隐藏显示
function WishingDlgSetTogGroup()
	local b_toggle = m_uiRankCostBtn.transform:GetComponent( "UIToggle" ).isOn;
	if b_toggle == true then
		SetTrue(m_uiToglGroup);
		WishingDlgRankCostView();
	else
		SetFalse(m_uiToglGroup);
	end
end

--隐藏ToggleGroup的
function WishingDlgSelectCloseToggle()
	local b_toggle = m_uiRankCostBtn.transform:GetComponent( "UIToggle");
	if b_toggle.isOn == true then
		b_toggle.isOn = false;
	end
end

-- 点击兑换资源按钮
function WishingDlgOnBtnChange(index)
	-- 判断时间
	if WishingDlgIsCanChangeByTime() == false then 
		pop(T(3013));
		return;
	end
	-- 判断材料
	if WishingDlgIsCanChangeByStuff() == false then 
		MsgBox(T(3014));
		return;
	end
	
	local kind = nil;
	if m_ChangeType == 1 then 
		kind = AWARDKIND_SILVER;
	elseif m_ChangeType == 2 then 
		kind = AWARDKIND_WOOD;
	elseif m_ChangeType == 3 then 
		kind = AWARDKIND_FOOD;	
	elseif m_ChangeType == 4 then 
		kind = 1;	
	elseif m_ChangeType == 5 then 
		kind = 11;	
	elseif m_ChangeType == 6 then 
		kind = 21;			
	end
	local ChangeKind = m_ChangeTab[index].m_kind;		
	system_askinfo( ASKINFO_WISHING, "", 4, kind, ChangeKind );
end

--获取兑换时间是否充足
function WishingDlgIsCanChangeByTime()
	if m_ChangeCD < global.wishing_change_cdmax then 
		return true ;
	end
	return false;
end

--获取兑换材料是否充足
function WishingDlgIsCanChangeByStuff()
	local Have = nil;
	local Need = nil;
	if m_ChangeType == 1 then 
		Need = m_recvChange.m_silver;
		Have = GetPlayer().m_silver;
	elseif m_ChangeType == 2 then 
		Need = m_recvChange.m_wood;
		Have = GetPlayer().m_wood;
	elseif m_ChangeType == 3 then 
		Need = m_recvChange.m_food;	
		Have = GetPlayer().m_food;
	elseif m_ChangeType == 4 then 
		Need = global.wishing_green_to_draw;
		Have = GetItem():GetCount( 1 )
	elseif m_ChangeType == 5 then 
		Need = global.wishing_green_to_draw;
		Have = GetItem():GetCount( 11 )
	elseif m_ChangeType == 6 then 
		Need = global.wishing_green_to_draw;
		Have = GetItem():GetCount( 21 )
	end
	
	if Have >= Need then
		return true;
	end
	return false;
end

--接收打包信息
function WishingDlgPackRecv(recvValue)
	m_recvPack = recvValue;
	WishingDlgPackChangeView()
end

--打包选择界面
function WishingDlgPackChangeView()
	if m_PackType ~= nil then 
		WishingDlgPackInfoView(m_PackType)
	else
		SetTrue(m_uiPackChange);
		SetFalse(m_uiPackInfo);
		for i = 0, table.nums(m_recvPack.m_list)-1 ,1 do 
			WishingDlgSetPackChangeGrid(i,m_recvPack.m_list[i+1]);
		end
	end	
end

--返回打包选择界面
function WishingDlgBackPackChange()
	m_PackType = nil;
	WishingDlgPackChangeView();
end

--设置打包选择资源
function WishingDlgSetPackChangeGrid(index,data)
	local uiObj = m_uiPackContent.transform:GetChild( index );
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiColor = objs[0];
	local uiItemIcon = objs[1];
	local uiName = objs[2];
	
	local sprite, color, name, c, desc = AwardInfo( data.m_costkind - 50000 +119 );		
	SetImage(uiItemIcon,sprite);
	SetImage(uiColor,color);
	SetText(uiName,name);
end

--打包信息界面
function WishingDlgPackInfoView(index)
	m_PackType = index;
	SetFalse(m_uiPackChange);
	SetTrue(m_uiPackInfo);
	local data = m_recvPack.m_list[m_PackType];
	WishingDlgSetPackGrid(data);
	WishingDlgSetPackToGrid(data);
end
--设置打包前资源
function WishingDlgSetPackGrid(data)
	local objs = m_uiPackGrid.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiColor = objs[0];
	local uiItemIcon = objs[1];
	local uiName = objs[2];	
	local uiCostIcon = objs[3];
	local uiCostNum = objs[4];
	
	local sprite, color, name, c, desc = AwardInfo( data.m_costkind - 50000 +119 );		
	SetImage(uiItemIcon,sprite);
	SetImage(uiColor,color);
	SetText(uiName,name);
	
	SetImage(uiCostIcon,ResIcon(data.m_costkind-50000));
	local bHave,Have =  WishingDlgIsCanPackByStuff(data.m_costkind-50000,data.m_costnum);
	SetText(uiCostNum,knum(data.m_costnum).."/"..knum(Have));	
	if bHave == false then 
		SetTextColor( uiCostNum, Hex2Color(0xE80017FF))
	else
		SetTextColor( uiCostNum, Hex2Color(0x03DE27FF))
	end
end

--设置打包后资源
function WishingDlgSetPackToGrid(data)
	local objs = m_uiPackToGrid.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiColor = objs[0];
	local uiItemIcon = objs[1];
	local uiItemNum = objs[2];
	local uiName = objs[3];
	local uiCostNum = objs[4];
	
	local sprite, color, name, c, desc = AwardInfo( data.m_awardkind );		
	SetImage(uiItemIcon,sprite);
	SetImage(uiColor,color);
	SetText(uiName,name);
	SetText( m_uiPackItemDesc.transform:Find("Text"), desc )
	SetText(uiItemNum,"x"..knum(data.m_awardnum));		
	SetText(uiCostNum,knum(data.m_token));	
	--设置资源的数量颜色
	if data.m_token > GetPlayer().m_token then 
		SetTextColor( uiCostNum, Hex2Color(0xE80017FF))
	else
		SetTextColor( uiCostNum, Hex2Color(0xF7F3BBFF))
	end
end

--获取打包材料是否充足
function WishingDlgIsCanPackByStuff(kind,Need)
	local Have = nil;
	if kind == 1 then 
		Have = GetPlayer().m_silver;
	elseif kind == 2 then 
		Have = GetPlayer().m_wood;
	elseif kind == 3 then 
		Have = GetPlayer().m_food;
	end
	
	if Have >= Need then
		return true,Have;
	end
	return false,Have;
end

function WishingDlgOnBtnPack()
	local data = m_recvPack.m_list[m_PackType];
	--判断资源
	local bHave,Have = WishingDlgIsCanPackByStuff(m_PackType,data.m_costnum);
	if bHave == false then 
		pop(T(3015));
		return;
	end
	--判断元宝
	if data.m_token > GetPlayer().m_token then 
		JumpToken();
		return;
	end
	
	system_askinfo( ASKINFO_WISHING, "", 6, data.m_id ); -- 打包
end

function WishingDlgShowAwardLayer( awardkind, value )
	if value == 1 then
		SetFalse( m_uiAwardDescLayer )
		return
	end
	local sprite, color, name, c, desc = AwardInfo( awardkind );	
	SetTrue( m_uiAwardDescLayer )
	SetText( m_uiAwardDescLayer.transform:Find("Name"), name )
	SetText( m_uiAwardDescLayer.transform:Find("Desc"), desc )	
end