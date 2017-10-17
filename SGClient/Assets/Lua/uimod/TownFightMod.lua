local m_Mod;
----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function TownFightModOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
		--print( "Button Clicked, nControlID:" .. nControlID );
		if nControlID == 1 then
			TownFightModClick( gameObject )
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
function TownFightModOnAwake( gameObject )
	m_Mod = gameObject;
	local objs = gameObject:GetComponent( "UIMod" ).relatedGameObject;
end

-- 界面初始化时调用
function TownFightModOnStart( gameObject )
	
end

-- 界面显示时调用
function TownFightModOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function TownFightModOnDisable( gameObject )
	
end

-- 界面删除时调用
function TownFightModOnDestroy( gameObject )
	
end

-- 每帧调用
function TownFightModOnLogic( gameObject )
	
end

function TownFightModGet()
	return m_Mod;
end
----------------------------------------
-- 自定
----------------------------------------
function TownFightModClick( gameObject )
	local ShareData = gameObject.transform:GetComponent("ShareData");
	local unit_index = ShareData.intValue[0];
	local recvValue = WorldMap.m_nMapUnitList[unit_index];
	if recvValue == nil then
		return;
	end
	MapNationFightDlgShow( unit_index )
end