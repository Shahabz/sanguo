local m_Mod;
----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function CityMapCallModOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
		--print( "Button Clicked, nControlID:" .. nControlID );
		if nControlID == 1 then
			CityMapCallModClick( gameObject )
		end
	elseif nType == UI_EVENT_PRESS then
		if value == 0 then
			--print( "Button Pressed Down, nControlID:" .. nControlID );
		elseif value == 1 then
			--print( "Button Pressed UP, nControlID:" .. nControlID);
		end
	end
end

-- 载入时调用
function CityMapCallModOnAwake( gameObject )
	m_Mod = gameObject;
	local objs = gameObject:GetComponent( "UIMod" ).relatedGameObject;
end

-- 界面初始化时调用
function CityMapCallModOnStart( gameObject )
	
end

-- 界面显示时调用
function CityMapCallModOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function CityMapCallModOnDisable( gameObject )
	
end

-- 界面删除时调用
function CityMapCallModOnDestroy( gameObject )
	
end

-- 每帧调用
function CityMapCallModOnLogic( gameObject )
	
end

function CityMapCallModGet()
	return m_Mod;
end
----------------------------------------
-- 自定
----------------------------------------
function CityMapCallModClick( gameObject )
	local ShareData = gameObject.transform:GetComponent("ShareData");
	local unit_index = ShareData.intValue[0];
	local index = ShareData.intValue[1];
	if unit_index == WorldMap.m_nMyCityUnitIndex then
		MsgBox( T(1866), function() 
			system_askinfo( ASKINFO_WORLDMAP, "", 13 );
		end )
	else
		MsgBox( T(1867), function() 
			system_askinfo( ASKINFO_WORLDMAP, "", 12, index );
		end )
	end
end
