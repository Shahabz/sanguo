-- 界面
local m_Dlg = nil;
local m_DialogFrameMod = nil;

local m_uiItemBtn = nil; --UnityEngine.GameObject
local m_uiEquipBtn = nil; --UnityEngine.GameObject
local m_uiItemScroll = nil; --UnityEngine.GameObject
local m_uiEquipScroll = nil; --UnityEngine.GameObject
local m_uiItemContent = nil; --UnityEngine.GameObject
local m_uiEquipContent = nil; --UnityEngine.GameObject
local m_uiUIP_ItemRow = nil; --UnityEngine.GameObject
local m_uiUIP_EquipRow = nil; --UnityEngine.GameObject
local m_uiItemInfo = nil; --UnityEngine.GameObject
local m_uiEquipInfo = nil; --UnityEngine.GameObject
local m_uiEquipExt = nil; --UnityEngine.GameObject
local m_uiEquipExtButton = nil; --UnityEngine.GameObject

local m_uiUseNum = nil;
local m_SelectType = 0;
local m_SelectItemIndex = -1;
local m_SelectEquipIndex = -1;
local m_SelectItemNum = 0;
local m_SelectHeroKind = 0;

-- 数据缓存
local m_CacheItemCache = { };
local m_CacheEquipCache = { };

-- 对象缓存
local m_CacheItemList = { };
local m_CacheEquipList = { };

-- 打开界面
function BagDlgOpen()
	m_Dlg = eye.uiManager:Open( "BagDlg" );
	m_DialogFrameMod = DialogFrameModOpen( m_Dlg, T(556), 9, BagDlgClose );
end

-- 隐藏界面
function BagDlgClose()
	if m_Dlg == nil then
		return;
	end
	DialogFrameModClose( m_DialogFrameMod );
	eye.uiManager:Close( "BagDlg" );
end

-- 删除界面
function BagDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function BagDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            BagDlgClose();
		elseif nControlID == -2 then
			BagDlgSelectItem( -1 );
			BagDlgSelectEquip( -1 );
		elseif nControlID == 1 then
			BagDlgSelectType(1);
		elseif nControlID == 2 then
			BagDlgSelectType(2);
		
		-- 购买容量
		elseif nControlID == 10 then
			BagDlgEquipBuyEquipExt()
			
		-- 减10
		elseif nControlID == 11 then
			BagDlgSelectItemSetNum( -10 )
			
		-- 减1
		elseif nControlID == 12 then
			BagDlgSelectItemSetNum( -1 )
		
		-- 加10
		elseif nControlID == 13 then
			BagDlgSelectItemSetNum( 10 )
			
		-- 加1
		elseif nControlID == 14 then
			BagDlgSelectItemSetNum( 1 )
		
		-- 售卖
		elseif nControlID == 15 then
			BagDlgItemSell()
			
		-- 使用
		elseif nControlID == 16 then
			BagDlgItemUse()
		
		-- 分解
		elseif nControlID == 21 then
			BagDlgEquipResolve()
		
		-- 装备
		elseif nControlID == 22 then
			BagDlgEquipUp()
			
		-- 替换
		elseif nControlID == 23 then
			BagDlgEquipUp()
			
		-- 洗炼
		elseif nControlID == 24 then
			EquipWashDlgShow()
			BagDlgClose();
			
		-- 已装备
		elseif nControlID >= 31 and nControlID <= 34 then
		
			
		-- 选择道具
		elseif nControlID >= 1000 and nControlID < 2000 then
			BagDlgSelectItem( nControlID-1000 )
			
		-- 选择装备
		elseif nControlID >= 2000 and nControlID < 3000 then
			BagDlgSelectEquip( nControlID-2000 )
        end
	end
end

-- 载入时调用
function BagDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiItemBtn = objs[0];
	m_uiEquipBtn = objs[1];
	m_uiItemScroll = objs[2];
	m_uiEquipScroll = objs[3];
	m_uiItemContent = objs[4];
	m_uiEquipContent = objs[5];
	m_uiUIP_ItemRow = objs[6];
	m_uiUIP_EquipRow = objs[7];
	m_uiItemInfo = objs[8];
	m_uiEquipInfo = objs[9];
	m_uiEquipExt = objs[10];
	m_uiEquipExtButton = objs[11];
end

-- 界面初始化时调用
function BagDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function BagDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function BagDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function BagDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function BagDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function BagDlgShow()
	BagDlgOpen()
	BagDlgSelectType(1);
end

-- 选择道具or装备
function BagDlgSelectType(type)
    m_SelectType = type;
    -- 读取缓存
	if type == 1 then
		SetTrue( m_uiItemScroll )
		SetFalse( m_uiEquipScroll )
		BagDlgLoadItem()
	elseif type == 2 then
		SetFalse( m_uiItemScroll )
		SetTrue( m_uiEquipScroll )
		BagDlgLoadEquip()
	end
	BagDlgSelectItem( -1 )
	BagDlgSelectEquip( -1 )
end
-- 缓存排序
function BagDlgItemCacheSort(a, b)
    if a ~= nil and b ~= nil then
        if a.m_sort < b.m_sort then
            return true;
        else
            return false;
        end
    else
        return false;
    end
end
function BagDlgEquipCacheSort(a, b)
    if a ~= nil and b ~= nil then
        if a.m_kind < b.m_kind then
            return true;
        else
            return false;
        end
    else
        return false;
    end
end

-- 清空道具对象
function BagDlgClearItem()
    if m_uiItemContent ~= nil then
        local childCount = m_uiItemContent.transform.childCount;
        for i = 6, childCount - 1, 1 do
            local child = m_uiItemContent.transform:GetChild(i).gameObject;
            -- 不删除,改为隐藏
            child:SetActive(false);
        end
    end
    m_CacheItemCache = { };
    m_CacheItemList = { };
end
-- 清空装备对象
function BagDlgClearEquip()
    if m_uiEquipContent ~= nil then
        local childCount = m_uiEquipContent.transform.childCount;
        for i = 6, childCount - 1, 1 do
            local child = m_uiEquipContent.transform:GetChild(i).gameObject;
            -- 不删除,改为隐藏
            child:SetActive(false);
        end
    end
    m_CacheEquipCache = { };
    m_CacheEquipList = { };
end


-- 清除一行的状态为初始的显示
function BagDlgClearItemRow(row)
    local trans = row.transform;
    for tmpi = 0, trans.childCount - 1, 1 do
        local child = trans:GetChild(tmpi);
		local objs = child.transform:GetComponent( typeof(Reference) ).relatedGameObject;
		local uiShape = objs[0];
		local uiNum = objs[1];
		local uiColor = objs[2];
		local uiNew = objs[3];
		SetFalse( uiShape )
		SetFalse( uiNum )
		SetFalse( uiColor )
		SetFalse( uiNew )
		child:GetComponent("UIButton").controlID = 0;
    end
end
-- 清除一行的状态为初始的显示
function BagDlgClearEquipRow(row)
    local trans = row.transform;
    for tmpi = 0, trans.childCount - 1, 1 do
        local child = trans:GetChild(tmpi);
		local objs = child.transform:GetComponent( typeof(Reference) ).relatedGameObject;
		local uiShape = objs[0];
		local uiColor = objs[1];
		local uiNew = objs[2];
		SetFalse( uiShape )
		SetFalse( uiColor )
		SetFalse( uiNew )
		child:GetComponent("UIButton").controlID = 0;
    end
end

-- 读取缓存道具
function BagDlgLoadItem()
	if m_Dlg == nil then
		return;
	end
	if IsActive( m_uiItemScroll ) == false then
		return;
	end
    BagDlgClearItem()
    local itemCount = 0;
    local currItemRow = nil;
    local rowCount = 0;

    -- 先放进临时缓存
    for nItemIndex = 1, MAX_ITEMNUM, 1 do
        local pItem = GetItem().m_Item[nItemIndex];
        if pItem ~= nil and pItem.m_kind > 0 then
            table.insert(m_CacheItemCache, { m_kind = pItem.m_kind, m_num = pItem.m_num,  m_bIsNew = pItem.m_bIsNew, m_offset = nItemIndex, m_sort = item_getsort(pItem.m_kind) });
        end
    end

    -- 排序
    table.sort(m_CacheItemCache, BagDlgItemCacheSort);

    -- 创建对象
    for nItemIndex = 1, #m_CacheItemCache, 1 do
        local pItem = m_CacheItemCache[nItemIndex];
        if pItem ~= nil and pItem.m_kind > 0 then
            local itemIndex = itemCount % 5;
            if itemIndex == 0 then
                if rowCount >= m_uiItemContent.transform.childCount then
                    currItemRow = GameObject.Instantiate(m_uiUIP_ItemRow);
                    currItemRow.transform:SetParent(m_uiItemContent.transform);
                    currItemRow.transform.localScale = Vector3.one;
                    currItemRow:SetActive(true);
                else
                    currItemRow = m_uiItemContent.transform:GetChild(rowCount).gameObject;
                    currItemRow:SetActive(true);
                end
				BagDlgClearItemRow(currItemRow);
                rowCount = rowCount + 1;
            end
            local uiItemObj = currItemRow.transform:GetChild(itemIndex);
            uiItemObj:GetComponent("UIButton").controlID = 1000 + pItem.m_offset;
			local objs = uiItemObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
			local uiShape = objs[0];
			local uiNum = objs[1];
			local uiColor = objs[2];
			local uiNew = objs[3];
			
			SetTrue( uiShape )
			SetTrue( uiNum )
			SetTrue( uiColor )
			SetShow( uiNew, pItem.m_bIsNew )
			
			SetImage( uiShape, ItemSprite(pItem.m_kind) )
			SetText( uiNum, "x"..pItem.m_num )
			SetImage( uiColor, ItemColorSprite(item_getcolor(pItem.m_kind)) )
			
			m_CacheItemList[pItem.m_offset] = uiItemObj;
			itemCount = itemCount + 1;
        end
    end
end
function BagDlgLoadEquip()
	if m_Dlg == nil then
		return;
	end
	if IsActive( m_uiEquipScroll ) == false then
		return;
	end
    BagDlgClearEquip()
	BagDlgEquipExtSet();
    local equipCount = 0;
    local currEquipRow = nil;
    local rowCount = 0;

    -- 先放进临时缓存
    for nEquipIndex = 1, MAX_EQUIPNUM, 1 do
        local pEquip = GetEquip().m_Equip[nEquipIndex];
        if pEquip ~= nil and pEquip.m_kind > 0 then
            table.insert(m_CacheEquipCache, { m_kind = pEquip.m_kind, m_bIsNew = pEquip.m_bIsNew, m_offset = nEquipIndex, });
        end
    end

    -- 排序
    table.sort(m_CacheEquipCache, BagDlgEquipCacheSort);

    -- 创建对象
    for nEquipIndex = 1, #m_CacheEquipCache, 1 do
        local pEquip = m_CacheEquipCache[nEquipIndex];
        if pEquip ~= nil and pEquip.m_kind > 0 then
            local equipIndex = equipCount % 5;
            if equipIndex == 0 then
                if rowCount >= m_uiEquipContent.transform.childCount then
                    currEquipRow = GameObject.Instantiate(m_uiUIP_EquipRow);
                    currEquipRow.transform:SetParent(m_uiEquipContent.transform);
                    currEquipRow.transform.localScale = Vector3.one;
                    currEquipRow:SetActive(true);
                else
                    currEquipRow = m_uiEquipContent.transform:GetChild(rowCount).gameObject;
                    currEquipRow:SetActive(true);
                end
				BagDlgClearEquipRow(currEquipRow);
                rowCount = rowCount + 1;
            end
            local uiEquipObj = currEquipRow.transform:GetChild(equipIndex);
            uiEquipObj:GetComponent("UIButton").controlID = 2000 + pEquip.m_offset;
			local objs = uiEquipObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
			local uiShape = objs[0];
			local uiColor = objs[1];
			local uiNew = objs[2];
			
			SetTrue( uiShape )
			SetTrue( uiColor )
			SetShow( uiNew, pEquip.m_bIsNew )
			
			SetImage( uiShape, EquipSprite(pEquip.m_kind) )
			SetImage( uiColor, ItemColorSprite(equip_getcolor(pEquip.m_kind)) )
			
			m_CacheEquipList[pEquip.m_offset] = uiEquipObj;
			equipCount = equipCount + 1;
        end
    end
end

-- 单个道具刷新
function BagDlgItemChange( offset )
	-- 获取道具
    local pItem = GetItem():GetAnyItem( offset );
    if pItem == nil then
        return;
    end
	-- 道具对象
	local uiItemObj = m_CacheItemList[offset+1];
    if uiItemObj == nil then
        return;
    end
	SetText( uiItemObj.transform:Find("Num"), "x"..pItem.m_num );
end

-- 选择道具
function BagDlgSelectItem( offset )
	m_SelectItemIndex = offset;
	m_SelectItemNum = 1;
	m_uiUseNum = nil;
	if offset == -1 then
		SetFalse( m_uiItemInfo );
		return
	end
	
	-- 获取道具
    local pItem = GetItem():GetAnyItem(m_SelectItemIndex - 1);
    if pItem == nil then
        return;
    end
	
	-- 道具对象
	local uiItemObj = m_CacheItemList[m_SelectItemIndex];
    if uiItemObj == nil then
        return;
    end
	pItem.m_bIsNew = false;
    SetFalse( uiItemObj.transform:Find("New") );
	
	SetTrue( m_uiItemInfo );
	local objs = m_uiItemInfo.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local ItemObj = objs[0];
	local ItemName = objs[1];
	local ItemDesc = objs[2];
	local UseNum = objs[3];
	local SellButton = objs[4];
	local UseButton = objs[5];
	local NumSelect = objs[6];
	m_uiUseNum = UseNum;
			
	SetImage( ItemObj.transform:Find("Shape"), ItemSprite(pItem.m_kind) )
	SetImage( ItemObj.transform:Find("Color"), ItemColorSprite(item_getcolor(pItem.m_kind)) )
	SetText( ItemObj.transform:Find("Num"), "x"..pItem.m_num );
	SetText( ItemName, item_getname( pItem.m_kind ) );
	SetText( m_uiUseNum, m_SelectItemNum );
	
	-- 出售
	if item_getprice( pItem.m_kind ) > 0 then
		SetTrue( SellButton );
	else
		SetFalse( SellButton );
	end
	
	-- 使用
	if (pItem.m_type == ITEM_TYPE_NORMAL_USE or
        pItem.m_type == ITEM_TYPE_SCRIPT_USE or 
		pItem.m_type == ITEM_TYPE_EQUIP_DRAWING or
		pItem.m_type == ITEM_TYPE_HEROEXP) then
		SetTrue( NumSelect );
		SetTrue( UseButton );
        -- 检查物品是否可以一起使用多个
        if Utils.byteAndOp(pItem.m_situation, 1) == 0 then
           
        else
           
        end
	else
		SetFalse( NumSelect );
		SetFalse( UseButton );
	end
end

-- 设置使用道具的数量
function BagDlgSelectItemSetNum( num )
	local pItem = GetItem():GetAnyItem(m_SelectItemIndex - 1);
    if pItem == nil then
        return;
    end
	if num > 0 then
		m_SelectItemNum = m_SelectItemNum + num
		if m_SelectItemNum > pItem.m_num then
			m_SelectItemNum = pItem.m_num;
		end
	else
		m_SelectItemNum = m_SelectItemNum + num
		if m_SelectItemNum <= 0 then
			m_SelectItemNum = 1;
		end
	end
	SetText( m_uiUseNum, m_SelectItemNum );
end

-- 道具售卖
function BagDlgItemSell()
	local pItem = GetItem():GetAnyItem(m_SelectItemIndex - 1);
    if pItem == nil then
        return;
    end
	if m_SelectItemNum <= 0 then
		return;
	end
	MsgBox( F( 686, m_SelectItemNum, item_getname(pItem.m_kind), item_getprice(pItem.m_kind)*m_SelectItemNum ), function()
		system_askinfo( ASKINFO_ITEM, "", ITEM_PROCESS_SELL, m_SelectItemIndex - 1, m_SelectItemNum );
		BagDlgSelectItem( -1 )
	end )
end

-- 道具使用
function BagDlgItemUse()
	local pItem = GetItem():GetAnyItem(m_SelectItemIndex - 1);
    if pItem == nil then
        return;
    end
	if m_SelectItemNum <= 0 then
		return;
	end
	system_askinfo( ASKINFO_ITEM, "", ITEM_PROCESS_USE, m_SelectItemIndex - 1, m_SelectItemNum, -1, 1 );
	BagDlgSelectItem( -1 )
end

-- 选择装备
function BagDlgSelectEquip( offset )
	m_SelectEquipIndex = offset;
	if offset == -1 then
		SetFalse( m_uiEquipInfo );
		return
	end
	
	-- 获取装备
    local pEquip = GetEquip():GetAnyEquip(m_SelectEquipIndex - 1);
    if pEquip == nil then
        return;
    end
	
	-- 装备对象
	local uiEquipObj = m_CacheEquipList[m_SelectEquipIndex];
    if uiEquipObj == nil then
        return;
    end
	pEquip.m_bIsNew = false;
    SetFalse( uiEquipObj.transform:Find("New") );
	
	SetTrue( m_uiEquipInfo );
	local objs = m_uiEquipInfo.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local EquipObj = objs[0];
	local EquipName = objs[1];
	local EquipDesc = objs[2];
	local SellButton = objs[3];
	local UseButton = objs[4];
	local HeroEquip = {};
	HeroEquip[1] = objs[5];
	HeroEquip[2] = objs[6];
	HeroEquip[3] = objs[7];
	HeroEquip[4] = objs[8];
	local ReplaceButton = objs[9];
	local WashButton = objs[10];
			
	SetImage( EquipObj.transform:Find("Shape"), EquipSprite(pEquip.m_kind) )
	SetImage( EquipObj.transform:Find("Color"), ItemColorSprite(equip_getcolor(pEquip.m_kind)) )
	SetText( EquipName, equip_getname( pEquip.m_kind ) );
	SetText( EquipDesc, equip_getabilityname( pEquip.m_kind ) );
	
	SetTrue( SellButton ); -- 分解
	SetFalse( UseButton ); -- 装备
	SetFalse( ReplaceButton ); -- 替换
	-- 洗炼
	if equip_getcolor(pEquip.m_kind) >= ITEM_COLOR_LEVEL_BLUE then
		SetTrue( WashButton );
	else
		SetFalse( WashButton );
	end
	
end

-- 装备分解
function BagDlgEquipResolve()
	local pEquip = GetEquip():GetAnyEquip(m_SelectEquipIndex - 1);
    if pEquip == nil then
        return;
    end
	MsgBox( F( 689, T(152).."x"..equip_getprestige( pEquip.m_kind ) ), function()
		system_askinfo( ASKINFO_EQUIP, "", 1, m_SelectEquipIndex - 1 );
		BagDlgSelectEquip( -1 )
	end )
end

-- 装备上
function BagDlgEquipUp()
	local pEquip = GetEquip():GetAnyEquip(m_SelectEquipIndex - 1);
    if pEquip == nil then
        return;
    end
	
end

-- 购买装备容量
function BagDlgEquipBuyEquipExt()
	MsgBox( F( 685, global.equipext_token ), function()
		system_askinfo( ASKINFO_EQUIP, "", 4 );
	end )
end

-- 装备容量
function BagDlgEquipExtSet()	
	SetText( m_uiEquipExt, T(683) .." ".. GetEquip():GetTotal() .."/" ..(GetPlayer().m_equipext+MAX_DEFAULT_EQUIPNUM) )
	if GetPlayer().m_equipext >= (MAX_EQUIPNUM-MAX_DEFAULT_EQUIPNUM) then
		SetFalse( m_uiEquipExtButton );
	else
		SetTrue( m_uiEquipExtButton )
	end
end
