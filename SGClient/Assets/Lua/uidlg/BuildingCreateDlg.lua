-- 界面
local m_Dlg = nil;
local m_uiUIP_Building = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_offset = -1;
-- 打开界面
function BuildingCreateDlgOpen()
	m_Dlg = eye.uiManager:Open( "BuildingCreateDlg" );
end

-- 隐藏界面
function BuildingCreateDlgClose()
	if m_Dlg == nil then
		return;
	end
	clearChild( m_uiContent.transform );
	eye.uiManager:Close( "BuildingCreateDlg" );
end

-- 删除界面
function BuildingCreateDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function BuildingCreateDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            BuildingCreateDlgClose();
		elseif nControlID > 0 and nControlID < 64 then
			BuildingCreateDlgBuild( kind )
        end
	end
end

-- 载入时调用
function BuildingCreateDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;
	m_uiUIP_Building = objs[0];
	m_uiContent = objs[1];	
end

-- 界面初始化时调用
function BuildingCreateDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function BuildingCreateDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function BuildingCreateDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function BuildingCreateDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function BuildingCreateDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function BuildingCreateDlgShow( buildingkinds, offset )
	if buildingkinds == nil then
		return
	end
	local len = buildingkinds.Length;
	if len == 0 then
		pop("固定建筑位")
		return
	end	
	BuildingCreateDlgOpen()
	m_offset = offset;
	for i = 0, len - 1 do
		local kind = buildingkinds[i];
		local uiObj = GameObject.Instantiate( m_uiUIP_Building );
		uiObj.gameObject:SetActive( true );
		uiObj.transform:SetParent( m_uiContent.transform );
		uiObj.transform.localScale = Vector3.one;
		uiObj:GetComponent( "UIButton" ).controlID = kind;
		uiObj.transform:Find("Shape"):GetComponent( "Image" ).sprite = BuildingSprite( kind );
		uiObj.transform:Find("Name"):GetComponent( "UIText" ).text = T( kind );
	end 
end

function BuildingCreateDlgBuild( kind )
	BuildingCreateDlgClose()
end