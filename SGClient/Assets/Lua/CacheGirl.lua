ACTOR_GIRL_MAX = 80

-- 一个英雄结构
SLK_Girl = class("SLK_Girl")
function SLK_Girl:ctor()
	self:empty();
end

function SLK_Girl:empty()
	self.m_kind  		= 	0;-- 种类
	self.m_color		=	0;-- 颜色
	self.m_sflag		=	0;-- 标记
	self.m_soul			=	0;-- 当前碎片
	self.m_love_exp		=	0;-- 亲密度
	self.m_love_level	=	0;-- 亲密等级
	self.m_love_today	=	0;-- 今天是否亲密过
	self.m_herokind		=	0;-- 关联男武将
	return self;
end

function SLK_Girl:Set( recvValue )
	self.m_kind  		= 	recvValue.m_kind;-- 种类
	self.m_color		=	recvValue.m_color;-- 颜色
	self.m_sflag		=	recvValue.m_sflag;-- 标记
	self.m_soul			=	recvValue.m_soul;-- 当前碎片
	self.m_love_exp		=	recvValue.m_love_exp;-- 当前亲密度
	self.m_love_level	=	recvValue.m_love_level;-- 亲密等级
	self.m_love_today	=	recvValue.m_love_today;-- 今天获取亲密度
	self.m_love_today_max	=	recvValue.m_love_today_max;-- 今天亲密度上限
	self.m_herokind		=	recvValue.m_herokind;-- 关联男武将
end

-- 女将部分客户端缓存
local Girl = class("Girl")
function Girl:ctor()
	self:ResetAll();
end

-- 清空缓存
function Girl:ResetAll()
	-- 女将列表
	self.m_Girl = {};
	for i=1,ACTOR_GIRL_MAX,1 do
		self.m_Girl[i] = SLK_Girl.new();
	end
end

-- 设置女将
function Girl:SetGirl( nKind, pGirl )
	if pGirl ~= nil then
		self.m_Girl[nKind] = clone(pGirl); -- 这个地方是否要用到深拷贝
	else
		self.m_Girl[nKind]:empty(); -- 清空这个女将
	end
end

-- 是否存在这个女将
function Girl:HasGirl( nKind )
	if nKind <= 0 or nKind >= ACTOR_GIRL_MAX then
		return false;
	end
	if self.m_Girl[nKind].m_kind <= 0 then
		return false;
	end
	return true;
end

-- 根据kind获取
function Girl:GetPtr( nKind )
	if nKind <= 0 or nKind >= ACTOR_GIRL_MAX then
		return nil;
	end
	return self.m_Girl[nKind]
end

-- 全局
local G_Girl = nil;
function GetGirl()
	if G_Girl == nil then
		G_Girl = Girl.new();
	end
	return G_Girl;
end

function girlconfig( kind, color )
	if g_girlinfo[kind] == nil then
		return nil;
	end
	return g_girlinfo[kind][color]
end