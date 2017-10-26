-- 界面
local m_Dlg = nil;
local m_uiTitle = nil; --UnityEngine.GameObject
local m_uiDesc = nil; --UnityEngine.GameObject
local m_uiShape = nil; --UnityEngine.GameObject
local m_uiProgressTitle = nil; --UnityEngine.GameObject
local m_uiProgress = nil; --UnityEngine.GameObject
local m_uiFightButton = nil; --UnityEngine.GameObject
local m_uiCancelButton = nil; --UnityEngine.GameObject
local m_uiWarn = nil; --UnityEngine.GameObject
local m_bossid = 0;
local m_limit_actorlevel = {};
-- 打开界面
function WorldBossDlgOpen()
	m_Dlg = eye.uiManager:Open( "WorldBossDlg" );
end

-- 隐藏界面
function WorldBossDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "WorldBossDlg" );
end

-- 删除界面
function WorldBossDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function WorldBossDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            WorldBossDlgClose();
		elseif nControlID == 1 then
			WorldBossDlgFight();
        end
	end
end

-- 载入时调用
function WorldBossDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiTitle = objs[0];
	m_uiDesc = objs[1];
	m_uiShape = objs[2];
	m_uiProgressTitle = objs[3];
	m_uiProgress = objs[4];
	m_uiFightButton = objs[5];
	m_uiCancelButton = objs[6];
	m_uiWarn = objs[7];
end

-- 界面初始化时调用
function WorldBossDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function WorldBossDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function WorldBossDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function WorldBossDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function WorldBossDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function WorldBossDlgShow( bossid, questid )
	WorldBossDlgOpen()
	m_bossid = bossid
	SetFalse( m_uiCancelButton )
	SetFalse( m_uiFightButton )
	SetText( m_uiTitle, T(1359) )
	if questid > 0 then
		SetText( m_uiDesc, Localization.text_quest( 2050+questid ) );
	else
		SetText( m_uiDesc, "" )
	end
	SetImage( m_uiShape, LoadSprite( "ui_worldboss_shape"..bossid ) );
	system_askinfo( ASKINFO_QUEST, "", 13, bossid );
end

-- m_bossid=0,m_hp=0,m_maxhp=0,m_isfight=0,m_actorlevel=0
function WorldBossDlgRecvValue( recvValue )
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	m_limit_actorlevel = recvValue.m_actorlevel
	SetProgress( m_uiProgress, recvValue.m_hp/recvValue.m_maxhp );
	SetText( m_uiProgress.transform:Find("Text"), recvValue.m_hp.."/"..recvValue.m_maxhp )
	if recvValue.m_isfight < 0 then
	elseif recvValue.m_isfight > 0 then
		SetTrue( m_uiCancelButton )
		SetFalse( m_uiFightButton )
		SetText( m_uiWarn, T(1358) )
	else
		SetTrue( m_uiFightButton )
		SetFalse( m_uiCancelButton )
		SetText( m_uiWarn, T(1357) )
	end
end

-- 战斗
function WorldBossDlgFight()
	if GetPlayer().m_level < m_limit_actorlevel then
		AlertMsg( F(1369, m_limit_actorlevel ) )
		return  
	end
	BattleDlgShowByWorldBoss( m_bossid )
	WorldBossDlgClose()
end
