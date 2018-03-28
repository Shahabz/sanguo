-- 界面
local m_Dlg = nil;
--------------------------------------------------------------
local m_uiHeroHead = nil; --UnityEngine.GameObject
local m_uiHeroColor = nil; --UnityEngine.GameObject
local m_uiHeroCorps = nil; --UnityEngine.GameObject
local m_uiHeroName = nil; --UnityEngine.GameObject
local m_uiAttack = nil; --UnityEngine.GameObject
local m_uiDefense = nil; --UnityEngine.GameObject
local m_uiTroops = nil; --UnityEngine.GameObject
local m_uiGrowth = nil; --UnityEngine.GameObject
local m_uiCloseBtn = nil; --UnityEngine.GameObject
local m_uiHaveCloseBtn = nil; --UnityEngine.GameObject
local m_uiText = nil; --UnityEngine.GameObject

local m_herokind = 0;
local m_WaitCallback = nil;
local m_WaitValue = nil;

-- 打开界面
function HeroGetDlgOpen()
	ResourceManager.LoadAssetBundle( "_ab_ui_static_npc1" );
	m_Dlg = eye.uiManager:Open( "HeroGetDlg" );
end

-- 隐藏界面
function HeroGetDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "HeroGetDlg" );
	HeroVisitDlgCloseHeroGetEvent();
	
	if m_WaitCallback then
		m_WaitCallback( m_WaitValue );
	end
	m_WaitCallback = nil;
	m_WaitValue = nil;
end

-- 删除界面
function HeroGetDlgDestroy()
	GameObject.Destroy( m_Dlg );
	ResourceManager.UnloadAssetBundle( "_ab_ui_static_npc1" )
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
	m_uiAttack = objs[5];
	m_uiDefense = objs[6];
	m_uiTroops = objs[7];
	m_uiGrowth = objs[8];
	m_uiCloseBtn = objs[9];
	m_uiHaveCloseBtn = objs[10];
	m_uiText = objs[11];
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
	m_herokind = recvValue.m_kind+AWARDKIND_HEROBASE;
	local b_haveHero = HeroVisitDlgIsHaveHero( m_herokind );
	if b_haveHero == true and recvValue.m_itemnum ~= nil then
		SetTrue( m_uiHaveCloseBtn );
		SetFalse( m_uiCloseBtn );
		SetText( m_uiText, F( 1968, recvValue.m_itemnum ) );
	else
		SetFalse( m_uiHaveCloseBtn );
		SetTrue( m_uiCloseBtn );
		HeroVisitDlgAddHero(m_herokind);
	end
	SetImage( m_uiHeroHead, HeroHeadSprite( recvValue.m_kind )  );
	SetImage( m_uiHeroColor,  ItemColorSprite( recvValue.m_color )  );
	SetImage( m_uiHeroCorps,  CorpsSprite( recvValue.m_corps )  );
	SetText( m_uiHeroName, HeroName( recvValue.m_kind ).."("..T(134+recvValue.m_corps)..")", NameColor(recvValue.m_color) )
	
	local attack = recvValue.m_attack_base;
	local defense = recvValue.m_defense_base;
	local troops = recvValue.m_troops_base;
	local basegrowth = attack + defense + troops;
	local washgrowth = recvValue.m_attack_wash + recvValue.m_defense_wash + recvValue.m_troops_wash;
	
	if washgrowth ~= 0 then
		SetText( m_uiGrowth, T( 1671 ).."<color=#FFFFFF>"..basegrowth.."</color> +"..washgrowth ,NameColor(recvValue.m_color) ) --洗髓资质不为0才显示
	else
		SetText( m_uiGrowth, T( 1671 )..basegrowth)
	end
	
	SetText( m_uiAttack, F( 138, attack+recvValue.m_attack_wash )  ) -- 攻资质
	SetText( m_uiDefense, F( 139, defense+ recvValue.m_defense_wash )  ) -- 防资质
	SetText( m_uiTroops, F( 140, troops+ recvValue.m_troops_wash )  ) -- 兵资质
end

function HeroGetDlgIsShow()
	if m_Dlg ~= nil then
		return IsActive( m_Dlg )
	end
	return false;
end

-- 设置等待数据
function HeroGetDlgWait( callback, value )
	m_WaitCallback = callback;
	m_WaitValue = value;
end
