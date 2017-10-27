local m_Mod;
----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function TownProduceModOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
		if nControlID == 1 then
			TownProduceModClick( gameObject )
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
function TownProduceModOnAwake( gameObject )
	m_Mod = gameObject;
	local objs = gameObject:GetComponent( "UIMod" ).relatedGameObject;
end

-- 界面初始化时调用
function TownProduceModOnStart( gameObject )
	
end

-- 界面显示时调用
function TownProduceModOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function TownProduceModOnDisable( gameObject )
	
end

-- 界面删除时调用
function TownProduceModOnDestroy( gameObject )
	
end

-- 每帧调用
function TownProduceModOnLogic( gameObject )
	
end

function TownProduceModGet()
	return m_Mod;
end
----------------------------------------
-- 自定
----------------------------------------
function TownProduceModClick( gameObject )
	local ShareData = gameObject.transform:GetComponent("ShareData");
	local unit_index = ShareData.intValue[0];
	local recvValue = WorldMap.m_nMapUnitList[unit_index];
	if recvValue == nil then
		return;
	end
	local nation = recvValue.m_char_value[1];
	if nation == GetPlayer().m_nation then
		MapTownDlgShow( 1, recvValue )
	else
		AlertMsg( T(1290) )
	end
end