-- 界面
local m_Dlg = nil;
local m_uiTypeName = nil; --UnityEngine.GameObject
local m_uiLeftNation = nil; --UnityEngine.GameObject
local m_uiRightNation = nil; --UnityEngine.GameObject
local m_uiLeftName = nil; --UnityEngine.GameObject
local m_uiRightName = nil; --UnityEngine.GameObject
local m_uiLeftHeroList = nil; --UnityEngine.GameObject
local m_uiRightHeroList = nil; --UnityEngine.GameObject
local m_uiLeftHp = nil; --UnityEngine.GameObject
local m_uiRightHp = nil; --UnityEngine.GameObject
local m_uiSkipDesc = nil; --UnityEngine.GameObject
local m_uiSkipBtn = nil; --UnityEngine.GameObject
local m_uiLeftPk = nil; --UnityEngine.GameObject
local m_uiRightPk = nil; --UnityEngine.GameObject
local m_uiPkIcon = nil; --UnityEngine.GameObject
local m_uiTop = nil; --UnityEngine.GameObject
local m_uiBottom = nil; --UnityEngine.GameObject
local m_uiLeftGround = nil; --UnityEngine.GameObject
local m_uiRightGround = nil; --UnityEngine.GameObject

local m_recvValue = nil;
local m_assetBundleName = ""

local m_uiLeftHeroCache = {} -- 左面武将信息缓存
local m_uiRightHeroCache = {}-- 右面武将信息缓存
local m_playing = 0;


-- 打开界面
function FightDlgOpen()
	m_Dlg = eye.uiManager:Open( "FightDlg" );
end

-- 隐藏界面
function FightDlgClose()
	if m_Dlg == nil then
		return;
	end
	m_playing = 0;
	eye.uiManager:Close( "FightDlg" );
	FightScene.Delete()
end

-- 删除界面
function FightDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
	Invoke( function() 
			ResourceManager.UnloadAssetBundleImmediately( "_ab_char_shape_infantry" )
		end, 0.3 );
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function FightDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            FightDlgClose();
		
		elseif nControlID == 1 then
			FightDlgClose();
        end
	end
end

-- 载入时调用
function FightDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiTypeName = objs[0];
	m_uiLeftNation = objs[1];
	m_uiRightNation = objs[2];
	m_uiLeftName = objs[3];
	m_uiRightName = objs[4];
	m_uiLeftHeroList = objs[5];
	m_uiRightHeroList = objs[6];
	m_uiLeftHp = objs[7];
	m_uiRightHp = objs[8];
	m_uiSkipDesc = objs[9];
	m_uiSkipBtn = objs[10];
	m_uiLeftPk = objs[11];
	m_uiRightPk = objs[12];
	m_uiPkIcon = objs[13];
	m_uiTop = objs[14];
	m_uiBottom = objs[15];
	m_uiLeftGround = objs[16];
	m_uiRightGround = objs[17];
end

-- 界面初始化时调用
function FightDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function FightDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function FightDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function FightDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function FightDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function FightDlgShow( recvValue )
	FightDlgOpen()
	SetFalse( m_uiTop )
	SetFalse( m_uiBottom )
	m_playing = 0;
	m_recvValue = recvValue;
	local info = json.decode( m_recvValue.m_fight_content );
	
	-- 显示战前信息
	FightInfoDlgShow( info );
	
	-- 战斗类型	
	local fighttype = info["ft"]
	if fighttype == nil then
		SetText( m_uiTypeName, "" )
	else
		SetText( m_uiTypeName, T(2000+fighttype-1) )
	end
	
	-- 战场对象初始化
	FightScene.Create()
	
	-- 战场初始化
	fight_init( info["randspeed"] );
	
	-- 我是攻击方，攻击方显示左面
	local my = info["my"];
	if my == nil or my == 1 then
		-- 攻击方
		SetText( m_uiLeftName, FightInfoDlgGetName( info["a_type"], info["a_name"] ) );
		if info["a_nation"] > 0 then
			SetTrue( m_uiLeftNation )
			SetImage( m_uiLeftNation, NationSprite( info["a_nation"] ) );
		else
			SetFalse( m_uiLeftNation )
		end
		-- 防御方
		SetText( m_uiRightName, FightInfoDlgGetName( info["d_type"], info["d_name"] ) );
		if info["d_nation"] > 0 then
			SetTrue( m_uiRightNation )
			SetImage( m_uiRightNation, NationSprite( info["d_nation"] ) );
		else
			SetFalse( m_uiRightNation )
		end

		-- 攻击方对象
		for i=1, #info["a_unit"], 1 do
			local v =  info["a_unit"][i]
			fight_add_unit( FIGHT_ATTACK, v.i, v.t, v.kd, v.sp, v.lv, v.cr, v.cs, v.a1, v.a2, v.mhp, v.a3, v.a4, v.a5, v.a6, v.a7, v.a8, v.a9 )
		end
		
		-- 防御方对象
		for i=1, #info["d_unit"], 1 do
			local v =  info["d_unit"][i]
			fight_add_unit( FIGHT_DEFENSE, v.i, v.t, v.kd, v.sp, v.lv, v.cr, v.cs, v.a1, v.a2, v.mhp, v.a3, v.a4, v.a5, v.a6, v.a7, v.a8, v.a9 )
		end
	
	-- 我是防御方，防御方显示左面
	else
		-- 攻击方
		SetText( m_uiLeftName, FightInfoDlgGetName( info["d_type"], info["d_name"] ) );
		if info["d_nation"] > 0 then
			SetTrue( m_uiLeftNation )
			SetImage( m_uiLeftNation, NationSprite( info["d_nation"] ) );
		else
			SetFalse( m_uiLeftNation )
		end
		-- 防御方
		SetText( m_uiRightName, FightInfoDlgGetName( info["a_type"], info["a_name"] ) );
		if info["a_nation"] > 0 then
			SetTrue( m_uiRightNation )
			SetImage( m_uiRightNation, NationSprite( info["a_nation"] ) );
		else
			SetFalse( m_uiRightNation )
		end
		
		
		-- 攻击方对象
		for i=1, #info["d_unit"], 1 do
			local v =  info["d_unit"][i]
			fight_add_unit( FIGHT_ATTACK, v.i, v.t, v.kd, v.sp, v.lv, v.cr, v.cs, v.a1, v.a2, v.mhp, v.a3, v.a4, v.a5, v.a6, v.a7, v.a8, v.a9 )
		end
		
		-- 防御方对象
		for i=1, #info["a_unit"], 1 do
			local v =  info["a_unit"][i]
			fight_add_unit( FIGHT_DEFENSE, v.i, v.t, v.kd, v.sp, v.lv, v.cr, v.cs, v.a1, v.a2, v.mhp, v.a3, v.a4, v.a5, v.a6, v.a7, v.a8, v.a9 )
		end
	end
	FightDlgUnitUpdate()
end

-- 更新英雄阵列显示
function FightDlgUnitUpdate()
	for i=0, 3, 1 do
		local uiLeftObj = m_uiLeftHeroList.transform:GetChild(i)
		SetFalse( uiLeftObj.transform:GetChild(1) )
		SetFalse( uiLeftObj.transform:GetChild(2) )
		SetFalse( uiLeftObj.transform:GetChild(3) )
		SetFalse( uiLeftObj.transform:GetChild(4) )
		
		local uiRightObj = m_uiRightHeroList.transform:GetChild(i)
		SetFalse( uiRightObj.transform:GetChild(1) )
		SetFalse( uiRightObj.transform:GetChild(2) )
		SetFalse( uiRightObj.transform:GetChild(3) )
		SetFalse( uiRightObj.transform:GetChild(4) )
	end
	
	local LeftNum = g_fight.attack_unit_num
	if LeftNum > 4 then
		LeftNum = 4
	end
	for i=1, LeftNum, 1 do
		local unit = g_fight.attack_unit[i-1]
		FightDlgUnit( m_uiLeftHeroList.transform:GetChild(i-1), i, unit.type, unit.kind, unit.shape, unit.color, unit.corps, unit.level, unit.maxhp, unit.hp )
	end
	
	local RightNum = g_fight.defense_unit_num
	if RightNum > 4 then
		RightNum = 4
	end
	for i=1, RightNum, 1 do
		local unit = g_fight.defense_unit[i-1]
		FightDlgUnit( m_uiRightHeroList.transform:GetChild(i-1), i, unit.type, unit.kind, unit.shape, unit.color, unit.corps, unit.level, unit.maxhp, unit.hp )
	end
	
	-- 对战信息
	FightDlgUnitVsUpdateName()
	FightDlgUnitVsUpdateHp()
	FightDlgUnitVsUpdateLayer()
end

-- 设置一个unit信息
function FightDlgUnit( uiObj, index, unittype, kind, shape, color, corps, level, maxhp, hp )
	SetTrue( uiObj.transform:GetChild(1) )
	SetTrue( uiObj.transform:GetChild(2) )
	SetTrue( uiObj.transform:GetChild(3) )
	
	SetImage( uiObj.transform:GetChild(1), FightDlgUnitShape( unittype, kind, shape ) );
	SetImage( uiObj.transform:GetChild(2), ItemColorSprite( color ) );
	SetImage( uiObj.transform:GetChild(3), CorpsSprite( corps ) );
	
	if index == 1 then
		SetFalse( uiObj.transform:GetChild(4) )
	else
		SetTrue( uiObj.transform:GetChild(4) )
		SetText( uiObj.transform:GetChild(4), FightDlgUnitName( unittype, kind ) );
	end
			
end

-- 设置战斗单元名称
function FightDlgUnitName( unittype, kind )
	-- 玩家 部队 资源点
	if unittype == FIGHT_UNITTYPE_HERO then
		return HeroName( kind );
			
	-- 城镇
	elseif unittype == FIGHT_UNITTYPE_GUARD then
		return T( 1119 )
		
	-- 流寇
	elseif unittype == FIGHT_UNITTYPE_MONSTER then
		return EnemyName( kind )
	end
	return ""
end

-- 设置战斗单元名称
function FightDlgUnitShape( unittype, kind, shape )
	-- 玩家 部队 资源点
	if unittype == FIGHT_UNITTYPE_HERO then
		return HeroHeadSprite( kind )
			
	-- 城镇
	elseif unittype == FIGHT_UNITTYPE_GUARD then
		return GuardSprite( shape )
		
	-- 流寇
	elseif unittype == FIGHT_UNITTYPE_MONSTER then
		return EnemyHeadSprite( shape )
	end
	return ""
end

-- 对战者名称
function FightDlgUnitVsUpdateName()
	local unit = fight_nextptr( FIGHT_ATTACK )
	if unit ~= nil then
		SetText( m_uiLeftHp.transform:Find("Name"), FightDlgUnitName( unit.type, unit.kind ) )
	end
	
	unit = fight_nextptr( FIGHT_DEFENSE )
	if unit ~= nil then
		SetText( m_uiRightHp.transform:Find("Name"), FightDlgUnitName( unit.type, unit.kind ) )
	end
end

-- 对战者血量
function FightDlgUnitVsUpdateHp()
	local unit = fight_nextptr( FIGHT_ATTACK )
	if unit ~= nil then
		SetText( m_uiLeftHp.transform:Find("Text"), unit.hp )
		SetProgress( m_uiLeftHp.transform:Find("Progress"), unit.hp/unit.maxhp )
	end
	
	unit = fight_nextptr( FIGHT_DEFENSE )
	if unit ~= nil then
		SetText( m_uiRightHp.transform:Find("Text"), unit.hp )
		SetProgress( m_uiRightHp.transform:Find("Progress"), unit.hp/unit.maxhp )
	end
end

-- 创建对战页信息
function FightDlgUnitVsUpdateLayer()
	local unit = fight_nextptr( FIGHT_ATTACK )
	if unit ~= nil then
		SetImage( m_uiLeftPk.transform:Find("UIP_Hero/Shape"), FightDlgUnitShape( unit.type, unit.kind, unit.shape ) )
		SetImage( m_uiLeftPk.transform:Find("UIP_Hero/Color"), ItemColorSprite( unit.color ) )
		SetImage( m_uiLeftPk.transform:Find("UIP_Hero/Corps"), CorpsSprite( unit.corps ) )
		SetText( m_uiLeftPk.transform:Find("Name"), FightDlgUnitName( unit.type, unit.kind ) )
		SetText( m_uiLeftPk.transform:Find("Hp/Text"), unit.hp )
	end
	
	unit = fight_nextptr( FIGHT_DEFENSE )
	if unit ~= nil then
		SetImage( m_uiRightPk.transform:Find("UIP_Hero/Shape"), FightDlgUnitShape( unit.type, unit.kind, unit.shape ) )
		SetImage( m_uiRightPk.transform:Find("UIP_Hero/Color"), ItemColorSprite( unit.color ) )
		SetImage( m_uiRightPk.transform:Find("UIP_Hero/Corps"), CorpsSprite( unit.corps ) )
		SetText( m_uiRightPk.transform:Find("Name"), FightDlgUnitName( unit.type, unit.kind ) )
		SetText( m_uiRightPk.transform:Find("Hp/Text"), unit.hp )
	end
end

-- 双方当前对战武将详情
function FightDlgUnitVsLayerShow()
	local leftTween = m_uiLeftPk.transform:GetComponent( "UITweenRectPosition" )
	leftTween:Play( true )
	
	local rightTween = m_uiRightPk.transform:GetComponent( "UITweenRectPosition" )
	rightTween:Play( true )
	
	Invoke( function() 
		SetTrue( m_uiPkIcon )
	end, 0.5 );
		
	Invoke( function() 
		SetFalse( m_uiPkIcon )
		leftTween:Play( false )
		rightTween:Play( false )
	end, 1.5 );
end

-- 启动战斗
function FightDlgStart()
	if m_playing == 1 then
		return
	end
	SetTrue( m_uiTop )
	SetTrue ( m_uiBottom )
	m_playing = 1;
	
	FightDlgUnitVsLayerShow()
	
	FightDlgCreateUnit()
end


-- 战斗逻辑每一秒一次
function FightDlgLogic()
	if m_playing == 0 then
		return
	end
	local result = fight_oneturn()
	if result > 0 then
		if  result == FIGHT_WIN then
			fight_debug( "[ATK WIN]" );
		elseif result == FIGHT_LOSE then
			fight_debug( "[DEF WIN]" );
		end
		m_playing = 0
	end
end
