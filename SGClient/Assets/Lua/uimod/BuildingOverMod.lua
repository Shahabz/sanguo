local m_Mod;
----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function BuildingOverModOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
		--print( "Button Clicked, nControlID:" .. nControlID );
		BuildingOverModClick( gameObject )
	elseif nType == UI_EVENT_PRESS then
		if value == 0 then
			--print( "Button Pressed Down, nControlID:" .. nControlID );
		elseif value == 1 then
			--print( "Button Pressed UP, nControlID:" .. nControlID);
		end
	end
end

-- 载入时调用
function BuildingOverModOnAwake( gameObject )
	m_Mod = gameObject;
	local objs = gameObject:GetComponent( "UIMod" ).relatedGameObject;
end

-- 界面初始化时调用
function BuildingOverModOnStart( gameObject )
	
end

-- 界面显示时调用
function BuildingOverModOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function BuildingOverModOnDisable( gameObject )
	
end

-- 界面删除时调用
function BuildingOverModOnDestroy( gameObject )
	
end

-- 每帧调用
function BuildingOverModOnLogic( gameObject )
	
end

function BuildingOverModGet()
	return m_Mod;
end
----------------------------------------
-- 自定
----------------------------------------
function BuildingOverModClick( gameObject )
	local ShareData = gameObject.transform:GetComponent("ShareData");
	local kind = ShareData.intValue[0];
	if kind == BUILDING_Tech then
		system_askinfo( ASKINFO_TECH, "", 4 );
	
	elseif kind == BUILDING_Craftsman then
		system_askinfo( ASKINFO_MATERIALMAKE, "", 4 );
		
	elseif kind >= BUILDING_Infantry and kind <= BUILDING_Militiaman_Archer then
		system_askinfo( ASKINFO_TRAIN, "", 4, kind );
	
	elseif kind == BUILDING_Smithy then
		system_askinfo( ASKINFO_EQUIPFORGING, "", 4 );
		
	end
	gameObject:SetActive(false);
end
