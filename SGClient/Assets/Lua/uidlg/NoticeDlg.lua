-- 界面
local m_Dlg = nil;
local m_uiNoticeText = nil; --UnityEngine.GameObject
local m_uiTogButton = nil; --UnityEngine.GameObject

-- 打开界面
function NoticeDlgOpen()
	ResourceManager.LoadAssetBundle( "_ab_ui_static_pic_3" )
	ResourceManager.LoadAssetBundle( "_ab_ui_static_notice1" )
	ResourceManager.LoadAssetBundle( "_ab_ui_static_notice2" )
	m_Dlg = eye.uiManager:Open( "NoticeDlg" );
end

-- 隐藏界面
function NoticeDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "NoticeDlg" );
end

-- 删除界面
function NoticeDlgDestroy()
	GameObject.Destroy( m_Dlg );
	ResourceManager.UnloadAssetBundle( "_ab_ui_static_pic_3" )
	ResourceManager.UnloadAssetBundle( "_ab_ui_static_notice1" )
	ResourceManager.UnloadAssetBundle( "_ab_ui_static_notice2" )
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function NoticeDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            NoticeDlgClose();
        end
	elseif nType == UI_EVENT_TOGGLEVALUECHANGE then
		if nControlID == 1 then
			print("value:"..tostring(value));
		end
	end
end

-- 载入时调用
function NoticeDlgOnAwake( gameObject )	
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiNoticeText = objs[0];
	m_uiTogButton = objs[1];
end

-- 界面初始化时调用
function NoticeDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function NoticeDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function NoticeDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function NoticeDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function NoticeDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function NoticeDlgShow()
	NoticeDlgOpen()
	HttpRequest.GetFile( "notice.cfg", function( response )
		SetText(m_uiNoticeText, response)
	end )
end
