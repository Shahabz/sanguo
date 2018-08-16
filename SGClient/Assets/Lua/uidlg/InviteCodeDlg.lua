-- 界面
local m_Dlg = nil;
local m_uiDesc = nil; --UnityEngine.GameObject
local m_uiDesc1 = nil; --UnityEngine.GameObject
local m_uiDesc2 = nil; --UnityEngine.GameObject
local m_uiRegPhoneEdit = nil; --UnityEngine.GameObject
local m_uiRegQQEdit = nil; --UnityEngine.GameObject
local m_uiRegWeiXinEdit = nil; --UnityEngine.GameObject
local m_uiRegInviteCodeEdit = nil; --UnityEngine.GameObject
local m_uiRegFriendCodeEdit = nil; --UnityEngine.GameObject
local m_uiWaiting = nil; --UnityEngine.GameObject
local m_uiListLayer = nil; --UnityEngine.GameObject
local m_uiScroll = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiUIP_Friend = nil; --UnityEngine.GameObject
local m_ObjectPool = nil
local m_friendlist = nil;
-- 打开界面
function InviteCodeDlgOpen()
	m_Dlg = eye.uiManager:Open( "InviteCodeDlg" );
end

-- 隐藏界面
function InviteCodeDlgClose()
	if m_Dlg == nil then
		return;
	end
	eye.uiManager:Close( "InviteCodeDlg" );
	m_friendlist = nil;
end

-- 删除界面
function InviteCodeDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function InviteCodeDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
		if nControlID == -2 then
			SetFalse( m_uiListLayer )
        elseif nControlID == -1 then
            InviteCodeDlgClose();
		elseif nControlID == 1 then
			InviteCodeDlgSend( 1 )
		elseif nControlID == 2 then
			InviteCodeDlgSend( 2 )
		elseif nControlID == 3 then
			InviteCodeDlgSend( 3 )
		elseif nControlID == 4 then
			InviteCodeDlgSend( 4 )
		elseif nControlID == 5 then
			InviteCodeDlgGetList()
		elseif nControlID >= 1000 then
			InviteCodeDlgListGetAward( nControlID - 1000 )
        end
	elseif nType == UI_EVENT_INPUTVALUECHANGED then
		if nControlID == 1 then
			local text = m_uiRegPhoneEdit.transform:Find("Input"):GetComponent( "UIInputField" ).text;
			if text == "" then
				SetTrue( m_uiRegPhoneEdit.transform:Find("Input/Hint") )
			else
				SetFalse( m_uiRegPhoneEdit.transform:Find("Input/Hint") )
			end
			
		elseif nControlID == 2 then
			local text = m_uiRegQQEdit.transform:Find("Input"):GetComponent( "UIInputField" ).text;
			if text == "" then
				SetTrue( m_uiRegQQEdit.transform:Find("Input/Hint") )
			else
				SetFalse( m_uiRegQQEdit.transform:Find("Input/Hint") )
			end
			
		elseif nControlID == 3 then
			local text = m_uiRegWeiXinEdit.transform:Find("Input"):GetComponent( "UIInputField" ).text;
			if text == "" then
				SetTrue( m_uiRegWeiXinEdit.transform:Find("Input/Hint") )
			else
				SetFalse( m_uiRegWeiXinEdit.transform:Find("Input/Hint") )
			end
			
		elseif nControlID == 4 then
			local text = m_uiRegInviteCodeEdit.transform:Find("Input"):GetComponent( "UIInputField" ).text;
			if text == "" then
				SetTrue( m_uiRegInviteCodeEdit.transform:Find("Input/Hint") )
			else
				SetFalse( m_uiRegInviteCodeEdit.transform:Find("Input/Hint") )
			end
			
		elseif nControlID == 5 then
			local text = m_uiRegFriendCodeEdit.transform:Find("Input"):GetComponent( "UIInputField" ).text;
			if text == "" then
				SetTrue( m_uiRegFriendCodeEdit.transform:Find("Input/Hint") )
			else
				SetFalse( m_uiRegFriendCodeEdit.transform:Find("Input/Hint") )
			end
		end
	end
end

-- 载入时调用
function InviteCodeDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;
	m_uiDesc = objs[0];
	m_uiDesc1 = objs[1];
	m_uiDesc2 = objs[2];
	m_uiRegPhoneEdit = objs[3];
	m_uiRegQQEdit = objs[4];
	m_uiRegWeiXinEdit = objs[5];
	m_uiRegInviteCodeEdit = objs[6];
	m_uiRegFriendCodeEdit = objs[7];
	m_uiWaiting = objs[8];
	m_uiListLayer = objs[9];
	m_uiScroll = objs[10];
	m_uiContent = objs[11];
	m_uiUIP_Friend = objs[12];
	
	-- 对象池
	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );
	m_ObjectPool:CreatePool("UIP_Friend", 3, 3, m_uiUIP_Friend);	
end

-- 界面初始化时调用
function InviteCodeDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function InviteCodeDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function InviteCodeDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function InviteCodeDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function InviteCodeDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function InviteCodeDlgShow()
	InviteCodeDlgOpen()
	SetTrue( m_uiWaiting )
	SetFalse( m_uiDesc )
	SetFalse( m_uiDesc1 )
	SetFalse( m_uiDesc2 )
	SetFalse( m_uiRegPhoneEdit )
	SetFalse( m_uiRegQQEdit )
	SetFalse( m_uiRegWeiXinEdit )
	SetFalse( m_uiRegInviteCodeEdit )
	SetFalse( m_uiRegFriendCodeEdit )
	
	HttpRequest.InviteCode( function( response )
		local json = require "cjson"
		local info = json.decode( response );
		if info == nil then
			netlog( response );
			return;
		end
		if info["result"] == 0 then
			InviteCodeDlgSet( info )
		end
		SetFalse( m_uiWaiting )
	end );
end

-- 设置信息
function InviteCodeDlgSet( info )
	SetTrue( m_uiDesc )
	SetText( m_uiDesc, info["desc"] )
	
	SetTrue( m_uiDesc1 )
	SetText( m_uiDesc1, info["desc1"] )
	
	SetTrue( m_uiDesc2 )
	SetText( m_uiDesc2, info["desc2"] )
	
	SetTrue( m_uiRegPhoneEdit )
	if info["phone"] ~= "" then
		m_uiRegPhoneEdit.transform:Find("Input"):GetComponent( "UIInputField" ).text = info["phone"]
		m_uiRegPhoneEdit.transform:Find("Input"):GetComponent( "UIInputField" ).readOnly = true;
		SetFalse( m_uiRegPhoneEdit.transform:Find("Input/Hint") )
	else
		m_uiRegPhoneEdit.transform:Find("Input"):GetComponent( "UIInputField" ).readOnly = false;
		SetTrue( m_uiRegPhoneEdit.transform:Find("Input/Hint") )
		m_uiRegPhoneEdit.transform:Find("Input"):GetComponent( "UIInputField" ).text = "";
	end
	if info["award_phone"] == "0" then
		SetTrue( m_uiRegPhoneEdit.transform:Find("AwardButton") )
	else
		SetFalse( m_uiRegPhoneEdit.transform:Find("AwardButton") )
	end
	
	SetTrue( m_uiRegQQEdit )
	if info["qq"] ~= "" then
		m_uiRegQQEdit.transform:Find("Input"):GetComponent( "UIInputField" ).text = info["qq"]
		m_uiRegQQEdit.transform:Find("Input"):GetComponent( "UIInputField" ).readOnly = true;
		SetFalse( m_uiRegQQEdit.transform:Find("Input/Hint") )
	else
		m_uiRegQQEdit.transform:Find("Input"):GetComponent( "UIInputField" ).readOnly = false;
		SetTrue( m_uiRegQQEdit.transform:Find("Input/Hint") )
		m_uiRegQQEdit.transform:Find("Input"):GetComponent( "UIInputField" ).text = ""
	end
	if info["award_qq"] == "0" then
		SetTrue( m_uiRegQQEdit.transform:Find("AwardButton") )
	else
		SetFalse( m_uiRegQQEdit.transform:Find("AwardButton") )
	end
	
	SetTrue( m_uiRegWeiXinEdit )
	if info["wchat"] ~= "" then
		m_uiRegWeiXinEdit.transform:Find("Input"):GetComponent( "UIInputField" ).text = info["wchat"]
		m_uiRegWeiXinEdit.transform:Find("Input"):GetComponent( "UIInputField" ).readOnly = true;
		SetFalse( m_uiRegWeiXinEdit.transform:Find("Input/Hint") )
	else
		m_uiRegWeiXinEdit.transform:Find("Input"):GetComponent( "UIInputField" ).readOnly = false;
		SetTrue( m_uiRegWeiXinEdit.transform:Find("Input/Hint") )
		m_uiRegWeiXinEdit.transform:Find("Input"):GetComponent( "UIInputField" ).text = ""
	end
	if info["award_wchat"] == "0" then
		SetTrue( m_uiRegWeiXinEdit.transform:Find("AwardButton") )
	else
		SetFalse( m_uiRegWeiXinEdit.transform:Find("AwardButton") )
	end
	
	SetTrue( m_uiRegInviteCodeEdit )
	local InviteCodeInput = m_uiRegInviteCodeEdit.transform:Find("Input"):GetComponent( "UIInputField" )
	InviteCodeInput.readOnly = false;
	InviteCodeInput.text = info["invite_code"]
	InviteCodeInput.readOnly = true;
	
	SetTrue( m_uiRegFriendCodeEdit )
	if info["friend_invite_code"] ~= "" then
		m_uiRegFriendCodeEdit.transform:Find("Input"):GetComponent( "UIInputField" ).text = info["friend_invite_code"]
		m_uiRegFriendCodeEdit.transform:Find("Input"):GetComponent( "UIInputField" ).readOnly = true;
		SetFalse( m_uiRegFriendCodeEdit.transform:Find("Input/Hint") )
		
		if info["award_invite_code"] == "0" then
			SetTrue( m_uiRegFriendCodeEdit.transform:Find("AwardButton") )
		else
			SetFalse( m_uiRegFriendCodeEdit.transform:Find("AwardButton") )
		end
	
	else
		m_uiRegFriendCodeEdit.transform:Find("Input"):GetComponent( "UIInputField" ).readOnly = true;
		SetTrue( m_uiRegFriendCodeEdit.transform:Find("Input/Hint") )
		m_uiRegFriendCodeEdit.transform:Find("Input"):GetComponent( "UIInputField" ).text = ""
		SetFalse( m_uiRegFriendCodeEdit.transform:Find("AwardButton") )
	end
end

function InviteCodeDlgSend( path )
	local msg = "";
	if path == 1 then
		msg = m_uiRegPhoneEdit.transform:Find("Input"):GetComponent( "UIInputField" ).text
	elseif path == 2 then
		msg = m_uiRegQQEdit.transform:Find("Input"):GetComponent( "UIInputField" ).text
	elseif path == 3 then
		msg = m_uiRegWeiXinEdit.transform:Find("Input"):GetComponent( "UIInputField" ).text
	elseif path == 4 then
		msg = m_uiRegFriendCodeEdit.transform:Find("Input"):GetComponent( "UIInputField" ).text
	end
	
	-- 非法检查
	local len = string.len( msg );
	if len < 3 or len > 32 then
		if path == 1 then
			pop( T(4225) )
		elseif path == 2 then
			pop( T(4226) )
		elseif path == 3 then
			pop( T(4227) )
		elseif path == 4 then
			pop( T(4228) )
		end
		return
	end
	SetTrue( m_uiWaiting )
	system_askinfo( ASKINFO_INVITECODE, msg, 0, path )
end

function InviteCodeDlgOpRecv( result, path )
	if path == 1 then
		if result == -1 then
			SetFalse( m_uiWaiting )
			pop( T(4225) )
			return
		end
	elseif path == 2 then
		if result == -1 then
			SetFalse( m_uiWaiting )
			pop( T(4226) )
			return
		end
	elseif path == 3 then
		if result == -1 then
			SetFalse( m_uiWaiting )
			pop( T(4227) )
			return
		end
	elseif path == 4 then
		if result == -1 then
			SetFalse( m_uiWaiting )
			pop( T(4228) )
			return
		end
	elseif path == 5 then
		if result == -1 then
			SetFalse( m_uiWaiting )
			return
		end
		InviteCodeDlgGetList()
		return
	end

	HttpRequest.InviteCode( function( response )
		SetFalse( m_uiWaiting )
		local json = require "cjson"
		local info = json.decode( response );
		if info == nil then
			netlog( response );
			return;
		end
		if info["result"] == 0 then
			InviteCodeDlgSet( info )
		end
	end );
end

function InviteCodeDlgSetFriendObj( index, info )
	local uiObj = m_ObjectPool:Get( "UIP_Friend" );
	uiObj.transform:SetParent( m_uiContent.transform );	

	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiName = objs[0]
	local uiTime = objs[1]
	local uiHas = objs[2]
	local uiButton = objs[3]
	
	SetControlID( uiButton, 1000 + index );
	SetText( uiName, info["n"] );
	SetText( uiTime, getDateStringByInterval(info["t"]) );
	
	if tonumber(info["h"]) > 0 then
		-- 已经领取
		SetFalse( uiButton )
		SetTrue( uiHas )
	else
		SetTrue( uiButton )
		SetFalse( uiHas )
	end
end

function FriendDlgFriendClear()
	local objs = {};
	for i = 0 ,m_uiContent.transform.childCount - 1 do
		table.insert( objs, m_uiContent.transform:GetChild(i).gameObject )
    end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "UIP_Friend(Clone)" then
			m_ObjectPool:Release( "UIP_Friend", obj );
		end
    end
end

function InviteCodeDlgGetList()
	SetTrue( m_uiListLayer )
	FriendDlgFriendClear()
	local InviteCode = m_uiRegInviteCodeEdit.transform:Find("Input"):GetComponent( "UIInputField" ).text
	HttpRequest.InviteCodeFriend( InviteCode, function( response )
		SetFalse( m_uiWaiting )
		local json = require "cjson"
		local info = json.decode( response );
		if info == nil then
			netlog( response );
			return;
		end
		m_friendlist = info["list"];
		for i=1, #m_friendlist, 1 do
			InviteCodeDlgSetFriendObj( i, m_friendlist[i] )
		end
	end );
end

function InviteCodeDlgListGetAward( index )
	local info = m_friendlist[index]
	if info == nil then
		return
	end
	SetTrue( m_uiWaiting )
	system_askinfo( ASKINFO_INVITECODE, info["n"], 0, 5 )
end