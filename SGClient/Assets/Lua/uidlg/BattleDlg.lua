-- 界面
local m_Dlg = nil;
local m_uiTitleText = nil; --UnityEngine.GameObject
local m_uiCancelBtn = nil; --UnityEngine.GameObject
local m_uiFightBtn = nil; --UnityEngine.GameObject
local m_uiArmyIcon = nil; --UnityEngine.GameObject
local m_uiLeftName = nil; --UnityEngine.GameObject
local m_uiRightName = nil; --UnityEngine.GameObject
local m_uiLeftScroll = nil; --UnityEngine.GameObject
local m_uiLeftContent = nil; --UnityEngine.GameObject
local m_uiRightScroll = nil; --UnityEngine.GameObject
local m_uiRightContent = nil; --UnityEngine.GameObject
local m_uiCenterScroll = nil; --UnityEngine.GameObject
local m_uiCenterContent = nil; --UnityEngine.GameObject
local m_uiUIP_Unit = nil; --UnityEngine.GameObject
local m_uiStoryExp = nil; --UnityEngine.GameObject
local m_uiStoryBody = nil; --UnityEngine.GameObject

local m_ObjectPool = nil;
local m_type = 0;
local m_battleType = 0;
local m_storyid = 0;
local m_bossid = 0;
local m_HeroList = {};
local m_SortList = nil;
local m_EnemyData = {};
local m_EnemyCount = 0;

-- 打开界面
function BattleDlgOpen()
	ResourceManager.LoadAssetBundle( "_ab_ui_static_battle_back" );
	m_Dlg = eye.uiManager:Open( "BattleDlg" );
end

-- 隐藏界面
function BattleDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "BattleDlg" );
end

-- 删除界面
function BattleDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
	ResourceManager.UnloadAssetBundle( "_ab_ui_static_battle_back" );
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function BattleDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            BattleDlgClose();
		
		-- 战斗
		elseif nControlID == 1 then
			BattleDlgFight();
			
		-- 上移
		elseif nControlID >= 201 and nControlID <= 204 then
			BattleDlgHeroUp( nControlID-200 )
        end
	end
end

-- 载入时调用
function BattleDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiTitleText = objs[0];
	m_uiCancelBtn = objs[1];
	m_uiFightBtn = objs[2];
	m_uiCenterScroll = objs[3];
	m_uiArmyIcon = objs[4];
	m_uiCenterContent = objs[5];
	m_uiLeftName = objs[6];
	m_uiRightName = objs[7];
	m_uiLeftScroll = objs[8];
	m_uiLeftContent = objs[9];
	m_uiRightScroll = objs[10];
	m_uiRightContent = objs[11];
	m_uiUIP_Unit = objs[12];
	m_uiStoryExp = objs[13];
	m_uiStoryBody = objs[14];

	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );
	m_ObjectPool:CreatePool("UIP_Unit", 8, 8, m_uiUIP_Unit);
end

-- 界面初始化时调用
function BattleDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function BattleDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function BattleDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function BattleDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function BattleDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function BattleDlgShow()
	BattleDlgOpen()
end

-- 副本使用
function BattleDlgShowByStory( storyid )
	BattleDlgOpen()
	m_type = 0
	m_battleType = 0;
	m_storyid = storyid;
	SetTrue( m_uiTitleText )
	SetText( m_uiTitleText, StoryChapterName( math.floor(storyid/10) ) )
	SetTrue( m_uiCancelBtn )
	SetTrue( m_uiFightBtn )
	SetTrue( m_uiStoryExp )
	SetTrue( m_uiStoryBody )
	-- 预计获得经验
	SetText( m_uiStoryExp, F(2016, g_story[storyid].exp ) )
	-- 消耗体力
	if GetPlayer().m_body < g_story[storyid].body then
		SetText( m_uiStoryBody.transform:Find("Text"), F(2018, g_story[storyid].body, GetPlayer().m_body ) )	
	else
		SetText( m_uiStoryBody.transform:Find("Text"), F(2017, g_story[storyid].body,GetPlayer().m_body ) )	
	end
	
	m_HeroList = {};
	-- 本次首次打开
	if m_SortList == nil then
		m_SortList = {}
		-- 英雄放到缓存
		for i=0,3,1 do
			if GetHero().m_CityHero[i].m_kind > 0 then
				table.insert( m_HeroList, clone(GetHero().m_CityHero[i]) )
			end
			table.insert( m_SortList, i )
		end
	
	-- 第二次之后打开使用缓存排序的	
	else
		for i=0,3,1 do
			local offset = m_SortList[i+1];
			local pHero = GetHero().m_CityHero[offset];
			if pHero and pHero.m_kind > 0 then
				table.insert( m_HeroList, clone(pHero) )
			end
		end
	end
		
	BattleDlgClearRightUnit()
	system_askinfo( ASKINFO_STORY, "", 1, storyid );
	BattleDlgHeroList()
	
	SetTrue( m_uiCancelBtn )
	SetTrue( m_uiFightBtn )
end

-- 世界boss使用
function BattleDlgShowByWorldBoss( bossid )
	BattleDlgOpen()
	m_type = 0
	m_battleType = 1;
	m_bossid = bossid;
	SetTrue( m_uiTitleText )
	SetText( m_uiTitleText, T(1359) )
	SetTrue( m_uiCancelBtn )
	SetTrue( m_uiFightBtn )
	SetFalse( m_uiStoryExp )
	SetFalse( m_uiStoryBody )
	
	-- 英雄放到缓存
	m_HeroList = {};
	for i=0,3,1 do
		if GetHero().m_CityHero[i].m_kind > 0 then
			table.insert( m_HeroList, clone(GetHero().m_CityHero[i]) )
		end
	end
	BattleDlgClearRightUnit()
	system_askinfo( ASKINFO_QUEST, "", 14, bossid );
	BattleDlgHeroList()
end

-- 副本和世界boss接收使用
-- m_count=0,m_list={m_monsterid=0,m_shape=0,m_level=0,m_color=0,m_corps=0,m_hp,[m_count]},m_exp=0,m_body=0,m_type=0
function BattleDlgStoryRecv( recvValue )
	
	-- 副本
	if recvValue.m_type == 0 then
		
	-- 世界boss
	elseif recvValue.m_type == 1 then
		
	end
	m_EnemyData = recvValue.m_list;
	m_EnemyCount = recvValue.m_count;
	for i=1, recvValue.m_count, 1 do
		local unit = recvValue.m_list[i]
		BattleDlgUnit( m_uiRightContent, i, FIGHT_UNITTYPE_MONSTER, unit.m_monsterid, nil, unit.m_shape, unit.m_color, unit.m_corps, unit.m_level, unit.m_hp )
	end
	BattleDlgCenter()
end

-- 英雄列表
function BattleDlgHeroList()
	BattleDlgClearLeftUnit()
	for i=1,#m_HeroList,1 do
		BattleDlgHero( i, m_HeroList[i] );
	end
	BattleDlgSetHeroSort();
end

-- 设置一个英雄信息
function BattleDlgHero( index, pHero )
	local uiObj = m_ObjectPool:Get( "UIP_Unit" );
	uiObj.transform:SetParent( m_uiLeftContent.transform );
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiShape = objs[0];
	local uiColor = objs[1];
	local uiCorps = objs[2];
	local uiName = objs[3];
	local uiHp = objs[4];
	local uiSort = objs[5];	
	local uiUpButton = objs[6];	
	local uiFristButton = objs[7];	
	
	SetTrue( uiUpButton )
	SetImage( uiShape, HeroHeadSprite( pHero.m_kind )  );
	SetImage( uiColor,  ItemColorSprite( pHero.m_color )  );
	SetImage( uiCorps,  CorpsSprite( pHero.m_corps )  );
	SetText( uiName, HeroNameLv( pHero.m_kind, pHero.m_level ) );
	SetText( uiHp, pHero.m_troops );
	SetText( uiSort, index );
end

-- 设置英雄出战顺序
function BattleDlgSetHeroSort()
	local sort = 1;
	for i=1,#m_HeroList,1 do
		local pHero = m_HeroList[i];
		local uiObj = m_uiLeftContent.transform:GetChild(i-1).gameObject
		local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
		if pHero == nil or pHero.m_kind <= 0 then
			return;
		end
		local uiSort = objs[5];	
		local uiUpButton = objs[6];
		local uiFristButton = objs[7];
		SetFalse( uiSort )

		if sort == 1 then 
			SetTrue( uiFristButton )
			SetFalse( uiUpButton )
			SetControlID( uiFristButton, 0 )
			SetControlID( uiUpButton, 0 )
		else
			SetFalse( uiFristButton )
			SetTrue( uiUpButton )
			SetControlID( uiUpButton, 200 + i )
		end
		sort = sort + 1;
	end
	
end

-- 交换
function BattleDlgHeroUp( index )
	if index == 1 then
		return;
	end
	m_HeroList[index-1], m_HeroList[index] = m_HeroList[index], m_HeroList[index-1]
	m_SortList[index-1], m_SortList[index] = m_SortList[index], m_SortList[index-1]
	BattleDlgHeroList()
	BattleDlgCenter();
end

-- 设置一个unit信息
function BattleDlgUnit( root, index, unittype, kind, name, shape, color, corps, level, hp )
	local uiObj = m_ObjectPool:Get( "UIP_Unit" );
	uiObj.transform:SetParent( root.transform );
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiShape = objs[0];
	local uiColor = objs[1];
	local uiCorps = objs[2];
	local uiName = objs[3];
	local uiHp = objs[4];
	local uiSort = objs[5];	
	local uiUpButton = objs[6];
	local uiFristButton = objs[7];	
	
	SetFalse( uiUpButton )
	if hp == nil then
		SetFalse( uiHp )
	else
		SetTrue( uiHp )
		SetText( uiHp, hp );
	end

	-- 玩家英雄
	if unittype == FIGHT_UNITTYPE_HERO then
		SetImage( uiShape, HeroHeadSprite( kind ) );
		SetImage( uiColor, ItemColorSprite( color ) );
		SetImage( uiCorps, CorpsSprite( corps ) );
		if name and name ~= "" then
			SetText( uiName, "["..name.."]".."Lv."..level.." "..HeroName( kind ) );
		else
			SetText( uiName, "Lv."..level.." "..HeroName( kind ) );
		end
		
	-- 玩家城墙守卫
	elseif unittype == FIGHT_UNITTYPE_GUARD then
		SetImage( uiShape, GuardSprite( shape ) );
		SetImage( uiColor, ItemColorSprite( color ) );
		SetImage( uiCorps, CorpsSprite( corps ) );
		SetText( uiName, "Lv."..level.." "..T( 1119 ) );
		
	-- 怪物表
	elseif unittype == FIGHT_UNITTYPE_MONSTER then
		SetImage( uiShape, EnemyHeadSprite( shape ) );
		SetImage( uiColor, ItemColorSprite( color ) );
		SetImage( uiCorps, CorpsSprite( corps ) );
		SetText( uiName, "Lv."..level.." "..EnemyName( kind ) );
	end
	SetText( uiSort, index );
end

--设置兵种相克
function BattleDlgCenter()
	for i = 0 ,m_uiCenterContent.transform.childCount - 1 do
		local uiObj = m_uiCenterContent.transform:GetChild(i).gameObject		
		if i+1 > m_EnemyCount or m_EnemyData[i+1] == nil or m_HeroList[i+1] == nil then 
			SetFalse(uiObj);
		else
			SetTrue(uiObj);
			local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
			local uiBack = objs[0];
			local EnemyCorps = m_EnemyData[i+1].m_corps;
			local HeroCorps = m_HeroList[i+1].m_corps;
			SetImage(uiBack,BattleDlgGetCenterImage(HeroCorps,EnemyCorps))
		end
	end	
end

-- 获得相克图片
function BattleDlgGetCenterImage(HeroCorps,EnemyCorps)
	if HeroCorps == EnemyCorps then 
		return LoadSprite("ui_battle_icon_5")
	else
		if HeroCorps == 0 then 
			if EnemyCorps == 1 then
				return LoadSprite("ui_battle_icon_4")
			elseif EnemyCorps == 2 then 
				return LoadSprite("ui_battle_icon_3")
			end
		elseif HeroCorps == 1 then 
			if EnemyCorps == 0 then
				return LoadSprite("ui_battle_icon_3")
			elseif EnemyCorps == 2 then 
				return LoadSprite("ui_battle_icon_4")
			end
		elseif HeroCorps == 2 then 
			if EnemyCorps == 0 then
				return LoadSprite("ui_battle_icon_4")
			elseif EnemyCorps == 1 then 
				return LoadSprite("ui_battle_icon_3")
			end
		end
	end
end

-- 清空
function BattleDlgClearLeftUnit()
	local objs = {};
	for i = 0 ,m_uiLeftContent.transform.childCount - 1 do
		table.insert( objs, m_uiLeftContent.transform:GetChild(i).gameObject )
    end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "UIP_Unit(Clone)" then
			m_ObjectPool:Release( "UIP_Unit", obj );
		end
    end
end

-- 清空
function BattleDlgClearRightUnit()
	local objs = {};
	for i = 0 ,m_uiRightContent.transform.childCount - 1 do
		table.insert( objs, m_uiRightContent.transform:GetChild(i).gameObject )
    end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "UIP_Unit(Clone)" then
			m_ObjectPool:Release( "UIP_Unit", obj );
		end
    end
end

-- 战斗
function BattleDlgFight()
	SetFalse( m_uiCancelBtn )
	SetFalse( m_uiFightBtn )
	if m_battleType == 0 then	
		-- 消耗体力
		if GetPlayer().m_body < g_story[m_storyid].body then
			BattleDlgClose()
			JumpBody()
			return
		end	
		-- m_storyid=0,m_herokind={[4]},
		local sendValue={}
		sendValue.m_storyid = m_storyid;
		sendValue.m_herokind = {0,0,0,0}
		for i=1, #m_HeroList, 1 do
			sendValue.m_herokind[i] = m_HeroList[i].m_kind
		end 
		netsend_storybattle_C( sendValue )
		
	elseif m_battleType == 1 then
		-- m_bossid=0,m_herokind={[4]},
		local sendValue={}
		sendValue.m_bossid = m_bossid;
		sendValue.m_herokind = {0,0,0,0}
		for i=1, #m_HeroList, 1 do
			sendValue.m_herokind[i] = m_HeroList[i].m_kind
		end 
		netsend_worldbossbattle_C( sendValue )
		BattleDlgClose()
		
	end
end

function BattleDlgGetStoryid()
	return m_storyid
end
