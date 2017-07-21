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

local m_SelectType = 0;
local m_SelectItemIndex = -1;
local m_SelectEquipIndex = -1;

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
		elseif nControlID == 1 then
			BagDlgSelectType(1);
		elseif nControlID == 2 then
			BagDlgSelectType(2);
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
    BagDlgClearEquip()
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

