FightScene = {};
FightScene.m_sceneObject = nil -- 战斗场景对象
FightScene.m_displayRoot = nil
FightScene.m_mapABName = "_ab_ui_static_fight_back_1"
FightScene.m_mapMainDlgIsActive = false;
FightScene.m_leftUnit = {}
FightScene.m_rightUnit = {}
FightScene.m_objectPool  = {}

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


-- 创建战场
function FightScene.Create()
	FightScene.m_mapABName = "_ab_ui_static_fight_back_1"
	ResourceManager.LoadAssetBundle( FightScene.m_mapABName );
	ResourceManager.LoadAssetBundle( "_ab_fight"  );
	ResourceManager.LoadAssetBundle( "_ab_char_shape_fightunit"  );
	FightScene.m_sceneObject = GameObject.Instantiate( LoadPrefab("FightScene") );
	FightScene.m_displayRoot = FightScene.m_sceneObject.transform:Find( "Display" );
	MainDlgClose()
	ChatDlgClose()
	FightScene.m_mapMainDlgIsActive = MapMainDlgIsActive()
	if FightScene.m_mapMainDlgIsActive == true then
		MapMainDlgClose()
	end
	
	-- 战斗bgm
	eye.audioManager:Play(202);
end

-- 销毁战场
function FightScene.Delete()
	if FightScene.m_sceneObject ~= nil then
		GameObject.Destroy( FightScene.m_sceneObject );
		FightScene.m_sceneObject = nil;
	end
	FightScene.m_leftUnit = {}
	FightScene.m_rightUnit = {}
	FightScene.m_objectPool  = {}
	ResourceManager.UnloadAssetBundleImmediately( FightScene.m_mapABName )
	ResourceManager.UnloadAssetBundleImmediately( "_ab_fight" )
	ResourceManager.UnloadAssetBundleImmediately( "_ab_char_shape_fightunit"  );
	MainDlgOpen()
	if FightScene.m_mapMainDlgIsActive == true then
		MapMainDlgShow()
		-- 城外bgm
		eye.audioManager:Play(202);
	else
		-- 城内bgm
		eye.audioManager:Play(203);
	end
	fight_destory()
end

-- 从士兵池获取一个士兵对象
function FightScene.PoolGet( corps )
	local unitObj = nil
	if FightScene.m_objectPool[corps] ~= nil then
		for index, unit in pairs( FightScene.m_objectPool[corps] ) do
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
		if FightScene.m_objectPool[corps] == nil then
			FightScene.m_objectPool[corps] = {}
		end
		table.insert( FightScene.m_objectPool[corps], unitObj );
	end
	unitObj.gameObject:SetActive( true );
	return unitObj
end

-- 从士兵池获取一个士兵对象
function FightScene.PoolRelease( unitObj )
	if unitObj ~= nil then
		unitObj.gameObject:SetActive( false );
	end
end
	
-- 创建军阵
function FightScene.UnitCreate( pos, unit )
	if unit == nil then
		return
	end
	if pos == FIGHT_ATTACK then
		for line=1, unit.line, 1 do
			FightScene.m_leftUnit[line] = {}
			for i=1, 9, 1 do
				local charactor = FightScene.PoolGet( unit.corps+1 );
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
		for line=1, unit.line, 1 do
			FightScene.m_rightUnit[line] = {}
			for i=1, 9, 1 do
				local charactor = FightScene.PoolGet( unit.corps+1 );
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
		for line=1, unit.line, 1 do
			if FightScene.m_leftUnit[line] ~= nil then
				for k, v in pairs ( FightScene.m_leftUnit[line] ) do
					local charactor = v;
					charactor:SetSpeed( math.random( 18, 21 )/10 )
					charactor:MoveTo( Vector3.New( LeftUnitPosX[line], charactor.transform.localPosition.y, charactor.transform.localPosition.z ) )
					charactor:Walk();
				end
			end
		end
	else
		for line=1, unit.line, 1 do
			if FightScene.m_rightUnit[line] ~= nil then
				for k, v in pairs ( FightScene.m_rightUnit[line] ) do
					local charactor = v;
					charactor:SetSpeed( math.random( 18, 21 )/10 )
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
