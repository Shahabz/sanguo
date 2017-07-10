-- 界面
local m_Dlg = nil;
local m_uiShape = nil; --UnityEngine.GameObject
local m_uiName = nil; --UnityEngine.GameObject
local m_uiDesc = nil; --UnityEngine.GameObject
local m_uiNormalPanel = nil; --UnityEngine.GameObject
local m_uiMovePanel = nil; --UnityEngine.GameObject

local m_ObjectPool = nil;
local m_kind = {};
local m_offset = {};
local m_info = {};

-- 打开界面
function BuildingGetDlgOpen()
	m_Dlg = eye.uiManager:Open( "BuildingGetDlg" );
end

-- 隐藏界面
function BuildingGetDlgClose()
	if m_Dlg == nil then
		return;
	end
	local objs = {};
	for i = 0 ,m_uiMovePanel.transform.childCount - 1 do
		table.insert( objs, m_uiMovePanel.transform:GetChild(i).gameObject )
    end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "Shape(Clone)" then
			m_ObjectPool:Release( "Shape", obj );
		end
    end
	m_kind = {};
	m_offset = {};
	m_info = {};
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
	
	-- 对象池
	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );
	m_ObjectPool:CreatePool("Shape", 4, 4, m_uiShape);
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
function BuildingGetDlgShow( kind, offset, info )
	BuildingGetDlgOpen();
	table.insert( m_kind, kind );
	table.insert( m_offset, offset );
	table.insert( m_info, info );
	
	m_uiNormalPanel.gameObject:SetActive(true);
	m_uiMovePanel.gameObject:SetActive(false);
	m_uiShape:GetComponent( "Image" ).sprite = BuildingSprite( kind );
	m_uiName:GetComponent( "UIText" ).text = T( kind );
end

function BuildingGetDlgMove()
	m_uiNormalPanel.gameObject:SetActive(false);
	m_uiMovePanel.gameObject:SetActive(true);
	m_uiShape.gameObject:SetActive(false);
	
	local count = 0;
	for k, v in pairs( m_kind ) do
		count = count + 1;
	end
	
	local index = 0;
	local moveing = false;
	local unitObj = {};
	local uiShape = {};
	for k, v in pairs( m_kind ) do
		if m_kind[k] >= BUILDING_Silver and m_kind[k] <= BUILDING_Iron then
			unitObj[k] = GetPlayer():SetBuildingRes( m_kind[k], m_offset[k], m_info[k], false )
		else
			unitObj[k] = GetPlayer():SetBuilding( m_kind[k], m_info[k], false )
		end
		if moveing == false then
			City.m_Camera:TweenPosToInBound( unitObj[k].transform.position, 1 );
			moveing = true;
		end
		
		
		uiShape[k] = m_ObjectPool:Get( "Shape" );
		uiShape[k].transform:SetParent( m_uiMovePanel.transform );
		uiShape[k]:GetComponent( "Image" ).sprite = BuildingSprite( m_kind[k] );
		
		Invoke( function() 
			uiShape[k]:GetComponent( "UITweenRectPosition" ).from = Vector2( uiShape[k].transform.localPosition.x, uiShape[k].transform.localPosition.y );
			local screenPos = City.m_CameraMain:WorldToScreenPoint(unitObj[k].transform.position);
			local x = screenPos.x-Screen.width/2;
			local y= screenPos.y-Screen.height/2;
			uiShape[k]:GetComponent( "UITweenRectPosition" ).to = Vector2( x, y ); 
			uiShape[k]:GetComponent( "UITweenRectPosition" ):Play( true );
		end, 1.1 )
		
		Invoke( function() 
			uiShape[k]:GetComponent( "UITweenScale" ):ToInit();
			uiShape[k]:GetComponent( "UITweenRectPosition" ):ToInit();
			unitObj[k].gameObject:SetActive(true);
			if k == count then
				BuildingGetDlgClose();
			end
		end, 1.6 )
		
		
	end
end

