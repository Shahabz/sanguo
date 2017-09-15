-- 界面
local m_Dlg = nil;
local m_uiShape = nil; --UnityEngine.GameObject
local m_uiName = nil; --UnityEngine.GameObject
local m_uiMarchTime1 = nil; --UnityEngine.GameObject
local m_uiMarchTime2 = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiUIP_Type1 = nil; --UnityEngine.GameObject
local m_uiUIP_Type2 = nil; --UnityEngine.GameObject
local m_uiUIP_Type3 = nil; --UnityEngine.GameObject
local m_uiWarn = nil; --UnityEngine.GameObject

local m_recvValue = nil
-- 打开界面
function MapCityFightDlgOpen()
	m_Dlg = eye.uiManager:Open( "MapCityFightDlg" );
end

-- 隐藏界面
function MapCityFightDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "MapCityFightDlg" );
end

-- 删除界面
function MapCityFightDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function MapCityFightDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            MapCityFightDlgClose();
		elseif nControlID >= 1 and nControlID <= 3 then
			MapCityFightDlgSelect( nControlID );
        end
	end
end

-- 载入时调用
function MapCityFightDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiShape = objs[0];
	m_uiName = objs[1];
	m_uiMarchTime1 = objs[2];
	m_uiMarchTime2 = objs[3];
	m_uiContent = objs[4];
	m_uiUIP_Type1 = objs[5];
	m_uiUIP_Type2 = objs[6];
	m_uiUIP_Type3 = objs[7];
	m_uiWarn = objs[8];
end

-- 界面初始化时调用
function MapCityFightDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function MapCityFightDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function MapCityFightDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function MapCityFightDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function MapCityFightDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function MapCityFightDlgShow( recvValue )
	MapCityFightDlgOpen()
	m_recvValue 	= recvValue;
	local state 	= recvValue.m_state;
	local name 		= recvValue.m_name;
	local posx 		= recvValue.m_posx;
	local posy 		= recvValue.m_posy;
	local level 	= recvValue.m_char_value[1];
	local nation	= recvValue.m_char_value[2];
	local ptbuff	= recvValue.m_char_value[3];
	local unit_index = recvValue.m_unit_index;
	
	-- 形象
    m_uiShape:GetComponent("SpriteRenderer").sprite = LoadSprite( MapUnitCityShapeList[level].."_"..nation );
			
	-- 名字
	SetText( m_uiName, F(1228, level, name, posx, posy) )
	
	-- 行军时间
	m_marchtime = WorldMap.MarchTime( WorldMap.m_nMyCityPosx, WorldMap.m_nMyCityPosy, posx, posy )
	SetText( m_uiMarchTime1, F(953, secnum(m_marchtime) ) )
	SetText( m_uiMarchTime2, secnum(m_marchtime) )
end

-- 选择模式
function MapCityFightDlgSelect( type )
	MapCityFightDlgClose()
	MapBattleDlgShow( m_recvValue.m_unit_index, m_recvValue )
end

