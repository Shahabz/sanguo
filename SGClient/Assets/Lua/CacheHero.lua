-- 英雄缓存信息
MAX_HERONUM			= 128 -- 英雄数量
MAX_CITYHERONUM		= 12 -- 上阵英雄数量

-- 一个英雄结构
SLK_Hero = class("SLK_Hero")
function SLK_Hero:ctor()
	self:empty();
end

function SLK_Hero:empty()
	
	self.m_kind  		= 	0;-- 种类
	self.m_level		=	0;-- 等级
	self.m_corps  		= 	0;-- 兵种
	self.m_color		=	0;-- 颜色
	self.m_exp			=	0;-- 经验
	self.m_exp_max		=	0;-- 经验上限
	self.m_soldiers		=	0;-- 当前兵力
	self.m_state		=	0;-- 当前
	
	self.m_attack_base	=	0;--基础攻击资质
	self.m_attack_wash	=	0;--洗髓攻击资质
	self.m_defense_base	=	0;--基础防御资质
	self.m_defense_wash	=	0;--洗髓防御资质
	self.m_troops_base	=	0;--基础兵力资质
	self.m_troops_wash	=	0;--洗髓兵力资质
	
	self.m_attack		=	0;-- 攻击
	self.m_defense		=	0;-- 防御
	self.m_troops		=	0;-- 兵力
	
	-- 英雄装备
	self.m_Equip			=	{};
	for i=0,5,1 do
		local object = SLK_Equip.new();
		self.m_Equip[i] = object;
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
function Hero:ResetAll()
	
	-- 上阵英雄列表
	self.m_CityHero = {};
	for i=0,MAX_CITYHERONUM-1,1 do
		self.m_CityHero[i] = SLK_Hero.new();
	end
	
	-- 英雄列表
	self.m_Hero = {};
	for i=0,MAX_HERONUM-1,1 do
		self.m_Hero[i] = SLK_Hero.new();
	end
	
end

-- 设置背包英雄
function Hero:SetHero( nIndex, pHero )
	if pHero ~= nil then
		self.m_Hero[nIndex] = clone(pHero); -- 这个地方是否要用到深拷贝
	else
		self.m_Hero[nIndex]:empty(); -- 清空这个英雄
	end
	self:OnHeroChange( nIndex );
end

-- 设置城池英雄
function Hero:SetCityHero( nIndex, pHero )
	if pHero ~= nil then
		self.m_CityHero[nIndex] = clone(pHero); -- 这个地方是否要用到深拷贝
	else
		self.m_CityHero[nIndex]:empty(); -- 清空这个英雄
	end
	self:OnCityHeroChange( nIndex );
end

-- 
function Hero:OnHeroChange( nIndex )
	
end

-- 
function Hero:OnCityHeroChange( nIndex )
	
end

-- 这个格子是否存在英雄
function Hero:HasHero( nIndex )
	if nIndex < 0 or nIndex >= MAX_HERONUM then
		return false;
	end
	if self.m_Hero[nIndex].m_kind <= 0 then
		return false;
	end
	return true;
end

-- 这个格子是否存在英雄
function Hero:HasCityHero( nIndex )
	if nIndex < 0 or nIndex >= MAX_CITYHERONUM then
		return false;
	end
	if self.m_CityHero[nIndex].m_kind <= 0 then
		return false;
	end
	return true;
end

-- 根据kind获取
function Hero:GetIndex( kind )
	for tmpi=0, MAX_HERONUM-1, 1 do
		if self.m_Hero[tmpi].m_kind == kind then
			return "Hero", tmpi;
		end
	end
	for tmpi=0, MAX_CITYHERONUM-1, 1 do
		if self.m_CityHero[tmpi].m_kind == kind then
			return "CityHero", tmpi;
		end
	end
	return "", -1;
end

-- 清空所有的New标示
function Item:ClearAllNew()
	for tmpi=0, MAX_HERONUM-1, 1 do
		self.m_Hero[tmpi].m_bIsNew = false;
	end
	for tmpi=0, MAX_CITYHERONUM-1, 1 do
		self.m_CityHero[tmpi].m_bIsNew = false;
	end
end

-- 全局
local G_Hero = nil;
function GetHero()
	if G_Hero == nil then
		G_Hero = Hero.new();
	end
	return G_Hero;
end
