-- 界面
local m_Dlg = nil;
--local m_DialogFrameMod = nil;
local m_uiOpeningSpeech = nil; --UnityEngine.GameObject

-- 打开界面
function FilmDlgOpen()
	m_Dlg = eye.uiManager:Open( "FilmDlg" );
	--m_DialogFrameMod = DialogFrameModOpen( m_Dlg, T(0), nil, FilmDlgClose );
end

-- 隐藏界面
function FilmDlgClose()
	if m_Dlg == nil then
		return;
	end
	--DialogFrameModClose( m_DialogFrameMod );
	--m_DialogFrameMod = nil;
	eye.uiManager:Close( "FilmDlg" );
end

-- 删除界面
function FilmDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function FilmDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            FilmDlgClose();
        end
	end
end

-- 载入时调用
function FilmDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiOpeningSpeech = objs[0];
end

-- 界面初始化时调用
function FilmDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function FilmDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function FilmDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function FilmDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function FilmDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function FilmDlgShow()
	FilmDlgOpen()
end
