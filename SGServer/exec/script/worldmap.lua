-- �����ͼ
local TMX_WIDTH 			= 100;  	-- һ��tmx��ͼ��С
local TMX_HEIGHT 			= 100;  	-- һ��tmx��ͼ��С

MAPUNIT_TYPE_CITY			=	1	-- �ǳ�
MAPUNIT_TYPE_ARMY			=	2	-- ����
MAPUNIT_TYPE_TOWN			=	3	-- ����
MAPUNIT_TYPE_ENEMY			=	4	-- ����
MAPUNIT_TYPE_RES			=	5	-- ��Դ

g_map_enemy_maxcount		= 30000;-- �����������
g_map_res_maxcount			= 5000;	-- ��Դ���������
g_map_event_maxcount		= 100000;-- ��ͼ�¼��������

-- �����ͼ��ʼ��
function IN_OnWorldMapInit( nMaxWidth, nMaxHeight )
	
	-- �����������
	c_map_enemy_maxcount( g_map_enemy_maxcount );
	
	-- ��Դ���������
	c_map_res_maxcount( g_map_res_maxcount );
	
	-- ��ͼ�¼��������
	c_map_event_maxcount( g_map_event_maxcount );
	
end

-- ��ͼ�赲
function IN_OnWorldMapBlock( posx, posy )
	-- ��ת����һС���ͼ�����꣬��Ϊ��ͼ��N*N��ƴ������
	local localposx = math.floor( posx % TMX_WIDTH );
	local localposy = math.floor( posy % TMX_HEIGHT );
	local index = localposx + localposy*TMX_HEIGHT + 1;	
	return 0;
end

-- ˢ�ַ���������ʱ���ˢ�������ǲ��䣬ˢ�����ʱ����
function IN_OnWorldMapBrush()
	WorldMapBrushTimer()
end

-- ������������ˢ����(1����һ��)
function WorldMapBrushTimer()
	local minute = 0;
	minute = c_world_data_get( 1 )
	if minute == 0 or minute >= global.brush_enemy_minute then
		BrushEnemy()
	else
		c_world_data_set( 1, minute + 1 );
	end
	
	minute =  c_world_data_get( 2 )
	if minute == 0 or minute >= global.brush_res_minute then
		BrushRes()
	else
		c_world_data_set( 2, minute + 1 );
	end
end

-- ˢ����
function BrushEnemy()
	c_world_data_set( 1, 1 );
	
	-- ɾ������δѡ�е�����
	local nowtime = os.time();
	for index = 0, g_map_enemy_maxcount-1, 1 do
		local level, posx, posy, deltime, selected_count, kind = c_map_enemy_info( index );
		if kind > 0 and kind <= 40 and selected_count <= 0 then
			if deltime <= 0 then
				c_map_enemy_delete( index );
			elseif nowtime >= deltime then
				c_map_enemy_delete( index );
			end
		end
	end
	
	-- ������ˢ��
	for zoneid=1, g_zoneinfo_maxnum-1, 1 do
		local brush = 1;
		if g_zoneinfo[zoneid].type == 1 then
			if c_get_open_zone_sili() == 0 then
				-- ˾��û��
				brush = 0;
			end
		end
		if brush == 1 then
			c_brush_enemy_queue_add( 0, zoneid )
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
	
	-- �Ѽ����еĿ���λ��
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
	
	-- ϴ�ƴ����ѵ��Ŀ��������
	-- ��Ϊ table.remove( emptylist, randidx ); -- Ч�ʷǳ��ͣ����Բ�����ˣ�˳��ȡ
	for  i = 1, #emptylist, 1 do  
		local index = math.random( 1, #emptylist );
		emptylist[i], emptylist[index] = emptylist[index],emptylist[i]
	end

	
	-- ˢ���ܼ���
	local kindgroup = g_zoneinfo[zoneid].enemykind;
	local numgroup = g_zoneinfo[zoneid].enemynum;
	local kindlist = string.split( kindgroup, "," );
	local numlist = string.split( numgroup, "," );
	local indexcount = 1;
	
	-- ˢ����
	for i=1, #kindlist, 1 do
		local kind = tonumber(kindlist[i])
		local num = tonumber(numlist[i]);
		local hasnum = c_map_enemy_num( zoneid, kind );
		if num ~= nil and num > 0 then
			for j=1, (num-hasnum), 1 do
				if #emptylist == 0 or indexcount > #emptylist then
					break;
				end
				--local randidx = math.random( 1, #emptylist );
				c_map_enemy_create( kind, emptylist[indexcount].m_posx, emptylist[indexcount].m_posy, 0, 0 );
				indexcount = indexcount + 1;
				--table.remove( emptylist, randidx ); -- �˷���Ч�ʷǳ���
			end
		end
	end
end

function BrushResClear()
	for index = 0, g_map_res_maxcount-1, 1 do
		c_map_res_delete( index );
	end
end

-- ˢ��Դ��
function BrushRes()
	c_world_data_set( 2, 1 );
	-- ������ˢ��
	for zoneid=1, g_zoneinfo_maxnum-1, 1 do
		local brush = 1;
		if g_zoneinfo[zoneid].type == 1 then
			if c_get_open_zone_sili() == 0 then
				-- ˾��û��
				brush = 0;
			end
		end
		if brush == 1 then
			c_brush_enemy_queue_add( 1, zoneid )
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
	
	-- �Ѽ����еĿ���λ��
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
	
	-- ϴ�ƴ����ѵ��Ŀ��������
	-- ��Ϊ table.remove( emptylist, randidx ); -- Ч�ʷǳ��ͣ����Բ�����ˣ�˳��ȡ
	for  i = 1, #emptylist, 1 do
		local index = math.random( 1, #emptylist );
		emptylist[i], emptylist[index] = emptylist[index],emptylist[i]
	end
	-- ˢ��Դ�㼯��
	local kindgroup = g_zoneinfo[zoneid].reskind;
	local numgroup = g_zoneinfo[zoneid].resnum;
	local kindlist = string.split( kindgroup, "," );
	local numlist = string.split( numgroup, "," );
	local indexcount = 1;
	
	-- ˢ��Դ��
	for i=1, #kindlist, 1 do
		local kind = tonumber(kindlist[i])
		local num = tonumber(numlist[i]);
		local hasnum = c_map_res_num( zoneid, kind );
		if num ~= nil and num > 0 then
			for j=1, (num-hasnum), 1 do
				if #emptylist == 0 or indexcount > #emptylist then
					break;
				end
				--local randidx = math.random( 1, #emptylist );
				c_map_res_create( kind, emptylist[indexcount].m_posx, emptylist[indexcount].m_posy );
				indexcount = indexcount + 1;
				--table.remove( emptylist, randidx );-- �˷���Ч�ʷǳ���
			end
		end
	end
end

-- ˢ��ͼ�¼�
function BrushEventWithCity( city_index, zoneid )
	if zoneid <= 0 or zoneid >= g_zoneinfo_maxnum then
		return
	end
	
	-- ����ǳ���Ϣ
	local actorid, posx, posy, level = c_city_baseinfo( city_index );
	if actorid <= 0 then
		return
	end
	
	-- ɾ�������ҵ������¼�
	c_map_event_delete_actor( actorid, city_index );
	
	-- ����ˢ�¼�
	local eventgroup = g_zoneinfo[zoneid].eventkind;
	local num = g_zoneinfo[zoneid].eventnum;
	local kindlist = string.split( eventgroup, "," );
	for i=1, num, 1 do
		local idx = math.random( 1, #kindlist );
		c_map_event_range_brush( kindlist[idx], posx, posy, 2, city_index );
	end
	
end
