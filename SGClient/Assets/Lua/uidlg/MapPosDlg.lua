-- 界面
local m_Dlg = nil;
local m_uiMyPos = nil; --UnityEngine.GameObject
local m_uiXPosEdit = nil; --UnityEngine.GameObject
local m_uiYPosEdit = nil; --UnityEngine.GameObject

-- 打开界面
function MapPosDlgOpen()
	m_Dlg = eye.uiManager:Open( "MapPosDlg" );
end

-- 隐藏界面
function MapPosDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "MapPosDlg" );
end

-- 删除界面
function MapPosDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function MapPosDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            MapPosDlgClose();
		elseif nControlID == 1 then
			MapPosDlgGoto()
        end
	end
end

-- 载入时调用
function MapPosDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiMyPos = objs[0];
	m_uiXPosEdit = objs[1];
	m_uiYPosEdit = objs[2];
end

-- 界面初始化时调用
function MapPosDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function MapPosDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function MapPosDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function MapPosDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function MapPosDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function MapPosDlgShow()
	MapPosDlgOpen()
	SetText( m_uiMyPos, F(941,WorldMap.m_nMyCityPosx,WorldMap.m_nMyCityPosy) )
	local MapCamera = WorldMap.GetCamera();
	if MapCamera ~= nil then
		local cameraPosX = MapCamera:GetComponent("Transform").position.x;
		local cameraPosY = MapCamera:GetComponent("Transform").position.y;
		local gameCoorX, gameCoorY = WorldMap.ConvertCameraToGame( cameraPosX, cameraPosY );
		m_uiXPosEdit.transform:Find("Input"):GetComponent( "UIInputField" ).text = gameCoorX;
		m_uiYPosEdit.transform:Find("Input"):GetComponent( "UIInputField" ).text = gameCoorY;
	end
end

function MapPosDlgGoto()
	MapPosDlgClose();
	local gameCoorX = tonumber( m_uiXPosEdit.transform:Find("Input"):GetComponent( "UIInputField" ).text );
	local gameCoorY	= tonumber( m_uiYPosEdit.transform:Find("Input"):GetComponent( "UIInputField" ).text );
	if gameCoorX < 0 or gameCoorY < 0 or gameCoorX >= 500 or gameCoorY >= 500 then
		AlertMsg(T(945))
		return
	end
	WorldMap.GotoCoor( gameCoorX, gameCoorY )
end
