-- 任务类型
QUEST_DATAINDEX_ACTOR_LEVEL			=1	-- 主公等级 datatype=1 datakind=0 needvalue=等级
QUEST_DATAINDEX_BUILDING_LEVEL		=2	-- 建筑等级 datatype=2 datakind=建筑kind dataoffset=资源建筑编号 needvalue=建筑等级
QUEST_DATAINDEX_BUILDING_SILVER		=3	-- N个民居达到N级 datatype=3 datakind=等级 needvalue=建筑数量
QUEST_DATAINDEX_BUILDING_WOOD		=4	-- N个木场达到N级 datatype=4 datakind=等级 needvalue=建筑数量
QUEST_DATAINDEX_BUILDING_FOOD		=5	-- N个农田达到N级 datatype=5 datakind=等级 needvalue=建筑数量
QUEST_DATAINDEX_BUILDING_IRON		=6	-- N个矿场达到N级 datatype=6 datakind=等级 needvalue=建筑数量
QUEST_DATAINDEX_LEVYNUM				=7	-- 征收次数 datatype=7 datakind=0 needvalue=数量
QUEST_DATAINDEX_HISTORY				=8	-- 副本 datatype=8 datakind=副本编号 needvalue=1
QUEST_DATAINDEX_KILLMONSTER			=9	-- 打流寇N次 datatype=9 datakind=0 needvalue=数量
QUEST_DATAINDEX_KILLMONSTER_LEVEL	=10	-- 打流寇N级N次 datatype=10 datakind=等级 needvalue=数量
QUEST_DATAINDEX_EQUIP_FORGING		=11	-- 打造N装备N数量 datatype=11 datakind=装备kind needvalue=数量
QUEST_DATAINDEX_EQUIP_WASH			=12	-- 装备洗炼次数 datatype=12 datakind=0 needvalue=数量
QUEST_DATAINDEX_EQUIP_COLORWASH		=13	-- 将N品质装备的属性洗炼至满级 datatype=13 datakind=颜色 needvalue=1
QUEST_DATAINDEX_TRAIN				=14	-- 募兵N兵种N数量 datatype=14 datakind=兵种（1，2，3） needvalue=数量
QUEST_DATAINDEX_TRAINCOUNT			=15	-- 募兵N次 datatype=15 datakind=0 needvalue=数量
QUEST_DATAINDEX_CITY_TECH			=16	-- 研究N科技N级 datatype=16 datakind=科技ID needvalue=等级
QUEST_DATAINDEX_CITY_TECHONE		=17	-- 研究一次N科技 datatype=17 datakind=科技ID needvalue=1
QUEST_DATAINDEX_HERO_WASHCOUNT		=18	-- 武将洗髓N次 datatype=18 datakind=0 needvalue=次数
QUEST_DATAINDEX_HERO_COLORWASH		=19	-- 将一名N品质的武将属性洗满 datatype=19 datakind=颜色 needvalue=1
QUEST_DATAINDEX_HERO_CALL			=20	-- 招募N名武将 datatype=20 datakind=0 needvalue=数量
QUEST_DATAINDEX_WISHING				=21	-- 聚宝盆N次 datatype=21 datakind=0 needvalue=次数
QUEST_DATAINDEX_CITYFIGHT			=22	-- 成功击败N名敌国玩家 datatype=22 datakind=0 needvalue=次数
QUEST_DATAINDEX_CREATENAME			=50	-- 创建名称

-- 任务缓存
CacheQuest = {};
function CacheQuestClear()
	CacheQuest = {};
end

-- m_count=0,m_list={m_questid=0,m_flag=0,m_datatype=0,m_datakind=0,m_dataoffset=0,m_value=0,m_needvalue=0,m_awardkind={[5]},m_awardnum={[5]},m_nameid=0,[m_count]},
function CacheQuestSet( recvValue )
	CacheQuest = recvValue;
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
	if datatype == QUEST_DATAINDEX_ACTOR_LEVEL then-- 主公等级 datatype=1 datakind=0 needvalue=等级
		name = name..FQUEST( 1, value );
	elseif datatype == QUEST_DATAINDEX_BUILDING_LEVEL then-- 建筑等级 datatype=2 datakind=建筑kind dataoffset=资源建筑编号 needvalue=建筑等级
		name = name..FQUEST( 2, F(99, dataoffset+1, BuildingName(datakind) ), needvalue );
	elseif datatype == QUEST_DATAINDEX_BUILDING_SILVER then-- N个民居达到N级 datatype=3 datakind=等级 needvalue=建筑数量
		name = name..FQUEST( 3, value, needvalue );
	elseif datatype == QUEST_DATAINDEX_BUILDING_WOOD then-- N个木场达到N级 datatype=4 datakind=等级 needvalue=建筑数量
		name = name..FQUEST( 4, value, needvalue );
	elseif datatype == QUEST_DATAINDEX_BUILDING_FOOD then-- N个农田达到N级 datatype=5 datakind=等级 needvalue=建筑数量
		name = name..FQUEST( 5, value, needvalue );
	elseif datatype == QUEST_DATAINDEX_BUILDING_IRON then-- N个矿场达到N级 datatype=6 datakind=等级 needvalue=建筑数量
		name = name..FQUEST( 6, value, needvalue );
	elseif datatype == QUEST_DATAINDEX_LEVYNUM then-- 征收次数 datatype=7 datakind=0 needvalue=数量
		name = name..FQUEST( 7, value, needvalue );
	elseif datatype == QUEST_DATAINDEX_HISTORY then-- 副本 datatype=8 datakind=副本编号 needvalue=1
		name = name..FQUEST( 8, datakind );
	elseif datatype == QUEST_DATAINDEX_KILLMONSTER then-- 打流寇N次 datatype=9 datakind=0 needvalue=数量
		name = name..FQUEST( 9, value, needvalue );
	elseif datatype == QUEST_DATAINDEX_KILLMONSTER_LEVEL then-- 打流寇N级N次 datatype=10 datakind=等级 needvalue=数量
		name = name..FQUEST( 10, datakind, value, needvalue );
	elseif datatype == QUEST_DATAINDEX_EQUIP_FORGING then-- 打造N装备N数量 datatype=11 datakind=装备kind needvalue=数量
		name = name..FQUEST( 11, EquipName(datakind), value, needvalue );
	elseif datatype == QUEST_DATAINDEX_EQUIP_WASH then-- 装备洗炼次数 datatype=12 datakind=0 needvalue=数量
		name = name..FQUEST( 12, value, needvalue );
	elseif datatype == QUEST_DATAINDEX_EQUIP_COLORWASH then-- 将N品质装备的属性洗炼至满级 datatype=13 datakind=颜色 needvalue=1
		name = name..FQUEST( 13, datakind );
	elseif datatype == QUEST_DATAINDEX_TRAIN then-- 募兵N兵种N数量 datatype=14 datakind=兵种（1，2，3） needvalue=数量
		name = name..FQUEST( 14, CorpsName(datakind-1), value, needvalue );
	elseif datatype == QUEST_DATAINDEX_TRAINCOUNT then-- 募兵N次 datatype=15 datakind=0 needvalue=数量
		name = name..FQUEST( 15, value, needvalue );
	elseif datatype == QUEST_DATAINDEX_CITY_TECH then-- 研究N科技N级 datatype=16 datakind=科技ID needvalue=等级
		name = name..FQUEST( 16, TechName(datakind), value, needvalue );
	elseif datatype == QUEST_DATAINDEX_CITY_TECHONE	 then-- 研究一次N科技 datatype=17 datakind=科技ID needvalue=1
		name = name..FQUEST( 17, TechName(datakind) );
	elseif datatype == QUEST_DATAINDEX_HERO_WASHCOUNT then-- 武将洗髓N次 datatype=18 datakind=0 needvalue=次数
		name = name..FQUEST( 18, value, needvalue );
	elseif datatype == QUEST_DATAINDEX_HERO_COLORWASH then-- 将一名N品质的武将属性洗满 datatype=19 datakind=颜色 needvalue=1
		name = name..FQUEST( 19, datakind );
	elseif datatype == QUEST_DATAINDEX_HERO_CALL then-- 招募N名武将 datatype=20 datakind=0 needvalue=数量
		name = name..FQUEST( 20, value, needvalue );
	elseif datatype == QUEST_DATAINDEX_WISHING then	-- 聚宝盆N次 datatype=21 datakind=0 needvalue=次数
		name = name..FQUEST( 21, value, needvalue );
	elseif datatype == QUEST_DATAINDEX_CITYFIGHT then-- 成功击败N名敌国玩家 datatype=22 datakind=0 needvalue=次数
		name = name..FQUEST( 22, value, needvalue );
	else
		name = Localization.text_quest( recvValue.m_nameid );
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

	if datatype == QUEST_DATAINDEX_ACTOR_LEVEL then
	elseif datatype == QUEST_DATAINDEX_BUILDING_LEVEL then-- 建筑等级 datatype=2 datakind=建筑kind dataoffset=资源建筑编号 needvalue=建筑等级
		local offset = 0;
		if datakind == BUILDING_Silver then
			offset = dataoffset;
		elseif datakind == BUILDING_Wood then
			offset = dataoffset+16;
		elseif datakind == BUILDING_Food then
			offset = dataoffset+32;
		elseif datakind == BUILDING_Iron then
			offset = dataoffset+48;
		end
		City.Move( datakind, offset, true )
		
	elseif datatype == QUEST_DATAINDEX_BUILDING_SILVER then-- N个民居达到N级 datatype=3 datakind=等级 needvalue=建筑数量
		local offset =  GetPlayer():BuildingResMinLevel( BUILDING_Silver );
		if offset >= 0 then
			City.Move( BUILDING_Silver, offset, true )
		end
		
	elseif datatype == QUEST_DATAINDEX_BUILDING_WOOD then-- N个木场达到N级 datatype=4 datakind=等级 needvalue=建筑数量
		local offset =  GetPlayer():BuildingResMinLevel( BUILDING_Wood );
		if offset >= 0 then
			City.Move( BUILDING_Wood, dataoffset, true )
		end
		
	elseif datatype == QUEST_DATAINDEX_BUILDING_FOOD then-- N个农田达到N级 datatype=5 datakind=等级 needvalue=建筑数量
		local offset =  GetPlayer():BuildingResMinLevel( BUILDING_Food );
		if offset >= 0 then
			City.Move( BUILDING_Food, dataoffset, true )
		end
		
	elseif datatype == QUEST_DATAINDEX_BUILDING_IRON then-- N个矿场达到N级 datatype=6 datakind=等级 needvalue=建筑数量
		local offset =  GetPlayer():BuildingResMinLevel( BUILDING_Iron );
		if offset >= 0 then
			City.Move( BUILDING_Iron, dataoffset, true )
		end
		
	elseif datatype == QUEST_DATAINDEX_LEVYNUM then-- 征收次数 datatype=7 datakind=0 needvalue=数量
		
	end
end
