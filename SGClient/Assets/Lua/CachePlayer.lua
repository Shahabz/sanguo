-- 涓�娆℃�у彉閲�
ACTOR_SFLAG_BODY_FREEBUY	=	0	-- 棣栨璐拱浣撳姏鍏嶈垂
ACTOR_SFLAG_MAPZONE_GO_ZC	=	1	-- 棣栨鍏嶈垂鍓嶅線宸炲煄
ACTOR_SFLAG_QUEST_AUTOBUILD	=	2	-- 鏂版墜浠诲姟鏄惁棰嗗彇杩囪嚜鍔ㄥ缓閫�
ACTOR_SFLAG_SKIPFIGHT		=	3	-- 鏄惁鍙互璺宠繃鎴樻枟
ACTOR_SFLAG_PALACE			=	4	-- 鏄惁寮�鍚殗瀹唴闄�
ACTOR_SFLAG_STORYSWEEP		=	5	-- 鏄惁寮�鍚壂鑽″壇鏈�
ACTOR_SFLAG_OFFICIAL_TECH	=	6   -- 鏄惁寮�鍚传鑹茬爺绌跺憳
ACTOR_SFLAG_EQUPIPDRAWING	=	7	-- 鏄惁寮�鍚澶囧垎瑙ｈ幏寰楀浘绾�
ACTOR_SFLAG_MATERIAL_MAKEWILL = 8   -- 鏄惁寮�鍚綔鍧婇璁�

CITY_BUFF_MAX		=	6 -- buff鏁伴噺
CITY_BUFF_MARCH		=	0 -- 鍐涙浌绠uff 琛屽啗鑰楁椂闄嶄綆15%
CITY_BUFF_TRAIN		=	1 -- 姝﹀崚绠uff 鎷涘嫙鑰楁椂闄嶄綆15%
CITY_BUFF_WIND		=	2 -- 椋庡瓧浠uff 琛屽啗鑰楁椂闄嶄綆15%
CITY_BUFF_FIRE		=	3 -- 鐏瓧浠uff 澹叺鏀诲嚮+5%
CITY_BUFF_MOUNTAIN	=	4 -- 灞卞瓧浠uff 澹叺闃插尽+5%
CITY_BUFF_FOREST	=	5 -- 鏋楀瓧浠uff 鍦ㄥ叺钀ヤ腑棰濆澧炲姞20%浼ょ梾鎭㈠

--  瑙掕壊缂撳瓨淇℃伅
local Player = class("Player");
function Player:ctor()
	self:Init();
end
function Player:Init()
	self.m_usertype			=	0;	-- 鐢ㄦ埛绫诲瀷
	self.m_createtime		=	0;	-- 鍒涘缓鏃堕棿
	self.m_config		    =   {};  --閰嶇疆淇℃伅
	self.m_servertime 		= 	0;
	self.m_clienttime 		= 	0
	
	self.m_actorid			=	0;	-- 瑙掕壊缂栧彿
	self.m_name				=	"";	-- 瑙掕壊鍚嶇О
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
	
	self.m_view_zoneid		=	0;
	self.m_award_mailid		=	int64.new(0);
end

-- 灞炴�у彉鍖�
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
	
	-- 瀹屾垚鏁板�肩姸鎬�
	if info.m_overvalue > 0 then
		City.BuildingSetOver( kind );
	end
	
	-- 鍔犻�熸爣璁�
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
	--[[寰佹敹娆℃暟=12鏃讹紝鎵�鏈夌殑璧勬簮鐢帮紙100%锛変笂鏂归兘鎮诞涓�涓噾鑹叉皵娉�
寰佹敹娆℃暟=11鏃讹紝鎵�鏈夌殑璧勬簮鐢帮紙90%锛夋樉绀轰负閲戣壊姘旀场锛�10%鏄剧ず缁胯壊姘旀场
寰佹敹娆℃暟=10鏃讹紝鎵�鏈夌殑璧勬簮鐢帮紙80%锛夋樉绀轰负閲戣壊姘旀场锛�20%鏄剧ず缁胯壊姘旀场
寰佹敹娆℃暟=9鏃讹紝鎵�鏈夌殑璧勬簮鐢帮紙70%锛夋樉绀轰负閲戣壊姘旀场锛�30%鏄剧ず缁胯壊姘旀场
寰佹敹娆℃暟=8鏃讹紝鎵�鏈夌殑璧勬簮鐢帮紙60%锛夋樉绀轰负閲戣壊姘旀场锛�40%鏄剧ず缁胯壊姘旀场
寰佹敹娆℃暟=7鏃讹紝鎵�鏈夌殑璧勬簮鐢帮紙50%锛夋樉绀轰负閲戣壊姘旀场锛�50%鏄剧ず缁胯壊姘旀场
寰佹敹娆℃暟=6鏃讹紝鎵�鏈夌殑璧勬簮鐢帮紙40%锛夋樉绀轰负閲戣壊姘旀场锛�60%鏄剧ず缁胯壊姘旀场
寰佹敹娆℃暟=5鏃讹紝鎵�鏈夌殑璧勬簮鐢帮紙0%锛夋樉绀轰负閲戣壊姘旀场锛�40%鏄剧ず缁胯壊姘旀场
寰佹敹娆℃暟=4鏃讹紝姣忕璧勬簮鐢板悇鏈�4涓豢鑹叉场娉�
寰佹敹娆℃暟=3鏃讹紝姣忕璧勬簮鐢板悇鏈�3涓豢鑹叉场娉�
寰佹敹娆℃暟=2鏃讹紝姣忕璧勬簮鐢板悇鏈�2涓豢鑹叉场娉�
寰佹敹娆℃暟=1鏃讹紝姣忕璧勬簮鐢板悇鏈�1涓豢鑹叉场娉�--]]
	
	-- 鑾峰彇姣忕寤虹瓚鏁伴噺
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
	
	-- 璁＄畻姣忎竴绉嶈祫婧愰噾鑹叉暟閲�
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
	
	-- 璁＄畻姣忎竴绉嶈祫婧愮豢鑹叉暟閲�
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
		
	-- 鍏堝叏閮ㄩ殣钘�
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
	
	-- 鏍规嵁鏁伴噺杩涜鏄剧ず
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

-- 鎵句竴涓瓑绾ф渶浣庣殑璧勬簮鐢�
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

-- 榛戝悕鍗�
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

-- 鏈嶅姟鍣ㄥ彂杩囨潵鐨勬椂闂存埑
function Player:SetServerTime( servertime )
	self.m_servertime = servertime;
	self.m_clienttime = os.time();
end

-- 鏈嶅姟鍣ㄦ椂闂存埑锛屾父鎴忔墍鏈夋椂闂村潎浠ユ湇鍔″櫒鏃堕棿鎴充负鍑�
-- 褰撳墠鏃堕棿-瀹㈡埛绔敹鍒版湇鍔″櫒鏃堕棿鎴崇殑鏃堕棿涓�=娴侀�濇椂闂�
-- 娴侀�濇椂闂�+鏈嶅姟鍣ㄦ椂闂�=褰撳墠鏃堕棿
function GetServerTime()
	return GetPlayer().m_servertime + (os.time()-GetPlayer().m_clienttime);
end

-- 鍏ㄥ眬
G_Player = nil;
function GetPlayer()
	if G_Player == nil then
		G_Player = Player.new();
	end
	return G_Player;
end
