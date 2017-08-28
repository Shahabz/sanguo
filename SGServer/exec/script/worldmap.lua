-- 世界地图
local TMX_WIDTH 			= 100;  	-- 一块tmx地图大小
local TMX_HEIGHT 			= 100;  	-- 一块tmx地图大小

MAPUNIT_TYPE_CITY			=	1	-- 城池
MAPUNIT_TYPE_ARMY			=	2	-- 部队
MAPUNIT_TYPE_TOWN			=	3	-- 城镇
MAPUNIT_TYPE_ENEMY			=	4	-- 流寇
MAPUNIT_TYPE_RES			=	5	-- 资源

g_map_enemy_maxcount		= 30000;	-- 流寇最大数量
g_map_res_maxcount			= 5000;	-- 资源点最大数量

-- 世界地图初始化
function IN_OnWorldMapInit( nMaxWidth, nMaxHeight )
	
	-- 流寇最大数量
	c_map_enemy_maxcount( g_map_enemy_maxcount );
	
	-- 资源点最大数量
	c_map_res_maxcount( g_map_res_maxcount );
	
end

-- 地图阻挡
function IN_OnWorldMapBlock( posx, posy )
	-- 先转换成一小块地图的坐标，因为地图是N*N的拼起来的
	local localposx = math.floor( posx % TMX_WIDTH );
	local localposy = math.floor( posy % TMX_HEIGHT );
	local index = localposx + localposy*TMX_HEIGHT + 1;	
	return 0;
end

-- 刷怪服务器启动时候的刷对象，算是补充，刷对象计时重置
function IN_OnWorldMapBrush()
	BrushEnemy()
	BrushRes()
end

-- 服务器运行中刷对象(1分钟一次)
function WorldMapBrushTimer()
	local minute = 0;
	minute = c_world_data_get( 1 )
	if minute >= global.brush_enemy_minute then
		BrushEnemy()
	else
		c_world_data_set( 1, minute + 1 );
	end
	
	minute =  c_world_data_get( 2 )
	if minute >= global.brush_res_minute then
		BrushRes()
	else
		c_world_data_set( 2, minute + 1 );
	end
end

-- 刷流寇
function BrushEnemy()
	c_world_data_set( 1, 0 );
	
	-- 删除所有未选中的流寇
	local nowtime = os.time();
	for index = 0, g_map_enemy_maxcount-1, 1 do
		local level, posx, posy, deltime, selected_count, kind = c_map_enemy_info( index );
		if kind > 0 and selected_count <= 0 then
			if deltime <= 0 then
				c_map_enemy_delete( index );
			elseif nowtime >= deltime then
				c_map_enemy_delete( index );
			end
		end
	end
	
	-- 按地区刷新
	for zoneid=1, g_zoneinfo_maxnum-1, 1 do
		if g_zoneinfo[zoneid].open == 1 then
			c_brush_enemy_queue_add( zoneid )
		end
	end
end
function BrushEnemyWithZone( zoneid )
	if zoneid <= 0 or zoneid >= g_zoneinfo_maxnum then
		return
	end
	if g_zoneinfo[zoneid].open == 0 then
		return
	end
	
	-- 搜集所有的空余位置
	local emptylist = {};
	local totalvalue = 0;
	for posx = g_zoneinfo[zoneid].top_left_posx, g_zoneinfo[zoneid].bottom_right_posx, 1 do
		for posy = g_zoneinfo[zoneid].top_left_posy, g_zoneinfo[zoneid].bottom_right_posy, 1 do
			if c_map_canmove( posx, posy ) == 1 then
				table.insert( emptylist, {m_posx=posx, m_posy=posy, m_value=2} );
				totalvalue = totalvalue + 2;
			end
		end
	end
	
	-- 刷流寇集合
	local kindgroup = g_zoneinfo[zoneid].enemykind;
	local numgroup = g_zoneinfo[zoneid].enemynum;
	local kindlist = string.split( kindgroup, "," );
	local numlist = string.split( numgroup, "," );
	
	-- 刷流寇
	for i=1, #kindlist, 1 do
		local kind = tonumber(kindlist[i])
		local num = tonumber(numlist[i]);
		local hasnum = c_map_enemy_num( zoneid, kind );
		if num ~= nil and num > 0 then
			for j=1, (num-hasnum), 1 do
				if #emptylist == 0 then
					break;
				end
				local randidx = math.random( 1, #emptylist );
				c_map_enemy_create( kind, emptylist[randidx].m_posx, emptylist[randidx].m_posy, 0 );
				table.remove( emptylist, randidx );
			end
		end
	end
end

-- 刷资源点
function BrushRes()
	c_world_data_set( 2, 0 );
	-- 按地区刷新
	for zoneid=1, g_zoneinfo_maxnum-1, 1 do
		if g_zoneinfo[zoneid].open == 1 then
			BrushResWithZone( zoneid );
		end
	end
end
function BrushResWithZone( zoneid )
	if zoneid <= 0 or zoneid >= g_zoneinfo_maxnum then
		return
	end
	if g_zoneinfo[zoneid].open == 0 then
		return
	end
	
	-- 搜集所有的空余位置
	local emptylist = {};
	local totalvalue = 0;
	for posx = g_zoneinfo[zoneid].top_left_posx, g_zoneinfo[zoneid].bottom_right_posx, 1 do
		for posy = g_zoneinfo[zoneid].top_left_posy, g_zoneinfo[zoneid].bottom_right_posy, 1 do
			if c_map_canmove( posx, posy ) == 1 then
				table.insert( emptylist, {m_posx=posx, m_posy=posy, m_value=2} );
				totalvalue = totalvalue + 2;
			end
		end
	end
	-- 刷资源点集合
	local kindgroup = g_zoneinfo[zoneid].reskind;
	local numgroup = g_zoneinfo[zoneid].resnum;
	local kindlist = string.split( kindgroup, "," );
	local numlist = string.split( numgroup, "," );
	
	-- 刷资源点
	for i=1, #kindlist, 1 do
		local kind = tonumber(kindlist[i])
		local num = tonumber(numlist[i]);
		local hasnum = c_map_res_num( zoneid, kind );
		if num ~= nil and num > 0 then
			for j=1, (num-hasnum), 1 do
				if #emptylist == 0 then
					break;
				end
				local randidx = math.random( 1, #emptylist );
				c_map_res_create( kind, emptylist[randidx].m_posx, emptylist[randidx].m_posy );
				table.remove( emptylist, randidx );
			end
		end
	end
end
