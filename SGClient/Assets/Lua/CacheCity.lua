City = {};
City.m_CameraMain = nil;
City.m_Camera = nil;
City.m_LastSelect = nil;
City.m_BuildingRoot = nil
City.m_BuildingUI = nil
City.m_BuildingTimerMod = nil;
City.m_BuildingFreeMod = nil;
City.m_BuildingOverMod = nil;
City.m_BuildingQuestMod = nil;
City.m_Buildings = {};
City.m_Buildings_res = {};

-- 初始化
function City.Init()
	City.m_Camera = GameManager.MainCity.transform:Find("CityCamera"):GetComponent("CityCamera");
	City.m_CameraMain = GameManager.MainCity.transform:Find("CityCamera"):GetComponent("Camera");
	City.m_BuildingUI = GameManager.MainCity.transform:Find( "BuildingUI" );
	City.m_BuildingTimerMod = GameManager.MainCity.transform:Find( "BuildingUI/BuildingTimerMod" );
	City.m_BuildingFreeMod = GameManager.MainCity.transform:Find( "BuildingUI/BuildingFreeMod" );
	City.m_BuildingOverMod = GameManager.MainCity.transform:Find( "BuildingUI/BuildingOverMod" );
	City.m_BuildingQuestMod = GameManager.MainCity.transform:Find( "BuildingUI/BuildingQuestMod" );
end

-- 所有建筑父节点
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
		--if City.m_LastSelect == transform then
			--return;
		--end
		-- 关闭之前渐变动画
		--City.m_LastSelect:GetComponent("UITweenColor"):Kill(true);
		City.m_LastSelect = nil;
	end
	
	local building = nil;	
	if transform == nil then
		BuildingOpratorModShow( false, 0, -1, nil );
		return;
	else

		City.m_LastSelect = transform;
		--City.m_LastSelect:GetComponent("UITweenColor"):Play(true);
		building = transform:GetComponent("CityBuilding");
		City.m_Camera:TweenPosToInBound( transform.position, 0.2 );
	end
	
	-- 打开加速界面	
	if GetPlayer().m_worker_kind == building.kind and GetPlayer().m_worker_offset == building.offset or 
		GetPlayer().m_worker_kind_ex == building.kind and GetPlayer().m_worker_offset_ex == building.offset then
		if GetPlayer().m_worker_kind == building.kind then
			QuickItemDlgShow( 1, building.kind, building.offset, GetPlayer().m_worker_sec );
		else
			QuickItemDlgShow( 1, building.kind, building.offset, GetPlayer().m_worker_sec_ex );
		end
	else
		BuildingOpratorModShow( false, 0, -1, nil );
		if building.kind == BUILDING_Smithy then -- 铁匠铺
			-- 打造有完成的，直接领取
			if GetPlayer():BuildingOverValue( building.kind ) > 0 then
				City.BuildingHideOver( building.kind )
				system_askinfo( ASKINFO_EQUIPFORGING, "", 4 );
			else
				EquipForgingDlgShow();
			end
		elseif building.kind == BUILDING_Wash then -- 洗炼铺
			EquipWashDlgShow();
		elseif building.kind == BUILDING_Fangshi then -- 坊市
			
		elseif building.kind == BUILDING_Shop then -- 商店
			
		elseif building.kind == BUILDING_Hero then -- 聚贤馆
			HeroListDlgShow();
			
		elseif building.kind == BUILDING_Wishing then -- 聚宝盆

		elseif building.kind == BUILDING_Help then -- 帮助
		
		else
			-- 科技有完成的，直接领取
			if building.kind == BUILDING_Tech and GetPlayer():BuildingOverValue( building.kind ) > 0 then
				City.BuildingHideOver( building.kind )
				system_askinfo( ASKINFO_TECH, "", 4 );
			
			-- 材料工坊有完成的，直接领取
			elseif building.kind == BUILDING_Craftsman and GetPlayer():BuildingOverValue( building.kind ) > 0 then
				system_askinfo( ASKINFO_MATERIALMAKE, "", 4 );
				
 			-- 募兵有完成的，直接领取
			elseif building.kind >= BUILDING_Infantry and building.kind <= BUILDING_Militiaman_Archer and GetPlayer():BuildingOverValue( building.kind ) > 0 then
				City.BuildingHideOver( building.kind )
				system_askinfo( ASKINFO_TRAIN, "", 4, building.kind );
						
			else
				BuildingOpratorModShow( true, building.kind, building.offset, transform );
			end
		end
	end
end

-- 点击空地块
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

-- 取消选择
function City.BuildingUnSelect()
	-- 关闭之前渐变动画
	if City.m_LastSelect ~= nil then
		City.m_LastSelect:GetComponent("UITweenColor"):Kill(true);
		City.m_LastSelect = nil;
	end
end

-- 获取建筑对象
function City.GetBuilding( kind, offset )
	if kind >= BUILDING_Silver and kind <= BUILDING_Iron then
		return City.m_Buildings_res[kind][offset];
	else
		return City.m_Buildings[kind];
	end
end

-- 移到制定建筑
function City.Move( kind, offset, select )
	local unitObj = City.GetBuilding( kind, offset )
	if unitObj == nil then
		return;
	end
	City.m_Camera:TweenPosToInBound( unitObj.position, 0.2 );
	if select == true then
		Invoke( function() 
			City.BuildingSelect( unitObj )
		end, 0.3 );
	end
end

-- 添加建筑
function City.BuildingAdd( info, active )
	local kind = info.m_kind;
	local offset = info.m_offset
	local landname = "";
	
	-- 有了就修改
	local unitObj = nil;
	if kind >= BUILDING_Silver and kind <= BUILDING_Iron then
		if City.m_Buildings_res[kind] ~= nil then
			unitObj = City.m_Buildings_res[kind][offset];
		end
	else
		unitObj = City.m_Buildings[kind];
	end
	
	-- 没有就创建
	if unitObj == nil then
		if kind >= BUILDING_Silver and kind <= BUILDING_Iron then
			landname = BuildingPrefab[kind].land..offset;
		else
			landname = BuildingPrefab[kind].land;
		end
		
		local land = City.BuildingRoot().transform:Find( landname );
		local prefab = LoadPrefab( BuildingPrefab[kind].prefab );
		unitObj = GameObject.Instantiate( prefab ).transform;
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
		unitObj:GetComponent("CityBuilding").offset = offset;
	end
	
	City.BuildingSetName( info );
	City.BuildingSetTimer( info );
	City.BuildingHideFree( kind, offset )
	return unitObj;
end

-- 删除建筑
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

-- 刷新
function City.BuildingRefurbish( info ) 
	City.BuildingSetName( info );
	City.BuildingSetTimer( info );
end

-- 建筑名称
function City.BuildingSetName( info )
	local kind = info.m_kind;
	local offset = info.m_offset;
	if kind >= BUILDING_Silver and kind <= BUILDING_Iron then
		unitObj = City.m_Buildings_res[kind][offset];
	else
		unitObj = City.m_Buildings[kind];
	end

	if kind <= BUILDING_Militiaman_Archer then
		unitObj:Find("panel/name"):GetComponent( typeof(UIText) ).text = T(kind).." "..info.m_level;
	elseif kind <= BUILDING_Iron then
		unitObj:Find("panel/name"):GetComponent( typeof(UIText) ).text = "Lv."..info.m_level;
	else
		unitObj:Find("panel/name"):GetComponent( typeof(UIText) ).text = T(kind)
	end
end

-- 操作计时器
function City.BuildingSetTimer( info )
	local kind = info.m_kind;
	local offset = info.m_offset;
	if info.m_sec == nil then
		return;
	end
	local unitObj = nil;
	if kind >= BUILDING_Silver and kind <= BUILDING_Iron then
		unitObj = City.m_Buildings_res[kind][offset];
	else
		unitObj = City.m_Buildings[kind];
	end
	local timerObj = unitObj:GetComponent("CityBuilding").BuildingTimerMod;
	if timerObj == nil and info.m_sec > 0 then
		timerObj = GameObject.Instantiate( City.m_BuildingTimerMod );
		timerObj.transform:SetParent( City.m_BuildingUI );
		timerObj.transform.position = unitObj.transform.position;
		timerObj.transform.localPosition = Vector3.New( timerObj.transform.localPosition.x, timerObj.transform.localPosition.y-50, 0 );
		timerObj.transform.localScale = Vector3.one;
		unitObj:GetComponent("CityBuilding").BuildingTimerMod = timerObj;
	end
	
	if timerObj ~= nil then
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
end

-- 升级计时器
function City.BuildingSetUpgradeing( kind, offset, needsec, sec )
	local unitObj = nil;
	if kind >= BUILDING_Silver and kind <= BUILDING_Iron then
		unitObj = City.m_Buildings_res[kind][offset];
	else
		unitObj = City.m_Buildings[kind];
	end
	
	local timerObj = unitObj:GetComponent("CityBuilding").BuildingTimerMod;
	if timerObj == nil then
		timerObj = GameObject.Instantiate( City.m_BuildingTimerMod );
		timerObj.transform:SetParent( City.m_BuildingUI );
		timerObj.transform.position = unitObj.transform.position;
		timerObj.transform.localPosition = Vector3.New( timerObj.transform.localPosition.x, timerObj.transform.localPosition.y-50, 0 );
		timerObj.transform.localScale = Vector3.one;
		unitObj:GetComponent("CityBuilding").BuildingTimerMod = timerObj;
	end
	if sec <= 0 then
		timerObj.gameObject:SetActive(false);
		--timerObj.transform:Find( "Icon" ):GetComponent( "Image" ).sprite;
		local timer = timerObj.transform:Find( "Text" ):GetComponent( "UITextTimeCountdown" );
		timer.controlID = 0;
		timer.uiMod = timerObj.transform:GetComponent("UIMod");
		timer.uiProgress = timerObj.transform:Find( "Progress" ):GetComponent( "UIProgress" );
		timer:SetTime( 0, 0 );
	else
		timerObj.gameObject:SetActive(true);
		--timerObj.transform:Find( "Icon" ):GetComponent( "Image" ).sprite;
		local timer = timerObj.transform:Find( "Text" ):GetComponent( "UITextTimeCountdown" );
		timer.controlID = 1;
		timer.uiMod = timerObj.transform:GetComponent("UIMod");
		timer.uiProgress = timerObj.transform:Find( "Progress" ):GetComponent( "UIProgress" );
		timer:SetTime( needsec, needsec-sec );
	end
end

-- 建造队列
function City.BuildingWorker()
	if GetPlayer().m_worker_kind > 0 then
		City.BuildingSetUpgradeing( 
		GetPlayer().m_worker_kind, 
		GetPlayer().m_worker_offset, 
		GetPlayer().m_worker_needsec,
		GetPlayer().m_worker_sec );
		
		if GetPlayer().m_worker_free > 0 then
			City.BuildingSetFree( GetPlayer().m_worker_kind, GetPlayer().m_worker_offset );
		end	
	end
	
	if GetPlayer().m_worker_kind_ex > 0 then
		City.BuildingSetUpgradeing( 
		GetPlayer().m_worker_kind_ex, 
		GetPlayer().m_worker_offset_ex, 
		GetPlayer().m_worker_needsec_ex,
		GetPlayer().m_worker_sec_ex );
		
		if GetPlayer().m_worker_free_ex > 0 then
			City.BuildingSetFree( GetPlayer().m_worker_kind_ex, GetPlayer().m_worker_offset_ex );
		end
	end
end

-- 免费头
function City.BuildingSetFree( kind, offset )
	local unitObj = nil;
	if kind >= BUILDING_Silver and kind <= BUILDING_Iron then
		unitObj = City.m_Buildings_res[kind][offset];
	else
		unitObj = City.m_Buildings[kind];
	end
	local freeObj = unitObj:GetComponent("CityBuilding").BuildingFreeMod;
	if freeObj == nil then
		freeObj = GameObject.Instantiate( City.m_BuildingFreeMod );
		freeObj.transform:SetParent( City.m_BuildingUI );
		freeObj.transform.position = unitObj.transform.position;
		freeObj.transform.localPosition = Vector3.New( freeObj.transform.localPosition.x, freeObj.transform.localPosition.y+80, 0 );
		freeObj.transform.localScale = Vector3.one;
		unitObj:GetComponent("CityBuilding").BuildingFreeMod = freeObj;
	end
	local ShareData = freeObj.transform:GetComponent("ShareData");
	ShareData.intValue[0] = kind;
	ShareData.intValue[1] = offset;
	freeObj.gameObject:SetActive(true);
end

function City.BuildingHideFree( kind, offset )
	local unitObj = nil;
	if kind >= BUILDING_Silver and kind <= BUILDING_Iron then
		unitObj = City.m_Buildings_res[kind][offset];
	else
		unitObj = City.m_Buildings[kind];
	end
	local freeObj = unitObj:GetComponent("CityBuilding").BuildingFreeMod;
	if freeObj == nil then
		return
	end
	freeObj.gameObject:SetActive(false);
end

-- 完成标记
function City.BuildingSetOver( kind )
	if kind >= BUILDING_Silver and kind <= BUILDING_Iron then
		return;
	end
	local unitObj = City.m_Buildings[kind];
	local overObj = unitObj:GetComponent("CityBuilding").BuildingOverMod;
	if overObj == nil then
		overObj = GameObject.Instantiate( City.m_BuildingOverMod );
		overObj.transform:SetParent( City.m_BuildingUI );
		overObj.transform.position = unitObj.transform.position;
		overObj.transform.localPosition = Vector3.New( overObj.transform.localPosition.x, overObj.transform.localPosition.y, 0 );
		overObj.transform.localScale = Vector3.one;
		unitObj:GetComponent("CityBuilding").BuildingOverMod = overObj;
	end
	local ShareData = overObj.transform:GetComponent("ShareData");
	ShareData.intValue[0] = kind;
	overObj.gameObject:SetActive(true);
end

-- 完成标记隐藏
function City.BuildingHideOver( kind )
	if kind >= BUILDING_Silver and kind <= BUILDING_Iron then
		return;
	end
	local unitObj = City.m_Buildings[kind];
	local overObj = unitObj:GetComponent("CityBuilding").BuildingOverMod;
	if overObj == nil then
		return
	end
	overObj.gameObject:SetActive(false);
end

-- 征收次数
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

-- 点击建造队列
function City.GoToWorker()
	-- 移动并选择
	if GetPlayer().m_worker_kind > 0 then
		City.Move( GetPlayer().m_worker_kind, GetPlayer().m_worker_offset, true )
		return;
	end
	
	-- 找到一个可以升级的
	
end

-- 点击建造队列商用
function City.GoToWorkerEx()
	-- 移动并选择
	if GetPlayer().m_worker_kind_ex > 0 then
		City.Move( GetPlayer().m_worker_kind_ex, GetPlayer().m_worker_offset_ex, true )
		return
	end
	
	-- 打开购买商用建造队界面
	if GetPlayer().m_worker_expire_ex <= 0 then
		-- 
		BuyWorkerDlgShow();
	end
	
	-- 找到一个可以升级的
	
end

-- 任务图标
function City.BuildingQuestMod( questid )
	City.m_BuildingQuestMod.gameObject:SetActive(true);
	local ShareData = City.m_BuildingQuestMod.transform:GetComponent("ShareData");
	ShareData.intValue[0] = questid;
end
