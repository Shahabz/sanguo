-- 界面
local m_Dlg = nil;
local m_uiLeftBtn = nil; --UnityEngine.GameObject
local m_uiRightBtn = nil; --UnityEngine.GameObject
local m_uiInfoConten = nil; --UnityEngine.GameObject

-- 打开界面
function RankKingListDlgOpen()
	m_Dlg = eye.uiManager:Open( "RankKingListDlg" );
end

-- 隐藏界面
function RankKingListDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "RankKingListDlg" );
end

-- 删除界面
function RankKingListDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function RankKingListDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            RankKingListDlgClose();
        end
	end
end

-- 载入时调用
function RankKingListDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiLeftBtn = objs[0];
	m_uiRightBtn = objs[1];
	m_uiInfoConten = objs[2];
end

-- 界面初始化时调用
function RankKingListDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function RankKingListDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function RankKingListDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function RankKingListDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function RankKingListDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function RankKingListDlgShow()
	RankKingListDlgOpen()
end

--填充排行榜信息
function RankKingListDlgFillInfo()
	
end




