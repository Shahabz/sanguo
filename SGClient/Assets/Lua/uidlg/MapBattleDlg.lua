-- 界面
local m_Dlg = nil;
local m_uiShape = nil; --UnityEngine.GameObject
local m_uiName = nil; --UnityEngine.GameObject
local m_uiMarchTime1 = nil; --UnityEngine.GameObject
local m_uiMarchTime2 = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiCost = nil; --UnityEngine.GameObject
local m_uiTitle = nil; --UnityEngine.GameObject
local m_uiBattleButton = nil; --UnityEngine.GameObject
local m_uiAttackDesc = nil; --UnityEngine.GameObject
local m_uiSoldiersBtn = nil; --UnityEngine.GameObject

local m_action = 0;
local m_unit_index = -1;
local m_marchtime = 0;
local m_cost_food = 0;
local m_cost_body = 0;
local m_HeroList = {};
local m_recvValue = {};
local m_uiMoveLayerPlay = { false, false, false, false };
local m_appdata = 0;
local m_id = 0;
local m_group_index = -1;

-- 打开界面
function MapBattleDlgOpen()
	m_Dlg = eye.uiManager:Open( "MapBattleDlg" );
end

-- 隐藏界面
function MapBattleDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "MapBattleDlg" );
end

-- 删除界面
function MapBattleDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
	
	m_unit_index = -1;
	m_marchtime = 0;
	m_cost_food = 0;
	m_cost_body = 0;
	m_HeroList = {};
	m_recvValue = {};
	m_uiMoveLayerPlay = { false, false, false, false };
	m_appdata = 0;
	m_group_index = -1;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function MapBattleDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            MapBattleDlgClose();
		
		-- 点击选取
		elseif nControlID >= 1 and nControlID <= 4 then
			MapBattleDlgClick( nControlID );
			
		-- 补兵
		elseif nControlID >= 101 and nControlID <= 104 then
			MapBattleDlgSoldiers( nControlID-100 )
			
		-- 上移
		elseif nControlID >= 201 and nControlID <= 204 then
			MapBattleDlgUp( nControlID-200 )
			
		-- 帮助
		elseif nControlID == 1000 then
			HelpDlgShowBySystem( HELP_MapBattleDlg )
			
		-- 出击
		elseif nControlID == 1001 then
			MapBattleDlgBattle()
        end
	end
end

-- 载入时调用
function MapBattleDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiShape = objs[0];
	m_uiName = objs[1];
	m_uiMarchTime1 = objs[2];
	m_uiMarchTime2 = objs[3];
	m_uiContent = objs[4];
	m_uiCost = objs[5];
	m_uiTitle = objs[6];
	m_uiBattleButton = objs[7];
	m_uiAttackDesc = objs[8];
	m_uiSoldiersBtn = objs[9];
end

-- 界面初始化时调用
function MapBattleDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function MapBattleDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function MapBattleDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function MapBattleDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function MapBattleDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function MapBattleDlgShow( recvValue, action, group_index )
	MapBattleDlgOpen()
	SetTrue( m_uiCost )
	
	m_appdata = 0;
	m_id = 0;
	m_group_index = group_index;
	m_action = action;
	m_recvValue 	= recvValue;
	m_unit_index 	= recvValue.m_unit_index
	local posx 		= recvValue.m_posx;
	local posy 		= recvValue.m_posy;
	
	if recvValue.m_type == MAPUNIT_TYPE_CITY then-- 城池
		local state 	= recvValue.m_state;
		local name 		= recvValue.m_name;
		local level 	= recvValue.m_char_value[1];
		local nation	= recvValue.m_char_value[2];
		-- 标题
		if action == ARMY_ACTION_HELP_TROOP then
			SetFalse( m_uiAttackDesc )
			SetText( m_uiTitle.transform:Find("Text"), T(1231) )
			SetText( m_uiBattleButton.transform:Find("Back/Text"), T(1229) );
			
		elseif action == ARMY_ACTION_GROUP_CREATE then
			SetTrue( m_uiAttackDesc )
			SetText( m_uiTitle.transform:Find("Text"), T(1222) )
			SetText( m_uiBattleButton.transform:Find("Back/Text"), T(1254) );
		
		elseif action == ARMY_ACTION_GROUP_ATTACK then
			SetTrue( m_uiAttackDesc )
			SetText( m_uiTitle.transform:Find("Text"), T(1252) )
			SetText( m_uiBattleButton.transform:Find("Back/Text"), T(1252) );
			
		elseif action == ARMY_ACTION_GROUP_DEFENSE then
			SetFalse( m_uiAttackDesc )
			SetText( m_uiTitle.transform:Find("Text"), T(1253) )
			SetText( m_uiBattleButton.transform:Find("Back/Text"), T(1253) );
			
		else
			SetText( m_uiTitle.transform:Find("Text"), "" )
		end
		-- 形象
		SetImage( m_uiShape, LoadSprite( MapUnitCityShapeList[level].."_"..nation ) );
		-- 名字
		SetText( m_uiName, F(1228, level, name, posx, posy) )
		
	elseif recvValue.m_type == MAPUNIT_TYPE_TOWN then -- 城镇
		local posx 				= recvValue.m_posx;
		local posy 				= recvValue.m_posy;
		local custom_name		= recvValue.m_name;
		local custom_namelen	= recvValue.m_namelen;
		local nation 			= recvValue.m_char_value[1];
		local townid 			= recvValue.m_short_value[1];
		
		-- 标题
		if action == ARMY_ACTION_NATION_ATTACK then
			SetTrue( m_uiAttackDesc )
			SetText( m_uiTitle.transform:Find("Text"), T(1296) )
			SetText( m_uiBattleButton.transform:Find("Back/Text"), T(1296) );
		elseif action == ARMY_ACTION_NATION_DEFENSE then	
			SetFalse( m_uiAttackDesc )
			SetText( m_uiTitle.transform:Find("Text"), T(1297) )
			SetText( m_uiBattleButton.transform:Find("Back/Text"), T(1297) );
		else
			
		end
		
		-- 形象
		local type 	= g_towninfo[townid].type
		SetImage( m_uiShape, LoadSprite( MapUnitTownShapeList[type].."_"..nation ) )
		
		-- 名字
		SetText( m_uiName, F(3019, MapTownName( townid ), g_towninfo[townid].posx, g_towninfo[townid].posy) )
		
	elseif recvValue.m_type == MAPUNIT_TYPE_ENEMY then -- 流寇
		local level	= recvValue.m_char_value[1];
		local kind 	= recvValue.m_short_value[1];
		-- 标题
		SetText( m_uiTitle.transform:Find("Text"), T(954) )
		-- 形象
		SetImage( m_uiShape, EnemySprite( g_enemyinfo[kind].shape, 1 ) )
		-- 名字+位置
		if g_enemyinfo[kind].nameid > 0 then
			SetText( m_uiName, F(2388, level, T(g_enemyinfo[kind].nameid), posx, posy) )
		else
			SetText( m_uiName, F(955, level, posx, posy) )
		end
		-- 按钮名称
		SetText( m_uiBattleButton.transform:Find("Back/Text"), T(961) );
		SetTrue( m_uiAttackDesc )
	
	elseif recvValue.m_type == MAPUNIT_TYPE_NATIONHERO then -- 国家名将
		local kind	= recvValue.m_short_value[1];
		local level = recvValue.m_short_value[2];
		-- 标题
		SetText( m_uiTitle.transform:Find("Text"), T(2344) )
		-- 形象
		SetImage( m_uiShape, LoadSprite("mapunit_nationhero") )
		-- 名字+位置
		SetText( m_uiName, "Lv."..level.." "..Nation( g_nation_heroinfo[kind].nation ).."·"..HeroName(kind) )
		--SetText( m_uiName, F(955, level, posx, posy) )
		-- 按钮名称
		SetText( m_uiBattleButton.transform:Find("Back/Text"), T(961) );
		SetTrue( m_uiAttackDesc )
	
	elseif recvValue.m_type == MAPUNIT_TYPE_ACTIVITY then -- 活动怪
		local kind	= recvValue.m_char_value[1];
		SetText( m_uiTitle.transform:Find("Text"), T(MapUnitActivityNameList[kind]) )
		SetImage( m_uiShape, LoadSprite(MapUnitActivityShapeList[kind]) )
		SetText( m_uiName, F(4270, T(MapUnitActivityNameList[kind]), posx, posy) )

		-- 按钮名称
		SetText( m_uiBattleButton.transform:Find("Back/Text"), T(961) );
		SetTrue( m_uiAttackDesc )
				
	elseif recvValue.m_type == MAPUNIT_TYPE_KINGWAR_TOWN then-- 皇城血战
		townid = 200
		-- 标题
		SetTrue( m_uiAttackDesc )
		SetText( m_uiTitle.transform:Find("Text"), T(1296) )
		SetText( m_uiBattleButton.transform:Find("Back/Text"), T(1296) );
		-- 形象
		local type 	= g_towninfo[townid].type
		SetImage( m_uiShape, LoadSprite( MapUnitTownShapeList[type].."_"..recvValue.m_nation ) )
		--m_uiShape.transform.localScale = Vector3.New( 48, 48, 48 );
		-- 名字
		SetText( m_uiName, F(3019, MapTownName( townid ), g_towninfo[townid].posx, g_towninfo[townid].posy) )	
		-- 按钮名称
		SetText( m_uiBattleButton.transform:Find("Back/Text"), T(1392) );
		SetFalse( m_uiCost )
	end
	
	
	-- 行军时间
	if recvValue.m_type == MAPUNIT_TYPE_KINGWAR_TOWN then
		SetText( m_uiMarchTime1, T(1399) )
		SetText( m_uiMarchTime2, "" )
	else
		if WorldMap.m_nMyCityPosx <= 0 or WorldMap.m_nMyCityPosy <=0 then
			WorldMap.m_nMyCityPosx = GetPlayer().m_posx
			WorldMap.m_nMyCityPosy = GetPlayer().m_posy
		end
		m_marchtime = WorldMap.MarchTime( WorldMap.m_nMyCityPosx, WorldMap.m_nMyCityPosy, posx, posy )
		SetText( m_uiMarchTime1, F(953, secnum(m_marchtime) ) )
		SetText( m_uiMarchTime2, secnum(m_marchtime) )
	end
	
	-- 英雄放到缓存
	m_HeroList = {};
	-- 未出阵的
	for i=0,3,1 do
		if GetHero().m_CityHero[i].m_kind > 0 and GetHero().m_CityHero[i].m_state == 0 then
			table.insert( m_HeroList, clone(GetHero().m_CityHero[i]) )
		end
	end
	-- 已经出阵的
	for i=0,3,1 do
		if GetHero().m_CityHero[i].m_kind > 0 and GetHero().m_CityHero[i].m_state > 0 then
			table.insert( m_HeroList, clone(GetHero().m_CityHero[i]) )
		end
	end
	-- 空
	for i=0,3,1 do
		if GetHero().m_CityHero[i].m_kind <= 0 then
			table.insert( m_HeroList, clone(GetHero().m_CityHero[i]) )
		end
	end
	MapBattleDlgHeroList();
end

-- 英雄列表
function MapBattleDlgHeroList()
	for i=1,4,1 do
		MapBattleDlgHero( i, m_HeroList[i] );
	end
	MapBattleDlgSetSort();
	MapBattleDlgSetCost();
end

-- 设置一个英雄信息
function MapBattleDlgHero( index, pHero )
	local UIP_Hero = m_uiContent.transform:GetChild(index-1).gameObject
	local objs = UIP_Hero.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiStateTitle = objs[0];
	local uiStateTitle1 = objs[1];
	local uiUpBtn = objs[2];
	local uiSoldiersBtn = objs[3];
	local uiUpText = objs[4];
	local uiUnLockText = objs[5];
	local uiShape = objs[6];
	local uiColor = objs[7];
	local uiSortBack = objs[8];
	local uiSort = objs[9];	
	local uiCorps = objs[10];
	local uiName = objs[11];
	local uiSoldiersProgress = objs[12];
	local uiStateTitle2 = objs[13];
	local uiMoveLayer = objs[14];
	local uiSoldiersText = objs[15];
	local uiStateTitle3 = objs[16];
	local uiType = objs[17];
	
	if pHero == nil or pHero.m_kind <= 0 then
		SetControlID( UIP_Hero, 0 )
		SetTrue( uiUnLockText )
		SetFalse( uiStateTitle )
		SetFalse( uiStateTitle1 )
		SetFalse( uiUpBtn )
		SetFalse( uiSoldiersBtn )
		SetFalse( uiUpText )
		SetFalse( uiMoveLayer )
		SetFalse( uiStateTitle2 )
		SetFalse( uiStateTitle3 )
		return;
	end
	
	SetControlID( UIP_Hero, index )
	SetFalse( uiUnLockText )
	SetTrue( uiStateTitle )
	SetTrue( uiStateTitle1 )
	SetTrue( uiUpBtn )
	SetTrue( uiSoldiersBtn )
	SetTrue( uiUpText )
	SetTrue( uiMoveLayer )
	SetTrue( uiStateTitle2 )
	SetFalse( uiStateTitle3 )
	
	SetImage( uiShape, HeroHeadSprite( pHero.m_kind )  );
	SetImage( uiColor,  ItemColorSprite( pHero.m_color )  );
	SetImage( uiCorps,  CorpsSprite( pHero.m_corps )  );
	SetText( uiName, HeroNameLv( pHero.m_kind, pHero.m_level ) );
	SetText( uiSort, index );
	
	local only = GetHero():IsNationHeroOnly( pHero.m_kind )
	if only == true and pHero.m_god == 1 then
		SetTrue( uiType )
		SetText( uiType, T(2359) )
	elseif only == true then
		SetTrue( uiType )
		SetText( uiType, T(2357) )
	elseif pHero.m_god == 1 then
		SetTrue( uiType )
		SetText( uiType, T(2358) )
	else
		SetFalse( uiType )
	end
	
	-- 兵力
	SetProgress( uiSoldiersProgress, pHero.m_soldiers/pHero.m_troops );
	SetText( uiSoldiersText, pHero.m_soldiers.."/"..pHero.m_troops );
	
	-- 状态-出征中
	if pHero.m_state > 0 then
		pHero.m_battle = 0;
		SetFalse( uiSoldiersBtn );
		SetFalse( uiUpText )
		SetFalse( uiStateTitle2 )
		SetTrue( uiStateTitle3 )
		if m_uiMoveLayerPlay[index] == false then
			m_uiMoveLayerPlay[index] = true
			uiMoveLayer.transform:GetComponent( "UITweenRectPosition" ):Play( true );
		end
		SetControlID( UIP_Hero, 0 )
	else
		
		-- 兵力是否足够
		if pHero.m_soldiers < pHero.m_troops then
			SetTrue( uiSoldiersBtn );
			SetControlID( uiSoldiersBtn, 100 + index )
		else
			SetFalse( uiSoldiersBtn );
			SetControlID( uiSoldiersBtn, 0 )
		end
		
		-- 低于20%不能出征
		if math.floor( pHero.m_soldiers/pHero.m_troops *100 ) < global.hero_battle_minhp then
			pHero.m_battle = 0;
			SetTrue( uiStateTitle2 )
			if m_uiMoveLayerPlay[index] == false then
				m_uiMoveLayerPlay[index] = true
				uiMoveLayer.transform:GetComponent( "UITweenRectPosition" ):Play( true );
			end
		else
			uiMoveLayer.transform:GetComponent( "UITweenRectPosition" ):Play( false );
			m_uiMoveLayerPlay[index] = false;
			pHero.m_battle = 1;
			SetFalse( uiStateTitle2 )
			--上阵特效
			MapBattleDlgClickEffect(UIP_Hero);
		end
	end

end

-- 设置排序号
function MapBattleDlgSetSort()
	local sort = 1;
	for i=1,4,1 do
		local pHero = m_HeroList[i];
		local UIP_Hero = m_uiContent.transform:GetChild(i-1).gameObject
		local objs = UIP_Hero.transform:GetComponent( typeof(Reference) ).relatedGameObject;
		if pHero == nil or pHero.m_kind <= 0 then
			return;
		end
		local uiUpBtn = objs[2];
		local uiUpText = objs[4];
		local uiSortBack = objs[8];
		local uiSort = objs[9];	
		if pHero.m_battle == 1 then
			SetTrue( uiSortBack )
			SetTrue( uiSort )
			SetText( uiSort, sort ) -- 排序号
			if sort == 1 then 
				SetFalse( uiUpBtn )
				SetText( uiUpText.transform:Find("Text"), T(960) )-- 首发出阵
			else
				SetTrue( uiUpBtn )
				SetControlID( uiUpBtn, 200 + i )
			end
			sort = sort + 1;
		else
			SetFalse( uiSortBack )
			SetFalse( uiSort )
			SetFalse( uiUpBtn )
			SetText( uiUpText.transform:Find("Text"), T(964) ) -- 未出阵
		end
	end
	
end

-- 设置消耗和总兵力
function MapBattleDlgSetCost()
	local total = 0;
	for i=1,4,1 do
		if m_HeroList[i].m_battle == 1 then
			total = total + m_HeroList[i].m_soldiers;
		end
	end
	
	-- 城池
	if m_recvValue.m_type == MAPUNIT_TYPE_CITY then
		--[[m_cost_body = 5
		if m_cost_food > GetPlayer().m_body then
			SetText( m_uiCost, F(967, total, m_cost_body, GetPlayer().m_body ) )
		else
			SetText( m_uiCost, F(966, total, m_cost_body, GetPlayer().m_body ) )
		end--]]
		SetText( m_uiCost, "" )
		
	-- 城镇
	elseif m_recvValue.m_type == MAPUNIT_TYPE_TOWN then
		SetText( m_uiCost, "" )
	
	-- 流寇
	elseif m_recvValue.m_type == MAPUNIT_TYPE_ENEMY or m_recvValue.m_type == MAPUNIT_TYPE_NATIONHERO or m_recvValue.m_type == MAPUNIT_TYPE_ACTIVITY then
		m_cost_food = math.ceil(global.army_march_food_v1*total + global.army_march_food_v2*(m_marchtime*m_marchtime) + global.army_march_food_v3*m_marchtime)
		if m_cost_food > GetPlayer().m_food then
			SetText( m_uiCost, F(965, total, knum(m_cost_food), knum(GetPlayer().m_food) ) )
		else
			SetText( m_uiCost, F(962, total, knum(m_cost_food), knum(GetPlayer().m_food) ) )
		end
	
	end
	
end

-- 点击
function MapBattleDlgClick( index )
	local pHero = m_HeroList[index];
	local UIP_Hero = m_uiContent.transform:GetChild(index-1).gameObject
	local objs = UIP_Hero.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiMoveLayer = objs[14];
	
	-- 低于20%不能出征
	if math.floor( pHero.m_soldiers/pHero.m_troops *100 ) < global.hero_battle_minhp then
		pop( F(2421,global.hero_battle_minhp) )
		return
	end
		
	
	if m_uiMoveLayerPlay[index] == false then
		uiMoveLayer.transform:GetComponent( "UITweenRectPosition" ):Play( true );
		m_uiMoveLayerPlay[index] = true;
		pHero.m_battle = 0
	else
		uiMoveLayer.transform:GetComponent( "UITweenRectPosition" ):Play( false );
		m_uiMoveLayerPlay[index] = false;
		pHero.m_battle = 1
		--上阵特效
		MapBattleDlgClickEffect(UIP_Hero);
	end
	MapBattleDlgSetSort()
	MapBattleDlgSetCost()
end

--上阵特效
function MapBattleDlgClickEffect(uiObj)
	local effect = GameObject.Instantiate( LoadPrefab( "Fxtx2" ) )
	effect.transform:SetParent( uiObj.transform );
	effect.transform.localPosition = Vector3.New( -540, 2, 0 );		
	effect.transform.localScale = Vector3.New( 135, 150, 150 );
	GameObject.Destroy(effect,0.2)
end

-- 补兵
function MapBattleDlgSoldiers( index )
	local pHero = m_HeroList[index];
	if pHero == nil then
		return;
	end
	
	local left = pHero.m_troops - pHero.m_soldiers;
	if left == 0 then
		return
	end
	
	if pHero.m_corps == 0 then
		if GetPlayer().m_infantry_num == 0 then
			pop(T(2414))
			return
		end
	elseif pHero.m_corps == 1 then
		if GetPlayer().m_cavalry_num == 0 then
			pop(T(2415))
			return
		end
	elseif pHero.m_corps == 2 then
		if GetPlayer().m_archer_num == 0 then
			pop(T(2416))
			return
		end
	end
	
	system_askinfo( ASKINFO_HERO, "", 1, pHero.m_kind );
	eye.audioManager:Play(312);
end

-- 更新血量
function MapBattleDlgUpdate( herokind, soldiers )
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	for i=1,4,1 do
		if m_HeroList[i].m_kind == herokind then
			m_HeroList[i].m_soldiers = soldiers;
			MapBattleDlgHero( i, m_HeroList[i] );
			break;
		end
	end
	MapBattleDlgSetSort();
	MapBattleDlgSetCost();
end

-- 交换
function MapBattleDlgUp( index )
	if index == 1 then
		return;
	end
	m_HeroList[index-1], m_HeroList[index] = m_HeroList[index], m_HeroList[index-1]
	MapBattleDlgHeroList()
end

-- 设置附加数据
function MapBattleDlgAppData( appdata )
	m_appdata = appdata
end

-- 设置附加数据
function MapBattleDlgID( id )
	m_id = id
end

-- 出征
-- m_to_unit_type=0,m_to_unit_index=0,m_id=0,m_herokind={[4]},m_to_posx=0,m_to_posy=0,m_appdata=0,m_action=0,
function MapBattleDlgBattle()
	local sendValue = {};
	sendValue.m_to_unit_type = m_recvValue.m_type;
	sendValue.m_to_unit_index = m_unit_index
	sendValue.m_to_posx = m_recvValue.m_posx;
	sendValue.m_to_posy = m_recvValue.m_posy;
	sendValue.m_id = m_id;
	sendValue.m_appdata = m_appdata;
	sendValue.m_action = m_action;
	sendValue.m_group_index = m_group_index;
	-- 城池
	if m_recvValue.m_type == MAPUNIT_TYPE_CITY then
--[[		if m_cost_body > GetPlayer().m_body then
			JumpBody()
			return
		end--]]
	
	-- 城镇
	elseif m_recvValue.m_type == MAPUNIT_TYPE_TOWN then
	
		
	-- 流寇	
	elseif m_recvValue.m_type == MAPUNIT_TYPE_ENEMY or m_recvValue.m_type == MAPUNIT_TYPE_NATIONHERO then
		if m_cost_food > GetPlayer().m_food then
			JumpRes(3,m_cost_food-GetPlayer().m_food)
			return
		end
		sendValue.m_id = m_recvValue.m_short_value[1];
		
	-- 活动怪
	elseif m_recvValue.m_type == MAPUNIT_TYPE_ACTIVITY then
		if m_cost_food > GetPlayer().m_food then
			JumpRes(3,m_cost_food-GetPlayer().m_food)
			return
		end
		sendValue.m_id = m_recvValue.m_char_value[1];
		
	end
	
	-- 派出武将
	sendValue.m_herokind = {}
	for i=1,4,1 do
		if m_HeroList[i].m_battle == 1 then
			table.insert( sendValue.m_herokind, m_HeroList[i].m_kind );
		end
	end
	for i=1,4,1 do
		if m_HeroList[i].m_battle == nil or m_HeroList[i].m_battle == 0 then
			table.insert( sendValue.m_herokind, 0 );
		end
	end
	if sendValue.m_herokind[1] <= 0 then
		pop( T(1000) )
		return
	end
	
	-- 战前补兵，客户端先行处理
	local has = 0;
	if Utils.get_int_sflag( GetPlayer().m_function, CITY_FUNCTION_BATTLE_ADDHP ) == 1 then
		local infantry_num = GetPlayer().m_infantry_num;
		local cavalry_num = GetPlayer().m_cavalry_num;
		local archer_num = GetPlayer().m_archer_num;
		for i=1,4,1 do
			if m_HeroList[i].m_battle == 1 then
				local pHero = m_HeroList[i];
				local add = pHero.m_troops - pHero.m_soldiers;
				if add > 0 then
					if pHero.m_corps == 0 then
						if add > infantry_num then
							add = infantry_num
						else
							infantry_num = infantry_num - add
						end
					elseif pHero.m_corps == 1 then
						if add > cavalry_num then
							add = cavalry_num
						else
							cavalry_num = cavalry_num - add
						end
					elseif pHero.m_corps == 2 then
						if add > archer_num then
							add = archer_num
						else
							archer_num = archer_num - add
						end
					end
				end
				
				if add > 0 then
					has = 1;
					local UIP_Hero = m_uiContent.transform:GetChild(i-1).gameObject
					local objs = UIP_Hero.transform:GetComponent( typeof(Reference) ).relatedGameObject;
					local uiSoldiersProgress = objs[12];
					local uiSoldiersText = objs[15];
					
					local beginvalue = pHero.m_soldiers/pHero.m_troops;
					local endvalue = (pHero.m_soldiers+add)/pHero.m_troops;
					SetProgressPlay( uiSoldiersProgress, beginvalue, endvalue, 3, 1, function() 
						pHero.m_soldiers = pHero.m_soldiers+add
						SetProgress( uiSoldiersProgress, pHero.m_soldiers/pHero.m_troops );
						SetText( uiSoldiersText, pHero.m_soldiers.."/"..pHero.m_troops )
					end )
				end
			end
		end
	end
	
	if has == 0 then			
		netsend_mapbattle_C( sendValue )
		MapBattleDlgClose()
		eye.audioManager:Play(337);
	else
			
		Invoke( function() 
			netsend_mapbattle_C( sendValue )
			MapBattleDlgClose()
			eye.audioManager:Play(337);
		end, 0.5, nil, "MapBattleDlgBattle" );
	end
end