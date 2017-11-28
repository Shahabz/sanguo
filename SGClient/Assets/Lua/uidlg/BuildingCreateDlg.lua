-- 界面
local m_Dlg = nil;
local m_uiTitle = nil; --UnityEngine.GameObject
local m_uiGrid = nil; --UnityEngine.GameObject
local m_uiUIP_Building = {nil,nil,nil}; --UnityEngine.GameObject
local m_uiUIP_BuildingRes = {nil,nil,nil,nil}; --UnityEngine.GameObject

local m_kind = 0;
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
			BuildingCreateDlgBuild( nControlID )
        end
	end
end

-- 载入时调用
function BuildingCreateDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;
	m_uiTitle = objs[0];
	m_uiGrid = objs[1];
	m_uiUIP_Building[1] = objs[2];
	m_uiUIP_Building[2] = objs[3];
	m_uiUIP_Building[3] = objs[4];
	m_uiUIP_BuildingRes[1] = objs[5];
	m_uiUIP_BuildingRes[2] = objs[6];
	m_uiUIP_BuildingRes[3] = objs[7];
	m_uiUIP_BuildingRes[4] = objs[8];
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

-- 民兵营建筑
function BuildingCreateDlgShowByID( op, id )
	if op == 0 and id ~= 9 then
		return
	end
	BuildingCreateDlgOpen()
	for i = 1, 4, 1 do
		SetFalse( m_uiUIP_BuildingRes[i] );
	end
	SetText( m_uiTitle.transform:Find("Text"), T(1465) )
	if op == 0 then
		m_kind = 0;
	else
		m_kind = id;
	end
	m_offset = -1;
	local buildingList = { BUILDING_Militiaman_Infantry, BUILDING_Militiaman_Cavalry, BUILDING_Militiaman_Archer }
	local corpsList = { 0, 1, 2 }
	local descList = { 1470, 1471, 1472 }
	for i = 1, #buildingList, 1 do
		local kind = buildingList[i];
		SetTrue( m_uiUIP_Building[i] );
		SetControlID( m_uiUIP_Building[i].transform:Find("CreateButton"), kind );
		SetImage( m_uiUIP_Building[i].transform:Find("Shape"), BuildingSprite( kind ) );
		SetImage( m_uiUIP_Building[i].transform:Find("Icon"), CorpsSprite( corpsList[i] ) );
		SetText( m_uiUIP_Building[i].transform:Find("Name"), BuildingName( kind ) )
		SetText( m_uiUIP_Building[i].transform:Find("Desc1"), T(descList[i]) )
	end
end

-- 资源田建筑
function BuildingCreateDlgShowByRes( buildingkind, offset )
	if buildingkind < BUILDING_Silver or buildingkind > BUILDING_Iron then
		return
	end
	BuildingCreateDlgOpen()
	for i = 1, 3, 1 do
		SetFalse( m_uiUIP_Building[i] );
	end
	SetText( m_uiTitle.transform:Find("Text"), T(1465) )
	m_kind = buildingkind;
	m_offset = offset;
	local buildingList = { BUILDING_Silver, BUILDING_Wood, BUILDING_Food, BUILDING_Iron }
	for i = 1, #buildingList, 1 do
		local kind = buildingList[i];
		SetTrue( m_uiUIP_BuildingRes[i] );
		SetControlID( m_uiUIP_BuildingRes[i].transform:Find("CreateButton"), kind );
		SetImage( m_uiUIP_BuildingRes[i].transform:Find("Shape"), BuildingSprite( kind ) );
		SetText( m_uiUIP_BuildingRes[i].transform:Find("Name"), BuildingName( kind ) )
		SetText( m_uiUIP_BuildingRes[i].transform:Find("Desc"), T(70+i) )
	end
end

-- 建造或改建
function BuildingCreateDlgBuild( buildkind )
	--system_askinfo( ASKINFO_BUILDING, "", 2, m_kind, m_offset, buildkind );
	BuildingCreateDlgClose()
	BuildingUpgradeDlgShow( m_kind, m_offset, buildkind )
end