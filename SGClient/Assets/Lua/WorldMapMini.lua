-- 世界地图小地图
WORLDMAP_ZORDER_CAMERA		=	-10000;	-- 摄像机层级

WorldMapMini = {}
WorldMapMini.m_nLastAreaIndex		= -1;   -- 上一次发送区域
WorldMapMini.m_nMapUnitList			= {};   -- 显示对象缓存链表
WorldMapMini.m_nLastCameraX 		= 0; 	-- 上次摄像机位置（屏幕坐标）
WorldMapMini.m_nLastCameraY			= 0; 	-- 上次摄像机位置（屏幕坐标）
WorldMapMini.m_nLastCameraGameX 	= -1; 	-- 上次摄像机位置（格子坐标）
WorldMapMini.m_nLastCameraGameY 	= -1; 	-- 上次摄像机位置（格子坐标）
WorldMapMini.m_nZoneID				= 0;	-- 当前地图

local TmxShowThreshold = 24;
local TmxShowList = { 0, 0, 0, 0 };
local TmxLastList = { 0, 0, 0, 0 };
local m_LastPosX;
local m_LastPosY;
local m_Faded = false;
local m_FadeStopCount = 0;
local m_MoveType = 0;

-- 显示单元创建队列
local procQueue = Queue.new();
-- 部队单拿出来一个队列
local procQueueArmy = Queue.new();
local m_updateWaitFrame = 0;

-- 地图对象组件
local WorldMapMiniPrefab	= nil;	-- 地图
local MapTmxRoot			= nil;	-- tmx地图根节点
local MapCamera 			= nil;	-- 地图摄像机
local MapTmx            	= {};   -- 地图池，只实例化4个

-- 创建小地图
function WorldMapMini.Create( recvValue )
	WorldMapMini.CameraSetPosition( MAPUNIT_TYPE_CITY, recvValue.m_target_posx, recvValue.m_target_posy );
	WorldMapMini.m_nMapUnitList			= {};
end

-- 删除小地图
function WorldMapMini.Delete()
    WorldMapMini.m_nMapUnitList = {};
	WorldMapMini.QueueClear();
    MapCamera = nil;
    m_LastPosX = nil;
    m_LastPosY = nil;

end

-- 确定哪个图显示
function WorldMapMini.RefreshShow( gamePosX, gamePosY )
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
function WorldMapMini.Start( Prefab )
	-- 初始化根节点
	WorldMapMiniPrefab	= Prefab;
    MapUnit.unitRoot = Prefab.transform:Find("MapUnitRoot");

	-- 获取组件
	MapTmxRoot				= WorldMapMiniPrefab.transform:Find( "MapTmxRoot" );
	MapUnitRoot				= WorldMapMiniPrefab.transform:Find( "MapUnitRoot" );
	MapCamera				= WorldMapMiniPrefab.transform:Find( "MapCamera" );
	
	-- 根据地图大小动态加载N*N个Tmx地图
	local TmxMapPrefab = LoadPrefab( "TmxMap" );

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
function WorldMapMini.ConvertCameraToGame( cameraPosX, cameraPosY )
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
function WorldMapMini.ConvertGameToCamera( gameCoorX, gameCoorY )
    if gameCoorX == nil then
        return;
    end
	local px  = ( MAP_TILEWIDTH/2/100 ) * ( gameCoorX - gameCoorY );
    local py  = 0 -( MAP_TILEHEIGHT/2/100 ) * ( gameCoorX + gameCoorY );
	return px + WorldMap.m_nMaxWidth*MAP_TILEWIDTH/2/100, py
end

-- 添加单元
function WorldMapMini.AddMapUnit( recvValue )
	-- 接收到的完整数据插入缓存
	WorldMapMini.m_nMapUnitList[recvValue.m_unit_index] = recvValue;
end

-- 移除单元
function WorldMapMini.DelMapUnit( recvValue )
	-- 移除接收到的缓存数据
	WorldMapMini.m_nMapUnitList[recvValue.m_unit_index] = nil;
end

-- 更新单元
function WorldMapMini.UpdateMapUnit( recvValue )
	-- 添加
	WorldMapMini.AddMapUnit( recvValue );
end

-- 帧逻辑
function WorldMapMini.Update()
	m_updateWaitFrame = m_updateWaitFrame + 1;
	if m_updateWaitFrame > 90 then
		if Queue.has( procQueue ) == false then
			return;
		end
	end

	-- 一帧创建一个对象
	WorldMapMini.QueueFetch();
end

--  每秒通知2次位置变化了-负责区域计算
function WorldMapMini.ViewChangeSec()
    --if MainDlgCutSceneIsPlaying() then
        --return;
    --end

	local cameraPosX = MapCamera:GetComponent("Transform").position.x;
	local cameraPosY = MapCamera:GetComponent("Transform").position.y;


	-- 坐标计算区域索引
	local gameCoorX, gameCoorY = WorldMapMini.ConvertCameraToGame( cameraPosX, cameraPosY );
    WorldMapMini.RefreshShow( gameCoorX, gameCoorY );

	local now_areaindex = Area.GetIndex( gameCoorX, gameCoorY );
	if now_areaindex < 0 or now_areaindex == WorldMapMini.m_nLastAreaIndex then
		return
	end
	
	WorldMapMini.m_nLastAreaIndex = now_areaindex;
	WorldMapMini.SendAreaIndex( now_areaindex, gameCoorX, gameCoorY )
	m_updateWaitFrame = 0;
end

-- 设置摄像机位置
function WorldMapMini.CameraSetPosition( unittype, gameCoorX, gameCoorY )
	-- 位置
    local cameraPosX, cameraPosY = WorldMapMini.ConvertGameToCamera( gameCoorX, gameCoorY );
	local posx, posy = MapUnit.getGridTrans( unittype, 0, cameraPosX, cameraPosY );
	if MapCamera then
		MapCamera.transform.position = Vector3.New( posx, posy, WORLDMAP_ZORDER_CAMERA );
		WorldMapMini.m_nLastCameraX 	 = posx;
		WorldMapMini.m_nLastCameraY		 = posy;
		WorldMapMini.m_nLastCameraGameX  = gameCoorX;
		WorldMapMini.m_nLastCameraGameY  = gameCoorY;
	end

    WorldMapMini.RefreshShow( gameCoorX, gameCoorY );
    
    m_LastPosX = nil;
    m_LastPosY = nil;
end

-- 摄像机滑动至
function WorldMapMini.TweenPosition( unittype, gameCoorX, gameCoorY, time )
	-- 位置
    local cameraPosX, cameraPosY = WorldMapMini.ConvertGameToCamera( gameCoorX, gameCoorY );
	local posx, posy = MapUnit.getGridTrans( unittype, 0, cameraPosX, cameraPosY );
	if MapCamera then
		MapCamera:GetComponent( "Camera2D" ):TweenPosTo( Vector3.New( posx, posy, WORLDMAP_ZORDER_CAMERA ), time );
		WorldMapMini.m_nLastCameraX 	 = posx;
		WorldMapMini.m_nLastCameraY		 = posy;
		WorldMapMini.m_nLastCameraGameX  = gameCoorX;
		WorldMapMini.m_nLastCameraGameY  = gameCoorY;
	end
end

-- 添加到创建队列
function WorldMapMini.QueueAdd( operation, recvValue )
	
	local unittype = 0;
	if operation == 1 then
		unittype = recvValue.m_type;
	end
	
	m_updateWaitFrame = m_updateWaitFrame - 1;
	if m_updateWaitFrame < 0 then
		m_updateWaitFrame = 0;
	end
end

-- 清空创建队列
function WorldMapMini.QueueClear()
	Queue.clear( procQueue );
	Queue.clear( procQueueArmy );
end

-- 创建队列取出一项
function WorldMapMini.QueueFetch()
	
	-- 普通对象
	for tmpi=1, 6, 1 do
		local v = Queue.popFirst( procQueue );
		if v == nil then
			return;
		end
		if v.op == 1 then
			WorldMapMini.AddMapUnit( v.value );
		elseif v.op == 2 then
			WorldMapMini.DelMapUnit( v.value );
		end
		m_updateWaitFrame = m_updateWaitFrame + 1;
	end
	
end

-- 获得摄像机
function WorldMapMini.GetCamera()
    return MapCamera;
end
