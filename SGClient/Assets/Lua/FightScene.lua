FightScene = {};
FightScene.m_sceneObject = nil -- 战斗场景对象
FightScene.m_displayRoot = nil
FightScene.m_effectRoot = nil
FightScene.m_mapABName = "_ab_ui_static_fight_back_1"
FightScene.m_mapMainDlgIsActive = false;
FightScene.m_leftUnit = {}
FightScene.m_rightUnit = {}
FightScene.m_unitPool  = {}
FightScene.m_effectPool  = {}

-- 形象
local corps_shape = { 4, 5, 6 }

-- 每列每一个兵的Y坐标点
local UnitPosY = { 2.8,2.0,1.2, 0,-0.8,-1.6, -2.8,-3.6,-4.4 }

-- 入场前每一列X坐标点
local LeftUnitPrePosX = { -4.2, -5, -5.8, -6.6, -7.4, -8.2, -9, -9.8, -10.6, -11.4, -12.2 }
local RightUnitPrePosX = { 4.2, 5, 5.8, 6.6, 7.4, 8.2, 9, 9.8, 10.6, 11.4, 12.2 }

-- 入场后每一列X坐标点
local LeftUnitPosX = { -0.4, -1.8, -2.6, -3.4, -4.2, -5, -5.8, -6.6, -7.4, -8.2, -9 }
local RightUnitPosX = { 0.4, 1.8, 2.6, 3.4, 4.2, 5, 5.8, 6.6, 7.4, 8.2, 9 }

-- 攻击点
local BeatPos = { 2.0, -0.8, -3.6 }

-- 战斗速度
FightScene.m_speed = 0.8

-- 创建战场
function FightScene.Create()
	FightScene.m_mapABName = "_ab_ui_static_fight_back_1"
	ResourceManager.LoadAssetBundle( FightScene.m_mapABName );
	ResourceManager.LoadAssetBundle( "_ab_char_shape_fightunit"  );
	ResourceManager.LoadAssetBundle( "_ab_fight"  );
	FightScene.m_sceneObject = GameObject.Instantiate( LoadPrefab("FightScene") );
	FightScene.m_displayRoot = FightScene.m_sceneObject.transform:Find( "Display" );
	FightScene.m_effectRoot = FightScene.m_sceneObject.transform:Find( "Effect" );
	MainDlgClose()
	ChatDlgClose()
	NationEquipDlgClose();
	FightScene.m_mapMainDlgIsActive = MapMainDlgIsActive()
	if FightScene.m_mapMainDlgIsActive == true then
		MapMainDlgClose()
	end
	GameManager.ChangeScence( "fight" )
end

-- 销毁战场
function FightScene.Delete()
	if FightScene.m_sceneObject ~= nil then
		GameObject.Destroy( FightScene.m_sceneObject );
		FightScene.m_sceneObject = nil;
	end
	FightScene.m_displayRoot = nil
	FightScene.m_effectRoot = nil
	FightScene.m_leftUnit = {}
	FightScene.m_rightUnit = {}
	FightScene.m_unitPool  = {}
	FightScene.m_effectPool  = {}
	ResourceManager.UnloadAssetBundleImmediately( FightScene.m_mapABName )
	ResourceManager.UnloadAssetBundleImmediately( "_ab_fight" )
	ResourceManager.UnloadAssetBundleImmediately( "_ab_char_shape_fightunit"  );
	GameManager.ChangeScence( "current" )
	MainDlgOpen()
	MainDlgSetWorker()
	if FightScene.m_mapMainDlgIsActive == true then
		MapMainDlgShow()
		-- 城外bgm
		MapPlayAudio()
	else
		-- 城内bgm
		CityPlayAudio()
	end
	fight_destory()
end

-- 从士兵池获取一个士兵对象
function FightScene.PoolGetUnit( corps )
	local unitObj = nil
	if FightScene.m_unitPool[corps] ~= nil then
		for index, unit in pairs( FightScene.m_unitPool[corps] ) do
			if unit and unit.gameObject.activeSelf == false then
				unitObj = unit;
				break;
			end
		end
	end
	-- 缓存没有，那么创建一个，并放进缓存
	if unitObj == nil then
		unitObj = Character.Create( corps_shape[corps] );
		unitObj.transform:Find("Sprite"):GetComponent("SpriteRenderer").sortingOrder  = 12;
		--unitObj.transform:Find("Sprite").gameObject.layer = 9;
		unitObj.transform:Find("Shadow"):GetComponent("SpriteRenderer").sortingOrder  = 11;
		--unitObj.transform:Find("Shadow").gameObject.layer = 9;
		if FightScene.m_unitPool[corps] == nil then
			FightScene.m_unitPool[corps] = {}
		end
		table.insert( FightScene.m_unitPool[corps], unitObj );
	end
	unitObj.gameObject:SetActive( true );
	return unitObj
end

-- 从士兵池释放一个士兵对象
function FightScene.PoolReleaseUnit( unitObj )
	if unitObj ~= nil then
		unitObj.gameObject:SetActive( false );
	end
end
	
-- 创建军阵
function FightScene.UnitCreate( pos, unit )
	if FightScene.m_displayRoot == nil then
		return
	end
	if unit == nil then
		return
	end
	if pos == FIGHT_ATTACK then
		for line=1, unit.line_left, 1 do
			FightScene.m_leftUnit[line] = {}
			
			local unitcount = 9;
			if line == unit.line_left and unit.troops > unit.maxhp then
				-- 战斗中武将每排兵力=武将兵力属性值/武将总带兵排数,结果向下取整
				local line_troops = math.floor( unit.troops / unit.line );
				-- 最后一排兵力=（可带兵力-实际参战兵力）% 每排兵力
				local line_hp
				if unit.troops - unit.maxhp >= line_troops then
					line_hp = line_troops;
				else
					line_hp = (unit.troops - unit.maxhp) % line_troops;
				end
				-- 战斗中最后一排士兵个数=最后一排兵力/每排兵力*9，结果向上取整
				unitcount =  math.floor( line_hp/line_troops*9 )
			end
			
			for i=1, unitcount, 1 do
				local charactor = FightScene.PoolGetUnit( unit.corps+1 );
				charactor.transform:SetParent( FightScene.m_displayRoot.transform );
				charactor.transform.localPosition = Vector3.New( LeftUnitPrePosX[line], UnitPosY[i], 0 );
				charactor.transform.localScale = Vector3.New( 1, 1, 1 );
				charactor:TurnTo( Vector3.New( LeftUnitPosX[1], UnitPosY[i], 0 ) );
				--charactor:Walk();
				-- 军阵管理列表
				FightScene.m_leftUnit[line][i] = charactor
			end
		end
	else
		for line=1, unit.line_left, 1 do
			FightScene.m_rightUnit[line] = {}
			
			local unitcount = 9;
			if line == unit.line_left and unit.troops > unit.maxhp then
				-- 战斗中武将每排兵力=武将兵力属性值/武将总带兵排数,结果向下取整
				local line_troops = math.floor( unit.troops / unit.line );
				-- 最后一排兵力=（可带兵力-实际参战兵力）% 每排兵力
				local line_hp
				if unit.troops - unit.maxhp >= line_troops then
					line_hp = line_troops;
				else
					line_hp = (unit.troops - unit.maxhp) % line_troops;
				end
				-- 战斗中最后一排士兵个数=最后一排兵力/每排兵力*9，结果向上取整
				unitcount =  math.floor( line_hp/line_troops*9 )
			end
			
			for i=1, unitcount, 1 do
				local charactor = FightScene.PoolGetUnit( unit.corps+1 );
				charactor.transform:SetParent( FightScene.m_displayRoot.transform );
				charactor.transform.localPosition = Vector3.New( RightUnitPrePosX[line], UnitPosY[i], 0 );
				charactor.transform.localScale = Vector3.New( 1, 1, 1 );
				charactor:TurnTo( Vector3.New( RightUnitPosX[1], UnitPosY[i], 0 ) );
				--charactor:Walk();
				-- 军阵管理列表
				FightScene.m_rightUnit[line][i] = charactor
			end
		end
	end
end


-- 部队入场
function FightScene.UnitWalk( pos, unit )
	if pos == FIGHT_ATTACK then
		for line=1, unit.line_left, 1 do
			if FightScene.m_leftUnit[line] ~= nil then
				for k, v in pairs ( FightScene.m_leftUnit[line] ) do
					local charactor = v;
					charactor:SetSpeed( math.random( 20, 23 )/(10*FightScene.m_speed) )
					charactor:MoveTo( Vector3.New( LeftUnitPosX[line], charactor.transform.localPosition.y, charactor.transform.localPosition.z ) )
					charactor:Walk();
				end
			end
		end
	else
		for line=1, unit.line_left, 1 do
			if FightScene.m_rightUnit[line] ~= nil then
				for k, v in pairs ( FightScene.m_rightUnit[line] ) do
					local charactor = v;
					charactor:SetSpeed( math.random( 20, 23 )/(10*FightScene.m_speed) )
					charactor:MoveTo( Vector3.New( RightUnitPosX[line], charactor.transform.localPosition.y, charactor.transform.localPosition.z ) )
					charactor:Walk();
				end
			end
		end
	end
end

-- 第一排攻击
function FightScene.UnitAttack( pos )
	if pos == FIGHT_ATTACK then
		if FightScene.m_leftUnit[1] == nil then
			return
		end
		for k, v in pairs ( FightScene.m_leftUnit[1] ) do
			local charactor = v;
			charactor:Attack();
		end
	else
		if FightScene.m_rightUnit[1] == nil then
			return
		end
		for k, v in pairs ( FightScene.m_rightUnit[1] ) do
			local charactor = v;
			charactor:Attack();
		end
	end
end

-- 死亡一排
function FightScene.UnitDead( pos )
	if pos == FIGHT_ATTACK then
		if FightScene.m_leftUnit[1] == nil then
			return
		end
		for k, v in pairs ( FightScene.m_leftUnit[1] ) do
			local charactor = v;
			charactor.disableOnHide = true;
			charactor.hideOnDeath = true
			charactor:Die();
		end
		table.remove( FightScene.m_leftUnit, 1 )
	else
		if FightScene.m_rightUnit[1] == nil then
			return
		end
		for k, v in pairs ( FightScene.m_rightUnit[1] ) do
			local charactor = v;
			charactor.disableOnHide = true;
			charactor.hideOnDeath = true
			charactor:Die();
		end
		table.remove( FightScene.m_rightUnit, 1 );
	end
end

-- 从特效池获取一个特效对象
function FightScene.PoolGetEffect( name )
	if FightScene.m_effectRoot == nil then
		return nil
	end
	local effectObj = nil
	if FightScene.m_effectPool[name] ~= nil then
		for index, effect in pairs( FightScene.m_effectPool[name] ) do
			if effect and effect.gameObject.activeSelf == false then
				effectObj = effect;
				break;
			end
		end
	end
	-- 缓存没有，那么创建一个，并放进缓存
	if effectObj == nil then
		effectObj = GameObject.Instantiate( LoadPrefab( name ) )
		effectObj.transform:SetParent( FightScene.m_effectRoot.transform );
		if FightScene.m_effectPool[name] == nil then
			FightScene.m_effectPool[name] = {}
		end
		table.insert( FightScene.m_effectPool[name], effectObj );
	end
	effectObj.gameObject:SetActive( true );
	return effectObj
end

-- 释放一个特效对象到特效池
function FightScene.PoolReleaseEffect( effectObj )
	if effectObj ~= nil then
		effectObj.gameObject:SetActive( false );
	end
end

-- 播放被击特效
function FightScene.PlayBeat()
	local list = {}
	for i=1,3,1 do
		local effectObj = FightScene.PoolGetEffect( "Tysj" )
		if effectObj == nil then
			return
		end
		effectObj.transform.localPosition = Vector3.New( 0, BeatPos[i], 0 );
		effectObj.transform.localScale = Vector3.New( 1, 1, 1 );
		table.insert( list, effectObj )
	end
	Invoke( function() 
		for i=1, #list, 1 do
			FightScene.PoolReleaseEffect( list[i] )
		end
	end, 0.5, nil, "FightInvoke_PlayBeat");
end

-- ax ay 进攻方技能位置偏移量
-- dx dy 防御方技能位置偏移量
-- s 技能方向
-- 步兵 jn3 骑兵 jn1 弓兵 jn2
-- 播放技能特效
function FightScene.PlaySkill( pos, skillid )
	local skill = { {name="Jn3",ax=0,ay=-0.7, dx=0,dy=-0.7,s = -1},{name="Jn1",ax=0.1,ay=0.14, dx=-0.1,dy=0.14,s = 1},{name="Jn2",ax=-0.12,ay=-0.67, dx=0.12,dy=-0.67,s = -1} }
	--PrintTable(skill,"skill")
	local effectObj = FightScene.PoolGetEffect( skill[skillid].name )
	if effectObj == nil then
		return
	end
	if pos == FIGHT_ATTACK then
		effectObj.transform.localPosition = Vector3.New( skill[skillid].ax, skill[skillid].ay, 0 );
		effectObj.transform.localScale = Vector3.New( -7*skill[skillid].s, 7, 7 );
	else
		effectObj.transform.localPosition = Vector3.New( skill[skillid].dx, skill[skillid].dy, 0 );
		effectObj.transform.localScale = Vector3.New( 7*skill[skillid].s, 7, 7 );
	end
	Invoke( function()
		FightScene.PoolReleaseEffect( effectObj )
	end, 1, nil, "FightInvoke_PlaySkill");
end