local m_Mod;
----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function BuildingFreeModOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
		BuildingFreeModClick( gameObject )
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
function BuildingFreeModOnAwake( gameObject )
	m_Mod = gameObject;
	local objs = gameObject:GetComponent( "UIMod" ).relatedGameObject;
end

-- 界面初始化时调用
function BuildingFreeModOnStart( gameObject )
	
end

-- 界面显示时调用
function BuildingFreeModOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function BuildingFreeModOnDisable( gameObject )
	
end

-- 界面删除时调用
function BuildingFreeModOnDestroy( gameObject )
	
end

-- 每帧调用
function BuildingFreeModOnLogic( gameObject )
	
end

function BuildingFreeModGet()
	return m_Mod;
end
----------------------------------------
-- 自定
----------------------------------------
function BuildingFreeModClick( gameObject )
	local ShareData = gameObject.transform:GetComponent("ShareData");
	local kind = ShareData.intValue[0];
	local offset = ShareData.intValue[1];
	system_askinfo( ASKINFO_BUILDING, "", 2, kind, offset );
	gameObject:SetActive(false);
	if IsGuiding() and GetCurrentGuideType() == GUIDE_FREE then GuideNext() end;
end
