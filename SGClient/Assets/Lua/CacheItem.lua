-- 道具缓存信息
-- 函数参数名称带有"_"的都是服务器真实的offset，0开始

MAX_ITEMNUM		= 300 -- 道具数量

ITEM_BASEABILITY_NUM	= 2  -- 道具最多有2个基础属性
ITEM_ABILITY_NUM		= 4  -- 道具最多有4个可变属性

ITEM_COLOR_LEVEL_WHITE	= 0 -- 白色
ITEM_COLOR_LEVEL_BLUE	= 1 -- 蓝色
ITEM_COLOR_LEVEL_GREEN	= 2 -- 绿色
ITEM_COLOR_LEVEL_GOLD	= 3 -- 金色
ITEM_COLOR_LEVEL_RED	= 4 -- 红色
ITEM_COLOR_LEVEL_PURPLE	= 5 -- 紫色
ITEM_COLOR_LEVEL_ORANGE = 6 -- 橙色

ITEM_PROCESS_INFO		    = 1 -- 获取道具信息
ITEM_PROCESS_USE		    = 2 -- 使用道具
ITEM_PROCESS_SELL		    = 3 -- 丢弃道具
ITEM_PROCESS_TOKENUSE	    = 4	-- 钻石方式使用

-- 道具类型
ITEM_TYPE_NORMAL_USE			=	1	--	点击类道具（主动使用）
ITEM_TYPE_SCRIPT_USE			=	2	--	脚本使用类道具
ITEM_TYPE_EQUIP_MATERIAL		=	3	-- 装备材料
ITEM_TYPE_EQUIP_DRAWING			=	4	-- 装备图纸
ITEM_TYPE_ACTIVITY				=	5	-- 活动类（无点击按钮，被动使用）
ITEM_TYPE_AUTO					=	6	-- 自动使用类
ITEM_TYPE_HEROEXP				=	7	-- 武将经验

-- 动态属性
ITEM_ABILITY_NONE				=	0
ITEM_ABILITY_AWARDGROUP			=	1	-- 掉落包
ITEM_ABILITY_SCRIPTID			=	2	-- 脚本调用ID
ITEM_ABILITY_ADDBODY			=	3	-- 加体力
ITEM_ABILITY_ADDEXP				=	4	-- 加主公经验
ITEM_ABILITY_ADDHEROEXP			=	5	-- 加英雄经验
ITEM_ABILITY_CITYRES_SILVER		=	11	-- 银币数量
ITEM_ABILITY_CITYRES_WOOD		=	12	-- 木材数量
ITEM_ABILITY_CITYRES_FOOD		=	13	-- 粮食数量
ITEM_ABILITY_CITYRES_IRON		=	14	-- 铁数量
ITEM_ABILITY_BUFF				=	15	-- buff

-- 获取道具配置信息
function item_getinfo( itemkind )
	local iteminfo = g_itemkind[itemkind];
	if iteminfo == nil then
		return nil;
	end
	if itemkind <= 0 then
		return nil;
	end
	return g_itemkind[itemkind];
end

-- 获取道具配置名称
function item_getname( itemkind )
	local name = Localization.text_item( itemkind );
	return name;
end

-- 获取道具配置描述
function item_getdesc( itemkind )
	local desc = Localization.text_item( itemkind+5000 );
	if desc == "" then
		return "";
	end
	return desc;
end

-- 获取道具等级
function item_getlevel( itemkind )
  if g_itemkind[itemkind] == nil then
     return 0;
  end
	local level = g_itemkind[itemkind].level;
	if level == nil or level < 0 then
		return 0;
	end
	return level;
end

-- 获取道具配置颜色
function item_getcolor( itemkind )
	if g_itemkind[itemkind] == nil then
		return 0;
	end
	local color = g_itemkind[itemkind].color_level;
	if color == nil or color < 0 then
		return 0;
	end
	return color;
end

-- 获取道具配置顺序
function item_getsort( itemkind )
	local sort = g_itemkind[itemkind].sort;
	if sort == nil or sort < 0 then
		return 0;
	end
	return sort;
end

-- 获取道具售价
function item_getprice( itemkind )
  if g_itemkind[itemkind] == nil then
    return 0;
  end
	local price = g_itemkind[itemkind].price;
	if price == nil then
		return 0;
	end
	return price;
end

-- 获取道具钻石使用价格
function item_gettoken( itemkind )
  if g_itemkind[itemkind] == nil then
    return 0;
  end
	local token = g_itemkind[itemkind].token;
	if token == nil then
		return 0;
	end
	return token;
end

-- 获取道具类型
function item_gettype( itemkind )
	if g_itemkind[itemkind] == nil then 
		return -1;
	end
	local type = g_itemkind[itemkind].type;
	if type == 0 then
		return 0;
	end
	return type;
end

-- 获取道具属性描述
function item_getabilitydesc( ability )
	local abilitydesc = Localization.text_item( ability+10000 );
	if abilitydesc == "" then
		return "";
	end
	return abilitydesc;
end

-- 根据类别获取道具
function item_getlist_withtype( type )
    local refTable = {};
    for k, v in pairs(g_itemkind) do
        if v.type == type then
			table.insert( refTable, v )
        end
    end
    return refTable
end

-- 一个道具结构
SLK_Item = class("SLK_Item")
function SLK_Item:ctor()
	self:empty();
end

function SLK_Item:empty()
	
	-- 固定配置信息
	self.m_kind  			= 0;-- 物品种类 决定了物品的名字
	self.m_level 			= 0;-- 等级
	self.m_type  			= 0;-- 物品类别
	self.m_situation		= 0;-- 物品使用状态
	
	-- 动态基础信息
	self.m_id  				= 0;-- 物品在这个服务器的唯一id
	self.m_num 				= 0;-- 物品个数
	self.m_color_level 		= 0;-- 颜色级别
	
	
	-- 属性信息
	self.m_ability   		= { 0,0,0,0,0,0 };
	self.m_value     		= { 0,0,0,0,0,0 };
	self.m_valuebase 		= { 0,0,0,0,0,0 };
	
	self.m_bIsUpdate = false; -- 是否已经更新
	self.m_bIsNew = false;
	return self;
end


-- 道具部分客户端缓存
local Item = class("Item")
function Item:ctor()
	self:ResetAll();
end

-- 清空缓存
function Item:ResetAll()
	
	-- 背包栏
	self.m_Item = {};
	for i=1,MAX_ITEMNUM,1 do
		local object = SLK_Item.new();
		table.insert( self.m_Item, object );
	end
	
end

-- 设置背包道具
function Item:SetItem( _ItemIndex, pItem )
	local nItemIndex = _ItemIndex + 1;
	if pItem ~= nil then
		self.m_Item[nItemIndex] = clone(pItem); -- 这个地方是否要用到深拷贝

	else
		self.m_Item[nItemIndex]:empty(); -- 清空这个道具
	end
	self:OnItemChange( _ItemIndex );
end

-- 改变道具信息
-- EventProtocol.addEventListener( "OnItemChange", function( recvValue ) end )
function Item:OnItemChange( _ItemIndex )
	local nItemIndex = _ItemIndex + 1;
	BagDlgLoadItem();
	-- 发布事件
	--EventProtocol.dispatchEvent( "OnItemChange", { itemIndex=_ItemIndex, kind=self.m_Item[nItemIndex].m_kind, num=self.m_Item[nItemIndex].m_num } );
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
		
	-- 道具使用没了
	if self.m_Item[nItemIndex].m_num <= 0 then
		self:SetItem( _ItemIndex, nil );
		self:OnItemChange( _ItemIndex );
	else
		BagDlgItemChange( _ItemIndex )
	end
		
end

-- 得到服务器返回的消息，得到一个物品
function Item:ItemGet( _ItemIndex, kind, type, num, color, situation, path )
	local nItemIndex = _ItemIndex + 1;
	if kind <= 0 or num <= 0 then
		return;
	end
	
	-- 得到物品
	local new = false;
	local pitem = self.m_Item[nItemIndex];
	if pitem.m_kind ~= kind or pitem.m_num <= 0 then
		pitem.m_bIsUpdate = false;
		new = true;
	end
	pitem.m_num = pitem.m_num + num;
	pitem.m_kind = kind;
	pitem.m_type = type;
	pitem.m_color_level = color;
	pitem.m_situation = situation;
	self:SetItem( _ItemIndex, pitem );

	self:OnGetItem( _ItemIndex, kind, num, new, path );
end

-- 对得到的这个物品进行其它处理，比如提示等
function Item:OnGetItem( _ItemIndex, nItemKind, num, new, path )
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
	
	if new == true then
		self:OnItemChange( _ItemIndex );
	else
		BagDlgItemChange( _ItemIndex )
	end
	
	--if path == PATH_GM or path == PATH_SYSTEM then
     -- 忽略
     --return;
	--end
	
	pop( F( 690, item_getname(nItemKind), num ) )
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
			--return;
		end
	elseif pItem.m_level > 0 then
		-- 角色等级
		if GetPlayer().m_level < pItem.m_level then
			--Notify( 60, { pItem.m_level } )
			--return;
		end
	end
	system_askinfo( ASKINFO_ITEM, "", ITEM_PROCESS_USE, nItemIndex, count, -1, 1 );
end

-- 使用某个kind的道具通过钻石
function Item:UseWithToken( nItemKind, nNum , isCheckToken )
	if nItemKind <= 0 then
		return -1;
	end

	if ( isCheckToken == nil or isCheckToken == true ) and GetPlayer().m_token < item_gettoken( nItemKind ) * nNum then
        -- 跳转打开充值界面
        --Jump_Token();
		return -1;
	end
	system_askinfo( ASKINFO_ITEM, "", ITEM_PROCESS_TOKENUSE, nItemKind, nNum, -1, 1 );
end

-- 全局
local G_Item = nil;
function GetItem()
	if G_Item == nil then
		G_Item = Item.new();
	end
	return G_Item;
end
