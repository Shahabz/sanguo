-- 装备缓存信息
-- 函数参数名称带有"_"的都是服务器真实的offset，0开始
MAX_EQUIPNUM	= 150  -- 装备数量
EQUIP_ABILITY_NUM		= 4  -- 道具最多有4个可变属性

EQUIP_PROCESS_EQUIP		    = 1 -- 装备穿上
EQUIP_PROCESS_UNEQUIP	    = 2 -- 装备卸下

-- 装备类型
EQUIP_TYPE_EQUIP1					=	1	--	武器
EQUIP_TYPE_EQUIP2					=	2	--	坐骑
EQUIP_TYPE_EQUIP3					=	3	--	盔甲
EQUIP_TYPE_EQUIP4					=	4	--	头盔
EQUIP_TYPE_EQUIP5					=	5	--	将印
EQUIP_TYPE_EQUIP6					=	6	--	兵符

-- 动态属性
EQUIP_ABILITY_NONE				=	0
EQUIP_ABILITY_A					=	1	-- 攻击
EQUIP_ABILITY_D					=	2	-- 防御
EQUIP_ABILITY_S					=	3 	-- 兵力

-- 获取装备配置信息
function equip_getinfo( equipkind )
	local equipinfo = Data.equip[equipkind];
	if equipinfo == nil then
		return nil;
	end
	if equipkind <= 0 then
		return nil;
	end
	return Data.equip[equipkind];
end

-- 获取装备配置名称
function equip_getname( equipkind )
	local name = Localization.text_equip( equipkind );
	if name == "" then
		-- 这里注意，equipkind>100000可以返回奖励名称
		if G_AwardKindList[equipkind] == nil then
			return "";
		end
		return GetLocalizeText(G_AwardKindList[equipkind].nameid)
	end
	return name;
end

-- 获取装备配置描述
function equip_getdesc( equipkind )
	local desc = Localization.text_equip( equipkind+3000 );
	if desc == "" then
		return "";
	end
	return desc;
end

-- 获取装备等级
function equip_getlevel( equipkind )
  if Data.equip[equipkind] == nil then
     return 0;
  end
	local level = Data.equip[equipkind]["level"];
	if level == nil or level < 0 then
		return 0;
	end
	return level;
end

-- 获取装备配置颜色
function equip_getcolor( equipkind )
  if Data.equip[equipkind] == nil then
    return 0;
  end
	local color = Data.equip[equipkind]["color_level"];
	if color == nil or color < 0 then
		return 0;
	end
	return color;
end

-- 获取装备配置顺序
function equip_getsort( equipkind )
	local sort = Data.equip[equipkind]["sort"];
	if sort == nil or sort < 0 then
		return 0;
	end
	return sort;
end

-- 获取装备价值
function equip_getprice( equipkind )
    --if Data.equip[equipkind] == nil then
        --return 0;
    --end
	local price = Data.equip[equipkind].price;
	if price == nil then
		return 0;
	end
	return price;
end

-- 获取装备配置过滤
function equip_getfilter( equipkind )
    --if Data.equip[equipkind] == nil then
        --return 1;
    --end
	local filter = Data.equip[equipkind].filter;
	if filter == nil or filter == 0 then
		return 1;
	end
	return filter;
end

-- 获取装备属性描述
function equip_gettype( equipkind )
    if Data.equip[equipkind] == nil then 
        return -1;
    end
	local type = Data.equip[equipkind].type;
	if type == 0 then
		return 0;
	end
	return type;
end

-- 获取装备属性描述
function equip_getabilitydesc( ability )
	local abilitydesc = Localization.text_equip( ability+10000 );
	if abilitydesc == "" then
		return "";
	end
	return abilitydesc;
end

-- 根据类别获取装备
function equip_getlist_withtype( type )
    local refTable = {};
    for k, v in pairs(Data.equip) do
        if v.type == type then
			table.insert( refTable, v )
        end
    end
    return refTable
end

-- 一个装备结构
SLK_Equip = class("SLK_Equip")
function SLK_Equip:ctor()
	self:empty();
end

function SLK_Equip:empty()
	
	-- 固定配置信息
	self.m_kind  			= 0;-- 物品种类 决定了物品的名字
	self.m_level 			= 0;-- 等级
	self.m_type  			= 0;-- 装备类别
		
	-- 属性信息
	self.m_ability   		= { 0,0,0,0,0,0 };
	self.m_value     		= { 0,0,0,0,0,0 };
	self.m_valuebase 		= { 0,0,0,0,0,0 };
	
	self.m_bIsUpdate = false; -- 是否已经更新
	self.m_bIsNew = false;
	return self;
end

-- 装备部分客户端缓存
local Equip = class("Equip")
function Equip:ctor()
	self:ResetAll();
end

-- 清空缓存
function Equip:ResetAll()
	-- 装备栏
	self.m_Equip = {};
	for i=1,MAX_EQUIPNUM,1 do
		local object = SLK_Equip.new();
		table.insert( self.m_Equip, object );
	end
end

-- 设置背包装备
function Equip:SetEquip( _EquipIndex, pEquip )
	local nEquipIndex = _EquipIndex + 1;
	if pEquip ~= nil then
		self.m_Equip[nEquipIndex] = clone(pEquip); -- 这个地方是否要用到深拷贝
	else
		self.m_Equip[nEquipIndex]:empty(); -- 清空这个装备
	end
	self:OnEquipChange( _EquipIndex );
end

-- 改变装备信息
function Equip:OnEquipChange( _EquipIndex )
	local nEquipIndex = _EquipIndex + 1;
		
end

-- 得到服务器返回的消息，得到一个物品
function Equip:EquipGet( _EquipIndex, kind, type, num, color, situation, path )
	local nEquipIndex = _EquipIndex + 1;
	if kind <= 0 or num <= 0 then
		return;
	end
		
		-- 得到普通物品
		local pequip = self.m_Equip[nEquipIndex];
		if pequip.m_kind ~= kind or pequip.m_num <= 0 then
			pequip.m_bIsUpdate = false;
		end
		pequip.m_num = pequip.m_num + num;
		pequip.m_kind = kind;
		pequip.m_type = type;
		pequip.m_color_level = color;
		pequip.m_situation = situation;
		self:SetEquip( _EquipIndex, pequip );
	
	self:OnGetEquip( _EquipIndex, kind, num, path );
end

-- 对得到的这个物品进行其它处理，比如提示等
function Equip:OnGetEquip( _EquipIndex, nEquipKind, num, path )
	local nEquipIndex = _EquipIndex + 1;
	local pEquip = self:GetAnyEquip( _EquipIndex );
	if pEquip == nil then
		return;
	end
	-- 新装备标示
	pEquip.m_bIsNew = true;

end

-- 得到服务器返回的消息，失去装备
function Equip:EquipLost( _EquipIndex, path )
	local nEquipIndex = _EquipIndex + 1;
	if _EquipIndex < 0 or _EquipIndex >= MAX_EQUIPNUM then
		return;
	end
	
	if self:HasEquip( _EquipIndex ) == false then
		return;
	end
	

	self:SetEquip( _EquipIndex, nil );
	self:OnEquipChange( _EquipIndex );
end

-- 背包整理
function Equip:EquipSettle( new_index, maxnum )
	local tmpEquip = {};
	if maxnum > MAX_EQUIPNUM then
		maxnum = MAX_EQUIPNUM;
	end
	
	for tmpi=1, maxnum, 1 do
		local nEquipIndex = new_index[tmpi]+1;
		table.insert( tmpEquip, clone(self.m_Equip[nEquipIndex]) );
	end
	
	for tmpi=1, maxnum, 1 do
		self:SetEquip( tmpi-1, nil );
		self:SetEquip( tmpi-1, tmpEquip[tmpi] );
	end
end

-- 获得装备详细信息
function Equip:EquipInfo( pInfo, outInfo )
	
	local pEquip = nil;
	local _EquipIndex = -1;
	local bNeedChange = false;
	
	-- 如果有输出结构，就使用输出结构
	if outInfo == nil then
		_EquipIndex = pInfo.m_equipoffset;
		pEquip = self:GetAnyEquip( _EquipIndex );
		-- 是否需要更新背包栏显示的信息
		bNeedChange = false;
	else
		pEquip = outInfo;
	end
	
	if pEquip == nil then
		return;
	end
	-- 获取服务器装备详细信息
	pEquip.m_level 		= pInfo.m_level;
	pEquip.m_type 		= pInfo.m_type;
	pEquip.m_price		= pInfo.m_price;

	if outInfo == nil then
		-- 对数值有改变的进行处理
		if  pEquip.m_kind ~= pInfo.m_equipkind or pEquip.m_color_level ~= pInfo.m_color_level then
			pEquip.m_kind 		= pInfo.m_equipkind;
			pEquip.m_color_level = pInfo.m_color_level;				
			bNeedChange = true;
		end	
	else
		pEquip.m_kind 		= pInfo.m_equipkind;	
		pEquip.m_color_level = pInfo.m_color_level;
	end
	
	
	-- 获取服务器装备动态信息
	local ability_num = 1;
	local baseability_num = 1;
	for tmpi=1, pInfo.m_attr_num, 1 do
		if pInfo.m_attr[tmpi].m_type == 0 or pInfo.m_attr[tmpi].m_type == 1 then			
			-- 基础属性
			pEquip.m_ability[ability_num] 	= pInfo.m_attr[tmpi].m_ability;
			pEquip.m_value[ability_num] 		= pInfo.m_attr[tmpi].m_value;
			pEquip.m_valuebase[ability_num] 	= pInfo.m_attr[tmpi].m_addvalue;
			ability_num = ability_num + 1;
		end
	end
	
	-- 有必要更新背包栏显示信息
	if bNeedChange == true then
		--self:OnEquipChange( _EquipIndex );
	end
end

-- 这个装备格子是否存在装备
function Equip:HasEquip( _EquipIndex )
	local nEquipIndex = _EquipIndex + 1;
	if _EquipIndex < 0 or _EquipIndex >= MAX_EQUIPNUM then
		return false;
	end
	if self.m_Equip[nEquipIndex].m_kind <= 0 or self.m_Equip[nEquipIndex].m_num <= 0 then
		return false;
	end

	return true;
end

-- 找到一个空格子
function Equip:GetEmptyIndex()
	for tmpi=0, MAX_EQUIPNUM-1, 1 do
		local pEquip = self:GetAnyEquip( tmpi );
		if pEquip ~= nil and pEquip.m_kind <= 0 and pEquip.m_num <= 0 then
            return tmpi;
		end
	end
	return -1;
end

-- 根据索引获取装备信息
function Equip:GetAnyEquip( _EquipIndex )
	local nEquipIndex = _EquipIndex + 1;
	local pEquip = self.m_Equip[nEquipIndex];
	return pEquip;
end

-- 根据索引获取装备kind
function Equip:GetKind( _EquipIndex )
	local pEquip = self:GetAnyEquip( _EquipIndex );
	if pEquip == nil then
		return 0;
	end
	return pEquip.m_kind;
end

-- 根据索引获取装备type
function Equip:GetType( _EquipIndex )
	local pEquip = self:GetAnyEquip( _EquipIndex );
	if pEquip == nil then
		return 0;
	end
	return pEquip.m_type;
end

-- 根据索引获取装备颜色
function Equip:GetColor( _EquipIndex )
	local pEquip = self:GetAnyEquip( _EquipIndex );
	if pEquip == nil then
		return 0;
	end
	return pEquip.m_color_level;
end

-- 根据索引获取装备level
function Equip:GetLevel( _EquipIndex )
	local pEquip = self:GetAnyEquip( _EquipIndex );
	if pEquip == nil then
		return 0;
	end
	return pEquip.m_level;
end

-- 根据索引获取装备名称
function Equip:GetName( _EquipIndex )
	local pEquip = self:GetAnyEquip( _EquipIndex );
	if pEquip == nil then
		return "";
	end
	return equip_getname( pEquip.m_kind );
end

-- 根据索引获取装备描述
function Equip:GetDesc( _EquipIndex )
	local pEquip = self:GetAnyEquip( _EquipIndex );
	if pEquip == nil then
		return "";
	end
	return equip_getdesc( pEquip.m_kind );
end

-- 根据索引获取装备资源
function Equip:GetShape( _EquipIndex )
	local pEquip = self:GetAnyEquip( _EquipIndex );
	if pEquip == nil then
		return -1;
	end
	return equip_getres( pEquip.m_kind );
end

-- 根据装备id获得背包内索引
function Equip:GetIndex( nEquipKind )
	for tmpi=0, MAX_EQUIPNUM-1, 1 do
		local pEquip = self:GetAnyEquip( tmpi );
		if pEquip ~= nil and pEquip.m_kind == nEquipKind then
			return tmpi;
		end
	end
	return -1;
end

-- 根据装备类型获取所有该类型的装备集合(table)
function Equip:GetEquipsByType( equiptype )
	-- m_equipindex, m_equipkind, m_equipcount
	local equips = {};
	for tmpi=0, MAX_EQUIPNUM-1, 1 do
		local pEquip = self:GetAnyEquip( tmpi );
		if pEquip ~= nil and pEquip.m_type == equiptype then
			table.insert( equips, {
				m_equipindex = tmpi,
				m_equipkind = pEquip.m_kind,
				m_equipcount = pEquip.m_num,
                m_color_level = pEquip.m_color_level;
			} );
		end
	end
	return equips;
end

-- 根据装备类型 和 最小的颜色等级获取所有该类型的装备集合(table)
function Equip:GetEquipsByTypeWithMinColor( equiptype , mincolor , maxcolor )
	-- m_equipindex, m_equipkind, m_equipcount
	local equips = {};
	for tmpi=0, MAX_EQUIPNUM-1, 1 do
		local pEquip = self:GetAnyEquip( tmpi );
		if pEquip ~= nil and pEquip.m_type == equiptype and pEquip.m_color_level >= mincolor and pEquip.m_color_level <= maxcolor then
			table.insert( equips, {
				m_equipindex = tmpi,
				m_equipkind = pEquip.m_kind,
				m_equipcount = pEquip.m_num,
                m_color_level = pEquip.m_color_level;
			} );
		end
	end
	return equips;
end

-- 根据装备kind区间获取所有的装备集合(table)
function Equip:GetEquipsByAreaKind( minkind, maxkind )
	-- m_equipindex, m_equipkind, m_equipcount
	local equips = {};
	for tmpi=0, MAX_EQUIPNUM-1, 1 do
		local pEquip = self:GetAnyEquip( tmpi );
		if pEquip ~= nil and pEquip.m_kind >= minkind and pEquip.m_kind <= maxkind then
			table.insert( equips, {
				m_equipindex = tmpi,
				m_equipkind = pEquip.m_kind,
				m_equipcount = pEquip.m_num,
                m_color_level = pEquip.m_color_level,
			} );
		end
	end
	return equips;
end

-- 清空所有的New标示
function Equip:ClearAllNew()
	for tmpi=0, MAX_EQUIPNUM-1, 1 do
		local pEquip = self:GetAnyEquip( tmpi );
		if pEquip ~= nil then
			pEquip.m_bIsNew = false;
		end
	end
end

-- 清空New标示
function Equip:ClearNew( _EquipIndex )
	local pEquip = self:GetAnyEquip( _EquipIndex );
	if pEquip ~= nil then
		pEquip.m_bIsNew = false;
	end
end

-- 全局
local G_Equip = nil;
function GetEquip()
	if G_Equip == nil then
		G_Equip = Equip.new();
	end
	return G_Equip;
end
