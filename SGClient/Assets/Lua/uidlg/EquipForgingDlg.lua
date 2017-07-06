-- 界面
local m_Dlg = nil;
local m_DialogFrameMod = nil;

local m_uiUIP_ForgingEquip = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiEquipInfo = nil; --UnityEngine.GameObject
local m_uiShape = nil; --UnityEngine.GameObject
local m_uiName = nil; --UnityEngine.GameObject
local m_uiAbility = nil; --UnityEngine.GameObject
local m_uiUIP_Material = nil; --UnityEngine.GameObject
local m_uiMaterialGrid = nil; --UnityEngine.GameObject

-- 打开界面
function EquipForgingDlgOpen()
	m_Dlg = eye.uiManager:Open( "EquipForgingDlg" );
	m_DialogFrameMod = DialogFrameModOpen( m_Dlg, "铁匠铺", 6, EquipForgingDlgClose );
end

-- 隐藏界面
function EquipForgingDlgClose()
	if m_Dlg == nil then
		return;
	end
	DialogFrameModClose( m_DialogFrameMod );
	eye.uiManager:Close( "EquipForgingDlg" );
end

-- 删除界面
function EquipForgingDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function EquipForgingDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            EquipForgingDlgClose();
        end
	end
end

-- 载入时调用
function EquipForgingDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;
	m_uiUIP_ForgingEquip = objs[0];
	m_uiContent = objs[1];
	m_uiEquipInfo = objs[2];
	m_uiShape = objs[3];
	m_uiName = objs[4];
	m_uiAbility = objs[5];
	m_uiUIP_Material = objs[6];
	m_uiMaterialGrid = objs[7];
end

-- 界面初始化时调用
function EquipForgingDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function EquipForgingDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function EquipForgingDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function EquipForgingDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function EquipForgingDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function EquipForgingDlgShow()
	EquipForgingDlgOpen();
end
