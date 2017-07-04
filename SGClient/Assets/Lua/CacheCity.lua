City = {};
City.m_CameraMain = nil;
City.m_Camera = nil;
City.m_LastSelect = nil;
City.m_BuildingRoot = nil
City.m_BuildingUI = nil
City.m_BuildingTimerMod = nil;
City.m_Buildings = {};
City.m_Buildings_res = {};

-- 初始化
function City.Init()
	City.m_Camera = GameManager.MainCity.transform:Find("CityCamera"):GetComponent("CityCamera");
	City.m_CameraMain = GameManager.MainCity.transform:Find("CityCamera"):GetComponent("Camera");
	City.m_BuildingUI = GameManager.MainCity.transform:Find( "BuildingUI" );
	City.BuildingTimerMod = GameManager.MainCity.transform:Find( "BuildingUI/BuildingTimerMod" );
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
		City.m_Camera:TweenPosToInBound( transform.position, 0.2 );
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
		City.m_Camera:TweenPosToInBound( transform.position, 0.2 );
	end
end

function City.BuildingUnSelect()
	-- 关闭之前渐变动画
	City.m_LastSelect:GetComponent("UITweenColor"):Kill(true);
	City.m_LastSelect = nil;
end

function City.BuildingAdd( info, active )
	local kind = info.m_kind;
	local offset = info.m_offset
	local landname = "";
	if kind >= BUILDING_Silver and kind <= BUILDING_Iron then
		landname = BuildingPrefab[kind].land..offset;
	else
		landname = BuildingPrefab[kind].land;
	end
	
	local land = City.BuildingRoot().transform:Find( landname );
	local prefab = LoadPrefab( BuildingPrefab[kind].prefab );
	local unitObj = GameObject.Instantiate( prefab ).transform;
	unitObj:SetParent( City.BuildingRoot().transform );
	unitObj.localScale = Vector3.one;
	unitObj.position = land.transform.position;
	unitObj.localPosition = Vector3.New( unitObj.localPosition.x, unitObj.localPosition.y, 0 );
	
	if active ~= nil and active == false then
		unitObj.gameObject:SetActive(false);
	else
		unitObj.gameObject:SetActive(true);
	end
	
	land.gameObject:SetActive(false);
	
	if kind >= BUILDING_Silver and kind <= BUILDING_Iron then
		if City.m_Buildings_res[kind] == nil then
			City.m_Buildings_res[kind] = {};
		end
		City.m_Buildings_res[kind][offset] = unitObj;	
	else
		City.m_Buildings[kind] = unitObj;
	end
	City.BuildingSetName( info );
	City.BuildingSetTimer( info );
	return unitObj;
end

function City.BuildingDel( info )
	local kind = info.m_kind;
	local offset = info.m_offset;	
	local unitObj = nil;
	local landname = "";
	if kind >= BUILDING_Silver and kind <= BUILDING_Iron then
		landname = BuildingPrefab[kind].land..offset;
		unitObj = City.m_Buildings_res[kind][offset];
	else
		landname = BuildingPrefab[kind].land;
		unitObj = City.m_Buildings[kind];
	end
	
	GameObject.Destroy( unitObj );
	
	local land = City.BuildingRoot().transform.Find( landname );
	land:SetActive(true);
	
	if kind >= BUILDING_Silver and kind <= BUILDING_Iron then
		City.m_Buildings_res[kind][offset] = nil;	
	else	
		City.m_Buildings[kind] = nil;		
	end
end

function City.BuildingRefurbish( info ) 
	City.BuildingSetName( info );
	City.BuildingSetTimer( info );
end

function City.BuildingSetName( info )
	local kind = info.m_kind;
	local offset = info.m_offset;
	if kind >= BUILDING_Silver and kind <= BUILDING_Iron then
		unitObj = City.m_Buildings_res[kind][offset];
	else
		unitObj = City.m_Buildings[kind];
	end
	if kind <= BUILDING_Militiaman_Archer then
		unitObj:Find("panel/name"):GetComponent( typeof(UIText) ).text = T(kind).." Lv."..info.m_level;
	elseif kind <= BUILDING_Iron then
		unitObj:Find("panel/name"):GetComponent( typeof(UIText) ).text = "Lv."..info.m_level;
	else
		unitObj:Find("panel/name"):GetComponent( typeof(UIText) ).text = T(kind)
	end
end

function City.BuildingSetTimer( info )
	local kind = info.m_kind;
	local offset = info.m_offset;
	if info.m_sec == nil or info.m_sec <= 0 then
		return;
	end
	local unitObj = nil;
	if kind >= BUILDING_Silver and kind <= BUILDING_Iron then
		unitObj = City.m_Buildings_res[kind][offset];
	else
		unitObj = City.m_Buildings[kind];
	end
	local timerObj = unitObj:GetComponent("CityBuilding").BuildingTimerMod;
	if timerObj == nil then
		timerObj = GameObject.Instantiate( City.BuildingTimerMod );
		timerObj.transform:SetParent( City.m_BuildingUI );
		timerObj.transform.position = unitObj.transform.position;
		timerObj.transform.localPosition = Vector3.New( timerObj.transform.localPosition.x, timerObj.transform.localPosition.y-80, 0 );
		timerObj.transform.localScale = Vector3.one;
		unitObj:GetComponent("CityBuilding").BuildingTimerMod = timerObj;
	end
	if info.m_sec <= 0 then
		timerObj.gameObject:SetActive(false);
	else
		timerObj.gameObject:SetActive(true);
	end

	--timerObj.transform:Find( "Icon" ):GetComponent( "Image" ).sprite;
	local timer = timerObj.transform:Find( "Text" ):GetComponent( "UITextTimeCountdown" );
	timer.controlID = 1;
	timer.uiMod = timerObj.transform:GetComponent("UIMod");
	timer.uiProgress = timerObj.transform:Find( "Progress" ):GetComponent( "UIProgress" );
    timer:SetTime( info.m_needsec, info.m_needsec-info.m_sec );
end

function City.BuildingSetUpgradeing( kind, offset, needsec, sec )
	local unitObj = nil;
	if kind >= BUILDING_Silver and kind <= BUILDING_Iron then
		unitObj = City.m_Buildings_res[kind][offset];
	else
		unitObj = City.m_Buildings[kind];
	end
	
	local timerObj = unitObj:GetComponent("CityBuilding").BuildingTimerMod;
	if timerObj == nil then
		timerObj = GameObject.Instantiate( City.BuildingTimerMod );
		timerObj.transform:SetParent( City.m_BuildingUI );
		timerObj.transform.position = unitObj.transform.position;
		timerObj.transform.localPosition = Vector3.New( timerObj.transform.localPosition.x, timerObj.transform.localPosition.y-80, 0 );
		timerObj.transform.localScale = Vector3.one;
		unitObj:GetComponent("CityBuilding").BuildingTimerMod = timerObj;
	end
	if sec <= 0 then
		timerObj.gameObject:SetActive(false);
	else
		timerObj.gameObject:SetActive(true);
	end
	--timerObj.transform:Find( "Icon" ):GetComponent( "Image" ).sprite;
	local timer = timerObj.transform:Find( "Text" ):GetComponent( "UITextTimeCountdown" );
	timer.controlID = 1;
	timer.uiMod = timerObj.transform:GetComponent("UIMod");
	timer.uiProgress = timerObj.transform:Find( "Progress" ):GetComponent( "UIProgress" );
    timer:SetTime( needsec, needsec-sec );
end

function City.BuildingWorker()
	if GetPlayer().m_worker_kind > 0 then
		City.BuildingSetUpgradeing( 
		GetPlayer().m_worker_kind, 
		GetPlayer().m_worker_offset, 
		GetPlayer().m_worker_needsec,
		GetPlayer().m_worker_sec );
	end
	if GetPlayer().m_worker_kind_ex > 0 then
		City.BuildingSetUpgradeing( 
		GetPlayer().m_worker_kind_ex, 
		GetPlayer().m_worker_offset_ex, 
		GetPlayer().m_worker_needsec_ex,
		GetPlayer().m_worker_sec_ex );
	end
end

function City.BuildingAddLevy()	
	for i=21, 24, 1 do
		if City.m_Buildings_res[i] then
			for k, v in pairs( City.m_Buildings_res[i] ) do
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
	for i=21, 24, 1 do
		if City.m_Buildings_res[i] then
			for k, v in pairs( City.m_Buildings_res[i] ) do
				local obj = v:Find("LevyMod").gameObject;
				if obj and obj.activeSelf == true then
					obj:SetActive( false );
					break;
				end
			end
		end
	end
end
