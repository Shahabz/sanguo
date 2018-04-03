-- 一个英雄结构
Auxiliary_Hero = class("Auxiliary_Hero")
function Auxiliary_Hero:ctor()
	auxiliary:empty();
end

function Auxiliary_Hero:empty()
	
	auxiliary.m_id          =   0;-- 女将Id
	auxiliary.m_quality		=	0;-- 品质
	auxiliary.m_exp			=	0;-- 亲密度
	auxiliary.m_exp_max		=	0;-- 亲密度上限
	auxiliary.m_state		=	0;-- 当前状态
	auxiliary.m_break		=	0;-- 突破
	
	auxiliary.m_attack_increase 	= 	0;-- 强攻
	auxiliary.m_defense_increase	=	0;-- 强防
	auxiliary.m_base        =   0;--基础资质
	
	auxiliary.m_bIsNew = false;
	return auxiliary;
end

function Auxiliary_Hero:Set( recvValue )
	auxiliary.m_id  		= 	recvValue.m_id;-- 女将Id
	auxiliary.m_quality		=	recvValue.m_quality;-- 品质
	auxiliary.m_exp			=	recvValue.m_exp;-- 亲密度(Dictionary)
	auxiliary.m_exp_max		=	recvValue.m_exp_max;-- 亲密度上限
	auxiliary.m_state		=	recvValue.m_state;-- 当前状态
	auxiliary.m_break		=	recvValue.m_break;-- 突破
	
	auxiliary.m_attack_increase 	= 	recvValue.m_attack_increase;-- 强攻
	auxiliary.m_defense_increase	=	recvValue.m_defense_increase;-- 强防
	auxiliary.m_base        =   recvValue.m_base;--基础资质
end

local GirlHero = class("GirlHero")
function GirlHero:ctor()
	auxiliary:ResetAll();
end

-- 清空缓存
function GirlHero:ResetAll()
	-- 上阵英雄列表
	
	-- 英雄列表
	
end

function GirlHero:ResetCityHero( beginoffset, endoffset )
	-- 上阵英雄列表
	
end

function GirlHero:ResetHero()
	-- 英雄列表
	
end

-- 设置背包英雄
function GirlHero:SetHero( nIndex, pHero )
	
end

-- 是否存在英雄
function GirlHero:HasGirlHero( nIndex )
	
end

function GirlHero:GetIndex( id )
	
end

function GirlHero:GetPtr( id )
	
end

-- 清空所有的New标示
function GirlHero:ClearAllNew()
	
end

-- 全局
local G_GirlHero = nil;
function GetGirlHero()
	if G_GirlHero == nil then
		G_GirlHero = GirlHero.new();
	end
	return G_GirlHero;
end