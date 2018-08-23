-- 界面
local m_Dlg = nil;
local m_uiBindLayer = nil; --UnityEngine.GameObject
local m_uiBindDesc = nil; --UnityEngine.GameObject
local m_uiBindUserEdit = nil; --UnityEngine.GameObject
local m_uiBindPwdEdit = nil; --UnityEngine.GameObject
local m_uiBindPwdReEdit = nil; --UnityEngine.GameObject
local m_uiChangePwdLayer = nil; --UnityEngine.GameObject
local m_uiRegUserEdit = nil; --UnityEngine.GameObject
local m_uiRegPwdEdit = nil; --UnityEngine.GameObject
local m_uiRegPwdNewEdit = nil; --UnityEngine.GameObject
local m_uiRegPwdNewReEdit = nil; --UnityEngine.GameObject
local m_uiWaiting = nil; --UnityEngine.GameObject

-- 打开界面
function UserInfoDlgOpen()
	m_Dlg = eye.uiManager:Open( "UserInfoDlg" );
end

-- 隐藏界面
function UserInfoDlgClose()
	if m_Dlg == nil then
		return;
	end
	eye.uiManager:Close( "UserInfoDlg" );
end

-- 删除界面
function UserInfoDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function UserInfoDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            UserInfoDlgClose();
		elseif nControlID == 1 then
			UserInfoDlgBindLayer()
		elseif nControlID == 2 then
			UserInfoDlgChangePwdLayer()
		elseif nControlID == 101 then
			UserInfoDlgBindLayerSend()
		elseif nControlID == 102 then
			UserInfoDlgChangePwdLayerSend()
        end
	elseif nType == UI_EVENT_INPUTVALUECHANGED then
		if nControlID == 1 then
			local text = m_uiBindUserEdit.transform:Find("Input"):GetComponent( "UIInputField" ).text;
			if text == "" then
				SetTrue( m_uiBindUserEdit.transform:Find("Input/Hint") )
			else
				SetFalse( m_uiBindUserEdit.transform:Find("Input/Hint") )
			end
			
		elseif nControlID == 2 then
			local text = m_uiBindPwdEdit.transform:Find("Input"):GetComponent( "UIInputField" ).text;
			if text == "" then
				SetTrue( m_uiBindPwdEdit.transform:Find("Input/Hint") )
			else
				SetFalse( m_uiBindPwdEdit.transform:Find("Input/Hint") )
			end
			
		elseif nControlID == 3 then
			local text = m_uiBindPwdReEdit.transform:Find("Input"):GetComponent( "UIInputField" ).text;
			if text == "" then
				SetTrue( m_uiBindPwdReEdit.transform:Find("Input/Hint") )
			else
				SetFalse( m_uiBindPwdReEdit.transform:Find("Input/Hint") )
			end
			
		elseif nControlID == 11 then
			local text = m_uiRegUserEdit.transform:Find("Input"):GetComponent( "UIInputField" ).text;
			if text == "" then
				SetTrue( m_uiRegUserEdit.transform:Find("Input/Hint") )
			else
				SetFalse( m_uiRegUserEdit.transform:Find("Input/Hint") )
			end
			
		elseif nControlID == 12 then
			local text = m_uiRegPwdEdit.transform:Find("Input"):GetComponent( "UIInputField" ).text;
			if text == "" then
				SetTrue( m_uiRegPwdEdit.transform:Find("Input/Hint") )
			else
				SetFalse( m_uiRegPwdEdit.transform:Find("Input/Hint") )
			end
			
		elseif nControlID == 13 then
			local text = m_uiRegPwdNewEdit.transform:Find("Input"):GetComponent( "UIInputField" ).text;
			if text == "" then
				SetTrue( m_uiRegPwdNewEdit.transform:Find("Input/Hint") )
			else
				SetFalse( m_uiRegPwdNewEdit.transform:Find("Input/Hint") )
			end
			
		elseif nControlID == 14 then
			local text = m_uiRegPwdNewReEdit.transform:Find("Input"):GetComponent( "UIInputField" ).text;
			if text == "" then
				SetTrue( m_uiRegPwdNewReEdit.transform:Find("Input/Hint") )
			else
				SetFalse( m_uiRegPwdNewReEdit.transform:Find("Input/Hint") )
			end
		end
	end
end

-- 载入时调用
function UserInfoDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiBindLayer = objs[0];
	m_uiBindDesc = objs[1];
	m_uiBindUserEdit = objs[2];
	m_uiBindPwdEdit = objs[3];
	m_uiBindPwdReEdit = objs[4];
	m_uiChangePwdLayer = objs[5];
	m_uiRegUserEdit = objs[6];
	m_uiRegPwdEdit = objs[7];
	m_uiRegPwdNewEdit = objs[8];
	m_uiRegPwdNewReEdit = objs[9];
	m_uiWaiting = objs[10];
end

-- 界面初始化时调用
function UserInfoDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function UserInfoDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function UserInfoDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function UserInfoDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function UserInfoDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function UserInfoDlgShow()
	UserInfoDlgOpen()
	SetFalse( m_uiBindLayer )
	SetFalse( m_uiChangePwdLayer )
	UserInfoDlgBindLayer()
	
end

function UserInfoDlgBindLayer()
	SetTrue( m_uiBindLayer )
	SetFalse( m_uiChangePwdLayer )
	
	local name = GameManager.ini( "USERNAME", "" );
	local pwd = GameManager.ini( "PASSTOKEN", "" );
	if pwd == '0' or pwd == "" then
		SetTrue( m_uiBindDesc )
		SetTrue( m_uiBindUserEdit )
		m_uiBindUserEdit.transform:Find("Input"):GetComponent( "UIInputField" ).text = ""
		m_uiBindUserEdit.transform:Find("Input"):GetComponent( "UIInputField" ).readOnly = false;
		SetTrue( m_uiBindUserEdit.transform:Find("Input/Hint") )
		SetTrue( m_uiBindPwdEdit )
		SetTrue( m_uiBindPwdReEdit )
	else
		SetFalse( m_uiBindDesc )
		SetTrue( m_uiBindUserEdit )
		m_uiBindUserEdit.transform:Find("Input"):GetComponent( "UIInputField" ).text = name
		m_uiBindUserEdit.transform:Find("Input"):GetComponent( "UIInputField" ).readOnly = true;
		SetFalse( m_uiBindUserEdit.transform:Find("Input/Hint") )
		SetFalse( m_uiBindPwdEdit )
		SetFalse( m_uiBindPwdReEdit )
	end
end

-- 绑定账户
function UserInfoDlgBindLayerSend()
	local userName = m_uiBindUserEdit.transform:Find("Input"):GetComponent( "UIInputField" ).text
	local passWord = m_uiBindPwdEdit.transform:Find("Input"):GetComponent( "UIInputField" ).text
	local passWordRe = m_uiBindPwdReEdit.transform:Find("Input"):GetComponent( "UIInputField" ).text
	-- 非法检查
	local len = string.len( userName );
	if len < 4 or len > 64 then
		pop( T(417) );
		return
	end
	if string.checksign( userName ) == false then
		pop( T(417) );
		return
	end
	
	local pwdlen = string.len( passWord );
	if passWord == "" or pwdlen < 4 or pwdlen > 64 then
		pop( T(419) );
		return
	end
	
	if passWord ~= passWordRe then
		pop( T(416) );
		return
	end
	local devicename = GameManager.ini( "USERNAME", "" );
	SetTrue( m_uiWaiting )
	HttpRequest.BindUser( devicename, userName, passWord, function( response )
		SetFalse( m_uiWaiting )
		local json = require "cjson"
		local info = json.decode( response );
		if info == nil then
			netlog( response );
			return;
		end
		
		if info["result"] == -1 then
			pop( T(418) )
			return
		elseif info["result"] == -2 then
			pop( T(417) )
			return
		elseif info["result"] == -3 then
			pop( T(419) )
			return
		elseif info["result"] < 0 then
			pop( info["error"] )
			return
		end
	
		pop( T(448) );
		GameManager.writeini( "USERNAME", info["u"] );
		GameManager.writeini( "PASSTOKEN", info["p"] );
		GameManager.writeini( "LASTLOGINTYPE", 1 );	
		UserInfoDlgBindLayer()
	end )
end


function UserInfoDlgChangePwdLayer()
	local name = GameManager.ini( "USERNAME", "" );
	local pwd = GameManager.ini( "PASSTOKEN", "" ); 
	if pwd == '0' or pwd == "" then
		pop( T(4239) )
		return
	end
	SetFalse( m_uiBindLayer )
	SetTrue( m_uiChangePwdLayer )

	m_uiRegUserEdit.transform:Find("Input"):GetComponent( "UIInputField" ).text = name
	m_uiRegUserEdit.transform:Find("Input"):GetComponent( "UIInputField" ).readOnly = true;
	SetFalse( m_uiRegUserEdit.transform:Find("Input/Hint") )
end

-- 修改密码
function UserInfoDlgChangePwdLayerSend()
	local userName = m_uiRegUserEdit.transform:Find("Input"):GetComponent( "UIInputField" ).text
	local passWordOld = m_uiRegPwdEdit.transform:Find("Input"):GetComponent( "UIInputField" ).text
	local passWordNew = m_uiRegPwdNewEdit.transform:Find("Input"):GetComponent( "UIInputField" ).text
	local passWordNewRe = m_uiRegPwdNewReEdit.transform:Find("Input"):GetComponent( "UIInputField" ).text
	
	local pwdlen = string.len( passWordOld );
	if passWordOld == "" then
		pop( T(419) );
		return
	end
	
	pwdlen = string.len( passWordNew );
	if passWordNew == "" or pwdlen < 4 or pwdlen > 64 then
		pop( T(419) );
		return
	end
	
	pwdlen = string.len( passWordNewRe );
	if passWordNewRe == "" or pwdlen < 4 or pwdlen > 64 then
		pop( T(419) );
		return
	end
	
	if passWordNew ~= passWordNewRe then
		pop( T(416) );
		return
	end
	
	SetTrue( m_uiWaiting )
	HttpRequest.ChangePwd( userName, passWordOld, passWordNew, function(response)
		SetFalse( m_uiWaiting )
		local json = require "cjson"
		local info = json.decode( response );
		if info == nil then
			netlog( response );
			return;
		end
		
		if info["result"] == -1 then
			pop( T(505) )
			return
		elseif info["result"] == -2 then
			pop( T(417) )
			return
		elseif info["result"] == -3 then
			pop( T(419) )
			return
		elseif info["result"] < 0 then
			pop( info["error"] )
			return
		end
		pop( T(421) )
	end )
end
