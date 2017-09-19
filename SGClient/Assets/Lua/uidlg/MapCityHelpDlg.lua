-- 界面
local m_Dlg = nil;
local m_uiShape = nil; --UnityEngine.GameObject
local m_uiName = nil; --UnityEngine.GameObject
local m_uiOwn = nil; --UnityEngine.GameObject
local m_uiSpace = nil; --UnityEngine.GameObject
local m_uiSoldiers = nil; --UnityEngine.GameObject
local m_uiNation = nil; --UnityEngine.GameObject
local m_uiScroll = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiUIP_Army = nil; --UnityEngine.GameObject

local m_ObjectPool = nil;
local m_recvValue = nil;

-- 打开界面
function MapCityHelpDlgOpen()
	m_Dlg = eye.uiManager:Open( "MapCityHelpDlg" );
end

-- 隐藏界面
function MapCityHelpDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "MapCityHelpDlg" );
end

-- 删除界面
function MapCityHelpDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function MapCityHelpDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            MapCityHelpDlgClose();
		elseif nControlID == 1 then
			MapCityHelpDlgHelp();
		elseif nControlID >= 1000 then
			MapCityHelpDlgReturn( nControlID-1000 )
        end
	end
end

-- 载入时调用
function MapCityHelpDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiShape = objs[0];
	m_uiName = objs[1];
	m_uiOwn = objs[2];
	m_uiSpace = objs[3];
	m_uiSoldiers = objs[4];
	m_uiNation = objs[5];
	m_uiScroll = objs[6];
	m_uiContent = objs[7];
	m_uiUIP_Army = objs[8];
	
	-- 对象池
	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );
	m_ObjectPool:CreatePool("UIP_Army", 5, 5, m_uiUIP_Army);
end

-- 界面初始化时调用
function MapCityHelpDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function MapCityHelpDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function MapCityHelpDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function MapCityHelpDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function MapCityHelpDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function MapCityHelpDlgShow( recvValue )
	MapCityHelpDlgOpen()
	MapCityHelpDlgClear()
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
    SetImage( m_uiShape, LoadSprite( MapUnitCityShapeList[level].."_"..nation ) );
			
	-- 名字
	SetText( m_uiName, F(1228, level, T(995), posx, posy) )
	SetText( m_uiOwn, F(1232, Nation(nation), name ) )
	
	-- 国家
	SetImage( m_uiNation, NationSprite(nation) );
	
	-- 获取列表
	system_askinfo( ASKINFO_CITYHELP, "", 0, unit_index );
end

-- 清空单元
function MapCityHelpDlgClear()
	local objs = {};
	for i = 0 ,m_uiContent.transform.childCount - 1 do
		table.insert( objs, m_uiContent.transform:GetChild(i).gameObject )
    end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "UIP_Army(Clone)" then
			m_ObjectPool:Release( "UIP_Army", obj );
		end
    end
end

-- 接收信息
-- m_count=0,m_list={m_level=0,m_actorname_length=0,m_actorname="[m_actorname_length]",m_herokind=0,m_soldiers=0,m_actorid=0,m_army_index=0,[m_count]},
function MapCityHelpDlgRecv( recvValue )
	local total = 0;
	for i=1, recvValue.m_count, 1 do
		MapCityHelpDlgCreateArmy( recvValue.m_list[i] )
		total = total + recvValue.m_list[i].m_soldiers;
	end
	-- 总部队
	SetText( m_uiSpace, T(1233)..":"..recvValue.m_count.."/"..recvValue.m_walllevel );
	-- 总兵力
	SetText( m_uiSoldiers, T(1245)..":"..total );
end
function MapCityHelpDlgCreateArmy( info )
	local uiObj = m_ObjectPool:Get( "UIP_Army" );
	uiObj.transform:SetParent( m_uiContent.transform );
	local objs = uiObj:GetComponent( typeof(Reference) ).relatedGameObject;
	uiLevel = objs[0];
	uiActorName = objs[1];
	uiHeroName = objs[2];
	uiSoldiers = objs[3];
	uiButton = objs[4];
	SetText( uiLevel, info.m_level );
	SetText( uiActorName, info.m_actorname );
	SetText( uiHeroName, HeroName(info.m_herokind) );
	SetText( uiSoldiers, info.m_soldiers );
	if info.m_actorid == GetPlayer().m_actorid then
		SetTrue( uiButton );
		SetControlID( uiButton, 1000+info.m_army_index )
	else
		SetFalse( uiButton );
		SetControlID( uiButton, 0 )
	end
end

-- 撤防
function MapCityHelpDlgReturn( army_index )
	-- 已达目的点的撤回
	MsgBox( T( 1214 ),function()
		system_askinfo( ASKINFO_WORLDMAP, "", 3, army_index );
		MapCityHelpDlgClose()
	end )
end

-- 协防
function MapCityHelpDlgHelp()
	MapCityHelpDlgClose()
	MapBattleDlgShow( m_recvValue, ARMY_ACTION_HELP_TROOP )
end

