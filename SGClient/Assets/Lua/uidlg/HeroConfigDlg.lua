-- 界面
local m_Dlg = nil;

local m_uiShape = nil; --UnityEngine.GameObject
local m_uiAbilityArea = nil; --UnityEngine.GameObject
local m_uiAttackBase = nil; --UnityEngine.GameObject
local m_uiDefenseBase = nil; --UnityEngine.GameObject
local m_uiTroopsBase = nil; --UnityEngine.GameObject
local m_uiTotalGrowth = nil; --UnityEngine.GameObject
local m_uiAttack = nil; --UnityEngine.GameObject
local m_uiDefense = nil; --UnityEngine.GameObject
local m_uiSoldier = nil; --UnityEngine.GameObject
local m_uiExpPanel = nil; --UnityEngine.GameObject
local m_uiSoldierPanel = nil; --UnityEngine.GameObject
local m_uiName = nil; --UnityEngine.GameObject
local m_uiOwnerWarn = nil; --UnityEngine.GameObject
local m_uiOwner = nil; --UnityEngine.GameObject
local m_uiAttackIncreaseIcon = nil; --UnityEngine.GameObject
local m_uiAttackIncrease = nil; --UnityEngine.GameObject
local m_uiDefenseIncreaseIcon = nil; --UnityEngine.GameObject
local m_uiDefenseIncrease = nil; --UnityEngine.GameObject
local m_uiLoyal = nil; --UnityEngine.GameObject

local m_recvValue = nil;

-- 打开界面
function HeroConfigDlgOpen( path )
	ResourceManager.LoadAssetBundle( "_ab_ui_heroinfodlg" );
	m_Dlg = eye.uiManager:Open( "HeroConfigDlg" );
	if path == 0 then
		m_DialogFrameMod = DialogFrameModOpen( m_Dlg, T(550), HELP_HeroDlg, HeroConfigDlgClose );
	elseif path == 1 then
		m_DialogFrameMod = DialogFrameModOpen( m_Dlg, T(1539), HELP_HeroDlg, HeroConfigDlgClose );
	end
end

-- 隐藏界面
function HeroConfigDlgClose()
	if m_Dlg == nil then
		return;
	end
	DialogFrameModClose( m_DialogFrameMod );
	m_DialogFrameMod = nil;	
	eye.uiManager:Close( "HeroConfigDlg" );
	m_recvValue = nil;
end

-- 删除界面
function HeroConfigDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
	Invoke( function() 
			ResourceManager.UnloadAssetBundleImmediately( "_ab_ui_heroinfodlg" )
	end, 0.3 );
	m_recvValue = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function HeroConfigDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            HeroConfigDlgClose();
		elseif nControlID == 1 then
			HeroConfigDlgViewActor();
        end
	end
end

-- 载入时调用
function HeroConfigDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiShape = objs[0];
	m_uiAbilityArea = objs[1];
	m_uiAttackBase = objs[2];
	m_uiDefenseBase = objs[3];
	m_uiTroopsBase = objs[4];
	m_uiTotalGrowth = objs[5];
	m_uiAttack = objs[6];
	m_uiDefense = objs[7];
	m_uiSoldier = objs[8];
	m_uiExpPanel = objs[9];
	m_uiSoldierPanel = objs[10];
	m_uiName = objs[11];
	m_uiOwnerWarn = objs[12];
	m_uiOwner = objs[13];
	m_uiAttackIncreaseIcon = objs[14];
	m_uiAttackIncrease = objs[15];
	m_uiDefenseIncreaseIcon = objs[16];
	m_uiDefenseIncrease = objs[17];
	m_uiLoyal = objs[18];
end

-- 界面初始化时调用
function HeroConfigDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function HeroConfigDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function HeroConfigDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function HeroConfigDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function HeroConfigDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function HeroConfigDlgShow( pHero, path )
	if pHero == nil or pHero.kind <= 0 then
		return
	end
	HeroConfigDlgOpen( path )
	
	-- 普通查看
	if path == 0 then
		HeroConfigDlgSetBase( pHero )
		
	elseif path == 1 then
		local kind = pHero.kind
		system_askinfo( ASKINFO_NATIONHERO, "", 5, kind );
	end
end

-- 接收
-- m_attr={m_kind=0,m_color=0,m_level=0,m_corps=0,m_exp=0,m_exp_max=0,m_soldiers=0,m_state=0,m_attack_base=0,m_attack_wash=0,m_defense_base=0,m_defense_wash=0,m_troops_base=0,m_troops_wash=0,m_attack=0,m_defense=0,m_troops=0,m_attack_increase=0,m_defense_increase=0,m_offset=0,m_god=0,},m_namelen=0,m_name="[m_namelen]",m_actorid=0,m_open=0,
function HeroConfigDlgRecv( recvValue )
	m_recvValue = recvValue;
	local pHero = {}
	pHero.kind = recvValue.m_attr.m_kind
	pHero.level = recvValue.m_attr.m_level
	pHero.exp = recvValue.m_attr.m_exp
	pHero.exp_max = recvValue.m_attr.m_exp_max
	pHero.soldiers = recvValue.m_attr.m_soldiers
	pHero.troops = recvValue.m_attr.m_troops
	pHero.attack_wash = recvValue.m_attr.m_attack_wash
	pHero.defense_wash = recvValue.m_attr.m_defense_wash
	pHero.troops_wash = recvValue.m_attr.m_troops_wash
	pHero.attack_base = recvValue.m_attr.m_attack_base
	pHero.defense_base = recvValue.m_attr.m_defense_base
	pHero.troops_base = recvValue.m_attr.m_troops_base
	pHero.attack = recvValue.m_attr.m_attack
	pHero.defense = recvValue.m_attr.m_defense
	pHero.troops = recvValue.m_attr.m_troops
	pHero.attack_increase = recvValue.m_attr.m_attack_increase
	pHero.defense_increase = recvValue.m_attr.m_defense_increase
	HeroConfigDlgSetBase( pHero )
	HeroConfigDlgSetOther( pHero )
	HeroConfigDlgSetOwner()
end


function HeroConfigDlgSetBase( pHero )
	-- 形象
	SetImage( m_uiShape, HeroFaceSprite( pHero.kind ) )	
	-- 名称
	SetText( m_uiName,HeroName(pHero.kind));
	-- 经验
	SetFalse( m_uiExpPanel )
	-- 兵力
	if pHero.soldiers ~= nil then
		SetProgress( m_uiSoldierPanel.transform:Find("Progress"), pHero.soldiers/pHero.troops )
		SetText( m_uiSoldierPanel.transform:Find("Text"), knum(pHero.soldiers).."/"..knum(pHero.troops) )
		SetImage( m_uiSoldierPanel.transform:Find("Corps"), CorpsSprite( hero_getcorps( pHero.kind ) ) )
		SetTrue( m_uiSoldierPanel )
	else
		SetFalse( m_uiSoldierPanel )
	end
	
	-- 属性
	SetText( m_uiAttackBase, T(143).." "..(pHero.attack_base+pHero.attack_wash) );
	SetText( m_uiDefenseBase, T(144).." "..(pHero.defense_base+pHero.defense_wash) );
	SetText( m_uiTroopsBase, T(145).." "..(pHero.troops_base+pHero.troops_wash) );
	
	local total = pHero.attack_base + pHero.defense_base + pHero.troops_base;
	local total_wash = pHero.attack_wash + pHero.defense_wash + pHero.troops_wash;
	SetText( m_uiTotalGrowth, T(149)..":"..(total).."+"..(total_wash) );
	SetText( m_uiAttack, T(146)..":"..pHero.attack );
	SetText( m_uiDefense, T(147)..":"..pHero.defense )
	SetText( m_uiSoldier, T(148)..":"..pHero.troops )
	
	-- 强攻
	if pHero.attack_increase ~= nil and pHero.attack_increase > 0 then
		SetTrue( m_uiAttackIncreaseIcon )
		SetTrue( m_uiAttackIncrease )
		SetText( m_uiAttackIncrease, T(165).."+"..pHero.attack_increase );
	else
		SetFalse( m_uiAttackIncreaseIcon )
		SetFalse( m_uiAttackIncrease )
	end
	
	-- 强防
	if pHero.defense_increase ~= nil and pHero.defense_increase > 0 then
		SetTrue( m_uiDefenseIncreaseIcon )
		SetTrue( m_uiDefenseIncrease )
		SetText( m_uiDefenseIncrease, T(166).."+"..pHero.defense_increase );
	else
		SetFalse( m_uiDefenseIncreaseIcon )
		SetFalse( m_uiDefenseIncrease )
	end
	
	SetFalse( m_uiAbilityArea )
	SetTrue( m_uiAbilityArea )

	-- 能力范围图
	local config = g_heroinfo[pHero.kind][hero_getnormalcolor( pHero.kind )];
	local uiPolygonChart = m_uiAbilityArea.transform:GetComponent( typeof(UIPolygonChart) );
	uiPolygonChart.VerticesDistances[0] = (pHero.attack_base+pHero.attack_wash)/(pHero.attack_base+config.attack_wash_limit)
	uiPolygonChart.VerticesDistances[1] = (pHero.defense_base+pHero.defense_wash)/(pHero.defense_base+config.defense_wash_limit)
	uiPolygonChart.VerticesDistances[2] = (pHero.troops_base+pHero.troops_wash)/(pHero.troops_base+config.troops_wash_limit)
	
	SetFalse( m_uiOwnerWarn )
	SetFalse( m_uiOwner )
	SetFalse( m_uiLoyal )
end

function HeroConfigDlgSetOther( pHero )
	-- 名称
	SetText( m_uiName, HeroNameLv(pHero.kind, pHero.level ) )
	-- 经验
	SetProgress( m_uiExpPanel.transform:Find("Progress"), pHero.exp/pHero.exp_max )
	SetText( m_uiExpPanel.transform:Find("Text"), knum(pHero.exp).."/"..knum(pHero.exp_max) )
	SetTrue( m_uiExpPanel )
end

-- 持有者
function HeroConfigDlgSetOwner()
	if m_recvValue.m_open == 0 then
		SetTrue( m_uiOwnerWarn )
		SetFalse( m_uiOwner )
		return
	end
	
	SetFalse( m_uiOwnerWarn )
	SetTrue( m_uiOwner )
	if m_recvValue.m_actorid > 0 then
		SetText( m_uiOwner.transform:Find("Name"), "<color=#"..NationColorStr(m_recvValue.m_nation)..">["..Nation( m_recvValue.m_nation ).."]</color>"..m_recvValue.m_name )
		SetTrue( m_uiOwner.transform:Find("ViewBtn") )
		
		-- 忠诚度
		local loyal = m_recvValue.m_loyal
		local maxloyal = 100;
		SetProgress( m_uiLoyal.transform:Find("Progress"), loyal/maxloyal );
		SetText( m_uiLoyal.transform:Find("Progress/Text"), loyal.."/"..maxloyal )
		SetTrue( m_uiLoyal )
	
	else
		SetText( m_uiOwner.transform:Find("Name"), T(2069) )
		SetFalse( m_uiOwner.transform:Find("ViewBtn") )
		SetFalse( m_uiLoyal )
	end
end

-- 查看
function HeroConfigDlgViewActor()
	if m_recvValue.m_actorid <= 0 then
		return
	end
	ActorSearchDlgShow( m_recvValue.m_actorid )
end
