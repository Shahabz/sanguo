-- 界面
local m_Dlg = nil;
local m_DialogFrameMod = nil;

local m_uiShape = nil; --UnityEngine.GameObject
local m_uiEquip = {}; --UnityEngine.GameObject
local m_uiAbilityArea = nil; --UnityEngine.GameObject
local m_uiAttackBase = nil; --UnityEngine.GameObject
local m_uiDefenseBase = nil; --UnityEngine.GameObject
local m_uiTroopsBase = nil; --UnityEngine.GameObject
local m_uiTotalGrowth = nil; --UnityEngine.GameObject
local m_uiAttack = nil; --UnityEngine.GameObject
local m_uiDefense = nil; --UnityEngine.GameObject
local m_uiSoldier = nil; --UnityEngine.GameObject
local m_uiUpgradeBtn = nil; --UnityEngine.GameObject
local m_uiWashBtn = nil; --UnityEngine.GameObject

-- 打开界面
function HeroInfoDlgOpen()
	m_Dlg = eye.uiManager:Open( "HeroInfoDlg" );
	m_DialogFrameMod = DialogFrameModOpen( m_Dlg, "武将", 3, HeroInfoDlgClose );
end

-- 隐藏界面
function HeroInfoDlgClose()
	if m_Dlg == nil then
		return;
	end
	DialogFrameModClose( m_DialogFrameMod );
	eye.uiManager:Close( "HeroInfoDlg" );
end

-- 删除界面
function HeroInfoDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function HeroInfoDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            HeroInfoDlgClose();
        end
	end
end

-- 载入时调用
function HeroInfoDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiShape = objs[0];
	m_uiEquip[0] = objs[1];
	m_uiEquip[1] = objs[2];
	m_uiEquip[2] = objs[3];
	m_uiEquip[3] = objs[4];
	m_uiEquip[4] = objs[5];
	m_uiEquip[5] = objs[6];
	m_uiAbilityArea = objs[7];
	m_uiAttackBase = objs[8];
	m_uiDefenseBase = objs[9];
	m_uiTroopsBase = objs[10];
	m_uiTotalGrowth = objs[11];
	m_uiAttack = objs[12];
	m_uiDefense = objs[13];
	m_uiSoldier = objs[14];
	m_uiUpgradeBtn = objs[15];
	m_uiWashBtn = objs[16];
end

-- 界面初始化时调用
function HeroInfoDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function HeroInfoDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function HeroInfoDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function HeroInfoDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function HeroInfoDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------

function HeroInfoDlgShow( pHero )
	HeroInfoDlgOpen();
	local uiPolygonChart = m_uiAbilityArea.transform:GetComponent( typeof(UIPolygonChart) );
	uiPolygonChart.VerticesDistances[0] = 0.5
	uiPolygonChart.VerticesDistances[1] = 0.5
	uiPolygonChart.VerticesDistances[2] = 0.5
	
	--SetImage( m_uiShape, PlayerFaceSprite( pHero.m_kind ) )
	
	SetText( m_uiAttackBase, T(143).." "..pHero.m_attack_base );
	SetText( m_uiDefenseBase, T(144).." "..pHero.m_defense_base );
	SetText( m_uiTroopsBase, T(145).." "..pHero.m_troops_base );
	
	local total = pHero.m_attack_base + pHero.m_defense_base + pHero.m_troops_base;
	local total_wash = pHero.m_attack_wash + pHero.m_defense_wash + pHero.m_troops_wash;
	SetText( m_uiTotalGrowth, T(149)..":"..(total).."+"..(total_wash) );
	SetText( m_uiAttack,  T(146)..":"..pHero.m_attack );
	SetText( m_uiDefense,  T(147)..":"..pHero.m_defense )
	SetText( m_uiSoldier,  T(148)..":"..pHero.m_troops )
end
