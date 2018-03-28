-- 打开界面
function GirlTrainDlgOpen()
	m_Dlg = eye.uiManager:Open( "GirlTrainDlg" );
	m_DialogFrameMod = DialogFrameModOpen( m_Dlg, T(34), HELP_GirltrainDlg, GirlTrainDlgClose );
end

-- 隐藏界面
function GirlTrainDlgClose()
	if m_Dlg == nil then
		return;
	end
	DialogFrameModClose( m_DialogFrameMod );
	m_DialogFrameMod = nil;
	eye.uiManager:Close( "GirlTrainDlg" );
end

-- 删除界面
function GirlTrainDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function GirlTrainDlgOnEvent( nType, nControlID, value, gameObject )
	
end

-- 载入时调用
function GirlTrainDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	
end

-- 界面初始化时调用
function GirlTrainDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function GirlTrainDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function GirlTrainDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function GirlTrainDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function GirlTrainDlgOnLogic( gameObject )
	
end