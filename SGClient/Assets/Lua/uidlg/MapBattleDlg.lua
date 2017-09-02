-- 界面
local m_Dlg = nil;
local m_uiShape = nil; --UnityEngine.GameObject
local m_uiName = nil; --UnityEngine.GameObject
local m_uiMarchTime1 = nil; --UnityEngine.GameObject
local m_uiMarchTime2 = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiCost = nil; --UnityEngine.GameObject

local m_unit_index = -1;
local m_marchtime = 0;
local m_cost_food = 0;
local m_cost_body = 0;
local m_HeroList = {};
local m_recvValue = {};
local m_uiMoveLayerPlay = { false, false, false, false };

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
function MapBattleDlgShow( unit_index, recvValue )
	MapBattleDlgOpen()
	m_unit_index = unit_index
	m_recvValue 	= recvValue;
	local posx 		= recvValue.m_posx;
	local posy 		= recvValue.m_posy;
	
	if recvValue.m_type == MAPUNIT_TYPE_CITY then-- 城池
	elseif recvValue.m_type == MAPUNIT_TYPE_ARMY then -- 部队
	elseif recvValue.m_type == MAPUNIT_TYPE_TOWN then -- 城镇
	elseif recvValue.m_type == MAPUNIT_TYPE_ENEMY then -- 流寇
		local level	= recvValue.m_char_value[1];
		local kind 	= recvValue.m_short_value[1];
		-- 形象
		m_uiShape:GetComponent("SpriteRenderer").sprite = LoadSprite("mapunit_enemy_level"..level);
		-- 名字+位置
		SetText( m_uiName, F(955, level, posx, posy) )
		
	elseif recvValue.m_type == MAPUNIT_TYPE_RES then-- 资源点
	end
	
	
	-- 行军时间
	m_marchtime = WorldMap.MarchTime( WorldMap.m_nMyCityPosx, WorldMap.m_nMyCityPosy, posx, posy )
	SetText( m_uiMarchTime1, F(953, secnum(m_marchtime) ) )
	SetText( m_uiMarchTime2, secnum(m_marchtime) )
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
			pHero.m_battle = 1;
			SetFalse( uiStateTitle2 )
			if m_uiMoveLayerPlay[index] == true then
				m_uiMoveLayerPlay[index] = false
				uiMoveLayer.transform:GetComponent( "UITweenRectPosition" ):Play( false );
			end
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
		m_cost_body = math.ceil(global.army_march_food_v1*total + global.army_march_food_v2*(m_marchtime*m_marchtime) + global.army_march_food_v3*m_marchtime)
		if m_cost_food > GetPlayer().m_body then
			SetText( m_uiCost, F(967, total, m_cost_body, GetPlayer().m_body ) )
		else
			SetText( m_uiCost, F(966, total, m_cost_body, GetPlayer().m_body ) )
		end
	
	-- 部队
	elseif m_recvValue.m_type == MAPUNIT_TYPE_ARMY then
	
	-- 城镇
	elseif m_recvValue.m_type == MAPUNIT_TYPE_TOWN then
	
	-- 流寇
	elseif m_recvValue.m_type == MAPUNIT_TYPE_ENEMY then
		m_cost_food = math.ceil(global.army_march_food_v1*total + global.army_march_food_v2*(m_marchtime*m_marchtime) + global.army_march_food_v3*m_marchtime)
		if m_cost_food > GetPlayer().m_food then
			SetText( m_uiCost, F(965, total, knum(m_cost_food), knum(GetPlayer().m_food) ) )
		else
			SetText( m_uiCost, F(962, total, knum(m_cost_food), knum(GetPlayer().m_food) ) )
		end
	
	-- 资源点
	elseif m_recvValue.m_type == MAPUNIT_TYPE_RES then
	end
	
end

-- 点击
function MapBattleDlgClick( index )
	local pHero = m_HeroList[index];
	local UIP_Hero = m_uiContent.transform:GetChild(index-1).gameObject
	local objs = UIP_Hero.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiMoveLayer = objs[14];
	
	if m_uiMoveLayerPlay[index] == false then
		uiMoveLayer.transform:GetComponent( "UITweenRectPosition" ):Play( true );
		m_uiMoveLayerPlay[index] = true;
		pHero.m_battle = 0
	else
		uiMoveLayer.transform:GetComponent( "UITweenRectPosition" ):Play( false );
		m_uiMoveLayerPlay[index] = false;
		pHero.m_battle = 1
	end
	MapBattleDlgSetSort()
	MapBattleDlgSetCost()
end

-- 补兵
function MapBattleDlgSoldiers( index )
	if m_HeroList[index] == nil then
		return;
	end
	system_askinfo( ASKINFO_HERO, "", 1, m_HeroList[index].m_kind );
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

-- 出征
-- m_to_unit_type=0,m_to_unit_index=0,m_id=0,m_herokind={[4]},m_to_posx=0,m_to_posy=0,m_appdata=0,m_action=0,
function MapBattleDlgBattle()
	local sendValue = {};
	sendValue.m_to_unit_type = m_recvValue.m_type;
	sendValue.m_to_unit_index = m_unit_index
	sendValue.m_to_posx = m_recvValue.m_posx;
	sendValue.m_to_posy = m_recvValue.m_posy;
	sendValue.m_appdata = 0;
	sendValue.m_action = 0;
	
	-- 城池
	if m_recvValue.m_type == MAPUNIT_TYPE_CITY then
		if m_cost_body > GetPlayer().m_body then
			JumpBody()
			return
		end
		sendValue.m_action = ARMY_ACTION_FIGHT
		
	-- 部队
	elseif m_recvValue.m_type == MAPUNIT_TYPE_ARMY then
		sendValue.m_action = ARMY_ACTION_FIGHT
		
	-- 城镇
	elseif m_recvValue.m_type == MAPUNIT_TYPE_TOWN then
	
		
	-- 流寇	
	elseif m_recvValue.m_type == MAPUNIT_TYPE_ENEMY then
		if m_cost_food > GetPlayer().m_food then
			JumpRes(3)
			return
		end
		sendValue.m_id = m_recvValue.m_short_value[1];
		sendValue.m_action = ARMY_ACTION_FIGHT
		
	-- 资源点
	elseif m_recvValue.m_type == MAPUNIT_TYPE_RES then
		
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
		return
	end
	netsend_mapbattle_C( sendValue )
	MapBattleDlgClose()
end

