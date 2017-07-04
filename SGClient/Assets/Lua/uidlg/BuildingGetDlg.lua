-- 界面
local m_Dlg = nil;
local m_uiShape = nil; --UnityEngine.GameObject
local m_uiName = nil; --UnityEngine.GameObject
local m_uiDesc = nil; --UnityEngine.GameObject
local m_uiNormalPanel = nil; --UnityEngine.GameObject
local m_uiMovePanel = nil; --UnityEngine.GameObject

local m_kind = 0;
local m_offset = -1;

-- 打开界面
function BuildingGetDlgOpen()
	m_Dlg = eye.uiManager:Open( "BuildingGetDlg" );
end

-- 隐藏界面
function BuildingGetDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "BuildingGetDlg" );
end

-- 删除界面
function BuildingGetDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function BuildingGetDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == 1 then
			BuildingGetDlgMove();
        end
	end
end

-- 载入时调用
function BuildingGetDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;
	m_uiShape = objs[0];
	m_uiName = objs[1];
	m_uiDesc = objs[2];
	m_uiNormalPanel = objs[3];
	m_uiMovePanel = objs[4];
end

-- 界面初始化时调用
function BuildingGetDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function BuildingGetDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function BuildingGetDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function BuildingGetDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function BuildingGetDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function BuildingGetDlgShow( kind, offset )
	BuildingGetDlgOpen();
	m_kind = kind;
	m_offset = offset;
	m_uiNormalPanel.gameObject:SetActive(true);
	m_uiMovePanel.gameObject:SetActive(false);
	m_uiShape:GetComponent( "Image" ).sprite = BuildingSprite( kind );
	m_uiName:GetComponent( "UIText" ).text = T( kind );
end

function BuildingGetDlgMove()
	m_uiNormalPanel.gameObject:SetActive(false);
	m_uiMovePanel.gameObject:SetActive(true);
	
	local unitObj = nil;
	if m_kind >= BUILDING_Silver and m_kind <= BUILDING_Iron then
		unitObj = GetPlayer():SetBuildingRes( m_kind, m_offset, { m_kind = m_kind,m_offset = m_offset,m_level = 1,m_sec = 0,m_needsec = 0,m_quick = 0 }, false )
	else
		unitObj = GetPlayer():SetBuilding( m_kind, { m_kind = m_kind,m_offset = m_offset,m_level = 1,m_sec = 0,m_needsec = 0,m_quick = 0 }, false )
	end
	
	City.m_Camera:TweenPosToInBound( unitObj.transform.position, 1 );
	
	Invoke( function() 
		--m_uiShape:GetComponent( "UITweenScale" ):Play( true );
		m_uiShape:GetComponent( "UITweenRectPosition" ).from = Vector2( m_uiShape.transform.localPosition.x, m_uiShape.transform.localPosition.y );
		local screenPos = City.m_CameraMain:WorldToScreenPoint(unitObj.transform.position);
		m_uiShape:GetComponent( "UITweenRectPosition" ).to = Vector2( math.abs(screenPos.x-Screen.width/2), screenPos.y-Screen.height/2 ); 
		m_uiShape:GetComponent( "UITweenRectPosition" ):Play( true );
	end, 1.1 )
	
	Invoke( function() 
		m_uiShape:GetComponent( "UITweenScale" ):ToInit();
		m_uiShape:GetComponent( "UITweenRectPosition" ):ToInit();
		BuildingGetDlgClose();
		unitObj.gameObject:SetActive(true);
	end, 1.6 )
end

