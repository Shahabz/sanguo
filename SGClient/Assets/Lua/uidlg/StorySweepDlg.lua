-- 界面
local m_Dlg = nil;
local m_uiTitle = nil; --UnityEngine.GameObject
local m_uiDesc = nil; --UnityEngine.GameObject
local m_uiLayer1 = nil; --UnityEngine.GameObject
local m_uiLayer2 = nil; --UnityEngine.GameObject
local m_uiGrid = nil; --UnityEngine.GameObject
local m_uiBody = nil; --UnityEngine.GameObject
local m_uiSelectGrid = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiUIP_HeroHead = nil; --UnityEngine.GameObject

local m_storyid = 0;

-- 打开界面
function StorySweepDlgOpen()
	m_Dlg = eye.uiManager:Open( "StorySweepDlg" );
end

-- 隐藏界面
function StorySweepDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "StorySweepDlg" );
end

-- 删除界面
function StorySweepDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function StorySweepDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            StorySweepDlgClose();
		elseif nControlID == -2 then
			StorySweepDlgHideSelectLayer()
		elseif nControlID == 1 then
			StorySweepDlgFight()
		elseif nControlID == 2 then
			StorySweepDlgSweep()
		elseif nControlID == 3 then
			StorySweepDlgShowSelectLayer()
        end
	end
end

-- 载入时调用
function StorySweepDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiTitle = objs[0];
	m_uiDesc = objs[1];
	m_uiLayer1 = objs[2];
	m_uiLayer2 = objs[3];
	m_uiGrid = objs[4];
	m_uiBody = objs[5];
	m_uiSelectGrid = objs[6];
	m_uiContent = objs[7];
	m_uiUIP_HeroHead = objs[8];
end

-- 界面初始化时调用
function StorySweepDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function StorySweepDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function StorySweepDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function StorySweepDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function StorySweepDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function StorySweepDlgShow( storyid )
	StorySweepDlgOpen()
	m_storyid = storyid;
	SetTrue( m_uiLayer1 )
	SetFalse( m_uiLayer2 )
	SetText( m_uiTitle.transform:Find("Text"), StoryRankName( storyid ) )
	
	-- 当前体力
	if GetPlayer().m_body < g_story[storyid].body then
		SetRichText( m_uiBody, F(2045, GetPlayer().m_body ) )	
	else
		SetRichText( m_uiBody, F(2044, GetPlayer().m_body ) )	
	end
	
	-- 设置武将信息
	
	--m_uiGrid = objs[4];
	--m_uiSelectGrid = objs[6];
	--m_uiContent = objs[7];
	--m_uiUIP_HeroHead = objs[8];
end

-- 显示选择武将页
function StorySweepDlgShowSelectLayer()
	SetFalse( m_uiLayer1 )
	SetTrue( m_uiLayer2 )
end

-- 隐藏选择武将页
function StorySweepDlgHideSelectLayer()
	SetTrue( m_uiLayer1 )
	SetFalse( m_uiLayer2 )
end

-- 挑战
function StorySweepDlgFight()
	StorySweepDlgClose();
	BattleDlgShowByStory( m_storyid )
end

-- 扫荡
function StorySweepDlgSweep()
	-- 消耗体力
	if GetPlayer().m_body < g_story[m_storyid].body*5 then
		StorySweepDlgClose();
		JumpBody()
		return
	end	
	system_askinfo( ASKINFO_STORY, "", 8, m_storyid );
end

