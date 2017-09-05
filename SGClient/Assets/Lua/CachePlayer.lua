-- 一次性变量
ACTOR_SFLAG_BODY_FREEBUY	=	0	-- 首次购买体力免费

--  角色缓存信息
local Player = class("Player");
function Player:ctor()
	self:Init();
end
function Player:Init()
	self.m_usertype			=	0;	-- 用户类型
	self.m_createtime		=	0;	-- 创建时间
	self.m_config		    =   {};  --配置信息
	
	self.m_actorid			=	0;	-- 角色编号
	self.m_name				=	"";	-- 角色名称
	self.m_nation			=	0;
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
	self.m_ptsec			=	0;
	self.m_forgingkind		=	0;
	self.m_forgingsec		=	0;
	self.m_equipext			=	0;
	self.m_techlevel		=   {};
	self.m_techprogress		=	{};
	self.m_officialhire		=	{};
	
	self.m_function			=	0;
	self.m_levynum			=	0;
	self.m_worker_kind		=	0;
	self.m_worker_offset	=	0;
	self.m_worker_sec		=	0;
	self.m_worker_needsec   =	0;
	self.m_worker_free   	=	0;
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
	self.m_attr 			= 	{}
end

-- 属性变化
function Player:Set( recvValue )
	self.m_actorid			=	recvValue.m_actorid;
	self.m_name				=	recvValue.m_name;
	self.m_nation			=	recvValue.m_nation;
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
	self.m_ptsec			=	recvValue.m_ptsec;
	self.m_forgingkind		=	recvValue.m_forgingkind;
	self.m_forgingsec		=	recvValue.m_forgingsec;
	for kind = 1, 40, 1 do
		self.m_techlevel[kind-1]	=   recvValue.m_techlevel[kind];
		self.m_techprogress[kind-1]	=	recvValue.m_techprogress[kind];
	end
	for i = 1, 3, 1 do
		self.m_officialhire[i-1] = recvValue.m_officialhire[i]
	end
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
	self.m_worker_kind		=	recvValue.m_worker_kind;
	self.m_worker_offset	=	recvValue.m_worker_offset;
	self.m_worker_sec		=	recvValue.m_worker_sec;
	self.m_worker_needsec   =	recvValue.m_worker_needsec;
	self.m_worker_free   	=	recvValue.m_worker_free;
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
	
	--
	if old > levynum then
		local changenum = old - levynum;
		for i=1, changenum, 1 do
			City.BuildingSubLevy();
		end
	else
		local changenum = levynum - old;
		for i=1, changenum, 1 do
			City.BuildingAddLevy();
		end
	end
end

function Player:GetBuilding( kind, offset )
	if kind >= BUILDING_Silver and kind <= BUILDING_Iron then
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

-- 全局
G_Player = nil;
function GetPlayer()
	if G_Player == nil then
		G_Player = Player.new();
	end
	return G_Player;
end
