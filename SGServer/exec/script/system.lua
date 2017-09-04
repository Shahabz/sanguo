
local totle_minute;
function IN_GetCondSql( cond, sql )
	
end

-- 系统启动时执行
function IN_Script_Init()
	math.randomseed(os.time())
	print( "global.actorlevel_max="..global.actorlevel_max )
end

-- 系统每分钟执行一次
function IN_Script_Timer()
	WorldMapBrushTimer()
end

-- 系统每小时执行一次
function IN_Script_Hour()

end

-- 整点执行
function IN_OnClockProcess( hour )
	if hour == 3 then

	end
end

-- 系统时间字符串
function IN_TimeString( timestamp )
	return os.date("%Y-%m-%d %X", timestamp), 0;
end

-- 系统GM指令
function IN_Script_Command( v1, v2, v3, v4, msg, PlayerIdx )
	local temp = os.date("*t", os.time());
	
	-- 刷流寇
	if v1 == 1 then
		if v2 == 0 then
			BrushEnemy()
		else
			BrushEnemyWithZone( v2 )
		end
	
	-- 刷资源点
	elseif v1 == 2 then
		if v2 == 0 then
			BrushResClear()
			BrushRes()
		else
			BrushResWithZone( v2 )
		end
	end
	return 0;
end

function IN_Script_Exec( id, value1, value2 )
	if id == 1 then
		BrushEnemyWithZone( value1 )
	elseif id == 10 then
		BrushEventWithCity( value1, value2 )
	end
end

