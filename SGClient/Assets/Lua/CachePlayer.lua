-- 一次性变量
ACTOR_SFLAG_BODY_FREEBUY	=	0	-- 首次购买体力免费
ACTOR_SFLAG_MAPZONE_GO_ZC	=	1	-- 首次免费前往州城
ACTOR_SFLAG_QUEST_AUTOBUILD	=	2	-- 新手任务是否领取过自动建造
ACTOR_SFLAG_SKIPFIGHT		=	3	-- 是否可以跳过战斗
ACTOR_SFLAG_PALACE			=	4	-- 是否开启皇宫内院
ACTOR_SFLAG_STORYSWEEP		=	5	-- 是否开启扫荡副本
ACTOR_SFLAG_OFFICIAL_TECH	=	6   -- 是否开启紫色研究员
ACTOR_SFLAG_EQUPIPDRAWING	=	7	-- 是否开启装备分解获得图纸
ACTOR_SFLAG_MATERIAL_MAKEWILL = 8   -- 是否开启作坊预设

CITY_BUFF_MAX		=	6 -- buff数量
CITY_BUFF_MARCH		=	0 -- 军曹管buff 行军耗时降低15%
CITY_BUFF_TRAIN		=	1 -- 武卒管buff 招募耗时降低15%
CITY_BUFF_WIND		=	2 -- 风字令buff 行军耗时降低15%
CITY_BUFF_FIRE		=	3 -- 火字令buff 士兵攻击+5%
CITY_BUFF_MOUNTAIN	=	4 -- 山字令buff 士兵防御+5%
CITY_BUFF_FOREST	=	5 -- 林字令buff 在兵营中额外增加20%伤病恢复

--  角色缓存信息
local Player = class("Player");
function Player:ctor()
	self:Init();
end
function Player:Init()
	self.m_usertype			=	0;	-- 用户类型
	self.m_createtime		=	0;	-- 创建时间
	self.m_config		    =   {};  --配置信息
	self.m_servertime 		= 	0;
	self.m_clienttime 		= 	0
	
	self.m_actorid			=	0;	-- 角色编号
	self.m_name				=	"";	-- 角色名称
	self.m_state			=	0;
	self.m_nation			=	0;
	self.m_posx				=	0;
	self.m_posy				=	0;
	self.m_shape			=	0;
	self.m_level			=	0;
	self.m_exp				=	0;
	self.m_exp_max			=	0;
	self.m_token			=	0;
	self.m_viplevel			=	0;
	self.m_vipexp			=	0;
	self.m_vipexp_max		=	0;
	self.m_body				=	0;
	self.m_bodysec			=	0;
	self.m_place			=	0;
	self.m_official			=	0;
	self.m_zone				=	0;
	self.m_battlepower		=	0;
	self.m_silver			=	0;
	self.m_wood				=	0;
	self.m_food				=	0;
	self.m_iron				=	0;
	self.m_infantry_num		=	0;
	self.m_cavalry_num		=	0;
	self.m_archer_num		=	0;
	self.m_prestige			=	0;
	self.m_mokilllv			=	0;
	self.m_actor_sflag		=	0;
	self.m_sflag			=	0;
	self.m_autobuild		=	0;
	self.m_autobuildopen	=	0;
	self.m_autoguard		=	0;
	self.m_autoguardopen	=	0;
	self.m_ptsec			=	0;
	self.m_forgingkind		=	0;
	self.m_forgingsec		=	0;
	self.m_equipext			=	0;
	self.m_techlevel		=   {};
	self.m_techprogress		=	{};
	self.m_officialhire		=	{};
	self.m_guardnum			=	0;
	self.m_guardsec			=	0;
	self.m_questid			=	0;
	self.m_storyid			=	0;
	
	self.m_function			=	0;
	self.m_levynum			=	0;
	self.m_worker_op		=	0;
	self.m_worker_kind		=	0;
	self.m_worker_offset	=	0;
	self.m_worker_sec		=	0;
	self.m_worker_needsec   =	0;
	self.m_worker_free   	=	0;
	self.m_worker_op_ex		=	0;
	self.m_worker_kind_ex	=	0;
	self.m_worker_offset_ex	=	0;
	self.m_worker_sec_ex	=	0;
	self.m_worker_needsec_ex=	0;
	self.m_worker_free_ex   =	0;
	self.m_worker_expire_ex	=	0;
	self.m_wnquick			=	0;
	self.m_wnquick_ex		=	0;
	self.m_buildings 		=	{};
	self.m_buildings_res 	=	{};
	self.m_attr 			= 	{};
	self.m_blacklist		= 	{};
	self.m_buff_endtime		= 	{};
	self.m_bufftrain		= 	{};
	self.m_open_zone_sili	=	0;
	self.m_open_zone_luoyang=	0;
	self.m_shape_bag		=	0;
	self.m_maidname 		= "";
	self.m_equip_washnum	= 0;
	self.m_hero_washnum		= 0;
	self.m_token_sale		= 0;
	self.m_token_ret		= 0;
	self.m_edquest_point	= 0;
	
	self.m_view_zoneid		=	0;
	self.m_award_mailid		=	int64.new(0);
end

-- 属性变化
function Player:Set( recvValue )
	self.m_actorid			=	recvValue.m_actorid;
	self.m_name				=	recvValue.m_name;
	self.m_state			=	recvValue.m_state;
	self.m_nation			=	recvValue.m_nation;
	self.m_posx				=	recvValue.m_posx;
	self.m_posy				=	recvValue.m_posy;
	self.m_shape			=	recvValue.m_shape;
	self.m_level			=	recvValue.m_level;
	self.m_exp				=	recvValue.m_exp;
	self.m_exp_max			=	recvValue.m_exp_max;
	self.m_token			=	recvValue.m_token;
	self.m_viplevel			=	recvValue.m_viplevel;
	self.m_vipexp			=	recvValue.m_vipexp;
	self.m_vipexp_max		=	recvValue.m_vipexp_max;
	self.m_body				=	recvValue.m_body;
	self.m_bodysec			=	recvValue.m_bodysec
	self.m_place			=	recvValue.m_place;
	self.m_official			=	recvValue.m_official;
	self.m_zone				=	recvValue.m_zone;
	self.m_battlepower		=	recvValue.m_battlepower;
	self.m_silver			=	recvValue.m_silver;
	self.m_wood				=	recvValue.m_wood;
	self.m_food				=	recvValue.m_food;
	self.m_iron				=	recvValue.m_iron;
	self.m_infantry_num		=	recvValue.m_infantry_num;
	self.m_cavalry_num		=	recvValue.m_cavalry_num;
	self.m_archer_num		=	recvValue.m_archer_num;
	self.m_mokilllv			=	recvValue.m_mokilllv;
	self.m_actor_sflag		=	recvValue.m_actor_sflag;
	self.m_sflag			=	recvValue.m_sflag;
	self.m_autobuild		=	recvValue.m_autobuild;
	self.m_autobuildopen	=	recvValue.m_autobuildopen;
	self.m_autoguard		=	recvValue.m_autoguard;
	self.m_autoguardopen	=	recvValue.m_autoguardopen;
	self.m_ptsec			=	recvValue.m_ptsec;
	self.m_forgingkind		=	recvValue.m_forgingkind;
	self.m_forgingsec		=	recvValue.m_forgingsec;
	self.m_guardnum			=	recvValue.m_guardnum
	self.m_guardsec			=	recvValue.m_guardsec
	self.m_questid			=	recvValue.m_questid
	self.m_storyid			=	recvValue.m_storyid
	self.m_prestige			=	recvValue.m_prestige
	for kind = 1, 40, 1 do
		self.m_techlevel[kind-1]	=   recvValue.m_techlevel[kind];
		self.m_techprogress[kind-1]	=	recvValue.m_techprogress[kind];
	end
	for i = 1, 3, 1 do
		self.m_officialhire[i-1] = recvValue.m_officialhire[i]
	end
	for i = 1, CITY_BUFF_MAX, 1 do
		self.m_buff_endtime[i-1] = recvValue.m_buff_endtime[i]
	end
	self.m_bufftrain		= 	recvValue.m_bufftrain
	self.m_open_zone_sili		=	recvValue.m_open_zone_sili;
	self.m_open_zone_luoyang	=	recvValue.m_open_zone_luoyang;
	self.m_shape_bag		=	recvValue.m_shape_bag;
	self.m_maidname			=	recvValue.m_maidname;
	self.m_equip_washnum	= 	recvValue.m_equip_washnum;
	self.m_hero_washnum		= 	recvValue.m_hero_washnum;
	self.m_token_sale		=   recvValue.m_token_sale;
	self.m_token_ret		=   recvValue.m_token_ret;
end

--
function Player:SetAttr( recvValue )
	self.m_attr = recvValue;
end
-- 
function Player:SetBuilding( kind, info, active )
	self.m_buildings[kind] = info;
	--
	local unitObj = City.BuildingAdd( info, active );
	
	-- 完成数值状态
	if info.m_overvalue > 0 then
		City.BuildingSetOver( kind );
	end
	
	-- 加速标记
	if info.m_quick > 0 then
		City.BuildingSetQuick( kind );
	end
	return unitObj;
end

function Player:SetBuildingRes( kind, offset, info, active )
	if self.m_buildings_res[kind] == nil then
		self.m_buildings_res[kind] = {};
	end
	self.m_buildings_res[kind][offset] = info;
	-- 
	local unitObj = City.BuildingAdd( info, active );
	return unitObj;
end

function Player:SetBuildingWorker( recvValue )
	self.m_worker_op		=	recvValue.m_worker_op;
	self.m_worker_kind		=	recvValue.m_worker_kind;
	self.m_worker_offset	=	recvValue.m_worker_offset;
	self.m_worker_sec		=	recvValue.m_worker_sec;
	self.m_worker_needsec   =	recvValue.m_worker_needsec;
	self.m_worker_free   	=	recvValue.m_worker_free;
	self.m_worker_op_ex		=	recvValue.m_worker_op_ex;
	self.m_worker_kind_ex	=	recvValue.m_worker_kind_ex;
	self.m_worker_offset_ex	=	recvValue.m_worker_offset_ex;
	self.m_worker_sec_ex	=	recvValue.m_worker_sec_ex;
	self.m_worker_needsec_ex=	recvValue.m_worker_needsec_ex;
	self.m_worker_free_ex   =	recvValue.m_worker_free_ex;
	self.m_worker_expire_ex	=	recvValue.m_worker_expire_ex;
	self.m_wnquick			=	recvValue.m_wnquick;
	self.m_wnquick_ex		=	recvValue.m_wnquick_ex;
	-- 
	City.BuildingWorker();
	MainDlgSetWorker();
end

function Player:SetBuildingLevy( levynum )
	local old 				= 	self.m_levynum;
	self.m_levynum			=	levynum;
	if GetPlayer().m_questid < 80 then
		return
	end
	--[[征收次数=12时，所有的资源田（100%）上方都悬浮一个金色气泡
征收次数=11时，所有的资源田（90%）显示为金色气泡，10%显示绿色气泡
征收次数=10时，所有的资源田（80%）显示为金色气泡，20%显示绿色气泡
征收次数=9时，所有的资源田（70%）显示为金色气泡，30%显示绿色气泡
征收次数=8时，所有的资源田（60%）显示为金色气泡，40%显示绿色气泡
征收次数=7时，所有的资源田（50%）显示为金色气泡，50%显示绿色气泡
征收次数=6时，所有的资源田（40%）显示为金色气泡，60%显示绿色气泡
征收次数=5时，所有的资源田（0%）显示为金色气泡，40%显示绿色气泡
征收次数=4时，每种资源田各有4个绿色泡泡
征收次数=3时，每种资源田各有3个绿色泡泡
征收次数=2时，每种资源田各有2个绿色泡泡
征收次数=1时，每种资源田各有1个绿色泡泡--]]
	
	-- 获取每种建筑数量
	local total = 0
	local res_goldnum = {}
	local res_greennum = {}
	local resnum = {}
	for i=21, 24, 1 do
		if self.m_buildings_res[i] then
			resnum[i] = 0
			res_goldnum[i] = 0
			res_greennum[i] = 0;
			for k, v in pairs( self.m_buildings_res[i] ) do
				total = total + 1
				resnum[i] = resnum[i] + 1
			end
		end
	end
	
	local goldnum = 0
	local greennum = 0
	if levynum >= 12 then
		goldnum = total
		greennum = total - goldnum
		
	elseif levynum == 11 then
		goldnum = math.floor(total*0.9)
		greennum = total - goldnum

	elseif levynum == 10 then
		goldnum = math.floor(total*0.8)
		greennum = total - goldnum
		
	elseif levynum == 9 then
		goldnum = math.floor(total*0.7)
		greennum = total - goldnum
		
	elseif levynum == 8 then
		goldnum = math.floor(total*0.6)
		greennum = total - goldnum
		
	elseif levynum == 7 then
		goldnum = math.floor(total*0.5)
		greennum = total - goldnum
		
	elseif levynum == 6 then
		goldnum = math.floor(total*0.4)
		greennum = total - goldnum
		
	elseif levynum == 5 then
		goldnum = 0
		greennum = math.floor(total*0.4)
	
	elseif levynum == 4 then
		goldnum = 0
		greennum = 4
		
	elseif levynum == 3 then
		goldnum = 0
		greennum = 3
		
	elseif levynum == 2 then
		goldnum = 0
		greennum = 2
		
	elseif levynum == 1 then
		goldnum = 0
		greennum = 1
	else
		goldnum = 0
		greennum = 0
	end
	
	-- 计算每一种资源金色数量
	local temp = goldnum
	for tmpi=1, goldnum do
		for i=21, 24, 1 do
			if res_goldnum[i] then
				res_goldnum[i] = res_goldnum[i] + 1
				temp = temp - 1
				if temp <= 0 then
					break
				end
			end
		end
		if temp <= 0 then
			break
		end
	end
	
	-- 计算每一种资源绿色数量
	if levynum > 4 then
		local temp = greennum
		for tmpi=1, greennum do
			for i=21, 24, 1 do
				if res_greennum[i] then
					res_greennum[i] = res_greennum[i] + 1
					temp = temp - 1
					if temp <= 0 then
						break
					end
				end
			end
			if temp <= 0 then
				break
			end
		end
	else
		for i=21, 24, 1 do
			if res_greennum[i] then
				res_greennum[i] = greennum
			end
		end
	end
		
	-- 先全部隐藏
	for i=21, 24, 1 do
		if City.m_Buildings_res[i] then
			for k, v in pairs( City.m_Buildings_res[i] ) do
				local obj = v:Find("LevyMod").gameObject;
				if obj and obj.activeSelf == true then
					obj:SetActive( false );
				end
				
				local effect = v:Find("Effect").gameObject;
				if effect then
					effect:SetActive( false );
				end
			end
		end
	end
	
	-- 根据数量进行显示
	for i=21, 24, 1 do
		if City.m_Buildings_res[i] then
			for k, v in pairs( City.m_Buildings_res[i] ) do
				local obj = v:Find("LevyMod").gameObject;
				local drawingObj = v:Find("ResDrawingMod").gameObject;
				if drawingObj == nil or drawingObj.activeSelf == false then
					if obj then
						
						if res_goldnum[i] > 0 then
							obj:SetActive( true );
							res_goldnum[i] = res_goldnum[i] - 1
							SetTrue( obj.transform:Find("Back/Gold") )
							SetFalse( obj.transform:Find("Back/Green") )
							SetTrue( v.transform:Find("Effect") )
							
						elseif res_greennum[i] > 0 then
							obj:SetActive( true );
							res_greennum[i] = res_greennum[i] - 1
							SetFalse( obj.transform:Find("Back/Gold") )
							SetTrue( obj.transform:Find("Back/Green") )
							SetFalse( v.transform:Find("Effect") )
						else
							obj:SetActive( false );
						end
						
					end
				end
			end
		end
	end
end

function Player:GetBuilding( kind, offset )
	if kind >= BUILDING_Silver and kind <= BUILDING_Iron then
		if self.m_buildings_res[kind] == nil then
			return nil
		end
		return self.m_buildings_res[kind][offset];
	else
		return self.m_buildings[kind];
	end 
end

function Player:BuildingLevel( kind, offset )
	local pBuilding = self:GetBuilding( kind, offset );
	if pBuilding == nil then
		return 0;
	end
	return pBuilding.m_level;
end

function Player:BuildingOverValue( kind )
	local pBuilding = self:GetBuilding( kind, -1 );
	if pBuilding == nil then
		return 0;
	end
	return pBuilding.m_overvalue;
end

-- 找一个等级最低的资源田
function Player:BuildingResMinLevel( kind )
	local minlevel = 99;
	local offset = -1;
	if self.m_buildings_res[kind] == nil then
		return
	end
	for k, v in pairs( self.m_buildings_res[kind] ) do
		if v.m_level < minlevel then
			minlevel = v.m_level;
			offset = k;
		end
	end
	return offset;
end

function Player:CityLevel()
	return self.m_buildings[1].m_level;
end

-- 黑名单
function Player:SetBlacklist( recvValue )
	for i=1, recvValue.m_count do
		local actorid = recvValue.m_actorid[i]
		self.m_blacklist[actorid] = actorid
	end
end
function Player:AddBlacklist( actorid )
	if self.m_blacklist == nil then
		return
	end
	self.m_blacklist[actorid] = actorid
end
function Player:DelBlacklist( actorid )
	if self.m_blacklist == nil then
		return
	end
	if self.m_blacklist[actorid] == nil then
		return
	end
	self.m_blacklist[actorid] = nil
end
function Player:CheckBlacklist( actorid )
	if self.m_blacklist == nil then
		return false
	end
	if self.m_blacklist[actorid] == nil then
		return false
	end
	return true
end

-- 服务器发过来的时间戳
function Player:SetServerTime( servertime )
	self.m_servertime = servertime;
	self.m_clienttime = os.time();
end

-- 服务器时间戳，游戏所有时间均以服务器时间戳为准
-- 当前时间-客户端收到服务器时间戳的时间为=流逝时间
-- 流逝时间+服务器时间=当前时间
function GetServerTime()
	return GetPlayer().m_servertime + (os.time()-GetPlayer().m_clienttime);
end

-- 全局
G_Player = nil;
function GetPlayer()
	if G_Player == nil then
		G_Player = Player.new();
	end
	return G_Player;
end
