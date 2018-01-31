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
City.m_BuildingQuickMod = nil;
City.m_BuildingWorkerQuickMod = nil;
City.m_Fires = nil
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
	City.m_BuildingQuickMod = GameManager.MainCity.transform:Find( "BuildingUI/BuildingQuickMod" );
	City.m_BuildingWorkerQuickMod = GameManager.MainCity.transform:Find( "BuildingUI/BuildingWorkerQuickMod" );
	City.m_Fires = GameManager.MainCity.transform:Find( "Content/Fires" );
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
	if GetPlayer().m_worker_kind == building.kind then
		if building.kind >= BUILDING_Silver and building.kind <= BUILDING_Iron then
			if GetPlayer().m_worker_offset == building.offset then
				if GetPlayer().m_worker_op == 4 then
					QuickItemDlgShow( 3, building.kind, building.offset, GetPlayer().m_worker_sec );
				else
					QuickItemDlgShow( 1, building.kind, building.offset, GetPlayer().m_worker_sec );
				end
				return
			end
		else
			if GetPlayer().m_worker_op == 4 then
				QuickItemDlgShow( 3, building.kind, building.offset, GetPlayer().m_worker_sec );
			else
				QuickItemDlgShow( 1, building.kind, building.offset, GetPlayer().m_worker_sec );
			end
			return
		end
	end
		
	if GetPlayer().m_worker_kind_ex == building.kind then
		if building.kind >= BUILDING_Silver and building.kind <= BUILDING_Iron then
			if GetPlayer().m_worker_offset_ex == building.offset then
				if GetPlayer().m_worker_op_ex == 4 then
					QuickItemDlgShow( 3, building.kind, building.offset, GetPlayer().m_worker_sec_ex );
				else
					QuickItemDlgShow( 1, building.kind, building.offset, GetPlayer().m_worker_sec_ex );
				end
				return
			end
		else
			if GetPlayer().m_worker_op_ex == 4 then
				QuickItemDlgShow( 3, building.kind, building.offset, GetPlayer().m_worker_sec_ex );
			else
				QuickItemDlgShow( 1, building.kind, building.offset, GetPlayer().m_worker_sec_ex );
			end
			return
		end
	end
	
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
		ShopDlgShow()
		
	elseif building.kind == BUILDING_Hero then -- 聚贤馆
		HeroListDlgShow( HEROLIST_PATH_HERO_LIST );
		
	elseif building.kind == BUILDING_Wishing then -- 聚宝盆
		WishingDlgShow();
	elseif building.kind == BUILDING_Help then -- 帮助
		HelpDlgShow();
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
	
			if building.kind == BUILDING_Wood and GetPlayer().m_questid == QUEST_MAINID_MADAI then
				BuildingQuestModHide()
				QuestTalkAsk( 5 )
				return
			elseif building.kind == BUILDING_Silver and GetPlayer().m_questid == QUEST_MAINID_LIUKOU then
				BuildingQuestModHide()
				QuestTalkAsk( 14 )
				return
			end

			BuildingOpratorModShow( true, building.kind, building.offset, transform );
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
		BuildingCreateDlgShowByID( 0, buildingLand.id );
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
	-- 在城外就切回城内
	if GameManager.currentScence == "worldmap" then
		WorldMap.ReturnCity()
	end
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
		if BuildingPrefab[kind] == nil then
			return
		end
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
	City.BuildingHideWorkerQuick( kind, offset )
	
	-- 城墙-补充城防头
	if kind == BUILDING_Wall then
		City.GuardCallMod( unitObj, true )
	-- 聚贤馆-免费寻访头
	elseif kind == BUILDING_Hero then
		City.HeroVisitMod( unitObj, false, 0 )
	end
	
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
	GameObject.Destroy( unitObj.gameObject );
	

	if kind >= BUILDING_Silver and kind <= BUILDING_Iron then
		City.m_Buildings_res[kind][offset] = nil;	
	else	
		City.m_Buildings[kind] = nil;		
	end
end

-- 显示空地
function City.BuildingLandShow( kind, offset )
	local unitObj = nil;
	if kind >= BUILDING_Silver and kind <= BUILDING_Iron then
		if City.m_Buildings_res[kind] ~= nil then
			unitObj = City.m_Buildings_res[kind][offset];
		end
	else
		unitObj = City.m_Buildings[kind];
	end
	-- 没有就显示
	if unitObj == nil then
		if BuildingPrefab[kind] == nil then
			return
		end
		local landname = "";
		if kind >= BUILDING_Silver and kind <= BUILDING_Iron then
			landname = BuildingPrefab[kind].land..offset;
		else
			landname = BuildingPrefab[kind].land;
		end
		
		local land = City.BuildingRoot().transform:Find( landname );
		SetTrue( land )
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
	local unitObj = nil;
	if kind >= BUILDING_Silver and kind <= BUILDING_Iron then
		unitObj = City.m_Buildings_res[kind][offset];
	else
		unitObj = City.m_Buildings[kind];
	end

	if kind <= BUILDING_Militiaman_Archer then
		unitObj:Find("panel/name"):GetComponent( typeof(UIText) ).text = T(kind).." "..info.m_level;
	elseif kind <= BUILDING_Iron then
		if info.m_level <= 0 then
			SetSpriteGray( unitObj:Find("shape"), true )
			SetFalse( unitObj:Find("panel") )
			SetTrue( unitObj:Find("ResDrawingMod") );
		else
			if info.m_level == 1 then -- 只有1级有必要
				SetSpriteGray( unitObj:Find("shape"), false )
				SetTrue( unitObj:Find("panel") )
				SetFalse( unitObj:Find("ResDrawingMod") );
			end
			unitObj:Find("panel/name"):GetComponent( typeof(UIText) ).text = "Lv."..info.m_level;
		end
	else
		unitObj:Find("panel/name"):GetComponent( typeof(UIText) ).text = T(kind)
	end
end

-- 设置建筑形象
function City.BuildingSetShape( info, shape )
	local kind = info.m_kind;
	local offset = info.m_offset;
	local unitObj = nil;
	if kind >= BUILDING_Silver and kind <= BUILDING_Iron then
		unitObj = City.m_Buildings_res[kind][offset];
	else
		unitObj = City.m_Buildings[kind];
	end
	SetSprite( unitObj:Find("shape"), LoadSprite(shape) );
end

-- 设置建筑不启用
function City.BuildingSetGray()
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
		timerObj.transform.localPosition = Vector3.New( timerObj.transform.localPosition.x-10, timerObj.transform.localPosition.y-100, 0 );
		timerObj.transform.localScale = Vector3.one;
		unitObj:GetComponent("CityBuilding").BuildingTimerMod = timerObj;
	end
	
	if timerObj ~= nil then
		if info.m_sec <= 0 then
			timerObj.gameObject:SetActive(false);
		else
			timerObj.gameObject:SetActive(true);
		end
		
		local iconname = "";
		if kind == BUILDING_Tech then -- 太学院
			iconname = "ui_opration_state_3"
		elseif kind == BUILDING_Craftsman then -- 材料作坊
			iconname = "ui_opration_item_"..info.m_value;
		elseif kind >= BUILDING_Infantry and kind <= BUILDING_Militiaman_Archer then -- 兵营
			iconname = "ui_opration_state_2"
		elseif kind == BUILDING_Smithy then -- 铁匠铺
			iconname = "ui_opration_state_4"
		end
		timerObj.transform:Find( "Icon" ):GetComponent( "Image" ).sprite = LoadSprite( iconname );
		local timer = timerObj.transform:Find( "Text" ):GetComponent( "UITextTimeCountdown" );
		timer.controlID = 1;
		timer.uiMod = timerObj.transform:GetComponent("UIMod");
		timer.uiProgress = timerObj.transform:Find( "Progress" ):GetComponent( "UIProgress" );
		timer:SetTime( info.m_needsec, info.m_needsec-info.m_sec );
	end
end

-- 升级计时器
function City.BuildingSetUpgradeing( kind, offset, needsec, sec, op )
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
		timerObj.transform.localPosition = Vector3.New( timerObj.transform.localPosition.x-10, timerObj.transform.localPosition.y-100, 0 );
		timerObj.transform.localScale = Vector3.one;
		unitObj:GetComponent("CityBuilding").BuildingTimerMod = timerObj;
	end
	if sec <= 0 then
		unitObj.transform:Find("shape"):GetComponent("SpriteRenderer").color = Hex2Color( 0xffffffff )
		SetFalse( unitObj.transform:Find("shape_up") )
		timerObj.gameObject:SetActive(false);
		local timer = timerObj.transform:Find( "Text" ):GetComponent( "UITextTimeCountdown" );
		timer.controlID = 0;
		timer.uiMod = timerObj.transform:GetComponent("UIMod");
		timer.uiProgress = timerObj.transform:Find( "Progress" ):GetComponent( "UIProgress" );
		timer:SetTime( 0, 0 );
	else
		timerObj.gameObject:SetActive(true);
		
		local opicon = "ui_opration_state_1";
		if op == 1 then -- 建造
			SetTrue( unitObj.transform:Find("shape_up") )
			opicon = "ui_opration_3";
		elseif op == 2 then -- 升级
			SetTrue( unitObj.transform:Find("shape_up") )
			opicon = "ui_opration_state_1";
		elseif op == 4 then -- 拆除
			unitObj.transform:Find("shape"):GetComponent("SpriteRenderer").color = Hex2Color( 0xffffff7f )
			opicon = "ui_opration_3";
		end
		timerObj.transform:Find( "Icon" ):GetComponent( "Image" ).sprite = LoadSprite( opicon );
		
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
		GetPlayer().m_worker_sec,
		GetPlayer().m_worker_op );
		
		if GetPlayer().m_worker_free > 0 then
			City.BuildingSetFree( GetPlayer().m_worker_kind, GetPlayer().m_worker_offset );
		elseif GetPlayer().m_wnquick > 0 then
			City.BuildingSetWorkerQuick( GetPlayer().m_worker_kind, GetPlayer().m_worker_offset );
		end	
	end
	
	if GetPlayer().m_worker_kind_ex > 0 then
		City.BuildingSetUpgradeing( 
		GetPlayer().m_worker_kind_ex, 
		GetPlayer().m_worker_offset_ex, 
		GetPlayer().m_worker_needsec_ex,
		GetPlayer().m_worker_sec_ex,
		GetPlayer().m_worker_op_ex );
		
		if GetPlayer().m_worker_free_ex > 0 then
			City.BuildingSetFree( GetPlayer().m_worker_kind_ex, GetPlayer().m_worker_offset_ex );
		elseif GetPlayer().m_wnquick_ex > 0 then
			City.BuildingSetWorkerQuick( GetPlayer().m_worker_kind, GetPlayer().m_worker_offset );
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
		if kind >= BUILDING_Silver and kind <= BUILDING_Iron then
			freeObj.transform.localPosition = Vector3.New( freeObj.transform.localPosition.x, freeObj.transform.localPosition.y+50, 0 );
		else
			freeObj.transform.localPosition = Vector3.New( freeObj.transform.localPosition.x, freeObj.transform.localPosition.y+80, 0 );
		end
		freeObj.transform.localScale = Vector3.one;
		unitObj:GetComponent("CityBuilding").BuildingFreeMod = freeObj;
	end
	local ShareData = freeObj.transform:GetComponent("ShareData");
	ShareData.intValue[0] = kind;
	ShareData.intValue[1] = offset;
	freeObj.gameObject:SetActive(true);
	-- 显示免费头，关闭城防补充头
	City.GuardCallMod( unitObj, false );
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
	-- 关闭免费头，开启城防补充头
	City.GuardCallMod( unitObj, false );
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
		if kind == BUILDING_Craftsman then -- 材料作坊特殊处理
			local info = GetPlayer():GetBuilding( kind, -1 )
			local iconname = "ui_opration_item_"..info.m_overvalue;
			SetImage( overObj.transform:Find("Back"), LoadSprite( iconname ) );
		elseif kind >= BUILDING_Militiaman_Infantry and kind <= BUILDING_Militiaman_Archer then
			SetImage( overObj.transform:Find("Back"), LoadSprite( "ui_opration_finish_"..(kind-3) ) );
		else
			SetImage( overObj.transform:Find("Back"), LoadSprite( "ui_opration_finish_"..kind ) );
		end
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


-- 升级加速头
function City.BuildingSetWorkerQuick( kind, offset )
	local unitObj = nil;
	if kind >= BUILDING_Silver and kind <= BUILDING_Iron then
		unitObj = City.m_Buildings_res[kind][offset];
	else
		unitObj = City.m_Buildings[kind];
	end
	local freeObj = unitObj:GetComponent("CityBuilding").BuildingWorkerQuickMod;
	if freeObj == nil then
		freeObj = GameObject.Instantiate( City.m_BuildingWorkerQuickMod );
		freeObj.transform:SetParent( City.m_BuildingUI );
		freeObj.transform.position = unitObj.transform.position;
		freeObj.transform.localPosition = Vector3.New( freeObj.transform.localPosition.x, freeObj.transform.localPosition.y+80, 0 );
		freeObj.transform.localScale = Vector3.one;
		unitObj:GetComponent("CityBuilding").BuildingWorkerQuickMod = freeObj;
	end
	local ShareData = freeObj.transform:GetComponent("ShareData");
	ShareData.intValue[0] = kind;
	ShareData.intValue[1] = offset;
	freeObj.gameObject:SetActive(true);
end

function City.BuildingHideWorkerQuick( kind, offset )
	local unitObj = nil;
	if kind >= BUILDING_Silver and kind <= BUILDING_Iron then
		unitObj = City.m_Buildings_res[kind][offset];
	else
		unitObj = City.m_Buildings[kind];
	end
	local freeObj = unitObj:GetComponent("CityBuilding").BuildingWorkerQuickMod;
	if freeObj == nil then
		return
	end
	freeObj.gameObject:SetActive(false);
end

-- 加速头
function City.BuildingSetQuick( kind )
	local unitObj = nil;
	if kind >= BUILDING_Silver and kind <= BUILDING_Iron then
		return
	end
	unitObj = City.m_Buildings[kind];
	local quickObj = unitObj:GetComponent("CityBuilding").BuildingQuickMod;
	if quickObj == nil then
		quickObj = GameObject.Instantiate( City.m_BuildingQuickMod );
		quickObj.transform:SetParent( City.m_BuildingUI );
		quickObj.transform.position = unitObj.transform.position;
		quickObj.transform.localPosition = Vector3.New( quickObj.transform.localPosition.x, quickObj.transform.localPosition.y+80, 0 );
		quickObj.transform.localScale = Vector3.one;
		unitObj:GetComponent("CityBuilding").BuildingQuickMod = quickObj;
	end
	local ShareData = quickObj.transform:GetComponent("ShareData");
	ShareData.intValue[0] = kind;
	quickObj.gameObject:SetActive(true);
end

-- 加速头隐藏
function City.BuildingHideQuick( kind )
	local unitObj = nil;
	if kind >= BUILDING_Silver and kind <= BUILDING_Iron then
		return
	end
	unitObj = City.m_Buildings[kind];
	local quickObj = unitObj:GetComponent("CityBuilding").BuildingQuickMod;
	if quickObj == nil then
		return
	end
	quickObj.gameObject:SetActive(false);
end

-- 征收次数
function City.BuildingAddLevy()	
	for i=21, 24, 1 do
		if City.m_Buildings_res[i] then
			for k, v in pairs( City.m_Buildings_res[i] ) do
				local obj = v:Find("LevyMod").gameObject;
				local drawingObj = v:Find("ResDrawingMod").gameObject;
				if drawingObj == nil or drawingObj.activeSelf == false then
					if obj and obj.activeSelf == false then
						obj:SetActive( true );
						break;
					end
				end
			end
		end
	end
end

-- 征收次数
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

-- 城墙-补充城防头
function City.GuardCallMod( unitObj, show )
	if show == true then
		local level = GetPlayer():BuildingLevel( BUILDING_Wall, -1 )
		if level < global.city_guard_level then
			show = false -- 没开启
		end
		if GetPlayer().m_guardnum >= level then
			show = false -- 满了
		end
		if GetPlayer().m_guardsec > 0 then
			show = false -- 冷却
		end
	end
	if unitObj == nil then
		unitObj = City.m_Buildings[BUILDING_Wall];
		if unitObj == nil then
			return
		end
	end
	local modObj = unitObj.transform:Find( "GuardCallMod" )
	if modObj == nil then
		return
	end
	
	modObj.gameObject:SetActive( show );	
end

-- 聚贤馆-免费寻访头
function City.HeroVisitMod( unitObj, show, type )
	if unitObj == nil then
		unitObj = City.m_Buildings[BUILDING_Hero];
		if unitObj == nil then
			return
		end
	end
	local modObj = unitObj.transform:Find( "HeroVisitMod" )
	if modObj == nil then
		return
	end
	if show == true then
		if type == 1 then
			SetText( modObj.transform:Find("Back/Text"), T(1952) )
		else
			SetText( modObj.transform:Find("Back/Text"), T(1932) )
		end
	end
	modObj.gameObject:SetActive( show );
end


-- 点击建造队列
function City.GoToWorker()
	-- 移动并选择
	if GetPlayer().m_worker_kind > 0 then
		if GetPlayer().m_worker_free > 0 then
			-- 免费加速
			system_askinfo( ASKINFO_BUILDING, "", 2, GetPlayer().m_worker_kind, GetPlayer().m_worker_offset );
			City.Move( GetPlayer().m_worker_kind, GetPlayer().m_worker_offset, false )
		else
			City.Move( GetPlayer().m_worker_kind, GetPlayer().m_worker_offset, true )
		end
		return;
	end
	
	-- 找到一个可以升级的
	City.FindCanUpgrade( function( kind, offset )
		City.Move( kind, offset, true );
	end )
end

-- 点击建造队列商用
function City.GoToWorkerEx()
	-- 移动并选择
	if GetPlayer().m_worker_kind_ex > 0 then
		if GetPlayer().m_worker_free_ex > 0 then
			-- 免费加速
			system_askinfo( ASKINFO_BUILDING, "", 2, GetPlayer().m_worker_kind_ex, GetPlayer().m_worker_offset_ex );
			City.Move( GetPlayer().m_worker_kind_ex, GetPlayer().m_worker_offset_ex, false )
		else
			City.Move( GetPlayer().m_worker_kind_ex, GetPlayer().m_worker_offset_ex, true )
		end
		return
	end
	
	-- 打开购买商用建造队界面
	if GetPlayer().m_worker_expire_ex <= 0 then
		-- 
		BuyWorkerDlgShow();
		return
	end
	
	-- 找到一个可以升级的
	City.FindCanUpgrade( function( kind, offset )
		City.Move( kind, offset, true );
	end )
end

-- 找到一个可以升级的
function City.AutoBuild()
	if GetPlayer().m_autobuildopen == 0 then
		return
	end
	City.FindCanUpgrade( function( kind, offset )
		system_askinfo( ASKINFO_AUTOBUILD, "", 1, kind, offset );
	end )
end

-- 找到一个可以升级的
function City.FindCanUpgrade( callback )
	
	-- 空闲建造队是否满足
	local flag = true;
	if GetPlayer().m_worker_kind > 0 and GetPlayer().m_worker_kind_ex > 0 then
		flag = false;
	elseif GetPlayer().m_worker_kind > 0 and GetPlayer().m_worker_expire_ex <= 0 then
		flag = false;
	end
	if flag == false then
		return;
	end
	
	-- 优先普通建筑
	for k, v in pairs( g_building_upgrade ) do
		if k >= BUILDING_Main and k <= BUILDING_Militiaman_Archer and (k ~= GetPlayer().m_worker_kind and k ~= GetPlayer().m_worker_kind_ex) then
			local pBuilding = GetPlayer():GetBuilding( k, -1 )
			if pBuilding ~= nil then
				local buildingConfig = v[pBuilding.m_level+1]
				if GetPlayer():CityLevel() >= buildingConfig.citylevel and
					GetPlayer().m_level >= buildingConfig.actorlevel and
					GetPlayer().m_silver >= buildingConfig.silver and
					GetPlayer().m_wood >= buildingConfig.wood and
					GetPlayer().m_food >= buildingConfig.food and
					GetPlayer().m_iron >= buildingConfig.iron then
					callback( k, -1 )
					return
				end
			end
		end
	end
	
	-- 找资源建筑
	for k, v in pairs( g_building_upgrade ) do
		if k >= BUILDING_Silver and k <= BUILDING_Iron and (k ~= GetPlayer().m_worker_kind and k ~= GetPlayer().m_worker_kind_ex) then
			for offset=1, 64, 1 do
				local pBuilding = GetPlayer():GetBuilding( k, offset )
				if pBuilding ~= nil and pBuilding.m_level < #v then
					local buildingConfig = v[pBuilding.m_level+1]
					if GetPlayer():CityLevel() >= buildingConfig.citylevel and
						GetPlayer().m_level >= buildingConfig.actorlevel and
						GetPlayer().m_silver >= buildingConfig.silver and
						GetPlayer().m_wood >= buildingConfig.wood and
						GetPlayer().m_food >= buildingConfig.food and
						GetPlayer().m_iron >= buildingConfig.iron then
						callback( k, offset )
						return
					end
				end
			end
		end
	end
end

-- 找到一个可以升级的
function City.QuestFindCanUpgrade( kind, callback )	
	-- 找资源建筑
	for k, v in pairs( g_building_upgrade ) do
		if k == kind then
			for offset=1, 64, 1 do
				local pBuilding = GetPlayer():GetBuilding( k, offset )
				if pBuilding ~= nil and pBuilding.m_level < #v then
					local buildingConfig = v[pBuilding.m_level+1]
					if GetPlayer():CityLevel() >= buildingConfig.citylevel and
						GetPlayer().m_level >= buildingConfig.actorlevel and
						GetPlayer().m_silver >= buildingConfig.silver and
						GetPlayer().m_wood >= buildingConfig.wood and
						GetPlayer().m_food >= buildingConfig.food and
						GetPlayer().m_iron >= buildingConfig.iron then
						callback( k, offset )
						return
					end
				end
			end
		end
	end
end

-- 任务图标
function City.BuildingQuestMod( questid )
	City.m_BuildingQuestMod.gameObject:SetActive(true);
	local uiTween = City.m_BuildingQuestMod.transform:GetComponent("UITweenRectPosition")
	if questid == 20 then
		City.m_BuildingQuestMod.transform.localPosition = Vector3.New( -110, -461, 0 );
		uiTween.from = Vector2.New( -110, -461 );
		uiTween.to = Vector2.New( -110, -451 );
		uiTween:Play(true)
		SetImage( City.m_BuildingQuestMod.transform:Find("Back"), LoadSprite("ui_opration_quest_1") )
	elseif questid == 55 then
		City.m_BuildingQuestMod.transform.localPosition = Vector3.New( 407, -314, 0 );
		uiTween.from = Vector2.New( 407, -314 );
		uiTween.to = Vector2.New( 407, -304 );
		uiTween:Play(true)
		SetImage( City.m_BuildingQuestMod.transform:Find("Back"), LoadSprite("ui_opration_quest_2") )
	end
	local ShareData = City.m_BuildingQuestMod.transform:GetComponent("ShareData");
	ShareData.intValue[0] = questid;
end
