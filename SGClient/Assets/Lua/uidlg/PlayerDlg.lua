-- 界面
local m_Dlg = nil;
local m_uiPlayNameText = nil; --UnityEngine.GameObject
local m_uiNationText = nil; --UnityEngine.GameObject
local m_uiShape = nil; --UnityEngine.GameObject
local m_uiVipText = nil; --UnityEngine.GameObject
local m_uiExpProgress = nil; --UnityEngine.GameObject
local m_uiBodyProgress = nil; --UnityEngine.GameObject
local m_uiLabelNationText = nil; --UnityEngine.GameObject
local m_uiLabelNameText = nil; --UnityEngine.GameObject
local m_uiLabelPlaceText = nil; --UnityEngine.GameObject
local m_uiLabelServerText = nil; --UnityEngine.GameObject
local m_uiLabelIDText = nil; --UnityEngine.GameObject
local m_uiLabelBattlepowerText = nil; --UnityEngine.GameObject

-- 打开界面
function PlayerDlgOpen()
	m_Dlg = eye.uiManager:Open( "PlayerDlg" );
end

-- 隐藏界面
function PlayerDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "PlayerDlg" );
end

-- 删除界面
function PlayerDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function PlayerDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            PlayerDlgClose();
        end
	end
end

-- 载入时调用
function PlayerDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiPlayNameText = objs[0];
	m_uiNationText = objs[1];
	m_uiShape = objs[2];
	m_uiVipText = objs[3];
	m_uiExpProgress = objs[4];
	m_uiBodyProgress = objs[5];
	m_uiLabelNationText = objs[6];
	m_uiLabelNameText = objs[7];
	m_uiLabelPlaceText = objs[8];
	m_uiLabelServerText = objs[9];
	m_uiLabelIDText = objs[10];
	m_uiLabelBattlepowerText = objs[11];
end

-- 界面初始化时调用
function PlayerDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function PlayerDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function PlayerDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function PlayerDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function PlayerDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function PlayerDlgShow()
	PlayerDlgOpen()
	PlayerDlgSet()
end

function PlayerDlgSet()
	if m_Dlg == nil or m_Dlg.gameObject.activeSelf == false then
		return;
	end
	m_uiPlayNameText:GetComponent( "UIText" ).text = GetPlayer().m_name;
	m_uiNationText:GetComponent( "UIText" ).text = Nation( GetPlayer().m_nation );
	m_uiShape:GetComponent( "Image" ).sprite = PlayerFaceSprite( GetPlayer().m_shape );
	m_uiVipText:GetComponent( "UIText" ).text = "VIP "..GetPlayer().m_viplevel;
	
	-- 经验
	m_uiExpProgress:GetComponent( "UIProgress" ):SetValue( GetPlayer().m_exp/GetPlayer().m_exp_max );
	m_uiExpProgress.transform:Find("Text"):GetComponent( "UIText" ).text = GetPlayer().m_exp.."/"..GetPlayer().m_exp_max;
	m_uiExpProgress.transform:Find("Level"):GetComponent( "UIText" ).text = "Lv."..GetPlayer().m_level;
	
	-- 体力
	m_uiBodyProgress:GetComponent( "UIProgress" ):SetValue( GetPlayer().m_body/100 );
	m_uiBodyProgress.transform:Find("Text"):GetComponent( "UIText" ).text = GetPlayer().m_body.."/100";
	if GetPlayer().m_body >= 100 then
		m_uiBodyProgress.transform:Find("Level"):GetComponent( "UIText" ).text = zh("已满");
	end
	
	-- 其它信息
	m_uiLabelNationText:GetComponent( "UIText" ).text = NationEx( GetPlayer().m_nation );
	m_uiLabelNameText:GetComponent( "UIText" ).text = GetPlayer().m_name;
	m_uiLabelPlaceText:GetComponent( "UIText" ).text = "";
	m_uiLabelServerText:GetComponent( "UIText" ).text = Const.servername;
	m_uiLabelIDText:GetComponent( "UIText" ).text = GetPlayer().m_actorid;
	m_uiLabelBattlepowerText:GetComponent( "UIText" ).text = GetPlayer().m_battlepower;
end