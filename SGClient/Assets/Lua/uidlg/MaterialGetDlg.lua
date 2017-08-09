-- 界面
local m_Dlg = nil;
local m_uiText = nil; --UnityEngine.GameObject
local m_uiTabs = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject

local m_nTabKind = {}
local m_nContentKind = {}
local m_index = 0;

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
			
		-- 选择页签
		elseif nControlID >= 1 and nControlID <= 4 then
			MaterialGetDlgSelectTab( nControlID )
			
		-- 前往征收
		elseif nControlID == 10 then
			LevyDlgShow()
			MaterialGetDlgClose();
		
		-- 使用指定道具
		elseif nControlID > 1000 and nControlID < 2000 then
			MaterialGetDlgUse( nControlID-1000 )
			
		-- 购买指定道具
		elseif nControlID > 2000 and nControlID < 3000 then
			MaterialGetDlgBuy( nControlID-2000 )
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
		
	elseif itemkind == 1 or itemkind == 11 or itemkind == 21 or itemkind == 31 then
		SetText( m_uiText, T(775) );
		m_nTabKind = { 1, 11, 21, 31 };
		m_nContentKind = { { -1,81,91,92 }, { -11,81,91,92 },{ -21,81,91,92 }, { -31,81,91,92 }}
		
	elseif itemkind == 2 or itemkind == 12 or itemkind == 22 or itemkind == 32 then
		SetText( m_uiText, T(776) );
		m_nTabKind = { 2, 12, 22, 32 };
		m_nContentKind = { { -1,83,93,94 }, { -11,83,93,94 },{ -21,83,93,94 }, { -31,83,93,94 }}
		
	elseif itemkind == 3 or itemkind == 13 or itemkind == 23 or itemkind == 33 then
		SetText( m_uiText, T(777) );
		m_nTabKind = { 3, 13, 23, 33 };
		m_nContentKind = { { -1,85,95,96 }, { -11,85,95,96 },{ -21,85,95,96 }, { -31,85,95,96 }}
		
	end
	
	for i=1, #m_nTabKind do
		local uiTab = m_uiTabs.transform:GetChild(i-1).gameObject;
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

-- 购买道具
function MaterialGetDlgBuy( itemkind )
	if GetPlayer().m_token < item_gettoken( itemkind ) then
        JumpToken();
		return -1;
	end
	system_askinfo( ASKINFO_ITEM, "", ITEM_PROCESS_BUY, itemkind, 1 );
end