-- 世界地图缩略图

WorldMapThumb = {};
WorldMapThumb.m_nMaxWidth 	= 1280;
WorldMapThumb.m_nMaxHeight 	= 640;
WorldMapThumb.m_nZoneID = 0;
WorldMapThumb.m_nLastClickGameCoorX = -1;
WorldMapThumb.m_nLastClickGameCoorY = -1;
-- 点击标示
WorldMapThumb.clickEffectObj = nil;

-- 地图缩略图
local WorldMapThumbObject = nil;

-- 地图缩略图对象组件
local WorldMapThumbPrefab	= nil;	-- 地图根
local ThumbDisplayPrefab	= nil;	-- 地图显示层
local ThumbCityRoot			= nil;	-- 地图显示层
local ThumbCamera			= nil;	-- 地图摄像机
local ThumbDisplayTownPrefab = nil;
local ThumbDisplayCityPrefab = nil;
local ThumbUIMod = nil;

-- 只是显示隐藏 
function WorldMapThumb.Show( bShow )
	if WorldMapThumbObject ~= nil then
		WorldMapThumbObject:SetActive( bShow );
	end
end

-- 创建
function WorldMapThumb.Create( zoneid )
	WorldMapThumb.m_nZoneID = zoneid
	WorldMapThumbObject = GameObject.Instantiate( LoadPrefab("WorldMapThumb") );
	if GameManager.WorldMap ~= nil then
		GameManager.WorldMap.gameObject:SetActive( false );
	end
	
    --MainDlgClose();
    --fruit.audioManager:Play(71);
	-- 注释掉，防止去缩略图后返回大地图部队显示不正确
    --WorldMapDlgClose(); 
end

-- 销毁
function WorldMapThumb.Delete()
	if WorldMapThumbObject ~= nil then
		GameObject.Destroy( WorldMapThumbObject );
		WorldMapThumbObject = nil;
	end
	ThumbMaskPrefab		= nil;	
	ThumbDisplayPrefab	= nil;
	ThumbCityRoot		= nil;
	ThumbCamera			= nil;
	ThumbUIMod = nil;
	WorldMapThumb.clickEffectObj = nil;
	if GameManager.WorldMap ~= nil then
		GameManager.WorldMap.gameObject:SetActive( true );
	end
	ThumbDisplayTownPrefab= nil;
	ThumbDisplayCityPrefab = nil;
    --fruit.audioManager:Play(72);
    --MainDlgPlayBGM(true);
    --MainDlgOpen();
    --WorldMapDlgOpen();
end

-- 世界地图缩略图启动
function WorldMapThumb.Start( Prefab )
	-- 初始化根节点
	WorldMapThumbPrefab	= Prefab;
	ThumbDisplayPrefab	= WorldMapThumbPrefab:GetComponent("Transform"):Find( "Display" );
	ThumbCityRoot		= WorldMapThumbPrefab:GetComponent("Transform"):Find( "CityRoot" );
	ThumbCamera			= WorldMapThumbPrefab:GetComponent("Transform"):Find( "ThumbCamera" );
	ThumbUIMod			= WorldMapThumbPrefab:GetComponent("Transform"):Find( "MapThumbUIMod" );
	SetFalse( ThumbUIMod )
	
	-- 点击特效
	if WorldMapThumb.clickEffectObj == nil then
		WorldMapThumb.clickEffectObj = GameObject.Instantiate( LoadPrefab("ThumbClickEffect") );
		WorldMapThumb.clickEffectObj.transform:SetParent( ThumbDisplayPrefab );
	end
	
	-- 是我所在的地图，显示我的位置
	local myzoneid =  map_zone_getid( WorldMap.m_nMyCityPosx, WorldMap.m_nMyCityPosy )
	if myzoneid == WorldMapThumb.m_nZoneID then
		WorldMapThumb.SetMyPos();
		
		-- 显示当前位置
		local thumbX, thumbY = WorldMapThumb.ConvertMapToThumb( WorldMap.m_nLastCameraGameX, WorldMap.m_nLastCameraGameY )
		WorldMapThumb.SetCurPos( thumbX, thumbY );
	
	else
		-- 显示当前位置
		WorldMapThumb.SetCurPos( 0, 0 );
	end
	
	-- 获取单元
	local areaupdate = 1;
	-- 当前所在区域
	local zoneid = WorldMapThumb.m_nZoneID
	
	-- 如果皇城开启，哪都可以看到
	if GetPlayer().m_open_townking <= 0 then
		-- 如果州城开启，只能看见四个州城和对应郡城
		if GetPlayer().m_open_town6 > 0 then
			if g_zoneinfo[zoneid].type ~= 1 then
				if map_zone_ismovezone( GetPlayer().m_zone, zoneid ) == 0 then
					areaupdate = 0
				end
			end
		else
			-- 只能看见自己的郡城
			if GetPlayer().m_zone ~= zoneid then
				areaupdate = 0
			end
		end
	end
	
	if areaupdate == 1 then
		-- 获取玩家信息
		system_askinfo( ASKINFO_MAPZONE, "", 0, WorldMapThumb.m_nZoneID );
		
		-- 获取城镇信息
		system_askinfo( ASKINFO_MAPZONE, "", 1, WorldMapThumb.m_nZoneID );
	else
		WorldMapThumb.Alert( T(2369) )
	end
end

-- 缩略图坐标（摄像机）=》地区地图坐标(菱形格)
function WorldMapThumb.ConvertThumbToZone( thumbX, thumbY )
	local worldCameraX = thumbX * (WorldMap.m_nMaxWidth * MAP_TILEWIDTH) / WorldMapThumb.m_nMaxWidth;
	local worldCameraY = thumbY * (WorldMap.m_nMaxHeight * MAP_TILEHEIGHT) / WorldMapThumb.m_nMaxHeight;
	
	local gameCoorX, gameCoorY = WorldMap.ConvertCameraToGame( worldCameraX, worldCameraY );
	gameCoorX = WorldMap.m_nMaxWidth + gameCoorX;
	
	gameCoorX = math.floor(gameCoorX/5) + g_zoneinfo[WorldMapThumb.m_nZoneID].top_left_posx;
	gameCoorY = math.floor(gameCoorY/5) + g_zoneinfo[WorldMapThumb.m_nZoneID].top_left_posy;
	--print( gameCoorX ..","..gameCoorY )
	return gameCoorX, gameCoorY;
end

-- 大地图坐标(菱形格)=》缩略图坐标（摄像机）
function WorldMapThumb.ConvertMapToThumb( gameCoorX, gameCoorY )
	gameCoorX = (gameCoorX - g_zoneinfo[WorldMapThumb.m_nZoneID].top_left_posx) / 10;
	gameCoorY = (gameCoorY - g_zoneinfo[WorldMapThumb.m_nZoneID].top_left_posy) / 10;
	--gameCoorX = math.floor( (gameCoorX - g_zoneinfo[WorldMapThumb.m_nZoneID].top_left_posx) / 10 );
	--gameCoorY = math.floor( (gameCoorY - g_zoneinfo[WorldMapThumb.m_nZoneID].top_left_posy) / 10 );
	
	--gameCoorX = math.floor( gameCoorX * ((WorldMap.m_nMaxWidth * MAP_TILEWIDTH) / WorldMapThumb.m_nMaxWidth) )
	--gameCoorY = math.floor( gameCoorY * ((WorldMap.m_nMaxHeight * MAP_TILEHEIGHT) / WorldMapThumb.m_nMaxHeight) )
	
	--print( gameCoorX ..","..gameCoorY )
	local px  = ( 128/2/100 ) * ( gameCoorX - gameCoorY );
    local py  = 0 -( 64/2/100 ) * ( gameCoorX + gameCoorY ) + ( 64/2/10 );
	
	return px, py
	
	--local cameraPosX, cameraPosY = WorldMap.ConvertGameToCamera( gameCoorX, gameCoorY );--]]
	
--[[	local cameraPosX, cameraPosY = WorldMap.ConvertGameToCamera( gameCoorX, gameCoorY );
	local thumbX = cameraPosX / ((WorldMap.m_nMaxWidth * MAP_TILEWIDTH) / WorldMapThumb.m_nMaxWidth) - WorldMapThumb.m_nMaxWidth/2/100;
	local thumbY = cameraPosY / ((WorldMap.m_nMaxHeight * MAP_TILEHEIGHT) / WorldMapThumb.m_nMaxHeight) + WorldMapThumb.m_nMaxHeight/2/100;--]]
	--return gameCoorX, gameCoorY;
end

-- 触发点击地图
function WorldMapThumb.OnClick( obj, touchpos )
	if obj == nil then
		print("obj == nil")
		return;
	end
	local gameCoorX = -1; 
	local gameCoorY = -1;
	local shareData = obj:GetComponent("ShareData");
	if shareData then
		local posType = tonumber( shareData:GetValue( "PosType" ) );
		local posIndex = tonumber( shareData:GetValue( "PosIndex" ) );
		-- 点击的是我自己位置
		if posType == 1 then
			gameCoorX = WorldMap.m_nMyCityPosx;
			gameCoorY = WorldMap.m_nMyCityPosy;
		
		-- 点击的是城镇位置		
		elseif posType == 3 then
			local townid = posIndex
			gameCoorX = g_towninfo[townid].posx
			gameCoorY = g_towninfo[townid].posy
		end	
		if gameCoorX < 0 or gameCoorY < 0 then
			gameCoorX = WorldMap.m_nMyCityPosx;
			gameCoorY = WorldMap.m_nMyCityPosy;
		end
		touchpos.x, touchpos.y = WorldMapThumb.ConvertMapToThumb( gameCoorX, gameCoorY );
		
	else
		
		-- 屏幕坐标转换到世界坐标
		touchpos = ThumbCamera:GetComponent("Camera"):ScreenToWorldPoint( touchpos );
		touchpos = obj.transform:InverseTransformPoint( touchpos );
		-- 缩略图转大地图坐标
		gameCoorX, gameCoorY = WorldMapThumb.ConvertThumbToZone( touchpos.x, touchpos.y );
	end
	
	if gameCoorX < 0 or gameCoorX >= WorldMap.m_nMaxWidth or
		gameCoorY < 0 or gameCoorY >= WorldMap.m_nMaxHeight then
		return;
	end	
	
	-- 双击位置
	if gameCoorX >= WorldMapThumb.m_nLastClickGameCoorX - 2 and gameCoorX <= WorldMapThumb.m_nLastClickGameCoorX + 2 and
		gameCoorY >= WorldMapThumb.m_nLastClickGameCoorY - 2 and gameCoorY <= WorldMapThumb.m_nLastClickGameCoorY + 2 then
		-- 关闭界面
		MapZoneDlgClose();
		-- 跳转到位置
		WorldMap.GotoCoor( WorldMapThumb.m_nLastClickGameCoorX, WorldMapThumb.m_nLastClickGameCoorY )
	else
		WorldMapThumb.m_nLastClickGameCoorX = gameCoorX;
		WorldMapThumb.m_nLastClickGameCoorY = gameCoorY;
		-- 点击标示
		WorldMapThumb.SetCurPos( touchpos.x, touchpos.y );
	end
		
	
	
	-- 摄像机移动
	--ThumbCamera:GetComponent("WorldMapThumbCamera"):TweenPosTo( Vector3.New( touchpos.x, touchpos.y, 0 ), 0.5 );
	--ThumbCamera:GetComponent("WorldMapThumbCamera"):TweenSizeTo( 1, 0.5 );
	--Invoke( WorldMapThumb.Goto, 0.5, Vector3.New( gameCoorX, gameCoorY, 0 ) );
	
end

-- 触发拖拽对象
function WorldMapThumb.OnDrag( touchpos )
end

-- 跳转
function WorldMapThumb.Goto( gameCoor )
	WorldMapThumb.Delete();
	WorldMap.CameraSetPosition( 0, gameCoor.x, gameCoor.y );
end

-- 显示当前位置
function WorldMapThumb.SetCurPos( x, y )
	-- 点击标示
	if WorldMapThumb.clickEffectObj == nil then
		WorldMapThumb.clickEffectObj = GameObject.Instantiate( LoadPrefab("ThumbClickEffect") );
		WorldMapThumb.clickEffectObj.transform:SetParent( ThumbDisplayPrefab );
	end
	WorldMapThumb.clickEffectObj.transform.localPosition = Vector3.New( x, y, -1 );
end

-- 设置我自己的位置
function WorldMapThumb.SetMyPos()
	local thumbX, thumbY = WorldMapThumb.ConvertMapToThumb( WorldMap.m_nMyCityPosx, WorldMap.m_nMyCityPosy );
	local thumbObj = GameObject.Instantiate( LoadPrefab("ThumbDisplayMy") );
	thumbObj.transform:SetParent( ThumbDisplayPrefab );
	thumbObj.transform.localPosition = Vector3.New( thumbX, thumbY, 0 );
	thumbObj.transform:GetComponent("ShareData"):AddValue( "PosType", 1 );
		
--[[	Invoke( function()
        if ThumbCamera ~= nil then
		    ThumbCamera:GetComponent("WorldMapThumbCamera"):TweenPosToInBound( Vector3.New( thumbX, thumbY, 0 ), 0.5 );
        end
	end, 0.1 )--]]
end

-- 设置玩家的位置
-- m_count=0,m_list={m_posx=0,m_posy=0,m_nation=0,m_level=0,[m_count]},
function WorldMapThumb.SetCityInfo( recvValue )
	if ThumbCityRoot == nil then
		return;
	end

	for tmpi=1, recvValue.m_count, 1 do
		local posx = recvValue.m_list[tmpi].m_posx
		local posy = recvValue.m_list[tmpi].m_posy
		local nation = recvValue.m_list[tmpi].m_nation
		local level = recvValue.m_list[tmpi].m_level

		local thumbX, thumbY = WorldMapThumb.ConvertMapToThumb( posx, posy );
		if ThumbDisplayCityPrefab == nil then
		   ThumbDisplayCityPrefab = LoadPrefab("ThumbDisplayCity");
		end
		local thumbObj = GameObject.Instantiate( ThumbDisplayCityPrefab );
		thumbObj.transform:SetParent( ThumbCityRoot );
		thumbObj.transform.localPosition = Vector3.New( thumbX, thumbY, 0 );
		
		if level <= 10 then
			thumbObj.transform.localScale = Vector3( 0.15, 0.15, 0.15 );
		elseif level <= 20 then
			thumbObj.transform.localScale = Vector3( 0.3, 0.3, 0.3 );
		else
			thumbObj.transform.localScale = Vector3( 0.4, 0.4, 0.4 );
		end
		thumbObj.transform:GetComponent( "SpriteRenderer" ).color = Hex2Color( MapUnitRangeColor[nation] )
	end
end

-- 设置城镇的位置
-- m_count=0,m_list={m_townid=0,m_nation=0,m_protect_sec=0,m_from_nation="[4]",[m_count]},m_zoneid=0,
function WorldMapThumb.SetTownInfo( recvValue )
	if ThumbDisplayPrefab == nil then
		return;
	end
	if recvValue == nil then
		system_askinfo( ASKINFO_MAPZONE, "", 1 );
		return;
	end
	if recvValue.m_zoneid == 13 then
		SetFalse( ThumbDisplayPrefab.transform:Find("Back1") )
		SetFalse( ThumbDisplayPrefab.transform:Find("Back2") )
	else
		SetTrue( ThumbDisplayPrefab.transform:Find("Back1") )
		SetTrue( ThumbDisplayPrefab.transform:Find("Back2") )
	end
	for tmpi=1, recvValue.m_count, 1 do
		local townid = recvValue.m_list[tmpi].m_townid;
		local nation = recvValue.m_list[tmpi].m_nation;
		local type = g_towninfo[townid].type
		local posx = g_towninfo[townid].posx
		local posy = g_towninfo[townid].posy
		local thumbX, thumbY = WorldMapThumb.ConvertMapToThumb( posx, posy ); 
		if ThumbDisplayTownPrefab == nil then
		   ThumbDisplayTownPrefab = LoadPrefab("ThumbDisplayTown");
		end
		local thumbObj = GameObject.Instantiate( ThumbDisplayTownPrefab );
		thumbObj.transform:SetParent( ThumbDisplayPrefab );
		thumbObj.transform.localPosition = Vector3.New( thumbX, thumbY, 0 );
		thumbObj.transform:GetComponent("ShareData"):AddValue( "PosType", 3 );
		thumbObj.transform:GetComponent("ShareData"):AddValue( "PosIndex", townid );
		-- 名称
		thumbObj.transform:Find( "Name" ):GetComponent( "UIText" ).text = MapTownName( townid )
		-- 形象
		if type == MAPUNIT_TYPE_TOWN_TYPE3 or type == MAPUNIT_TYPE_TOWN_TYPE6 or type == MAPUNIT_TYPE_TOWN_TYPE9 then
			thumbObj.transform:Find( "Shape" ):GetComponent( "SpriteRenderer" ).sprite = LoadSprite( "ui_worldmap_nation_"..nation )
			MapZoneDlgSetNation( nation )
		else
			thumbObj.transform:Find( "Shape" ):GetComponent( "SpriteRenderer" ).sprite = LoadSprite( "ui_worldmap_nation_small_"..nation )
		end
		-- 进攻箭头
		local index = 0;
		for i=1, 3, 1 do
			local from_nation = recvValue.m_list[tmpi].m_from_nation[i]
			local uiArrow = thumbObj.transform:Find( "Arrow" ):GetChild( index );
			if from_nation >= 0 then
				SetTrue( uiArrow )
				uiArrow:GetComponent( "SpriteRenderer" ).sprite = LoadSprite( "ui_mapzone_arrow_"..from_nation )
			else
				SetFalse( uiArrow )
			end
			index = index + 1
		end
		-- 皇城地区的范围
		local Range = thumbObj.transform:Find( "Range" );
		if recvValue.m_zoneid == 13 then
			SetTrue( Range )
			Range:GetComponent( "SpriteRenderer" ).color = Hex2Color( MapUnitRangeColorA[nation] )
		else
			SetFalse( Range )
		end
	end
end

-- 显示关闭蒙版
function WorldMapThumb.MaskVisible( visible )
	if ThumbMaskPrefab then
		ThumbMaskPrefab.gameObject:SetActive( visible );
	end
end

function WorldMapThumb.Alert( text )
	if ThumbUIMod == nil then
		return
	end
	SetTrue( ThumbUIMod )
	SetText( ThumbUIMod.transform:Find("Alert/Text"), text )
end

function WorldMapThumb.AlertClose()
	if ThumbUIMod == nil then
		return
	end
	SetFalse( ThumbUIMod )
end