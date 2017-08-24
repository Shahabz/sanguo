-- 地图显示单元

-- 显示单元类型
MAPUNIT_TYPE_CITY		=	1	-- 城池
MAPUNIT_TYPE_ARMY		=	2	-- 部队
MAPUNIT_TYPE_TOWN		=	3	-- 城镇
MAPUNIT_TYPE_ENEMY		=	4	-- 流寇
MAPUNIT_TYPE_RES		=	5	-- 资源点


-- 城池状态
CITY_STATE_FIRE			=	0x01	-- 着火中
CITY_STATE_KICK			=	0x02	-- 击飞中
CITY_STATE_FIGHT		=	0x04	-- 战斗中

-- 单只部队状态
ARMY_STATE_IDLE				=	0	-- 空闲
ARMY_STATE_GROUP_START		=	1	-- 发起集结
ARMY_STATE_GROUP_ING		=	2	-- 集结中
ARMY_STATE_GROUP_END		=	3	-- 到达指定集结地点,集结完毕
ARMY_STATE_READY            =   4   -- 准备中
ARMY_STATE_MARCH			=	10	-- 行军中
ARMY_STATE_FIGHT			=	20	-- 战斗中
ARMY_STATE_OCCUPY			=	21	-- 驻扎中
ARMY_STATE_GATHER			=	22	-- 采集中
ARMY_STATE_HELP				=	24	-- 援助中
ARMY_STATE_REBACK			=	30	-- 返程中

-- 行为
ARMY_ACTION_IDLE			=	0	-- 空闲
ARMY_ACTION_FIGHT			=	1	-- 去战斗
ARMY_ACTION_OCCUPY			=	2	-- 去驻扎
ARMY_ACTION_GATHER			=	3	-- 去采集
ARMY_ACTION_HELP_TROOP		=	6	-- 去士兵援助
ARMY_ACTION_GROUP_START		=	10	-- 发起集结中
ARMY_ACTION_GROUP_ING		=	11	-- 去集结中

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
local MapUnitRes 		= nil;
local MapUnitMonster 	= nil;
local MapUnitTown 		= nil;
local MapUnitShelter 	= nil;
local MapUnitActivity	= nil;
local MapUnitArmyOccupy	= nil;
local MapUnitClubTown 	= nil;
local MapUnitClubStore 	= nil;
local MapUnitClubJet 	= nil;
local MapUnitClubRes 	= nil;
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

-- 资源点形象
MapUnitResShapeList = {
[1] = "mapunit_res_silver",
[2] = "mapunit_res_wood",
[3] = "mapunit_res_food",
[4] = "mapunit_res_iron",
[5] = "mapunit_res_token",
}

-- 城镇形象
MapUnitTownShapeList = {
[1] = "mapunit_type1",
[2] = "mapunit_type2",
[3] = "mapunit_type3",
[4] = "mapunit_type4",
[5] = "mapunit_type5",
[6] = "mapunit_type6",
}

-- 资源点名称
MapUnitResNameList = {
[1] = 171,
[2] = 172,
[3] = 173,
[4] = 174,
[5] = 175,
}

-- 范围颜色
MapUnitRangeColor = {
[0] = 0xecc244ff,
[1] = 0x25c9ffff,
[2] = 0xa01515ff,
[3] = 0x03de27ff,
}

-- 军队状态名称
MapUnitArmyStateNameList = {
[ARMY_STATE_GROUP_START]        = 2729,
[ARMY_STATE_GROUP_ING]          = 2730,
[ARMY_STATE_GROUP_END]          = 2731,
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

-- 初始化
function MapUnit.init()
	if MapUnitInited == false then
		-- 预置的初始化
		MapUnitCity 		= LoadPrefab("MapUnitCity");
		MapUnitArmy 		= LoadPrefab("MapUnitArmy");
		MapUnitTown 		= LoadPrefab("MapUnitTown");
		MapUnitEnemy 		= LoadPrefab("MapUnitEnemy");
		MapUnitRes 			= LoadPrefab("MapUnitRes");
		MapBorder			= LoadPrefab("MapBorder");
		MapTownRange		= LoadPrefab("MapTownRange");
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
	
	MapUnitInited		= false;
	MapUnit.unitRoot 	= nil;
	
	MapUnit.objectPoolCity 		= {};
	MapUnit.objectPoolArmy		= {};
	MapUnit.objectPoolTown 		= {};
	MapUnit.objectPoolEnemy 	= {};
	MapUnit.objectPoolRes 		= {};

	MapUnit.cache 				= {};
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
		
	-- 形象
    unitObj.transform:Find("Shape"):GetComponent("SpriteRenderer").sprite = LoadSprite( MapUnitCityShapeList[level].."_"..nation );
			
	-- 名字
	unitObj.transform:Find("Name"):GetComponent("UIText").text = "Lv."..level.." "..name

	-- 范围框
	local uiRange = unitObj.transform:Find("Range");
	if recvValue.m_unit_index == WorldMap.m_nMyCityUnitIndex then
		SetTrue( uiRange )
		uiRange.transform:GetComponent("SpriteRenderer").color = Hex2Color( MapUnitRangeColor[nation] )
		uiRange.transform.localScale = Vector3.New( 3, 3, 3 );
	else
		SetFalse( uiRange )
	end
	
	-- 保护BUFF
	if ptbuff > 0 then
		--unitObj.transform:FindChild("EffectProtect").gameObject:SetActive( true );
	else
		--unitObj.transform:FindChild("EffectProtect").gameObject:SetActive( false );
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
		unitObj.transform:SetParent( MapUnit.unitRoot );
		table.insert( MapUnit.objectPoolArmy, unitObj );
	end
	
	-- 关联数据
	local ShareData = unitObj:GetComponent("ShareData");
	if ShareData then
		ShareData:AddValue( "unit_index", recvValue.m_unit_index );
	end
	
	-- 根据状态设置属性
	if state == ARMY_STATE_GROUP_ING or state == ARMY_STATE_MARCH or state == ARMY_STATE_REBACK then
		local posx 			= recvValue.m_posx;
		local posy 			= recvValue.m_posy;	
		local from_type 	= recvValue.m_short_value[1];
		local from_posx 	= recvValue.m_short_value[2];
		local from_posy 	= recvValue.m_short_value[3];
		local to_type 		= recvValue.m_short_value[4];
		local to_posx 		= recvValue.m_short_value[5];
		local to_posy		= recvValue.m_short_value[6];
		local action		= recvValue.m_short_value[7];
		local army_index 	= recvValue.m_int_value[1];
		local move_time 	= recvValue.m_int_value[2];
		local move_needtime = recvValue.m_int_value[3];

		-- 出发点
		local cameraPosX, cameraPosY = WorldMap.ConvertGameToCamera( from_posx, from_posy );
		local fposx, fposy = MapUnit.getGridTrans( from_type, 0, cameraPosX, cameraPosY );

		-- 目的点
		cameraPosX, cameraPosY = WorldMap.ConvertGameToCamera( to_posx, to_posy );
		local tposx, tposy = MapUnit.getGridTrans( to_type, 0, cameraPosX, cameraPosY );
		
		-- 总距离
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
		local moveAttr = unitObj:GetComponent("MapUnitMove");
		moveAttr.stat = 1;
		moveAttr.speed = speed;
		moveAttr.toPosition = Vector3.New( tposx, tposy, WORLDMAP_ZORDER_ARMY );
		
		-- 形象
		local shapeObj = unitObj.transform:FindChild("Shape");
		local childCount = shapeObj.transform.childCount;
		for i = 0, childCount - 1, 1 do
			GameObject.Destroy( shapeObj.transform:GetChild(i).gameObject );
		end

		-- 采集行军，显示马车
		if action == ARMY_ACTION_GATHER or action == ARMY_ACTION_HELP_RES or action == ARMY_ACTION_CLUB_STORE or action == ARMY_ACTION_CLUB_STORE_OUT then
			local charactor = Character.Create( 99 );
			charactor.transform:SetParent( unitObj.transform:FindChild("Shape").transform );
			charactor.transform.localPosition = Vector3.New( 0, 0, 0 );
            charactor.transform.localScale = Vector3.one;
			charactor:Show(true)
			charactor:TurnTo( direction );
			charactor:Walk();
			
		-- 交谈
		elseif action == ARMY_ACTION_TALK then
			local troopInfo = {};
			table.insert( troopInfo, { shape=2, num=1 } );
			local legion = TroopLegionCreate( troopInfo, direction, 0.1 );
			legion.transform:SetParent( unitObj.transform:FindChild("Shape").transform );
			legion.transform.localPosition = Vector3.New( 0, 0, 0 );
			legion.transform.localScale = Vector3.New( 0.65, 0.65, 0.65 );
			
		-- 城镇部队
		elseif from_type == MAPUNIT_TYPE_TOWN and action ~= ARMY_ACTION_GROUP_START and action ~= ARMY_ACTION_GROUP_ING and action ~= ARMY_ACTION_TRAFFICARMY and action ~= ARMY_ACTION_TOWNOCCUPY then
--[[			local charactor = FightCharacter.Create( 102 );
			charactor.transform:SetParent( unitObj.transform:FindChild("Shape").transform );
			charactor.transform.localPosition = Vector3.New( 0, 0, 0 );
			charactor:Show(true)
			charactor:TurnTo( direction );
			charactor:Walk();--]]
			
			local troopInfo = { { shape = 102, num=1 }, { shape = 108, num=3 }, { shape = 108, num=3 }  };
			local legion = TroopLegionCreate( troopInfo, direction, 0.1 );
			legion.transform:SetParent( unitObj.transform:FindChild("Shape").transform );
			legion.transform.localPosition = Vector3.New( 0, 0, 0 );
			legion.transform.localScale = Vector3.New( 0.8, 0.8, 0.8 );
		
		-- 龙穴部队
		elseif from_type == MAPUNIT_TYPE_SHELTER and action ~= ARMY_ACTION_SHELTER then
			local charactor = FightCharacter.Create( 114 );
			charactor.transform:SetParent( unitObj.transform:FindChild("Shape").transform );
			charactor.transform.localPosition = Vector3.New( 0, 0, 0 );
            charactor.transform.localScale = Vector3.one;
			charactor:Show(true)
			charactor:TurnTo( direction );
			charactor:Walk();
			
		-- 其它行军，显示部队
		else
			local shownum = 3;
			if action == ARMY_ACTION_GROUP_START and state == ARMY_STATE_MARCH then
				shownum = 5;
			end
			local troopInfo = {};
            
			for tmpi=1, recvValue.m_char_value_count, 1 do
				local corps = recvValue.m_char_value[tmpi];
                if corps == 100 then
                    table.insert( troopInfo, { shape = 10, num=1 } );
				else
                    table.insert( troopInfo, { shape = Data.Troop_Client()[corps][1]["shape"], num=shownum } );
                end
			end
			local legion = TroopLegionCreate( troopInfo, direction, 0.1 );
			legion.transform:SetParent( unitObj.transform:FindChild("Shape").transform );
			legion.transform.localPosition = Vector3.New( 0, 0, 0 );
			legion.transform.localScale = Vector3.New( 0.65, 0.65, 0.65 );
		end
		
	-- 驻扎中
	elseif state == ARMY_STATE_OCCUPY then
		local action 		= recvValue.m_char_value[1];
		local to_posx 		= recvValue.m_short_value[1];
		local to_posy 		= recvValue.m_short_value[2];
		local army_index 	= recvValue.m_int_value[1];
	
		-- 位置
		local cameraPosX, cameraPosY = WorldMap.ConvertGameToCamera( to_posx, to_posy );
		local tposx, tposy = MapUnit.getGridTrans( MAPUNIT_TYPE_ARMY, 0, cameraPosX, cameraPosY );
		unitObj.transform.localPosition = Vector3.New( tposx, tposy, tposy );
		
		-- 停止移动
		local moveAttr = unitObj:GetComponent("MapUnitMove");
		moveAttr.stat = 0;
		
		-- 形象
		local shapeObj = unitObj.transform:FindChild("Shape");
		local childCount = shapeObj.transform.childCount;
		for i = 0, childCount - 1, 1 do
			GameObject.Destroy( shapeObj.transform:GetChild(i).gameObject );
		end
		local charactor = GameObject.Instantiate( MapUnitArmyOccupy );
		charactor.transform:SetParent( unitObj.transform:FindChild("Shape").transform );
		charactor.transform.localPosition = Vector3.New( 0, 0, 0 );
        charactor.transform.localScale = Vector3.one;
		
		-- 挖宝
		if action == ARMY_ACTION_TREASURE then
            unitObj.gameObject:SetActive( true );
            charactor.transform:FindChild( "FX" ).gameObject:SetActive( true );
			local treasure_proc 	= recvValue.m_int_value[3];
			local treasure_total 	= recvValue.m_int_value[4];
			if treasure_total > 0 and treasure_proc >= treasure_total then
				-- 挖掘完毕
                charactor.transform:FindChild( "FX" ):GetComponent( "Animator" ):SetInteger( "State", 1 );
			else
				-- 正在挖掘中
                charactor.transform:FindChild( "FX" ):GetComponent( "Animator" ):SetInteger( "State", 0 );
			end
        else
            charactor.transform:FindChild( "FX" ).gameObject:SetActive( false );
		end
		
		
	-- 战斗	
	elseif state == ARMY_STATE_FIGHT then
		local to_unittype 	= recvValue.m_char_value[1];
		local to_posx 		= recvValue.m_short_value[1];
		local to_posy 		= recvValue.m_short_value[2];
		
		-- 目标位置
		local cameraPosX, cameraPosY = WorldMap.ConvertGameToCamera( to_posx, to_posy );
		local tposx, tposy = MapUnit.getGridTrans( to_unittype, 0, cameraPosX, cameraPosY );
		
		-- 停止移动
		local moveAttr = unitObj:GetComponent("MapUnitMove");
		moveAttr.stat = 0;
		
		-- 形象动作
		local shapeObj = unitObj.transform:FindChild("Shape");
		local childCount = shapeObj.transform.childCount;
		for i = 0, childCount - 1, 1 do
			local charactor = shapeObj.transform:GetChild(i);
			
			-- 单独角色部队
			if charactor:GetComponent("FightCharacter") then
				charactor:GetComponent("FightCharacter"):Siege( Vector3.New( tposx, tposy, tposy ) );
				
			-- 其他部队
			else
				TroopLegionSiege( charactor, Vector3.New( tposx, tposy, tposy ) );
			end
		end
		
	end
	return unitObj;
end

-- 创建城镇
function MapUnit.createTown( recvValue )
	local state 	= recvValue.m_state;
	local posx 		= recvValue.m_posx;
	local posy 		= recvValue.m_posy;
	local nation 	= recvValue.m_char_value[1];
	local townid 	= recvValue.m_short_value[1];
	local type = g_towninfo[townid].type
	local grid = g_towninfo[townid].grid
	local range = g_towninfo[townid].range
	
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
	
	-- 形象
	local shapeSprite = ""
	shapeSprite = MapUnitTownShapeList[type].."_"..nation;
    unitObj.transform:Find("Shape"):GetComponent("SpriteRenderer").sprite = LoadSprite( shapeSprite );
	
	-- 名字
	unitObj.transform:Find("Name"):GetComponent("UIText").text = MapTownName( townid )
	
	-- 范围框
	local uiRange = unitObj.transform:Find("Range");
	if type < 4 then
		uiRange.transform.localScale = Vector3.New( range, range, range );
		uiRange.transform:GetComponent("SpriteRenderer").color = Hex2Color( MapUnitRangeColor[0] )
		SetTrue( uiRange )
	else
		SetFalse( uiRange )
	end
	return unitObj;
end

-- 创建资源点
function MapUnit.createRes( recvValue )
	local state 	= recvValue.m_state;
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
		
	-- 形象
    unitObj.transform:FindChild("Shape"):GetComponent("SpriteRenderer").sprite = LoadSprite( MapUnitResShapeList[restype] );
	
	-- 等级
	unitObj.transform:FindChild("Level"):GetChild(0):GetComponent("CityBoard"):SetNum( level );

	-- 采集状态
	if state == ARMY_STATE_GATHER then
		local cityid = recvValue.m_int_value[1];
		local clubid = recvValue.m_int_value[2];
		-- 我的
		if GetCity().m_cityid == cityid then
			unitObj.transform:FindChild("EffectGather"):GetComponent("SpriteRenderer").sprite = LoadSprite( "UI_signIcon_1" );
			
		-- 联盟的
		elseif GetCity().m_clubid == clubid and clubid > 0 then
			unitObj.transform:FindChild("EffectGather"):GetComponent("SpriteRenderer").sprite = LoadSprite( "UI_signIcon_3" );
		
		-- 敌方的
		else
			unitObj.transform:FindChild("EffectGather"):GetComponent("SpriteRenderer").sprite = LoadSprite( "UI_signIcon_2" );
		end
		unitObj.transform:FindChild("EffectGather").gameObject:SetActive( true );
	else
		unitObj.transform:FindChild("EffectGather").gameObject:SetActive( false );
	end
	return unitObj;
end

-- 刷新下目标怪物
function MapUnit.RefreshTargetMonster( targetLevel )
    for index, unit in pairs( WorldMap.m_nMapUnitList ) do
        if unit.m_type == MAPUNIT_TYPE_MONSTER then
            local unitObj = MapUnit.cache[ unit.m_unit_index ];
            if unitObj ~= nil then
                local level = unit.m_char_value[2];
                -- 是否是使命目标怪物
                if targetLevel == level then
                    unitObj.transform:FindChild("Mark").gameObject:SetActive( true );
                    --unitObj.transform:FindChild("Name"):GetComponent("UIText").color = Color.red;
                else
                    unitObj.transform:FindChild("Mark").gameObject:SetActive( false );
                    --unitObj.transform:FindChild("Name"):GetComponent("UIText").color = Color.white;
                end
            end
        end
    end    
end

-- 创建野怪
function MapUnit.createMonster( recvValue )
	local state 	= recvValue.m_state;
	local posx 		= recvValue.m_posx;
	local posy 		= recvValue.m_posy;
	local action	= recvValue.m_char_value[1];
	local level		= recvValue.m_char_value[2];
	local nameid 	= recvValue.m_short_value[1];
	local shape 	= recvValue.m_short_value[2];
	
	-- 先搜索缓存，如果缓存有，那么就更新
	local unitObj = MapUnit.cache[recvValue.m_unit_index];
	
	-- 先检查对象缓存池是否有空余的
	if unitObj == nil then
		for index, unit in pairs( MapUnit.objectPoolMonster ) do
			if unit and unit.gameObject.activeSelf == false then
				unitObj = unit;
				break;
			end
		end
	end
	
	-- 没有空余的就新创建一个
	if unitObj == nil then
		unitObj = GameObject.Instantiate( MapUnitMonster );
		unitObj.transform:SetParent( MapUnit.unitRoot );
		table.insert( MapUnit.objectPoolMonster, unitObj );
	end
	
	-- 位置
	local cameraPosX, cameraPosY = WorldMap.ConvertGameToCamera( posx, posy );
	posx, posy = MapUnit.getGridTrans( MAPUNIT_TYPE_MONSTER, 0, cameraPosX, cameraPosY );
	unitObj.transform.localPosition = Vector3.New( posx, posy, posy-1 );
	
	-- 名字
	unitObj.transform:FindChild("Info/Name"):GetComponent("UIText").text = GetLocalizeText( nameid );
	
	-- 等级
	unitObj.transform:FindChild("Info/LevelBack/Level"):GetComponent("UIText").text = level;
	
	-- 形象
	local shapeObj = unitObj.transform:FindChild("Shape");
	local childCount = shapeObj.transform.childCount;
	for i = 0, childCount - 1, 1 do
		GameObject.Destroy( shapeObj.transform:GetChild(i).gameObject );
	end
	local charactor = Character.Create( shape );
    charactor.transform:SetParent( unitObj.transform:FindChild("Shape").transform );
	charactor.transform.localPosition = Vector3.New( 0, 0, 0 );
    charactor.transform.localScale = Vector3.one;
	charactor.transform:FindChild("Sprite"):GetComponent("SpriteRenderer").sortingOrder  = 0
	charactor.transform:FindChild("Shadow"):GetComponent("SpriteRenderer").sortingOrder  = 0;
    charactor.defalutDirction = math.random( 0, 1 ) * 2 + 3;
	charactor:Show(true);

    -- 是否是使命目标怪物
    if level == MissionGetTargetMosnterLevel() then
        unitObj.transform:FindChild("Mark").gameObject:SetActive( true );
        --unitObj.transform:FindChild("Name"):GetComponent("UIText").color = Color.red;
    else
        unitObj.transform:FindChild("Mark").gameObject:SetActive( false );
        --unitObj.transform:FindChild("Name"):GetComponent("UIText").color = Color.white;
    end
	
	-- 动作
	-- 待机
	if action == MONSTER_ACTION_IDLE then
	    charactor:GetComponent("Character"):Idle();
		
	-- 攻击	
	elseif action == MONSTER_ACTION_ATTACK then
		charactor:GetComponent("Character"):Attack();
	
	-- 死亡	
	elseif action == MONSTER_ACTION_DEAD then
		charactor:GetComponent("Character"):Die();
		
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

-- 创建城镇范围
function MapUnit.createTownRange( grid, posx, posy, range )
	if MapUnitRoot == nil then
		return;
	end

	local unitObj = GameObject.Instantiate( MapTownRange );
	unitObj.transform:SetParent( MapUnitRoot );
	
	-- 位置
	local cameraPosX, cameraPosY = WorldMap.ConvertGameToCamera( posx, posy );
	posx, posy = MapUnit.getGridTrans( MAPUNIT_TYPE_TOWN, grid, cameraPosX, cameraPosY );
	unitObj.transform.localPosition = Vector3.New( posx, posy, 0 );

	-- 范围
	unitObj.transform:GetComponent("MapBorder"):SetSize( range );
    unitObj.transform:GetComponent("MapBorder"):SetColor( Color.New( 255 / 255, 0 / 255, 0 / 255, 128 / 255 ) );
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
		
	local to_type 		= recvValue.m_short_value[4];
	local to_posx 		= recvValue.m_short_value[5];
	local to_posy		= recvValue.m_short_value[6];
	local move_time 	= statetime;
	local move_needtime = stateduration;

	-- 目的点
	cameraPosX, cameraPosY = WorldMap.ConvertGameToCamera( to_posx, to_posy );
	local tposx, tposy = MapUnit.getGridTrans( to_type, 0, cameraPosX, cameraPosY );
	
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
	local moveAttr = unitObj:GetComponent("MapUnitMove");
	moveAttr.speed = speed;
end

