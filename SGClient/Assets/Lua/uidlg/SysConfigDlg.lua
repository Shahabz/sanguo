-- 界面
local m_Dlg = nil;
local m_uiBgm = nil; --UnityEngine.GameObject
local m_uiAudio = nil; --UnityEngine.GameObject
local m_uiAutoHp = nil; --UnityEngine.GameObject
local m_uiZH = nil; --UnityEngine.GameObject
local m_uiTW = nil; --UnityEngine.GameObject
local m_uiSignature = nil; --UnityEngine.GameObject
local m_uiCDKey = nil; --UnityEngine.GameObject
local m_uiSignatureInput = nil; --UnityEngine.GameObject
local m_uiCDKeyInput = nil; --UnityEngine.GameObject
local m_uiWaiting = nil; --UnityEngine.GameObject
local m_uiUnBindBtn = nil; --UnityEngine.GameObject

-- 打开界面
function SysConfigDlgOpen()
	m_Dlg = eye.uiManager:Open( "SysConfigDlg" );
end

-- 隐藏界面
function SysConfigDlgClose()
	if m_Dlg == nil then
		return;
	end
	eye.uiManager:Close( "SysConfigDlg" );
end

-- 删除界面
function SysConfigDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function SysConfigDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            SysConfigDlgClose();
		elseif nControlID == 1 then
			SysConfigDlgClickBgm()
		elseif nControlID == 2 then
			SysConfigDlgClickAudio()
		elseif nControlID == 3 then
			SysConfigDlgClickAutoHp()
		elseif nControlID >= 11 and nControlID < 20 then
			SysConfigDlgClickLang( nControlID )
		elseif nControlID == 21 then
			SysConfigDlgSendSignature()
		elseif nControlID == 22 then
			SysConfigDlgSendCdkey()	
		elseif nControlID == 31 then
			SysConfigDlgUnBind()
        end
		
	elseif nType == UI_EVENT_INPUTVALUECHANGED then
		if nControlID == 0 then
			local text = m_uiSignatureInput.transform:GetComponent( "UIInputField" ).text;
			if text == "" then
				SetTrue( m_uiSignatureInput.transform:Find("Hint") )
			else
				SetFalse( m_uiSignatureInput.transform:Find("Hint") )
			end
		elseif nControlID == 1 then
			local text = m_uiCDKeyInput.transform:GetComponent( "UIInputField" ).text;
			if text == "" then
				SetTrue( m_uiCDKeyInput.transform:Find("Hint") )
			else
				SetFalse( m_uiCDKeyInput.transform:Find("Hint") )
			end
		end
	end
end

-- 载入时调用
function SysConfigDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiBgm = objs[0];
	m_uiAudio = objs[1];
	m_uiAutoHp = objs[2];
	m_uiZH = objs[3];
	m_uiTW = objs[4];
	m_uiSignature = objs[5];
	m_uiCDKey = objs[6];
	m_uiSignatureInput = objs[7];
	m_uiCDKeyInput = objs[8];
	m_uiWaiting = objs[9];
	m_uiUnBindBtn = objs[10];
end

-- 界面初始化时调用
function SysConfigDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function SysConfigDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function SysConfigDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function SysConfigDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function SysConfigDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function SysConfigDlgShow()
	SysConfigDlgOpen()
	SysConfigDlgBaseReset()
	SysConfigDlgLangReset()
	
	if Const.platid == 15 or Const.platid == 23 or Const.platid == 27 or Const.platid == 38 or Const.platid == 31 or Const.platid == 33 or Const.platid == 36 then
		SetFalse(m_uiCDKey)
	else
		SetTrue(m_uiCDKey)
	end
	
	SysConfigDlgUnBindSetInfo()
end

function SysConfigDlgBaseReset()
	-- 背景音乐
	local onORoff = GameManager.ini( "SETTING_BGM", "1" )
	if onORoff == "1" then
		SetTrue(m_uiBgm.transform:Find("Flag/Icon"))
	else
		SetFalse(m_uiBgm.transform:Find("Flag/Icon"))
	end
	
	-- 游戏音效
	onORoff = GameManager.ini( "SETTING_AUDIO", "1" )
	if onORoff == "1" then
		SetTrue(m_uiAudio.transform:Find("Flag/Icon"))
	else
		SetFalse(m_uiAudio.transform:Find("Flag/Icon"))
	end
	
	-- 战前自动补兵
	onORoff = GameManager.ini( "SETTING_AUTOSUPPLY", "1" )
	if onORoff == "1" then
		SetTrue(m_uiAutoHp.transform:Find("Flag/Icon"))
	else
		SetFalse(m_uiAutoHp.transform:Find("Flag/Icon"))
	end
end

-- 背景音乐
function SysConfigDlgClickBgm()
	local onORoff = GameManager.ini( "SETTING_BGM", "1" )
	if onORoff == "1" then
		GameManager.writeini( "SETTING_BGM", "0" )
		eye.audioManager:SetAudioSwitch( 2, false )
	else
		GameManager.writeini( "SETTING_BGM", "1" )
		eye.audioManager:SetAudioSwitch( 2, true )
	end
	SysConfigDlgBaseReset()
end

-- 音效
function SysConfigDlgClickAudio()
	local onORoff = GameManager.ini( "SETTING_AUDIO", "1" )
	if onORoff == "1" then
		GameManager.writeini( "SETTING_AUDIO", "0" )
		eye.audioManager:SetAudioSwitch( 1, false )
		eye.audioManager:SetAudioSwitch( 3, false )
		eye.audioManager:SetAudioSwitch( 4, false )
	else
		GameManager.writeini( "SETTING_AUDIO", "1" )
		eye.audioManager:SetAudioSwitch( 1, true )
		eye.audioManager:SetAudioSwitch( 3, true )
		eye.audioManager:SetAudioSwitch( 4, true )
	end
	SysConfigDlgBaseReset()
end

-- 补兵
function SysConfigDlgClickAutoHp()
	local onORoff = GameManager.ini( "SETTING_AUTOSUPPLY", "1" )
	if onORoff == "1" then
		GameManager.writeini( "SETTING_AUTOSUPPLY", "0" )
		system_askinfo( ASKINFO_ACTOR, "", 6, CITY_FUNCTION_BATTLE_ADDHP, 0 );
	else
		GameManager.writeini( "SETTING_AUTOSUPPLY", "1" )
		system_askinfo( ASKINFO_ACTOR, "", 6, CITY_FUNCTION_BATTLE_ADDHP, 1 );
	end
	SysConfigDlgBaseReset()
end

-- 语言
function SysConfigDlgLangReset()
	local onORoff = GameManager.ini( "LANGUAGE", "zh" )
	if onORoff == "zh" then
		SetTrue(m_uiZH.transform:Find("Flag/Icon"))
		SetFalse(m_uiTW.transform:Find("Flag/Icon"))
	elseif onORoff == "tw" then
		SetFalse(m_uiZH.transform:Find("Flag/Icon"))
		SetTrue(m_uiTW.transform:Find("Flag/Icon"))
	end
end

-- 选择语言
function SysConfigDlgClickLang( nControlID )
	local onORoff = GameManager.ini( "LANGUAGE", "zh" )
	if nControlID == 11 and onORoff == "zh" then
		return
	end
	if nControlID == 12 and onORoff == "tw" then
		return
	end
	MsgBox( T(1723), function()
		if nControlID == 11 then
			GameManager.writeini( "LANGUAGE", "zh" )
		elseif nControlID == 12 then
			GameManager.writeini( "LANGUAGE", "tw" )
		end
		Invoke( function() 
			GameManager.Restart();
			GameManager.Logout( 1 );
		end, 0.3 );
	end )
end

-- 发送签名
function SysConfigDlgSendSignature()	
	local signature = m_uiSignatureInput.transform:GetComponent( "UIInputField" ).text;
	local len = string.len( signature )
	--非法检查
	if len == 0  then
		pop(T(1730))
		return;
	end
	if string.checksign( signature ) == false then
		pop(T(788))
		return
	end
	if Utils.MaskWordCheck( signature ) == false then
		pop(T(1732))
		return;
	end
	system_askinfo( ASKINFO_CHANGESIGN, signature, 0 );
	pop( T(1731) )
end

-- 发送兑换码
function SysConfigDlgSendCdkey()
	local cdkey =  m_uiCDKeyInput.transform:GetComponent( "UIInputField" ).text;
	local len = string.len( cdkey )
	--非法检查
	if len < 6 then
		pop(T(1735))
		return
	end
	system_askinfo( ASKINFO_CDKEY, cdkey, 0 );
end

-- 解绑
function SysConfigDlgUnBindSetInfo()
	if GetPlayer().m_usertype >= 100 then
		if SDK.logintype == "google" then
			SetTrue( m_uiUnBindBtn )
			SetText( m_uiUnBindBtn.transform:Find("Back/Text"), T(353) )
		elseif SDK.logintype == "facebook" then
			SetTrue( m_uiUnBindBtn )
			SetText( m_uiUnBindBtn.transform:Find("Back/Text"), T(352) )
		else
			SetFalse( m_uiUnBindBtn )
		end
	else
		SetFalse( m_uiUnBindBtn )
	end
end

-- 解绑
function SysConfigDlgUnBind()
	if SDK.logintype == "google" then
		SDK.UnBindGoogle()
	elseif SDK.logintype == "facebook" then
		SDK.UnBindFacebook()
	end
end
