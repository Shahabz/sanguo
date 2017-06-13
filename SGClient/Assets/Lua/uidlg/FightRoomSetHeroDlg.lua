-- 界面
local m_Dlg = nil;
local m_uiCountdown = nil; --UnityEngine.GameObject
local m_uiAttack = nil; --UnityEngine.GameObject
local m_uiDefense = nil; --UnityEngine.GameObject

-- 打开界面
function FightRoomSetHeroDlgOpen()
	m_Dlg = eye.uiManager:Open( "FightRoomSetHeroDlg" );
end

-- 隐藏界面
function FightRoomSetHeroDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "FightRoomSetHeroDlg" );
end

-- 删除界面
function FightRoomSetHeroDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function FightRoomSetHeroDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            FightRoomSetHeroDlgClose();
        end
	end
end

-- 载入时调用
function FightRoomSetHeroDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;
	m_uiCountdown = objs[0];
	m_uiAttack = objs[1];
	m_uiDefense = objs[2];
end

-- 界面初始化时调用
function FightRoomSetHeroDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function FightRoomSetHeroDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function FightRoomSetHeroDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function FightRoomSetHeroDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
local m_speed = 0;
local m_Countdown = 30;
function FightRoomSetHeroDlgOnLogic( gameObject )
	if m_speed == 60 and m_Countdown > 0 then
		m_Countdown = m_Countdown - 1;
		m_uiCountdown:GetComponent( typeof(UIText) ).text = m_Countdown;
		m_speed = 0;
	end
	m_speed = m_speed + 1;
end


----------------------------------------
-- 自定
----------------------------------------
function FightRoomSetHeroDlgShow( matchRecvValue, recvValue )
	FightRoomSetHeroDlgOpen();
end

