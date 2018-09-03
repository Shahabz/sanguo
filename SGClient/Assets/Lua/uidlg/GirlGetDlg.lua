-- 界面
local m_Dlg = nil;
local m_uiShape = nil; --UnityEngine.GameObject
local m_uiColor = nil; --UnityEngine.GameObject
local m_uiName = nil; --UnityEngine.GameObject
local m_uiAttackIncrease = nil; --UnityEngine.GameObject
local m_uiDefenseIncrease = nil; --UnityEngine.GameObject
local m_uiAttackGrowth = nil; --UnityEngine.GameObject
local m_uiDefenseGrowth = nil; --UnityEngine.GameObject
local m_uiTroopsGrowth = nil; --UnityEngine.GameObject
local m_uiCloseBtn = nil; --UnityEngine.GameObject
local m_uiHaveCloseBtn = nil; --UnityEngine.GameObject

local m_girlkind = 0;
local m_WaitCallback = nil;
local m_WaitValue = nil;

-- 打开界面
function GirlGetDlgOpen()
	m_Dlg = eye.uiManager:Open( "GirlGetDlg" );
end

-- 隐藏界面
function GirlGetDlgClose()
	if m_Dlg == nil then
		return;
	end
	eye.uiManager:Close( "GirlGetDlg" );
	
	if m_WaitCallback then
		m_WaitCallback( m_WaitValue );
	end
	m_WaitCallback = nil;
	m_WaitValue = nil;
	
	DelayQueueNext()
end

-- 删除界面
function GirlGetDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function GirlGetDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            GirlGetDlgClose();
        end
	end
end

-- 载入时调用
function GirlGetDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiShape = objs[0];
	m_uiColor = objs[1];
	m_uiName = objs[2];
	m_uiAttackIncrease = objs[3];
	m_uiDefenseIncrease = objs[4];
	m_uiAttackGrowth = objs[5];
	m_uiDefenseGrowth = objs[6];
	m_uiTroopsGrowth = objs[7];
	m_uiCloseBtn = objs[8];
	m_uiHaveCloseBtn = objs[9];
end

-- 界面初始化时调用
function GirlGetDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function GirlGetDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function GirlGetDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function GirlGetDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function GirlGetDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
-- m_kind=0,m_path=0,m_soulnum=0,m_girl={m_kind=0,m_color=0,m_soul=0,m_love_level=0,m_love_exp=0,m_love_num=0,},
function GirlGetDlgShow( recvValue )
	GirlGetDlgOpen()
	if recvValue.m_soulnum > 0 then
		SetFalse( m_uiCloseBtn );
		SetTrue( m_uiHaveCloseBtn );
		SetText( m_uiHaveCloseBtn.transform:Find("Back/Text"), F( 3335, GirlName(recvValue.m_girl.m_kind), recvValue.m_soulnum ) );
	else
		SetTrue( m_uiCloseBtn );
		SetFalse( m_uiHaveCloseBtn );
	end
	local config = girlconfig( recvValue.m_girl.m_kind, recvValue.m_girl.m_color )
	if config == nil then
		return
	end
	SetImage( m_uiShape, GirlFaceSprite( recvValue.m_girl.m_kind ) );
	SetImage( m_uiColor,  HeroColorSprite( recvValue.m_girl.m_color) )
	SetText( m_uiName, GirlName( recvValue.m_girl.m_kind ).."("..GirlType( config.type )..")", NameColor(recvValue.m_girl.m_color) )
	-- 强攻
	if config.attack_increase > 0 then
		SetText(m_uiAttackIncrease.transform:Find("Text"), T(165).."+"..config.attack_increase)
		SetTrue(m_uiAttackIncrease)
	else
		SetFalse(m_uiAttackIncrease)
	end
	-- 强防
	if config.defense_increase > 0 then
		SetText(m_uiDefenseIncrease.transform:Find("Text"), T(166).."+"..config.defense_increase)
		SetTrue(m_uiDefenseIncrease)
	else
		SetFalse(m_uiDefenseIncrease)
	end
	-- 攻击资质
	if config.attack_growth > 0 then
		SetText(m_uiAttackGrowth.transform:Find("Text"), F(3336,config.attack_growth))
		SetTrue(m_uiAttackGrowth)
	else
		SetFalse(m_uiAttackGrowth)
	end
	-- 防御资质
	if config.defense_growth > 0 then
		SetText(m_uiDefenseGrowth.transform:Find("Text"), F(3337,config.defense_growth))
		SetTrue(m_uiDefenseGrowth)
	else
		SetFalse(m_uiDefenseGrowth)
	end
	-- 兵力资质
	if config.troops_growth > 0 then
		SetText(m_uiTroopsGrowth.transform:Find("Text"), F(3338,config.troops_growth))
		SetTrue(m_uiTroopsGrowth)
	else
		SetFalse(m_uiTroopsGrowth)
	end
end

function GirlGetDlgIsShow()
	if m_Dlg ~= nil then
		return IsActive( m_Dlg )
	end
	return false;
end

-- 设置等待数据
function GirlGetDlgWait( callback, value )
	m_WaitCallback = callback;
	m_WaitValue = value;
end

function GirlGetDlgShowByFansghi(girlkind,callback)
	local recvValue = {};
	recvValue.m_girl = {};
	recvValue.m_soulnum = 0;
	recvValue.m_girl.m_kind = girlkind;
	recvValue.m_girl.m_color = g_girlinfo[girlkind][0].init_color;
	
	GirlGetDlgShow( recvValue )
	GirlGetDlgWait(callback)
end