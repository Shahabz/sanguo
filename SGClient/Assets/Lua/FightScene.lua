FightScene = {};
FightScene.m_sceneObject = nil -- 战斗场景对象
FightScene.m_displayRoot = nil
FightScene.m_mapABName = "_ab_ui_static_fight_back_1"
FightScene.m_mapMainDlgIsActive = false;
FightScene.m_leftUnit = {}
FightScene.m_rightUnit = {}

local corps_shape = { 4, 5, 6 }
local unitpos = { 2.8,2.0,1.2, 0,-0.8,-1.6, -2.8,-3.6,-4.4 }
local pk_posx_left = { -0.4, -1.8, -2.6, -3.4, -4.2, -5, -5.8, -6.6, -7.4, -8.2, -9 }
local pk_posx_right = { 0.4, 1.8, 2.6, 3.4, 4.2, 5, 5.8, 6.6, 7.4, 8.2, 9 }

-- 创建
function FightScene.Create()
	FightScene.m_mapABName = "_ab_ui_static_fight_back_1"
	ResourceManager.LoadAssetBundle( FightScene.m_mapABName );
	ResourceManager.LoadAssetBundle( "_ab_fight"  );
	FightScene.m_sceneObject = GameObject.Instantiate( LoadPrefab("FightScene") );
	FightScene.m_displayRoot = FightScene.m_sceneObject.transform:Find( "Display" );
	MainDlgClose()
	FightScene.m_mapMainDlgIsActive = MapMainDlgIsActive()
	if FightScene.m_mapMainDlgIsActive == true then
		MapMainDlgClose()
	end
end

-- 销毁
function FightScene.Delete()
	if FightScene.m_sceneObject ~= nil then
		GameObject.Destroy( FightScene.m_sceneObject );
		FightScene.m_sceneObject = nil;
	end
	ResourceManager.UnloadAssetBundleImmediately( FightScene.m_mapABName )
	ResourceManager.UnloadAssetBundleImmediately( "_ab_fight" )
	MainDlgOpen()
	if FightScene.m_mapMainDlgIsActive == true then
		MapMainDlgShow()
	end
end


-- 创建军阵
function FightDlgCreateUnit()
	-- 攻击方军阵
	local unit = fight_nextptr( FIGHT_ATTACK )
	if unit ~= nil then
		
		--coroutine.stop( BagDlgCreateItem )
		--coroutine.start( BagDlgCreateItem )
		for line=1, unit.line, 1 do
			FightScene.m_leftUnit[line] = {}
			for i=1, 9, 1 do
				local charactor = Character.Create( corps_shape[unit.corps+1] );
				charactor.transform:SetParent( FightScene.m_displayRoot.transform );
				charactor.transform.localPosition = Vector3.New( pk_posx_left[line], unitpos[i], 0 );
				charactor.transform.localScale = Vector3.New( 1, 1, 1 );
				charactor.transform:Find("Sprite"):GetComponent("SpriteRenderer").sortingOrder  = 12;
				--charactor.transform:Find("Sprite").gameObject.layer = 9;
				charactor.transform:Find("Shadow"):GetComponent("SpriteRenderer").sortingOrder  = 11;
				--charactor.transform:Find("Shadow").gameObject.layer = 9;
				charactor:TurnTo( Vector3.New( -1, 0, 0 ) );
				--charactor:Walk();
				FightScene.m_leftUnit[line][i] = charactor
			end
		end
	end
	
	-- 防御方军阵
	unit = fight_nextptr( FIGHT_DEFENSE )
	if unit ~= nil then
		for line=1, unit.line, 1 do
			FightScene.m_rightUnit[line] = {}
			for i=1, 9, 1 do
				local charactor = Character.Create( corps_shape[unit.corps+1] );
				charactor.transform:SetParent( FightScene.m_displayRoot.transform );
				charactor.transform.localPosition = Vector3.New( pk_posx_right[line], unitpos[i], 0 );
				charactor.transform.localScale = Vector3.New( 1, 1, 1 );
				charactor.transform:Find("Sprite"):GetComponent("SpriteRenderer").sortingOrder  = 12;
				--charactor.transform:Find("Sprite").gameObject.layer = 9;
				charactor.transform:Find("Shadow"):GetComponent("SpriteRenderer").sortingOrder  = 11;
				--charactor.transform:Find("Shadow").gameObject.layer = 9;
				charactor:TurnTo( Vector3.New( 0, 0, 0 ) );
				--charactor:Walk();
				FightScene.m_rightUnit[line][i] = charactor
			end
		end
	end
end