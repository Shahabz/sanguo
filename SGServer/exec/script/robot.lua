-- 机器人

-- 机器人总量百分比，0.3表示总城池数量的百分之五十
local g_RobotTotalPer = 0;

-- 国家占比
-- 156	中国
-- 360	印尼
-- 458	马来
-- 608	菲律宾
-- 643	俄罗斯
-- 840	美国
-- 702	新加坡

local g_RobotCountryPer = {
{ country = 156, name_country = 156, name_head="CN", enemy = {840,392}, per=0.2, ai={ 28,29,30,31,32,35,38,39 }, aiper={ 0.125,0.125,0.125,0.125,0.125,0.125,0.125,0.125,} }, 
{ country = 702, name_country = 158, name_head="TW", enemy = {840,392}, per=0.1, ai={ 33,34,36,37,38,39,40 }, aiper={ 0.1,0.1,0.1,0.1,0.2,0.2,0.2,} }, 
{ country = 392, name_country = 392, name_head="JP", enemy = {840,392}, per=0.12, ai={ 33,34,36,37,38,39,40 }, aiper={ 0.1,0.1,0.1,0.1,0.2,0.2,0.2,} }, 
{ country = 410, name_country = 410, name_head="KR", enemy = {840,392}, per=0.12, ai={ 33,34,36,37,38,39,40 }, aiper={ 0.1,0.1,0.1,0.1,0.2,0.2,0.2,} }, 
{ country = 704, name_country = 704, name_head="VN", enemy = {840,392}, per=0.06, ai={ 28,29,30,31,32,35,38,39 }, aiper={ 0.125,0.125,0.125,0.125,0.125,0.125,0.125,0.125,} }, 
{ country = 764, name_country = 764, name_head="TH", enemy = {840,392}, per=0.1, ai={ 28,29,30,31,32,35,38,39 }, aiper={ 0.125,0.125,0.125,0.125,0.125,0.125,0.125,0.125,} }, 
{ country = 458, name_country = 158, name_head="TW", enemy = {840,392}, per=0.08, ai={ 28,29,30,31,32,35,38,39 }, aiper={ 0.125,0.125,0.125,0.125,0.125,0.125,0.125,0.125,} }, 
{ country = 608, name_country = 158, name_head="TW", enemy = {840,392}, per=0.06, ai={ 28,29,30,31,32,35,38,39 }, aiper={ 0.125,0.125,0.125,0.125,0.125,0.125,0.125,0.125,} }, 
{ country = 276, name_country = 704, name_head="DE", enemy = {840,392}, per=0.08, ai={ 33,34,36,37,38,39,40 }, aiper={ 0.1,0.1,0.1,0.1,0.2,0.2,0.2,} }, 
{ country = 250, name_country = 704, name_head="FR", enemy = {840,392}, per=0.08, ai={ 33,34,36,37,38,39,40 }, aiper={ 0.1,0.1,0.1,0.1,0.2,0.2,0.2,} }, 
}

-- 创建机器人
function RobotCreate()
	local has = c_world_data_get( 7 );
	if has > 0 then
		return;
	end
	
	if g_RobotTotalPer == 0 then
		return;
	end
	-- 最多的城池数量
	local city_maxcount = c_city_maxcount();
	-- 最多创建多少机器人
	local robot_count = math.floor( city_maxcount * g_RobotTotalPer );
	local index = 1;
	--c_db_start_transaction();	
	for _, v in ipairs( g_RobotCountryPer ) do
		-- 这个国家创建多少机器人
		local country_num = math.floor( v["per"] * robot_count );
		for n, m in ipairs( v["aiper"] ) do
			-- 这个AI创建多少机器人
			local ai_num = math.floor( m * country_num );
			for tmpi=1, ai_num, 1 do
				-- 创建
				c_robot_create( v["ai"][n], string.format( v["name_head"].."%05d", index ), v["country"], v["name_country"] );
				index = index + 1;
			end
		end
	end
	--c_db_commit();
	-- 记录已经创建过
	c_world_data_set( 7, 1 );
end
