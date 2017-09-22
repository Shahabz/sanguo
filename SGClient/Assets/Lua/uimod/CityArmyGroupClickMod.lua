local m_Mod;
----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function CityArmyGroupClickModOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
		MapArmyGroupDlgShow( nControlID )
		--print( "Button Clicked, nControlID:" .. nControlID );
	elseif nType == UI_EVENT_PRESS then
		if value == 0 then
			print( "Button Pressed Down, nControlID:" .. nControlID );
		elseif value == 1 then
			print( "Button Pressed UP, nControlID:" .. nControlID);
		end
	end
end

-- 载入时调用
function CityArmyGroupClickModOnAwake( gameObject )
	m_Mod = gameObject;
	local objs = gameObject:GetComponent( "UIMod" ).relatedGameObject;
end

-- 界面初始化时调用
function CityArmyGroupClickModOnStart( gameObject )
	
end

-- 界面显示时调用
function CityArmyGroupClickModOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function CityArmyGroupClickModOnDisable( gameObject )
	
end

-- 界面删除时调用
function CityArmyGroupClickModOnDestroy( gameObject )
	
end

-- 每帧调用
function CityArmyGroupClickModOnLogic( gameObject )
	
end

function CityArmyGroupClickModGet()
	return m_Mod;
end
----------------------------------------
-- 自定
----------------------------------------
