local m_Mod;
----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function BuildingTimerModOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
		--print( "Button Clicked, nControlID:" .. nControlID );
	elseif nType == UI_EVENT_PRESS then
		if value == 0 then
			--print( "Button Pressed Down, nControlID:" .. nControlID );
		elseif value == 1 then
			--print( "Button Pressed UP, nControlID:" .. nControlID);
		end
	elseif nType == UI_EVENT_TIMECOUNTEND then
		gameObject:SetActive(false);
	end
end

-- 载入时调用
function BuildingTimerModOnAwake( gameObject )
	m_Mod = gameObject;
	local objs = gameObject:GetComponent( "UIMod" ).relatedGameObject;
end

-- 界面初始化时调用
function BuildingTimerModOnStart( gameObject )
	
end

-- 界面显示时调用
function BuildingTimerModOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function BuildingTimerModOnDisable( gameObject )
	
end

-- 界面删除时调用
function BuildingTimerModOnDestroy( gameObject )
	
end

-- 每帧调用
function BuildingTimerModOnLogic( gameObject )
	
end

function BuildingTimerModGet()
	return m_Mod;
end
----------------------------------------
-- 自定
----------------------------------------
