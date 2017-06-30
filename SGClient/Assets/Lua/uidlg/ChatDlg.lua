-- 界面
local m_Dlg = nil;
local m_uiScrollView = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiUIP_ChatItem = nil; --UnityEngine.GameObject
local m_uiChannelBtn1 = nil; --UnityEngine.GameObject
local m_uiChannelBtn2 = nil; --UnityEngine.GameObject
local m_uiChatEdit = nil; --UnityEngine.GameObject
local m_uiUIP_ChatItemSelf = nil; --UnityEngine.GameObject
local m_objectPool = {};
-- 打开界面
function ChatDlgOpen()
	m_Dlg = eye.uiManager:Open( "ChatDlg" );
end

-- 隐藏界面
function ChatDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "ChatDlg" );
end

-- 删除界面
function ChatDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function ChatDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            ChatDlgClose();
		elseif nControlID == 1 then
			ChatDlgSend()
		elseif nControlID == 2 then
		elseif nControlID == 3 then
		elseif nControlID == 4 then
        end
	end
end

-- 载入时调用
function ChatDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;
	m_uiScrollView = objs[0];
	m_uiContent = objs[1];
	m_uiUIP_ChatItem = objs[2];
	m_uiChannelBtn1 = objs[3];
	m_uiChannelBtn2 = objs[4];
	m_uiChatEdit = objs[5];
	m_uiUIP_ChatItemSelf = objs[6];
end

-- 界面初始化时调用
function ChatDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function ChatDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function ChatDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function ChatDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function ChatDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function ChatDlgShow()
	ChatDlgOpen();
	for i=1, 10, 1 do
		ChatDlgAddMsg( 1, math.random(0,5), math.random(1,100), "wwdadwad", 1, "BloodImage继承至RawImage，所以只能接收Texture类型的图片源，他主要负责的就是根据血条的长度对图片进行循环排版" )
	end
end

function ChatDlgAddMsg( actorid, shape, level, name, zone, msg )
	local uiObj = nil;
	if actorid == GetPlayer().m_actorid then
		uiObj = GameObject.Instantiate( m_uiUIP_ChatItemSelf );
	else
		uiObj = GameObject.Instantiate( m_uiUIP_ChatItem );
	end
	uiObj.gameObject:SetActive( true );
	uiObj.transform:SetParent( m_uiContent.transform );
	uiObj.transform.localScale = Vector3.one;
	uiObj.transform:Find("Head/Back"):GetComponent( "Image" ).sprite = PlayerHeadSprite( shape );
	uiObj.transform:Find("Name"):GetComponent( "YlyRichText" ).text = "<emote=001><color=00AE01FF>Lv."..level.." "..name.."</color> <color=00FFC0FF>青州</color>"
	uiObj.transform:Find("Chat/Text"):GetComponent( "YlyRichText" ).text = msg
	
	table.insert( m_objectPool, uiObj );
	--m_uiScrollView:GetComponent("ScrollRect"):ResetScroll();
	m_uiScrollView:GetComponent("ScrollRect"):ScrollToBottom();
end

function ChatDlgSend()
	local msg = m_uiChatEdit.transform:Find("Input"):GetComponent( "UIInputField" ).text;
	if msg == "" then
		return;
	end
	ChatDlgAddMsg( GetPlayer().m_actorid, GetPlayer().m_shape, GetPlayer().m_level, GetPlayer().m_name, GetPlayer().m_zone, msg )
end
