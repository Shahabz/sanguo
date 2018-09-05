-- 界面
local m_Dlg = nil;
local m_uiShape = nil; --UnityEngine.GameObject
local m_uiName = nil; --UnityEngine.GameObject
local m_uiTalk = nil; --UnityEngine.GameObject
local m_uiAwardGrid = nil; --UnityEngine.GameObject
local m_uiMarchTime = nil; --UnityEngine.GameObject
local m_uiBattleButton = nil; --UnityEngine.GameObject

local m_unit_index = -1;
local m_recvValue = nil
local m_awardCache = {};

-- 打开界面
function MapActivityDlgOpen()
	m_Dlg = eye.uiManager:Open( "MapActivityDlg" );
end

-- 隐藏界面
function MapActivityDlgClose()
	if m_Dlg == nil then
		return;
	end
	eye.uiManager:Close( "MapActivityDlg" );
	m_recvValue = nil
end

-- 删除界面
function MapActivityDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
	m_recvValue = nil
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function MapActivityDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            MapActivityDlgClose();
		elseif nControlID == 1 then
			MapActivityDlgBattle()
        end
	end
end

-- 载入时调用
function MapActivityDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;
	m_uiShape = objs[0];
	m_uiName = objs[1];
	m_uiTalk = objs[2];
	m_uiAwardGrid = objs[3];
	m_uiMarchTime = objs[4];
	m_uiBattleButton = objs[5];
end

-- 界面初始化时调用
function MapActivityDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function MapActivityDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function MapActivityDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function MapActivityDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function MapActivityDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function MapActivityDlgShow( recvValue )
	MapActivityDlgOpen()
	for i = 0 ,m_uiAwardGrid.transform.childCount - 1 do
		local objs = m_uiAwardGrid.transform:GetChild(i).gameObject
		SetFalse( objs );
    end
	
	m_recvValue = recvValue;
	m_unit_index = WorldMap.m_nLastTouchUnitIndex;
	local posx 		= recvValue.m_posx;
	local posy 		= recvValue.m_posy;
	local kind		= recvValue.m_char_value[1];
	local level		= recvValue.m_char_value[1];
	local type 		= recvValue.m_char_value[1];
	local hp		= 0;
	local maxhp		= 0;
	
	-- 获取奖励
	if m_awardCache[kind] == nil then
		system_askinfo( ASKINFO_WORLDMAP, "", 16, kind );
	else
		MapActivityDlgRecvAward( m_awardCache[kind] )
	end
	SetText( m_uiName, T(MapUnitActivityNameList[kind]) )
			
	-- 西凉铁骑
	if kind == 1 then
		SetImage( m_uiShape, LoadSprite("enemyface_1") )
		SetTrue( m_uiBattleButton )
		SetTrue( m_uiMarchTime )
		SetText( m_uiTalk, T(4282) )
		
	-- 南蛮兵
	elseif kind == 2 then
		SetImage( m_uiShape, LoadSprite("enemyface_2") )
		SetFalse( m_uiBattleButton )
		SetFalse( m_uiMarchTime )
		SetText( m_uiTalk, T(4283) )
	end

	-- 行军时间
	local marchTime = WorldMap.MarchTime( WorldMap.m_nMyCityPosx, WorldMap.m_nMyCityPosy, posx, posy )
	SetText( m_uiMarchTime, F(953, secnum(marchTime) ) )
end

-- 获取奖励
-- m_count=0,m_list={m_kind=0,m_num=0,[m_count]},m_callback_code=0,m_value
function MapActivityDlgRecvAward( recvValue )
	m_awardCache[recvValue.m_value] = recvValue;
	local count = math.min( recvValue.m_count, m_uiAwardGrid.transform.childCount );
	for i = 0 ,count - 1 do
		local uiObj = m_uiAwardGrid.transform:GetChild(i).gameObject
		SetTrue( uiObj );
		local sprite, color, name,c = AwardInfo( recvValue.m_list[i+1].m_kind )
		SetImage( uiObj.transform:Find("Shape"), sprite );
		SetImage( uiObj.transform:Find("Color"), color );
		SetText( uiObj.transform:Find("Name"), name, NameColor(c) );
		if recvValue.m_list[i+1].m_num > 1 then
			SetText( uiObj.transform:Find("Num"), knum(recvValue.m_list[i+1].m_num) );
			SetTrue( uiObj.transform:Find("NumBack") )
		else
			SetText( uiObj.transform:Find("Num"), "" );
			SetFalse( uiObj.transform:Find("NumBack") )
		end
    end
	
end

-- 选择武将界面
function MapActivityDlgBattle()		
	MapBattleDlgShow( m_recvValue, ARMY_ACTION_FIGHT, -1 )
	MapActivityDlgClose();
end
