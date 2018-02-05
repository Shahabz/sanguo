-- 界面
local m_Dlg = nil;
local m_uiText = nil; --UnityEngine.GameObject
local m_uiTabs = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiBuyInfo = nil; --UnityEngine.GameObject

local m_nTabKind = {}
local m_nContentKind = {}
local m_index = 0;

local m_buytoken = 0;
local m_SelectItemNum = 0;
local m_SelectItem = 0;

local m_uiCostToken = nil;
local m_uiUseNum = nil;
	
-- 打开界面
function MaterialGetDlgOpen()
	m_Dlg = eye.uiManager:Open( "MaterialGetDlg" );
end

-- 隐藏界面
function MaterialGetDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "MaterialGetDlg" );
end

-- 删除界面
function MaterialGetDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function MaterialGetDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            MaterialGetDlgClose();
		elseif nControlID == -3 then
			MaterialGetDlgHideBuyInfoLayer()
			
		-- 选择页签
		elseif nControlID >= 1 and nControlID <= 4 then
			MaterialGetDlgSelectTab( nControlID )
			
		-- 前往征收
		elseif nControlID == 10 then
			LevyDlgShow()
			MaterialGetDlgClose();
			
		-- 减10
		elseif nControlID == 11 then
			MaterialGetDlgBuyItemSetNum( -10 )
					
		-- 减1
		elseif nControlID == 12 then
			MaterialGetDlgBuyItemSetNum( -1 )
			
		-- 加1
		elseif nControlID == 13 then
			MaterialGetDlgBuyItemSetNum( 1 )
		
		-- 加10
		elseif nControlID == 14 then
			MaterialGetDlgBuyItemSetNum( 10 )
			
		-- 购买
		elseif nControlID == 15 then
			MaterialGetDlgItemBuy( 0 )
		
		-- 使用指定道具
		elseif nControlID > 1000 and nControlID < 2000 then
			MaterialGetDlgUse( nControlID-1000 )
			
		-- 购买指定道具
		elseif nControlID > 2000 and nControlID < 3000 then
			MaterialGetDlgBuyAsk( nControlID-2000 )
        end
	end
end

-- 载入时调用
function MaterialGetDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiText = objs[0];
	m_uiTabs = objs[1];
	m_uiContent = objs[2];
	m_uiBuyInfo = objs[3];
end

-- 界面初始化时调用
function MaterialGetDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function MaterialGetDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function MaterialGetDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function MaterialGetDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function MaterialGetDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function MaterialGetDlgShow( itemkind )
	MaterialGetDlgOpen()
	m_nTabKind = {}
	m_nContentKind = {}
	if itemkind == 120 or itemkind == 121 or itemkind == 122 or itemkind == 123 then
		SetText( m_uiText, T(774) );
		m_nTabKind = { 120, 121, 122, 123 };
		m_nContentKind = { { 101,102,103, -10000 }, { 104,105,106, -10000 },{ 107,108,109, -10000 }, { 110,111,112, -10000 }}
		
	elseif itemkind == 1 or itemkind == 11 or itemkind == 21 then -- 绿
		SetText( m_uiText, T(775) );
		m_nTabKind = { 1, 11, 21 };
		m_nContentKind = { { -1,81,91,92 }, { -11,81,91,92 },{ -21,81,91,92 }, { -31,81,91,92 }}
		SetFalse( m_uiTabs.transform:GetChild(3).gameObject )
		
	elseif itemkind == 2 or itemkind == 12 or itemkind == 22 or itemkind == 32 then -- 金
		SetText( m_uiText, T(776) );
		m_nTabKind = { 2, 12, 22, 32 };
		m_nContentKind = { { -2,83,93,94 }, { -12,83,93,94 },{ -22,83,93,94 }, { -32,83,93,94 }}
		
	elseif itemkind == 3 or itemkind == 13 or itemkind == 23 or itemkind == 33 then -- 红
		SetText( m_uiText, T(777) );
		m_nTabKind = { 3, 13, 23, 33 };
		m_nContentKind = { { -3,85,95,96 }, { -13,85,95,96 },{ -23,85,95,96 }, { -33,85,95,96 }}
	
	elseif itemkind == 4 or itemkind == 14 or itemkind == 24 or itemkind == 34 then -- 紫
		SetText( m_uiText, T(777) );
		m_nTabKind = { 4, 14, 24, 34 };
		m_nContentKind = { { -4,97,95,96 }, { -14,97,95,96 },{ -24,97,95,96 }, { -34,97,95,96 }}	
	end
	
	for i=1, #m_nTabKind do
		local uiTab = m_uiTabs.transform:GetChild(i-1).gameObject;
		SetTrue( uiTab )
		SetControlID( uiTab, i )
		if m_nTabKind[i] == itemkind then
			MaterialGetDlgSelectTab( i )
		end
		SetImage( uiTab.transform:Find("Back/Icon"), ResKindIcon(m_nTabKind[i]) )
		SetText( uiTab.transform:Find("Back/Num"), knum( GetItem():GetCount(m_nTabKind[i]) ) )	
	end
end

-- 选择页签
function MaterialGetDlgSelectTab( index )
	m_index = index;
	for i=1, #m_nTabKind do
		local uiTab = m_uiTabs.transform:GetChild(i-1).gameObject;
		if i == index then
			SetImage( uiTab.transform:Find("Back"), LoadSprite("ui_button_page1") )
		else
			SetImage( uiTab.transform:Find("Back"), LoadSprite("ui_button_page2") )
		end
	end

	local contentKind = m_nContentKind[index]
	for i=1, #contentKind, 1 do
		local itemkind = contentKind[i]
		local uiItem = m_uiContent.transform:GetChild(i-1).gameObject;
		local objs = uiItem.transform:GetComponent( typeof(Reference) ).relatedGameObject;
		local uiShape = objs[0];
		local uiColor = objs[1];
		local uiName = objs[2];
		local uiUseBtn = objs[3];
		local uiBuyBtn = objs[4];
		local uiGotoBtn = objs[5];
		local uiNum = objs[6];
		
		-- 前往
		if itemkind == -10000 then
			SetTrue( uiGotoBtn );
			SetFalse( uiUseBtn );
			SetFalse( uiBuyBtn );
			SetFalse( uiNum )
			SetImage( uiShape, BuildingSprite(1) )
			SetImage( uiColor, ItemColorSprite(4) )
			SetText( uiName, BuildingName(1).."("..T(659)..")" )
		
		-- 购买	
		elseif itemkind < 0 then
			
			local _kind = -itemkind
			SetFalse( uiUseBtn );
			SetFalse( uiGotoBtn );
			SetTrue( uiBuyBtn );
			SetTrue( uiNum )
			SetControlID( uiBuyBtn, 2000+_kind )
			SetImage( uiShape, ItemSprite(_kind) )
			SetImage( uiColor, ItemColorSprite(item_getcolor(_kind)) )
			SetText( uiName, item_getname(_kind).."("..item_getdesc(_kind)..")", NameColor( item_getcolor(_kind) ) )
			SetText( uiNum, GetItem():GetCount( _kind ) )
			
			SetText( uiBuyBtn.transform:Find("Back/Num"), item_gettoken(_kind) )	
			
		-- 使用
		else
			SetTrue( uiUseBtn );
			SetFalse( uiGotoBtn );
			SetFalse( uiBuyBtn );
			SetTrue( uiNum )
			SetControlID( uiUseBtn, 1000+itemkind )
			SetImage( uiShape, ItemSprite(itemkind) )
			SetImage( uiColor, ItemColorSprite(item_getcolor(itemkind)) )
			SetText( uiName, item_getname(itemkind).."("..item_getdesc(itemkind)..")", NameColor( item_getcolor(itemkind) ) )
			SetText( uiNum, GetItem():GetCount( itemkind ) )
		end
	end
end

-- 刷新
function MaterialGetDlgUpdate()
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	if m_index <= 0 then
		return
	end
	
	for i=1, #m_nTabKind do
		local uiTab = m_uiTabs.transform:GetChild(i-1).gameObject;
		SetText( uiTab.transform:Find("Back/Num"), knum( GetItem():GetCount(m_nTabKind[i]) ) )	
	end
	
	local contentKind = m_nContentKind[m_index]
	for i=1, #contentKind, 1 do
		local itemkind = contentKind[i]
		local uiItem = m_uiContent.transform:GetChild(i-1).gameObject;
		local objs = uiItem.transform:GetComponent( typeof(Reference) ).relatedGameObject;
		local uiNum = objs[6];
		if itemkind == -10000 then
		elseif itemkind < 0 then
			SetText( uiNum, GetItem():GetCount( -itemkind ) )
		elseif itemkind > 0 then
			SetText( uiNum, GetItem():GetCount( itemkind ) )
		end
	end
	
	-- 刷新打造所需材料界面
	EquipForgingDlgUpdate()
	
	-- 建筑建造界面刷新
	BuildingUpgradeDlgUpdate()
end

-- 使用道具
function MaterialGetDlgUse( itemkind )
	local num = GetItem():GetCount( itemkind )
	if num <= 0 then
		pop(T(779))
		return
	end
	GetItem():Use( itemkind )
end

-- 购买确定框
function  MaterialGetDlgBuyAsk( itemkind )	
	m_SelectItem = itemkind
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
		
	local sprite, color, name, c, desc = AwardInfo( m_SelectItem )
	SetImage( ItemObj.transform:Find("Shape"), sprite )
	SetImage( ItemObj.transform:Find("Color"), color )
	SetText( ItemObj.transform:Find("Num"), "" );
	SetText( ItemName, name, NameColor(c) );
	SetText( ItemDesc, desc )
	SetText( m_uiUseNum, m_SelectItemNum );
	
	m_buytoken = item_gettoken( m_SelectItem );
	local cost = m_SelectItemNum*m_buytoken
	if GetPlayer().m_token < cost then
		SetRichText( CostToken, F( 2274, cost ) )
	else
		SetRichText( CostToken, F( 2273, cost ) )
	end
	SetFalse( UseButton )
end

-- 隐藏购买确定页
function MaterialGetDlgHideBuyInfoLayer()
	SetFalse( m_uiBuyInfo )
end

-- 设置数量
function MaterialGetDlgBuyItemSetNum( num )
	m_SelectItemNum = m_SelectItemNum + num
	if m_SelectItemNum <= 0 then
		m_SelectItemNum = 1;
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
function MaterialGetDlgItemBuy()
	local cost = m_SelectItemNum*m_buytoken
	if GetPlayer().m_token < cost then
		JumpToken()
		return
	end

	MsgBox( F(1778, cost, item_getname( m_SelectItem ), m_SelectItemNum ), function() 
		system_askinfo( ASKINFO_ITEM, "", ITEM_PROCESS_BUY, m_SelectItem, m_SelectItemNum );
		MaterialGetDlgHideBuyInfoLayer()
	end )
end