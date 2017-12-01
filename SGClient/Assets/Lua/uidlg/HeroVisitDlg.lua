-- 界面
local m_Dlg = nil;
local m_DialogFrameMod = nil;
local m_uiDot = nil; --UnityEngine.GameObject
local m_uiGodHero = nil; --UnityEngine.GameObject
local m_uiGodHeroGray = nil; --UnityEngine.GameObject
local m_uiGoodOpenTime = nil; --UnityEngine.GameObject
local m_uiGodOpenTime = nil; --UnityEngine.GameObject

-- 打开界面
function HeroVisitDlgOpen()
	ResourceManager.LoadAssetBundle( "_ab_ui_static_herovisit_gray" );
	ResourceManager.LoadAssetBundle( "_ab_ui_static_herovisit_up" );
	m_Dlg = eye.uiManager:Open( "HeroVisitDlg" );
	m_DialogFrameMod = DialogFrameModOpen( m_Dlg, T(1923), HELP_HeroDlg, HeroVisitDlgClose );
end

-- 隐藏界面
function HeroVisitDlgClose()
	if m_Dlg == nil then
		return;
	end
	DialogFrameModClose( m_DialogFrameMod );
	m_DialogFrameMod = nil;
	eye.uiManager:Close( "HeroVisitDlg" );
end

-- 删除界面
function HeroVisitDlgDestroy()
	GameObject.Destroy( m_Dlg );
	ResourceManager.UnloadAssetBundle( "_ab_ui_static_herovisit_gray" )
	ResourceManager.UnloadAssetBundle( "_ab_ui_static_herovisit_up" )
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function HeroVisitDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            HeroVisitDlgClose();
        end
	end
end

-- 载入时调用
function HeroVisitDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiDot = objs[0];
	m_uiGodHero = objs[1];
	m_uiGodHeroGray = objs[2];
	m_uiGoodOpenTime = objs[3];
	m_uiGodOpenTime = objs[4];
	
end

-- 界面初始化时调用
function HeroVisitDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function HeroVisitDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function HeroVisitDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function HeroVisitDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function HeroVisitDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function HeroVisitDlgShow()
	HeroVisitDlgOpen()
	system_askinfo( ASKINFO_HERO_VISIT, "", 0 );
	
end

-- m_hv_free_cd=0,m_hv_high_sec=0,m_hv_high_free=0,m_hv_low_num=0,m_hv_high_num=0,m_hv_progress=0,
function HeroVisitDlgRecv( recvValue )
	-- m_hv_free_cd=0,良将寻访免费CD
	-- m_hv_high_sec=0,神将寻访解锁时长
	-- m_hv_high_free=0,神将寻访剩余免费次数
	-- m_hv_low_num=0,良将寻访次数
	-- m_hv_high_num=0,神将寻访次数
	-- m_hv_progress=0,寻访进度
	
--[[
global.hero_visit_actorlevel = 88 --武将寻访玩家等级限制
global.hero_visit_mainlevel = 17 --武将寻访官府等级限制
global.hero_visit_low_token10 = 1800 --良将寻访十连钻石
global.hero_visit_high_token = 800 --神将寻访钻石
global.hero_visit_high_token10 = 6400 --神将寻访十连钻石
global.hero_visit_low_itemnum = 1 --良将寻访道具数
global.hero_visit_low_itemnum10 = 10 --良将寻访十连道具数
global.hero_visit_high_itemnum = 1 --神将寻访道具数
global.hero_visit_high_itemnum10 = 10 --神将寻访十连道具数
global.hero_visit_low_max = 10 --良将特殊抽取达到次数
global.hero_visit_high_max = 10 --神将特殊抽取达到次数
global.hero_visit_low_normal_award = 1 --良将普通寻访获得道具和武将
global.hero_visit_low_hero_award = 2 --良将特殊寻访获得武将
global.hero_visit_high_normal_award = 3 --神将普通寻访获得道具和武将
global.hero_visit_high_hero_award = 4 --神将特殊寻访获得武将
global.hero_visit_progress_color1 = 10 --寻访突破获取进度
global.hero_visit_progress_color2 = 5 --寻访突破获取进度
global.hero_visit_progress_color3 = 15 --寻访突破获取进度
global.hero_visit_progress_color4 = 50 --寻访突破获取进度
global.hero_visit_progress_normal = 2 --寻访获取进度
--]]
end

-- 良将寻访
function HeroVisitDlgLow()
	-- 关键条件判断
	
	-- 发送信息
	system_askinfo( ASKINFO_HERO_VISIT, "", 1, 0 );
end

-- 良将寻访10连
function HeroVisitDlgLow10()
	-- 关键条件判断
	
	-- 发送信息
	system_askinfo( ASKINFO_HERO_VISIT, "", 1, 1 );
end

-- 神将寻访
function HeroVisitDlgHigh()
	-- 关键条件判断
	
	-- 发送信息
	system_askinfo( ASKINFO_HERO_VISIT, "", 2, 0 );
end

-- 神将寻访10连
function HeroVisitDlgHigh()
	-- 关键条件判断
	
	-- 发送信息
	system_askinfo( ASKINFO_HERO_VISIT, "", 2, 1 );
end


--设置良将
function HeroVisitDlgSetGood()

end 

--设置神将 
function HeroVisitDlgSetGod()
	
end

