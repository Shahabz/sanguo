-- 界面
local m_Dlg = nil;
local m_uiShape = nil; --UnityEngine.GameObject
local m_uiName = nil; --UnityEngine.GameObject
local m_uiPos = nil; --UnityEngine.GameObject
local m_uiNation = nil; --UnityEngine.GameObject
local m_uiAwardGrid = nil; --UnityEngine.GameObject
local m_uiMarchTime = nil; --UnityEngine.GameObject
local m_uiColor = nil; --UnityEngine.GameObject
local m_uiCorps = nil; --UnityEngine.GameObject

local m_unit_index = -1;
local m_recvValue = nil;
local m_awardCache = {};

-- 打开界面
function MapNationHeroDlgOpen()
	m_Dlg = eye.uiManager:Open( "MapNationHeroDlg" );
	
end

-- 隐藏界面
function MapNationHeroDlgClose()
	if m_Dlg == nil then
		return;
	end
	eye.uiManager:Close( "MapNationHeroDlg" );
	m_recvValue = nil;
end

-- 删除界面
function MapNationHeroDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
	m_recvValue = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function MapNationHeroDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            MapNationHeroDlgClose();
		elseif nControlID == 1 then
			MapNationHeroDlgBattle()
        end
	end
end

-- 载入时调用
function MapNationHeroDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiShape = objs[0];
	m_uiName = objs[1];
	m_uiPos = objs[2];
	m_uiNation = objs[3];
	m_uiAwardGrid = objs[4];
	m_uiMarchTime = objs[5];
	m_uiColor = objs[6];
	m_uiCorps = objs[7];
end

-- 界面初始化时调用
function MapNationHeroDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function MapNationHeroDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function MapNationHeroDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function MapNationHeroDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function MapNationHeroDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function MapNationHeroDlgShow( recvValue )
	MapNationHeroDlgOpen()
	for i = 0 ,m_uiAwardGrid.transform.childCount - 1 do
		local objs = m_uiAwardGrid.transform:GetChild(i).gameObject
		SetFalse( objs );
    end
	
	m_recvValue = recvValue;
	m_unit_index = WorldMap.m_nLastTouchUnitIndex;
	local posx 		= recvValue.m_posx;
	local posy 		= recvValue.m_posy;
	local kind		= recvValue.m_short_value[1];
	local level 	= recvValue.m_short_value[2];
	
	-- 获取奖励
	if m_awardCache[kind] == nil then
		system_askinfo( ASKINFO_NATIONHERO, "", 2, kind );
	else
		MapNationHeroDlgRecvAward( m_awardCache[kind] )
	end
	
	-- 形象
	SetImage( m_uiShape, HeroHeadSprite( kind ) )
	SetImage( m_uiCorps, CorpsSprite( g_heroinfo[kind][5].corps ) )
	-- 名字
	SetText( m_uiName, "Lv."..level.." "..Nation( g_nation_heroinfo[kind].nation ).."·"..HeroName(kind) )
	-- 位置
	SetText( m_uiPos, F(2343, posx, posy) ) 
	-- 所属
	SetText( m_uiNation, T(1211).." "..T( 1884 + g_nation_heroinfo[kind].nation ) ) 
	
	-- 行军时间
	local marchTime = WorldMap.MarchTime( WorldMap.m_nMyCityPosx, WorldMap.m_nMyCityPosy, posx, posy )
	SetText( m_uiMarchTime, F(953, secnum(marchTime) ) )
end

-- 获取奖励
-- m_count=0,m_list={m_kind=0,m_num=0,[m_count]},m_callback_code=0,m_value
function MapNationHeroDlgRecvAward( recvValue )
	m_awardCache[recvValue.m_value] = recvValue;
	local count = math.min( recvValue.m_count, m_uiAwardGrid.transform.childCount );
	for i = 0 ,count - 1 do
		local uiObj = m_uiAwardGrid.transform:GetChild(i).gameObject
		SetTrue( uiObj );
		local sprite, color, name = AwardInfo( recvValue.m_list[i+1].m_kind )
		SetImage( uiObj.transform:Find("Shape"), sprite );
		SetImage( uiObj.transform:Find("Color"), color );
		SetText( uiObj.transform:Find("Name"), name );
    end
	
end

-- 选择武将界面
function MapNationHeroDlgBattle()
	-- 不在同一区域
	if map_zone_checksame( WorldMap.m_nMyCityPosx, WorldMap.m_nMyCityPosy, m_recvValue.m_posx, m_recvValue.m_posy ) == 0 then
		AlertMsg(T(1322))
		MapNationHeroDlgClose();
		return
	end
		
	MapBattleDlgShow( m_recvValue, ARMY_ACTION_FIGHT, -1 )
	MapNationHeroDlgClose();
end
