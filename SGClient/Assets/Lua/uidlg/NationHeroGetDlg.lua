-- 界面
local m_Dlg = nil;
local m_uiTitle = nil; --UnityEngine.GameObject
local m_uiShape = nil; --UnityEngine.GameObject
local m_uiTalk = nil; --UnityEngine.GameObject

-- 打开界面
function NationHeroGetDlgOpen()
	m_Dlg = eye.uiManager:Open( "NationHeroGetDlg" );

end

-- 隐藏界面
function NationHeroGetDlgClose()
	if m_Dlg == nil then
		return;
	end
	eye.uiManager:Close( "NationHeroGetDlg" );
end

-- 删除界面
function NationHeroGetDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function NationHeroGetDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            NationHeroGetDlgClose();
		elseif nControlID == 1 then
			NationHeroGetDlgClose()
			HeroListDlgShow( HEROLIST_PATH_HERO_LIST );
        end
	end
end

-- 载入时调用
function NationHeroGetDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiTitle = objs[0];
	m_uiShape = objs[1];
	m_uiTalk = objs[2];
end

-- 界面初始化时调用
function NationHeroGetDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function NationHeroGetDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function NationHeroGetDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function NationHeroGetDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function NationHeroGetDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function NationHeroGetDlgShow( kind )
	NationHeroGetDlgOpen()
	local name = HeroName(kind)
	SetText( m_uiTitle.transform:Find("Text"), F( 2345, name ) )
	SetImage( m_uiShape, HeroHeadSprite( kind ) )
	SetText( m_uiTalk.transform:Find("Text"), F( 2346, name ) )
end
