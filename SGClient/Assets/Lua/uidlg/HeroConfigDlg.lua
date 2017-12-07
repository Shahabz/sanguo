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


-- 打开界面
function HeroConfigDlgOpen()
	m_Dlg = eye.uiManager:Open( "HeroConfigDlg" );
	m_DialogFrameMod = DialogFrameModOpen( m_Dlg, T(550), 3, HeroConfigDlgClose );
end

-- 隐藏界面
function HeroConfigDlgClose()
	if m_Dlg == nil then
		return;
	end
	DialogFrameModClose( m_DialogFrameMod );
	m_DialogFrameMod = nil;	
	eye.uiManager:Close( "HeroConfigDlg" );
end

-- 删除界面
function HeroConfigDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;

end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function HeroConfigDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            HeroConfigDlgClose();
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
function HeroConfigDlgShow( pHero)
	if pHero == nil or pHero.kind <= 0 then
		return
	end
	HeroConfigDlgOpen()
	HeroConfigDlgSet(  pHero )
end
function HeroConfigDlgSet( pHero)
	
	m_pCacheHero = pHero;	
	-- 形象
	SetImage( m_uiShape, HeroFaceSprite( pHero.kind ) )	
	-- 名称
	--SetText( m_uiName, HeroNameLv(pHero.m_kind, pHero.m_level ) )
	SetText( m_uiName,pHero.name);
	-- 经验
	--SetProgress( m_uiExpPanel.transform:Find("Progress"), pHero.m_exp/pHero.m_exp_max )
	--SetText( m_uiExpPanel.transform:Find("Text"), knum(pHero.m_exp).."/"..knum(pHero.m_exp_max) )
	-- 兵力
	SetProgress( m_uiSoldierPanel.transform:Find("Progress"), pHero.troops_wash/pHero.troops_wash )
	SetText( m_uiSoldierPanel.transform:Find("Text"), knum(pHero.troops_wash).."/"..knum(pHero.troops_wash) )
	SetImage( m_uiSoldierPanel.transform:Find("Corps"), CorpsSprite( pHero.corps ) )
	-- 属性
	SetText( m_uiAttackBase, T(143).." "..(pHero.attack_base+pHero.attack_wash) );
	SetText( m_uiDefenseBase, T(144).." "..(pHero.defense_base+pHero.defense_wash) );
	SetText( m_uiTroopsBase, T(145).." "..(pHero.troops_base+pHero.troops_wash) );
	
	local total = pHero.attack_base + pHero.defense_base + pHero.troops_base;
	local total_wash = pHero.attack_wash + pHero.defense_wash + pHero.troops_wash;
	SetText( m_uiTotalGrowth, T(149)..":"..(total).."+"..(total_wash) );
	SetText( m_uiAttack,  T(146)..":"..pHero.attack );
	SetText( m_uiDefense,  T(147)..":"..pHero.defense )
	SetText( m_uiSoldier,  T(148)..":"..pHero.troops )

	SetFalse( m_uiAbilityArea )
	SetTrue( m_uiAbilityArea )

	-- 能力范围图
	local uiPolygonChart = m_uiAbilityArea.transform:GetComponent( typeof(UIPolygonChart) );
	uiPolygonChart.VerticesDistances[0] = pHero.attack_base/total
	uiPolygonChart.VerticesDistances[1] = pHero.defense_base/total
	uiPolygonChart.VerticesDistances[2] = pHero.troops_base/total
end



