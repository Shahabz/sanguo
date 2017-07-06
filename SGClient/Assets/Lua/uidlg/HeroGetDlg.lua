-- 界面
local m_Dlg = nil;
--------------------------------------------------------------
local m_uiHeroHead = nil; --UnityEngine.GameObject
local m_uiHeroColor = nil; --UnityEngine.GameObject
local m_uiHeroCorps = nil; --UnityEngine.GameObject
local m_uiHeroName = nil; --UnityEngine.GameObject
local m_uiGrowth = nil; --UnityEngine.GameObject
local m_uiAttack = nil; --UnityEngine.GameObject
local m_uiDefense = nil; --UnityEngine.GameObject
local m_uiTroops = nil; --UnityEngine.GameObject

-- 打开界面
function HeroGetDlgOpen()
	m_Dlg = eye.uiManager:Open( "HeroGetDlg" );
end

-- 隐藏界面
function HeroGetDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "HeroGetDlg" );
end

-- 删除界面
function HeroGetDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function HeroGetDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            HeroGetDlgClose();
        end
	end
end

-- 载入时调用
function HeroGetDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;
	m_uiHeroHead = objs[0];
	m_uiHeroColor = objs[1];
	m_uiHeroCorps = objs[2];
	m_uiHeroName = objs[3];
	m_uiGrowth = objs[4];
	m_uiAttack = objs[5];
	m_uiDefense = objs[6];
	m_uiTroops = objs[7];
end

-- 界面初始化时调用
function HeroGetDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function HeroGetDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function HeroGetDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function HeroGetDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function HeroGetDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
--{m_kind=0,m_color=0,m_level=0,m_corps=0,m_exp=0,m_exp_max=0,m_soldiers=0,m_state=0,m_attack_base=0,m_attack_wash=0,m_defense_base=0,m_defense_wash=0,m_troops_base=0,m_troops_wash=0,m_attack=0,m_defense=0,m_troops=0,m_offset=0,},
function HeroGetDlgShow( recvValue )
	HeroGetDlgOpen()
	SetImage( m_uiHeroHead, HeroHeadSprite( recvValue.m_kind )  );
	SetImage( m_uiHeroColor,  HeroColorSprite( recvValue.m_color )  );
	SetImage( m_uiHeroCorps,  CorpsSprite( recvValue.m_corps )  );
	SetText( m_uiHeroName, HeroName( recvValue.m_kind ) )
	
	local attack = recvValue.m_attack_base + recvValue.m_attack_wash;
	local defense = recvValue.m_defense_base + recvValue.m_defense_wash;
	local troops = recvValue.m_troops_base + recvValue.m_troops_wash;
	local growth = attack + defense + troops;

	SetText( m_uiGrowth, F( 137, growth ) ) -- 总资质
	SetText( m_uiAttack, F( 138, attack )  ) -- 攻资质
	SetText( m_uiDefense, F( 139, defense )  ) -- 防资质
	SetText( m_uiTroops, F( 140, troops )  ) -- 兵资质
end


