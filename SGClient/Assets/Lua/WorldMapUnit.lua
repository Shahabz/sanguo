-- 地图显示单元

-- 显示单元类型
MAPUNIT_TYPE_CITY		=	1	-- 城池
MAPUNIT_TYPE_ARMY		=	2	-- 部队
MAPUNIT_TYPE_TOWN		=	3	-- 城镇
MAPUNIT_TYPE_ENEMY		=	4	-- 流寇
MAPUNIT_TYPE_RES		=	5	-- 资源点
MAPUNIT_TYPE_EVENT		=	6	-- 随机事件
MAPUNIT_TYPE_KINGWAR_TOWN	= 10-- 皇城血战据点

MAPUNIT_TYPE_TOWN_TYPE1	= 1
MAPUNIT_TYPE_TOWN_TYPE2	= 2
MAPUNIT_TYPE_TOWN_TYPE3	= 3
MAPUNIT_TYPE_TOWN_TYPE4	= 4
MAPUNIT_TYPE_TOWN_TYPE5	= 5
MAPUNIT_TYPE_TOWN_TYPE6	= 6
MAPUNIT_TYPE_TOWN_TYPE7	= 7 -- 名城
MAPUNIT_TYPE_TOWN_TYPE8	= 8 -- 都城
MAPUNIT_TYPE_TOWN_TYPE9	= 9 -- 皇城

-- 城池状态
CITY_STATE_FIRE			=	0x01	-- 着火中
CITY_STATE_ARMYGROUP	=	0x02	-- 被城战中

-- 单只部队状态
ARMY_STATE_IDLE				=	0	-- 空闲
ARMY_STATE_GROUP_END		=	3	-- 集结完毕
ARMY_STATE_READY            =   4   -- 准备中
ARMY_STATE_MARCH			=	10	-- 行军中
ARMY_STATE_FIGHT			=	20	-- 战斗中
ARMY_STATE_OCCUPY			=	21	-- 驻扎中
ARMY_STATE_GATHER			=	22	-- 采集中
ARMY_STATE_HELP				=	24	-- 援助中
ARMY_STATE_REBACK			=	30	-- 返程中
ARMY_STATE_KINGWAR_READY	=	40	-- 皇城战已到集结点
ARMY_STATE_KINGWAR_FIGHT	=	41	-- 皇城战排队战斗中
ARMY_STATE_KINGWAR_WAITSOS	=	42	-- 皇城战死亡等待救援中
ARMY_STATE_KINGWAR_DEAD		=	43	-- 皇城战战斗已经死亡

-- 行为
ARMY_ACTION_IDLE			=	0	-- 空闲
ARMY_ACTION_FIGHT			=	1	-- 去战斗
ARMY_ACTION_OCCUPY			=	2	-- 去驻扎
ARMY_ACTION_GATHER			=	3	-- 去采集
ARMY_ACTION_HELP_TROOP		=	6	-- 去士兵援助
ARMY_ACTION_GROUP_CREATE	=	10	-- 创建集结
ARMY_ACTION_GROUP_ATTACK	=	11	-- 参与集结(攻击方)
ARMY_ACTION_GROUP_DEFENSE	=	12	-- 参与集结(防御方)
ARMY_ACTION_NATION_ATTACK	=	13	-- 参与国战(攻击方)
ARMY_ACTION_NATION_DEFENSE	=	14	-- 参与国战(防御方)
ARMY_ACTION_KINGWAR			=	20	-- 皇城战

ARMY_REBACK_NORMAL			=	0	-- 返程类型-正常
ARMY_REBACK_RETURN			=	1	-- 返程类型-操作返回
ARMY_REBACK_CALLBACK		=	2	-- 返程类型-操作召回
ARMY_REBACK_FIGHTWIN		=	3	-- 返程类型-战斗胜利返回
ARMY_REBACK_FIGHTLOSE		=	4	-- 返程类型-战斗失败打回
ARMY_REBACK_REPATRIAT		=	5	-- 返程类型-他人遣回

ENEMY_ACTION_IDLE			=	0	-- 无
ENEMY_ACTION_ATTACK			=	1	-- 攻击
ENEMY_ACTION_DEAD			=	2	-- 死亡

MAPTOWN_STATE_PROTECT		=	1	-- 保护状态
MAPTOWN_STATE_NOPROTECT		=	2	-- 破防状态
MAPTOWN_STATE_TEMPOCCUPY	=	3	-- 临时占领状态
MAPTOWN_STATE_OCCUPY		=	4	-- 占领状态

KINGWAR_STATE_NORMAL		=   0	-- 未开启状态
KINGWAR_STATE_START			=   1	-- 开启状态
KINGWAR_STATE_OFFICIAL      =   2   -- 封官状态

-- Prefab
local MapUnitCity 		= nil;
local MapUnitArmy 		= nil;
local MapUnitTown 		= nil;
local MapUnitEnemy 		= nil;
local MapUnitRes 		= nil;
local MapUnitEvent 		= nil;
local MapBorder			= nil;
local MapTownRange		= nil;
local MapUnitInited		= false;

-- 玩家每等级的城市外观
MapUnitCityShapeList = {
[0] = "mapunit_city_level1",
[1] = "mapunit_city_level1",
[2] = "mapunit_city_level1",
[3] = "mapunit_city_level1",
[4] = "mapunit_city_level1",
[5] = "mapunit_city_level1",
[6] = "mapunit_city_level1",
[7] = "mapunit_city_level1",
[8] = "mapunit_city_level1",
[9] = "mapunit_city_level2",
[10] = "mapunit_city_level2",
[11] = "mapunit_city_level2",
[12] = "mapunit_city_level2",
[13] = "mapunit_city_level2",
[14] = "mapunit_city_level2",
[15] = "mapunit_city_level2",
[16] = "mapunit_city_level2",
[17] = "mapunit_city_level3",
[18] = "mapunit_city_level3",
[19] = "mapunit_city_level3",
[20] = "mapunit_city_level3",
[21] = "mapunit_city_level3",
[22] = "mapunit_city_level3",
[23] = "mapunit_city_level3",
[24] = "mapunit_city_level3",
[25] = "mapunit_city_level4",
[26] = "mapunit_city_level4",
[27] = "mapunit_city_level4",
[28] = "mapunit_city_level4",
[29] = "mapunit_city_level4",
[30] = "mapunit_city_level4",
}

-- 城镇形象
MapUnitTownShapeList = {
[1] = "mapunit_type1",
[2] = "mapunit_type2",
[3] = "mapunit_type3",
[4] = "mapunit_type4",
[5] = "mapunit_type5",
[6] = "mapunit_type6",
[7] = "mapunit_type7",
[8] = "mapunit_type8",
[9] = "mapunit_type9",
}

-- 资源点形象
MapUnitResShapeList = {
[1] = "mapunit_res_silver",
[2] = "mapunit_res_wood",
[3] = "mapunit_res_food",
[4] = "mapunit_res_iron",
[5] = "mapunit_res_token",
}

-- 事件形象
MapUnitEventShapeList = {
[1] = "mapunit_event_1",
[2] = "mapunit_event_2",
[3] = "mapunit_event_3",
[4] = "mapunit_event_4",
[5] = "mapunit_event_5",
}

-- 资源点名称
MapUnitResNameList = {
[1] = 171,
[2] = 172,
[3] = 173,
[4] = 174,
[5] = 175,
}

-- 事件名称
MapUnitEventNameList = {
[1] = 1010,
[2] = 1011,
[3] = 1012,
[4] = 1013,
[5] = 1014,
}

-- 范围颜色
MapUnitRangeColor = {
[0] = 0xecc244ff,
[1] = 0x25c9ffff,
[2] = 0xa01515ff,
[3] = 0x03de27ff,
}

-- 范围颜色
MapUnitRangeColorA = {
[0] = 0xecc24432,
[1] = 0x25c9ff32,
[2] = 0xa0151532,
[3] = 0x03de2732,
}

-- 军队状态名称
MapUnitArmyStateNameList = {
[ARMY_STATE_READY]              = 2779,
[ARMY_STATE_MARCH]              = 2732,
[ARMY_STATE_FIGHT]              = 2733,
[ARMY_STATE_OCCUPY]             = 2734,
[ARMY_STATE_GATHER]             = 2735,
[ARMY_STATE_REBACK]             = 2737,
[ARMY_STATE_HELP]               = 2738,
}

-- 显示单元集合
MapUnit = {};

-- 地图对象的根节点
MapUnit.unitRoot = nil;

-- 当前显示的单元对象缓存
MapUnit.cache = {};

-- 显示单元对象缓存池
-- 已经创建的，删除时不删，隐藏，下次无需创建直接拿来用
MapUnit.objectPoolCity 			= {}; 	-- 城池
MapUnit.objectPoolArmy 			= {}; 	-- 部队
MapUnit.objectPoolTown 			= {}; 	-- 城镇
MapUnit.objectPoolEnemy 		= {}; 	-- 流寇
MapUnit.objectPoolRes 			= {}; 	-- 资源田
MapUnit.objectPoolEvent 		= {}; 	-- 事件
MapUnit.objectCenterTownRange 	= {}; 	-- 皇城区域范围
MapUnit.SpritePoolEnemy			= {};	-- 流寇形象
MapUnit.SpritePoolRes			= {};	-- 资源田形象

-- 初始化
function MapUnit.init()
	if MapUnitInited == false then
		-- 预置的初始化
		MapUnitCity 		= LoadPrefab("MapUnitCity");
		MapUnitArmy 		= LoadPrefab("MapUnitArmy");
		MapUnitTown 		= LoadPrefab("MapUnitTown");
		MapUnitEnemy 		= LoadPrefab("MapUnitEnemy");
		MapUnitRes 			= LoadPrefab("MapUnitRes");
		MapUnitEvent		= LoadPrefab("MapUnitEvent");
		MapBorder			= LoadPrefab("MapBorder");
		MapTownRange		= LoadPrefab("MapTownRange");
		
		-- 流寇资源
		for k, v in pairs( g_enemyinfo ) do
			if MapUnit.SpritePoolEnemy[v.shape] == nil or #MapUnit.SpritePoolEnemy[v.shape] == 0 then
				MapUnit.SpritePoolEnemy[v.shape] = {}
				MapUnit.SpritePoolEnemy[v.shape][1] = EnemySprite( v.shape, 1 )
				MapUnit.SpritePoolEnemy[v.shape][2] = EnemySprite( v.shape, 2 )
				MapUnit.SpritePoolEnemy[v.shape][3] = EnemySprite( v.shape, 3 )
			end	
		end
		
		-- 资源田形象 g_resinfo
		for k, v in pairs( g_resinfo ) do
			if MapUnit.SpritePoolRes[v.type] == nil then
				MapUnit.SpritePoolRes[v.type] = LoadSprite( MapUnitResShapeList[v.type] )
			end
		end
		
		MapUnitInited 	= true;
	end
end

-- 清除
function MapUnit.clear()
	MapUnitCity 		= nil;
	MapUnitArmy 		= nil;
	MapUnitTown 		= nil;
	MapUnitEnemy 		= nil;
	MapUnitRes 			= nil;
	MapUnitEvent 		= nil;
	
	MapUnitInited		= false;
	MapUnit.unitRoot 	= nil;
	
	MapUnit.objectPoolCity 		= {};
	MapUnit.objectPoolArmy		= {};
	MapUnit.objectPoolTown 		= {};
	MapUnit.objectPoolEnemy 	= {};
	MapUnit.objectPoolRes 		= {};
	MapUnit.objectPoolEvent 	= {};
	MapUnit.SpritePoolEnemy		= {};
	MapUnit.SpritePoolRes		= {};
	
	MapUnit.cache 				= {};
	MapUnit.objectCenterTownRange = {};
end

-- 添加显示单元
function MapUnit.add( unitRoot, recvValue )
	local unit = nil;
	MapUnit.unitRoot = unitRoot;

	-- 城池
	if recvValue.m_type == MAPUNIT_TYPE_CITY then
		unit = MapUnit.createCity( recvValue );
		
	-- 部队
	elseif recvValue.m_type == MAPUNIT_TYPE_ARMY then
		unit = MapUnit.createArmy( recvValue )
	
	-- 城镇
	elseif recvValue.m_type == MAPUNIT_TYPE_TOWN then
		unit = MapUnit.createTown( recvValue );
		
	-- 流寇	
	elseif recvValue.m_type == MAPUNIT_TYPE_ENEMY then
		unit = MapUnit.createEnemy( recvValue );
		
	-- 资源
	elseif recvValue.m_type == MAPUNIT_TYPE_RES then
		unit = MapUnit.createRes( recvValue );
	
	-- 随机事件
	elseif recvValue.m_type == MAPUNIT_TYPE_EVENT then
		unit = MapUnit.createEvent( recvValue );				
	end
	
	-- 缓存起来，以便删除
	if unit ~= nil then
		unit.gameObject:SetActive( true );
		MapUnit.cache[recvValue.m_unit_index] = unit;
	end

    return unit;
end

-- 删除显示单元
function MapUnit.del( unit_index )
	local unit = MapUnit.cache[unit_index];
	if unit ~= nil then
		unit.gameObject:SetActive( false );
		MapUnit.cache[unit_index] = nil;
	end
end

-- 创建城池
function MapUnit.createCity( recvValue )
	local state 	= recvValue.m_state;
	local name 		= recvValue.m_name;
	local posx 		= recvValue.m_posx;
	local posy 		= recvValue.m_posy;
	local level 	= recvValue.m_char_value[1];
	local nation	= recvValue.m_char_value[2];
	local ptbuff	= recvValue.m_char_value[3];
	local unitindex = recvValue.m_unit_index;
		
	-- 先搜索缓存，如果缓存有，那么就更新
	local unitObj = MapUnit.cache[recvValue.m_unit_index];
	
	-- 先检查对象缓存池是否有空余的
	if unitObj == nil then
		for index, unit in pairs( MapUnit.objectPoolCity ) do
			if unit and unit.gameObject.activeSelf == false then
				unitObj = unit;
				break;
			end
		end
	end
	
	-- 没有空余的就新创建一个
	if unitObj == nil then
		unitObj = GameObject.Instantiate( MapUnitCity );
		unitObj.transform:SetParent( MapUnit.unitRoot );
		table.insert( MapUnit.objectPoolCity, unitObj );
	end
	
	-- 位置
	local cameraPosX, cameraPosY = WorldMap.ConvertGameToCamera( posx, posy );
	posx, posy = MapUnit.getGridTrans( MAPUNIT_TYPE_CITY, 0, cameraPosX, cameraPosY );
	unitObj.transform.localPosition = Vector3.New( posx, posy, posy );
	
	-- 获取引用
	local objs = unitObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiShape = objs[0];
	local uiName = objs[1];
	local uiRange = objs[2];
	local uiEffectProtect = objs[3];
	local uiArmyGroup = objs[4];
	
	-- 形象
    uiShape:GetComponent("SpriteRenderer").sprite = LoadSprite( MapUnitCityShapeList[level].."_"..nation );
			
	-- 名字
	uiName:GetComponent("UIText").text = "Lv."..level.." "..name

	-- 范围框
	if recvValue.m_unit_index == WorldMap.m_nMyCityUnitIndex then
		SetTrue( uiRange )
		uiRange.transform:GetComponent("SpriteRenderer").color = Hex2Color( MapUnitRangeColor[nation] )
		uiRange.transform.localScale = Vector3.New( 3, 3, 3 );
	else
		SetFalse( uiRange )
	end
	
	-- 保护BUFF
	if ptbuff > 0 then
		--SetTrue( uiEffectProtect )
	else
		--SetFalse( uiEffectProtect )
	end
	
	-- 着火
	if Utils.byteAndOp( recvValue.m_state, CITY_STATE_FIRE ) == CITY_STATE_FIRE then
	else	
	end
	
	-- 城战
	if Utils.byteAndOp( state, CITY_STATE_ARMYGROUP ) == CITY_STATE_ARMYGROUP then
		SetTrue( uiArmyGroup )
		uiArmyGroup.transform:GetComponent( typeof(UIButton) ).uiMod = CityArmyGroupClickMod;
		uiArmyGroup.transform:GetComponent( typeof(UIButton) ).controlID = unitindex;
	else
		SetFalse( uiArmyGroup )
	end
	
	return unitObj;
end

-- 创建部队
function MapUnit.createArmy( recvValue )
	local oldstate = 0;
	local cacheValue  = WorldMap.m_nMapUnitList[recvValue.m_unit_index];
	if cacheValue ~= nil then
		oldstate = cacheValue.m_state;
	end
	local state 	= recvValue.m_state;
	local posx 		= recvValue.m_posx;
	local posy 		= recvValue.m_posy;
	local action	= recvValue.m_short_value[5];
	local nation	= recvValue.m_char_value[5];
	local from_actorid = recvValue.m_int_value[4];
	local to_actorid = recvValue.m_int_value[5];
			
	-- 驻防玩家和被驻防玩家可看见驻防部队
	if state == ARMY_STATE_MARCH or state == ARMY_STATE_REBACK then
		if action == ARMY_ACTION_HELP_TROOP then
			if from_actorid ~= GetPlayer().m_actorid and to_actorid ~= GetPlayer().m_actorid then
				return
			end
		end
	end
	
	-- 先搜索缓存，如果缓存有，那么就更新
	local unitObj = MapUnit.cache[recvValue.m_unit_index];
	
	-- 先检查对象缓存池是否有空余的
	if unitObj == nil then
		for index, unit in pairs( MapUnit.objectPoolArmy ) do
			if unit and unit.gameObject.activeSelf == false then
				unitObj = unit;
				break;
			end
		end
	end
	
	-- 没有空余的就新创建一个
	if unitObj == nil then
		unitObj = GameObject.Instantiate( MapUnitArmy );
		unitObj.transform:SetParent( MapArmyRoot );
		table.insert( MapUnit.objectPoolArmy, unitObj );
	end
	
	-- 关联数据
	local ShareData = unitObj:GetComponent("ShareData");
	if ShareData then
		ShareData:AddValue( "unit_index", recvValue.m_unit_index );
	end
	
	-- 根据状态设置属性
	if state == ARMY_STATE_MARCH or state == ARMY_STATE_REBACK then
		local from_type 	= recvValue.m_char_value[1];
		local from_grid 	= recvValue.m_char_value[2];
		local from_posx 	= recvValue.m_short_value[1];
		local from_posy 	= recvValue.m_short_value[2];
		local to_type 		= recvValue.m_char_value[3];
		local to_grid 		= recvValue.m_char_value[4];
		local to_posx 		= recvValue.m_short_value[3];
		local to_posy		= recvValue.m_short_value[4];
		local army_index 	= recvValue.m_int_value[1];
		local move_time 	= recvValue.m_int_value[2];
		local move_needtime = recvValue.m_int_value[3];

		-- 武将
		local heroid = {};
		if recvValue.m_short_value_count >= 6 then
			for i=6,recvValue.m_short_value_count,1 do
				table.insert( heroid, recvValue.m_short_value[i] )
			end
		end
		
		-- 出发点
		local cameraPosX, cameraPosY = WorldMap.ConvertGameToCamera( from_posx, from_posy );
		local fposx, fposy = MapUnit.getGridTrans( from_type, from_grid, cameraPosX, cameraPosY );

		-- 目的点
		cameraPosX, cameraPosY = WorldMap.ConvertGameToCamera( to_posx, to_posy );
		local tposx, tposy = MapUnit.getGridTrans( to_type, to_grid, cameraPosX, cameraPosY );
		
		-- 总距离(不是计算行军时间的距离，这是两点之间的直线距离)
		local distance = math.sqrt( math.pow( ( tposx - fposx ), 2 ) + math.pow( ( tposy - fposy ), 2) );

		-- 计算客户端需要的速度
		local speed = distance/move_needtime;

		-- 当前位置
		local movelen = speed * move_time;
        local x = ( ( tposx - fposx ) * movelen ) / distance + fposx;
        local y = ( ( tposy - fposy ) * movelen ) / distance + fposy;
		unitObj.transform.localPosition = Vector3.New( x, y, WORLDMAP_ZORDER_ARMY );
		
		-- 当前方向
		local direction = Vector3.New( tposx - fposx, tposy - fposy, 0 );

		-- 开始移动
		local moveAttr = unitObj:GetComponent("ArmyMove");
		moveAttr.stat = 1;
		moveAttr.speed = speed;
		moveAttr.fromPosition = Vector3.New( fposx, fposy, WORLDMAP_ZORDER_ARMY );
		moveAttr.toPosition = Vector3.New( tposx, tposy, WORLDMAP_ZORDER_ARMY );
		--[[if state == ARMY_STATE_MARCH then
			moveAttr.reback = 0
		elseif state == ARMY_STATE_REBACK then
			moveAttr.reback = 1
		end--]]
		
		-- 武将形象
		if from_actorid == GetPlayer().m_actorid then
			moveAttr.invokeShow = 1;
			for i=1, 4, 1 do
				local uiHero = unitObj.transform:GetChild(i-1);
				if i <= #heroid then
					if i == 1 then
						SetTrue(uiHero) -- 第1个武将默认显示
					else
						SetFalse(uiHero)
					end
					if from_type == MAPUNIT_TYPE_TOWN and heroid[i] >= 1000 then
						SetText( uiHero.transform:Find("Name"), T(1330) )
					else
						SetText( uiHero.transform:Find("Name"), HeroName( heroid[i] ) )
					end	
					local shapeObj = uiHero.transform:Find("Shape");
					local childCount = shapeObj.transform.childCount;
					for i = 0, childCount - 1, 1 do
						GameObject.Destroy( shapeObj.transform:GetChild(i).gameObject );
					end
					local shape = nation;
					if shape == 0 then
						shape = 1;
					end
					local charactor = Character.Create( shape );
					charactor.transform:SetParent( shapeObj.transform );
					charactor.transform.localPosition = Vector3.New( 0, 0, 0 );
					charactor.transform.localScale = Vector3.one;
					charactor:TurnTo( direction );
					charactor:Walk();
					
					local pos = direction.normalized * ( ( i - 1 ) * -0.8 )
					pos.z = pos.y;
					uiHero.transform.localPosition = pos;

				else
					SetFalse( uiHero )
				end
			end
			moveAttr.heroCount = #heroid
		else
			moveAttr.invokeShow = 0;
			-- 别人的部队
			local uiHero = unitObj.transform:GetChild(0);
			SetFalse( unitObj.transform:GetChild(1) )
			SetFalse( unitObj.transform:GetChild(2) )
			SetFalse( unitObj.transform:GetChild(3) )
			SetTrue( uiHero )
			SetText( uiHero.transform:Find("Name"), recvValue.m_name )
			local shapeObj = uiHero.transform:Find("Shape");
			local childCount = shapeObj.transform.childCount;
			for i = 0, childCount - 1, 1 do
				GameObject.Destroy( shapeObj.transform:GetChild(i).gameObject );
			end
			local shape = nation;
			local charactor = Character.Create( shape );
			charactor.transform:SetParent( shapeObj.transform );
			charactor.transform.localPosition = Vector3.New( 0, 0, 0 );
			charactor.transform.localScale = Vector3.one;
			charactor:TurnTo( direction );
			charactor:Walk();
		end
	end
	return unitObj;
end

-- 创建城镇
function MapUnit.createTown( recvValue )
	local state 		= recvValue.m_state;
	local posx 			= recvValue.m_posx;
	local posy 			= recvValue.m_posy;
	local custom_name	= recvValue.m_name;
	local custom_namelen= recvValue.m_namelen;
	local nation 		= recvValue.m_char_value[1];
	local dev_level 	= recvValue.m_char_value[2];
	local townid 		= recvValue.m_short_value[1];
	local produce_num	= recvValue.m_short_value[2];
	local protect_sec	= recvValue.m_int_value[1];
	local produce_sec	= recvValue.m_int_value[2];
	
	local type 			= g_towninfo[townid].type
	local grid 			= g_towninfo[townid].grid
	local range 		= g_towninfo[townid].range
	local produce_maxnum= g_towninfo[townid].produce_maxnum
	local produce_maxsec= g_towninfo[townid].produce_maxsec
	local level 		= g_towninfo[townid].level
	
	-- 先搜索缓存，如果缓存有，那么就更新
	local unitObj = MapUnit.cache[recvValue.m_unit_index];
	
	-- 先检查对象缓存池是否有空余的
	if unitObj == nil then
		for index, unit in pairs( MapUnit.objectPoolTown ) do
			if unit and unit.gameObject.activeSelf == false then
				unitObj = unit;
				break;
			end
		end
	end
	
	-- 没有空余的就新创建一个
	if unitObj == nil then
		unitObj = GameObject.Instantiate( MapUnitTown );
		unitObj.transform:SetParent( MapUnit.unitRoot );
		table.insert( MapUnit.objectPoolTown, unitObj );
	end
	
	-- 位置
	local cameraPosX, cameraPosY = WorldMap.ConvertGameToCamera( posx, posy );
	posx, posy = MapUnit.getGridTrans( MAPUNIT_TYPE_TOWN, grid, cameraPosX, cameraPosY );
	unitObj.transform.localPosition = Vector3.New( posx, posy, posy );
	
	-- 获取引用
	local objs = unitObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiShape = objs[0];
	local uiName = objs[1];
	local uiRange = objs[2];
	local uiTownProduceMod = objs[3];
	local uiTimerText = objs[4];
	local uiTimerOver = objs[5];
	local uiMatBtn = objs[6];
	local uiItemShape = objs[7];
	local uiTownFightMod = objs[8];
	
	-- 形象
	local shapeSprite = ""
	shapeSprite = MapUnitTownShapeList[type].."_"..nation;
	uiShape:GetComponent("SpriteRenderer").sprite = LoadSprite( shapeSprite );
	
	-- 名字
	local townname = ""
	if custom_namelen > 0 then
		townname = custom_name;
	else
		townname = MapTownName( townid );
	end
	if type == MAPUNIT_TYPE_TOWN_TYPE8 then
		uiName:GetComponent("UIText").text = "Lv."..(dev_level+1).." "..townname
	else
		uiName:GetComponent("UIText").text = townname
	end
	
	-- 范围框
	if type < MAPUNIT_TYPE_TOWN_TYPE7 then
		uiRange.transform.localScale = Vector3.New( range, range, range );
		uiRange.transform:GetComponent("SpriteRenderer").color = Hex2Color( MapUnitRangeColor[nation] )
		SetTrue( uiRange )
	else
		SetFalse( uiRange )
	end
	
	-- 非群雄状态显示
	if nation > 0 and type < MAPUNIT_TYPE_TOWN_TYPE8 then
		SetTrue( uiTownProduceMod )
	
		if produce_num > 0 then
			SetFalse( uiTimerText )
			SetTrue( uiTimerOver )
			SetText( uiTimerOver, T(1287) )
		else
			-- 生产倒计时
			if produce_sec > 0 then
				SetTrue( uiTimerText )
				SetFalse( uiTimerOver )
				SetTimer( uiTimerText, produce_sec, produce_maxsec );
			else
				SetFalse( uiTimerText )
				SetTrue( uiTimerOver )
				SetText( uiTimerOver, T(1287) )
			end
		end
		
		-- 保护状态
		if protect_sec > 0 then
		else
		end
		
		local ShareData = uiTownProduceMod.transform:GetComponent("ShareData");
		ShareData.intValue[0] = recvValue.m_unit_index;
	
	else
		SetFalse( uiTownProduceMod )
	end
	
	-- 国战
	local has = 0;
	local show = 0;
	for i=3, 6, 1 do
		local from_nation = recvValue.m_char_value[i]
		if from_nation >= 0 then 
			has = 1;
			if from_nation == GetPlayer().m_nation then
				show = 1;
				break;
			end
		end
	end
	
	if (has == 1 and nation == GetPlayer().m_nation) or show == 1 then
		SetTrue( uiTownFightMod )
		local ShareData = uiTownFightMod.transform:GetComponent("ShareData");
		ShareData.intValue[0] = recvValue.m_unit_index;
	else
		SetFalse( uiTownFightMod )
	end
	
	return unitObj;
end

-- 刷新下任务目标流寇
function MapUnit.RefreshQuestEnemy( targetLevel )
    for index, unit in pairs( WorldMap.m_nMapUnitList ) do
        if unit.m_type == MAPUNIT_TYPE_ENEMY then
            local unitObj = MapUnit.cache[ unit.m_unit_index ];
            if unitObj ~= nil then
                local level = unit.m_char_value[1];
                local objs = unitObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
				local uiQuest = objs[2];
	
				if level == g_QuestTargetEnemyLevel then
					if uiQuest == nil then
						uiQuest = GameObject.Instantiate( LoadPrefab( "Djjt" ) )
						uiQuest.transform:SetParent( unitObj.transform );
						uiQuest.transform.localPosition = Vector3( 0, 0.4, 0 );
						objs[2] = uiQuest;
					end
					SetTrue( uiQuest )
				else
					if uiQuest ~= nil then
						SetFalse( uiQuest )
					end
				end
            end
        end
    end    
end

-- 刷新下目标战斗流寇
function MapUnit.RefreshTargetEnemy( recvValue )
	for index, unit in pairs( WorldMap.m_nMapUnitList ) do
        if unit.m_type == MAPUNIT_TYPE_ENEMY then
            local unitObj = MapUnit.cache[ unit.m_unit_index ];
            if unitObj ~= nil then
				local posx 		= unit.m_posx;
				local posy 		= unit.m_posy;
                local objs 		= unitObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
				local uiTarget = objs[3];
				if recvValue ~= nil and recvValue.m_to_posx == posx and recvValue.m_to_posy == posy then
					print("zxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx")
					if uiTarget == nil then
						uiTarget = GameObject.Instantiate( LoadPrefab( "Zdtb" ) )
						uiTarget.transform:SetParent( unitObj.transform );
						uiTarget.transform.localPosition = Vector3( 0, 0.4, 0 );
						objs[3] = uiTarget;
					end
					SetTrue( uiTarget )
				else
					if uiTarget ~= nil then
						SetFalse( uiTarget )
					end
				end
            end
        end
    end
end

-- 创建流寇
function MapUnit.createEnemy( recvValue )
	local state 	= recvValue.m_state;
	local posx 		= recvValue.m_posx;
	local posy 		= recvValue.m_posy;
	local level	= recvValue.m_char_value[1];
	local kind 	= recvValue.m_short_value[1];
	
	-- 先搜索缓存，如果缓存有，那么就更新
	local unitObj = MapUnit.cache[recvValue.m_unit_index];
	
	-- 先检查对象缓存池是否有空余的
	if unitObj == nil then
		for index, unit in pairs( MapUnit.objectPoolEnemy ) do
			if unit and unit.gameObject.activeSelf == false then
				unitObj = unit;
				break;
			end
		end
	end
	
	-- 没有空余的就新创建一个
	if unitObj == nil then
		unitObj = GameObject.Instantiate( MapUnitEnemy );
		unitObj.transform:SetParent( MapUnit.unitRoot );
		table.insert( MapUnit.objectPoolEnemy, unitObj );
	end
	
	-- 位置
	local cameraPosX, cameraPosY = WorldMap.ConvertGameToCamera( posx, posy );
	posx, posy = MapUnit.getGridTrans( MAPUNIT_TYPE_ENEMY, 0, cameraPosX, cameraPosY );
	unitObj.transform.localPosition = Vector3.New( posx, posy, posy-1 );
	
	-- 获取引用
	local objs = unitObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiShape = objs[0];
	local uiName = objs[1];
	local uiQuest = objs[2];
	local uiTarget = objs[3];
	-- 名字
	if level > GetPlayer().m_mokilllv+1 then
		SetText( uiName, "Lv."..level.." "..T(938), Color.red )
	else
		SetText( uiName, "Lv."..level.." "..T(938), Color.white )
	end
	
	local shape = g_enemyinfo[kind].shape
	local spriteAni = uiShape:GetComponent( typeof(SpriteAnimation) )
	spriteAni.SpriteFrames[0] =  MapUnit.SpritePoolEnemy[shape][1]
	spriteAni.SpriteFrames[1] =  MapUnit.SpritePoolEnemy[shape][2]
	spriteAni.SpriteFrames[2] =  MapUnit.SpritePoolEnemy[shape][3]
	
	local spriteRenderer = uiShape:GetComponent("SpriteRenderer");
	spriteRenderer.sprite = MapUnit.SpritePoolEnemy[shape][1]
	if math.random( 1, 2 ) == 1 then
		spriteRenderer.flipX = true;
	else
		spriteRenderer.flipX = false;
	end	
	
	-- 是否是主线任务目标流寇
    if level == g_QuestTargetEnemyLevel then
		if uiQuest == nil then
			uiQuest = GameObject.Instantiate( LoadPrefab( "Djjt" ) )
			uiQuest.transform:SetParent( unitObj.transform );
			uiQuest.transform.localPosition = Vector3( 0, 0.4, 0 );
			objs[2] = uiQuest;
		end
		SetTrue( uiQuest )
    else
		if uiQuest ~= nil then
			SetFalse( uiQuest )
        end
    end
	
	-- 是否我部队行军目标
	local isTarget = 0;
	for i=1, #g_targetEnemyPos, 1 do
		if recvValue.m_posx == g_targetEnemyPos[i].x and recvValue.m_posy == g_targetEnemyPos[i].y then
			isTarget = 1;
			break
		end
	end
	if isTarget == 1 then
		if uiTarget == nil then
			uiTarget = GameObject.Instantiate( LoadPrefab( "Zdtb" ) )
			uiTarget.transform:SetParent( unitObj.transform );
			uiTarget.transform.localPosition = Vector3( 0, 0.4, 0 );
			objs[3] = uiTarget;
		end
		SetTrue( uiTarget )
    else
		if uiTarget ~= nil then
			SetFalse( uiTarget )
        end
    end
	return unitObj;
end

-- 创建资源点
function MapUnit.createRes( recvValue )
	local state 	= recvValue.m_state;
	local name 		= recvValue.m_name;
	local posx 		= recvValue.m_posx;
	local posy 		= recvValue.m_posy;
	local restype	= recvValue.m_char_value[1];
	local level		= recvValue.m_char_value[2];
	
	-- 先搜索缓存，如果缓存有，那么就更新
	local unitObj = MapUnit.cache[recvValue.m_unit_index];
	
	-- 先检查对象缓存池是否有空余的
	if unitObj == nil then
		for index, unit in pairs( MapUnit.objectPoolRes ) do
			if unit and unit.gameObject.activeSelf == false then
				unitObj = unit;
				break;
			end
		end
	end
	
	-- 没有空余的就新创建一个
	if unitObj == nil then
		unitObj = GameObject.Instantiate( MapUnitRes );
		unitObj.transform:SetParent( MapUnit.unitRoot );
		table.insert( MapUnit.objectPoolRes, unitObj );
	end
	
	-- 位置
	local cameraPosX, cameraPosY = WorldMap.ConvertGameToCamera( posx, posy );
	posx, posy = MapUnit.getGridTrans( MAPUNIT_TYPE_RES, 0, cameraPosX, cameraPosY );
	unitObj.transform.localPosition = Vector3.New( posx, posy, posy );
	
	-- 获取引用
	local objs = unitObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiShape = objs[0];
	local uiName = objs[1];
	local uiEffectGather = objs[2];
		
	-- 形象
    --uiShape:GetComponent("SpriteRenderer").sprite = LoadSprite( MapUnitResShapeList[restype] );
	uiShape:GetComponent("SpriteRenderer").sprite = MapUnit.SpritePoolRes[restype]
	
	-- 名字
	if state == ARMY_STATE_GATHER then
		local nation	= recvValue.m_char_value[3];
		local actorid	= recvValue.m_int_value[1];
		if actorid == GetPlayer().m_actorid then
			SetText( uiName, "["..Nation(nation).."]"..name, NameColor(2) )
		else
			SetText( uiName, "["..Nation(nation).."]"..name, NameColor(4) )
		end
		uiEffectGather.gameObject:SetActive( true );
	else
		SetText( uiName, "Lv."..level.." "..T(MapUnitResNameList[restype]), Hex2Color( 0xFFFFFFFF ) ) 
		uiEffectGather.gameObject:SetActive( false );
	end
	return unitObj;
end

-- 创建随机事件
function MapUnit.createEvent( recvValue )
	local state 	= recvValue.m_state;
	local name 		= recvValue.m_name;
	local posx 		= recvValue.m_posx;
	local posy 		= recvValue.m_posy;
	local type		= recvValue.m_char_value[1];
	local kind		= recvValue.m_short_value[1];
	local actorid	= recvValue.m_int_value[1];
	local waitsec	= recvValue.m_int_value[2];

	local info 		= g_mapeventinfo[kind]
	if info == nil then
		return
	end
	-- 先搜索缓存，如果缓存有，那么就更新
	local unitObj = MapUnit.cache[recvValue.m_unit_index];
	
	-- 先检查对象缓存池是否有空余的
	if unitObj == nil then
		for index, unit in pairs( MapUnit.objectPoolEvent ) do
			if unit and unit.gameObject.activeSelf == false then
				unitObj = unit;
				break;
			end
		end
	end
	
	-- 没有空余的就新创建一个
	if unitObj == nil then
		unitObj = GameObject.Instantiate( MapUnitEvent );
		unitObj.transform:SetParent( MapUnit.unitRoot );
		table.insert( MapUnit.objectPoolEvent, unitObj );
	end

	-- 位置
	local cameraPosX, cameraPosY = WorldMap.ConvertGameToCamera( posx, posy );
	posx, posy = MapUnit.getGridTrans( MAPUNIT_TYPE_EVENT, 0, cameraPosX, cameraPosY );
	unitObj.transform.localPosition = Vector3.New( posx, posy, posy );
	
	-- 获取引用
	local objs = unitObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiShape = objs[0];
	local uiName = objs[1];
	local uiEffect = objs[2];
	local uiIcon = objs[3];
	local uiTimer = objs[4];
	local uiResIcon = objs[5];
	local uiOpIcon = objs[6];
	local uiEffectResIcon = objs[7];
	local uiEffectOpIcon = objs[8];
	
	-- 形象
    uiShape:GetComponent("SpriteRenderer").sprite = LoadSprite( MapUnitEventShapeList[type] );
	
	-- 名字
	if type == 3 and info.costtype > 0 then --采集运输车
		if info.costnum > 100000 then
			uiName:GetComponent("UIText").text = knum(info.costnum)--[[ResName( info.costtype )==]]
		else
			uiName:GetComponent("UIText").text = info.costnum
		end
		SetImage( uiIcon, ResIcon( info.costtype ) )
		SetTrue( uiIcon )
	else
		uiName:GetComponent("UIText").text = T(MapUnitEventNameList[type])
		SetFalse( uiIcon )
	end
	
	-- 完成状态
	if state == 1 then
		SetTrue( uiEffect )
		if type == 1 then
			SetImage( uiEffectOpIcon, LoadSprite( "ui_opration_6" ) )
			SetTrue( uiEffectOpIcon )
			SetFalse( uiEffectResIcon )
			
		elseif type == 2 then
			SetImage( uiEffectOpIcon, LoadSprite( "ui_opration_2" ) )
			SetTrue( uiEffectOpIcon )
			SetFalse( uiEffectResIcon )
			
		elseif type == 3 then
			if info.awardkind == 50001 then
				SetImage( uiEffectResIcon, ResIcon( 1 ) )
			elseif info.awardkind == 50002 then
				SetImage( uiEffectResIcon, ResIcon( 2 ) )
			elseif info.awardkind == 50003 then
				SetImage( uiEffectResIcon, ResIcon( 3 ) )
			end
			SetFalse( uiEffectOpIcon )
			SetTrue( uiEffectResIcon )
			
		elseif type == 4 then
			SetImage( uiEffectResIcon, ResIcon( 3 ) )
			SetFalse( uiEffectOpIcon )
			SetTrue( uiEffectResIcon )
		
		elseif type == 5 then
			SetImage( uiEffectResIcon, LoadSprite( "ui_main_icon_bag" ) )
			SetFalse( uiEffectOpIcon )
			SetTrue( uiEffectResIcon )

		end
		waitsec = 0;
	else
		SetFalse( uiEffect )
	end
	
	-- 倒计时
	if waitsec > 0 then
		SetTrue( uiTimer )
		SetTimer( uiTimer.transform:Find("Text"), waitsec, info.waitsec );
		if type == 1 then
			SetImage( uiOpIcon, LoadSprite( "ui_opration_6" ) )
			SetTrue( uiOpIcon )
			SetFalse( uiResIcon )
			
		elseif type == 2 then
			SetImage( uiOpIcon, LoadSprite( "ui_opration_2" ) )
			SetTrue( uiOpIcon )
			SetFalse( uiResIcon )
			
		elseif type == 3 then 
			if info.awardkind == 50001 then
				SetImage( uiResIcon, ResIcon( 1 ) )
			elseif info.awardkind == 50002 then
				SetImage( uiResIcon, ResIcon( 2 ) )
			elseif info.awardkind == 50003 then
				SetImage( uiResIcon, ResIcon( 3 ) )
			end
			SetFalse( uiOpIcon )
			SetTrue( uiResIcon )
			
		elseif type == 4 then
			SetImage( uiResIcon, ResIcon( 3 ) )
			SetFalse( uiOpIcon )
			SetTrue( uiResIcon )
			
		elseif type == 5 then
			SetImage( uiResIcon, LoadSprite( "ui_main_icon_bag" ) )
			SetFalse( uiOpIcon )
			SetTrue( uiResIcon )
			
		end
	else
		SetFalse( uiTimer )
	end
	return unitObj;
end

-- 地图区域边界线
function MapUnit.createMapBorder( posx, posy, range )
	if MapUnitRoot == nil then
		return;
	end

	local unitObj = GameObject.Instantiate( MapBorder );
	unitObj.transform:SetParent( MapUnitRoot );
	
	-- 位置
	local cameraPosX, cameraPosY = WorldMap.ConvertGameToCamera( posx, posy );
	posx, posy = MapUnit.getGridTrans( MAPUNIT_TYPE_TOWN, 2, cameraPosX, cameraPosY );
	unitObj.transform.localPosition = Vector3.New( posx, posy, 0 );

	-- 范围
	unitObj.transform:GetComponent("MapBorder"):SetSize( range );
    --unitObj.transform:GetComponent("MapBorder"):SetColor( Color.New( 255 / 255, 0 / 255, 0 / 255, 128 / 255 ) );
end

-- 创建皇城区域名称和都城范围
function MapUnit.createTownRange( unitObj, grid, posx, posy, range, nation )
	if MapUnitRoot == nil then
		return;
	end
	
	if unitObj == nil then
		unitObj = GameObject.Instantiate( MapTownRange );
		unitObj.transform:SetParent( MapUnitRoot );
	end
	
	-- 位置
	local cameraPosX, cameraPosY = WorldMap.ConvertGameToCamera( posx, posy );
	posx, posy = MapUnit.getGridTrans( MAPUNIT_TYPE_TOWN, grid, cameraPosX, cameraPosY );
	unitObj.transform.localPosition = Vector3.New( posx, posy, 0 );

	-- 范围
	unitObj.transform:GetComponent("MapBorder"):SetSize( range );
    unitObj.transform:GetComponent("MapBorder"):SetColor( Hex2Color( MapUnitRangeColor[nation] ) );
	return unitObj;
end

-- 区域都城名城的范围
-- m_count=0,m_list={m_townid=0,m_nation=0,[m_count]}
function MapUnit.createCenterTownRange( recvValue )
	local townid = recvValue.m_townid;
	local nation = recvValue.m_nation;
	local v = g_towninfo[townid];
	if v.type >= MAPUNIT_TYPE_TOWN_TYPE7 then
		if v.grid == 2 then
			MapUnit.objectCenterTownRange[townid] = MapUnit.createTownRange( MapUnit.objectCenterTownRange[townid], 2, v.posx-1, v.posy, v.range, nation );
		elseif v.grid == 3 then
			MapUnit.objectCenterTownRange[townid] = MapUnit.createTownRange( MapUnit.objectCenterTownRange[townid], 2, v.posx-1, v.posy, v.range, nation );
		end
	end
end

-- 获取占地块
function MapUnit.getGrid( unittype, unitgrid )
	-- 城池
	if unittype == MAPUNIT_TYPE_CITY then
		return 1;
	-- 部队
	elseif unittype == MAPUNIT_TYPE_ARMY then
		return 1;
	-- 城镇
	elseif unittype == MAPUNIT_TYPE_TOWN then
		return unitgrid;
	-- 敌人
	elseif unittype == MAPUNIT_TYPE_ENEMY then
		return 1;
	-- 资源
	elseif unittype == MAPUNIT_TYPE_RES then
		return 1;
	-- 随机事件
	elseif unittype == MAPUNIT_TYPE_EVENT then
		return 1;
	end	
	return 1;
end

-- 根据占地块转换位置坐标
function MapUnit.getGridTrans( unittype, unitgrid, posx, posy )
	local grid = MapUnit.getGrid( unittype, unitgrid );
	if grid == 1 then
		return posx, posy - MAP_TILEHEIGHT/100/2;
	elseif grid == 2 then
		return posx + MAP_TILEWIDTH/100/2, posy - MAP_TILEHEIGHT/100/2;
	elseif grid == 3 then
		return posx, posy - MAP_TILEHEIGHT/100/2;
	end
	return posx, posy;
end

-- 部队加速
function MapUnit.armySpeedUpdate( unit_index, state, statetime, stateduration )
	local recvValue = WorldMap.m_nMapUnitList[unit_index];
	if recvValue == nil then
		return;
	end
	
	local unitObj = MapUnit.cache[unit_index];
	if unitObj == nil then
		return;
	end
		
	local to_type 		= recvValue.m_short_value[3];
	local to_grid 		= recvValue.m_short_value[4];
	local to_posx 		= recvValue.m_short_value[3];
	local to_posy		= recvValue.m_short_value[4];
	local move_time 	= statetime;
	local move_needtime = stateduration;

	-- 目的点
	cameraPosX, cameraPosY = WorldMap.ConvertGameToCamera( to_posx, to_posy );
	local tposx, tposy = MapUnit.getGridTrans( to_type, to_grid, cameraPosX, cameraPosY );
	
	-- 当前点
	local fposx = unitObj.transform.localPosition.x;
	local fposy = unitObj.transform.localPosition.y;
	
	-- 是否返程
	if state == ARMY_STATE_REBACK then
		fposx, tposx = tposx, fposx;
		fposy, tposy = tposy, fposy;
	end
	
	-- 总距离
	local distance = math.sqrt( math.pow( ( tposx - fposx ), 2 ) + math.pow( ( tposy - fposy ), 2) );

	-- 计算客户端需要的速度
	local speed = distance/(move_needtime-move_time);

	-- 开始移动
	local moveAttr = unitObj:GetComponent("ArmyMove");
	moveAttr.speed = speed;
end
