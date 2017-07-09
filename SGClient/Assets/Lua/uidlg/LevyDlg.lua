-- 界面
local m_Dlg = nil;
local m_DialogFrameMod = nil;

local m_uiBaseSilver = nil; --UnityEngine.GameObject
local m_uiBaseWood = nil; --UnityEngine.GameObject
local m_uiBaseFood = nil; --UnityEngine.GameObject
local m_uiBaseIron = nil; --UnityEngine.GameObject
local m_uiTechSilver = nil; --UnityEngine.GameObject
local m_uiTechWood = nil; --UnityEngine.GameObject
local m_uiTechFood = nil; --UnityEngine.GameObject
local m_uiTechIron = nil; --UnityEngine.GameObject
local m_uiWeatherSilver = nil; --UnityEngine.GameObject
local m_uiWeatherWood = nil; --UnityEngine.GameObject
local m_uiWeatherFood = nil; --UnityEngine.GameObject
local m_uiWeatherIron = nil; --UnityEngine.GameObject
local m_uiActivitySilver = nil; --UnityEngine.GameObject
local m_uiActivityWood = nil; --UnityEngine.GameObject
local m_uiActivityFood = nil; --UnityEngine.GameObject
local m_uiActivityIron = nil; --UnityEngine.GameObject
local m_uiOfficialSilver = nil; --UnityEngine.GameObject
local m_uiOfficialWood = nil; --UnityEngine.GameObject
local m_uiOfficialFood = nil; --UnityEngine.GameObject
local m_uiOfficialIron = nil; --UnityEngine.GameObject
local m_uiTotalSilver = nil; --UnityEngine.GameObject
local m_uiTotalWood = nil; --UnityEngine.GameObject
local m_uiTotalFood = nil; --UnityEngine.GameObject
local m_uiTotalIron = nil; --UnityEngine.GameObject
local m_uiTimerText = nil; --UnityEngine.GameObject
local m_uiLevyNumText = nil; --UnityEngine.GameObject

-- 打开界面
function LevyDlgOpen()
	m_Dlg = eye.uiManager:Open( "LevyDlg" );
	m_DialogFrameMod = DialogFrameModOpen( m_Dlg, "官府", 1, LevyDlgClose );
end

-- 隐藏界面
function LevyDlgClose()
	if m_Dlg == nil then
		return;
	end
	DialogFrameModClose( m_DialogFrameMod );
	eye.uiManager:Close( "LevyDlg" );
end

-- 删除界面
function LevyDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function LevyDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            LevyDlgClose();
        end
	end
end

-- 载入时调用
function LevyDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;
	m_uiBaseSilver = objs[0];
	m_uiBaseWood = objs[1];
	m_uiBaseFood = objs[2];
	m_uiBaseIron = objs[3];
	m_uiTechSilver = objs[4];
	m_uiTechWood = objs[5];
	m_uiTechFood = objs[6];
	m_uiTechIron = objs[7];
	m_uiWeatherSilver = objs[8];
	m_uiWeatherWood = objs[9];
	m_uiWeatherFood = objs[10];
	m_uiWeatherIron = objs[11];
	m_uiActivitySilver = objs[12];
	m_uiActivityWood = objs[13];
	m_uiActivityFood = objs[14];
	m_uiActivityIron = objs[15];
	m_uiOfficialSilver = objs[16];
	m_uiOfficialWood = objs[17];
	m_uiOfficialFood = objs[18];
	m_uiOfficialIron = objs[19];
	m_uiTotalSilver = objs[20];
	m_uiTotalWood = objs[21];
	m_uiTotalFood = objs[22];
	m_uiTotalIron = objs[23];
	m_uiTimerText = objs[24];	
	m_uiLevyNumText = objs[25];
end

-- 界面初始化时调用
function LevyDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function LevyDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function LevyDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function LevyDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function LevyDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function LevyDlgShow()
	LevyDlgOpen();

end

