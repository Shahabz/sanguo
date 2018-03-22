-- 界面
local m_Dlg = nil;
local m_uiPlayNameText = nil; --UnityEngine.GameObject
local m_uiNationBack = nil; --UnityEngine.GameObject
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
        elseif nControlID == 1 then
        	ChangeShapeDlgShow();
		
		-- 设置
		elseif nControlID == 11 then
			m_Setting_state = 1
			SettingDlgShow();
		-- 反馈
		elseif nControlID == 12 then
			pop( T(579) )
			push_train( 20 )
			SDK.gmbug()
			
		-- vip
		elseif nControlID == 13 then
			VipDlgShow()
			
		-- 充值
		elseif nControlID == 14 then
			PayDlgShow()
		
		-- 购买体力
		elseif nControlID == 15 then
			JumpBody()
        end
	elseif nType == UI_EVENT_TIMECOUNTEND then
		if nControlID == 1 then
			PlayerDlgSet();
		end	
	end
end

-- 载入时调用
function PlayerDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiPlayNameText = objs[0];
	m_uiNationBack = objs[1];
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
	SetText( m_uiPlayNameText, GetPlayer().m_name );
	SetImage( m_uiNationBack, NationSprite( GetPlayer().m_nation ) )
	SetImage( m_uiShape, PlayerFaceSprite( GetPlayer().m_shape ) );
	SetText( m_uiVipText, "v"..GetPlayer().m_viplevel );
	
	-- 经验
	SetProgress( m_uiExpProgress, GetPlayer().m_exp/GetPlayer().m_exp_max );
	SetText( m_uiExpProgress.transform:Find("Text"), GetPlayer().m_exp.."/"..GetPlayer().m_exp_max );
	SetLevel( m_uiExpProgress.transform:Find("Level"), GetPlayer().m_level );
	
	-- 体力
	SetProgress( m_uiBodyProgress, GetPlayer().m_body/100 );
	SetText( m_uiBodyProgress.transform:Find("Text"), GetPlayer().m_body.."/100" );
	
	local bodydesc = m_uiBodyProgress.transform:Find("Level")
	local bodytimer = m_uiBodyProgress.transform:Find("Timer")
	if GetPlayer().m_body >= 100 then
		SetText( bodydesc, T(666) );
		SetTrue( bodydesc );
		SetFalse( bodytimer );
	else
		SetFalse( bodydesc );
		SetTrue( bodytimer );
		SetTimer( bodytimer, GetPlayer().m_bodysec, GetPlayer().m_bodysec, 1 )
	end
	
	-- 其它信息
	SetText( m_uiLabelNationText, NationEx( GetPlayer().m_nation ) );
	SetText( m_uiLabelNameText, GetPlayer().m_name );
	SetText( m_uiLabelPlaceText, PlaceName( GetPlayer().m_place ) );
	SetText( m_uiLabelServerText, Const.servername );
	SetText( m_uiLabelIDText, GetPlayer().m_actorid );
	SetText( m_uiLabelBattlepowerText, GetPlayer().m_battlepower );
end