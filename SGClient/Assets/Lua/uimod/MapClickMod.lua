-- 世界地图操作
local m_Mod				= nil;
local m_uiEmptyInfo 	= nil; --UnityEngine.GameObject
local m_uiCityInfo		= nil; --UnityEngine.GameObject
local m_uiMoveCityBtn = nil; --UnityEngine.GameObject
local m_uiInfoBtn = nil; --UnityEngine.GameObject
local m_uiHelpBtn = nil; --UnityEngine.GameObject
local m_uiFightBtn = nil; --UnityEngine.GameObject
local m_uiEnterBtn = nil; --UnityEngine.GameObject
local m_uiMyFightBtn = nil; --UnityEngine.GameObject
local m_uiTownInfo = nil; --UnityEngine.GameObject
local m_uiNationInfoBtn = nil; --UnityEngine.GameObject
local m_uiNationFightBtn = nil; --UnityEngine.GameObject
local m_uiTreasureBtn = nil; --UnityEngine.GameObject
local m_uiCallBtn = nil; --UnityEngine.GameObject

local m_LastRecvValue = nil
local m_towntype = 0;

-- 移动距离
local uiButtonOffset = 85;

-- 移动距离
local uiButtonOffsetX = 100;
local uiButtonOffsetY = 70;

-- 所属按钮点击时调用
function MapClickModOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
		
		-- 迁城
		if nControlID == 1 then
			ItemUseExDlgShow( 1 )
			
		-- 城池信息
		elseif nControlID == 2 then
			MapCityDlgShow( m_LastRecvValue )
		
		-- 驻防
		elseif nControlID == 3 then
			MapCityHelpDlgShow( m_LastRecvValue )
			
		-- 城战
		elseif nControlID == 4 then
			MapArmyGroupDlgShow( m_LastRecvValue.m_unit_index )
			
		-- 进入城池
		elseif nControlID == 5 then
			if GameManager.currentScence == "worldmap" then
				WorldMap.ReturnCity()
			end
		
		-- 本城城战(无用)
		elseif nControlID == 6 then
		
		-- 挖宝
		elseif nControlID == 7 then
			TreasureDlgDo( WorldMap.m_nLastTouchGameCoorX, WorldMap.m_nLastTouchGameCoorY )
		
		-- 召唤
		elseif nControlID == 8 then
			MapCallDlgShow()
	
		-- 城镇信息
		elseif nControlID == 11 then
			if m_towntype == MAPUNIT_TYPE_TOWN_GJFD then
				MapTownExDlgShow( m_LastRecvValue )
			else
				MapTownDlgShow( 0, m_LastRecvValue )
			end
		
		-- 国战
		elseif nControlID == 12 then
			MapNationFightDlgShow( m_LastRecvValue.m_unit_index )
			
		end

		if m_Mod then
			m_Mod:SetActive( false );
		end
		WorldMap.ClickEffect().gameObject:SetActive( false );
	elseif nType == UI_EVENT_PRESS then
		
	end
end

-- 载入时调用
function MapClickModOnAwake( gameObject )
	m_Mod = gameObject;
	local objs = gameObject:GetComponent("UIMod").relatedGameObject;
	m_uiEmptyInfo = objs[0];
	m_uiCityInfo = objs[1];
	m_uiMoveCityBtn = objs[2];
	m_uiInfoBtn = objs[3];
	m_uiHelpBtn = objs[4];
	m_uiFightBtn = objs[5];
	m_uiEnterBtn = objs[6];
	m_uiMyFightBtn = objs[7];
	m_uiTownInfo = objs[8];
	m_uiNationInfoBtn = objs[9];
	m_uiNationFightBtn = objs[10];
	m_uiTreasureBtn = objs[11];
	m_uiCallBtn = objs[12];
end

-- 界面初始化时调用
function MapClickModOnStart( gameObject )
	
end

-- 界面显示时调用
function MapClickModOnEnable( gameObject )
	m_LastRecvValue = nil;
	local unit_index = WorldMap.m_nLastTouchUnitIndex;
	local gameCoorX  = WorldMap.m_nLastTouchGameCoorX;
	local gameCoorY  = WorldMap.m_nLastTouchGameCoorY;

	if unit_index == -2 then
		-- 不允许操作的空地
		MapClickModOpenNull(gameCoorX, gameCoorY);
		
	elseif unit_index < 0 then
		-- 空地
		MapClickModOpenEmpty( recvValue, gameCoorX, gameCoorY );
		
	else
		local recvValue = WorldMap.m_nMapUnitList[unit_index];
		-- 空地
		if recvValue == nil then
			MapClickModOpenEmpty( recvValue, gameCoorX, gameCoorY )
			
		-- 城池
		elseif recvValue.m_type == MAPUNIT_TYPE_CITY then
			MapClickModOpenCity( recvValue, gameCoorX, gameCoorY )
			
		-- 城镇
		elseif recvValue.m_type == MAPUNIT_TYPE_TOWN then
			MapClickModOpenTown( recvValue, gameCoorX, gameCoorY )
			
		end
	end
end

-- 界面隐藏时调用
function MapClickModOnDisable( gameObject )
	SetFalse( m_uiEmptyInfo )
	SetFalse( m_uiCityInfo )
	SetFalse( m_uiTownInfo )
	if m_LastRecvValue ~= nil then
		if m_LastRecvValue.m_type == MAPUNIT_TYPE_TOWN then
			MapClickModCloseTown( m_LastRecvValue )
		end
	end
end

-- 界面删除时调用
function MapClickModOnDestroy( gameObject )
	m_Mod = nil;
end

-- 每帧调用
function MapClickModOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
-- 更新坐标
function MapClickModCoordinate( gameCoorX, gameCoorY )
	--[[if WorldMap.MapClickMod().gameObject.activeSelf == false then
		return;
	end
	SetText( m_uiEmptyInfo.transform:Find("Pos"), "["..gameCoorX..","..gameCoorY.."]" )	--]]
end

-- 点击不允许操作的地面
function MapClickModOpenNull( gameCoorX, gameCoorY )
	MapClickModOpenEmpty( nil, gameCoorX, gameCoorY )
end

-- 点击空地显示的操作界面
function MapClickModOpenEmpty( recvValue, gameCoorX, gameCoorY )
	SetTrue( m_uiEmptyInfo )
	SetText( m_uiEmptyInfo.transform:Find("Pos"), "["..gameCoorX..","..gameCoorY.."]" )
	SetFalse( m_uiMoveCityBtn )
	SetFalse( m_uiTreasureBtn )
	SetFalse( m_uiCallBtn )
	local buttonList = {}
	
	-- 地块类型
	local terrain = MapTile.getTileData( gameCoorX, gameCoorY )
	if terrain == -1 then -- 边界
	elseif terrain == -2 then -- 河流
		SetText( m_uiEmptyInfo.transform:Find("TitleName"), T(3021) )
	elseif terrain == -3 then -- 高山
		SetText( m_uiEmptyInfo.transform:Find("TitleName"), T(3022) )
	else
		SetText( m_uiEmptyInfo.transform:Find("TitleName"), T(3023) )
		table.insert( buttonList, m_uiMoveCityBtn )
	end
	
	-- 领土所属
	if WorldMap.m_nZoneID == MAPZONE_CENTERID then
		if map_zone_inrange( MAPZONE_CENTERID, gameCoorX, gameCoorY ) == 1 then
			for townid = 161, 200, 1 do
				local info = g_towninfo[townid];
				if info then
					if gameCoorX >= info.posx-10 and gameCoorX <= info.posx+9 and gameCoorY >= info.posy-10 and gameCoorY <= info.posy+9 then
						local nation = WorldMap.m_CenterNation[townid];
						if nation then
							local color = Hex2Color(MapUnitRangeColor[nation]);
							SetText( m_uiEmptyInfo.transform:Find("Nation"), NationEx( WorldMap.m_Nation )..T(115), color )
						end
						break
					end
				end
			end
		else
			local color = Hex2Color(0x989898FF);
			SetText( m_uiEmptyInfo.transform:Find("Nation"), T(1183), color )
		end
	else
		if map_zone_inrange( WorldMap.m_nZoneID, gameCoorX, gameCoorY ) == 1 then
			local color = Hex2Color(MapUnitRangeColor[WorldMap.m_Nation]);
			SetText( m_uiEmptyInfo.transform:Find("Nation"), NationEx( WorldMap.m_Nation )..T(115), color )
		else
			local color = Hex2Color(0x989898FF);
			SetText( m_uiEmptyInfo.transform:Find("Nation"), T(1183), color )
		end
	end
	
	-- 挖宝活动中
	if MapMainDlgActivityTreasureState() == 1 then
		table.insert( buttonList, m_uiTreasureBtn )
	end
	MapClickModButton( buttonList );
end

-- 点击城市显示的操作界面
function MapClickModOpenCity( recvValue, gameCoorX, gameCoorY )	
	SetTrue( m_uiCityInfo )	
	m_LastRecvValue = recvValue;
	local name 		= recvValue.m_name;
	local posx 		= recvValue.m_posx;
	local posy 		= recvValue.m_posy;
	local level 	= recvValue.m_char_value[1];
	local nation	= recvValue.m_char_value[2];
	local mapcall	= recvValue.m_char_value[4];
	
	SetText( m_uiCityInfo.transform:Find("Pos"), "["..posx..","..posy.."]" )
	SetText( m_uiCityInfo.transform:Find("Level"), "Lv."..level..T(995) )
	SetText( m_uiCityInfo.transform:Find("Name"), name )
	SetImage( m_uiCityInfo.transform:Find("Nation"), NationSprite(nation) )
	
	-- 我自己的城池
	if recvValue.m_unit_index == WorldMap.m_nMyCityUnitIndex then
		SetFalse( m_uiInfoBtn )
		SetFalse( m_uiFightBtn )
		SetFalse( m_uiHelpBtn )
		SetTrue( m_uiEnterBtn )
		SetFalse( m_uiMyFightBtn )
		
		if GetPlayer().m_official > 0 and mapcall <= 0 then
			SetTrue( m_uiCallBtn )
			local buttonList = { m_uiEnterBtn, m_uiCallBtn };
			MapClickModButton( buttonList );
		else
			SetFalse( m_uiCallBtn )
			local buttonList = { m_uiEnterBtn };
			MapClickModButton( buttonList );
		end
	
	-- 我国城池
	elseif nation == GetPlayer().m_nation then
		SetTrue( m_uiInfoBtn )
		SetTrue( m_uiFightBtn )
		SetTrue( m_uiHelpBtn )
		SetFalse( m_uiEnterBtn )
		SetFalse( m_uiMyFightBtn )
		SetFalse( m_uiCallBtn )
		local buttonList = { m_uiInfoBtn, m_uiHelpBtn, m_uiFightBtn };
		MapClickModButton( buttonList );
		
	-- 敌国城池
	else
		SetTrue( m_uiInfoBtn )
		SetTrue( m_uiFightBtn )
		SetFalse( m_uiHelpBtn )
		SetFalse( m_uiEnterBtn )
		SetFalse( m_uiMyFightBtn )
		SetFalse( m_uiCallBtn )
		local buttonList = { m_uiInfoBtn, m_uiFightBtn };
		MapClickModButton( buttonList );
	end
end

-- 点击城镇显示的操作界面
function MapClickModOpenTown( recvValue, gameCoorX, gameCoorY )	
	SetTrue( m_uiTownInfo )	
	m_LastRecvValue = recvValue;
	local posx 			= recvValue.m_posx;
	local posy 			= recvValue.m_posy;
	local custom_name	= recvValue.m_name;
	local custom_namelen= recvValue.m_namelen;
	local nation 		= recvValue.m_char_value[1];
	local dev_level 	= recvValue.m_char_value[2];
	local townid 		= recvValue.m_short_value[1];
	local produce_num	= recvValue.m_short_value[2];
	local protect_sec	= recvValue.m_int_value[1];
	local produce_sec	= recvValue.m_int_value[2];
	
	local type 			= g_towninfo[townid].type
	local level 		= g_towninfo[townid].level
	local produce_maxnum= g_towninfo[townid].produce_maxnum
	local produce_maxsec= g_towninfo[townid].produce_maxsec
	m_towntype = type;
	
	SetText( m_uiTownInfo.transform:Find("Pos"), "["..posx..","..posy.."]" )
	if type == MAPUNIT_TYPE_TOWN_GJFD then
		SetText( m_uiTownInfo.transform:Find("Level"), "Lv."..(dev_level+1)..MapTownName(townid) )
	else
		SetText( m_uiTownInfo.transform:Find("Level"), MapTownName(townid) )
	end
	
	SetText( m_uiTownInfo.transform:Find("Name"), T(180+type) )
	SetImage( m_uiTownInfo.transform:Find("Nation"), NationSprite(nation) )
	
	-- 我国占领
	if nation == GetPlayer().m_nation or type == MAPUNIT_TYPE_TOWN_GJFD then
		SetTrue( m_uiNationInfoBtn )
		SetFalse( m_uiNationFightBtn )
		local buttonList = { m_uiNationInfoBtn };
		MapClickModButton( buttonList );
		
	-- 敌国占领
	else
		SetTrue( m_uiNationInfoBtn )
		SetTrue( m_uiNationFightBtn )
		local buttonList = { m_uiNationInfoBtn, m_uiNationFightBtn };
		MapClickModButton( buttonList );
	end
	
	-- 隐藏生产信息
	local unitObj = MapUnit.cache[recvValue.m_unit_index];
	if unitObj ~= nil then
		local objs = unitObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
		local uiTownProduceMod = objs[3];
		SetFalse( uiTownProduceMod )
	end
end

-- 城镇显示的操作界面关闭后调用
function MapClickModCloseTown( recvValue )
	local nation 		= recvValue.m_char_value[1];
	local townid 		= recvValue.m_short_value[1];
	local type 			= g_towninfo[townid].type
	if nation > 0 and type < MAPUNIT_TYPE_TOWN_GJFD then
		-- 显示生产信息
		local unitObj = MapUnit.cache[recvValue.m_unit_index];
		if unitObj ~= nil then
			local objs = unitObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
			local uiTownProduceMod = objs[3];
			SetTrue( uiTownProduceMod )
		end
	end
end

-------------------------------
-- 按钮表现
-------------------------------
local m_ButtonPos = 
{
    [1] = { Vector3.New( 0, -uiButtonOffset, 0 ) },
    [2] = { Vector3.New( -uiButtonOffsetX, -uiButtonOffset, 0 ), Vector3.New( uiButtonOffsetX, -uiButtonOffset, 0 ) },
    [3] = { Vector3.New( -uiButtonOffsetX, -uiButtonOffsetY, 0 ), Vector3.New( 0, -uiButtonOffset, 0 ), Vector3.New( uiButtonOffsetX, -uiButtonOffsetY, 0 ) },
    [4] = { Vector3.New( -uiButtonOffset, 0, 0 ), Vector3.New( -uiButtonOffset / 2, uiButtonOffset, 0 ), Vector3.New( uiButtonOffset / 2, uiButtonOffset, 0 ), Vector3.New( uiButtonOffset, 0, 0 ) },
    [5] = { Vector3.New( -uiButtonOffset, 0, 0 ), Vector3.New( -uiButtonOffset * 3 / 4, uiButtonOffset * 3 / 4, 0 ), Vector3.New( 0, uiButtonOffset, 0 ), Vector3.New( uiButtonOffset * 3 / 4, uiButtonOffset * 3 / 4, 0 ), Vector3.New( uiButtonOffset, 0, 0 ) },
}
function MapClickModButton( buttonList )
    for i = 1, #buttonList, 1 do
        buttonList[i].gameObject:SetActive( true );
        buttonList[i]:GetComponent(typeof(UITweenLocalPosition)).to = m_ButtonPos[ #buttonList ][i];
        buttonList[i]:GetComponent(typeof(UITweenLocalPosition)):Play( true );
    end
end

-------------------------------
-- 功能
-------------------------------
-- 点击-攻击
function MapClickModAttack()
	
	--[[local cacheValue = WorldMap.m_nMapUnitList[WorldMap.m_nLastTouchUnitIndex];
    if cacheValue == nil then
        return;
    end
	if cacheValue.m_type == MAPUNIT_TYPE_CITY or cacheValue.m_type == MAPUNIT_TYPE_ARMY or cacheValue.m_type == MAPUNIT_TYPE_RES then
		local buff = GetCity().BuffDuration[1];
		if buff and ( buff.endtime - GetServerTime() > 0 ) then
			MsgBox( GetLocalizeText(9004), function() TroopDlgOpenWithAction( ARMY_ACTION_FIGHT ) end );
			return
		end
	end
	TroopDlgOpenWithAction( ARMY_ACTION_FIGHT );--]]
end

-- 城战
function MapClickModOpenCityFight( unit_index )
	if m_LastRecvValue == nil then
		m_LastRecvValue = WorldMap.m_nMapUnitList[unit_index];
		if m_LastRecvValue == nil then
			return
		end
		if m_LastRecvValue.m_type ~= MAPUNIT_TYPE_CITY then
			return
		end
	end
	if m_LastRecvValue.m_unit_index ~= unit_index then
		m_LastRecvValue = WorldMap.m_nMapUnitList[unit_index];
		if m_LastRecvValue == nil then
			return
		end
		if m_LastRecvValue.m_type ~= MAPUNIT_TYPE_CITY then
			return
		end
	end
	MapCityFightDlgShow( m_LastRecvValue )
end
