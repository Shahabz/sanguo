-- 界面
local m_Dlg = nil;

local m_recvValue = nil;
-- 打开界面
function BlackListDlgOpen()
	m_Dlg = eye.uiManager:Open( "BlackListDlg" );
end

-- 隐藏界面
function BlackListDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "BlackListDlg" );
end

-- 删除界面
function BlackListDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function BlackListDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            BlackListDlgClose();
        end
	end
end

-- 载入时调用
function BlackListDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
end

-- 界面初始化时调用
function BlackListDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function BlackListDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function BlackListDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function BlackListDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function BlackListDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function BlackListDlgShow()
	BlackListDlgOpen()
	if m_recvValue == nil then
		system_askinfo( ASKINFO_ACTOR, "", 3 );
	else
		BlackListDlgRecv( m_recvValue )
	end
end

-- m_count=0,m_list={m_actorid=0,m_namelen=0,m_name="[m_namelen]",m_level=0,m_nation=0,[m_count]},
function BlackListDlgRecv( recvValue )
	m_recvValue = recvValue;
	for i=1, m_recvValue.m_count, 1 do
		
	end
end


-- 服务器通知添加一个人到黑名单
-- m_actorid=0,m_namelen=0,m_name="[m_namelen]",m_level=0,m_nation=0,
function BlackListDlgAdd( recvValue )
	if m_recvValue == nil then
		return
	end
	table.insert( m_recvValue, recvValue );
end

-- 客户端删除一个玩家
function BlackListDlgDel( actorid )
	system_askinfo( ASKINFO_ACTOR, "", 5, actorid );
	GetPlayer():DelBlacklist( actorid )
	if m_recvValue == nil then
		return
	end
	for i=1, m_recvValue.m_count do
		if m_recvValue.m_list[i].m_actorid == actorid then
			table.remove( m_recvValue, i );
			break;
		end
	end
end
