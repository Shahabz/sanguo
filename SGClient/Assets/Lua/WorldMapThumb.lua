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
local ThumbCamera			= nil;	-- 地图摄像机
local ThumbDisplayTownPrefab= nil;

local ThumbInfoCache = nil;

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
	--fruit.scenceManager.worldMapScence.gameObject:SetActive( false );

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
	ThumbCamera			= nil;
	--fruit.scenceManager.worldMapScence.gameObject:SetActive( true );
	ThumbInfoCache = nil;
	WorldMapThumb.clickEffectObj = nil;
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
	ThumbCamera			= WorldMapThumbPrefab:GetComponent("Transform"):Find( "ThumbCamera" );

	if WorldMapThumb.clickEffectObj == nil then
		WorldMapThumb.clickEffectObj = GameObject.Instantiate( LoadPrefab("ThumbClickEffect") );
		WorldMapThumb.clickEffectObj.transform:SetParent( ThumbDisplayPrefab );
	end
	
	-- 显示我自己的位置
	--WorldMapThumb.SetMyPos();
	
	-- 显示当前位置
	--local thumbX, thumbY = WorldMapThumb.ConvertMapToThumb( WorldMap.m_nLastCameraGameX, WorldMap.m_nLastCameraGameY )
	--WorldMapThumb.SetCurPos( thumbX, thumbY );
	
	-- 获取联盟信息
	--system_askinfo( ASKINFO_WORLDMAP, "", 10 );
	
	-- 通过缓存设置城镇信息
	--WorldMapThumb.SetTownPos( WorldMap.m_cacheTown );
end

-- 缩略图坐标（摄像机）=》地区地图坐标(菱形格)
function WorldMapThumb.ConvertThumbToZone( thumbX, thumbY )
	local worldCameraX = thumbX * (WorldMap.m_nMaxWidth * MAP_TILEWIDTH) / WorldMapThumb.m_nMaxWidth;
	local worldCameraY = thumbY * (WorldMap.m_nMaxHeight * MAP_TILEHEIGHT) / WorldMapThumb.m_nMaxHeight;
	
	local gameCoorX, gameCoorY = WorldMap.ConvertCameraToGame( worldCameraX, worldCameraY );
	gameCoorX = WorldMap.m_nMaxWidth + gameCoorX;
	
	gameCoorX = math.floor(gameCoorX/5) + g_zoneinfo[WorldMapThumb.m_nZoneID].top_left_posx;
	gameCoorY = math.floor(gameCoorY/5) + g_zoneinfo[WorldMapThumb.m_nZoneID].top_left_posy;
	print( gameCoorX ..","..gameCoorY )
	return gameCoorX, gameCoorY;
end

-- 大地图坐标(菱形格)=》缩略图坐标（摄像机）
function WorldMapThumb.ConvertMapToThumb( gameCoorX, gameCoorY )
	local cameraPosX, cameraPosY = WorldMap.ConvertGameToCamera( gameCoorX, gameCoorY );
	local thumbX = cameraPosX/ ((WorldMap.m_nMaxWidth * MAP_TILEWIDTH) / WorldMapThumb.m_nMaxWidth) - WorldMapThumb.m_nMaxWidth/2/100;
	local thumbY = cameraPosY/ ((WorldMap.m_nMaxHeight * MAP_TILEHEIGHT) / WorldMapThumb.m_nMaxHeight) + WorldMapThumb.m_nMaxHeight/2/100;
	return thumbX, thumbY;
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
	if shareData and ThumbInfoCache then
		local posType = tonumber( shareData:GetValue( "PosType" ) );
		local posIndex = tonumber( shareData:GetValue( "PosIndex" ) );
		-- 点击的是我自己位置
		if posType == 1 then
			gameCoorX = WorldMap.m_nMyCityPosx;
			gameCoorY = WorldMap.m_nMyCityPosy;

		-- 点击的是盟主位置
		elseif posType == 2 then
			gameCoorX = ThumbInfoCache.m_leader_pos.m_posx;
			gameCoorY = ThumbInfoCache.m_leader_pos.m_posy;
			
		-- 点击的是联盟成员位置		
		elseif posType == 3 then
			gameCoorX = ThumbInfoCache.m_member_pos[posIndex].m_posx;
			gameCoorY = ThumbInfoCache.m_member_pos[posIndex].m_posy;
		
		-- 点击的是联盟建筑位置		
		elseif posType == 4 then
			gameCoorX = ThumbInfoCache.m_building_pos[posIndex].m_posx;
			gameCoorY = ThumbInfoCache.m_building_pos[posIndex].m_posy;
		
		-- 点击的是城镇位置		
		elseif posType == 5 then
			if WorldMap.m_cacheTown then
				gameCoorX = WorldMap.m_cacheTown.m_list[posIndex].m_posx;
				gameCoorY = WorldMap.m_cacheTown.m_list[posIndex].m_posy;
			end
		end	
		if gameCoorX < 0 or gameCoorY < 0 then
			gameCoorX = 480;
			gameCoorY = 480;
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
		
	Invoke( function()
        if ThumbCamera ~= nil then
		    ThumbCamera:GetComponent("WorldMapThumbCamera"):TweenPosToInBound( Vector3.New( thumbX, thumbY, 0 ), 0.5 );
        end
	end, 0.1 )
end

-- 设置其他位置信息
function WorldMapThumb.SetOtherPos( recvValue )
	ThumbInfoCache = recvValue;
	WorldMapThumb.SetClubLeaderPos( recvValue );
	WorldMapThumb.SetClubMemberPos( recvValue );
	WorldMapThumb.SetClubBuildingPos( recvValue );
end

-- 设置盟主的位置
function WorldMapThumb.SetClubLeaderPos( recvValue )
	if ThumbDisplayPrefab == nil then
		return;
	end
	if recvValue.m_leader_pos.m_posx <= 0 and recvValue.m_leader_pos.m_posy <= 0 then
		return;
	end
	local thumbX, thumbY = WorldMapThumb.ConvertMapToThumb( recvValue.m_leader_pos.m_posx, recvValue.m_leader_pos.m_posy );
	local thumbObj = GameObject.Instantiate( LoadPrefab("ThumbDisplayClubLeader") );
	thumbObj.transform:SetParent( ThumbDisplayPrefab );
	thumbObj.transform.localPosition = Vector3.New( thumbX, thumbY, 0 );
	thumbObj.transform:GetComponent("ShareData"):AddValue( "PosType", 2 );
end

-- 设置盟成员的位置
function WorldMapThumb.SetClubMemberPos( recvValue )
	if ThumbDisplayPrefab == nil then
		return;
	end
	for tmpi=1, recvValue.m_member_count, 1 do
		if recvValue.m_member_pos[tmpi].m_posx >= 0 and recvValue.m_member_pos[tmpi].m_posy >= 0 then
			local thumbX, thumbY = WorldMapThumb.ConvertMapToThumb( recvValue.m_member_pos[tmpi].m_posx, recvValue.m_member_pos[tmpi].m_posy );
			local thumbObj = GameObject.Instantiate( LoadPrefab("ThumbDisplayClubMember") );
			thumbObj.transform:SetParent( ThumbDisplayPrefab );
			thumbObj.transform.localPosition = Vector3.New( thumbX, thumbY, 0 );
			thumbObj.transform:GetComponent("ShareData"):AddValue( "PosType", 3 );
			thumbObj.transform:GetComponent("ShareData"):AddValue( "PosIndex", tmpi );
		end
	end
end

-- 设置联盟建筑的位置
function WorldMapThumb.SetClubBuildingPos( recvValue )
	if ThumbDisplayPrefab == nil then
		return;
	end
	for tmpi=1, recvValue.m_building_count, 1 do
		local thumbX, thumbY = WorldMapThumb.ConvertMapToThumb( recvValue.m_building_pos[tmpi].m_posx, recvValue.m_building_pos[tmpi].m_posy );
		local thumbObj = GameObject.Instantiate( LoadPrefab("ThumbDisplayClubBuilding") );
		thumbObj.transform:SetParent( ThumbDisplayPrefab );
		thumbObj.transform.localPosition = Vector3.New( thumbX, thumbY, 0 );
		thumbObj.transform:GetComponent("ShareData"):AddValue( "PosType", 4 );
		thumbObj.transform:GetComponent("ShareData"):AddValue( "PosIndex", tmpi );
	end
end

-- 设置城镇的位置
function WorldMapThumb.SetTownPos( recvValue )
	if recvValue == nil then
		system_askinfo( ASKINFO_WORLDMAP, "", 14 );
		return;
	end
	if ThumbDisplayPrefab == nil then
		return;
	end
	for tmpi=1, recvValue.m_count, 1 do
		-- 暂时要塞不显示
		if recvValue.m_list[tmpi].m_townid < 8 then
			local thumbX, thumbY = WorldMapThumb.ConvertMapToThumb( recvValue.m_list[tmpi].m_posx, recvValue.m_list[tmpi].m_posy );

            -- 王城/其他
            if recvValue.m_list[tmpi].m_townid == 1 then
                ThumbDisplayPrefab:FindChild( "ThumbDisplayMain" ):GetComponent("ShareData"):AddValue( "PosType", 5 );
                ThumbDisplayPrefab:FindChild( "ThumbDisplayMain" ):GetComponent("ShareData"):AddValue( "PosIndex", tmpi );
            else

--			    if ThumbDisplayTownPrefab == nil then
--				    ThumbDisplayTownPrefab = LoadPrefab("ThumbDisplayTown");
--			    end
--			    local thumbObj = GameObject.Instantiate( ThumbDisplayTownPrefab );
--			    thumbObj.transform:SetParent( ThumbDisplayPrefab );
--			    thumbObj.transform.localPosition = Vector3.New( thumbX, thumbY, 0 );
--			    thumbObj.transform:GetComponent("ShareData"):AddValue( "PosType", 5 );
--			    thumbObj.transform:GetComponent("ShareData"):AddValue( "PosIndex", tmpi );

--                if recvValue.m_list[tmpi].m_townid  < 8 then
--                    thumbObj.transform:FindChild( "Icon" ):GetComponent( "SpriteRenderer" ).sprite = GetResSpriteByType( 3 );
--                else
--                    thumbObj.transform:FindChild( "Icon" ):GetComponent( "SpriteRenderer" ).sprite = GetResSpriteByType( 2 );
--                end
            end
		end
	end
end

-- 显示关闭蒙版
function WorldMapThumb.MaskVisible( visible )
	if ThumbMaskPrefab then
		ThumbMaskPrefab.gameObject:SetActive( visible );
	end
end