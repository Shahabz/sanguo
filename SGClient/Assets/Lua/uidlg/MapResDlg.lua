-- 界面
local m_Dlg = nil;
local m_uiShape = nil; --UnityEngine.GameObject
local m_uiName = nil; --UnityEngine.GameObject
local m_uiMarchTime1 = nil; --UnityEngine.GameObject
local m_uiMarchTime2 = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiCost = nil; --UnityEngine.GameObject
local m_uiResInfo = nil; --UnityEngine.GameObject
local m_uiScroll = nil; --UnityEngine.GameObject
local m_uiGatherButton = nil; --UnityEngine.GameObject
local m_uiBattleButton = nil; --UnityEngine.GameObject
local m_uiGatherInfo = nil; --UnityEngine.GameObject

local m_unit_index = -1;
local m_marchtime = 0;
local m_cost_food = 0;
local m_HeroList = {};
local m_recvValue = {};
local m_infoRecvValue = {};
local m_SelectHeroKind = 0;
local m_reskind = 0;
local m_defaultSelect = 0;

-- 打开界面
function MapResDlgOpen()
	ResourceManager.LoadAssetBundle( "_ab_ui_mapbattledlg" );
	m_Dlg = eye.uiManager:Open( "MapResDlg" );
end

-- 隐藏界面
function MapResDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "MapResDlg" );
end

-- 删除界面
function MapResDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
	ResourceManager.UnloadAssetBundle( "_ab_ui_mapbattledlg" );
	m_unit_index = -1;
	m_marchtime = 0;
	m_cost_food = 0;
	m_HeroList = {};
	m_recvValue = {};
	m_infoRecvValue = {};
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function MapResDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            MapResDlgClose();
			
		-- 采集
		elseif nControlID == 1 then
			MapResDlgGather()
			
		-- 占领采集
		elseif nControlID == 2 then
			MapResDlgFight()
		
		-- 占领采集出击
		elseif nControlID == 3 then
			MapResDlgBattle()
		
		-- 采集帮助
		elseif nControlID == 1000 then
			HelpDlgShowBySystem( HELP_MapResDlg )
			
		elseif nControlID > 10000 and nControlID < 20000 then
			 MapResDlgSelectHero( nControlID-10000 );
        end
	end
end

-- 载入时调用
function MapResDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiShape = objs[0];
	m_uiName = objs[1];
	m_uiMarchTime1 = objs[2];
	m_uiMarchTime2 = objs[3];
	m_uiContent = objs[4];
	m_uiCost = objs[5];
	m_uiResInfo = objs[6];
	m_uiScroll = objs[7];
	m_uiGatherButton = objs[8];
	m_uiBattleButton = objs[9];
	m_uiGatherInfo = objs[10];
end

-- 界面初始化时调用
function MapResDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function MapResDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function MapResDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function MapResDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function MapResDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function MapResDlgShow( recvValue )
	eye.audioManager:Play(334);
	MapResDlgOpen()
	m_SelectHeroKind = 0;
	m_defaultSelect = 0;
	for i = 0 ,m_uiContent.transform.childCount - 1 do
		SetFalse( m_uiContent.transform:GetChild(i).gameObject )
	end
	SetFalse( m_uiCost );
	SetFalse( m_uiGatherInfo );
	
	m_unit_index 	= recvValue.unit_index
	m_recvValue 	= recvValue;
	local state 	= recvValue.m_state;
	local posx 		= recvValue.m_posx;
	local posy 		= recvValue.m_posy;
	local restype	= recvValue.m_char_value[1];
	local level		= recvValue.m_char_value[2];
	m_reskind		= recvValue.m_short_value[1];
	
	system_askinfo( ASKINFO_WORLDMAP, "", 7, recvValue.m_unit_index );
	
	-- 形象
	SetImage( m_uiShape, LoadSprite( MapUnitResShapeList[restype] ) );
	-- 名字+位置
	SetText( m_uiName, F(986, level, T(MapUnitResNameList[restype]), posx, posy) )
		
	-- 行军时间
	m_marchtime = WorldMap.MarchTime( WorldMap.m_nMyCityPosx, WorldMap.m_nMyCityPosy, posx, posy )
	SetText( m_uiMarchTime1, F(953, secnum(m_marchtime) ) )
	SetText( m_uiMarchTime2, secnum(m_marchtime) )
	
	-- 采集状态
	if state == ARMY_STATE_GATHER then
	else
		MapResDlgHeroLayer()
	end
end

function MapResDlgHeroLayer()
	SetTrue( m_uiScroll )
	-- 英雄放到缓存
	m_HeroList = {};
	m_defaultSelect = 0;
	
	-- 已经出阵的包括财赋署的
	for i=0,7,1 do
		if GetHero().m_CityHero[i].m_kind > 0 and GetHero().m_CityHero[i].m_state > 0 then
			table.insert( m_HeroList, clone(GetHero().m_CityHero[i]) )
		end
	end
	
	-- 未出阵的包括财赋署的
	for i=0,7,1 do
		if GetHero().m_CityHero[i].m_kind > 0 and GetHero().m_CityHero[i].m_state == 0 then
			table.insert( m_HeroList, clone(GetHero().m_CityHero[i]) )
		end
	end
	
	for i=1,#m_HeroList,1 do
		MapResDlgHero( i, m_HeroList[i] );
	end	
end

-- 设置一个英雄信息
function MapResDlgHero( index, pHero )
	local UIP_Hero = m_uiContent.transform:GetChild(index-1).gameObject
	local objs = UIP_Hero.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiShape = objs[0];
	local uiColor = objs[1];
	local uiCorps = objs[2];
	local uiName = objs[3];
	local uiSoldiers = objs[4];
	local uiSelect = objs[5];
	SetTrue( UIP_Hero )
	SetImage( uiShape, HeroHeadSprite( pHero.m_kind )  );
	SetImage( uiColor,  ItemColorSprite( pHero.m_color )  );
	SetImage( uiCorps,  CorpsSprite( pHero.m_corps )  );
	SetText( uiSoldiers, pHero.m_soldiers.."/"..pHero.m_troops );
	
	-- 状态-出征中
	if pHero.m_state > 0 then
		SetText( uiName, T(992) );
		SetFalse( uiSelect )
		SetControlID( UIP_Hero, 0 )
	else
		-- 低于20%不能出征
		if math.floor( pHero.m_soldiers/pHero.m_troops *100 ) < global.hero_battle_minhp then
			SetText( uiName, T(993) );
			SetFalse( uiSelect )
			SetControlID( UIP_Hero, 0 )
		else
			if m_reskind > 0 and pHero.m_level < g_resinfo[m_reskind].herolevel then
				SetText( uiName, F(2423,g_resinfo[m_reskind].herolevel) );
				SetFalse( uiSelect )
				SetControlID( UIP_Hero, 0 )
			else
				SetText( uiName, HeroNameLv( pHero.m_kind, pHero.m_level ), NameColor( pHero.m_color ) );
				SetTrue( uiSelect )
				SetImage( uiSelect, LoadSprite("ui_button_select1") )
				SetControlID( UIP_Hero, 10000+index )
				if m_defaultSelect == 0 then
					MapResDlgSelectHero( index )
					m_defaultSelect = 1
				end
			end
		end
	end
	
end

-- 获取详细信息
-- m_kind=0,m_totalnum=0,m_totalsec=0,m_spacenum=0,m_herokind=0,m_herolevel=0,m_herocolor=0,m_herohp=0,m_actorlevel=0,
function MapResDlgRecv( infoRecvValue )
	m_infoRecvValue 	= infoRecvValue;
	
	local state 	= m_recvValue.m_state;
	local posx 		= m_recvValue.m_posx;
	local posy 		= m_recvValue.m_posy;
	local restype	= m_recvValue.m_char_value[1];
	local level		= m_recvValue.m_char_value[2];
	
	-- 采集状态
	if state == ARMY_STATE_GATHER and m_infoRecvValue.m_herokind > 0 then
		SetTrue( m_uiResInfo );
		SetFalse( m_uiCost );
		SetTrue( m_uiGatherInfo );
		m_uiGatherInfo.transform.localPosition = Vector2.New( 0, 0 )
		local nation	= m_recvValue.m_char_value[3];
		local actorid	= m_recvValue.m_int_value[1];
		
		SetImage( m_uiGatherInfo.transform:Find("TargetHero/Shape"), HeroHeadSprite(m_infoRecvValue.m_herokind) )
		SetImage( m_uiGatherInfo.transform:Find("TargetHero/Color"), ItemColorSprite(m_infoRecvValue.m_herocolor) )
		SetImage( m_uiGatherInfo.transform:Find("TargetHero/Corps"), CorpsSprite( g_heroinfo[m_infoRecvValue.m_herokind][m_infoRecvValue.m_herocolor].corps ) )
		
		-- 采集玩家
		SetText( m_uiGatherInfo.transform:Find("Target").transform:GetChild(0), F( 997, "["..Nation(nation).."] Lv."..m_infoRecvValue.m_actorlevel.." "..m_recvValue.m_name ) )
		
		-- 采集武将
		SetText( m_uiGatherInfo.transform:Find("Target").transform:GetChild(1), F( 998, " Lv."..m_infoRecvValue.m_herolevel.." "..HeroName(m_infoRecvValue.m_herokind) ) )
		
		-- 武将兵力
		SetText( m_uiGatherInfo.transform:Find("Target").transform:GetChild(2), F( 999, " "..m_infoRecvValue.m_herohp ) )
		
		-- 如果是自己的
		if actorid == GetPlayer().m_actorid or nation == GetPlayer().m_nation then
			SetFalse( m_uiGatherButton );
			SetFalse( m_uiBattleButton );
		else
			SetFalse( m_uiGatherButton );
			SetTrue( m_uiBattleButton );
			SetControlID( m_uiBattleButton,2 )
		end
		
		-- 资源
		SetImage( m_uiResInfo.transform:Find("Shape"),  ResItemIcon( restype ) )
		SetText( m_uiResInfo.transform:Find("Name"), ResName( restype ) )
		
		-- 剩余资源量/资源总量
		local text = m_uiResInfo.transform:Find("Desc").transform:GetChild(0);
		SetTrue( text )
		SetText( text, F( 987, m_infoRecvValue.m_spacenum, m_infoRecvValue.m_totalnum ) )
		SetFalse( m_uiResInfo.transform:Find("Desc").transform:GetChild(1) )
		SetFalse( m_uiResInfo.transform:Find("Desc").transform:GetChild(2) )
		SetFalse( m_uiResInfo.transform:Find("Desc").transform:GetChild(3) )
		
	-- 非采集状态	
	else
		
		SetFalse( m_uiGatherInfo );
		SetTrue( m_uiResInfo );
		SetImage( m_uiResInfo.transform:Find("Shape"),  ResItemIcon( restype ) )
		SetText( m_uiResInfo.transform:Find("Name"), ResName( restype ) )
		
		-- 剩余资源量/资源总量
		local text = m_uiResInfo.transform:Find("Desc").transform:GetChild(0);
		SetTrue( text )
		SetText( text, F( 987, m_infoRecvValue.m_spacenum, m_infoRecvValue.m_totalnum ) )
		
		-- 采集速度/秒
		local sec_gather = m_infoRecvValue.m_totalnum/m_infoRecvValue.m_totalsec;
		
		-- 采集速度/小时
		local text1 = m_uiResInfo.transform:Find("Desc").transform:GetChild(1);
		SetTrue( text1 )
		SetText( text1, F( 988, math.floor(sec_gather * 3600) ) )
		
		--武将采集时长
		local gatherlong = m_infoRecvValue.m_totalsec;
		local herocolor_time = {}
		herocolor_time[0] = global.hero_gather_duration0
		herocolor_time[1] = global.hero_gather_duration1
		herocolor_time[2] = global.hero_gather_duration2
		herocolor_time[3] = global.hero_gather_duration3
		herocolor_time[4] = global.hero_gather_duration4
		herocolor_time[5] = global.hero_gather_duration5
		
		-- 采集总时长不超过武将能采集的时长
		if herocolor_time[m_infoRecvValue.m_herocolor] > gatherlong then
			gatherlong = herocolor_time[m_infoRecvValue.m_herocolor];
		end
		
		local text2 = m_uiResInfo.transform:Find("Desc").transform:GetChild(2);
		SetTrue( text2 )
		SetText( text2, F( 989, secnum(gatherlong) ) )
		
		-- 预计采集数量
		local gather_base = sec_gather*gatherlong;
		if gather_base > m_infoRecvValue.m_spacenum then
			gather_base = m_infoRecvValue.m_spacenum;
		end
		
		local gather_total = math.round(gather_base*(1.0 + GetPlayer().m_attr.m_gather_per[1]/100) * (1.0 + GetPlayer().m_attr.m_gather_per[2]/100))
		if gather_total > m_infoRecvValue.m_spacenum then
			gather_total = m_infoRecvValue.m_spacenum;
		end
		local other = gather_total-gather_base; -- 额外收获
		local text3 = m_uiResInfo.transform:Find("Desc").transform:GetChild(3);
		SetTrue( text3 )
		if other > 0 then
			SetText( text3, F( 990, gather_base ).."("..T(1002).."+"..knum(other)..")" )
		else
			SetText( text3, F( 990, gather_base ) )
		end
		
		SetFalse( m_uiBattleButton );
		SetTrue( m_uiGatherButton )
	end
end

-- 选择武将
function MapResDlgSelectHero( index )
	local kind = m_HeroList[index]
	if kind == nil then
		return
	end
	m_SelectHeroKind = m_HeroList[index].m_kind;
	for i = 0 ,m_uiContent.transform.childCount - 1 do
		local UIP_Hero = m_uiContent.transform:GetChild(i).gameObject
		local objs = UIP_Hero.transform:GetComponent( typeof(Reference) ).relatedGameObject;
		local uiSelect = objs[5];
		if i == index-1 then
			SetImage( uiSelect, LoadSprite("ui_button_select2") )
		else
			SetImage( uiSelect, LoadSprite("ui_button_select1") )
		end
	end
	
	-- 行军消耗
	SetTrue( m_uiCost );
	local total = m_HeroList[index].m_soldiers;
	m_cost_food = math.ceil(global.army_march_food_v1*total + global.army_march_food_v2*(m_marchtime*m_marchtime) + global.army_march_food_v3*m_marchtime)
	if m_cost_food > GetPlayer().m_food then
		SetRichText( m_uiCost.transform:Find("Text"), "<color=03de27ff>"..T(991)..": <icon=food>"..knum(m_cost_food).."</color><color=e80017ff>/"..knum(GetPlayer().m_food).."</color>" )
	else
		SetRichText( m_uiCost.transform:Find("Text"), "<color=03de27ff>"..T(991)..": <icon=food>"..knum(m_cost_food).."</color><color=03de27ff>/"..knum(GetPlayer().m_food).."</color>" )
		--SetRichText( m_uiCost.transform:Find("Text"), "<color=03de27ff>消耗食物</color><icon=food>1000/<color=e80017ff>500</color>" )
	end
end

-- 采集
function MapResDlgGather()
	-- 不在同一区域
	if map_zone_checksame( WorldMap.m_nMyCityPosx, WorldMap.m_nMyCityPosy, m_recvValue.m_posx, m_recvValue.m_posy ) == 0 then
		AlertMsg(T(1322))
		MapResDlgClose()
		return
	end
	if m_cost_food > GetPlayer().m_food then
		JumpRes(3)
		return
	end
	if m_SelectHeroKind <= 0 then
		AlertMsg(T(1000))
		return
	end
		
	local sendValue = {};
	sendValue.m_to_unit_type = m_recvValue.m_type;
	sendValue.m_to_unit_index = m_recvValue.m_unit_index;
	sendValue.m_to_posx = m_recvValue.m_posx;
	sendValue.m_to_posy = m_recvValue.m_posy;
	sendValue.m_appdata = 0;
	sendValue.m_action = ARMY_ACTION_GATHER
	sendValue.m_id = 0;
	sendValue.m_group_index = -1;
		
	-- 派出武将
	sendValue.m_herokind = { m_SelectHeroKind, 0,0,0 }
	netsend_mapbattle_C( sendValue )
	MapResDlgClose()
end

-- 占领采集出击
function MapResDlgBattle()
	-- 不在同一区域
	if map_zone_checksame( WorldMap.m_nMyCityPosx, WorldMap.m_nMyCityPosy, m_recvValue.m_posx, m_recvValue.m_posy ) == 0 then
		AlertMsg(T(1322))
		MapResDlgClose()
		return
	end
	if m_cost_food > GetPlayer().m_food then
		JumpRes(3)
		return
	end
	if m_SelectHeroKind <= 0 then
		AlertMsg(T(1000))
		return
	end
		
	local sendValue = {};
	sendValue.m_to_unit_type = m_recvValue.m_type;
	sendValue.m_to_unit_index = m_recvValue.m_unit_index;
	sendValue.m_to_posx = m_recvValue.m_posx;
	sendValue.m_to_posy = m_recvValue.m_posy;
	sendValue.m_appdata = 0;
	sendValue.m_action = ARMY_ACTION_FIGHT
	sendValue.m_id = 0;
	sendValue.m_group_index = -1;
		
	-- 派出武将
	sendValue.m_herokind = { m_SelectHeroKind, 0,0,0 }
	netsend_mapbattle_C( sendValue )
	MapResDlgClose()
end

-- 占领采集
function MapResDlgFight()
	MapResDlgHeroLayer()
	SetFalse( m_uiResInfo );
	m_uiGatherInfo.transform.localPosition = Vector2.New( 0, -290 )
	SetControlID( m_uiBattleButton,3 )
end

