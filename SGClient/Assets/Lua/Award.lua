--
AWARDKIND_ITEMBASE		=	0	  -- 道具（0+道具编号）
AWARDKIND_EQUIPBASE		=	10000 -- 装备（10000+装备编号）
AWARDKIND_HEROBASE		=	20000 -- 英雄（20000+英雄编号）
AWARDKIND_BUILDINGBASE	=	30000 -- 建筑（30000+建筑编号）
AWARDKIND_FUNCTION		=	40000 -- 功能（40000+功能编号）
AWARDKIND_VALUEBASE		=	50000 -- 数值
AWARDKIND_SILVER		=	50001 -- 银币
AWARDKIND_WOOD			=	50002 -- 木材
AWARDKIND_FOOD			=	50003 -- 粮食
AWARDKIND_IRON			=	50004 -- 镔铁
AWARDKIND_TOKEN			=	50005 -- 元宝
AWARDKIND_BODY			=	50006 -- 体力
AWARDKIND_INFANTRY		=	50007 -- 步兵
AWARDKIND_CAVALRY		=	50008 -- 骑兵
AWARDKIND_ARCHER		=	50009 -- 弓兵
AWARDKIND_EXP			=	50010 -- 角色经验
AWARDKIND_VIPEXP		=	50011 -- VIP经验
AWARDKIND_AUTOBUILD		=	50012 -- 自动建造次数
AWARDKIND_LEVYNUM		=	50013 -- 征收次数
AWARDKIND_PEOPLE		=	50014 -- 人口
AWARDKIND_PRESTIGE		=	50015 -- 威望值
AWARDKIND_FRIENDSHIP	=	50016 -- 友谊积分
AWARDKIND_TRAIN_QUICK	=	50017 -- 募兵加速
AWARDKIND_BUILD_QUICK	=	50018 -- 建造加速
AWARDKIND_HERO_WASH		=	50019 -- 免费洗髓次数
AWARDKIND_EQUIP_WASH	=	50020 -- 免费洗练次数

-- 奖励形象
function AwardInfo( awardkind )
	local sprite = nil;
	local color = nil;
	local name = "";
	local c = 0;
	-- 掉落包
	if awardkind <= AWARDKIND_ITEMBASE then
		sprite = LoadSprite( "Char_Default" );
		color = ItemColorSprite( 0 );
		name = T(555)
		
	-- 道具
	elseif awardkind <= AWARDKIND_EQUIPBASE then
		sprite = ItemSprite( awardkind );
		color = ItemColorSprite( item_getcolor( awardkind ) );
		name = item_getname( awardkind )
		c = item_getcolor( awardkind );
		
	-- 装备
	elseif awardkind <= AWARDKIND_HEROBASE then
		local kind = awardkind-AWARDKIND_EQUIPBASE
		sprite = EquipSprite( kind )
		color = ItemColorSprite( equip_getcolor( kind ) );
		name = EquipName( awardkind )
		c = equip_getcolor( kind )
		
	-- 英雄	
	elseif awardkind <= AWARDKIND_BUILDINGBASE then
		local kind = awardkind-AWARDKIND_HEROBASE;
		sprite = HeroHeadSprite( kind )
		color = ItemColorSprite( 0 );
		name = HeroName( kind )
		
	-- 建筑	
	elseif awardkind <= AWARDKIND_FUNCTION then
		local kind = awardkind-AWARDKIND_BUILDINGBASE;
		sprite = BuildingSprite( awardkind-AWARDKIND_BUILDINGBASE )
		color = ItemColorSprite( 0 );
		name = BuildingName( kind )
		
	elseif awardkind == AWARDKIND_SILVER then -- 银币
		sprite = ItemSprite( 120 );
		color = ItemColorSprite( 0 );
		name = T(121)
		
	elseif awardkind == AWARDKIND_WOOD then	-- 木材
		sprite = ItemSprite( 121 );
		color = ItemColorSprite( 0 );
		name = T(122)
		
	elseif awardkind == AWARDKIND_FOOD then	-- 粮食
		sprite = ItemSprite( 122 );
		color = ItemColorSprite( 0 );
		name = T(123)
		
	elseif awardkind == AWARDKIND_IRON then	-- 镔铁
		sprite = ItemSprite( 123 );
		color = ItemColorSprite( 0 );
		name = T(124)
		
	elseif awardkind == AWARDKIND_TOKEN then-- 元宝
		sprite = ItemSprite( 124 );
		color = ItemColorSprite( 0 );
		name = T(125)
		
	elseif awardkind == AWARDKIND_BODY then	 -- 体力
		sprite = ItemSprite( 126 );
		color = ItemColorSprite( 0 );
		name = T(126)
		
	elseif awardkind == AWARDKIND_INFANTRY then -- 步兵
		sprite = LoadSprite( "Char_Default" );
		color = ItemColorSprite( 0 );
		
	elseif awardkind == AWARDKIND_CAVALRY then -- 骑兵
		sprite = LoadSprite( "Char_Default" );
		color = ItemColorSprite( 0 );
		
	elseif awardkind == AWARDKIND_ARCHER then -- 弓兵
		sprite = LoadSprite( "Char_Default" );
		color = ItemColorSprite( 0 );
		
	elseif awardkind == AWARDKIND_EXP then	-- 角色经验
		sprite = ItemSprite( 127 );
		color = ItemColorSprite( 0 );
		name = T(128)
		
	elseif awardkind == AWARDKIND_VIPEXP then	-- VIP经验
		sprite = LoadSprite( "Char_Default" );
		color = ItemColorSprite( 0 );
		name = T(190)
		
	elseif awardkind == AWARDKIND_AUTOBUILD	then -- 自动建造次数
		sprite = LoadSprite( "Char_Default" );
		color = ItemColorSprite( 0 );
		name = T(191)
		
	elseif awardkind == AWARDKIND_LEVYNUM then	 -- 征收次数
		sprite = LoadSprite( "Char_Default" );
		color = ItemColorSprite( 0 );
		
	elseif awardkind == AWARDKIND_PEOPLE then	-- 人口
		sprite = LoadSprite( "Char_Default" );
		color = ItemColorSprite( 0 );
		name = T(127)
		
	elseif awardkind == AWARDKIND_PRESTIGE then	-- 威望值
		sprite = LoadSprite( "Char_Default" );
		color = ItemColorSprite( 0 );
		name = T(152)
		
	elseif awardkind == AWARDKIND_FRIENDSHIP then -- 友谊积分
		sprite = LoadSprite( "Char_Default" );
		color = ItemColorSprite( 0 );
		name = ""
		
	elseif awardkind == AWARDKIND_TRAIN_QUICK then -- 募兵加速
		sprite = LoadSprite( "Char_Default" );
		color = ItemColorSprite( 0 );
		name = T(1010)
		
	elseif awardkind == AWARDKIND_BUILD_QUICK then-- 建造加速
		sprite = LoadSprite( "Char_Default" );
		color = ItemColorSprite( 0 );
		name = T(1011)
		
	elseif awardkind == AWARDKIND_HERO_WASH	then-- 免费洗髓次数
		sprite = LoadSprite( "Char_Default" );
		color = ItemColorSprite( 0 );
		name = T(1100)
		
	elseif awardkind == AWARDKIND_EQUIP_WASH then-- 免费洗练次数
		sprite = LoadSprite( "Char_Default" );
		color = ItemColorSprite( 0 );
		name = T(1101)
	end
	
	if sprite == nil then
		sprite = LoadSprite( "Char_Default" );
	end
	if color == nil then
		color = LoadSprite( ItemColorSprite( 0 ) );
	end
	return sprite, color, name, c;
end

