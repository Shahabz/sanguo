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
local m_uiLeftHpEffect = nil; --UnityEngine.GameObject
local m_uiRightHpEffect = nil; --UnityEngine.GameObject
local m_uiLeftMissEffect = nil; --UnityEngine.GameObject
local m_uiRightMissEffect = nil; --UnityEngine.GameObject
local m_uiResultLayer = nil; --UnityEngine.GameObject
local m_uiTitle = nil; --UnityEngine.GameObject
local m_uiPicWin = nil; --UnityEngine.GameObject
local m_uiPicLose = nil; --UnityEngine.GameObject
local m_uiAwardGrid = nil; --UnityEngine.GameObject
local m_uiHeroList = nil; --UnityEngine.GameObject
local m_uiCloseBtn = nil; --UnityEngine.GameObject
local m_uiReturnCityBtn = nil; --UnityEngine.GameObject
local m_uiReturnStoryBtn = nil; --UnityEngine.GameObject
local m_uiLeftSkill = nil; --UnityEngine.GameObject
local m_uiRightSkill = nil; --UnityEngine.GameObject
local m_uiStar = nil; --UnityEngine.GameObject
local m_uiStarWarn = nil; --UnityEngine.GameObject
local m_uiWarn = nil; --UnityEngine.GameObject
local m_uiSweepBtn = nil; --UnityEngine.GameObject

local m_recvValue = nil;
local m_jsonFightInfo = nil
local m_assetBundleName = ""

local m_uiLeftHeroCache = {} -- 左面武将信息缓存
local m_uiRightHeroCache = {}-- 右面武将信息缓存
local m_playing = 0;

local m_WaitCallback = nil;
local m_WaitValue = nil;
	
-- 打开界面
function FightDlgOpen()
	m_Dlg = eye.uiManager:Open( "FightDlg" );
end

-- 隐藏界面
function FightDlgClose()
	if m_Dlg == nil then
		return;
	end
	for i=0, 3, 1 do
		local uiObj = m_uiHeroList.transform:GetChild( i )
		SetProgressStop( uiObj.transform:Find("Progress") )
	end	
	m_playing = 0;
	eye.uiManager:Close( "FightDlg" );
	FightScene.Delete()
	
	if m_WaitCallback then
		m_WaitCallback( m_WaitValue );
	end
	m_WaitCallback = nil;
	m_WaitValue = nil;
end

-- 删除界面
function FightDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function FightDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            FightDlgClose();
		
		-- 跳过战斗提示
		elseif nControlID == -2 then
			JumpFightSkip()
			
		-- 跳过战斗
		elseif nControlID == 1 then
			FightDlgResultLayerShow()
		
		-- 返回城池	
		elseif nControlID == 2 then
			FightDlgClose();
			
		-- 返回副本
		elseif nControlID == 3 then	
			FightDlgClose();
			StoryDlgShow()
			
		-- 继续扫荡
		elseif nControlID == 4 then	
			FightDlgClose();
			StorySweepDlgSweep()
        end
		
	elseif nType == UI_EVENT_TWEENFINISH then

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
	m_uiLeftHpEffect = objs[16];
	m_uiRightHpEffect = objs[17];
	m_uiLeftMissEffect = objs[18];
	m_uiRightMissEffect = objs[19];
	m_uiResultLayer = objs[20];
	m_uiTitle = objs[21];
	m_uiPicWin = objs[22];
	m_uiPicLose = objs[23];
	m_uiAwardGrid = objs[24];
	m_uiHeroList = objs[25];
	m_uiCloseBtn = objs[26];
	m_uiReturnCityBtn = objs[27];
	m_uiReturnStoryBtn = objs[28];
	m_uiLeftSkill = objs[29];
	m_uiRightSkill = objs[30];
	m_uiStar = objs[31];
	m_uiStarWarn = objs[32];
	m_uiWarn = objs[33];
	m_uiSweepBtn = objs[34];
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
	SetFalse( m_uiLeftHpEffect );
	SetFalse( m_uiRightHpEffect );
	SetFalse( m_uiResultLayer )

	m_playing = 0;
	m_recvValue = recvValue;
	local info = json.decode( m_recvValue.m_fight_content );
	m_jsonFightInfo = info;
		
	-- 战斗类型	
	local fighttype = info["ft"]
	if fighttype == nil then
		SetText( m_uiTypeName, "" )
	else
		SetText( m_uiTypeName, T(2000+fighttype-1) )
	end
	
	-- 是否可以跳过战斗
	if fighttype == FIGHTTYPE_QUEST then
		SetFalse( m_uiBottom )
	elseif fighttype == FIGHTTYPE_STORY then
		local storyid = BattleDlgGetStoryid()
		if storyid > 0 and g_story[storyid].skip == 1 then
			SetTrue( m_uiBottom )
			if Utils.get_int_sflag( GetPlayer().m_actor_sflag, ACTOR_SFLAG_SKIPFIGHT ) == 1 then
				SetControlID( m_uiSkipBtn, 1 )
				SetText( m_uiSkipDesc, T(2279) )
			else
				SetControlID( m_uiSkipBtn, -2 )
				SetText( m_uiSkipDesc, T(2280) )
			end
		else
			SetFalse( m_uiBottom )
		end
	else
		SetTrue( m_uiBottom )
		SetControlID( m_uiSkipBtn, 1 )
		SetText( m_uiSkipDesc, T(2279) )
	end
	
	-- 显示战前信息
	if fighttype ~= FIGHTTYPE_QUEST then
		FightInfoDlgShow( info );
	end
	
	-- 战场对象初始化
	FightScene.Create()
	
	-- 战场初始化
	fight_init( info["randspeed"], fighttype );
	
	-- 我是攻击方，攻击方显示左面
	local my = m_recvValue.m_content_json["my"];
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
	
	-- 任务战特殊处理
	if fighttype == FIGHTTYPE_QUEST then
		FightDlgStart()
	end
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
	
	local index = 0;
	for i=1, g_fight.attack_unit_num, 1 do
		local unit = g_fight.attack_unit[i-1]
		if unit.line_left > 0 then
			FightDlgUnit( m_uiLeftHeroList.transform:GetChild(index), index, unit.type, unit.kind, unit.shape, unit.color, unit.corps, unit.level, unit.maxhp, unit.hp )
			index = index + 1
			if index >= 4 then
				break;
			end
		end
	end
	
	index = 0;
	for i=1, g_fight.defense_unit_num, 1 do
		local unit = g_fight.defense_unit[i-1]
		if unit.line_left > 0 then
			FightDlgUnit( m_uiRightHeroList.transform:GetChild(index), index, unit.type, unit.kind, unit.shape, unit.color, unit.corps, unit.level, unit.maxhp, unit.hp )
			index = index + 1
			if index >= 4 then
				break;
			end
		end
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
	
	if index == 0 then
		SetFalse( uiObj.transform:GetChild(4) )
	else
		SetTrue( uiObj.transform:GetChild(4) )
		SetText( uiObj.transform:GetChild(4), FightDlgUnitName( unittype, kind ) );
	end
			
end

-- 设置战斗单元名称
function FightDlgUnitName( unittype, kind )
	-- 玩家 部队 资源点
	if unittype == FIGHT_UNITTYPE_LEADER_HERO or unittype == FIGHT_UNITTYPE_HERO then
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
	if unittype == FIGHT_UNITTYPE_LEADER_HERO or unittype == FIGHT_UNITTYPE_HERO then
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
	else
		SetText( m_uiLeftHp.transform:Find("Text"), 0 )
		SetProgress( m_uiLeftHp.transform:Find("Progress"), 0 )
	end
	
	unit = fight_nextptr( FIGHT_DEFENSE )
	if unit ~= nil then
		SetText( m_uiRightHp.transform:Find("Text"), unit.hp )
		SetProgress( m_uiRightHp.transform:Find("Progress"), unit.hp/unit.maxhp )
	else
		SetText( m_uiRightHp.transform:Find("Text"), 0 )
		SetProgress( m_uiRightHp.transform:Find("Progress"), 0 )
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
	if g_fight.m_playing == false then
		return
	end
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

-- 播放血动画
function FightDlgPlayHpEffect( pos, hp, crit )
	if g_fight.m_playing == false then
		return
	end
	local uiObj = nil
	if pos == FIGHT_ATTACK then
		uiObj = m_uiLeftHpEffect
	else
		uiObj = m_uiRightHpEffect
	end
	
	SetFalse( uiObj );
	for i=0, uiObj.transform.childCount-1, 1 do
		SetFalse( uiObj.transform:GetChild(i) );
	end
	
	SetTrue( uiObj );
	
	uiObj.transform.localScale = Vector3.New( 1, 1, 1 );
	local color = NameColor( 4 )
	if crit == 1 then
		color = Hex2Color( 0xECC244FF )
		uiObj.transform.localScale = Vector3.New( 1.5, 1.5, 1 );
	end
	
	-- 计算位数 +1减号算一位
	local hpstr = "-"..hp
	local hpnum = string.len( hpstr )
	local offset = math.floor( (10-hpnum)/2 )
	
	local index = 1;
	for i=offset, offset+hpnum, 1 do
		SetText( uiObj.transform:GetChild(i), string.sub( hpstr, index,index ), color )
		uiObj.transform:GetChild(i).transform:GetComponent( "UITweenRectPosition" ).delay = (index-1)*0.1
		SetTrue( uiObj.transform:GetChild(i) );
		index = index + 1
	end
	
	Invoke( function()
			SetFalse( uiObj );
	end, 1 )
end


-- 播放Miss动画
function FightDlgPlayMissEffect( pos )
	if g_fight.m_playing == false then
		return
	end
	if pos == FIGHT_ATTACK then
		SetFalse( m_uiLeftMissEffect );
		SetTrue( m_uiLeftMissEffect );
		Invoke( function()
			SetFalse( m_uiLeftMissEffect );
		end, 0.8, nil, "FightInvoke_MissEffect_"..pos )
	else
		SetFalse( m_uiRightMissEffect );
		SetTrue( m_uiRightMissEffect );
		Invoke( function()
			SetFalse( m_uiRightMissEffect );
		end, 0.8 )
	end
end

-- 播放技能
function FightDlgPlaySkill( pos, skillid, unittype, kind, shape )
	if g_fight.m_playing == false then
		return
	end
	local uiObj = nil
	if pos == FIGHT_ATTACK then
		uiObj = m_uiLeftSkill
	else
		uiObj = m_uiRightSkill
	end
	SetTrue( uiObj )
	SetTrue( uiObj.transform:Find("Mask") )
	local uiName = uiObj.transform:Find("Name")
	local uiPic = uiObj.transform:Find("Pic");
	SetImage( uiPic.transform:Find("Shape"), FightDlgUnitShape( unittype, kind, shape ) )
	SetText( uiPic.transform:Find("Name"), FightDlgUnitName( unittype, kind ) )
	SetImage( uiPic, LoadSprite("ui_fightdlg_skill_back"..skillid) )
	SetImage( uiName, LoadSprite("ui_fightdlg_skill_"..skillid) )
	
	-- 播放移进动画
	local uiPicTween = uiPic.transform:GetComponent( "UITweenRectPosition" );
	local uiNameTween = uiName.transform:GetComponent( "UITweenRectPosition" );
	if pos == FIGHT_ATTACK then
		uiPicTween.from = Vector2( -750, 0 ); 
		uiPicTween.to = Vector2( 0, 0 ); 
		uiNameTween.from = Vector2( -540, -71 ); 
		uiNameTween.to = Vector2( 210, -71 ); 
	else
		uiPicTween.from = Vector2( 750, 0 ); 
		uiPicTween.to = Vector2( 0, 0 ); 
		uiNameTween.from = Vector2( 540, -71 ); 
		uiNameTween.to = Vector2( -210, -71 ); 
	end
	
	uiPicTween.duration = 0.3;
	uiPicTween:Play( true );
	uiNameTween.duration = 0.4;
	uiNameTween:Play( true );
	
	-- 播放移出动画
	Invoke( function()
		SetFalse( uiObj.transform:Find("Mask") )
		if pos == FIGHT_ATTACK then
			uiPicTween.from = Vector2( 0, 0 ); 
			uiPicTween.to = Vector2( 750, 0 ); 
			uiNameTween.from = Vector2( 210, -71 ); 
			uiNameTween.to = Vector2( 960, -71 ); 
		else
			uiPicTween.from = Vector2( 0, 0 ); 
			uiPicTween.to = Vector2( -750, 0 ); 
			uiNameTween.from = Vector2( -210, -71 ); 
			uiNameTween.to = Vector2( -960, -71 ); 
		end
		
		uiPicTween.duration = 0.2;
		uiPicTween:Play( true );
		uiNameTween.duration = 0.2;
		uiNameTween:Play( true );

	end, 1.2, nil, "FightInvoke_Skill_"..pos )
end
	
-- 启动战斗
function FightDlgStart()
	if m_playing == 1 then
		return
	end
	-- 防御方没人参加
	if g_fight.defense_total_maxhp <= 0 then
		FightDlgResultLayerShow()
		return
	end
	SetTrue( m_uiTop )
	m_playing = 1;
	
	FightDlgUnitVsLayerShow()
	
	-- 进场音效
	eye.audioManager:Play(301);
	-- 启动战斗
	fight_start();
end

-- 战斗结束页
function FightDlgResultLayerShow()
	if m_jsonFightInfo == nil then
		FightDlgClose()
		return
	end
	
	eye.audioManager:Stop(202);
	eye.audioManager:Stop(301);
	eye.audioManager:Stop(305);
	
	m_uiLeftSkill.transform:Find("Name"):GetComponent( "UITweenRectPosition" ):Kill(true)
	m_uiLeftSkill.transform:Find("Pic"):GetComponent( "UITweenRectPosition" ):Kill(true)
	m_uiRightSkill.transform:Find("Name"):GetComponent( "UITweenRectPosition" ):Kill(true)
	m_uiRightSkill.transform:Find("Pic"):GetComponent( "UITweenRectPosition" ):Kill(true)
	
	fight_destory()
	SetTrue( m_uiResultLayer )
	
	-- 战斗类型	
	local fighttype = m_jsonFightInfo["ft"]
	if fighttype == nil then
		SetText( m_uiTitle.transform:Find("Text"), "" )
	elseif fighttype == FIGHTTYPE_QUEST then
		FightDlgClose()
	else
		SetText( m_uiTitle.transform:Find("Text"), T(2000+fighttype-1) )
	end
	
	-- 战斗结果
	local my = m_recvValue.m_content_json["my"];
	local fightResult = m_jsonFightInfo["fr"]
	if fightResult == 1 then
		-- 如果是攻击方
		if my == nil or my == 1 then
			SetTrue( m_uiPicWin ) 
			SetFalse( m_uiPicLose ) 
			eye.audioManager:Play(408);
		else
			SetTrue( m_uiPicLose ) 
			SetFalse( m_uiPicWin )
			eye.audioManager:Play(409);
		end
	else
		if my == nil or my == 1 then
			SetTrue( m_uiPicLose ) 
			SetFalse( m_uiPicWin )
			eye.audioManager:Play(409);
		else
			SetTrue( m_uiPicWin ) 
			SetFalse( m_uiPicLose ) 
			eye.audioManager:Play(408);
		end
	end
	
	for i=0, 3, 1 do
		SetFalse( m_uiAwardGrid.transform:GetChild( i ) )
	end	
	for i=0, 3, 1 do
		SetFalse( m_uiHeroList.transform:GetChild( i ) )
	end	
	
	local index = 0
	local awardTable = {}
	-- 经验奖励	
	local exp = m_recvValue.m_content_json["exp"]
	if exp ~= nil then
		awardTable[index] = { kind = AWARDKIND_EXP, num = exp }
		index = index + 1
	end
	
	-- 资源奖励
	local silver = m_recvValue.m_content_json["silver"]
	if silver ~= nil then
		awardTable[index] = { kind = AWARDKIND_SILVER, num = silver }
		index = index + 1
	end
	
	local wood = m_recvValue.m_content_json["wood"]
	if wood ~= nil then
		awardTable[index] = { kind = AWARDKIND_WOOD, num = wood }
		index = index + 1
	end
	
	local food = m_recvValue.m_content_json["food"]
	if food ~= nil then
		awardTable[index] = { kind = AWARDKIND_FOOD, num = food }
		index = index + 1
	end
	
	local people = m_recvValue.m_content_json["people"]
	if people ~= nil then
		awardTable[index] = { kind = AWARDKIND_PEOPLE, num = people }
		index = index + 1
	end

	-- 道具奖励
	local awardstr = ""
	local award = m_recvValue.m_content_json["award"];
	if award ~= nil then
		local awardList = string.split( award, '@' )
		for i= 1, #awardList, 1 do
			local awardinfo = string.split( awardList[i], ',' )
			if #awardinfo == 2 then
				local kind = tonumber(awardinfo[1]);
				local num = tonumber(awardinfo[2]);
				awardTable[index] = { kind = kind, num = num }
				index = index + 1
			end
		end
	end	
	
	if index > 4 then
		index = 4
	end
	for i=0, index-1, 1 do
		local sprite, color, name = AwardInfo( awardTable[i].kind )
		local uiAward = m_uiAwardGrid.transform:GetChild( i )
		SetTrue( uiAward )
		SetImage( uiAward.transform:Find("Icon"), sprite );
		SetText( uiAward.transform:Find("Name"), name.."x"..awardTable[i].num  )
	end			
	
	-- 英雄
	-- m_uiHeroList = objs[25];
	local unitlist = nil
	local my = m_recvValue.m_content_json["my"];
	if my == nil or my == 1 then
		-- 攻击方对象
		unitlist = m_jsonFightInfo["a_unit"];
	else
		-- 防御方对象
		unitlist = m_jsonFightInfo["d_unit"];
	end
	
	local index = 0;
	for k, v in pairs(unitlist) do
		if v.t == FIGHT_UNITTYPE_LEADER_HERO then
			local uiObj = m_uiHeroList.transform:GetChild( index )
			SetImage( uiObj.transform:Find("Shape"), HeroHeadSprite( v.kd ) )
			SetImage( uiObj.transform:Find("Color"), ItemColorSprite( v.cr ) )
			
			-- 获取经验
			if v.exp ~= nil then 
				SetText( uiObj.transform:Find("Exp"), "+"..v.exp )
			else
				SetText( uiObj.transform:Find("Exp"), "+0" )
			end
			
			-- 检查是否升级
			local beginvalue = v.mexp/hero_getexp_max( v.lv, v.cr );
			local playcount = 1;
			local isup = 0;
			local level =  v.lv;
			v.mexp = v.mexp + v.exp;
			while v.mexp >= hero_getexp_max( v.lv, v.cr ) do
				local curlevel = v.lv;
				-- 可以升级
				if curlevel >= global.actorlevel_max then
					
					break
				end
				v.lv = v.lv + 1;
				v.mexp = v.mexp - hero_getexp_max( curlevel, v.cr );
				isup = isup + 1;
				playcount = playcount + 1
				--升级特效
				FightDlgLevelUpEffect(uiObj)
			end
			local endvalue = v.mexp/hero_getexp_max( v.lv, v.cr );
			
			-- 进度条SetProgressPlay( transform, beginvalue, endvalue, duration, playcount )
			if v.mexp ~= nil then
				SetProgressPlay( uiObj.transform:Find("Progress"), beginvalue, endvalue, 2, playcount, function() 
					-- 名称+等级
					level = level + 1
					if level > v.lv then
						level = v.lv
					end
					SetText( uiObj.transform:Find("Name"), HeroNameLv( v.kd, level ) )
				end )
			else
				SetProgress( uiObj.transform:Find("Progress"), 0 )
			end
			
			SetTrue( uiObj )
			index = index + 1
			if index >= 4 then
				break
			end
		end
	end
			
	-- 副本战斗返回按钮
	if fighttype == FIGHTTYPE_STORY then
		SetFalse( m_uiWarn )
		SetFalse( m_uiCloseBtn )
		SetTrue( m_uiReturnCityBtn )
		SetTrue( m_uiReturnStoryBtn )
		SetFalse( m_uiSweepBtn )
		local star = m_recvValue.m_content_json["star"]
		if star ~= nil and star > 0 then
			SetTrue( m_uiStar )
			SetTrue( m_uiStarWarn )
			StoryDlgStar( m_uiStar, star )
		else
			SetFalse( m_uiStar )
			SetFalse( m_uiStarWarn )
		end
	else
		SetFalse( m_uiWarn )
		SetTrue( m_uiCloseBtn )
		SetFalse( m_uiReturnCityBtn )
		SetFalse( m_uiReturnStoryBtn )
		SetFalse( m_uiSweepBtn )
		SetFalse( m_uiStar )
		SetFalse( m_uiStarWarn )
	end
end

-- 扫荡结果
-- m_exp=0,m_silver=0,m_awardcount=0,m_award={m_kind=0,m_num=0,[m_awardcount]},m_herocount=0,m_hero={m_kind=0,m_color=0,m_level=0,m_pre_exp=0,m_exp=0,[m_herocount]},
function FightDlgSweepResult( recvValue )
	FightDlgOpen()
	SetFalse( m_uiTop )
	SetFalse( m_uiBottom )
	SetFalse( m_uiLeftHpEffect );
	SetFalse( m_uiRightHpEffect );
	SetTrue( m_uiResultLayer )
	
	SetText( m_uiTitle.transform:Find("Text"), T(2000+FIGHTTYPE_STORY-1) )
	SetTrue( m_uiPicWin ) 
	SetFalse( m_uiPicLose ) 
	eye.audioManager:Play(408);
			
	SetFalse( m_uiCloseBtn )
	SetFalse( m_uiReturnCityBtn )
	SetTrue( m_uiReturnStoryBtn )
	SetTrue( m_uiSweepBtn )
	SetFalse( m_uiStar )
	SetFalse( m_uiStarWarn )
	SetTrue( m_uiWarn )
	SetText( m_uiWarn, T(2034) )
	
	for i=0, 3, 1 do
		SetFalse( m_uiAwardGrid.transform:GetChild( i ) )
	end	
	for i=0, 3, 1 do
		SetFalse( m_uiHeroList.transform:GetChild( i ) )
	end	
	
	
	local index = 0
	local awardTable = {}
	-- 经验奖励	
	if recvValue.m_exp > 0 then
		awardTable[index] = { kind = AWARDKIND_EXP, num = recvValue.m_exp }
		index = index + 1
	end
	
	-- 资源奖励
	if recvValue.m_silver > 0 then
		awardTable[index] = { kind = AWARDKIND_SILVER, num = recvValue.m_silver }
		index = index + 1
	end
	
	-- 道具奖励
	for i=1, recvValue.m_awardcount, 1 do
		awardTable[index] = { kind = recvValue.m_award[i].m_kind, num = recvValue.m_award[i].m_num }
		index = index + 1
	end
	
	-- 设置显示奖励
	if index > 4 then
		index = 4
	end
	for i=0, index-1, 1 do
		local sprite, color, name = AwardInfo( awardTable[i].kind )
		local uiAward = m_uiAwardGrid.transform:GetChild( i )
		SetTrue( uiAward )
		SetImage( uiAward.transform:Find("Icon"), sprite );
		SetText( uiAward.transform:Find("Name"), name.."x"..knum(awardTable[i].num) )
	end
	
	-- 武将经验信息
	for i=1, recvValue.m_herocount, 1 do
		local uiObj = m_uiHeroList.transform:GetChild( i-1 )
		local pHero = recvValue.m_hero[i]
		SetImage( uiObj.transform:Find("Shape"), HeroHeadSprite( pHero.m_kind ) )
		SetImage( uiObj.transform:Find("Color"), ItemColorSprite( pHero.m_color ) )		
		SetText( uiObj.transform:Find("Name"),  pHero.m_name  )

		SetTrue( uiObj )
		-- 获取经验
		SetText( uiObj.transform:Find("Exp"), "+"..pHero.m_exp )
		
		-- 检查是否升级
		local beginvalue = pHero.m_pre_exp/hero_getexp_max( pHero.m_level, pHero.m_color );
		local playcount = 1;
		local isup = 0;
		local level =  pHero.m_level;
		pHero.m_pre_exp = pHero.m_pre_exp + pHero.m_exp;
		while pHero.m_pre_exp >= hero_getexp_max( pHero.m_level, pHero.m_color ) do
			local curlevel = pHero.m_level
			-- 可以升级
			if curlevel >= global.actorlevel_max then				
				break
			end
			pHero.m_level = pHero.m_level + 1;
			pHero.m_pre_exp = pHero.m_pre_exp - hero_getexp_max( curlevel, pHero.m_color );
			isup = isup + 1;
			playcount = playcount + 1
			--升级特效
			FightDlgLevelUpEffect(uiObj)
		end
		local endvalue = pHero.m_pre_exp/hero_getexp_max( pHero.m_level, pHero.m_color );
		
		-- 进度条
		SetProgressPlay( uiObj.transform:Find("Progress"), beginvalue, endvalue, 2, playcount, function() 
			-- 名称+等级
			level = level + 1
			if level > pHero.m_level then
				level = pHero.m_level
			end
			SetText( uiObj.transform:Find("Name"), HeroNameLv( pHero.m_kind, level ) )
		end )
	end
end

--升级特效
function FightDlgLevelUpEffect(uiObj)
	local effect = GameObject.Instantiate( LoadPrefab( "Jzsj" ) )
	effect.transform:SetParent( uiObj.transform );
	effect.transform.localPosition = Vector3.New( 0, 40, 0 );		
	effect.transform.localScale = Vector3.New( 110, 110, 110 );
	GameObject.Destroy(effect,2)
end
function FightDlgIsShow()
	if m_Dlg ~= nil then
		return IsActive( m_Dlg )
	end
	return false;
end
-- 设置等待数据
function FightDlgWait( callback, value )
	m_WaitCallback = callback;
	m_WaitValue = value;
end