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
local MapUnitTownRange	= nil;
local MapUnitInited		= false;

-- 玩家每等级的城市外观
MapUnitCityShapeList = {
[0] = "MapUnit_City_level1",
[1] = "MapUnit_City_level1",
[2] = "MapUnit_City_level1",
[3] = "MapUnit_City_level1",
[4] = "MapUnit_City_level4",
[5] = "MapUnit_City_level5",
[6] = "MapUnit_City_level6",
[7] = "MapUnit_City_level7",
[8] = "MapUnit_City_level8",
[9] = "MapUnit_City_level9",
[10] = "MapUnit_City_level10",
[11] = "MapUnit_City_level11",
[12] = "MapUnit_City_level12",
[13] = "MapUnit_City_level13",
[14] = "MapUnit_City_level14",
[15] = "MapUnit_City_level15",
[16] = "MapUnit_City_level16",
[17] = "MapUnit_City_level17",
[18] = "MapUnit_City_level18",
[19] = "MapUnit_City_level19",
[20] = "MapUnit_City_level20",
[21] = "MapUnit_City_level21",
[22] = "MapUnit_City_level22",
[23] = "MapUnit_City_level23",
[24] = "MapUnit_City_level24",
[25] = "MapUnit_City_level25",
[26] = "MapUnit_City_level26",
[27] = "MapUnit_City_level27",
[28] = "MapUnit_City_level27",
[29] = "MapUnit_City_level29",
[30] = "MapUnit_City_level30",
}

-- 资源点形象
MapUnitResShapeList = {
[1] = "MapUnit_Res_Wood",
[2] = "MapUnit_Res_Food",
[3] = "MapUnit_Res_Iron",
[4] = "MapUnit_Res_Mithril",
[5] = "MapUnit_Res_Token",
}

-- 城镇形象
MapUnitTownShapeList = {
[1] = "MapUnit_Npc_Town1",
[2] = "MapUnit_Npc_Town2",
[3] = "MapUnit_Npc_Town3",
}

-- 资源点名称
MapUnitResNameList = {
[1] = 12,
[2] = 13,
[3] = 14,
[4] = 15,
[5] = 102,
[-14] = 2332,
[-15] = 2331,
[-16] = 2333,
[-17] = 2334,
}

-- 城镇状态名称
MapUnitTownStateNameList = {
    [MAPTOWN_STATE_PROTECT]     =   2640,
    [MAPTOWN_STATE_NOPROTECT]   =   2641,
    [MAPTOWN_STATE_OCCUPY]      =   7545,
    [MAPTOWN_STATE_TEMPOCCUPY]  =   2734,
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
MapUnit.objectPoolArmy 			= {}; 	-- 城池
MapUnit.objectPoolRes 			= {}; 	-- 资源田
MapUnit.objectPoolMonster 		= {}; 	-- 野怪
MapUnit.objectPoolTown 			= {}; 	-- 城镇
MapUnit.objectPoolShelter 		= {}; 	-- 庇护所
MapUnit.objectPoolActivity		= {};	-- 活动怪
MapUnit.objectPoolClubTown 		= {};	-- 联盟堡垒
MapUnit.objectPoolClubStore 	= {};	-- 联盟仓库
MapUnit.objectPoolClubJet 		= {};	-- 联盟软泥怪
MapUnit.objectPoolClubRes 		= {};	-- 联盟资源

MapUnit.objectOfficial           = {};   -- 官职

MapUnit.objectKingCity = nil;

-- 魔能塔缓存，坐标索引
MapUnit.cacheKingTower = 
{
    ["477477"] = { fx = "MagicTower_P_0", obj = nil },
    ["483477"] = { fx = "MagicTower_P_1", obj = nil },
    ["483483"] = { fx = "MagicTower_P_2", obj = nil },
    ["477483"] = { fx = "MagicTower_P_3", obj = nil }
}

-- 初始化
function MapUnit.init()
	if MapUnitInited == false then
		-- 预置的初始化
		MapUnitCity 		= LoadPrefab("MapUnitCity");
		MapUnitArmy 		= LoadPrefab("MapUnitArmy");
		MapUnitRes 			= LoadPrefab("MapUnitRes");
		MapUnitMonster 		= LoadPrefab("MapUnitMonster");
		MapUnitTown 		= LoadPrefab("MapUnitTown");
		MapUnitShelter 		= LoadPrefab("MapUnitShelter");
		MapUnitActivity 	= LoadPrefab("MapUnitActivity");
		MapUnitArmyOccupy 	= LoadPrefab("MapUnitArmyOccupy");
		MapUnitClubTown 	= LoadPrefab("MapUnitClubBuilding");
		MapUnitClubStore 	= LoadPrefab("MapUnitClubBuilding");
		MapUnitClubJet 		= LoadPrefab("MapUnitClubJet");
		MapUnitClubRes 		= LoadPrefab("MapUnitClubBuilding");
		MapUnitTownRange	= LoadPrefab("MapBorder");
        MapUnitKingTower    = LoadPrefab("MapUnitKingTower");
		MapUnitInited 	= true;
	end
end

-- 清除
function MapUnit.clear()
	MapUnitCity 		= nil;
	MapUnitArmy 		= nil;
	MapUnitRes 			= nil;
	MapUnitMonster 		= nil;
	MapUnitTown 		= nil;
	MapUnitShelter 		= nil;
	MapUnitActivity		= nil;
	MapUnitArmyOccupy	= nil;
	MapUnitClubTown 	= nil;
	MapUnitClubStore 	= nil;
	MapUnitClubJet 		= nil;
	MapUnitClubRes 		= nil;
	MapUnitInited		= false;
	MapUnit.unitRoot 	= nil;
	MapUnit.objectPoolCity 		= {};
	MapUnit.objectPoolArmy		= {};
	MapUnit.objectPoolRes 		= {};
	MapUnit.objectPoolMonster 	= {};
	MapUnit.objectPoolTown 		= {};
	MapUnit.objectPoolShelter 	= {};
	MapUnit.objectPoolActivity	= {};
	MapUnit.objectPoolClubTown 	= {};
	MapUnit.objectPoolClubStore = {};
	MapUnit.objectPoolClubJet = {};
	MapUnit.objectPoolClubRes 	= {};
	MapUnit.cache 				= {};
    MapUnit.objectOfficial      = {};
    MapUnit.objectKingCity = nil;

    -- 塔
    for i, tower in pairs( MapUnit.cacheKingTower ) do
        tower.obj = nil;
    end
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
		
	-- 资源
	elseif recvValue.m_type == MAPUNIT_TYPE_RES then
		unit = MapUnit.createRes( recvValue );
				
	-- 怪物	
	elseif recvValue.m_type == MAPUNIT_TYPE_MONSTER then
		unit = MapUnit.createMonster( recvValue );
		
	-- 城镇
	elseif recvValue.m_type == MAPUNIT_TYPE_TOWN then
		unit = MapUnit.createTown( recvValue );
		
	-- 庇护所
	elseif recvValue.m_type == MAPUNIT_TYPE_SHELTER then
		unit = MapUnit.createShelter( recvValue );
	
	-- 活动怪
	elseif recvValue.m_type == MAPUNIT_TYPE_ACTIVITY then
		unit = MapUnit.createActivity( recvValue );
		
	-- 联盟城堡
	elseif recvValue.m_type == MAPUNIT_TYPE_CLUB_TOWN then
		unit = MapUnit.createClubTown( recvValue );
		
	-- 联盟仓库
	elseif recvValue.m_type == MAPUNIT_TYPE_CLUB_STORE then
		unit = MapUnit.createClubStore( recvValue );
	
	-- 联盟软泥怪
	elseif recvValue.m_type == MAPUNIT_TYPE_CLUB_JET then
		unit = MapUnit.createClubJet( recvValue );
			
	-- 联盟资源
	elseif recvValue.m_type == MAPUNIT_TYPE_CLUB_RESWOOD or recvValue.m_type == MAPUNIT_TYPE_CLUB_RESFOOD or
        recvValue.m_type == MAPUNIT_TYPE_CLUB_RESIRON or recvValue.m_type == MAPUNIT_TYPE_CLUB_RESMITHRIL then
		unit = MapUnit.createClubRes( recvValue );

    -- 王城
    elseif recvValue.m_type == MAPUNIT_TYPE_KING_CITY then
		unit = MapUnit.createKingCity( recvValue );        

    -- 魔法塔
    elseif recvValue.m_type == MAPUNIT_TYPE_KING_TOWER then
		unit = MapUnit.createKingTower( recvValue ); 
	end
	
	-- 缓存起来，以便删除
	if unit ~= nil then
		unit.gameObject:SetActive( true );
		MapUnit.cache[recvValue.m_unit_index] = unit;
	end

    -- 设置缩放
    MapUnit.SetScale( unit );

    return unit;
end

-- 创建城池
function MapUnit.createCity( recvValue )
	local state 	= recvValue.m_state;
	local name 		= recvValue.m_name;
	local prefix	= recvValue.m_prefix;
	local posx 		= recvValue.m_posx;
	local posy 		= recvValue.m_posy;
	local level 	= recvValue.m_char_value[1];
	local buff 		= recvValue.m_char_value[2];
	local whiteflag = recvValue.m_char_value[3];
	local official  = recvValue.m_char_value[4];
	local clubflag 	= recvValue.m_short_value[1];
	local country 	= recvValue.m_short_value[2];
    local skin      = recvValue.m_short_value[3];
	local clubid	= recvValue.m_int_value[1];
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
	posx, posy = MapUnit.getGridTrans( MAPUNIT_TYPE_CITY, cameraPosX, cameraPosY );
	unitObj.transform.localPosition = Vector3.New( posx, posy, posy );
		
	-- 形象
    if skin ~= 0 and Data.Decorate()[skin] ~= nil and Data.Decorate()[skin].class == 3 then
        unitObj.transform:FindChild("Shape"):GetComponent("SpriteRenderer").sprite = LoadSprite( Data.Decorate()[skin].image );
    else
        unitObj.transform:FindChild("Shape"):GetComponent("SpriteRenderer").sprite = GetCityShape( level );
    end
	
	-- 等级
	unitObj.transform:FindChild("Level"):GetChild(0):GetComponent("CityBoard"):SetNum( level );
	
	-- 白旗联盟和联盟简称拼到一个prefix里了
	if whiteflag and whiteflag > 0 then
		unitObj.transform:FindChild("Title/WhiteFlag").gameObject:SetActive( true );
		local whiteflag_prefix = "";
		local club_prefix = prefix;
		if clubid > 0 then
			prefix = string.sub( club_prefix, 1, 3 );
			whiteflag_prefix = string.sub( club_prefix, recvValue.m_prefixlen-2, recvValue.m_prefixlen );
		else
			whiteflag_prefix = string.sub( club_prefix, 1, 3 );
		end
		if whiteflag_prefix then
			unitObj.transform:FindChild("Title/WhiteFlag"):FindChild("Name"):GetComponent("UIText").text = whiteflag_prefix;
			--[[unitObj.transform:FindChild("WhiteFlag").transform.localPosition = Vector3.New( 0, 1.2, 0 );--]]
		end
	else
		unitObj.transform:FindChild("Title/WhiteFlag").gameObject:SetActive( false );
	end
		
	-- 名字
	if unitindex == WorldMap.m_nMyCityUnitIndex then
		if GetCity().m_clubid > 0 then
			unitObj.transform:FindChild("Content"):FindChild("Name"):GetComponent("UIText").text = "<color=#f9f765>".."["..prefix.."]"..name.."("..GetLocalizeText(1049)..")</color>";
		else
			unitObj.transform:FindChild("Content"):FindChild("Name"):GetComponent("UIText").text = "<color=#f9f765>"..name.."("..GetLocalizeText(1049)..")</color>";
		end
	elseif clubid > 0 then
		unitObj.transform:FindChild("Content"):FindChild("Name"):GetComponent("UIText").text = "["..prefix.."]"..name;
		if clubid == GetCity().m_clubid then
			unitObj.transform:FindChild("Content"):FindChild("Name"):GetComponent("UIText").color = Color.New(165/255, 241/255, 255/255, 1.0)
		else
			unitObj.transform:FindChild("Content"):FindChild("Name"):GetComponent("UIText").color = Color.white;
		end
	else
		unitObj.transform:FindChild("Content"):FindChild("Name"):GetComponent("UIText").text ="<color=white>"..name.."</color>";
	end
	
	-- 国旗
	local countryflagObj = unitObj.transform:FindChild("Content"):FindChild("CountryFlag");
	if country > 0 then
		countryflagObj:GetComponent("Image").sprite = LoadSprite( Localization.countryFlag( country ) );
		countryflagObj.gameObject:SetActive( true );
	else
		countryflagObj.gameObject:SetActive( false );
	end
	
	
	-- 联盟旗帜 如果没有创建 
	local flagObj = unitObj.transform:FindChild("Content"):FindChild("ClubFlag"):FindChild( "Flag" );
    if clubflag >= 0 then
        unitObj.transform:FindChild("Content"):FindChild("ClubFlag").gameObject:SetActive( true );
        if flagObj == nil then
            flagObj = Club_Flag_Create( unitObj.transform:FindChild("Content"):FindChild("ClubFlag") );
            flagObj.name = "Flag";
            flagObj.anchoredPosition = Vector2.zero;
            flagObj.localScale = Vector3.New( 0.3, 0.3, 0.3 );

            -- 更改所有子物体的Layer
            Utils.ChangeLayer( flagObj, 0 );
        end
        -- 低端机不显示特效
        Club_Flag_Set( flagObj, clubflag, Const.quality == 2 );
		flagObj.gameObject:SetActive( true );
        -- 设置下Shader
        local ps = flagObj:FindChild( "Effect" ):GetComponentsInChildren( UnityEngine.ParticleSystemRenderer.GetClassType() );
        for i = 0, ps.Length - 1, 1 do
            ps[i].material.shader = UnityEngine.Shader.Find( "Custom/Particles/Scaler_Additive" );
        end
    else
        unitObj.transform:FindChild("Content"):FindChild("ClubFlag").gameObject:SetActive( false );
    end
	
	-- 当前城池状态
	-- 着火
	if Utils.byteAndOp( state, CITY_STATE_FIRE ) == CITY_STATE_FIRE then
		unitObj.transform:FindChild("Fire").gameObject:SetActive( true );
	else
		unitObj.transform:FindChild("Fire").gameObject:SetActive( false );
	end
	
	-- 战斗
	if Utils.byteAndOp( state, CITY_STATE_FIGHT ) == CITY_STATE_FIGHT then
		unitObj.transform:FindChild("BeatBack").gameObject:SetActive( true );
	else
		unitObj.transform:FindChild("BeatBack").gameObject:SetActive( false );
	end
	
	-- 被围困
	if Utils.byteAndOp( state, CITY_STATE_BESIEGED ) == CITY_STATE_BESIEGED then
		unitObj.transform:FindChild("Besieged").gameObject:SetActive( true );
	else
		unitObj.transform:FindChild("Besieged").gameObject:SetActive( false );
	end
	
	-- BUFF
	if buff > 0 then
		unitObj.transform:FindChild("EffectProtect").gameObject:SetActive( true );
	else
		unitObj.transform:FindChild("EffectProtect").gameObject:SetActive( false );
	end

	-- 官职
	MapUnit.SetOfficial( unitObj, official );
	
	return unitObj;
end

-- 设置官职图标，注意是用prefab名字做索引的。
function MapUnit.SetOfficial( unitObj, official )
    local officialMark = unitObj.transform:FindChild( "Title/OfficialMark" );

    -- 没有官职
    if official <= 0 then
        if officialMark ~= nil then
            officialMark.gameObject:SetActive( false );
            officialMark:SetParent( MapUnit.unitRoot );
        end
        return;
    end

    -- 是否没有这个官职
    if Data.KingOfficialClient()[official] == nil then
        return;
    end

    -- 
    local preName = Data.KingOfficialClient()[official].map_icon;

    -- 是否已存在官职
    if officialMark ~= nil then
        if preName == officialMark:GetChild( 0 ).name then
            -- 相同
            return;
        else
            -- 不相同隐藏
            officialMark.gameObject:SetActive( false );
        end
    end

    -- 初始化表
    if MapUnit.objectOfficial[preName] == nil then
        MapUnit.objectOfficial[preName] = {};
    end

    -- 从缓存中找一个空闲的
    for i, mark in pairs( MapUnit.objectOfficial[preName] ) do
        if mark.gameObject.activeSelf == false then
            officialMark = mark;
        end    
    end

    -- 没有创建一个
    if officialMark == nil then
        officialMark = GameObject.Instantiate( LoadPrefab( preName ) ).transform;
        officialMark.name = "OfficialMark";
        table.insert( MapUnit.objectOfficial[preName], officialMark );
    end

    -- 设置位置
    officialMark.gameObject:SetActive( true );
    officialMark:SetParent( unitObj.transform:FindChild( "Title" ) );
    officialMark.localPosition = Vector3.New( 0, 0.8, -1 );
    officialMark.localScale = Vector3.one;    
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
		local fposx, fposy = MapUnit.getGridTrans( from_type, cameraPosX, cameraPosY );

		-- 目的点
		cameraPosX, cameraPosY = WorldMap.ConvertGameToCamera( to_posx, to_posy );
		local tposx, tposy = MapUnit.getGridTrans( to_type, cameraPosX, cameraPosY );
		
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
		local tposx, tposy = MapUnit.getGridTrans( MAPUNIT_TYPE_ARMY, cameraPosX, cameraPosY );
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
		local tposx, tposy = MapUnit.getGridTrans( to_unittype, cameraPosX, cameraPosY );
		
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
	posx, posy = MapUnit.getGridTrans( MAPUNIT_TYPE_RES, cameraPosX, cameraPosY );
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
	posx, posy = MapUnit.getGridTrans( MAPUNIT_TYPE_MONSTER, cameraPosX, cameraPosY );
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

-- 创建城镇
function MapUnit.createTown( recvValue )
	local state 	= recvValue.m_state;
	local posx 		= recvValue.m_posx;
	local posy 		= recvValue.m_posy;
	local level 	= recvValue.m_char_value[1];
	local shape 	= recvValue.m_char_value[2];
	local nameid 	= recvValue.m_short_value[1];
	
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
	posx, posy = MapUnit.getGridTrans( MAPUNIT_TYPE_TOWN, cameraPosX, cameraPosY );
	unitObj.transform.localPosition = Vector3.New( posx, posy, posy );
	
	-- 名字
	unitObj.transform:FindChild("Content/Name"):GetComponent("UIText").text = GetLocalizeText( nameid );
	
	-- 等级
	unitObj.transform:FindChild("Content/Level/Level"):GetComponent("UIText").text = level;
	
	-- 形象
	local shapeSprite = MapUnitTownShapeList[shape];
	if shapeSprite == nil then
		shapeSprite = MapUnitTownShapeList[1];
	end
    unitObj.transform:FindChild("Shape"):GetComponent("SpriteRenderer").sprite = LoadSprite( shapeSprite );
	
	-- 保护状态
	if state == MAPTOWN_STATE_PROTECT then
		unitObj.transform:FindChild("StateName"):GetComponent("UIText").text = GetLocalizeText( 2640 );
		local StateTime = unitObj.transform:FindChild("StateTime");
		StateTime.gameObject:SetActive( true );
		StateTime:GetComponent( "UITextTimeCountdownEx" ).Duration = recvValue.m_int_value[1]*60;
		StateTime:GetComponent( "UITextTimeCountdownEx" ).PassTime = 1;
		
		
		if recvValue.m_namelen > 0 then
			-- 有目标
			unitObj.transform:FindChild("AttackBack").gameObject:SetActive( true );
			local AttackTime = unitObj.transform:FindChild("AttackTime");
			AttackTime.gameObject:SetActive( true );
			AttackTime:GetComponent( "UITextTimeCountdownEx" ).Duration = recvValue.m_int_value[3];
			AttackTime:GetComponent( "UITextTimeCountdownEx" ).PassTime = 1;
			AttackTime:GetComponent( "UITextTimeCountdownEx" ).formatText = "<color=red>"..T(2820).." "..recvValue.m_name.."</color>";
				
		else
			-- 没目标
			local AttackTime = unitObj.transform:FindChild("AttackTime");
			if recvValue.m_int_value[2] > 0 then
				AttackTime.gameObject:SetActive( true );
				AttackTime:GetComponent( "UITextTimeCountdownEx" ).Duration = recvValue.m_int_value[2]*60;
				AttackTime:GetComponent( "UITextTimeCountdownEx" ).PassTime = 1;
				AttackTime:GetComponent( "UITextTimeCountdownEx" ).formatText = "<color=yellow>"..T(2648)..": {0}".."</color>";
			else
				AttackTime.gameObject:SetActive( false );
				unitObj.transform:FindChild("AttackBack").gameObject:SetActive( false );
			end
			
		end
		
		unitObj.transform:FindChild("HP").gameObject:SetActive( false );
		
	-- 破防状态
	elseif state == MAPTOWN_STATE_NOPROTECT then
		unitObj.transform:FindChild("StateName"):GetComponent("UIText").text = GetLocalizeText( 2641 );
		unitObj.transform:FindChild("StateTime").gameObject:SetActive( false );
		unitObj.transform:FindChild("AttackTime").gameObject:SetActive( false );
		unitObj.transform:FindChild("AttackBack").gameObject:SetActive( false );
		unitObj.transform:FindChild("HP").gameObject:SetActive( false );
		unitObj.transform:FindChild("HP"):GetComponent( "UIProgress" ):SetValue( recvValue.m_int_value[1] / recvValue.m_int_value[2] );
		
	-- 临时占领状态
	elseif state == MAPTOWN_STATE_TEMPOCCUPY then
		unitObj.transform:FindChild("StateName"):GetComponent("UIText").text = "<color=red>"..recvValue.m_name.."["..T(2734).."]".."</color>";
		
		local StateTime = unitObj.transform:FindChild("StateTime");
		StateTime.gameObject:SetActive( true );
		StateTime:GetComponent( "UITextTimeCountdownEx" ).Duration = recvValue.m_int_value[1]*60;
		StateTime:GetComponent( "UITextTimeCountdownEx" ).PassTime = 1;
		unitObj.transform:FindChild("AttackTime").gameObject:SetActive( false );
		unitObj.transform:FindChild("AttackBack").gameObject:SetActive( false );
		
	-- 占领状态
	elseif state == MAPTOWN_STATE_OCCUPY then
		unitObj.transform:FindChild("StateName"):GetComponent("UIText").text = "<color=green>"..recvValue.m_name.."["..T(2735).."]".."</color>";
		
		local StateTime = unitObj.transform:FindChild("StateTime");
		StateTime.gameObject:SetActive( true );
		StateTime:GetComponent( "UITextTimeCountdownEx" ).Duration = recvValue.m_int_value[1]*60;
		StateTime:GetComponent( "UITextTimeCountdownEx" ).PassTime = 1;
		unitObj.transform:FindChild("AttackTime").gameObject:SetActive( false );
		unitObj.transform:FindChild("AttackBack").gameObject:SetActive( false );
	end
	return unitObj;
end

-- 创建王城
function MapUnit.createKingCity( recvValue )
	local state 	= recvValue.m_state;
	local posx 		= recvValue.m_posx;
	local posy 		= recvValue.m_posy;
    local ownClub   = recvValue.m_prefix;
	
	-- 先搜索缓存，如果缓存有，那么就更新
	local unitObj = MapUnit.cache[recvValue.m_unit_index];
	
	-- 先检查对象缓存池是否有空余的
    unitObj = MapUnit.objectKingCity;
	-- 没有空余的就新创建一个
	if unitObj == nil then
		unitObj = GameObject.Instantiate( LoadPrefab( "MapUnitKingCity" ) );
		unitObj.transform:SetParent( MapUnit.unitRoot );
		MapUnit.objectKingCity = unitObj;
	end
	
	-- 位置
	local cameraPosX, cameraPosY = WorldMap.ConvertGameToCamera( posx, posy );
	posx, posy = MapUnit.getGridTrans( MAPUNIT_TYPE_TOWN, cameraPosX, cameraPosY );
	unitObj.transform.localPosition = Vector3.New( posx, posy, posy );
	
	-- 名字
	unitObj.transform:FindChild("Name"):GetComponent("UIText").text = GetLocalizeText( 5010 );

    -- 显示状态
    if state == KINGWAR_STATE_NORMAL then
        -- 保护状态 
		unitObj.transform:FindChild("StateName"):GetComponent("UIText").text = GetLocalizeText( 2640 );
        unitObj.transform:FindChild("EffectProtect").gameObject:SetActive( true );

		local StateTime = unitObj.transform:FindChild("StateTime");
		StateTime.gameObject:SetActive( true );
		StateTime:GetComponent( "UITextTimeCountdownEx" ).Duration = recvValue.m_int_value[2];
		StateTime:GetComponent( "UITextTimeCountdownEx" ).PassTime = 1;

        -- 隐藏特效 
        unitObj.transform:FindChild( "Shape/Effect" ).gameObject:SetActive( false );
    elseif state == KINGWAR_STATE_START then
        -- 王城开启状态
		unitObj.transform:FindChild("StateName"):GetComponent("UIText").text = GetLocalizeText( 2639 );
		unitObj.transform:FindChild("EffectProtect").gameObject:SetActive( false );

		local StateTime = unitObj.transform:FindChild("StateTime");
		StateTime.gameObject:SetActive( true );        
		StateTime:GetComponent( "UITextTimeCountdownEx" ).Duration = recvValue.m_int_value[2];
		StateTime:GetComponent( "UITextTimeCountdownEx" ).PassTime = 1;

        -- 显示特效 
        unitObj.transform:FindChild( "Shape/Effect" ).gameObject:SetActive( true );
    elseif state == KINGWAR_STATE_OFFICIAL then
        -- 封官状态        
		unitObj.transform:FindChild("StateName"):GetComponent("UIText").text = GetLocalizeText( 2643 );
        unitObj.transform:FindChild("EffectProtect").gameObject:SetActive( false );

		local StateTime = unitObj.transform:FindChild("StateTime");
		StateTime.gameObject:SetActive( true );
		StateTime:GetComponent( "UITextTimeCountdownEx" ).Duration = recvValue.m_int_value[2];
		StateTime:GetComponent( "UITextTimeCountdownEx" ).PassTime = 1;

        -- 显示特效 
        unitObj.transform:FindChild( "Shape/Effect" ).gameObject:SetActive( true );
    end

    -- 被联盟占领
    if ownClub ~= nil and ownClub ~= "" then
        local clubName = unitObj.transform:FindChild("ClubName");
        clubName.gameObject:SetActive( true );
        clubName:FindChild( "ClubName" ):GetComponent( UIText.GetClassType() ).text = ownClub;
    else
        local clubName = unitObj.transform:FindChild("ClubName");
        clubName.gameObject:SetActive( false );
    end
	
	return unitObj;
end

-- 创建魔能塔
function MapUnit.createKingTower( recvValue )
	local state 	= recvValue.m_state;
	local posx 		= recvValue.m_posx;
	local posy 		= recvValue.m_posy;
    local ownClub   = recvValue.m_prefix;
    local atkType   = recvValue.m_char_value[1];
    local atkcd     = recvValue.m_int_value[3];
    local index     = posx .. posy;
	
	-- 先搜索缓存，如果缓存有，那么就更新
	local unitObj = MapUnit.cache[recvValue.m_unit_index];
	
	-- 先检查对象缓存池是否有空余的
	if unitObj == nil then
        unitObj = MapUnit.cacheKingTower[index].obj;
	end
	
	-- 没有空余的就新创建一个
	if unitObj == nil then
		unitObj = GameObject.Instantiate( MapUnitKingTower );
		unitObj.transform:SetParent( MapUnit.unitRoot );
		MapUnit.cacheKingTower[index].obj = unitObj;
	end
	
	-- 位置
	local cameraPosX, cameraPosY = WorldMap.ConvertGameToCamera( posx, posy );
	local posx, posy = MapUnit.getGridTrans( MAPUNIT_TYPE_TOWN, cameraPosX, cameraPosY );
	unitObj.transform.localPosition = Vector3.New( posx, posy, posy );
	
	-- 名字
    if ownClub ~= nil and ownClub ~= "" then
        unitObj.transform:FindChild("Name").gameObject:SetActive( true );
	    unitObj.transform:FindChild("Name/Name"):GetComponent("UIText").text = ownClub;
    else
        unitObj.transform:FindChild("Name").gameObject:SetActive( false );
    end

    -- 倒计时进度条
    local progress = unitObj.transform:FindChild( "Progress" ):GetComponent( UIProgress.GetClassType() );
    if atkcd > 0 and recvValue.m_int_value[1] == GetCity().m_clubid then    
	    local stateTime = unitObj.transform:FindChild( "StateTime" ):GetComponent( UITextTimeCountdown.GetClassType() );
        progress.gameObject:SetActive( true );
        local totalTime = tonumber( Global.GetValue( "bullet" .. atkType .."_cd" ) );
	    stateTime:GetComponent( "UITextTimeCountdown" ).Duration = totalTime;
	    stateTime:GetComponent( "UITextTimeCountdown" ).PassTime = totalTime - atkcd;
        stateTime.uiProgress = progress;
        stateTime:Start();

    else
        progress.gameObject:SetActive( false );

    end

    -- 显示状态
    if state == KINGWAR_STATE_NORMAL then
        -- 保护状态 
        unitObj.transform:FindChild("EffectProtect").gameObject:SetActive( true );
    elseif state == KINGWAR_STATE_START then
        -- 王城开启状态
		unitObj.transform:FindChild("EffectProtect").gameObject:SetActive( false );
    end

    -- 攻击状态
    if atkcd > 0 then
        -- 攻击准备中
        local atkFX = MapUnit.getKingTowerFX( unitObj, MapUnit.cacheKingTower[index].fx, atkType );
        atkFX:FindChild( "Atk_" .. atkType .. "/Hold" ).gameObject:SetActive( true );
        atkFX:FindChild( "Atk_" .. atkType .. "/Ready" ).gameObject:SetActive( false );
    elseif atkType > 10 then
        -- 攻击
        MapUnit.kingTowerAttack( unitObj, MapUnit.cacheKingTower[ recvValue.m_posx..recvValue.m_posy ].fx, atkType - 10 );
    elseif atkType == 0 then
        -- 没有攻击
        local atkFX = unitObj.transform:FindChild( "Shape/" .. MapUnit.cacheKingTower[index].fx );
        if atkFX ~= nil then
            for i = 1, 3, 1 do
                atkFX:FindChild( "Atk_" .. i .. "/Hold" ).gameObject:SetActive( false );
                atkFX:FindChild( "Atk_" .. i .. "/Ready" ).gameObject:SetActive( false );
            end
        end
    else
        -- 准备完毕
        local atkFX = MapUnit.getKingTowerFX( unitObj, MapUnit.cacheKingTower[index].fx, atkType );
        atkFX:FindChild( "Atk_" .. atkType .. "/Hold" ).gameObject:SetActive( false );
        atkFX:FindChild( "Atk_" .. atkType .. "/Ready" ).gameObject:SetActive( true );
    end
	
	return unitObj;
end

function MapUnit.getKingTowerFX( unitObj, prefab, atkType )
    local atkFX = unitObj.transform:FindChild( "Shape/" .. prefab );
    -- 攻击准备中
    if atkFX == nil then
        atkFX = GameObject.Instantiate( LoadPrefab( prefab ) ).transform;
        atkFX:SetParent( unitObj.transform:FindChild( "Shape" ) );
        atkFX.localScale = Vector3.one;
        atkFX.localPosition = Vector3.zero;
        atkFX.name = prefab;
    else
        atkFX.gameObject:SetActive( true );
    end

    for i = 1, 3, 1 do
        if i == atkType then
            atkFX:FindChild( "Atk_" .. i ).gameObject:SetActive( true );
        else
            atkFX:FindChild( "Atk_" .. i ).gameObject:SetActive( false );
        end
    end

    return atkFX;
end

function MapUnit.kingTowerAttack( unitObj, prefab, atkType )
    if unitObj == nil then
        return;
    end

    local atkFX = MapUnit.getKingTowerFX( unitObj, prefab, atkType );
    atkFX:FindChild( "Atk_" .. atkType .. "/Hold" ).gameObject:SetActive( false );
    atkFX:FindChild( "Atk_" .. atkType .. "/Ready" ).gameObject:SetActive( false );
    atkFX:FindChild( "Atk_" .. atkType .. "/Attack" ).gameObject:SetActive( true );
end

-- 创建Npc-庇护所
function MapUnit.createShelter( recvValue )
	local state 	= recvValue.m_state;
	local posx 		= recvValue.m_posx;
	local posy 		= recvValue.m_posy;
	
	-- 先搜索缓存，如果缓存有，那么就更新
	local unitObj = MapUnit.cache[recvValue.m_unit_index];
	
	-- 先检查对象缓存池是否有空余的
	if unitObj == nil then
		for index, unit in pairs( MapUnit.objectPoolShelter ) do
			if unit and unit.gameObject.activeSelf == false then
				unitObj = unit;
				break;
			end
		end
	end
	
	-- 没有空余的就新创建一个
	if unitObj == nil then
		unitObj = GameObject.Instantiate( MapUnitShelter );
		unitObj.transform:SetParent( MapUnit.unitRoot );
		table.insert( MapUnit.objectPoolShelter, unitObj );
	end
	
	-- 位置
	local cameraPosX, cameraPosY = WorldMap.ConvertGameToCamera( posx, posy );
	posx, posy = MapUnit.getGridTrans( MAPUNIT_TYPE_SHELTER, cameraPosX, cameraPosY );
	unitObj.transform.localPosition = Vector3.New( posx, posy, posy );
	return unitObj;
end

-- 创建活动怪
function MapUnit.createActivity( recvValue )
	local state 	= recvValue.m_state;
	local posx 		= recvValue.m_posx;
	local posy 		= recvValue.m_posy;
	local action	= recvValue.m_char_value[1];
	local level		= recvValue.m_char_value[2];
	local type		= recvValue.m_char_value[4];
	local grid		= recvValue.m_char_value[5];
	local nameid 	= recvValue.m_short_value[1];
	local shape 	= recvValue.m_short_value[2];

	-- 先搜索缓存，如果缓存有，那么就更新
	local unitObj = MapUnit.cache[recvValue.m_unit_index];
	
	-- 先检查对象缓存池是否有空余的
	if unitObj == nil then
		for index, unit in pairs( MapUnit.objectPoolActivity ) do
			if unit and unit.gameObject.activeSelf == false then
				unitObj = unit;
				break;
			end
		end
	end
	
	-- 没有空余的就新创建一个
	if unitObj == nil then
		unitObj = GameObject.Instantiate( MapUnitActivity );
		unitObj.transform:SetParent( MapUnit.unitRoot );
		table.insert( MapUnit.objectPoolActivity, unitObj );
	end

	-- 位置
	local cameraPosX, cameraPosY = WorldMap.ConvertGameToCamera( posx, posy );
	posx, posy = MapUnit.getGridTrans( -grid, cameraPosX, cameraPosY );
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

	local charactor = FightCharacter.Create( shape );
	charactor.transform:SetParent( unitObj.transform:FindChild("Shape").transform );
	charactor.transform.localPosition = Vector3.New( 0, 0, 0 );
    charactor.transform.localScale = Vector3.one;
	charactor.transform:FindChild("Sprite"):GetComponent("SpriteRenderer").sortingOrder  = 0
    local shadow = charactor.transform:FindChild("Shadow"):GetComponent("SpriteRenderer");
    if shadow ~= nil then
	    charactor.transform:FindChild("Shadow"):GetComponent("SpriteRenderer").sortingOrder  = 0;
    end
	charactor.defalutDirction = math.random( 0, 1 ) * 2 + 3;
	charactor:Show(true)
			
	-- 动作
	-- 待机
	if action == MONSTER_ACTION_IDLE then
		charactor:GetComponent("FightCharacter"):Idle();
		
	-- 攻击	
	elseif action == MONSTER_ACTION_ATTACK then
		charactor:GetComponent("FightCharacter"):AttackAt( charactor.transform.position );
	
	-- 死亡	
	elseif action == MONSTER_ACTION_DEAD then
		charactor:GetComponent("FightCharacter"):Die();
		
	end
	
	return unitObj;	
end

-- 联盟堡垒
function MapUnit.createClubTown( recvValue )
	local state 	= recvValue.m_state;
	local posx 		= recvValue.m_posx;
	local posy 		= recvValue.m_posy;

	-- 先搜索缓存，如果缓存有，那么就更新
	local unitObj = MapUnit.cache[recvValue.m_unit_index];
	
	-- 先检查对象缓存池是否有空余的
	if unitObj == nil then
		for index, unit in pairs( MapUnit.objectPoolClubTown ) do
			if unit and unit.gameObject.activeSelf == false then
				unitObj = unit;
				break;
			end
		end
	end
	
	-- 没有空余的就新创建一个
	if unitObj == nil then
		unitObj = GameObject.Instantiate( MapUnitClubTown );
        unitObj.transform:FindChild( "Shape" ):GetComponent( "SpriteRenderer" ).sprite = LoadSprite( "MapUnit_ClubTown" );
		unitObj.transform:SetParent( MapUnit.unitRoot );
		table.insert( MapUnit.objectPoolClubTown, unitObj );
	end
	
	-- 位置
	local cameraPosX, cameraPosY = WorldMap.ConvertGameToCamera( posx, posy );
	posx, posy = MapUnit.getGridTrans( MAPUNIT_TYPE_CLUB_TOWN, cameraPosX, cameraPosY );
	unitObj.transform.localPosition = Vector3.New( posx, posy, posy );
	
	-- 联盟ID
	local color = Color.New( 255 / 255, 0 / 255, 0 / 255, 128 / 255 );
	local clubid = recvValue.m_int_value[1];
	if clubid == GetCity().m_clubid then
		color = Color.New( 0 / 255, 0 / 255, 255 / 255, 128 / 255 );
	end
	
	-- 建造中显示范围
    unitObj:SetActive( true );
    local range = unitObj.transform:FindChild( "Range" );
    if recvValue.m_state == 1 then
        if range == nil then
	        range = GameObject.Instantiate( MapUnitTownRange ).transform;
            range.name = "Range";
	        range:SetParent( unitObj.transform );
            range.localPosition = Vector3.zero;
            range.gameObject:SetActive( true );
	
	        -- 范围
	        range = range:GetComponent("MapBorder");
            range:SetColor( Color.New( 1, 1, 1, 0.5 ) );
            range:PlaySize( tonumber( Global.GetValue( "club_town_range" ) ) );
        else
            range.gameObject:SetActive( true );
        end
    elseif range ~= nil then
        range.gameObject:SetActive( false );
    end
    
    ---
	MapUnit.SetClubBuildingState( unitObj, recvValue );

	-- 名字
	unitObj.transform:FindChild("NameContent/Name"):GetComponent("UIText").text = recvValue.m_name..T( 2327 );
		
	return unitObj;
end

-- 联盟仓库	
function MapUnit.createClubStore( recvValue )
	local state 	= recvValue.m_state;
	local posx 		= recvValue.m_posx;
	local posy 		= recvValue.m_posy;
	
	-- 先搜索缓存，如果缓存有，那么就更新
	local unitObj = MapUnit.cache[recvValue.m_unit_index];
	
	-- 先检查对象缓存池是否有空余的
	if unitObj == nil then
		for index, unit in pairs( MapUnit.objectPoolClubStore ) do
			if unit and unit.gameObject.activeSelf == false then
				unitObj = unit;
				break;
			end
		end
	end
	
	-- 没有空余的就新创建一个
	if unitObj == nil then
		unitObj = GameObject.Instantiate( MapUnitClubStore );
        unitObj.transform:FindChild( "Shape" ):GetComponent( "SpriteRenderer" ).sprite = LoadSprite( "MapUnit_ClubStore" );
		unitObj.transform:SetParent( MapUnit.unitRoot );
		table.insert( MapUnit.objectPoolClubStore, unitObj );
	end

    ---
    MapUnit.SetClubBuildingState( unitObj, recvValue );

	-- 名字
	unitObj.transform:FindChild("NameContent/Name"):GetComponent("UIText").text = "["..recvValue.m_name.."]"..T( 2330 );
	
	-- 位置
	local cameraPosX, cameraPosY = WorldMap.ConvertGameToCamera( posx, posy );
	posx, posy = MapUnit.getGridTrans( MAPUNIT_TYPE_CLUB_STORE, cameraPosX, cameraPosY );
	unitObj.transform.localPosition = Vector3.New( posx, posy, posy );
	return unitObj;
end

-- 联盟软泥怪
function MapUnit.createClubJet( recvValue )
	local state 	= recvValue.m_state;
	local posx 		= recvValue.m_posx;
	local posy 		= recvValue.m_posy;
    local targetX      = recvValue.m_short_value[1];
    local targetY      = recvValue.m_short_value[2];
	
	-- 先搜索缓存，如果缓存有，那么就更新
	local unitObj = MapUnit.cache[recvValue.m_unit_index];
	
	-- 先检查对象缓存池是否有空余的
	if unitObj == nil then
		for index, unit in pairs( MapUnit.objectPoolClubJet ) do
			if unit and unit.gameObject.activeSelf == false then
				unitObj = unit;
				break;
			end
		end
	end
	
	-- 没有空余的就新创建一个
	if unitObj == nil then
		unitObj = GameObject.Instantiate( MapUnitClubJet );
		unitObj.transform:SetParent( MapUnit.unitRoot );
		table.insert( MapUnit.objectPoolClubJet, unitObj );
	end

    -- 加个属性，CD完成时间
    recvValue.m_moveCDEndTime = GetServerTime() + recvValue.m_int_value[2];

    -- 设下联盟建筑基本信息
    MapUnit.SetClubBuildingState( unitObj, recvValue );

	-- 名字
	unitObj.transform:FindChild("NameContent/Name"):GetComponent("UIText").text = "["..recvValue.m_name.."]"..T( 2829 );
	
	-- 位置
	local cameraPosX, cameraPosY = WorldMap.ConvertGameToCamera( posx, posy );
	posx, posy = MapUnit.getGridTrans( MAPUNIT_TYPE_CLUB_JET, cameraPosX, cameraPosY );
	unitObj.transform.localPosition = Vector3.New( posx, posy, posy );

    -- 动作
    local char = unitObj.transform:FindChild( "Jet" ):GetComponent( "FightCharacter" );
    if targetX == nil or targetX == -1 then
        -- 没有攻击动作
        char:Stop();
    else
        -- 攻击目标城池
        local tarx, tary = WorldMap.ConvertGameToCamera( targetX, targetY );
        tarx, tary = MapUnit.getGridTrans( MAPUNIT_TYPE_CITY, tarx, tary );
        char:AttackAt( Vector3.New( tarx, tary, 0 ) );
    end	    
    
    return unitObj;
end

-- 设置联盟建筑
function MapUnit.SetClubBuildingState( unitObj, recvValue )
    local TimeBackObj = unitObj.transform:FindChild("TimeBack");
	local TimeObj = unitObj.transform:FindChild("Time");
	local OccupyClubObj = unitObj.transform:FindChild("OccupyClub");    

    -- 名字颜色
    if recvValue.m_int_value[1] == GetCity().m_clubid then
        unitObj.transform:FindChild( "NameContent/Name" ):GetComponent( "UIText" ).color = Color.New( 165/255, 241/255, 255/255, 1.0 );
    else
        unitObj.transform:FindChild( "NameContent/Name" ):GetComponent( "UIText" ).color = Color.white;
    end
    
    -- 建筑状态    
    OccupyClubObj.gameObject:SetActive( false );
	TimeBackObj.gameObject:SetActive( false );
    TimeObj.gameObject:SetActive( false );

	if recvValue.m_int_value[3] ~= nil and recvValue.m_int_value[3] > 0 then -- 被占领，显示占领的联盟
		TimeBackObj.gameObject:SetActive( true );
		OccupyClubObj.gameObject:SetActive( true );
		OccupyClubObj:GetComponent( "UIText" ).text = "<color=red>".."["..recvValue.m_prefix.."]"..T( 2734 ).."</color>";

    elseif recvValue.m_state == 1 then
        if recvValue.m_int_value[1] == GetCity().m_clubid then     -- 建造中，只能看到自己联盟的
		    TimeBackObj.gameObject:SetActive( true );
		    TimeObj.gameObject:SetActive( true );
		    TimeObj:GetComponent( "UITextTimeCountdown" ).formatText = T(1086)..": {0}";
		    TimeObj:GetComponent( "UITextTimeCountdown" ).Duration = recvValue.m_int_value[2];
		    TimeObj:GetComponent( "UITextTimeCountdown" ).PassTime = 0;
        else
            OccupyClubObj.gameObject:SetActive( true );   
            OccupyClubObj:GetComponent( "UIText" ).text = T(1086);
        end
    end

    -- 背景板
    if recvValue.m_state ~= 1 and ( recvValue.m_int_value[3] == nil or recvValue.m_int_value[3] <= 0 ) then
        TimeBackObj.gameObject:SetActive( false );
    end
end

-- 联盟资源	
function MapUnit.createClubRes( recvValue )
	local state 	= recvValue.m_state;
	local posx 		= recvValue.m_posx;
	local posy 		= recvValue.m_posy;
	
	-- 先搜索缓存，如果缓存有，那么就更新
	local unitObj = MapUnit.cache[recvValue.m_unit_index];
	
	-- 先检查对象缓存池是否有空余的
	if unitObj == nil then
		for index, unit in pairs( MapUnit.objectPoolClubRes ) do
			if unit and unit.gameObject.activeSelf == false then
				unitObj = unit;
				break;
			end
		end
	end
	
	-- 没有空余的就新创建一个
	if unitObj == nil then
		unitObj = GameObject.Instantiate( MapUnitClubRes );
		unitObj.transform:SetParent( MapUnit.unitRoot );
		table.insert( MapUnit.objectPoolClubRes, unitObj );
	end
	
	-- 位置
	local cameraPosX, cameraPosY = WorldMap.ConvertGameToCamera( posx, posy );
	posx, posy = MapUnit.getGridTrans( recvValue.m_type, cameraPosX, cameraPosY );
	unitObj.transform.localPosition = Vector3.New( posx, posy, posy );

    MapUnit.SetClubBuildingState( unitObj, recvValue );
		
	-- 形象
    unitObj.transform:FindChild("Shape"):GetComponent("SpriteRenderer").sprite = LoadSprite( MapUnitClubResList_Type[ recvValue.m_type ].shape );

	-- 名字
	unitObj.transform:FindChild("NameContent/Name"):GetComponent("UIText").text = "["..recvValue.m_name.."]"..T( MapUnitClubResList_Type[ recvValue.m_type ].name );

	return unitObj;
end

-- 城镇边界线
function MapUnit.createRange( unittype, posx, posy, range, play )
	if MapUnitRoot == nil then
		return;
	end
	local unitObj = GameObject.Instantiate( MapUnitTownRange );
	unitObj.transform:SetParent( MapUnitRoot );
	
	-- 位置
	local cameraPosX, cameraPosY = WorldMap.ConvertGameToCamera( posx, posy );
	posx, posy = MapUnit.getGridTrans( unittype, cameraPosX, cameraPosY );
	unitObj.transform.localPosition = Vector3.New( posx, posy, 0 );
	
	-- 范围
	unitObj.transform:GetComponent("MapBorder"):SetSize( range );
    unitObj.transform:GetComponent("MapBorder"):SetColor( Color.New( 255 / 255, 0 / 255, 0 / 255, 128 / 255 ) );
	
	-- 播放
	if play then
		unitObj.transform:GetComponent("MapBorder"):PlaySize( range );
	end
end

-- 创建城镇范围
function MapUnit.createTownRange( recvValue )
	if recvValue == nil then
		system_askinfo( ASKINFO_WORLDMAP, "", 14 );
		return;
	end
	for tmpi=1, recvValue.m_count, 1 do
		MapUnit.createRange( MAPUNIT_TYPE_TOWN, recvValue.m_list[tmpi].m_posx, recvValue.m_list[tmpi].m_posy, recvValue.m_list[tmpi].m_range, false );
		MapUnit.createRange( MAPUNIT_TYPE_TOWN, recvValue.m_list[tmpi].m_posx, recvValue.m_list[tmpi].m_posy, recvValue.m_list[tmpi].m_range, true );
	end
end

-- 删除显示单元
function MapUnit.del( unit_index )
	local unit = MapUnit.cache[unit_index];
	if unit ~= nil then
		unit.gameObject:SetActive( false );
		MapUnit.cache[unit_index] = nil;
	end
end

-- 获取占地块
function MapUnit.typeTotype( recvValue )
	if recvValue.m_type == MAPUNIT_TYPE_ACTIVITY then
		if recvValue.m_char_value[5] then
			return -recvValue.m_char_value[5];
		end
	end
	return recvValue.m_type;
end

-- 获取占地块
function MapUnit.getGrid( unittype )
	-- 城池
	if unittype == MAPUNIT_TYPE_CITY then
		return 2;
	-- 部队
	elseif unittype == MAPUNIT_TYPE_ARMY then
		return 1;
	-- NPC
	elseif unittype == MAPUNIT_TYPE_RES then
		return 1;
	-- 怪物	
	elseif unittype == MAPUNIT_TYPE_MONSTER then
		return 1;
	-- 城镇
	elseif unittype == MAPUNIT_TYPE_TOWN then
		return 3;
	-- 庇护所
	elseif unittype == MAPUNIT_TYPE_SHELTER then
		return 2;
	-- 活动怪
	elseif unittype == MAPUNIT_TYPE_ACTIVITY then
		return 2;
	-- 联盟堡垒
	elseif unittype == MAPUNIT_TYPE_CLUB_TOWN then
		return 1;
	-- 联盟仓库
	elseif unittype == MAPUNIT_TYPE_CLUB_STORE then
		return 2;
	-- 联盟软泥怪
	elseif unittype == MAPUNIT_TYPE_CLUB_JET then
		return 1;
    elseif unittype == MAPUNIT_TYPE_CLUB_RESWOOD or unittype == MAPUNIT_TYPE_CLUB_RESFOOD or unittype == MAPUNIT_TYPE_CLUB_RESIRON or unittype == MAPUNIT_TYPE_CLUB_RESMITHRIL then
        return 2;
    elseif unittype == MAPUNIT_TYPE_KING_CITY then
        return 3;
    elseif unittype == MAPUNIT_TYPE_KING_TOWER then
        return 1;
	elseif unittype == -2 then
		return 2;
	elseif unittype == -1 then
		return 1;
	end	
	return 1;
end

-- 根据占地块转换位置坐标
function MapUnit.getGridTrans( unittype, posx, posy )
	local grid = MapUnit.getGrid( unittype );
	if grid == 1 then
		return posx, posy - MAP_TILEHEIGHT/100/2;
	elseif grid == 2 then
		return posx + MAP_TILEWIDTH/100/2, posy - MAP_TILEHEIGHT/100/2;
	elseif grid == 3 then
		return posx, posy - MAP_TILEHEIGHT/100/2;
	end
	return posx, posy;
end
--判断自己的城堡是不是在诅咒之地里
function MapUnit.IsNotInBlastedLands ()
    local gridOffset = { { x=0, y=0 },{ x=0, y=-1 },{ x=1, y=-1 },{ x=1, y=0 },}
    local myX, myY = GetCity().m_posx, GetCity().m_posy;
    local isBoo=false;
    for i = 1, 4, 1 do
		local x = myX + gridOffset[i]["x"];
		local y = myY + gridOffset[i]["y"];

		if x >= 455 and y <= 505 and  x <= 505 and y >= 455 then
		    isBoo=true;
            --print("-----"..x..y);
        else
            isBoo=false;
            --print("@@@@@"..x..y);
	    end
    end
    return isBoo;
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
	local tposx, tposy = MapUnit.getGridTrans( to_type, cameraPosX, cameraPosY );
	
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

-- 野怪动作
function MapUnit.monsterAction( unit_index, action )
	local recvValue = WorldMap.m_nMapUnitList[unit_index];
	if recvValue == nil then
		return;
	end
	
	local unitObj = MapUnit.cache[unit_index];
	if unitObj == nil then
		return;
	end
	
	local shapeObj = unitObj.transform:FindChild("Shape");
	local childCount = shapeObj.transform.childCount;
	
	-- 待机
	if action == MONSTER_ACTION_IDLE then
		for i = 0, childCount - 1, 1 do
			local charactor = shapeObj.transform:GetChild(i);
			charactor:GetComponent("Character"):Idle();
		end
	
	-- 攻击	
	elseif action == MONSTER_ACTION_ATTACK then
		for i = 0, childCount - 1, 1 do
			local charactor = shapeObj.transform:GetChild(i);
			charactor:GetComponent("Character"):Attack();
		end
        fruit.audioManager:Play(85);
	
	-- 死亡	
	elseif action == MONSTER_ACTION_DEAD then
		for i = 0, childCount - 1, 1 do
			local charactor = shapeObj.transform:GetChild(i);
			charactor:GetComponent("Character"):Die();
		end

        -- 放特效
        -- 原地特效
        local obj = GameObject.Instantiate( LoadPrefab( "Effect_Importance_Spiritism" ) );
        obj.transform:SetParent( unitObj.transform.parent );
        obj.transform.localScale = Vector3.one;
        obj.transform.position = unitObj.transform.position;
        GameObject.Destroy( obj, 6 );
        fruit.audioManager:Play(63);
	end
end

-- 活动怪动作
function MapUnit.activityAction( unit_index, action )
	local recvValue = WorldMap.m_nMapUnitList[unit_index];
	if recvValue == nil then
		return;
	end
	
	local unitObj = MapUnit.cache[unit_index];
	if unitObj == nil then
		return;
	end
	
	local shapeObj = unitObj.transform:FindChild("Shape");
	local childCount = shapeObj.transform.childCount;

	-- 待机
	if action == MONSTER_ACTION_IDLE then
		for i = 0, childCount - 1, 1 do
			local charactor = shapeObj.transform:GetChild(i);
			charactor:GetComponent("FightCharacter"):Stop();
		end
	
	-- 攻击	
	elseif action == MONSTER_ACTION_ATTACK then
		for i = 0, childCount - 1, 1 do
			local charactor = shapeObj.transform:GetChild(i);
			charactor:GetComponent("FightCharacter"):AttackAt( charactor.transform.position );
		end
	
	-- 死亡	
	elseif action == MONSTER_ACTION_DEAD then
		for i = 0, childCount - 1, 1 do
			local charactor = shapeObj.transform:GetChild(i);
			charactor:GetComponent("FightCharacter"):Die();
		end
	end
end

-- 更新缩放
local m_ScaleRatio = 32;
function MapUnit.ResetScale( cameraPosX, cameraPosY )
    for i, unitObj in pairs( MapUnit.cache ) do
        MapUnit.SetScale( unitObj, cameraPosX, cameraPosY );
    end
end
-- 新unit缩放
function MapUnit.SetScale( unitObj, cameraPosX, cameraPosY )
    if unitObj == nil then
        return;
    end
    if cameraPosX == nil then
        cameraPosX, cameraPosY = WorldMap.m_nLastCameraX, WorldMap.m_nLastCameraY;
    end
    local offset = unitObj.transform.position.y - cameraPosY + 1.6;
    if unitObj.transform.childCount > 0 then
        if offset > 0 then
            unitObj.transform:GetChild( 0 ).localScale = Vector3.New( 1, 1 - offset / m_ScaleRatio, 1 );
        else
            unitObj.transform:GetChild( 0 ).localScale = Vector3.New( 1, 1, 1 );
        end
    end
end