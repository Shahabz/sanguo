local m_Mod;
----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function BuildingWorkerQuickModOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
		BuildingWorkerQuickModClick( gameObject )
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
function BuildingWorkerQuickModOnAwake( gameObject )
	m_Mod = gameObject;
	local objs = gameObject:GetComponent( "UIMod" ).relatedGameObject;
end

-- 界面初始化时调用
function BuildingWorkerQuickModOnStart( gameObject )
	
end

-- 界面显示时调用
function BuildingWorkerQuickModOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function BuildingWorkerQuickModOnDisable( gameObject )
	
end

-- 界面删除时调用
function BuildingWorkerQuickModOnDestroy( gameObject )
	
end

-- 每帧调用
function BuildingWorkerQuickModOnLogic( gameObject )
	
end

function BuildingWorkerQuickModGet()
	return m_Mod;
end
----------------------------------------
-- 自定
----------------------------------------
function BuildingWorkerQuickModClick( gameObject )
	local ShareData = gameObject.transform:GetComponent("ShareData");
	local kind = ShareData.intValue[0];
	local offset = ShareData.intValue[1];
	system_askinfo( ASKINFO_BUILDING, "", 4, kind, offset );
	City.BuildingHideWorkerQuick( kind, offset );
	gameObject:SetActive(false);
end
