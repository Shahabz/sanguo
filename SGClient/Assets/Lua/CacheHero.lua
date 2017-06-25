-- 英雄缓存信息
-- 函数参数名称带有"_"的都是服务器真实的offset，0开始

MAX_HERONUM		= 128 -- 英雄数量
MAX_CITYHERONUM		= 4 -- 上阵英雄数量

-- 一个英雄结构
SLK_Hero = class("SLK_Hero")
function SLK_Hero:ctor()
	self:empty();
end

function SLK_Hero:empty()
	
	self.m_kind  			= 0;-- 英雄种类
	self.m_level			=	0;-- 英雄等级
	self.m_soldiers		=	0;-- 兵力
	
	-- 英雄装备
	self.m_Equip			=	{};
	for i=0,5,1 do
		local object = SLK_Equip.new();
		table.insert( self.m_Equip, i, object );
	end
	
	self.m_bIsNew = false;
	return self;
end


-- 英雄部分客户端缓存
local Hero = class("Hero")
function Hero:ctor()
	self:ResetAll();
end

-- 清空缓存
function Item:ResetAll()
	
	-- 上阵英雄列表
	self.m_CityHero = {};
	for i=0,MAX_CITYHERONUM-1,1 do
		local object = SLK_Hero.new();
		table.insert( self.m_CityHero, i, object );
	end
	
	-- 英雄列表
	self.m_Hero = {};
	for i=0,MAX_HERONUM-1,1 do
		local object = SLK_Hero.new();
		table.insert( self.m_Hero, i, object );
	end
	
end

-- 设置
function Hero:SetHero( nIndex, pHero )
	if pHero ~= nil then
		self.m_Hero[nIndex] = clone(pHero); -- 这个地方是否要用到深拷贝
	else
		self.m_Item[nIndex]:empty(); -- 清空这个英雄
	end
	self:OnItemChange( nIndex );
end

-- 改变道具信息
-- EventProtocol.addEventListener( "OnItemChange", function( recvValue ) end )
function Item:OnItemChange( _ItemIndex )
	local nItemIndex = _ItemIndex + 1;
	-- 发布事件
	EventProtocol.dispatchEvent( "OnItemChange", { itemIndex=_ItemIndex, kind=self.m_Item[nItemIndex].m_kind, num=self.m_Item[nItemIndex].m_num } );
end

-- 服务器返回物品使用了
function Item:ItemUsed( _ItemIndex, nItemNum, nItemEff )
	local nItemIndex = _ItemIndex + 1;
	if _ItemIndex < 0 or _ItemIndex >= MAX_ITEMNUM then
		return;
	end
	if self:HasItem( _ItemIndex ) == false then
		return;
	end
	
	
		-- 正数表示的是使用个数，负数表示不能使用
		if nItemNum > 0 then
			self.m_Item[nItemIndex].m_num = self.m_Item[nItemIndex].m_num - nItemNum;
		elseif nItemNum == -2 then
		end

    -- 使用消息, 特定不显示
    --local kind = self.m_Item[nItemIndex].m_kind;
    --if kind ~= 113 then
        --Notify( 113, { item_getname( kind ) .. " x" .. nItemNum } );
    --end
		
		-- 道具使用没了
		if self.m_Item[nItemIndex].m_num <= 0 then
			self:SetItem( _ItemIndex, nil );
			return;
		end
		
		-- 改变道具信息
		self:OnItemChange( _ItemIndex );
end

-- 得到服务器返回的消息，得到一个物品
function Item:ItemGet( _ItemIndex, kind, type, num, color, situation, path )
	local nItemIndex = _ItemIndex + 1;
	if kind <= 0 or num <= 0 then
		return;
	end
	
	-- 得到物品
	local pitem = self.m_Item[nItemIndex];
	if pitem.m_kind ~= kind or pitem.m_num <= 0 then
		pitem.m_bIsUpdate = false;
	end
	pitem.m_num = pitem.m_num + num;
	pitem.m_kind = kind;
	pitem.m_type = type;
	pitem.m_color_level = color;
	pitem.m_situation = situation;
	self:SetItem( _ItemIndex, pitem );

	self:OnGetItem( _ItemIndex, kind, num, path );
end

-- 对得到的这个物品进行其它处理，比如提示等
function Item:OnGetItem( _ItemIndex, nItemKind, num, path )
	local nItemIndex = _ItemIndex + 1;
	local pItem = self:GetAnyItem( _ItemIndex );
	if pItem == nil then
		return;
	end
	-- 新道具标示
	pItem.m_bIsNew = true;
	
	-- 处理
	if GameManager.inited == false then
		return;
	end
	
  if path == PATH_GM or path == PATH_SYSTEM then
     -- 忽略
     return;
	end

  -- 提示
  --GainDlgAdd( 1, nItemKind, num );
end

-- 得到服务器返回的消息，失去道具
function Item:ItemLost( _ItemIndex, nItemNum, path )
	local nItemIndex = _ItemIndex + 1;
	if _ItemIndex < 0 or _ItemIndex >= MAX_ITEMNUM then
		return;
	end
	
	if self:HasItem( _ItemIndex ) == false or nItemNum <= 0 then
		return;
	end
	
	self.m_Item[nItemIndex].m_num = self.m_Item[nItemIndex].m_num - nItemNum;
	if self.m_Item[nItemIndex].m_num <= 0 then
		self:SetItem( _ItemIndex, nil );
		return;
	end
	self:OnItemChange( _ItemIndex );
end

-- 对物品数量进行改变
function Item:ItemChangeNum( _ItemIndex, nNum )
	local nItemIndex = _ItemIndex + 1;
	if _ItemIndex < 0 or _ItemIndex >= MAX_ITEMNUM then
		return;
	end
	if nNum > 0 then
		self.m_Item[nItemIndex].m_num = nNum;
	else
		self.m_Item[nItemIndex]:empty();
	end
	self:OnItemChange( _ItemIndex );
end

-- 背包整理
function Item:ItemSettle( new_index, maxnum )
	local tmpItem = {};
	if maxnum > MAX_ITEMNUM then
		maxnum = MAX_ITEMNUM;
	end
	
	for tmpi=1, maxnum, 1 do
		local nItemIndex = new_index[tmpi]+1;
		table.insert( tmpItem, clone(self.m_Item[nItemIndex]) );
	end
	
	for tmpi=1, maxnum, 1 do
		self:SetItem( tmpi-1, nil );
		self:SetItem( tmpi-1, tmpItem[tmpi] );
	end
end

-- 获得道具详细信息
function Item:ItemInfo( pInfo, outInfo )
	
	local pItem = nil;
	local _ItemIndex = -1;
	local bNeedChange = false;
	
	-- 如果有输出结构，就使用输出结构
	if outInfo == nil then
		_ItemIndex = pInfo.m_itemoffset;
		pItem = self:GetAnyItem( _ItemIndex );
		-- 是否需要更新背包栏显示的信息
		bNeedChange = false;
	else
		pItem = outInfo;
	end
	
	if pItem == nil then
		return;
	end
	-- 获取服务器道具详细信息
	pItem.m_level 	= pInfo.m_level;
	pItem.m_type 		= pInfo.m_type;
	pItem.m_price		= pInfo.m_price;

	if outInfo == nil then
		-- 对数值有改变的进行处理
		if pItem.m_kind ~= pInfo.m_itemkind or pItem.m_color_level ~= pInfo.m_color_level then
			pItem.m_kind 		= pInfo.m_itemkind;
			pItem.m_color_level = pInfo.m_color_level;	
			bNeedChange = true;
		end	
	else
		pItem.m_kind 		= pInfo.m_itemkind;	
		pItem.m_color_level = pInfo.m_color_level;
	end
	
	
	-- 获取服务器道具动态信息
	local ability_num = 1;
	local baseability_num = 1;
	for tmpi=1, pInfo.m_attr_num, 1 do
		if pInfo.m_attr[tmpi].m_type == 0 or pInfo.m_attr[tmpi].m_type == 1 then			
			-- 基础属性
			pItem.m_ability[ability_num] 	= pInfo.m_attr[tmpi].m_ability;
			pItem.m_value[ability_num] 		= pInfo.m_attr[tmpi].m_value;
			pItem.m_valuebase[ability_num] 	= pInfo.m_attr[tmpi].m_addvalue;
			ability_num = ability_num + 1;
		end
	end
	
	-- 有必要更新背包栏显示信息
	if bNeedChange == true then
		--self:OnItemChange( _ItemIndex );
	end
end

-- 这个道具格子是否存在道具
function Item:HasItem( _ItemIndex )
	local nItemIndex = _ItemIndex + 1;
	if _ItemIndex < 0 or _ItemIndex >= MAX_ITEMNUM then
		return false;
	end
	if self.m_Item[nItemIndex].m_kind <= 0 or self.m_Item[nItemIndex].m_num <= 0 then
		return false;
	end

	return true;
end

-- 找到一个空格子
function Item:GetEmptyIndex()
	for tmpi=0, MAX_ITEMNUM-1, 1 do
		local pItem = self:GetAnyItem( tmpi );
		if pItem ~= nil and pItem.m_kind <= 0 and pItem.m_num <= 0 then
            return tmpi;
		end
	end
	return -1;
end

-- 根据索引获取道具信息
function Item:GetAnyItem( _ItemIndex )
	local nItemIndex = _ItemIndex + 1;
	local pItem = self.m_Item[nItemIndex];
	return pItem;
end

-- 根据索引获取道具kind
function Item:GetKind( _ItemIndex )
	local pItem = self:GetAnyItem( _ItemIndex );
	if pItem == nil then
		return 0;
	end
	return pItem.m_kind;
end

-- 根据索引获取道具Num
function Item:GetNum( _ItemIndex )
	local pItem = self:GetAnyItem( _ItemIndex );
	if pItem == nil then
		return 0;
	end
	return pItem.m_num;
end

-- 根据索引获取道具type
function Item:GetType( _ItemIndex )
	local pItem = self:GetAnyItem( _ItemIndex );
	if pItem == nil then
		return 0;
	end
	return pItem.m_type;
end

-- 根据索引获取道具颜色
function Item:GetColor( _ItemIndex )
	local pItem = self:GetAnyItem( _ItemIndex );
	if pItem == nil then
		return 0;
	end
	return pItem.m_color_level;
end

-- 根据索引获取道具level
function Item:GetLevel( _ItemIndex )
	local pItem = self:GetAnyItem( _ItemIndex );
	if pItem == nil then
		return 0;
	end
	return pItem.m_level;
end

-- 根据索引获取道具situation
function Item:GetSituation( _ItemIndex )
	local pItem = self:GetAnyItem( _ItemIndex );
	if pItem == nil then
		return 0;
	end
	return pItem.m_situation;
end

-- 根据索引获取道具名称
function Item:GetName( _ItemIndex )
	local pItem = self:GetAnyItem( _ItemIndex );
	if pItem == nil then
		return "";
	end
	return item_getname( pItem.m_kind );
end

-- 根据索引获取道具描述
function Item:GetDesc( _ItemIndex )
	local pItem = self:GetAnyItem( _ItemIndex );
	if pItem == nil then
		return "";
	end
	return item_getdesc( pItem.m_kind );
end

-- 根据索引获取道具资源
function Item:GetShape( _ItemIndex )
	local pItem = self:GetAnyItem( _ItemIndex );
	if pItem == nil then
		return -1;
	end
	return item_getres( pItem.m_kind );
end

-- 根据道具id获取个数
function Item:GetCount( nItemKind )
	local count = 0;
	for tmpi=0, MAX_ITEMNUM-1, 1 do
		local pItem = self:GetAnyItem( tmpi );
		if pItem ~= nil and pItem.m_kind == nItemKind then
			count = count + pItem.m_num;
		end
	end
	return count;
end

-- 根据道具id获得背包内索引
function Item:GetIndex( nItemKind )
	for tmpi=0, MAX_ITEMNUM-1, 1 do
		local pItem = self:GetAnyItem( tmpi );
		if pItem ~= nil and pItem.m_kind == nItemKind then
			return tmpi;
		end
	end
	return -1;
end

-- 根据道具type获取个数
function Item:GetCountWithType( nItemType )
	local count = 0;
	for tmpi=0, MAX_ITEMNUM-1, 1 do
		local pItem = self:GetAnyItem( tmpi );
		if pItem ~= nil and pItem.m_type == nItemType then
			count = count + pItem.m_num;
		end
	end
	return count;
end

-- 根据道具类型获取所有该类型的道具集合(table)
function Item:GetItemsByType( itemtype )
	-- m_itemindex, m_itemkind, m_itemcount
	local items = {};
	for tmpi=0, MAX_ITEMNUM-1, 1 do
		local pItem = self:GetAnyItem( tmpi );
		if pItem ~= nil and pItem.m_type == itemtype then
			table.insert( items, {
				m_itemindex = tmpi,
				m_itemkind = pItem.m_kind,
				m_itemcount = pItem.m_num,
                m_color_level = pItem.m_color_level;
			} );
		end
	end
	return items;
end


-- 根据道具类型 和 最小的颜色等级获取所有该类型的道具集合(table)
function Item:GetItemsByTypeWithMinColor( itemtype , mincolor , maxcolor )
	-- m_itemindex, m_itemkind, m_itemcount
	local items = {};
	for tmpi=0, MAX_ITEMNUM-1, 1 do
		local pItem = self:GetAnyItem( tmpi );
		if pItem ~= nil and pItem.m_type == itemtype and pItem.m_color_level >= mincolor and pItem.m_color_level <= maxcolor then
			table.insert( items, {
				m_itemindex = tmpi,
				m_itemkind = pItem.m_kind,
				m_itemcount = pItem.m_num,
                m_color_level = pItem.m_color_level;
			} );
		end
	end
	return items;
end

-- 根据道具类型获取所有该类型的指定颜色的道具集合(table)
function Item:GetIndexesByKindWithColor( kind, color )
	-- m_itemindex, m_itemkind, m_itemcount
	local items = {};
	for tmpi=0, MAX_ITEMNUM-1, 1 do
		local pItem = self:GetAnyItem( tmpi );
		if pItem ~= nil and pItem.m_kind == kind and pItem.m_color_level == color then
			table.insert( items, tmpi );
		end
	end
	return items;
end

-- 根据道具kind区间获取所有的道具集合(table)
function Item:GetItemsByAreaKind( minkind, maxkind )
	-- m_itemindex, m_itemkind, m_itemcount
	local items = {};
	for tmpi=0, MAX_ITEMNUM-1, 1 do
		local pItem = self:GetAnyItem( tmpi );
		if pItem ~= nil and pItem.m_kind >= minkind and pItem.m_kind <= maxkind then
			table.insert( items, {
				m_itemindex = tmpi,
				m_itemkind = pItem.m_kind,
				m_itemcount = pItem.m_num,
                m_color_level = pItem.m_color_level,
			} );
		end
	end
	return items;
end

-- 根据道具类别区间获取所有道具
function Item:GetItemsByAreaType( mintype, maxtype )
  local items = {};
	for tmpi=0, MAX_ITEMNUM- 1, 1 do
		local pItem = self:GetAnyItem( tmpi );
		if pItem ~= nil and pItem.m_type >= mintype and pItem.m_type <= maxtype then
			table.insert( items, {
				m_itemindex = tmpi,
				m_itemkind = pItem.m_kind,
				m_itemcount = pItem.m_num,
                m_color_level = pItem.m_color_level,
                m_type = pItem.m_type,
			} );
		end
	end
	return items;
end

-- 清空所有的New标示
function Item:ClearAllNew()
	for tmpi=0, MAX_ITEMNUM-1, 1 do
		local pItem = self:GetAnyItem( tmpi );
		if pItem ~= nil then
			pItem.m_bIsNew = false;
		end
	end
end

-- 清空New标示
function Item:ClearNew( _ItemIndex )
	local pItem = self:GetAnyItem( _ItemIndex );
	if pItem ~= nil then
		pItem.m_bIsNew = false;
	end
end

-- 使用某个kind的道具
function Item:Use( nItemKind )
	self:UseWithCount( nItemKind , 1);
end

-- 使用多个某个kind的道具
function Item:UseWithCount( nItemKind , count)
    if nItemKind <= 0 then
		return -1;
	end
	local nItemIndex = self:GetIndex( nItemKind );
	if nItemKind < 0 then
		--Notify( 55 )
		return -1;
	end
	
	local pItem = GetItem():GetAnyItem( nItemIndex );
	if pItem == nil then
		return;
	end
	if pItem.m_level < 0 then
		-- 主城等级
		if GetCity().BuildingList[1].m_level < -pItem.m_level then
			--Notify( 59, { -pItem.m_level } )
			return;
		end
	elseif pItem.m_level > 0 then
		-- 角色等级
		if GetPlayer().m_level < pItem.m_level then
			--Notify( 60, { pItem.m_level } )
			return;
		end
	end
	system_askinfo( ASKINFO_STORAGE, "", ITEM_PROCESS_USE, nItemIndex, count, -1, -1, 1 );
end

-- 使用某个kind的道具通过钻石
function Item:UseWithToken( nItemKind, nNum , isCheckToken )
	if nItemKind <= 0 then
		return -1;
	end

	if ( isCheckToken == nil or isCheckToken == true ) and GetPlayer().m_token < item_getprice( nItemKind ) * nNum then
        -- 跳转打开充值界面
        --Jump_Token();
		return -1;
	end
	system_askinfo( ASKINFO_STORAGE, "", ITEM_PROCESS_TOKENUSE, nItemKind, nNum, -1, -1, 1 );
end

-- 全局
local G_Item = nil;
function GetItem()
	if G_Item == nil then
		G_Item = Item.new();
	end
	return G_Item;
end
