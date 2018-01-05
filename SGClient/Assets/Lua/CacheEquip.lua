-- 装备缓存信息
-- 函数参数名称带有"_"的都是服务器真实的offset，0开始
MAX_DEFAULT_EQUIPNUM	= 30   -- 缺省装备数量
MAX_EQUIPNUM			= 150  -- 装备最大数量
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
EQUIP_ABILITY_ATTACK			=	1	-- 攻击
EQUIP_ABILITY_DEFENSE			=	2	-- 防御
EQUIP_ABILITY_TROOPS			=	3 	-- 兵力
EQUIP_ABILITY_ATTACK_INCREASE	=	4	-- 强攻
EQUIP_ABILITY_DEFENSE_INCREASE	=	5	-- 强防
EQUIP_ABILITY_ASSAULT			=	6	-- 攻城
EQUIP_ABILITY_DEFEND			=	7	-- 守城

-- 获取装备配置信息
function equip_getinfo( equipkind )
	local equipinfo = g_equipinfo[equipkind];
	if equipinfo == nil then
		return nil;
	end
	if equipkind <= 0 then
		return nil;
	end
	return g_equipinfo[equipkind];
end

-- 获取装备配置名称
function equip_getname( equipkind )
	local name = Localization.text_item( equipkind + 2000 );
	return name;
end

-- 获取装备配置类型
function equip_gettype( equipkind )
	if g_equipinfo[equipkind] == nil then
		return 0;
	end
	local type = g_equipinfo[equipkind]["type"];
	return type;
end

-- 获取装备配置颜色
function equip_getcolor( equipkind )
	if g_equipinfo[equipkind] == nil then
		return 0;
	end
	local color = g_equipinfo[equipkind]["color"];
	if color == nil or color < 0 then
		return 0;
	end
	return color;
end

-- 获取分解威望
function equip_getprestige( equipkind )
	if g_equipinfo[equipkind] == nil then
		return 0;
	end
	local prestige = g_equipinfo[equipkind]["prestige"];
	if prestige == nil or prestige < 0 then
		return 0;
	end
	return prestige;
end

-- 装备基础属性
function equip_getabilityname( equipkind )
	if g_equipinfo[equipkind] == nil then
		return "";
	end
	local ability = g_equipinfo[equipkind]["ability"];
	local value = g_equipinfo[equipkind]["value"];
	if ability == EQUIP_ABILITY_ATTACK then
		name = T(146).."+"..value;
	elseif ability == EQUIP_ABILITY_DEFENSE then
		name = T(147).."+"..value;
	elseif ability == EQUIP_ABILITY_TROOPS then
		name = T(148).."+"..value;
	elseif ability == EQUIP_ABILITY_ATTACK_INCREASE then
		name = T(165).."+"..value;
	elseif ability == EQUIP_ABILITY_DEFENSE_INCREASE then
		name = T(166).."+"..value;
	elseif ability == EQUIP_ABILITY_ASSAULT then
		name = T(167).."+"..value;
	elseif ability == EQUIP_ABILITY_DEFEND then
		name = T(168).."+"..value;
	end
	return name;
end

-- 装备基础属性
function equip_getwashname( ability )
	local name = ""
	if ability == EQUIP_ABILITY_ATTACK then
		name = T(146)
	elseif ability == EQUIP_ABILITY_DEFENSE then
		name = T(147)
	elseif ability == EQUIP_ABILITY_TROOPS then
		name = T(148)
	elseif ability == EQUIP_ABILITY_ATTACK_INCREASE then
		name = T(165)
	elseif ability == EQUIP_ABILITY_DEFENSE_INCREASE then
		name = T(166)
	elseif ability == EQUIP_ABILITY_ASSAULT then
		name = T(167)
	elseif ability == EQUIP_ABILITY_DEFEND then
		name = T(168)
	end
	return name;
end

-- 根据类别获取装备
function equip_getlist_withtype( type )
    local refTable = {};
    for k, v in pairs(g_equipinfo) do
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
	self.m_offset			= -1;
		
	-- 洗炼信息
	self.m_washid   		= { 0,0,0,0 };
	
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
	BagDlgLoadEquip();	
end

-- 得到服务器返回的消息，得到一个装备
function Equip:EquipGet( _EquipIndex, kind, path )
	local nEquipIndex = _EquipIndex + 1;
	if kind <= 0 then
		return;
	end
		
	-- 得到普通物品
	local pequip = self.m_Equip[nEquipIndex];
	if pequip.m_kind ~= kind then
		pequip.m_bIsUpdate = false;
	end
	pequip.m_kind = kind;
	pequip.m_offset = _EquipIndex;
	self:SetEquip( _EquipIndex, pequip );
	self:OnGetEquip( _EquipIndex, kind, path );
end

-- 对得到的这个物品进行其它处理，比如提示等
function Equip:OnGetEquip( _EquipIndex, nEquipKind, path )
	local nEquipIndex = _EquipIndex + 1;
	local pEquip = self:GetAnyEquip( _EquipIndex );
	if pEquip == nil then
		return;
	end
	
	if path == PATH_EQUIP_DOWN then
		return
	end
	
	-- 新装备标示
	pEquip.m_bIsNew = true;
	
	pop( T(120)..": "..equip_getname(nEquipKind) )
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
end

-- 这个装备格子是否存在装备
function Equip:HasEquip( _EquipIndex )
	local nEquipIndex = _EquipIndex + 1;
	if _EquipIndex < 0 or _EquipIndex >= MAX_EQUIPNUM then
		return false;
	end
	if self.m_Equip[nEquipIndex].m_kind <= 0 then
		return false;
	end

	return true;
end

-- 找到一个空格子
function Equip:GetEmptyIndex()
	for tmpi=0, MAX_EQUIPNUM-1, 1 do
		local pEquip = self:GetAnyEquip( tmpi );
		if pEquip ~= nil and pEquip.m_kind <= 0 then
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
	return equip_gettype( pEquip.m_kind );
end

-- 根据索引获取装备颜色
function Equip:GetColor( _EquipIndex )
	local pEquip = self:GetAnyEquip( _EquipIndex );
	if pEquip == nil then
		return 0;
	end
	return pEquip.m_color_level;
end

-- 根据索引获取装备名称
function Equip:GetName( _EquipIndex )
	local pEquip = self:GetAnyEquip( _EquipIndex );
	if pEquip == nil then
		return "";
	end
	return equip_getname( pEquip.m_kind );
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
	local equips = {};
	for tmpi=0, MAX_EQUIPNUM-1, 1 do
		local pEquip = self:GetAnyEquip( tmpi );
		if pEquip ~= nil and equip_gettype(pEquip.m_kind) == equiptype then
			table.insert( equips, {
				m_offset = tmpi,
				m_kind = pEquip.m_kind,
			} );
		end
	end
	return equips;
end

-- 根据装备类型获取所有该类型的装备集合(table)
function Equip:HasByType( equiptype )
	for tmpi=0, MAX_EQUIPNUM-1, 1 do
		local pEquip = self:GetAnyEquip( tmpi );
		if pEquip ~= nil and equip_gettype(pEquip.m_kind) == equiptype then
			return true
		end
	end
	return false;
end

-- 根据装备类型 和 最小的颜色等级获取所有该类型的装备集合(table)
function Equip:GetEquipsByTypeWithMinColor( equiptype , mincolor , maxcolor )
	local equips = {};
	for tmpi=0, MAX_EQUIPNUM-1, 1 do
		local pEquip = self:GetAnyEquip( tmpi );
		if pEquip ~= nil and equip_gettype(pEquip.m_kind) == equiptype and pEquip.m_color_level >= mincolor and pEquip.m_color_level <= maxcolor then
			table.insert( equips, {
				m_equipindex = tmpi,
				m_equipkind = pEquip.m_kind,
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

-- 获取总装备数量
function Equip:GetCount( kind )
	local count = 0;
	for tmpi=0, MAX_EQUIPNUM-1, 1 do
		local pEquip = self:GetAnyEquip( tmpi );
		if pEquip ~= nil and pEquip.m_kind == kind then
			count = count + 1;
		end
	end
	return count;
end

-- 获取总装备数量
function Equip:GetCountBuyType( equiptype )
	local count = 0;
	for tmpi=0, MAX_EQUIPNUM-1, 1 do
		local pEquip = self:GetAnyEquip( tmpi );
		if pEquip ~= nil and equip_gettype(pEquip.m_kind) == equiptype then
			count = count + 1;
		end
	end
	return count;
end

-- 获取总装备数量
function Equip:GetTotal()
	local count = 0;
	for tmpi=0, MAX_EQUIPNUM-1, 1 do
		local pEquip = self:GetAnyEquip( tmpi );
		if pEquip ~= nil and pEquip.m_kind > 0 then
			count = count + 1;
		end
	end
	return count;
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

-- 设置装备洗练星级
function SetEquipWash( uiWash, pEquip )
	local has = 0;
	for i=1, 4, 1 do
		local uiStar = uiWash.transform:GetChild(i-1).gameObject;
		local color = equip_getcolor( pEquip.m_kind );
		local washid = pEquip.m_washid[i];
		if washid > 0 then
			SetTrue( uiStar )
			if g_equip_wash[washid].level >= g_equip_washrule[color].levellimit then
				SetImage( uiStar, LoadSprite("ui_icon_star_1") )
			else
				SetImage( uiStar, LoadSprite("ui_icon_star_2") )
			end
			has = 1;
		else	
			SetFalse( uiStar )
		end
	end
	if has == 1 then
		SetTrue( uiWash )
	else
		SetFalse( uiWash )
	end
end

-- 设置装备洗练属性信息
function SetEquipWashLevel( uiWashLevel, pEquip )
	local has = 0;
	for i=1, 4, 1 do
		local uiLevel = uiWashLevel.transform:GetChild(i-1).gameObject;
		local color = equip_getcolor( pEquip.m_kind );
		local washid = pEquip.m_washid[i];
		if washid > 0 then
			SetTrue( uiLevel )
			SetImage( uiLevel.transform:Find("Icon"), EquipWashSprite( g_equip_wash[washid].ability ) )
			SetText( uiLevel.transform:Find("Level"), "Lv."..g_equip_wash[washid].level )
			has = 1;
		else	
			SetFalse( uiLevel )
		end
	end
	if has == 1 then
		SetTrue( uiWashLevel )
	else
		SetFalse( uiWashLevel )
	end
end

