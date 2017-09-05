local m_Mod;
----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function BuildingQuickModOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
		BuildingQuickModClick( gameObject )
		--print( "Button Clicked, nControlID:" .. nControlID );
	elseif nType == UI_EVENT_PRESS then
		if value == 0 then
			--print( "Button Pressed Down, nControlID:" .. nControlID );
		elseif value == 1 then
			--print( "Button Pressed UP, nControlID:" .. nControlID);
		end
	end
end

-- 载入时调用
function BuildingQuickModOnAwake( gameObject )
	m_Mod = gameObject;
	local objs = gameObject:GetComponent( "UIMod" ).relatedGameObject;
end

-- 界面初始化时调用
function BuildingQuickModOnStart( gameObject )
	
end

-- 界面显示时调用
function BuildingQuickModOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function BuildingQuickModOnDisable( gameObject )
	
end

-- 界面删除时调用
function BuildingQuickModOnDestroy( gameObject )
	
end

-- 每帧调用
function BuildingQuickModOnLogic( gameObject )
	
end

function BuildingQuickModGet()
	return m_Mod;
end
----------------------------------------
-- 自定
----------------------------------------
function BuildingQuickModClick( gameObject )
	local ShareData = gameObject.transform:GetComponent("ShareData");
	local kind = ShareData.intValue[0];
	if kind >= BUILDING_Infantry and kind <= BUILDING_Militiaman_Archer then
		system_askinfo( ASKINFO_TRAIN, "", 7, kind );
	end
	City.BuildingHideQuick( kind );
	gameObject:SetActive(false);
end
