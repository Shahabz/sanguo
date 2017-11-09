-- 界面
local m_Dlg = nil;
local m_uiNationFight = nil; --UnityEngine.GameObject
local m_uiHeroState = nil; --UnityEngine.GameObject
local m_uiAward = nil; --UnityEngine.GameObject
local m_uiLeftButton = nil; --UnityEngine.GameObject
local m_uiHeroLayer = nil; --UnityEngine.GameObject
local m_uiNationLayer = nil; --UnityEngine.GameObject
local m_uiHeroLayerGrid = nil; --UnityEngine.GameObject
local m_uiNationLayerScroll = nil; --UnityEngine.GameObject
local m_uiNationLayerContent = nil; --UnityEngine.GameObject
local m_uiUIP_TownInfo = nil; --UnityEngine.GameObject
local m_uiWorldQuest = nil; --UnityEngine.GameObject
local m_uiGotoZone = nil; --UnityEngine.GameObject
local m_uiActivity1 = nil; --UnityEngine.GameObject
local m_uiActivity2 = nil; --UnityEngine.GameObject

local m_ObjectPool = nil;
local CONTROLOFFSET_REBACK = 10000000
local CONTROLOFFSET_QUICK = 20000000
local CONTROLOFFSET_GPS = 30000000
local CONTROLOFFSET_TOWNID = 40000000
local m_recvValue = {}
local m_WorldQuestCache = {}
local m_bHeroLayerShow = false;
local m_bNationLayerShow = false;

-- 打开界面
function MapMainDlgOpen()
	m_Dlg = eye.uiManager:Open( "MapMainDlg" );
end

-- 隐藏界面
function MapMainDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	m_uiLeftButton.transform:GetComponent( "UITweenRectPosition" ):ToInit();
	m_uiHeroLayer.transform:GetComponent( "UITweenRectPosition" ):ToInit();
	m_bHeroLayerShow = false;
	
	m_uiLeftButton.transform:GetComponent( "UITweenRectPosition" ):ToInit();
	m_uiNationLayer.transform:GetComponent( "UITweenRectPosition" ):ToInit();
	m_bNationLayerShow = false
	
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
			MapMainDlgNationLayerShow();
			
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
		
		-- 国战状态信息收起
		elseif nControlID == 7 then
			MapMainDlgNationLayerHide();
		
		-- 世界任务
		elseif nControlID == 10 then
			MapMainDlgClickWorldMap()
		
		-- 前往州城
		elseif nControlID == 11 then
			MapMainDlgClickGotoZone()	
		
		-- 血战皇城
		elseif nControlID == 12 then
			BloodyBattleDlgShow();
		
		-- 挖宝
		elseif nControlID == 13 then
		
			
		-- 恭贺
		elseif nControlID == 14 then

				
		-- 撤回
		elseif nControlID >= CONTROLOFFSET_REBACK and nControlID < CONTROLOFFSET_QUICK then
			MapMainDlgReback( nControlID-CONTROLOFFSET_REBACK )
			
		-- 加速
		elseif nControlID >= CONTROLOFFSET_QUICK and nControlID < CONTROLOFFSET_GPS then
			MapMainDlgQuick( nControlID-CONTROLOFFSET_QUICK )
			
		-- 定位
		elseif nControlID >= CONTROLOFFSET_GPS and nControlID < CONTROLOFFSET_TOWNID then
			MapMainDlgGps( nControlID-CONTROLOFFSET_GPS )
		
		-- 城镇id
		elseif nControlID >= CONTROLOFFSET_TOWNID then
			MapMainDlgNationGoto( nControlID-CONTROLOFFSET_TOWNID )
        end
	elseif nType == UI_EVENT_TIMECOUNTEND then
		if nControlID == 1 then
			
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
	m_uiNationLayerScroll = objs[7];
	m_uiNationLayerContent = objs[8];
	m_uiUIP_TownInfo = objs[9];
	m_uiWorldQuest = objs[10];
	m_uiGotoZone = objs[11];
	m_uiActivity1 = objs[12];
	m_uiActivity2 = objs[13];
	
	-- 对象池
	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );
	m_ObjectPool:CreatePool("UIP_TownInfo", 4, 4, m_uiUIP_TownInfo);
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
	SetFalse( m_uiWorldQuest )
	SetFalse( m_uiGotoZone )
	SetFalse( m_uiActivity1 )
	SetFalse( m_uiActivity2 )
	for i = 0, 3, 1 do
		local UIP_BattleInfo = m_uiHeroLayerGrid.transform:GetChild(i).gameObject
		SetFalse( UIP_BattleInfo )
	end
	-- 请求出征队列
	system_askinfo( ASKINFO_WORLDMAP, "", 2 );
	-- 请求世界任务
	system_askinfo( ASKINFO_QUEST, "", 10 );
end

-- 接收出征队列信息
-- m_count=0,m_list={m_army_index=0,m_unit_index=0,m_state=0,m_statetime=0,m_stateduration=0,m_action=0,m_to_posx=0,m_to_posy=0,m_to_type=0,m_herokind={[4]},[m_count]},m_unit_index=0,
function MapMainDlgBattleRecv( recvValue )
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	
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
	elseif info.m_state >= ARMY_STATE_KINGWAR_READY and info.m_state <= ARMY_STATE_KINGWAR_DEAD then
		SetText( uiState, T(1400) )
	end
	
	-- 状态时间
	if info.m_state >= ARMY_STATE_KINGWAR_READY and info.m_state <= ARMY_STATE_KINGWAR_DEAD then
		SetFalse( uiTimer )
	else
		SetTrue( uiTimer )
		SetTimer( uiTimer, info.m_stateduration-info.m_statetime, info.m_stateduration )
	end
	
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

function MapMainDlgGetRecvValue()
	return m_recvValue;
end

-- 显示武将层
function MapMainDlgHeroLayerShow()
	m_uiLeftButton.transform:GetComponent( "UITweenRectPosition" ):Play( true );
	m_uiHeroLayer.transform:GetComponent( "UITweenRectPosition" ):Play( true );
	m_bHeroLayerShow = true;
end

-- 隐藏武将层
function MapMainDlgHeroLayerHide()
	m_uiLeftButton.transform:GetComponent( "UITweenRectPosition" ):Play( false );
	m_uiHeroLayer.transform:GetComponent( "UITweenRectPosition" ):Play( false );
	m_bHeroLayerShow = false;
end

-- 显示国战层
function MapMainDlgNationLayerShow()
	m_uiLeftButton.transform:GetComponent( "UITweenRectPosition" ):Play( true );
	m_uiNationLayer.transform:GetComponent( "UITweenRectPosition" ):Play( true );
	m_bNationLayerShow = true
	-- 国战信息
	MapMainDlgNationLayerClear()
	system_askinfo( ASKINFO_NATIONARMYGROUP, "", 2 );
end

-- 隐藏国战层
function MapMainDlgNationLayerHide()
	m_uiLeftButton.transform:GetComponent( "UITweenRectPosition" ):Play( false );
	m_uiNationLayer.transform:GetComponent( "UITweenRectPosition" ):Play( false );
	m_bNationLayerShow = false
end

-- 清空
function MapMainDlgNationLayerClear()
	local objs = {};
	for i = 0 ,m_uiNationLayerContent.transform.childCount - 1 do
		table.insert( objs, m_uiNationLayerContent.transform:GetChild(i).gameObject )
    end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "UIP_TownInfo(Clone)" then
			m_ObjectPool:Release( "UIP_TownInfo", obj );
		end
    end
end

-- 排序
function MapMainDlgNationLayerSort( a, b )
	if a.m_statetime > b.m_statetime then
		return true
	else
		return false
	end
end

-- 设置国战信息
-- m_count=0,m_list={m_townid=0,m_statetime=0,m_attack=0,m_nation,[m_count]},
function MapMainDlgNationLayerRecv( recvValue )
	if recvValue.m_count == 0 then
		return
	end
	local tmpCache = {}
	for i=1, recvValue.m_count, 1 do
		table.insert( tmpCache, { m_townid=recvValue.m_list[i].m_townid, m_statetime=recvValue.m_list[i].m_statetime, m_attack=recvValue.m_list[i].m_attack, m_nation=recvValue.m_list[i].m_nation } )
	end
	table.sort( tmpCache, MapMainDlgNationLayerSort )
	
	for i=1, #tmpCache, 1 do
		local uiObj = m_ObjectPool:Get( "UIP_TownInfo" );
		uiObj.transform:SetParent( m_uiNationLayerContent.transform );
		local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
		local uiState = objs[0];
		local uiShape = objs[1];
		local uiName = objs[2];
		local uiTimer = objs[3];
		local townid = tmpCache[i].m_townid;
		local nation = 	tmpCache[i].m_nation;
		SetControlID( uiObj, CONTROLOFFSET_TOWNID+townid )
		-- 形象
		local type 	= g_towninfo[townid].type
		SetImage( uiShape, LoadSprite( MapUnitTownShapeList[type].."_"..nation ) )	
		-- 名称
		SetText( uiName, MapTownName( townid ), Hex2Color(MapUnitRangeColor[nation]) )
		-- 倒计时
		SetTimer( uiTimer, tmpCache[i].m_statetime, tmpCache[i].m_statetime, 1 )
		-- 攻守
		if tmpCache[i].m_attack == 1 then
			SetText( uiState, T(1264), Hex2Color(0xffde00ff) )
		elseif tmpCache[i].m_attack == 2 then
			SetText( uiState, T(1265), Hex2Color(0x03de27ff) )
		end
	end
end

-- 更新
function MapMainDlgNationLayerUpdate()
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	if m_bNationLayerShow == true then
		MapMainDlgNationLayerClear()
		system_askinfo( ASKINFO_NATIONARMYGROUP, "", 2 );
	end
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

-- 城镇定位
function MapMainDlgNationGoto( townid )
	if g_towninfo[townid] == nil then
		return
	end
	local posx 	= g_towninfo[townid].posx
	local posy 	= g_towninfo[townid].posy
	WorldMap.GotoCoor( posx, posy )
end

-- 设置世界任务
local g_worldmapinfo = { 
[1] = { pic="ui_mapmain_quest_1" },
[2] = { pic="ui_mapmain_quest_1" },
[3] = { pic="ui_mapmain_quest_1" },
[4] = { pic="ui_mapmain_quest_4" },
[5] = { pic="ui_mapmain_quest_4" },
[6] = { pic="ui_mapmain_quest_4" },
[7] = { pic="ui_mapmain_quest_7" },
[8] = { pic="ui_mapmain_quest_8" },
[9] = { pic="ui_mapmain_quest_8" },
[10] = { pic="ui_mapmain_quest_10" },
}
-- m_questid=0,m_value=0,m_maxvalue=0,m_complete=0,
function MapMainDlgSetWorldQuest( recvValue )
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	m_WorldQuestCache = recvValue;
	local questid = recvValue.m_questid
	if questid == 0 then
		SetFalse( m_uiWorldQuest );		
		return;
	end
	SetTrue( m_uiWorldQuest );
	-- 任务图
	SetImage( m_uiWorldQuest.transform:Find("Back/Pic"), LoadSprite(g_worldmapinfo[questid].pic) ); 
	-- 任务标题
	SetText( m_uiWorldQuest.transform:Find("Back/Title"), WorldQuestName(questid) ); 
	-- 任务进度或描述
	local str = "";
	if recvValue.m_complete == 1 then
		str = T(1348).."("..T(1349)..")";
	else
		if questid == 1 or questid == 2 or questid == 3 then
			str = T(1348)..recvValue.m_value.."/"..recvValue.m_maxvalue
		elseif questid == 4 or questid == 5 or questid == 6 or questid == 8 or questid == 9 then
			str = T(1348)..recvValue.m_value.."/1"
		else
			str = Localization.text_quest( 2050+questid )
		end
	end 
	SetText( m_uiWorldQuest.transform:Find("Back/Progress"), str );
end

-- 世界任务
function MapMainDlgClickWorldMap()
	local questid = m_WorldQuestCache.m_questid
	if questid == 7 then
		WorldBossDlgShow( 1, questid )
	elseif questid == 10 then
		WorldBossDlgShow( 2, questid )
	else
		WorldQuestInfoDlgShow( m_WorldQuestCache )
	end
end

-- 前往州城通知
function MapMainDlgShowGotoZone()
	SetTrue( m_uiGotoZone )
end
function MapMainDlgHideGotoZone()
	SetFalse( m_uiGotoZone )
end
function MapMainDlgClickGotoZone()
	NpcAlert( T(1370), T(1371), T(1372), T(1373), function()
		system_askinfo( ASKINFO_QUEST, "", 15 );
	end )
end	

-- 血战皇城活动
-- m_state=0,m_beginstamp=0,m_endstamp=0,m_nation=0,
function MapMainDlgActivityKingWar( recvValue )
	BloodyBattleDlgSetActivityInfo( recvValue )
	-- 尚未开启
	if recvValue.m_state == 0 and recvValue.m_beginstamp > 0 then
		SetTrue( m_uiActivity1 )
		local lefttime = recvValue.m_beginstamp-GetServerTime();
		SetTimer( m_uiActivity1.transform:Find("Back/Timer"), lefttime, lefttime, 0, T(1393) );
	
	-- 已经开启
	elseif recvValue.m_state == 1 then
		SetTrue( m_uiActivity1 )
		local lefttime = recvValue.m_endstamp-GetServerTime();
		if recvValue.m_nation == GetPlayer().m_nation then
			SetTimer( m_uiActivity1.transform:Find("Back/Timer"), lefttime, lefttime, 0, T(1394) );
		else
			SetTimer( m_uiActivity1.transform:Find("Back/Timer"), lefttime, lefttime, 0, T(1395) );
		end
	else
		SetFalse( m_uiActivity1 )
		BloodyBattleDlgClose()
	end
end

-- 小地图
function MainDlgMiniMapInit()
	for j=1, 5, 1 do
		for i=1, 5, 1 do
			local tmx = GameObject.Instantiate( m_uiMiniMap.m_uiUIP_MiniTmx );
			tmx.transform:SetParent( m_uiMiniMap.m_uiMoveLayer.transform );
			tmx.transform.localScale = Vector3.one;
			tmx.transform.localPosition = Vector3.New( (WorldMap.m_nMaxWidth*1024)/2/100 + (i-1)*TMX_WIDTH*1024/2/100 - (j-1)*TMX_WIDTH*1024/2/100,
												   -(i-1)*TMX_HEIGHT*512/2/100 - (j-1)*TMX_HEIGHT*512/2/100, 0 );
			SetTrue( tmx )
		end
	end
	-- 对象池
	m_uiMiniMap.m_ObjectPool = m_uiMiniMap.m_uiMoveLayer.transform.parent:GetComponent( typeof(ObjectPoolManager) );
	m_uiMiniMap.m_ObjectPool:CreatePool("m_uiUIP_ZoneUnit", 100, 100, m_uiMiniMap.m_uiUIP_ZoneUnit);
end
function MapMainDlgMiniMapChangeZone()
	--MainDlgMiniMapClearUnit()
	--m_uiMoveLayer
	--m_uiMiniMap.m_uiMoveLayer.transform:Find( "MapBorder" ):GetComponent("MapBorder"):SetSize( 110 );
end
function MapMainDlgMiniMapMove()

end
function MapMainDlgMiniMapClearUnit()
	local objs = {};
	for i = 0 ,m_uiMiniMap.m_uiMoveLayer.transform.childCount - 1 do
		table.insert( objs, m_uiMiniMap.m_uiMoveLayer.transform:GetChild(i).gameObject )
    end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "m_uiUIP_ZoneUnit(Clone)" then
			m_ObjectPool:Release( "m_uiUIP_ZoneUnit", obj );
		end
    end
end
-- {m_posx=0,m_posy=0,m_nation=0,m_level=0,[m_count]}
function MapMainDlgMiniMapAddUnit( recvValue )
	--print( recvValue.m_posx..","..recvValue.m_posy )
	--local uiObj = m_uiMiniMap.m_ObjectPool:Get( "m_uiUIP_ZoneUnit" );
	--uiObj.transform:SetParent( m_uiMiniMap.m_uiMoveLayer.transform );
	--uiObj.transform.localScale = Vector3.one;
	--uiObj.transform.localPosition = Vector3.New( recvValue.m_posx, recvValue.m_posy )
end
