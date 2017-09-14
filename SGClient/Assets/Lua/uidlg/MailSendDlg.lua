-- 界面
local m_Dlg = nil;
local m_uiRecvName = nil; --UnityEngine.GameObject
local m_uiContentField = nil; --UnityEngine.GameObject
local m_uiCost = nil; --UnityEngine.GameObject
local m_uiRichText = nil; --UnityEngine.GameObject
local m_uiSendButton = nil; --UnityEngine.GameObject

local m_unit_index = -1;
local m_actorid = 0;
local m_from_msg = "";
local m_from_recvtime = 0;

-- 打开界面
function MailSendDlgOpen()
	m_Dlg = eye.uiManager:Open( "MailSendDlg" );
end

-- 隐藏界面
function MailSendDlgClose()
	if m_Dlg == nil then
		return;
	end
	m_unit_index = -1;
	m_actorid = 0;
	m_from_msg = "";
	eye.uiManager:Close( "MailSendDlg" );
end

-- 删除界面
function MailSendDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function MailSendDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            MailSendDlgClose();
		elseif nControlID == 1 then
			MailSendDlgSendMail();
		elseif nControlID == 2 then
			MailSendDlgReplyMail();
        end
		
	elseif nType == UI_EVENT_INPUTVALUECHANGED then
		SetRichText( m_uiRichText, m_uiContentField:GetComponent("UIInputField").text )
	end
end

-- 载入时调用
function MailSendDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiRecvName = objs[0];
	m_uiContentField = objs[1];
	m_uiCost = objs[2];
	m_uiRichText = objs[3];
	m_uiSendButton = objs[4];
end

-- 界面初始化时调用
function MailSendDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function MailSendDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function MailSendDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function MailSendDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function MailSendDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function MailSendDlgSend( recvValue )
	MailSendDlgOpen()
	m_unit_index 	= recvValue.m_unit_index;
	local name 		= recvValue.m_name;
	local nation	= recvValue.m_char_value[2];
	
	SetText( m_uiRecvName, T(1196)..":["..Nation(nation).."]"..name )
	if nation == GetPlayer().m_nation then
		SetFalse( m_uiCost )
	else
		SetTrue( m_uiCost )
		SetText( m_uiCost.transform:Find("Text"), global.nation_mail_token )
	end
	SetControlID( m_uiSendButton, 1 );
end

function MailSendDlgReply( from_actorid, from_actorname, from_actornation, from_msg, from_recvtime )
	MailSendDlgOpen()
	
	m_actorid		= from_actorid;
	m_from_msg	    = from_msg;
	m_from_recvtime = from_recvtime;
	local name 		= from_actorname;
	local nation	= from_actornation;
	SetText( m_uiRecvName, T(1196)..":["..Nation(nation).."]"..name )
	if nation == GetPlayer().m_nation then
		SetFalse( m_uiCost )
	else
		SetTrue( m_uiCost )
		SetText( m_uiCost.transform:Find("Text"), global.nation_mail_token )
	end
	SetControlID( m_uiSendButton, 2 );
end

-- 发送邮件
function MailSendDlgSendMail()	
	local content = m_uiRichText.transform:GetComponent( typeof(YlyRichText) ).text;
	local len = string.len( content )
	if len < 4 or len >= 256 then
		pop(T(1195))
		return;
	end
	-- m_unit_index=0,m_actorid=0,m_content_length=0,m_content="[m_content_length]",
	local sendValue = {}
	sendValue.m_unit_index = m_unit_index;
	sendValue.m_actorid = m_actorid;
	sendValue.m_content = content;
	sendValue.m_content_length = len;
	netsend_mailsend_C( sendValue )	
	MailSendDlgClose()
	pop( T(1194) )
end

-- 回复邮件
function MailSendDlgReplyMail()	
	local content = m_uiRichText.transform:GetComponent( typeof(YlyRichText) ).text;
	local len = string.len( content )
	if len < 4 or len >= 256 then
		pop(T(1195))
		return;
	end
	print( len )
	-- m_actorid=0,m_content_length=0,m_content="[m_content_length]",m_reply_length=0,m_reply="[m_reply_length]",m_reply_recvtime,
	local sendValue = {}
	sendValue.m_actorid = m_actorid;
	sendValue.m_content = content;
	sendValue.m_content_length = len;
	sendValue.m_reply = m_from_msg;
	sendValue.m_reply_length = string.len( m_from_msg );
	if sendValue.m_reply_length > 500 then
		sendValue.m_reply_length = 500;
	end
	sendValue.m_reply_recvtime = m_from_recvtime
	netsend_mailreply_C( sendValue )	
	MailSendDlgClose()
	pop( T(1194) )
end