
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
	--[[for i=0,g_itemkind_maxnum-1,1 do
		local pitem = g_itemkind[i];
		if pitem ~= nil then
			print( "kind=".. pitem.m_kind .."	ability0="..pitem.m_base_ability[0].."	ability1="..pitem.m_base_ability[1].."	value0="..pitem.m_base_value[0].."	value1="..pitem.m_base_value[1].." overlap="..pitem.m_overlap.."\n" );
		end
	end
	print( "g_itemkind_maxnum="..g_itemkind_maxnum.."\n" );--]]
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
	
	if v1 == 1 then
		
	end
	return 0;
end

function IN_Script_Exec( id, value1, value2 )
	
end

