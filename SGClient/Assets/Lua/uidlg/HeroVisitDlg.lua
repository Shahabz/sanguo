-- 界面
local m_Dlg = nil;
local m_DialogFrameMod = nil;

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
end
