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
AWARDKIND_LOSTREBUILD	=	50021 -- 高级重建次数
AWARDKIND_CITYGUARDNUM	=	50022 -- 城防补充次数
AWARDKIND_BUFF_TRAIN	=	50025 -- 武卒官加速N%，时间1天
AWARDKIND_BUFF_MARCH	=	50026 -- 行军耗时降低N%，时间1天
AWARDKIND_LEVY_SILVER	=	50027 -- 奖励N次银币征收量
AWARDKIND_LEVY_WOOD		=	50028 -- 奖励N次木材征收量
AWARDKIND_LEVY_FOOD		=	50029 -- 奖励N次粮食征收量
AWARDKIND_LEVY_IRON		=	50030 -- 奖励N次镔铁征收量
AWARDKIND_TIANCE_POINT	=	50031 -- 天策府点数我国
AWARDKIND_TIANCE_POINT_1=	50032 -- 天策府点数魏国
AWARDKIND_TIANCE_POINT_2=	50033 -- 天策府点数蜀国
AWARDKIND_TIANCE_POINT_3=	50034 -- 天策府点数吴国
AWARDKIND_ACTIVITY05_XW	=	50036 -- 陈宫信物
AWARDKIND_PERMISSION_1	=	50051 -- 购买步兵权限（只用于显示）
AWARDKIND_PERMISSION_2	=	50052 -- 购买骑兵权限（只用于显示）
AWARDKIND_PERMISSION_3	=	50053 -- 购买弓兵权限（只用于显示）
AWARDKIND_PERMISSION_4	=	50054 -- 科技快研
AWARDKIND_PERMISSION_5	=	50055 -- 装备回收图纸
AWARDKIND_PERMISSION_6	=	50056 -- 作坊预设
AWARDKIND_RES_SILVER	=	60001 -- 奖励未启用资源点（民居）
AWARDKIND_RES_WOOD		=	60002 -- 奖励未启用资源点（木材厂）
AWARDKIND_RES_FOOD		=	60003 -- 奖励未启用资源点（农田）
AWARDKIND_RES_IRON		=	60004 -- 奖励未启用资源点（矿厂）
AWARDKIND_GIRLBASE		=	70000 -- 女将(70000+女将编号)
AWARDKIND_GIRLSOULBASE	=	71000 -- 女将碎片(71000+女将编号)

-- 奖励形象
function AwardInfo( awardkind )
	local sprite = nil;
	local color = nil;
	local name = "";
	local c = 0;
	local desc = ""
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
		desc = item_getdesc( awardkind )
		
	-- 装备
	elseif awardkind <= AWARDKIND_HEROBASE then
		local kind = awardkind-AWARDKIND_EQUIPBASE
		sprite = EquipSprite( kind )
		color = ItemColorSprite( equip_getcolor( kind ) );
		name = EquipName( kind )
		c = equip_getcolor( kind )
		desc = equip_getabilityname( kind )
		
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
		c = 1
		
	elseif awardkind == AWARDKIND_IRON then	-- 镔铁
		sprite = ItemSprite( 123 );
		color = ItemColorSprite( 1 );
		name = T(124)
		c = 1
		
	elseif awardkind == AWARDKIND_TOKEN then-- 元宝
		sprite = ItemSprite( 124 );
		color = ItemColorSprite( 0 );
		name = T(125)
		
	elseif awardkind == AWARDKIND_BODY then	 -- 体力
		sprite = ItemSprite( 126 );
		color = ItemColorSprite( 0 );
		name = T(126)
		
	elseif awardkind == AWARDKIND_INFANTRY then -- 步兵
		sprite = LoadSprite( "item_icon_445" );
		color = ItemColorSprite( 0 );
		name = T(134)
		
	elseif awardkind == AWARDKIND_CAVALRY then -- 骑兵
		sprite = LoadSprite( "item_icon_446" );
		color = ItemColorSprite( 0 );
		name = T(135)
		
	elseif awardkind == AWARDKIND_ARCHER then -- 弓兵
		sprite = LoadSprite( "item_icon_447" );
		color = ItemColorSprite( 0 );
		name = T(136)
		
	elseif awardkind == AWARDKIND_EXP then	-- 角色经验
		sprite = ItemSprite( 127 );
		color = ItemColorSprite( 0 );
		name = T(128)
		
	elseif awardkind == AWARDKIND_VIPEXP then	-- VIP经验
		sprite = LoadSprite( "item_icon_482" );
		color = ItemColorSprite( 0 );
		name = T(190)
		
	elseif awardkind == AWARDKIND_AUTOBUILD	then -- 自动建造次数
		sprite = LoadSprite( "item_icon_469" );
		color = ItemColorSprite( 2 );
		name = T(191)
		c = 2
		
	elseif awardkind == AWARDKIND_LEVYNUM then	 -- 征收次数
		sprite = LoadSprite( "Char_Default" );
		color = ItemColorSprite( 0 );
		
	elseif awardkind == AWARDKIND_PEOPLE then	-- 人口
		sprite = LoadSprite( "item_icon_488" );
		color = ItemColorSprite( 2 );
		name = T(127)
		c = 2
		
	elseif awardkind == AWARDKIND_PRESTIGE then	-- 威望值
		sprite = LoadSprite( "item_icon_125" );
		color = ItemColorSprite( 2 );
		name = T(152)
		c = 2
		
	elseif awardkind == AWARDKIND_FRIENDSHIP then -- 友谊积分
		sprite = LoadSprite( "item_icon_487" );
		color = ItemColorSprite( 0 );
		name = ""
		
	elseif awardkind == AWARDKIND_TRAIN_QUICK then -- 募兵加速
		sprite = LoadSprite( "item_icon_474" );
		color = ItemColorSprite( 0 );
		name = T(1010)
		
	elseif awardkind == AWARDKIND_BUILD_QUICK then-- 建造加速
		sprite = LoadSprite( "item_icon_470" );
		color = ItemColorSprite( 0 );
		name = T(1011)
		
	elseif awardkind == AWARDKIND_HERO_WASH	then-- 免费洗髓次数
		sprite = LoadSprite( "item_icon_164" );
		color = ItemColorSprite( 0 );
		name = T(1100)
		
	elseif awardkind == AWARDKIND_EQUIP_WASH then-- 免费洗练次数
		sprite = LoadSprite( "item_icon_462" );
		color = ItemColorSprite( 0 );
		name = T(1101)
		
	elseif awardkind == AWARDKIND_LOSTREBUILD then -- 高级重建次数
		sprite = LoadSprite( "item_icon_1000" );
		color = ItemColorSprite( 2 );
		name = T(1375)
		
	elseif awardkind == AWARDKIND_CITYGUARDNUM then -- 城防补充次数
		sprite = LoadSprite( "item_icon_471" );
		color = ItemColorSprite( 2 );
		name = T(1377)
		c = 2
		
	elseif awardkind == AWARDKIND_BUFF_TRAIN then -- 武卒官加速N%，时间1天
		sprite = LoadSprite( "item_icon_473" );
		color = ItemColorSprite( 5 );
		name = item_getname(474)
		c = 5
			
	elseif awardkind == AWARDKIND_BUFF_MARCH then -- 行军耗时降低N%，时间1天
		sprite = LoadSprite( "item_icon_474" );
		color = ItemColorSprite( 5 );
		name = item_getname(473)
		c = 5
		
	elseif awardkind == AWARDKIND_LEVY_SILVER then -- 奖励N次银币征收量
		sprite = ItemSprite( 120 );
		color = ItemColorSprite( 0 );
		name = T(121)
		
	elseif awardkind == AWARDKIND_LEVY_WOOD then -- 奖励N次木材征收量
		sprite = ItemSprite( 121 );
		color = ItemColorSprite( 0 );
		name = T(122)
		
	elseif awardkind == AWARDKIND_LEVY_FOOD then -- 奖励N次粮食征收量
		sprite = ItemSprite( 122 );
		color = ItemColorSprite( 0 );
		name = T(123)
		
	elseif awardkind == AWARDKIND_LEVY_IRON	then -- 奖励N次镔铁征收量
		sprite = ItemSprite( 123 );
		color = ItemColorSprite( 1 );
		name = T(124)
		c = 1
		
	elseif awardkind == AWARDKIND_PERMISSION_1 then -- 购买步兵权限（只用于显示）
		sprite = LoadSprite( "item_icon_445" );
		color = ItemColorSprite( 0 );
		name = T(2251)
		c = 3
		desc = T(2252)
		
	elseif awardkind == AWARDKIND_PERMISSION_2 then -- 购买骑兵权限（只用于显示）
		sprite = LoadSprite( "item_icon_446" );
		color = ItemColorSprite( 0 );
		name = T(2253)
		c = 3
		desc = T(2254)
		
	elseif awardkind == AWARDKIND_PERMISSION_3 then -- 购买弓兵权限（只用于显示）
		sprite = LoadSprite( "item_icon_447" );
		color = ItemColorSprite( 0 );
		name = T(2255)
		c = 3
		desc = T(2256)
		
	elseif awardkind == AWARDKIND_PERMISSION_4 then -- 科技快研
		sprite = LoadSprite( "herohead_4" );
		color = ItemColorSprite( 0 );
		name = T(2257)
		c = 5
		desc = T(2258)
		
	elseif awardkind == AWARDKIND_PERMISSION_5 then -- 装备回收图纸
		sprite = LoadSprite( "item_icon_226" );
		color = ItemColorSprite( 0 );
		name = T(2259)
		c = 5
		desc = T(2260)
		
	elseif awardkind == AWARDKIND_PERMISSION_6 then -- 作坊预设
		sprite = LoadSprite( "item_icon_34" );
		color = ItemColorSprite( 0 );
		name = T(2261)
		c = 5
		desc = T(2262)
	
	elseif awardkind == AWARDKIND_ACTIVITY05_XW	 then -- 陈宫的信物
		sprite = LoadSprite( "item_icon_489" );
		color = ItemColorSprite( 3 );
		name = item_getname( 489 )
		c = 3
		desc = item_getdesc( 489 )
	
	elseif awardkind == AWARDKIND_RES_SILVER then -- 奖励未启用资源点（民居）
		local kind = BUILDING_Silver
		sprite = BuildingSprite( kind )
		color = ItemColorSprite( 0 );
		name = BuildingName( kind )
		
	elseif AWARDKIND_RES_WOOD then-- 奖励未启用资源点（木材厂）
		local kind = BUILDING_Wood
		sprite = BuildingSprite( kind )
		color = ItemColorSprite( 0 );
		name = BuildingName( kind )
		
	elseif AWARDKIND_RES_FOOD then-- 奖励未启用资源点（农田）
		local kind = BUILDING_Food
		sprite = BuildingSprite( kind )
		color = ItemColorSprite( 0 );
		name = BuildingName( kind )
		
	elseif AWARDKIND_RES_IRON then-- 奖励未启用资源点（矿厂）
		local kind = BUILDING_Iron
		sprite = BuildingSprite( kind )
		color = ItemColorSprite( 0 );
		name = BuildingName( kind )

	-- 女将	
	elseif awardkind >= AWARDKIND_GIRLBASE and awardkind < AWARDKIND_GIRLSOULBASE then
		local kind = awardkind-AWARDKIND_GIRLBASE;
		sprite = GirlHeadSprite( kind )
		color = ItemColorSprite( g_girlinfo[kind][0].init_color );
		name = GirlName( kind )
		c = g_girlinfo[kind][0].init_color
		
	-- 女将碎片	
	elseif awardkind >= AWARDKIND_GIRLSOULBASE and awardkind < AWARDKIND_GIRLSOULBASE+1000 then
		local kind = awardkind-AWARDKIND_GIRLSOULBASE;
		sprite = GirlHeadSprite( kind )
		color = ItemColorSprite( g_girlinfo[kind][0].init_color );
		name = GirlName( kind )	
		c = g_girlinfo[kind][0].init_color
		
	end
	
	if sprite == nil then
		sprite = LoadSprite( "Char_Default" );
	end
	if color == nil then
		color = LoadSprite( ItemColorSprite( 0 ) );
	end
	return sprite, color, name, c, desc;
end

