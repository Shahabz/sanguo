local m_Mod;
----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function DayEventModOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
		EveryDayEventDlgShow()
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
function DayEventModOnAwake( gameObject )
	m_Mod = gameObject;
	local objs = gameObject:GetComponent( "UIMod" ).relatedGameObject;
end

-- 界面初始化时调用
function DayEventModOnStart( gameObject )
	
end

-- 界面显示时调用
function DayEventModOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function DayEventModOnDisable( gameObject )
	
end

-- 界面删除时调用
function DayEventModOnDestroy( gameObject )
	m_Mod = nil
end

-- 每帧调用
function DayEventModOnLogic( gameObject )
	
end

function DayEventModGet()
	return m_Mod;
end
----------------------------------------
-- 自定
----------------------------------------
