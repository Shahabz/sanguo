
local totle_minute;
function IN_GetCondSql( cond, sql )
	
end

-- ϵͳ����ʱִ��
function IN_Script_Init()
	math.randomseed(os.time())
	print( "global.actorlevel_max="..global.actorlevel_max )
end

-- ϵͳÿ����ִ��һ��
function IN_Script_Timer()
	WorldMapBrushTimer()
end

-- ϵͳÿСʱִ��һ��
function IN_Script_Hour()

end

-- ����ִ��
function IN_OnClockProcess( hour )
	if hour == 3 then

	end
end

-- ϵͳʱ���ַ���
function IN_TimeString( timestamp )
	return os.date("%Y-%m-%d %X", timestamp), 0;
end

-- ϵͳGMָ��
function IN_Script_Command( v1, v2, v3, v4, msg, PlayerIdx )
	local temp = os.date("*t", os.time());
	
	-- ˢ����
	if v1 == 1 then
		if v2 == 0 then
			BrushEnemy()
		else
			BrushEnemyWithZone( v2 )
		end
	
	-- ˢ��Դ��
	elseif v1 == 2 then
		if v2 == 0 then
			BrushResClear()
			BrushRes()
		else
			BrushResWithZone( v2 )
		end
	-- ˢʰȡ��Ʒ
	elseif v1 == 3 then
		if v2 == 0 then
			BrushPickupClear()
			BrushPickup()
		else
			BrushPickupWithZone( v2 )
		end
	end
	return 0;
end

function IN_Script_Exec( id, value1, value2 )
	if id == 1 then
		BrushEnemyWithZone( value1 )
	elseif id == 2 then
		BrushResWithZone( value1 )
	elseif id == 3 then
		BrushPickupWithZone( value1 )
	elseif id == 10 then
		BrushEventWithCity( value1, value2 )
	end
end

