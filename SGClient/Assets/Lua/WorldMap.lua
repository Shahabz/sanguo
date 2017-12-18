-- 世界地图
MAP_TILEWIDTH 		= 256;	-- 一块格子有多少像素	
MAP_TILEHEIGHT 		= 128;	-- 一块格子有多少像素

TMX_WIDTH 			= 100;  -- 一块tmx地图大小
TMX_HEIGHT 			= 100;  -- 一块tmx地图大小

WORLDMAP_ZORDER_CLICKMOD	=	-1000; 	-- 点击地图选择框层级
WORLDMAP_ZORDER_MARCHROUTE	=	-2000;	-- 行军路线层级
WORLDMAP_ZORDER_ARMY		=	-2001;	-- 部队层级
WORLDMAP_ZORDER_CAMERA		=	-10000;	-- 摄像机层级

WORLDMAP_MODE_NORMAL	=	0; -- 缺省模式
WORLDMAP_MODE_FOLLOW	=	1; -- 跟随模式

-- 世界地图
WorldMap = {}
WorldMap.m_nMaxWidth 			= 500;    -- 世界地图大小（格子数）
WorldMap.m_nMaxHeight 			= 500;    -- 世界地图大小（格子数）
WorldMap.m_nAreaWidth 			= 0;    -- 区域大小（一个区域几个地图格子）
WorldMap.m_nAreaHeight 			= 0;    -- 区域大小（一个区域几个地图格子）
WorldMap.m_nAreaXNum 			= 0;    -- 一个地图有多少区域
WorldMap.m_nAreaYNum 			= 0;    -- 一个地图有多少区域
WorldMap.m_nLastAreaIndex		= -1;   -- 上一次发送区域
WorldMap.m_nMapUnitList			= {};   -- 显示对象缓存链表
WorldMap.m_nMyCityPosx			= 0;    -- 玩家主城位置（格子）
WorldMap.m_nMyCityPosy			= 0;    -- 玩家主城位置（格子）
WorldMap.m_nMyCityCameraX 		= 0;    -- 玩家主城位置（屏幕坐标）
WorldMap.m_nMyCityCameraY		= 0;    -- 玩家主城位置（屏幕坐标）
WorldMap.m_nMyCityUnitIndex 	= -1;   -- 玩家主城的显示索引
WorldMap.m_nMyCityState			= 0;	-- 玩家主城状态
WorldMap.m_nLastCameraX 		= 0; 	-- 上次摄像机位置（屏幕坐标）
WorldMap.m_nLastCameraY			= 0; 	-- 上次摄像机位置（屏幕坐标）
WorldMap.m_nLastCameraGameX 	= -1; 	-- 上次摄像机位置（格子坐标）
WorldMap.m_nLastCameraGameY 	= -1; 	-- 上次摄像机位置（格子坐标）
WorldMap.m_nLastTouchGameCoorX 	= 0; 	-- 上次点击位置（格子）
WorldMap.m_nLastTouchGameCoorY	= 0; 	-- 上次点击位置（格子）
WorldMap.m_nLastTouchUnitIndex	= -1; 	-- 上次点击单位
WorldMap.m_nLastTouchArmyIndex	= -1; 	-- 上次点击部队
WorldMap.m_nLastGotoUnitIndex	= -1;	-- 等待跳转的索引
WorldMap.m_nLastGotoPosX		= -1;	-- 等待跳转的位置
WorldMap.m_nLastGotoPosY		= -1;	-- 等待跳转的位置
WorldMap.m_nZoneID				= 0;	-- 当前地图
WorldMap.m_nMode 				= WORLDMAP_MODE_NORMAL;

local TmxShowThreshold = 24;
local TmxShowList = { 0, 0, 0, 0 };
local TmxLastList = { 0, 0, 0, 0 };
local m_LastPosX;
local m_LastPosY;
local m_Faded = false;
local m_FadeStopCount = 0;
local m_MoveType = 0;

-- 地图区域
Area = {}
-- 根据世界坐标获取区域编号
function Area.GetIndex( posx, posy )
	local areax, areay;
	if posx >= WorldMap.m_nMaxWidth then
		posx = WorldMap.m_nMaxWidth - 1;
	end
	if posy >= WorldMap.m_nMaxHeight then
		posy = WorldMap.m_nMaxHeight - 1;
	end
	areax = math.modf( (posx+1)/WorldMap.m_nAreaWidth );
	areay = math.modf( (posy+1)/WorldMap.m_nAreaHeight );
	
	if areax >= WorldMap.m_nAreaXNum then
		areax = WorldMap.m_nAreaXNum - 1;
	end
	if areay >= WorldMap.m_nAreaYNum then
		areay = WorldMap.m_nAreaYNum - 1;
	end
		
	return math.modf( areay*(WorldMap.m_nAreaXNum) + areax );
end

-- 显示单元创建队列
local procQueue = Queue.new();
-- 部队单拿出来一个队列
local procQueueArmy = Queue.new();
local m_updateWaitFrame = 0;

-- 地图对象组件
local WorldMapPrefab	= nil;	-- 地图根
local MapTmxRoot		= nil;	-- tmx地图根节点
MapUnitRoot				= nil;	-- 所有显示对象的根节点
MapArmyRoot				= nil;	-- 部队单独拿出
local MapLineRoot		= nil;	-- 所有线根节点
local MapClickEffect	= nil;	-- 点击特效
local MapClickMod		= nil;	-- 操作界面
local MapCamera 		= nil;	-- 地图摄像机
local MapTmx            = {};   -- 地图池，只实例化4个
CityArmyGroupClickMod	= nil;	-- 
local m_RecvValue = nil;

-- 世界地图信息
function WorldMap.Recv( recvValue )
    m_RecvValue = recvValue;
	if GameManager.currentScence == "worldmap" then
		WorldMap.CameraSetPosition( MAPUNIT_TYPE_CITY, recvValue.m_target_posx, recvValue.m_target_posy );
		--WorldMap.ViewChangeSec();
		return;
	end
	
	WorldMap.m_nAreaWidth			= 	recvValue.m_area_width;
	WorldMap.m_nAreaHeight			= 	recvValue.m_area_height;
	WorldMap.m_nAreaXNum			= 	recvValue.m_map_area_xnum;
	WorldMap.m_nAreaYNum			= 	recvValue.m_map_area_ynum;
	WorldMap.m_nMyCityPosx			= 	recvValue.m_my_city_posx;
	WorldMap.m_nMyCityPosy			= 	recvValue.m_my_city_posy;
	WorldMap.m_nMyCityUnitIndex 	= 	recvValue.m_my_city_unit_index;
	WorldMap.m_nMyMonsterKillLevel	=	recvValue.m_monster_killlevel;
	WorldMap.m_nMyCityState			=	recvValue.m_citystate;
	WorldMap.m_nLastAreaIndex 		= 	-1;
	WorldMap.m_nLastGotoPosX		= 	recvValue.m_target_posx;
	WorldMap.m_nLastGotoPosY		= 	recvValue.m_target_posy;
	WorldMap.m_nMapUnitList			= {};
	MapUnit.init();
	
	-- 改变场景
	GameManager.ChangeScence( "worldmap" )
end

-- 清空
function WorldMap.Clear()
	WorldMap.m_nLastAreaIndex = -1;
	WorldMap.m_nLastTouchArmyIndex = -1;
	WorldMap.m_nLastTouchUnitIndex = -1;
	WorldMap.m_nLastGotoUnitIndex = -1;
    WorldMap.m_nLastCameraGameX = -1;
    WorldMap.m_nLastCameraGameY = -1;
    WorldMap.m_nMapUnitList = {};
	WorldMap.QueueClear();
	MapUnit.clear();
	MapTile.clear();
	MapMarchRoute.clear();
	MapUnitRoot = nil;
	MapArmyRoot = nil;
    MapCamera = nil;
	WorldMap.SendAreaIndex( WorldMap.m_nLastAreaIndex, -1, -1 )
    
    m_LastPosX = nil;
    m_LastPosY = nil;
	--collectgarbage("collect");
end

-- 前往世界地图
function WorldMap.GotoWorldMap( posx, posy )
	-- m_to_posx=0,m_to_posy=0,
	local sendValue = {};
	sendValue.m_to_posx = posx;
	sendValue.m_to_posy = posy;
	netsend_worldmapask_C( sendValue )
end

-- 返回城池
function WorldMap.ReturnCity()
	GameManager.ChangeScence( "city" )
	TmxLastList = { 0, 0, 0, 0 };
	WorldMap.Clear()
	MainDlgShowCity()
	-- 任务检查
	QuestClickCheck( 3 )
end

-- 发送客户端当前区域索引
function WorldMap.SendAreaIndex( areaindex, posx, posy )
	-- m_areaindex=0,
	local sendValue = {};
	sendValue.m_areaindex = areaindex;
	sendValue.m_posx = posx;
	sendValue.m_posy = posy;
	netsend_worldmapareaindex_C( sendValue );
end

-- 确定哪个图显示
function WorldMap.RefreshShow( gamePosX, gamePosY )
    TmxShowList = { 0, 0, 0, 0 };

    -- 小格坐标
	local localPosX = math.floor( math.mod( gamePosX, TMX_WIDTH ) );
	local localPosY = math.floor( math.mod( gamePosY, TMX_HEIGHT ) );

    -- 哪格地图
    local mapX = math.floor( gamePosX / TMX_WIDTH );
    local mapY = math.floor( gamePosY / TMX_HEIGHT );
    TmxShowList[1] = { mapX, mapY };

    local index = 2;
    -- 判断小格地图边缘
    -- X轴
    if localPosX < TmxShowThreshold then
        TmxShowList[index] = { mapX - 1, mapY };
        index = index + 1;
    elseif localPosX > TMX_WIDTH - TmxShowThreshold then
        TmxShowList[index] = { mapX + 1, mapY };
        index = index + 1;
    end
    
    -- Y轴
    if localPosY < TmxShowThreshold then
        TmxShowList[index] = { mapX, mapY - 1 };
        index = index + 1;
    elseif localPosY > TMX_HEIGHT - TmxShowThreshold then
        TmxShowList[index] = { mapX, mapY + 1 };
        index = index + 1;
    end
    
    -- XY轴上下
    if localPosX < TmxShowThreshold and localPosY < TmxShowThreshold then
        TmxShowList[index] = { mapX - 1, mapY - 1 };
        index = index + 1;
    elseif localPosX > TMX_WIDTH - TmxShowThreshold and localPosY > TMX_HEIGHT - TmxShowThreshold then
        TmxShowList[index] = { mapX + 1, mapY + 1 };
        index = index + 1;
    end    

    -- XY轴左右
    if localPosX < TmxShowThreshold and localPosY > TMX_HEIGHT - TmxShowThreshold then
        TmxShowList[index] = { mapX - 1, mapY + 1 };
        index = index + 1;
    elseif localPosX > TMX_WIDTH - TmxShowThreshold and localPosY < TmxShowThreshold then
        TmxShowList[index] = { mapX + 1, mapY - 1 };
        index = index + 1;
    end

    -- 刷新地图位置
    local changed = false;
    for i = 1, 4, 1 do
        if TmxShowList[i] ~= 0 and TmxLastList[i] ~= 0 and TmxLastList[i][1] == TmxShowList[i][1] and TmxLastList[i][2] == TmxShowList[i][2] then
			
        elseif TmxShowList[i] ~= 0 then
            MapTmx[i]:SetActive( true );
			MapTmx[i].transform:Find( "road" ).gameObject:SetActive( true );
			local x = WorldMap.m_nMaxWidth * MAP_TILEWIDTH / 2 / 100 + MAP_TILEWIDTH * TMX_WIDTH * ( TmxShowList[i][1] - TmxShowList[i][2] - 1 ) / 2 / 100
			local y = - MAP_TILEHEIGHT * TMX_HEIGHT * ( TmxShowList[i][2] + TmxShowList[i][1] ) / 2 / 100;
			if x >= 511 and x <= 513 and y >= -257 and y <= -255 then
				MapTmx[i].transform:Find( "road" ).gameObject:SetActive( false ); -- 中间
			else
				MapTmx[i].transform:Find( "road" ).gameObject:SetActive( true );
			end
            MapTmx[i].transform.localPosition = Vector3.New( x, y, 0 );

            changed = true;
        else
            MapTmx[i]:SetActive( false );
            changed = true;
        end
    end

    if changed then
        TmxLastList = clone( TmxShowList );
    end
end
	
-- 世界地图启动
function WorldMap.Start( Prefab )
	-- 初始化根节点
	WorldMapPrefab	= Prefab;
    MapUnit.unitRoot = Prefab.transform:Find("MapUnitRoot");

	-- 获取组件
	MapTmxRoot				= WorldMapPrefab.transform:Find( "MapTmxRoot" );
	MapUnitRoot				= WorldMapPrefab.transform:Find( "MapUnitRoot" );
	MapLineRoot				= WorldMapPrefab.transform:Find( "MapLineRoot" );
	MapArmyRoot				= WorldMapPrefab.transform:Find( "MapArmyRoot" );
	MapClickEffect			= WorldMapPrefab.transform:Find( "MapClickEffect" );
	MapClickMod				= WorldMapPrefab.transform:Find( "MapClickMod" );
	MapCamera				= WorldMapPrefab.transform:Find( "MapCamera" );
	CityArmyGroupClickMod	= WorldMapPrefab.transform:Find( "CityArmyGroupClickMod" ).transform:GetComponent("UIMod");
	
	-- 根据地图大小动态加载N*N个Tmx地图
	local TmxMapPrefab = LoadPrefab( "TmxMap" );
--	local rownum = math.floor( WorldMap.m_nMaxWidth/TMX_WIDTH );
--	local colnum = math.floor( WorldMap.m_nMaxHeight/TMX_HEIGHT );
--	for j=1, colnum, 1 do
--		for i=1, rownum, 1 do
--			local tmx = GameObject.Instantiate( TmxMapPrefab );
--			tmx.transform:SetParent( MapTmxRoot );
--			tmx.transform.localScale = Vector3.one;
--			tmx.transform.localPosition = Vector3.New( (WorldMap.m_nMaxWidth*MAP_TILEWIDTH)/2/100 + (i-2)*TMX_WIDTH*MAP_TILEWIDTH/2/100 - (j-1)*TMX_WIDTH*MAP_TILEWIDTH/2/100,
--												   -(i-1)*TMX_HEIGHT*MAP_TILEHEIGHT/2/100 - (j-1)*TMX_HEIGHT*MAP_TILEHEIGHT/2/100, 0 );

--		end
--	end

    -- 只实例化4个
    for i = 1, 4, 1 do
        MapTmx[i] = GameObject.Instantiate( TmxMapPrefab );
		MapTmx[i].transform:SetParent( MapTmxRoot );
		MapTmx[i].transform.localScale = Vector3.one;
        MapTmx[i].gameObject:SetActive( false );
    end
    	
	-- 设置摄像机初始位置为我的城池
	WorldMap.m_nMyCityCameraX, WorldMap.m_nMyCityCameraY = WorldMap.ConvertGameToCamera( WorldMap.m_nMyCityPosx, WorldMap.m_nMyCityPosy );
	WorldMap.m_nMyCityCameraX, WorldMap.m_nMyCityCameraY = MapUnit.getGridTrans( MAPUNIT_TYPE_CITY, 0, WorldMap.m_nMyCityCameraX, WorldMap.m_nMyCityCameraY );
		
	if WorldMap.m_nLastGotoPosX >=0 and WorldMap.m_nLastGotoPosY >= 0 then
		WorldMap.CameraSetPosition( MAPUNIT_TYPE_CITY, WorldMap.m_nLastGotoPosX, WorldMap.m_nLastGotoPosY );
	else
		-- 跳转到玩家城
		MapCamera.transform.position = Vector3.New( WorldMap.m_nMyCityCameraX, WorldMap.m_nMyCityCameraY, WORLDMAP_ZORDER_CAMERA );
	end
	
	-- 地图边界
	--MapUnit.createMapBorder( 249, 250, 500, false );
	
	-- 区域边界
	for i=1, #g_zoneinfo, 1 do
		MapUnit.createMapBorder( g_zoneinfo[i].center_posx-1, g_zoneinfo[i].center_posy, 100 );
	end
	
	-- 发皇城地区的范围归属
	system_askinfo( ASKINFO_WORLDMAP, "", 10 );
	
	-- 初始化行军路线
	MapMarchRoute.init( MapLineRoot );
	
	-- 初始化地图格子数据
	MapTile.init();
	
	-- 显示世界地图界面
	MainDlgShowMap();
	
	-- 任务检查
	QuestClickCheck( 2 )
	
	-- 返回地图数据给程序一份
	return WorldMap.m_nMaxWidth, WorldMap.m_nMaxHeight, MAP_TILEWIDTH, MAP_TILEHEIGHT
end


-- 摄像机坐标=》游戏坐标(菱形格)
function WorldMap.ConvertCameraToGame( cameraPosX, cameraPosY )
	local screen_x = math.modf(cameraPosX * 100) - WorldMap.m_nMaxWidth/2 * MAP_TILEWIDTH;
	local screen_y = math.modf(cameraPosY * 100) + WorldMap.m_nMaxHeight/2 * MAP_TILEHEIGHT;
	local logic_ptx = math.modf(screen_x - math.modf(MAP_TILEWIDTH*screen_y/MAP_TILEHEIGHT));
	local logic_pty = -math.modf(screen_x + math.modf(MAP_TILEWIDTH*screen_y/MAP_TILEHEIGHT));
	if logic_ptx < 0 then
		logic_ptx = logic_ptx - MAP_TILEWIDTH;
	end
	if logic_pty < 0 then
		logic_pty = logic_pty - MAP_TILEWIDTH;
	end
	local gameCoorX = math.modf(logic_ptx/MAP_TILEWIDTH) + WorldMap.m_nMaxWidth/2;
	local gameCoorY = math.modf(logic_pty/MAP_TILEWIDTH) + WorldMap.m_nMaxHeight/2;
	return gameCoorX, gameCoorY 
end

-- 游戏坐标(菱形格)=》摄像机坐标
function WorldMap.ConvertGameToCamera( gameCoorX, gameCoorY )
    if gameCoorX == nil then
        return;
    end
	local px  = ( MAP_TILEWIDTH/2/100 ) * ( gameCoorX - gameCoorY );
    local py  = 0 -( MAP_TILEHEIGHT/2/100 ) * ( gameCoorX + gameCoorY );
	return px + WorldMap.m_nMaxWidth*MAP_TILEWIDTH/2/100, py
end

-- 添加单元
function WorldMap.AddMapUnit( recvValue )
	-- 添加显示对象
	MapUnit.add( MapUnitRoot, recvValue );
	
	-- 添加格子数据
	MapTile.add( recvValue );
	
	-- 接收到的完整数据插入缓存
	WorldMap.m_nMapUnitList[recvValue.m_unit_index] = recvValue;
	
	-- 需要跳转
	if WorldMap.m_nLastGotoUnitIndex == recvValue.m_unit_index then
		WorldMap.OnSelect( MapUnit.cache[recvValue.m_unit_index], recvValue.m_posx, recvValue.m_posy, recvValue.m_unit_index );
		WorldMap.m_nLastGotoUnitIndex = -1;
	end
end

-- 移除单元
function WorldMap.DelMapUnit( recvValue )
	-- 移除显示对象
	MapUnit.del( recvValue.m_unit_index )
	
	-- 移除格子数据
	MapTile.del( WorldMap.m_nMapUnitList[recvValue.m_unit_index] )
	
	-- 移除接收到的缓存数据
	WorldMap.m_nMapUnitList[recvValue.m_unit_index] = nil;
	
	-- 如果正在显示操作界面
	if recvValue.m_unit_index == WorldMap.m_nLastTouchUnitIndex then
		WorldMap.OnClickClose();
		WorldMap.m_nLastTouchUnitIndex = -1;
	end
end

-- 更新单元
function WorldMap.UpdateMapUnit( recvValue )
	-- 如果之前是部队，并且是驻扎状态，现在不驻扎了
	--[[local oldCache = WorldMap.m_nMapUnitList[recvValue.m_unit_index];
	if oldCache ~= nil and oldCache.m_type == MAPUNIT_TYPE_ARMY then
		if oldCache.m_state == ARMY_STATE_OCCUPY and recvValue.m_state ~= ARMY_STATE_OCCUPY then
			-- 移除格子数据
			MapTile.del( WorldMap.m_nMapUnitList[recvValue.m_unit_index] )
		end
	end--]]

	-- 添加
	WorldMap.AddMapUnit( recvValue );
	
	-- 如果正在显示操作界面
	--[[if recvValue.m_unit_index == WorldMap.m_nLastTouchUnitIndex then
		WorldMap.OnClickClose();
		GatherInfoDlgClose();
		MonsterInfoDlgClose();
		if oldCache ~= nil and oldCache.m_type == MAPUNIT_TYPE_ARMY then
			WorldMap.m_nLastTouchUnitIndex = -1;
		end
	end--]]
end

-- 移动单元
function WorldMap.MoveMapUnit( recvValue )
	if WorldMap.m_nMapUnitList[recvValue.m_unit_index] == nil then
		return;
	end
	-- 移除格子数据
	if WorldMap.m_nMapUnitList[recvValue.m_unit_index].m_type ~= MAPUNIT_TYPE_ARMY then
		MapTile.del( WorldMap.m_nMapUnitList[recvValue.m_unit_index] );
	end
    WorldMap.m_nMapUnitList[recvValue.m_unit_index].m_posx = recvValue.m_posx;
    WorldMap.m_nMapUnitList[recvValue.m_unit_index].m_posy = recvValue.m_posy;
	
	
	if WorldMap.m_nMapUnitList[recvValue.m_unit_index].m_type ~= MAPUNIT_TYPE_ARMY then
		-- 添加格子数据
		MapTile.add( WorldMap.m_nMapUnitList[recvValue.m_unit_index] );
		WorldMap.UpdateMapUnit( WorldMap.m_nMapUnitList[recvValue.m_unit_index] ); 
	end
end

-- 随机获取的单元
function WorldMap.RandMapUnit( recvValue )
	-- 接收到的完整数据插入缓存
	WorldMap.m_nMapUnitList[recvValue.m_unit_index] = recvValue;
	WorldMap.m_nLastTouchUnitIndex = recvValue.m_unit_index;
	if recvValue.m_type == MAPUNIT_TYPE_MONSTER then
		--MonsterInfoDlgOnClick( recvValue, true );
	elseif recvValue.m_type == MAPUNIT_TYPE_RES then
		WorldMap.GotoUnit( recvValue.m_unit_index );
	end
end

-- 帧逻辑
function WorldMap.Update()
	WorldMap.QueueFetchArmy();
	
	m_updateWaitFrame = m_updateWaitFrame + 1;
	if m_updateWaitFrame > 90 then
		if Queue.has( procQueue ) == false then
			--WorldMapDlgWaiting( false );
			return;
		end
	end

	-- 一帧创建一个对象
	WorldMap.QueueFetch();
end

--  每秒通知2次位置变化了-负责区域计算
function WorldMap.ViewChangeSec()
    --if MainDlgCutSceneIsPlaying() then
        --return;
    --end

	local cameraPosX = MapCamera:GetComponent("Transform").position.x;
	local cameraPosY = MapCamera:GetComponent("Transform").position.y;


	-- 坐标计算区域索引
	local gameCoorX, gameCoorY = WorldMap.ConvertCameraToGame( cameraPosX, cameraPosY );
    WorldMap.RefreshShow( gameCoorX, gameCoorY );

	local now_areaindex = Area.GetIndex( gameCoorX, gameCoorY );
	if now_areaindex < 0 or now_areaindex == WorldMap.m_nLastAreaIndex then
		return
	end
	
	WorldMap.m_nLastAreaIndex = now_areaindex;
	WorldMap.SendAreaIndex( now_areaindex, gameCoorX, gameCoorY )
	--WorldMapDlgWaiting( true );
	m_updateWaitFrame = 0;
end

--  每秒通知30次位置变化了-负责距离和方位
function WorldMap.ViewChangeFrame()
    if MapCamera == nil then
        return;
    end
	local cameraPosX = MapCamera:GetComponent("Transform").position.x;
	local cameraPosY = MapCamera:GetComponent("Transform").position.y;
    
    m_LastPosX = cameraPosX;
    m_LastPosY = cameraPosY;
	
	-- 位置没变化就不计算了
	if cameraPosX == WorldMap.m_nLastCameraX and cameraPosY == WorldMap.m_nLastCameraY then
		return;
	end
	WorldMap.m_nLastCameraX = cameraPosX;
	WorldMap.m_nLastCameraY	= cameraPosY;

	local gameCoorX, gameCoorY = WorldMap.ConvertCameraToGame( cameraPosX, cameraPosY );
    WorldMap.m_nLastCameraGameX = gameCoorX;
    WorldMap.m_nLastCameraGameY = gameCoorY;
	
	-- 有移动行为就关闭
	if MapClickEffect.gameObject.activeSelf == true then
		if math.abs( MapClickEffect.transform.localPosition.x - cameraPosX ) > 2.56 or math.abs( MapClickEffect.transform.localPosition.y - cameraPosY ) > 1.28 then
			WorldMap.OnClickClose();
		end
	elseif MapClickMod.gameObject.activeSelf == true then
		if math.abs( MapClickMod.transform.localPosition.x - cameraPosX ) > 2.56 or math.abs( MapClickMod.transform.localPosition.y - cameraPosY ) > 1.28 then
			WorldMap.OnClickClose();
		end
	end
	
	-- 操作界面更新坐标
	--MapClickModCoordinate( gameCoorX, gameCoorY );
	
	-- 更新箭头朝向
    --WorldMap.UpdateArrow();
end

-- 更新箭头朝向
function WorldMap.UpdateArrow()

    -- 箭头位置
	--[[local arrowPos = WorldMapDlgArrowPosition();
	-- 计算自己主城与当前视口中心的方位和距离
	local cityPos = cc.p( WorldMap.m_nMyCityCameraX, WorldMap.m_nMyCityCameraY );
	local cameraPos = cc.p( WorldMap.m_nLastCameraX, WorldMap.m_nLastCameraY );
	
	-- 转换成游戏坐标
	local cityGameCoorX, cityGameCoorY = WorldMap.ConvertCameraToGame( WorldMap.m_nMyCityCameraX, WorldMap.m_nMyCityCameraY );
	local cameraGameCoorX, cameraGameCoorY = WorldMap.ConvertCameraToGame( WorldMap.m_nLastCameraX, WorldMap.m_nLastCameraY );
	
	-- 距离
	local distance = math.sqrt( ( cameraGameCoorX - cityGameCoorX )*( cameraGameCoorX - cityGameCoorX ) + ( cameraGameCoorY - cityGameCoorY )*( cameraGameCoorY - cityGameCoorY ) );
	
	-- 角度
	local o = cameraPos.x + arrowPos.x - cityPos.x;
	local a = cameraPos.y + arrowPos.y - cityPos.y;
	local angle = math.deg( math.atan( o/a ) );
	if a < 0 then
		if o < 0 then
			angle = 180 + math.abs(angle);
		else
			angle = 180 - math.abs(angle);
		end 
	end
	
	WorldMapDlgOnDirectionArrow( distance, 180-angle, cc.p( WorldMap.m_nLastCameraGameX, WorldMap.m_nLastCameraGameY ) );--]]
end

-- 触发点击地图
function WorldMap.OnClick( obj, touchpos, unit_index )
	if MapClickEffect.gameObject.activeSelf == true or MapClickMod.gameObject.activeSelf == true then
		WorldMap.OnClickClose();
		return;
	end
	
	-- 屏幕坐标转换到世界坐标
	local touchpos = Camera.main:ScreenToWorldPoint( touchpos );
	local gameCoorX, gameCoorY = WorldMap.ConvertCameraToGame( touchpos.x, touchpos.y );
    if gameCoorX < 0 or gameCoorX >= WorldMap.m_nMaxWidth or
       gameCoorY < 0 or gameCoorY >= WorldMap.m_nMaxHeight then
       return;
    end

    -- 如果在迁城状态则取消迁城
    --[[if MapCityMove.enable and unit_index == -1 then
        MapCityMove.close();
        return;
    end--]]
	
	-- 选择
	WorldMap.OnSelect( obj, gameCoorX, gameCoorY, unit_index );
end

-- 选择单元
function WorldMap.OnSelect( unit, gameCoorX, gameCoorY, unit_index )
	--if WorldMap.m_nMode == WORLDMAP_MODE_MOVECITY then
		--return;
	--end
	if MapClickEffect.gameObject.activeSelf == true or MapClickMod.gameObject.activeSelf == true then
		WorldMap.OnClickClose();
	end

	-- 只有army对象有触发器,并且只有移动中的才判定有效
	--[[if unit_index >= 0 then
		local recvValue = WorldMap.m_nMapUnitList[unit_index];
		if recvValue ~= nil then
			if recvValue.m_type == MAPUNIT_TYPE_ARMY then
				if recvValue.m_state == ARMY_STATE_MARCH or recvValue.m_state == ARMY_STATE_REBACK then
                    -- 播放音效
                    
					-- 记录点击的显示单元索引
					WorldMap.m_nLastTouchUnitIndex = unit_index;
					-- 记录点击的部队单元索引
					WorldMap.m_nLastTouchArmyIndex = recvValue.m_int_value[1];
					-- 	启动跟随
					WorldMap.StartFollow( unit_index );
					return;
				end
			end
		end
	end--]]
	
	-- 记录一下点击的坐标格子
	WorldMap.m_nLastTouchGameCoorX = gameCoorX;
	WorldMap.m_nLastTouchGameCoorY = gameCoorY;
	
	-- 游戏格子坐标转换成摄像机坐标
	local cameraPosX = 0;
	local cameraPosY = 0;
	
	-- 点击的格子是否有缓存数据
	unit_index = MapTile.get( gameCoorX, gameCoorY );  

    -- 其他
	if unit_index >= 0 then
		-- 接收数据
		local recvValue = WorldMap.m_nMapUnitList[unit_index];
		if recvValue == nil then
			return;
		end
		
		-- 检查资源点采集等级,点击就提示
		--[[if recvValue.m_type == MAPUNIT_TYPE_RES then
			local limitlevel = recvValue.m_char_value[3];
			if GetCity().BuildingList[1].m_level < -limitlevel then
				Notify( 173, { -limitlevel, GetResNameByType( recvValue.m_char_value[1] ) } )
				return;
			end
		end--]]
	
		-- 设置缩放
		local grid = 1;
		if recvValue.m_type == MAPUNIT_TYPE_TOWN then
			local townid 	= recvValue.m_short_value[1];
			grid = MapUnit.getGrid( recvValue.m_type, g_towninfo[townid].grid );
		else
			grid = MapUnit.getGrid( recvValue.m_type, 0 );
		end
		MapClickEffect.transform.localScale = Vector3.New( grid, grid, grid );
		-- 转换中心坐标
		cameraPosX, cameraPosY = WorldMap.ConvertGameToCamera( recvValue.m_posx, recvValue.m_posy );
		cameraPosX, cameraPosY = MapUnit.getGridTrans( recvValue.m_type, grid, cameraPosX, cameraPosY );
	else
		-- 设置缩放
		MapClickEffect.transform.localScale = Vector3.New( 1, 1, 1 );
		-- 转换中心坐标
		cameraPosX, cameraPosY = WorldMap.ConvertGameToCamera( gameCoorX, gameCoorY );
		cameraPosX, cameraPosY = MapUnit.getGridTrans( 0, 0, cameraPosX, cameraPosY );
	end
	
	-- 记录点击的显示单元索引
	WorldMap.m_nLastTouchUnitIndex = unit_index;
	WorldMap.m_nLastTouchArmyIndex = -1;
	-- 选择框
	MapClickEffect.transform.localPosition = Vector3.New( cameraPosX, cameraPosY, WORLDMAP_ZORDER_CLICKMOD );
	MapClickEffect.gameObject:SetActive( true );

	-- 操作界面
	local recvValue = WorldMap.m_nMapUnitList[unit_index];
	if recvValue then
		if recvValue.m_type == MAPUNIT_TYPE_ARMY then
			WorldMap.m_nLastTouchArmyIndex = recvValue.m_int_value[1];
		
		-- 城镇
		elseif recvValue.m_type == MAPUNIT_TYPE_TOWN then
			local townid 		= recvValue.m_short_value[1]
			local type 			= g_towninfo[townid].type
			if type == MAPUNIT_TYPE_TOWN_TYPE9 then
				BloodyBattleDlgTownClick( recvValue )
				MapClickEffect.gameObject:SetActive( false );
				return
			end
			
		-- 野怪
		elseif recvValue.m_type == MAPUNIT_TYPE_ENEMY then
			MapClickEffect.gameObject:SetActive( false );
			MapEnemyDlgShow( recvValue )
			return;	
		
		-- 资源点		
		elseif recvValue.m_type == MAPUNIT_TYPE_RES then
			MapClickEffect.gameObject:SetActive( false );
			MapResDlgShow( recvValue )
			return;	
		
		-- 随机事件	
		elseif recvValue.m_type == MAPUNIT_TYPE_EVENT then
			MapClickEffect.gameObject:SetActive( false );
			MapEventDlgShow( recvValue )
			return
		end
	end
	
	-- 操作框
	MapClickMod.transform.localPosition = Vector3.New( cameraPosX, cameraPosY, WORLDMAP_ZORDER_CLICKMOD );
	MapClickMod.gameObject:SetActive( true );
end

-- 显示点击特效
function WorldMap.OnClickShow( gameCoorX, gameCoorY )
	if MapClickEffect == nil then
		return
	end
	if MapClickEffect.gameObject == nil then
		return
	end
	
	-- 只有城镇才会大于1个格子
	local grid = 1
	local type = MAPUNIT_TYPE_CITY;
	for k, v in pairs( g_towninfo ) do
		if v.posx == gameCoorX and v.posy == gameCoorY then
			grid = v.grid
			type = MAPUNIT_TYPE_TOWN
			break;
		end
--[[		if v.grid == 2 then
			
		elseif v.grid == 3 then
			
		end--]]
	end
	
	MapClickEffect.transform.localScale = Vector3.New( grid, grid, grid );
	
	-- 转换中心坐标
	local cameraPosX, cameraPosY = WorldMap.ConvertGameToCamera( gameCoorX, gameCoorY );
	cameraPosX, cameraPosY = MapUnit.getGridTrans( type, grid, cameraPosX, cameraPosY );
		
	MapClickEffect.transform.localPosition = Vector3.New( cameraPosX, cameraPosY, WORLDMAP_ZORDER_CLICKMOD )
	MapClickEffect.gameObject:SetActive( true );
end

-- 关闭操作界面
function WorldMap.OnClickClose()
	if MapClickEffect ~= nil and MapClickEffect.gameObject ~= nil then
		MapClickEffect.gameObject:SetActive( false );
	end
	if MapClickMod ~= nil and MapClickMod.gameObject ~= nil then
		MapClickMod.gameObject:SetActive( false );
	end
	WorldMap.StopFollow();
end

-- 触发拖拽对象
function WorldMap.OnDrag( touchpos )
	--MapCityMove.OnDrag( touchpos )
end

-- 设置摄像机位置
function WorldMap.CameraSetPosition( unittype, gameCoorX, gameCoorY )
	-- 位置
    local cameraPosX, cameraPosY = WorldMap.ConvertGameToCamera( gameCoorX, gameCoorY );
	local posx, posy = MapUnit.getGridTrans( unittype, 0, cameraPosX, cameraPosY );
	if MapCamera then
		MapCamera.transform.position = Vector3.New( posx, posy, WORLDMAP_ZORDER_CAMERA );
		WorldMap.m_nLastCameraX 	 = posx;
		WorldMap.m_nLastCameraY		 = posy;
		WorldMap.m_nLastCameraGameX  = gameCoorX;
		WorldMap.m_nLastCameraGameY  = gameCoorY;
	end

    WorldMap.RefreshShow( gameCoorX, gameCoorY );
    
    m_LastPosX = nil;
    m_LastPosY = nil;
end

-- 摄像机滑动至
function WorldMap.TweenPosition( unittype, gameCoorX, gameCoorY, time )
	-- 位置
    local cameraPosX, cameraPosY = WorldMap.ConvertGameToCamera( gameCoorX, gameCoorY );
	local posx, posy = MapUnit.getGridTrans( unittype, 0, cameraPosX, cameraPosY );
	if MapCamera then
		MapCamera:GetComponent( "Camera2D" ):TweenPosTo( Vector3.New( posx, posy, WORLDMAP_ZORDER_CAMERA ), time );
		WorldMap.m_nLastCameraX 	 = posx;
		WorldMap.m_nLastCameraY		 = posy;
		WorldMap.m_nLastCameraGameX  = gameCoorX;
		WorldMap.m_nLastCameraGameY  = gameCoorY;
	end
end

-- 跳转到unit位置
function WorldMap.GotoUnit( unit_index )
	local recvValue = WorldMap.m_nMapUnitList[unit_index];
	if recvValue == nil then
		system_askinfo( ASKINFO_WORLDMAP, "", 6, unit_index );
	else
		WorldMap.CameraSetPosition( recvValue.m_type, recvValue.m_posx, recvValue.m_posy );
		WorldMap.OnSelect( MapUnit.cache[unit_index], recvValue.m_posx, recvValue.m_posy, unit_index );
	end
    
    m_LastPosX = nil;
    m_LastPosY = nil;
end

-- 服务器发回的到unit位置
-- m_posx=0,m_posy=0,m_type=0,m_unit_index=0,m_op=0,
function WorldMap.GotoUnitAsyn( recvValue )
	if recvValue.m_op == 1 then
		-- 挖宝前往使用
		WorldMap.CameraSetPosition( recvValue.m_type, recvValue.m_posx, recvValue.m_posy );
		WorldMap.OnClickShow( recvValue.m_posx, recvValue.m_posy )
		if recvValue.m_type > 0 then
			TreasureDlgDo( recvValue.m_posx, recvValue.m_posy )
		else
			WorldMap.OnSelect( nil, recvValue.m_posx, recvValue.m_posy, recvValue.m_unit_index );
		end
	else
		WorldMap.m_nLastGotoUnitIndex = recvValue.m_unit_index;
		WorldMap.CameraSetPosition( recvValue.m_type, recvValue.m_posx, recvValue.m_posy );
	end
	--WorldMap.ViewChangeSec();
end

-- 跳转到位置
function WorldMap.GotoCoor( gameCoorX, gameCoorY )
	if GameManager.currentScence == "city" then
		WorldMap.GotoWorldMap( gameCoorX, gameCoorY )
    elseif GameManager.currentScence == "worldmap" then
		WorldMap.CameraSetPosition( MAPUNIT_TYPE_CITY, gameCoorX, gameCoorY );
		WorldMap.OnClickShow( gameCoorX, gameCoorY )
		--WorldMap.ViewChangeSec();
	end
    
    m_LastPosX = nil;
    m_LastPosY = nil;
end

-- 我的城池
function WorldMap.GotoMyCity()
	WorldMap.GotoCoor( WorldMap.m_nMyCityPosx, WorldMap.m_nMyCityPosy )
	local recvValue = WorldMap.m_nMapUnitList[WorldMap.m_nMyCityUnitIndex];
	if recvValue == nil then
		WorldMap.m_nLastGotoUnitIndex = WorldMap.m_nMyCityUnitIndex
	else
		WorldMap.OnSelect( MapUnit.cache[recvValue.m_unit_index], recvValue.m_posx, recvValue.m_posy, recvValue.m_unit_index );
	end
end

-- 开启摄像机跟随模式
function WorldMap.StartFollow( unit_index )
    if WorldMap.m_nMode == WORLDMAP_MODE_NORMAL then
		local unit = MapUnit.cache[unit_index];
		if unit == nil then
			return;
		end
		
		-- 操作界面跟随
		MapClickMod.gameObject:SetActive( true );	
        MapClickMod.transform:SetParent( unit.transform );
		MapClickMod.transform.localPosition = Vector3.New( 0, 0, WORLDMAP_ZORDER_CLICKMOD );
        MapClickMod.transform.localScale = Vector3.New( 0.01, 0.01, 1 );
		
		-- 摄像机跟随
        local unitPos = unit.transform.position;
        MapCamera.transform.position= Vector3.New( unitPos.x, unitPos.y, WORLDMAP_ZORDER_CAMERA );
        MapCamera.transform:SetParent( unit.transform );
        MapCamera:GetComponent("WorldMapCamera").IsCanMoved = false;
		
		WorldMap.m_nMode = WORLDMAP_MODE_FOLLOW;
    end
end

-- 关闭跟随模式
function WorldMap.StopFollow()
    --if WorldMap.m_nMode == WORLDMAP_MODE_FOLLOW then
		-- 操作界面停止跟随
        MapClickMod.transform:SetParent( WorldMapPrefab.transform );
		MapClickMod.transform.localPosition = Vector3.New( 0, 0, WORLDMAP_ZORDER_CLICKMOD );
		
		-- 摄像机停止跟随
        MapCamera.transform:SetParent( WorldMapPrefab.transform );
		--MapCamera:GetComponent("WorldMapCamera").IsCanMoved = true;
		
		WorldMap.m_nMode = WORLDMAP_MODE_NORMAL;
    --end
end

-- 返回选择特效
function WorldMap.ClickEffect()
	return MapClickEffect
end
function WorldMap.MapClickMod()
	return MapClickMod;
end

-- 添加到创建队列
function WorldMap.QueueAdd( operation, recvValue )
	
	local unittype = 0;
	if operation == 1 then
		
		unittype = recvValue.m_type;
		
	elseif operation == 2 then
	
		local unit = WorldMap.m_nMapUnitList[recvValue.m_unit_index];
		if unit then
			unittype = unit.m_type;
		end
		
	elseif operation == 3 then
	
		unittype = recvValue.m_type;
		
	elseif operation == 4 then
	
		local unit = WorldMap.m_nMapUnitList[recvValue.m_unit_index];
		if unit then
			unittype = unit.m_type;
		end
		
	end
	
	if unittype == MAPUNIT_TYPE_ARMY then
		Queue.pushBack( procQueueArmy, { op=operation, value = recvValue } );		
	else
		Queue.pushBack( procQueue, { op=operation, value = recvValue } );
	end
	
	m_updateWaitFrame = m_updateWaitFrame - 1;
	if m_updateWaitFrame < 0 then
		m_updateWaitFrame = 0;
	end
end

-- 清空创建队列
function WorldMap.QueueClear()
	Queue.clear( procQueue );
	Queue.clear( procQueueArmy );
end

-- 创建队列取出一项
function WorldMap.QueueFetch()
	
	-- 普通对象
	for tmpi=1, 6, 1 do
		local v = Queue.popFirst( procQueue );
		if v == nil then
			return;
		end
		if v.op == 1 then
			WorldMap.AddMapUnit( v.value );
		elseif v.op == 2 then
			WorldMap.DelMapUnit( v.value );
		elseif v.op == 3 then
			WorldMap.UpdateMapUnit( v.value );
		elseif v.op == 4 then
			WorldMap.MoveMapUnit( v.value );
		elseif v.op == 5 then
			MapMarchRoute.add( v.value );
		elseif v.op == 6 then
			MapMarchRoute.del( v.value );
		end
		m_updateWaitFrame = m_updateWaitFrame + 1;
	end
	
end

-- 创建队列取出一项
function WorldMap.QueueFetchArmy()
	
	-- 部队对象
	for tmpi=1, 3, 1 do
		local v = Queue.popFirst( procQueueArmy );
		if v == nil then
			return;
		end
		if v.op == 1 then
			WorldMap.AddMapUnit( v.value );
		elseif v.op == 2 then
			WorldMap.DelMapUnit( v.value );
		elseif v.op == 3 then
			WorldMap.UpdateMapUnit( v.value );
		elseif v.op == 4 then
			WorldMap.MoveMapUnit( v.value );
		end
	end
	
end

-- 计算行军时间
function WorldMap.MarchTime(fposx, fposy, tposx, tposy)
    local distance = math.abs(fposx - tposx) + math.abs(fposy - tposy);
    local speed = global.army_move;
    if speed <= 0 then
        return 0;
    end
    local duration = math.floor(distance) * speed;
    duration = math.ceil(duration*( 1.0-GetPlayer().m_attr.m_movespeed_per[1]/100)*( 1.0-GetPlayer().m_attr.m_movespeed_per[2]/100)*( 1.0-GetPlayer().m_attr.m_movespeed_per[3]/100) );
    duration = math.max(duration, speed);
    return duration;
end

-- 迁城完毕
function WorldMap.OnCityMoved( unit_index, gameCoorX, gameCoorY )
	if GameManager.currentScence == "worldmap" then
		WorldMap.m_nMyCityUnitIndex = unit_index;
		WorldMap.m_nMyCityPosx = gameCoorX;
		WorldMap.m_nMyCityPosy = gameCoorY;
		WorldMap.CameraSetPosition( MAPUNIT_TYPE_CITY, gameCoorX, gameCoorY );	
		WorldMap.m_nMyCityCameraX, WorldMap.m_nMyCityCameraY = WorldMap.ConvertGameToCamera( WorldMap.m_nMyCityPosx, WorldMap.m_nMyCityPosy );
		WorldMap.m_nMyCityCameraX, WorldMap.m_nMyCityCameraY = MapUnit.getGridTrans( MAPUNIT_TYPE_CITY, 0, WorldMap.m_nMyCityCameraX, WorldMap.m_nMyCityCameraY );
		WorldMap.UpdateArrow();
		GetPlayer().m_posx = gameCoorX;
		GetPlayer().m_posy = gameCoorY;

        -- 特效
        --local obj = GameObject.Instantiate( LoadPrefab( "Effect_City_Change_Position_Smoke" ) );
        --obj.transform.position = Vector3.New( WorldMap.m_nMyCityCameraX, WorldMap.m_nMyCityCameraY, WorldMap.m_nMyCityCameraY );
        --GameObject.Destroy( obj, 5 );
	end
end

-- 获得摄像机
function WorldMap.GetCamera()
    return MapCamera;
end

-- 隐藏UI
function WorldMap.UIFade( fade )
    --if m_Faded == fade then
        --return;
    --end
    --m_Faded = fade;
    --local tween = fruit.uiManager:GetLayer( 0 ):GetComponent( UITweenFade.GetClassType() );
    --tween:Play( fade );
    --fruit.uiManager:GetLayer( 0 ):GetComponent( "CanvasGroup" ).blocksRaycasts = fade == false;
    -- MainDlgGetClubBtn().gameObject:SetActive( fade == false );
end

-- 获取地区id
function map_zone_getid( posx, posy )
	local zonex, zoney;
	if posx >= WorldMap.m_nMaxWidth then
		posx = WorldMap.m_nMaxWidth - 1;
	end
	if posy >= WorldMap.m_nMaxHeight then
		posy = WorldMap.m_nMaxHeight - 1;
	end
	zonex = math.floor(posx / 100);
	zoney = math.floor(posy / 100);
	return zoney*(5) + zonex + 1;
end

-- 检查是不是在同一个区域
function map_zone_checksame( posx, posy, tposx, tposy )
	local n = map_zone_getid( posx, posy );
	local m = map_zone_getid( tposx, tposy );
	if n == m then
		return 1;
	end
	return 0;
end
