-- 界面
local m_Dlg = nil;
local m_DialogFrameMod = nil;

local m_uiItemScroll = nil; --UnityEngine.GameObject
local m_uiItemContent = nil; --UnityEngine.GameObject
local m_uiUIP_ItemRow = nil; --UnityEngine.GameObject
local m_uiItemInfo = nil; --UnityEngine.GameObject
local m_uiResBtn = nil; --UnityEngine.GameObject
local m_uiDrawBtn = nil; --UnityEngine.GameObject
local m_uiOtherBtn = nil; --UnityEngine.GameObject

local m_uiUseNum = nil;
local m_SelectItemIndex = -1;
local m_SelectItemNum = 0;
local m_SelectType = 0;

-- 数据缓存
local m_CacheItemCache = {};
-- 对象缓存
local m_CacheItemList = {};


-- 打开界面
function BagDlgOpen()
	m_Dlg = eye.uiManager:Open( "BagDlg" );
	m_DialogFrameMod = DialogFrameModOpen( m_Dlg, T(556), HELP_BagDlg, BagDlgClose );
end

-- 隐藏界面
function BagDlgClose()
	if m_Dlg == nil then
		return;
	end
	DialogFrameModClose( m_DialogFrameMod );
	m_DialogFrameMod = nil;
	eye.uiManager:Close( "BagDlg" );
	m_CacheItemCache = {};
    m_CacheItemList = {};
end

-- 删除界面
function BagDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
	m_CacheItemCache = {};
    m_CacheItemList = {};
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
		elseif nControlID == 1 then
			BagDlgSelectType(1);
		elseif nControlID == 2 then
			BagDlgSelectType(2);
		elseif nControlID == 3 then
			BagDlgSelectType(3);
				
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
			
		-- 选择道具
		elseif nControlID >= 1000 and nControlID < 2000 then
			BagDlgSelectItem( nControlID-1000 )
        end
	end
end

-- 载入时调用
function BagDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiItemScroll = objs[0];
	m_uiItemContent = objs[1];
	m_uiUIP_ItemRow = objs[2];
	m_uiItemInfo = objs[3];
	m_uiResBtn = objs[4];
	m_uiDrawBtn = objs[5];
	m_uiOtherBtn = objs[6];
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

-- 选择道具0资源1图纸2其它
function BagDlgSelectType(type)
    -- 读取缓存
	BagDlgSelectItem( -1 )
	if type == 1 then
		m_SelectType = 1
		SetImage( m_uiResBtn.transform:Find("Back"), LoadSprite("ui_button_18") )
		SetTextColor( m_uiResBtn.transform:Find("Text"),Hex2Color(0xF7F3BBFF) )
		SetImage( m_uiDrawBtn.transform:Find("Back"), LoadSprite("ui_button_17") )
		SetTextColor( m_uiDrawBtn.transform:Find("Text"),Hex2Color(0xDDBD88FF) )
		SetImage( m_uiOtherBtn.transform:Find("Back"), LoadSprite("ui_button_17") )
		SetTextColor( m_uiOtherBtn.transform:Find("Text"),Hex2Color(0xDDBD88FF) )
		BagDlgLoadItem()
		
	elseif type == 2 then
		m_SelectType = 2
		SetImage( m_uiResBtn.transform:Find("Back"), LoadSprite("ui_button_17") )
		SetTextColor( m_uiResBtn.transform:Find("Text"),Hex2Color(0xDDBD88FF) )
		SetImage( m_uiDrawBtn.transform:Find("Back"), LoadSprite("ui_button_18") )
		SetTextColor( m_uiDrawBtn.transform:Find("Text"),Hex2Color(0xF7F3BBFF) )
		SetImage( m_uiOtherBtn.transform:Find("Back"), LoadSprite("ui_button_17") )
		SetTextColor( m_uiOtherBtn.transform:Find("Text"),Hex2Color(0xDDBD88FF) )
		BagDlgLoadItem()
		
	elseif type == 3 then
		m_SelectType = 0
		SetImage( m_uiResBtn.transform:Find("Back"), LoadSprite("ui_button_17") )
		SetTextColor( m_uiResBtn.transform:Find("Text"),Hex2Color(0xDDBD88FF) )
		SetImage( m_uiDrawBtn.transform:Find("Back"), LoadSprite("ui_button_17") )
		SetTextColor( m_uiDrawBtn.transform:Find("Text"),Hex2Color(0xDDBD88FF) )
		SetImage( m_uiOtherBtn.transform:Find("Back"), LoadSprite("ui_button_18") )
		SetTextColor( m_uiOtherBtn.transform:Find("Text"),Hex2Color(0xF7F3BBFF) )
		BagDlgLoadItem()
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

-- 清空道具对象
function BagDlgClearItem()
    if m_uiItemContent ~= nil then
		--[[for i = 0, 5, 1 do
			BagDlgClearItemRow( m_uiItemContent.transform:GetChild(i):Find("Content").gameObject )
		end--]]
        local childCount = m_uiItemContent.transform.childCount;
        for i = 0, childCount - 1, 1 do
            local child = m_uiItemContent.transform:GetChild(i).gameObject;
            -- 不删除,改为隐藏
			--if child.name ~= 'ItemInfo' then
				child:SetActive(false);
			--end
        end
    end
    m_CacheItemCache = { };
    m_CacheItemList = { };
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
		local uiSelect = objs[4];
		local uiNumBack = objs[5];
		SetFalse( uiShape )
		SetFalse( uiNum )
		SetFalse( uiColor )
		SetFalse( uiNew )
		SetFalse( uiSelect )
		SetFalse( uiNumBack )
		child:GetComponent("UIButton").controlID = 0;
    end
end

-- 读取缓存道具
function BagDlgLoadItem()
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	
	if IsActive( m_uiItemScroll ) == false then
		return;
	end
    BagDlgClearItem()
    -- 先放进临时缓存
    for nItemIndex = 1, MAX_ITEMNUM, 1 do
        local pItem = GetItem().m_Item[nItemIndex];
        if pItem ~= nil and pItem.m_kind > 0 and m_SelectType == item_getfilter(pItem.m_kind) then
            table.insert(m_CacheItemCache, { m_kind = pItem.m_kind, m_num = pItem.m_num,  m_bIsNew = pItem.m_bIsNew, m_offset = nItemIndex, m_sort = item_getsort(pItem.m_kind) });
        end
    end
	
	-- 一个道具都没有
	if #m_CacheItemCache == 0 then
		return;
	end
	
    -- 排序
    table.sort(m_CacheItemCache, BagDlgItemCacheSort);
	
	--coroutine.stop( BagDlgCreateItem )
	--coroutine.start( BagDlgCreateItem )
	-- 创建
	BagDlgCreateItem()
end

function BagDlgCreateItem()
	local itemCount = 0;
    local currItemRow = nil;
    local rowCount = 2;
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
					--coroutine.wait(0.03) -- 此处等待
                else
                    currItemRow = m_uiItemContent.transform:GetChild(rowCount).gameObject;
                    currItemRow:SetActive(true);
                end
				BagDlgClearItemRow(currItemRow.transform:Find("Content"));
                rowCount = rowCount + 1;
            end
            local uiItemObj = currItemRow.transform:Find("Content").transform:GetChild(itemIndex);
            uiItemObj:GetComponent("UIButton").controlID = 1000 + pItem.m_offset;
			local objs = uiItemObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
			local uiShape = objs[0];
			local uiNum = objs[1];
			local uiColor = objs[2];
			local uiNew = objs[3];
			local uiSelect = objs[4];
			local uiNumBack = objs[5];
			
			SetTrue( uiShape )
			SetTrue( uiNum )
			SetTrue( uiColor )
			SetShow( uiNew, pItem.m_bIsNew )
			SetFalse( uiSelect )
			SetTrue( uiNumBack )
			
			SetImage( uiShape, ItemSprite(pItem.m_kind) )
			SetText( uiNum, pItem.m_num )
			SetImage( uiColor, ItemColorSprite(item_getcolor(pItem.m_kind)) )
			
			m_CacheItemList[pItem.m_offset] = uiItemObj;
			itemCount = itemCount + 1;
        end
    end
end

-- 单个道具刷新
function BagDlgItemChange( offset )
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	if IsActive( m_uiItemScroll ) == false then
		return;
	end
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
	SetText( uiItemObj.transform:Find("Num"),pItem.m_num );
end

-- 选择道具
function BagDlgSelectItem( offset )
	-- 关掉上一个选择框
	local uiLastItemObj = m_CacheItemList[m_SelectItemIndex];
    if uiLastItemObj ~= nil then
        uiLastItemObj.transform:Find("Select").gameObject:SetActive(false);
	end	
	m_uiItemInfo.transform:SetAsFirstSibling();
	
	m_SelectItemNum = 1;
	m_uiUseNum = nil;
	if offset == -1 or m_SelectItemIndex == offset then
		m_SelectItemIndex = -1;
		m_SelectItemNum = 0;
		SetFalse( m_uiItemInfo );
		return
	end

	m_SelectItemIndex = offset;
	m_SelectItemNum = 1;
		
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
	SetTrue( uiItemObj.transform:Find("Select") )
	
	 -- 选择的道具位置
    local siblingIndex = uiItemObj.parent.parent:GetSiblingIndex();
    if m_uiItemInfo.transform:GetSiblingIndex() < siblingIndex then
        m_uiItemInfo.transform:SetSiblingIndex(siblingIndex);
    else
        m_uiItemInfo.transform:SetSiblingIndex(siblingIndex + 1);
    end
	SetTrue( m_uiItemInfo );
	-- 判断弹出菜单是否显示在最下面
    local uiItemParent = uiItemObj.parent.parent.transform;
    local posY = - uiItemParent.localPosition.y - m_uiItemContent.transform.offsetMax.y;
    if posY + 300 > m_uiItemScroll.transform.rect.height then
        local siblingIndex = m_uiItemInfo.transform:GetSiblingIndex();
        m_uiItemScroll:GetComponent("UIScrollRect"):ScrollToBottom(siblingIndex);
    end
		
	local objs = m_uiItemInfo.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local ItemName = objs[1];
	local ItemDesc = objs[2];
	local UseNum = objs[3];
	local SellButton = objs[4];
	local UseButton = objs[5];
	local NumSelect = objs[6];
	m_uiUseNum = UseNum;
	
	SetText( ItemName, item_getname( pItem.m_kind ) );
	SetText( ItemDesc, item_getdesc( pItem.m_kind ) )
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
           SetFalse( NumSelect );
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
	
	if pItem.m_type == ITEM_TYPE_EQUIP_DRAWING then
		EquipForgingDlgShow( nil, g_itemkind[pItem.m_kind].base_ability0 )
		BagDlgClose();
	elseif pItem.m_type == ITEM_TYPE_HEROEXP then
		if GetHero().m_CityHero[0] ~= nil then
			HeroExpDlgShow( GetHero().m_CityHero[0].m_kind )
		end
	else
		if pItem.m_kind == 131 then
			BagDlgClose()
			if GameManager.currentScence == "city" then
				WorldMap.GotoWorldMap(-1, -1)
			end
		end
		system_askinfo( ASKINFO_ITEM, "", ITEM_PROCESS_USE, m_SelectItemIndex - 1, m_SelectItemNum, -1, 1 );
	end	
	BagDlgSelectItem( -1 )
end
