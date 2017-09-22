-- 界面
local m_Dlg = nil;
local m_uiNationFight = nil; --UnityEngine.GameObject
local m_uiHeroState = nil; --UnityEngine.GameObject
local m_uiAward = nil; --UnityEngine.GameObject
local m_uiLeftButton = nil; --UnityEngine.GameObject
local m_uiHeroLayer = nil; --UnityEngine.GameObject
local m_uiNationLayer = nil; --UnityEngine.GameObject
local m_uiHeroLayerGrid = nil; --UnityEngine.GameObject
local CONTROLOFFSET_REBACK = 10000000
local CONTROLOFFSET_QUICK = 20000000
local CONTROLOFFSET_GPS = 30000000
local m_recvValue = {}

-- 打开界面
function MapMainDlgOpen()
	m_Dlg = eye.uiManager:Open( "MapMainDlg" );
end

-- 隐藏界面
function MapMainDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "MapMainDlg" );
end

-- 删除界面
function MapMainDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function MapMainDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            MapMainDlgClose();
		
		-- 国战
		elseif nControlID == 1 then
		
		-- 武将状态
		elseif nControlID == 2 then
			MapMainDlgHeroLayerShow();
			
		-- 掉落
		elseif nControlID == 3 then
		
		-- 武将状态信息收起
		elseif nControlID == 4 then
			MapMainDlgHeroLayerHide();
		
		-- 坐标搜索
		elseif nControlID == 5 then
			MapPosDlgShow()
			
		-- 我的城池
		elseif nControlID == 6 then
			WorldMap.GotoMyCity()
		
		-- 撤回
		elseif nControlID >= CONTROLOFFSET_REBACK and nControlID < CONTROLOFFSET_QUICK then
			MapMainDlgReback( nControlID-CONTROLOFFSET_REBACK )
			
		-- 加速
		elseif nControlID >= CONTROLOFFSET_QUICK and nControlID < CONTROLOFFSET_GPS then
			MapMainDlgQuick( nControlID-CONTROLOFFSET_QUICK )
			
		-- 定位
		elseif nControlID >= CONTROLOFFSET_GPS then
			MapMainDlgGps( nControlID-CONTROLOFFSET_GPS )
			
        end
	end
end

-- 载入时调用
function MapMainDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiNationFight = objs[0];
	m_uiHeroState = objs[1];
	m_uiAward = objs[2];
	m_uiLeftButton = objs[3];
	m_uiHeroLayer = objs[4];
	m_uiNationLayer = objs[5];
	m_uiHeroLayerGrid = objs[6];
end

-- 界面初始化时调用
function MapMainDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function MapMainDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function MapMainDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function MapMainDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function MapMainDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function MapMainDlgShow()
	MapMainDlgOpen()
	for i = 0, 3, 1 do
		local UIP_BattleInfo = m_uiHeroLayerGrid.transform:GetChild(i).gameObject
		SetFalse( UIP_BattleInfo )
	end
	-- 请求出征队列
	system_askinfo( ASKINFO_WORLDMAP, "", 2 );
end

-- 接收出征队列信息
-- m_count=0,m_list={m_army_index=0,m_unit_index=0,m_state=0,m_statetime=0,m_stateduration=0,m_action=0,m_to_posx=0,m_to_posy=0,m_to_type=0,m_herokind={[4]},[m_count]},m_unit_index=0,
function MapMainDlgBattleRecv( recvValue )
	m_recvValue = recvValue;
	
	local index = 0;
	for i=1, m_recvValue.m_count, 1 do
		-- 这里要检查，如果是财署武将
		MapMainDlgBattleSet( m_uiHeroLayerGrid, index, m_recvValue.m_list[i] )
		index = index + 1;
	end
	
	local HeroList = {};
	-- 未出阵的
	for i=0,3,1 do
		if GetHero().m_CityHero[i].m_kind > 0 and GetHero().m_CityHero[i].m_state == 0 then
			table.insert( HeroList, clone(GetHero().m_CityHero[i]) )
		end
	end
	-- 无
	for i=0,3,1 do
		if GetHero().m_CityHero[i].m_kind <= 0 then
			table.insert( HeroList, clone(GetHero().m_CityHero[i]) )
		end
	end
	
	-- 空闲武将
	for i=1,#HeroList,1 do
		MapMainDlgHero( m_uiHeroLayerGrid, index, HeroList[i] );
		index = index + 1;
	end
end

-- 更新
function MapMainDlgBattleUpdate( recvValue )
	for i=1, m_recvValue.m_count, 1 do
		if m_recvValue.m_list[i].m_army_index == recvValue.m_army_index then
			m_recvValue.m_list[i] = recvValue;
		end
	end
	MapMainDlgBattleRecv( m_recvValue )
end

-- 设置出征队列
function MapMainDlgBattleSet( root, index, info )
	local UIP_BattleInfo = root.transform:GetChild(index).gameObject
	local objs = UIP_BattleInfo.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiState = objs[0];
	local uiTimer = objs[1];
	local uiRebackBtn = objs[2];
	local uiQuickBtn = objs[3];
	local uiGpsBtn = objs[4];
	local uiIdle = objs[5];
	local uiHero1 = objs[6];
	local uiHero2 = objs[7];
	local uiHero3 = objs[8];
	local uiHero4 = objs[9];
	SetTrue( UIP_BattleInfo )
	SetTrue( uiState )
	SetTrue( uiTimer )
	SetFalse( uiRebackBtn )
	SetFalse( uiQuickBtn )
	SetFalse( uiGpsBtn )
	SetFalse( uiIdle )
	SetControlID( uiRebackBtn, CONTROLOFFSET_REBACK+info.m_army_index )
	SetControlID( uiQuickBtn, CONTROLOFFSET_QUICK+info.m_army_index )
	SetControlID( uiGpsBtn, CONTROLOFFSET_GPS+info.m_army_index )
	
	-- 状态名称
	if info.m_state == ARMY_STATE_MARCH or info.m_state == ARMY_STATE_GROUP_END then
		if info.m_action == ARMY_ACTION_FIGHT or info.m_action == ARMY_ACTION_GROUP_CREATE or info.m_action == ARMY_ACTION_GROUP_ATTACK or info.m_action == ARMY_ACTION_GROUP_DEFENSE then
			if info.m_to_type == MAPUNIT_TYPE_CITY then
				SetText( uiState, T(975) )
			elseif info.m_to_type == MAPUNIT_TYPE_ARMY then
				SetText( uiState, T(974) )
				SetTrue( uiQuickBtn )
			elseif info.m_to_type == MAPUNIT_TYPE_TOWN then
				SetText( uiState, T(969) )
			elseif info.m_to_type == MAPUNIT_TYPE_ENEMY then
				SetText( uiState, T(974) )
				SetTrue( uiQuickBtn )
			elseif info.m_to_type == MAPUNIT_TYPE_RES then
				SetText( uiState, T(974) )
				SetTrue( uiQuickBtn )
			end
		elseif info.m_action == ARMY_ACTION_GATHER then
			SetText( uiState, T(976) )
			SetTrue( uiQuickBtn )
		elseif info.m_action == ARMY_ACTION_HELP_TROOP then
			SetText( uiState, T(977) )
			SetTrue( uiQuickBtn )
		end
		SetTrue( uiRebackBtn )
		if info.m_state == ARMY_STATE_GROUP_END then
			SetTrue( uiGpsBtn )	
		end
	elseif info.m_state == ARMY_STATE_GATHER then	
		SetText( uiState, T(976) )
		SetTrue( uiRebackBtn )
		SetTrue( uiGpsBtn )
	elseif info.m_state == ARMY_STATE_HELP then
		SetText( uiState, T(977) )
		SetTrue( uiRebackBtn )
		SetTrue( uiGpsBtn )
	elseif info.m_state == ARMY_STATE_REBACK then
		SetText( uiState, T(978) )
		SetTrue( uiQuickBtn )
	end
	
	-- 状态时间
	SetTimer( uiTimer, info.m_stateduration-info.m_statetime, info.m_stateduration )
	
	-- 武将头像
	SetFalse( uiHero1 )
	SetFalse( uiHero2 )
	SetFalse( uiHero3 )
	SetFalse( uiHero4 )
	
	local herolist = {};
	for i=1, 4, 1 do
		if info.m_herokind[i] > 0 then
			table.insert( herolist, info.m_herokind[i] );
		end
	end
	
	if #herolist == 1 then
		SetTrue( uiHero1 )
		SetImage( uiHero1.transform:GetChild(0), HeroHeadSprite( herolist[1] ) )
	elseif #herolist == 2 then
		SetTrue( uiHero2 )
		SetImage( uiHero2.transform:GetChild(0), HeroHeadSprite( herolist[1] ) )
		SetImage( uiHero2.transform:GetChild(1), HeroHeadSprite( herolist[2] ) )
	elseif #herolist == 3 then
		SetTrue( uiHero3 )
		SetImage( uiHero3.transform:GetChild(0), HeroHeadSprite( herolist[1] ) )
		SetImage( uiHero3.transform:GetChild(1), HeroHeadSprite( herolist[2] ) )
		SetImage( uiHero3.transform:GetChild(2), HeroHeadSprite( herolist[3] ) )
	elseif #herolist == 4 then
		SetTrue( uiHero4 )
		SetImage( uiHero4.transform:GetChild(0), HeroHeadSprite( herolist[1] ) )
		SetImage( uiHero4.transform:GetChild(1), HeroHeadSprite( herolist[2] ) )
		SetImage( uiHero4.transform:GetChild(2), HeroHeadSprite( herolist[3] ) )
		SetImage( uiHero4.transform:GetChild(3), HeroHeadSprite( herolist[4] ) )
	end
end

-- 空闲武将信息
function MapMainDlgHero( root, index, pHero )
	local UIP_BattleInfo = root.transform:GetChild(index).gameObject
	if pHero.m_kind <= 0 then
		SetFalse( UIP_BattleInfo )
		return
	end

	SetTrue( UIP_BattleInfo )
	local objs = UIP_BattleInfo.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiState = objs[0];
	local uiTimer = objs[1];
	local uiRebackBtn = objs[2];
	local uiQuickBtn = objs[3];
	local uiGpsBtn = objs[4];
	local uiIdle = objs[5];
	local uiHero1 = objs[6];
	local uiHero2 = objs[7];
	local uiHero3 = objs[8];
	local uiHero4 = objs[9];
	
	-- 空闲状态
	if pHero.m_state == 0 then
		SetFalse( uiState )
		SetFalse( uiTimer )
		SetFalse( uiRebackBtn )
		SetFalse( uiQuickBtn )
		SetFalse( uiGpsBtn )
		SetTrue( uiIdle )
		SetTrue( uiHero1 )
		SetFalse( uiHero2 )
		SetFalse( uiHero3 )
		SetFalse( uiHero4 )
		SetImage( uiHero1.transform:GetChild(0), HeroHeadSprite( pHero.m_kind ) )
	end
end

-- 显示武将层
function MapMainDlgHeroLayerShow()
	m_uiLeftButton.transform:GetComponent( "UITweenRectPosition" ):Play( true );
	m_uiHeroLayer.transform:GetComponent( "UITweenRectPosition" ):Play( true );
end

-- 显示武将层
function MapMainDlgHeroLayerHide()
	m_uiLeftButton.transform:GetComponent( "UITweenRectPosition" ):Play( false );
	m_uiHeroLayer.transform:GetComponent( "UITweenRectPosition" ):Play( false );
end

-- 撤回
function MapMainDlgReback( army_index )
	local value = nil
	for i=1, m_recvValue.m_count, 1 do
		if m_recvValue.m_list[i].m_army_index == army_index then
			value = m_recvValue.m_list[i];
			break;
		end
	end
	if value == nil then
		return
	end
	
	if value.m_state == ARMY_STATE_MARCH then
		ItemUseDlgShow( 1, army_index )
	else
		-- 已达目的点的撤回
		MsgBox( T( 1214 ),function()
			system_askinfo( ASKINFO_WORLDMAP, "", 3, army_index );
		end )
	end
end
		
-- 加速
function MapMainDlgQuick( army_index )
	ItemUseDlgShow( 2, army_index )
end
		
-- 定位
function MapMainDlgGps( army_index )
	for i=1, m_recvValue.m_count, 1 do
		if m_recvValue.m_list[i].m_army_index == army_index then
			WorldMap.GotoCoor( m_recvValue.m_list[i].m_to_posx, m_recvValue.m_list[i].m_to_posy )
		end
	end
end
