-- 界面
local m_Dlg = nil;
local m_DialogFrameMod = nil;
--------------------------------------------------------------
local m_uiCorps = nil; --UnityEngine.GameObject
local m_uiShape = nil; --UnityEngine.GameObject
local m_uiDesc = nil; --UnityEngine.GameObject
local m_uiSoldiers = nil; --UnityEngine.GameObject
local m_uiSoldiersMax = nil; --UnityEngine.GameObject
local m_uiQueueLevel = nil; --UnityEngine.GameObject
local m_uiUIP_Train = nil; --UnityEngine.GameObject
local m_uiUIP_TrainQueue = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_buildingKind = 0;
-- 打开界面
function TrainDlgOpen()
	m_Dlg = eye.uiManager:Open( "TrainDlg" );
	m_DialogFrameMod = DialogFrameModOpen( m_Dlg, T(m_buildingKind), 8, TrainDlgClose );
end

-- 隐藏界面
function TrainDlgClose()
	if m_Dlg == nil then
		return;
	end
	DialogFrameModClose( m_DialogFrameMod );
	eye.uiManager:Close( "TrainDlg" );
end

-- 删除界面
function TrainDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function TrainDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            TrainDlgClose();
        end
	end
end

-- 载入时调用
function TrainDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiCorps = objs[0];
	m_uiShape = objs[1];
	m_uiDesc = objs[2];
	m_uiSoldiers = objs[3];
	m_uiSoldiersMax = objs[4];
	m_uiQueueLevel = objs[5];
	m_uiUIP_Train = objs[6];
	m_uiUIP_TrainQueue = objs[7];
	m_uiContent = objs[8];
end

-- 界面初始化时调用
function TrainDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function TrainDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function TrainDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function TrainDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function TrainDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function TrainDlgOnShow( buildingKind )
	m_buildingKind = buildingKind;
	TrainDlgOpen();
	local corps = 0;
	if m_buildingKind == BUILDING_Infantry or m_buildingKind == BUILDING_Militiaman_Infantry then
		corps = 0
	elseif m_buildingKind == BUILDING_Cavalry or m_buildingKind == BUILDING_Militiaman_Cavalry then
		corps = 1
	elseif m_buildingKind == BUILDING_Archer or m_buildingKind == BUILDING_Militiaman_Archer then
		corps = 2
	end
	SetImage( m_uiCorps, CorpsSprite( corps ) );
	SetImage( m_uiShape, CorpsSprite( corps ) );
end

