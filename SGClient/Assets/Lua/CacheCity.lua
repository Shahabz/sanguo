City = {};
City.m_Camera = nil;
City.m_LastSelect = nil;
City.m_BuildingRoot = nil
City.m_Buildings = {};
City.m_Buildings_res = {};

-- 初始化
function City.Init()
end

function City.BuildingRoot()
	if City.m_BuildingRoot == nil then
		City.m_BuildingRoot = GameManager.MainCity.transform:Find( "Content/Buildings" );
	end
	return City.m_BuildingRoot;
end

-- 选择建筑
function City.BuildingSelect( transform )
	if City.m_LastSelect ~= nil then
		-- 点击相同对象直接返回
		if City.m_LastSelect == transform then
			return;
		end
		-- 关闭之前渐变动画
		City.m_LastSelect:GetComponent("UITweenColor"):Kill(true);
		City.m_LastSelect = nil;
	end
	
	local building = nil;	
	if transform == nil then
		BuildingOpratorModShow( false, 0, -1, nil );
		return;
	else

		City.m_LastSelect = transform;
		City.m_LastSelect:GetComponent("UITweenColor"):Play(true);
		building = transform:GetComponent("CityBuilding");
		BuildingOpratorModShow( true, building.kind, building.offset, transform );
	end
	
end

function City.BuildingLandSelect( transform )
	if City.m_LastSelect ~= nil then
		-- 点击相同对象直接返回
		if City.m_LastSelect == transform then
			return;
		end
		-- 关闭之前渐变动画
		City.m_LastSelect:GetComponent("UITweenColor"):Kill(true);
		City.m_LastSelect = nil;
	end
	
	local buildingLand = nil;	
	if transform == nil then
		return;
	else
		buildingLand = transform:GetComponent("CityLand");
		BuildingCreateDlgShow( buildingLand.buildingkinds, buildingLand.offset );
	end
end

function City.BuildingUnSelect()
	-- 关闭之前渐变动画
	City.m_LastSelect:GetComponent("UITweenColor"):Kill(true);
	City.m_LastSelect = nil;
end

function City.BuildingAdd( info )
	local kind = info.m_kind;
	local offset = info.m_offset
	local landname = "";
	if kind >= BUILDING_Silver and kind <= BUILDING_Iron then
		landname = BuildingPrefab[kind].land..offset;
	else
		landname = BuildingPrefab[kind].land;
	end
	
	local root = City.BuildingRoot().transform.Find( landname );
	local prefab = LoadPrefab( BuildingPrefab[kind].prefab );
	local unitObj = GameObject.Instantiate( prefab ).transform;
	unitObj:SetParent( root );
	unitObj.localScale = Vector3.one;
	unitObj:SetActive(true);
	
	if kind >= BUILDING_Silver and kind <= BUILDING_Iron then
		City.m_Buildings_res[kind][offset] = unitObj;
		
		
	else
		City.m_Buildings[kind] = unitObj;
	end
	
	GetPlayer()m_levynum			=	0;
	self.m_worker_kind		=	0;
	self.m_worker_offset	=	0;
	self.m_worker_sec		=	0;
	self.m_worker_kind_ex	=	0;
	self.m_worker_offset_ex	=	0;
	self.m_worker_sec_ex	=	0;
	self.m_worker_expire_ex	=	0; 
end

function City.BuildingAddSec( kind, offset, sec, sec_max )	
	local unitObj = nil;
	if kind >= BUILDING_Silver and kind <= BUILDING_Iron then
		unitObj = City.m_Buildings_res[kind][offset];
	else
		unitObj = City.m_Buildings[kind];
	end
	
	local trans = GameManager.MainCity.transform:Find( "BuildingUI/BuildingUpgrade" ) 
	local trans = GameObject.Instantiate( trans ).transform;
    trans:SetParent( unitObj );
    trans.localScale = Vector3.one;
	trans:SetActive(true);
end

function City.BuildingAddLevy()	
	for i=1, 4, 1 do
		if City.m_Buildings_res[i] then
			for k, v in ipairs( City.m_Buildings_res[1] ) do
				local obj = v:Find("LevyMod").gameObject;
				if obj and obj.activeSelf == false then
					obj:SetActive( true );
					break;
				end
			end
		end
	end
end

function City.BuildingSubLevy()
	for i=1, 4, 1 do
		if City.m_Buildings_res[i] then
			for k, v in ipairs( City.m_Buildings_res[1] ) do
				local obj = v:Find("LevyMod").gameObject;
				if obj and obj.activeSelf == true then
					obj:SetActive( false );
					break;
				end
			end
		end
	end
end

function City.BuildingRefurbish( kind, offset )
	-- 
	
end