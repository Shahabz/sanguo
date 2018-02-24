-- 任务类型
QUEST_DATATYPE_WELCOME				=	0	-- 欢迎任务
QUEST_DATATYPE_CREATENAME			=	1	-- 创建名称
QUEST_DATATYPE_CLICK				=	2	-- 点击操作 datakind=1马岱 2前往世界 3返回城内 4查看铁匠铺 5前往副本
QUEST_DATATYPE_NEWFIGHT				=	3	-- 围剿扰乱民居的山贼
QUEST_DATATYPE_ACTOR_LEVEL			=	11	-- 主公等级 datatype=1 datakind=0 needvalue=等级
QUEST_DATATYPE_BUILDING_LEVEL		=	12	-- 建筑等级 datatype=2 datakind=建筑kind dataoffset=资源建筑编号 needvalue=建筑等级
QUEST_DATATYPE_BUILDING_SILVER		=	13	-- N个民居达到N级 datatype=3 datakind=等级 needvalue=建筑数量
QUEST_DATATYPE_BUILDING_WOOD		=	14	-- N个木场达到N级 datatype=4 datakind=等级 needvalue=建筑数量
QUEST_DATATYPE_BUILDING_FOOD		=	15	-- N个农田达到N级 datatype=5 datakind=等级 needvalue=建筑数量
QUEST_DATATYPE_BUILDING_IRON		=	16	-- N个矿场达到N级 datatype=6 datakind=等级 needvalue=建筑数量
QUEST_DATATYPE_LEVYNUM				=	17	-- 征收次数 datatype=7 datakind=0 needvalue=数量
QUEST_DATATYPE_STORY				=	18	-- 副本 datatype=8 datakind=副本编号 needvalue=1
QUEST_DATATYPE_KILLMONSTER			=	19	-- 打流寇N次 datatype=9 datakind=0 needvalue=数量
QUEST_DATATYPE_KILLMONSTER_LEVEL	=	20	-- 打流寇N级N次 datatype=10 datakind=等级 needvalue=数量
QUEST_DATATYPE_EQUIP_FORGING		=	21	-- 打造N装备N数量 datatype=11 datakind=装备kind needvalue=数量
QUEST_DATATYPE_EQUIP_WASH			=	22	-- 装备洗炼次数 datatype=12 datakind=0 needvalue=数量
QUEST_DATATYPE_EQUIP_COLORWASH		=	23	-- 将N品质装备的属性洗炼至满级 datatype=13 datakind=颜色 needvalue=1
QUEST_DATATYPE_TRAIN				=	24	-- 募兵N兵种N数量 datatype=14 datakind=兵种（1，2，3） needvalue=数量
QUEST_DATATYPE_TRAINCOUNT			=	25	-- 募兵N次 datatype=15 datakind=0 needvalue=数量
QUEST_DATATYPE_CITY_TECH			=	26	-- 研究N科技N级 datatype=16 datakind=科技ID needvalue=等级
QUEST_DATATYPE_CITY_TECHONE			=	27	-- 研究一次N科技 datatype=17 datakind=科技ID needvalue=1
QUEST_DATATYPE_HERO_WASHCOUNT		=	28	-- 武将洗髓N次 datatype=18 datakind=0 needvalue=次数
QUEST_DATATYPE_HERO_COLORWASH		=	29	-- 将一名N品质的武将属性洗满 datatype=19 datakind=颜色 needvalue=1
QUEST_DATATYPE_HERO_CALL			=	30	-- 招募武将 datatype=20 datakind=kind needvalue=1
QUEST_DATATYPE_WISHING				=	31	-- 聚宝盆N次 datatype=21 datakind=0 needvalue=次数
QUEST_DATATYPE_CITYFIGHT			=	32	-- 成功击败N名敌国玩家 datatype=22 datakind=0 needvalue=次数
QUEST_DATATYPE_EQUIP_UP				=	33	-- 给N武将穿N装备 datatype=33 datakind=武将kind dataoffset=装备编号
QUEST_DATATYPE_BUILDING_UPGRADE		=	34	-- 升级操作 datatype=34 datakind=建筑kind dataoffset=资源建筑编号 needvalue=建筑等级
QUEST_DATATYPE_TRAIN_OP				=	35	-- 操作-募兵N兵种N数量 datatype=24 datakind=兵种（1，2，3） needvalue=数量
QUEST_DATATYPE_TRAINCOUNT_OP		=	36	-- 操作-募兵N次 datatype=25 datakind=0 needvalue=数量
QUEST_DATATYPE_BRANCH_QUEST			=	37	-- 完成任意N个支线任务 datatype=37 datakind=0 needvalue=数量
QUEST_DATATYPE_EQUIP_FORGING_QUICK	=	38	-- 进行一次铁匠铺加速打造 datatype=38 datakind=0 needvalue=1
QUEST_DATATYPE_HERO_UP				=	39	-- 前往聚贤馆让N上阵 datatype=39 datakind=武将kind needvalue=1
QUEST_DATATYPE_SMITHYOFFHIRE		=	40	-- 铁匠铺内雇佣N级或更高级铁匠 datatype=40 datakind=0 needvalue=等级
QUEST_DATATYPE_NEQUIP_FORGING		=	41	-- 打造国器N datatype=41 datakind=国器kind needvalue=1
QUEST_DATATYPE_NEQUIP_LEVELNUM		=	42	-- 国器升级N次 datatype=42 datakind=0 needvalue=次数
QUEST_DATATYPE_MATERIAL_MAKE		=	43	-- 材料作坊生产材料N次（操作） datatype=43 datakind=0 needvalue=次数
QUEST_DATATYPE_HEROGUARD_UP			=	44	-- 御林卫上阵任意武将N个 datatype=44 datakind=0 needvalue=数量
QUEST_DATATYPE_HERO_VISIT			=	45	-- 进行良将寻访N次 datatype=45 datakind=0 needvalue=数量

QUEST_MAINID_MADAI	=	20	--	 马岱任务特殊处理
QUEST_MAINID_LIUKOU	=	55	--   木场流寇任务特殊处理

g_QuestTargetEnemyLevel = 0; -- 任务流寇等级

-- 任务缓存
CacheQuest = {};
function CacheQuestClear()
	CacheQuest = {};
end

-- m_count=0,m_list={m_questid=0,m_flag=0,m_datatype=0,m_datakind=0,m_dataoffset=0,m_value=0,m_needvalue=0,m_awardkind={[5]},m_awardnum={[5]},m_nameid=0,m_descid=0,[m_count]},
function CacheQuestSet( recvValue )
	g_QuestTargetEnemyLevel = 0;
	if CacheQuest ~= nil and CacheQuest.m_list ~= nil and CacheQuest.m_list[1] ~= nil then
		if CacheQuest.m_list[1].m_datatype == QUEST_DATATYPE_KILLMONSTER_LEVEL then
			-- 清除之前任务的目标流寇
			MapUnit.RefreshQuestEnemy( 0 )
		end
	end
	
	if recvValue.m_list[1].m_datatype == QUEST_DATATYPE_KILLMONSTER_LEVEL then
		-- 添加新任务的目标流寇
		g_QuestTargetEnemyLevel = recvValue.m_list[1].m_datakind;
		MapUnit.RefreshQuestEnemy( g_QuestTargetEnemyLevel )
	end
	
	CacheQuest = recvValue;
	GetPlayer().m_questid = recvValue.m_list[1].m_questid
end

-- 任务类型
function QuestType( recvValue )
	local typename = ""
	local datatype = recvValue.m_datatype;
	if datatype == QUEST_DATATYPE_ACTOR_LEVEL then-- 主公等级 datatype=1 datakind=0 needvalue=等级
		typename = Localization.text_quest( 90 )
	elseif datatype == QUEST_DATATYPE_BUILDING_LEVEL or datatype == QUEST_DATATYPE_BUILDING_UPGRADE then-- 建筑等级 datatype=2 datakind=建筑kind dataoffset=资源建筑编号 needvalue=建筑等级
		typename = Localization.text_quest( 91 )
	elseif datatype == QUEST_DATATYPE_BUILDING_SILVER then-- N个民居达到N级 datatype=3 datakind=等级 needvalue=建筑数量
		typename = Localization.text_quest( 91 )
	elseif datatype == QUEST_DATATYPE_BUILDING_WOOD then-- N个木场达到N级 datatype=4 datakind=等级 needvalue=建筑数量
		typename = Localization.text_quest( 91 )
	elseif datatype == QUEST_DATATYPE_BUILDING_FOOD then-- N个农田达到N级 datatype=5 datakind=等级 needvalue=建筑数量
		typename = Localization.text_quest( 91 )
	elseif datatype == QUEST_DATATYPE_BUILDING_IRON then-- N个矿场达到N级 datatype=6 datakind=等级 needvalue=建筑数量
		typename = Localization.text_quest( 91 )
	elseif datatype == QUEST_DATATYPE_LEVYNUM then-- 征收次数 datatype=7 datakind=0 needvalue=数量
		typename = Localization.text_quest( 91 )
	elseif datatype == QUEST_DATATYPE_HISTORY then-- 副本 datatype=8 datakind=副本编号 needvalue=1
		typename = Localization.text_quest( 92 )
	elseif datatype == QUEST_DATATYPE_KILLMONSTER then-- 打流寇N次 datatype=9 datakind=0 needvalue=数量
		typename = Localization.text_quest( 93 )
	elseif datatype == QUEST_DATATYPE_KILLMONSTER_LEVEL then-- 打流寇N级N次 datatype=10 datakind=等级 needvalue=数量
		typename = Localization.text_quest( 93 )
	elseif datatype == QUEST_DATATYPE_EQUIP_FORGING then-- 打造N装备N数量 datatype=11 datakind=装备kind needvalue=数量
		typename = Localization.text_quest( 94 )
	elseif datatype == QUEST_DATATYPE_EQUIP_WASH then-- 装备洗炼次数 datatype=12 datakind=0 needvalue=数量
		typename = Localization.text_quest( 94 )
	elseif datatype == QUEST_DATATYPE_EQUIP_COLORWASH then-- 将N品质装备的属性洗炼至满级 datatype=13 datakind=颜色 needvalue=1
		typename = Localization.text_quest( 94 )
	elseif datatype == QUEST_DATATYPE_TRAIN or datatype == QUEST_DATATYPE_TRAIN_OP then-- 募兵N兵种N数量 datatype=14 datakind=兵种（1，2，3） needvalue=数量
		typename = Localization.text_quest( 93 )
	elseif datatype == QUEST_DATATYPE_TRAINCOUNT or datatype == QUEST_DATATYPE_TRAINCOUNT_OP then-- 募兵N次 datatype=15 datakind=0 needvalue=数量
		typename = Localization.text_quest( 93 )
	elseif datatype == QUEST_DATATYPE_CITY_TECH then-- 研究N科技N级 datatype=16 datakind=科技ID needvalue=等级
		typename = Localization.text_quest( 96 )
	elseif datatype == QUEST_DATATYPE_CITY_TECHONE	 then-- 研究一次N科技 datatype=17 datakind=科技ID needvalue=1
		typename = Localization.text_quest( 96 )
	elseif datatype == QUEST_DATATYPE_HERO_WASHCOUNT then-- 武将洗髓N次 datatype=18 datakind=0 needvalue=次数
		typename = Localization.text_quest( 95 )
	elseif datatype == QUEST_DATATYPE_HERO_COLORWASH then-- 将一名N品质的武将属性洗满 datatype=19 datakind=颜色 needvalue=1
		typename = Localization.text_quest( 95 )
	elseif datatype == QUEST_DATATYPE_HERO_CALL then-- 招募武将 datatype=20 datakind=kind needvalue=1
		typename = Localization.text_quest( 95 )
	elseif datatype == QUEST_DATATYPE_WISHING then	-- 聚宝盆N次 datatype=21 datakind=0 needvalue=次数
		typename = Localization.text_quest( 99 )
	elseif datatype == QUEST_DATATYPE_CITYFIGHT then-- 成功击败N名敌国玩家 datatype=22 datakind=0 needvalue=次数
		typename = Localization.text_quest( 93 )
	elseif datatype == QUEST_DATATYPE_EQUIP_UP then-- 成功击败N名敌国玩家 datatype=22 datakind=0 needvalue=次数
		typename = Localization.text_quest( 93 )
	elseif datatype == QUEST_DATATYPE_BRANCH_QUEST then -- 完成N个支线任务
		typename = Localization.text_quest( 97 )
	elseif datatype == QUEST_DATATYPE_EQUIP_FORGING_QUICK then -- 进行一次铁匠铺加速打造 datatype=38 datakind=0 needvalue=1
		typename = Localization.text_quest( 94 )
	elseif datatype == QUEST_DATATYPE_HERO_UP then -- 前往聚贤馆让N上阵 datatype=39 datakind=武将kind needvalue=1
		typename = Localization.text_quest( 95 )
	elseif datatype == QUEST_DATATYPE_SMITHYOFFHIRE	then -- 铁匠铺内雇佣N级或更高级铁匠 datatype=40 datakind=0 needvalue=等级
		typename = Localization.text_quest( 94 )
	elseif datatype == QUEST_DATATYPE_NEQUIP_FORGING then -- 打造国器N datatype=41 datakind=国器kind needvalue=1
		typename = Localization.text_quest( 89 )
	elseif datatype == QUEST_DATATYPE_NEQUIP_LEVELNUM then -- 国器升级N次 datatype=42 datakind=0 needvalue=次数
		typename = Localization.text_quest( 89 )
	elseif datatype == QUEST_DATATYPE_MATERIAL_MAKE then -- 材料作坊生产材料N次（操作） datatype=43 datakind=0 needvalue=次数
		typename = Localization.text_quest( 98 )
	elseif datatype == QUEST_DATATYPE_HEROGUARD_UP then -- 御林卫上阵任意武将N个 datatype=44 datakind=0 needvalue=数量
		typename = Localization.text_quest( 95 )
	elseif datatype == QUEST_DATATYPE_HERO_VISIT then -- 进行良将寻访N次 datatype=45 datakind=0 needvalue=数量
		typename = Localization.text_quest( 95 )
	end
	return typename;
end

-- 任务名称
function QuestName( type, recvValue )
	local name = "";
	if type == 0 then
		name = name.."["..T(522).."]"
	elseif type == 1 then
		name = name.."["..T(521).."]"
	end
	
	local datatype = recvValue.m_datatype;
	local datakind = recvValue.m_datakind;
	local dataoffset = recvValue.m_dataoffset;
	local value = recvValue.m_value;
	local needvalue = recvValue.m_needvalue;
	if recvValue.m_nameid > 0 then
		name = Localization.text_quest( recvValue.m_nameid );
	elseif datatype == QUEST_DATATYPE_ACTOR_LEVEL then-- 主公等级 datatype=1 datakind=0 needvalue=等级
		name = name..FQUEST( 1, value );
	elseif datatype == QUEST_DATATYPE_BUILDING_LEVEL or datatype == QUEST_DATATYPE_BUILDING_UPGRADE then-- 建筑等级 datatype=2 datakind=建筑kind dataoffset=资源建筑编号 needvalue=建筑等级
		if dataoffset > 100 then
			name = name..FQUEST( 23, F(99, math.floor(dataoffset/100), BuildingName(datakind) ), F(99, math.floor(dataoffset%100), BuildingName(datakind) ), needvalue );
		else
			if datakind >= BUILDING_Silver and datakind <= BUILDING_Iron then
				name = name..FQUEST( 2, F(99, dataoffset, BuildingName(datakind) ), needvalue );
			else
				name = name..FQUEST( 2, BuildingName(datakind), needvalue );
			end
		end
	elseif datatype == QUEST_DATATYPE_BUILDING_SILVER then-- N个民居达到N级 datatype=3 datakind=等级 needvalue=建筑数量
		name = name..FQUEST( 3, value, needvalue, datakind );
	elseif datatype == QUEST_DATATYPE_BUILDING_WOOD then-- N个木场达到N级 datatype=4 datakind=等级 needvalue=建筑数量
		name = name..FQUEST( 4, value, needvalue, datakind );
	elseif datatype == QUEST_DATATYPE_BUILDING_FOOD then-- N个农田达到N级 datatype=5 datakind=等级 needvalue=建筑数量
		name = name..FQUEST( 5, value, needvalue, datakind);
	elseif datatype == QUEST_DATATYPE_BUILDING_IRON then-- N个矿场达到N级 datatype=6 datakind=等级 needvalue=建筑数量
		name = name..FQUEST( 6, value, needvalue, datakind );
	elseif datatype == QUEST_DATATYPE_LEVYNUM then-- 征收次数 datatype=7 datakind=0 needvalue=数量
		name = name..FQUEST( 7, value, needvalue );
	elseif datatype == QUEST_DATATYPE_HISTORY then-- 副本 datatype=8 datakind=副本编号 needvalue=1
		name = name..FQUEST( 8, StoryRankName( datakind ) );
	elseif datatype == QUEST_DATATYPE_KILLMONSTER then-- 打流寇N次 datatype=9 datakind=0 needvalue=数量
		name = name..FQUEST( 9, value, needvalue );
	elseif datatype == QUEST_DATATYPE_KILLMONSTER_LEVEL then-- 打流寇N级N次 datatype=10 datakind=等级 needvalue=数量
		name = name..FQUEST( 10, datakind, value, needvalue );
	elseif datatype == QUEST_DATATYPE_EQUIP_FORGING then-- 打造N装备N数量 datatype=11 datakind=装备kind needvalue=数量
		name = name..FQUEST( 11, EquipName(datakind), value, needvalue );
	elseif datatype == QUEST_DATATYPE_EQUIP_WASH then-- 装备洗炼次数 datatype=12 datakind=0 needvalue=数量
		name = name..FQUEST( 12, value, needvalue );
	elseif datatype == QUEST_DATATYPE_EQUIP_COLORWASH then-- 将N品质装备的属性洗炼至满级 datatype=13 datakind=颜色 needvalue=1
		name = name..FQUEST( 13, datakind );
	elseif datatype == QUEST_DATATYPE_TRAIN or datatype == QUEST_DATATYPE_TRAIN_OP then-- 募兵N兵种N数量 datatype=14 datakind=兵种（1，2，3） needvalue=数量
		name = name..FQUEST( 14, CorpsName(datakind-1), value, needvalue );
	elseif datatype == QUEST_DATATYPE_TRAINCOUNT or datatype == QUEST_DATATYPE_TRAINCOUNT_OP then-- 募兵N次 datatype=15 datakind=0 needvalue=数量
		name = name..FQUEST( 15, value, needvalue );
	elseif datatype == QUEST_DATATYPE_CITY_TECH then-- 研究N科技N级 datatype=16 datakind=科技ID needvalue=等级
		name = name..FQUEST( 16, TechName(datakind), value, needvalue );
	elseif datatype == QUEST_DATATYPE_CITY_TECHONE	 then-- 研究一次N科技 datatype=17 datakind=科技ID needvalue=1
		name = name..FQUEST( 17, TechName(datakind) );
	elseif datatype == QUEST_DATATYPE_HERO_WASHCOUNT then-- 武将洗髓N次 datatype=18 datakind=0 needvalue=次数
		name = name..FQUEST( 18, value, needvalue );
	elseif datatype == QUEST_DATATYPE_HERO_COLORWASH then-- 将一名N品质的武将属性洗满 datatype=19 datakind=颜色 needvalue=1
		name = name..FQUEST( 19, datakind );
	elseif datatype == QUEST_DATATYPE_HERO_CALL then-- 招募武将 datatype=20 datakind=kind needvalue=1
		name = name..FQUEST( 20, HeroName(datakind) );
	elseif datatype == QUEST_DATATYPE_WISHING then	-- 聚宝盆N次 datatype=21 datakind=0 needvalue=次数
		name = name..FQUEST( 21, value, needvalue );
	elseif datatype == QUEST_DATATYPE_CITYFIGHT then-- 成功击败N名敌国玩家 datatype=22 datakind=0 needvalue=次数
		name = name..FQUEST( 22, value, needvalue );
	elseif datatype == QUEST_DATATYPE_EQUIP_UP then-- 成功击败N名敌国玩家 datatype=22 datakind=0 needvalue=次数
		name = name..FQUEST( 24, HeroName(datakind), EquipName(dataoffset) );
	elseif datatype == QUEST_DATATYPE_BRANCH_QUEST then -- 完成N个支线任务
		name = name..FQUEST( 25, value, needvalue );
	elseif datatype == QUEST_DATATYPE_EQUIP_FORGING_QUICK then -- 进行一次铁匠铺加速打造 datatype=38 datakind=0 needvalue=1
		name = name..FQUEST( 26, value, needvalue );
	elseif datatype == QUEST_DATATYPE_HERO_UP then -- 前往聚贤馆让N上阵 datatype=39 datakind=武将kind needvalue=1
		name = name..FQUEST( 27, HeroName(datakind) );
	elseif datatype == QUEST_DATATYPE_SMITHYOFFHIRE	then -- 铁匠铺内雇佣N级或更高级铁匠 datatype=40 datakind=0 needvalue=等级
		name = name..FQUEST( 28, needvalue );
	elseif datatype == QUEST_DATATYPE_NEQUIP_FORGING then -- 打造国器N datatype=41 datakind=国器kind needvalue=1
		name = name..FQUEST( 29, NationEquipName(datakind) );
	elseif datatype == QUEST_DATATYPE_NEQUIP_LEVELNUM then -- 国器升级N次 datatype=42 datakind=0 needvalue=次数
		name = name..FQUEST( 30, value, needvalue );
	elseif datatype == QUEST_DATATYPE_MATERIAL_MAKE then -- 材料作坊生产材料N次（操作） datatype=43 datakind=0 needvalue=次数
		name = name..FQUEST( 31, value, needvalue );
	elseif datatype == QUEST_DATATYPE_HEROGUARD_UP then -- 御林卫上阵任意武将N个 datatype=44 datakind=0 needvalue=数量
		name = name..FQUEST( 32, value, needvalue );
	elseif datatype == QUEST_DATATYPE_HERO_VISIT then -- 进行良将寻访N次 datatype=45 datakind=0 needvalue=数量
		name = name..FQUEST( 33, value, needvalue );
	else
		name = ""
	end
	return name;
end

-- 跳转
function QuestGoto( index )
	if CacheQuest == nil or CacheQuest.m_list[index] == nil then
		return
	end

	local datatype = CacheQuest.m_list[index].m_datatype;
	local datakind = CacheQuest.m_list[index].m_datakind;
	local dataoffset = CacheQuest.m_list[index].m_dataoffset;
	local value = CacheQuest.m_list[index].m_value;
	local needvalue = CacheQuest.m_list[index].m_needvalue;
	if datatype == QUEST_DATATYPE_ACTOR_LEVEL then
	elseif datatype == QUEST_DATATYPE_BUILDING_LEVEL or datatype == QUEST_DATATYPE_BUILDING_UPGRADE then-- 建筑等级 datatype=2 datakind=建筑kind dataoffset=资源建筑编号 needvalue=建筑等级
				
		if dataoffset > 100 then
			local offset = 0;
			if datakind == BUILDING_Silver then
				offset = GetPlayer():BuildingResMinLevel( BUILDING_Silver );
			elseif datakind == BUILDING_Wood then
				offset = GetPlayer():BuildingResMinLevel( BUILDING_Wood );
			elseif datakind == BUILDING_Food then
				offset = GetPlayer():BuildingResMinLevel( BUILDING_Food );
			elseif datakind == BUILDING_Iron then
				offset = GetPlayer():BuildingResMinLevel( BUILDING_Iron );
			end
			City.Move( datakind, offset, true )
		else
			local offset = 0;
			if datakind == BUILDING_Silver then
				offset = dataoffset-1;
			elseif datakind == BUILDING_Wood then
				offset = dataoffset+16-1;
			elseif datakind == BUILDING_Food then
				offset = dataoffset+32-1;
			elseif datakind == BUILDING_Iron then
				offset = dataoffset+48-1;
			end
			City.Move( datakind, offset, true )
		end
		
	elseif datatype == QUEST_DATATYPE_BUILDING_SILVER then-- N个民居达到N级 datatype=3 datakind=等级 needvalue=建筑数量
		local offset =  GetPlayer():BuildingResMinLevel( BUILDING_Silver );
		if offset >= 0 then
			City.Move( BUILDING_Silver, offset, true )
		end
		
	elseif datatype == QUEST_DATATYPE_BUILDING_WOOD then-- N个木场达到N级 datatype=4 datakind=等级 needvalue=建筑数量
		local offset =  GetPlayer():BuildingResMinLevel( BUILDING_Wood );
		if offset >= 0 then
			City.Move( BUILDING_Wood, offset, true )
		end
		
	elseif datatype == QUEST_DATATYPE_BUILDING_FOOD then-- N个农田达到N级 datatype=5 datakind=等级 needvalue=建筑数量
		local offset =  GetPlayer():BuildingResMinLevel( BUILDING_Food );
		if offset >= 0 then
			City.Move( BUILDING_Food, offset, true )
		end
		
	elseif datatype == QUEST_DATATYPE_BUILDING_IRON then-- N个矿场达到N级 datatype=6 datakind=等级 needvalue=建筑数量
		local offset =  GetPlayer():BuildingResMinLevel( BUILDING_Iron );
		if offset >= 0 then
			City.Move( BUILDING_Iron, offset, true )
		end
		
	elseif datatype == QUEST_DATATYPE_LEVYNUM then-- 征收次数 datatype=7 datakind=0 needvalue=数量
		City.Move( BUILDING_Main, 0, true )
	
	elseif datatype == QUEST_DATATYPE_HISTORY then-- 副本 datatype=8 datakind=副本编号 needvalue=1
		StoryDlgShow()
		
	elseif datatype == QUEST_DATATYPE_KILLMONSTER then-- 打流寇N次 datatype=9 datakind=0 needvalue=数量
		if GameManager.currentScence == "city" then
			WorldMap.GotoWorldMap(-1, -1)
		end
			
	elseif datatype == QUEST_DATATYPE_KILLMONSTER_LEVEL then-- 打流寇N级N次 datatype=10 datakind=等级 needvalue=数量
		if GameManager.currentScence == "city" then
			WorldMap.GotoWorldMap(-1, -1)
		end
	
	elseif datatype == QUEST_DATATYPE_EQUIP_FORGING then-- 打造N装备N数量 datatype=11 datakind=装备kind needvalue=数量
		City.Move( BUILDING_Smithy, -1, true );
	
		
	elseif datatype == QUEST_DATATYPE_EQUIP_WASH then-- 装备洗炼次数 datatype=12 datakind=0 needvalue=数量
		City.Move( BUILDING_Wash, -1, true );
		
	elseif datatype == QUEST_DATATYPE_EQUIP_COLORWASH then-- 将N品质装备的属性洗炼至满级 datatype=13 datakind=颜色 needvalue=1
		City.Move( BUILDING_Wash, -1, true );
		
	elseif datatype == QUEST_DATATYPE_TRAIN or datatype == QUEST_DATATYPE_TRAIN_OP then-- 募兵N兵种N数量 datatype=14 datakind=兵种（1，2，3） needvalue=数量
		if datakind == 1 then
			City.Move( BUILDING_Infantry, -1, true );
		elseif datakind == 1 then
			City.Move( BUILDING_Cavalry, -1, true );
		elseif datakind == 1 then
			City.Move( BUILDING_Archer, -1, true );
		end
		
	elseif datatype == QUEST_DATATYPE_TRAINCOUNT or datatype == QUEST_DATATYPE_TRAINCOUNT_OP then-- 募兵N次 datatype=15 datakind=0 needvalue=数量
		City.Move( BUILDING_Infantry, -1, true );
		
	elseif datatype == QUEST_DATATYPE_CITY_TECH then-- 研究N科技N级 datatype=16 datakind=科技ID needvalue=等级
		City.Move( BUILDING_Tech, -1, true );
		
	elseif datatype == QUEST_DATATYPE_CITY_TECHONE	 then-- 研究一次N科技 datatype=17 datakind=科技ID needvalue=1
		City.Move( BUILDING_Tech, -1, true );
		
	elseif datatype == QUEST_DATATYPE_HERO_WASHCOUNT then-- 武将洗髓N次 datatype=18 datakind=0 needvalue=次数
		
		
	elseif datatype == QUEST_DATATYPE_HERO_COLORWASH then-- 将一名N品质的武将属性洗满 datatype=19 datakind=颜色 needvalue=1
		
	elseif datatype == QUEST_DATATYPE_HERO_CALL then-- 招募N名武将 datatype=20 datakind=0 needvalue=数量
		City.Move( BUILDING_Hero, -1, true );
		
	elseif datatype == QUEST_DATATYPE_WISHING then	-- 聚宝盆N次 datatype=21 datakind=0 needvalue=次数
		City.Move( BUILDING_Wishing, -1, true );
		
	elseif datatype == QUEST_DATATYPE_CITYFIGHT then-- 成功击败N名敌国玩家 datatype=22 datakind=0 needvalue=次数
	
	elseif datatype == QUEST_DATATYPE_EQUIP_UP then-- 给N武将穿N装备 datatype=33 datakind=武将kind dataoffset=装备编号
		HeroDlgShow()
		
	elseif datatype == QUEST_DATATYPE_BRANCH_QUEST then -- 完成N个支线任务
	
	elseif datatype == QUEST_DATATYPE_EQUIP_FORGING_QUICK then -- 进行一次铁匠铺加速打造 datatype=38 datakind=0 needvalue=1
		City.Move( BUILDING_Smithy, -1, true );
	
	elseif datatype == QUEST_DATATYPE_HERO_UP then -- 前往聚贤馆让N上阵 datatype=39 datakind=武将kind needvalue=1
		City.Move( BUILDING_Hero, -1, true );
		
	elseif datatype == QUEST_DATATYPE_NEQUIP_FORGING then -- 打造国器N datatype=41 datakind=国器kind needvalue=1
		NationEquipDlgShow()
		
	elseif datatype == QUEST_DATATYPE_NEQUIP_LEVELNUM then -- 国器升级N次 datatype=42 datakind=0 needvalue=次数
		NationEquipDlgShow()
		
	elseif datatype == QUEST_DATATYPE_MATERIAL_MAKE then -- 材料作坊生产材料N次（操作） datatype=43 datakind=0 needvalue=次数
		City.Move( BUILDING_Craftsman, -1, true );
		
	elseif datatype == QUEST_DATATYPE_HEROGUARD_UP then -- 御林卫上阵任意武将N个 datatype=44 datakind=0 needvalue=数量
		City.Move( BUILDING_Cabinet, -1, true );
		
	elseif datatype == QUEST_DATATYPE_HERO_VISIT then -- 进行良将寻访N次 datatype=45 datakind=0 needvalue=数量
		City.Move( BUILDING_Hero, -1, true );
	end
end

-- 点击类型任务
function QuestClickCheck( datakind )
	if CacheQuest.m_list[1] == nil then
		return
	end
	if CacheQuest.m_list[1].m_datatype ~= QUEST_DATATYPE_CLICK then
		return
	end
	if CacheQuest.m_list[1].m_datakind ~= datakind then
		return
	end
	system_askinfo( ASKINFO_QUEST, "", 2, CacheQuest.m_list[1].m_questid, datakind );
end

-- 请求一段对话
function QuestTalkAsk( talkid )
	local sendValue = {};
	sendValue.m_talkid = talkid;
	sendValue.m_type = 1;
	netsend_questtalknext_C( sendValue )
end
