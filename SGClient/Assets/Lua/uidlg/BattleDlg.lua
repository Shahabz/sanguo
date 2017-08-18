-- 界面
local m_Dlg = nil;
local m_uiTitleText = nil; --UnityEngine.GameObject
local m_uiCancelBtn = nil; --UnityEngine.GameObject
local m_uiFightBtn = nil; --UnityEngine.GameObject

local m_battleType = 0;
local m_storyid = 0;
local m_storyHeroKind = {};
-- 打开界面
function BattleDlgOpen()
	m_Dlg = eye.uiManager:Open( "BattleDlg" );
end

-- 隐藏界面
function BattleDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "BattleDlg" );
end

-- 删除界面
function BattleDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function BattleDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            BattleDlgClose();
		elseif nControlID == 1 then
			BattleDlgStoryFight();
        end
	end
end

-- 载入时调用
function BattleDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiTitleText = objs[0];
	m_uiCancelBtn = objs[1];
	m_uiFightBtn = objs[2];
end

-- 界面初始化时调用
function BattleDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function BattleDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function BattleDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function BattleDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function BattleDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function BattleDlgShow()
	BattleDlgOpen()
end

-- 副本使用
function BattleDlgShowByStory( id )
	BattleDlgOpen()
	m_battleType = 1;
	m_storyid = id;
	SetTrue( m_uiTitleText )
	SetText( m_uiTitleText, StoryChapterName( math.floor(id/10) ) )
	system_askinfo( ASKINFO_STORY, "", 1, id );
	
	-- 默认英雄
	m_storyHeroKind = {};
	for offset = 0, 3, 1 do
        local pHero = GetHero().m_CityHero[offset];
		m_storyHeroKind[offset] = pHero.m_kind;
	end
end

-- 副本使用
function BattleDlgStoryRecv( recvValue )
	
end

-- 副本使用
function BattleDlgStoryFight()
	-- m_storyid=0,m_herokind={[4]},
	local sendValue={}
	sendValue.m_storyid = m_storyid;
	sendValue.m_herokind = {}
	for i=0, 3, 1 do
		table.insert( sendValue.m_herokind, m_storyHeroKind[i] )
	end 
	netsend_storybattle_C( sendValue )
end

