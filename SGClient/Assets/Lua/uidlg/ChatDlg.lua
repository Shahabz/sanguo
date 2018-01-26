-- 界面
local json = require "cjson"
local m_Dlg = nil;
local m_uiScrollView = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiUIP_ChatItem = nil; --UnityEngine.GameObject
local m_uiNationBtn = nil; --UnityEngine.GameObject
local m_uiZoneBtn = nil; --UnityEngine.GameObject
local m_uiChatEdit = nil; --UnityEngine.GameObject
local m_uiUIP_ChatItemSelf = nil; --UnityEngine.GameObject
local m_uiInput = nil; --UnityEngine.GameObject
local m_uiUIP_ChatTime = nil; --UnityEngine.GameObject
local m_uiUIP_ChatSystalk = nil; --UnityEngine.GameObject
local m_uiNationSelectBtn = nil; --UnityEngine.GameObject
local m_uiZoneSelectBtn = nil; --UnityEngine.GameObject
local m_uiTitleText = nil; --UnityEngine.GameObject
local m_uiScrollViewZone = nil; --UnityEngine.GameObject
local m_uiContentZone = nil; --UnityEngine.GameObject
local m_uiHornLayer = nil; --UnityEngine.GameObject
local m_uiItem = nil; --UnityEngine.GameObject
local m_uiContentField = nil; --UnityEngine.GameObject
local m_uiBuyButton = nil; --UnityEngine.GameObject
local m_uiSendButton = nil; --UnityEngine.GameObject
local m_uiOpLayer = nil; --UnityEngine.GameObject
local m_uiOpForbidBtn = nil; --UnityEngine.GameObject

local m_ObjectPool = nil;
local m_SelectChannel = 0;

local m_ChatCache = {};
local m_ChatCacheCount = 0;
local m_ChatCacheObj = {};

local m_LastTime = 0;
local m_horn_itemnum = 0;

local m_OpLayerShow = false;
local m_clickActorid = 0;

CHAT_MSGTYPE_ACTORCHAT			=	0		-- 消息类型-玩家聊天
CHAT_MSGTYPE_VS					=	1		-- 消息类型-对战
CHAT_MSGTYPE_SPY				=	2		-- 消息类型-侦察
CHAT_MSGTYPE_ATTACK_ASKHELP		=	3		-- 消息类型-攻击请求支援
CHAT_MSGTYPE_DEFENSE_ASKHELP	=	4		-- 消息类型-防守请求支援

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
		elseif nControlID == -2 then
			ChatDlgHornLayerHide()
		elseif nControlID == -3 then
			ChatDlgOpLayerHide()
			
		-- 发送
		elseif nControlID == 1 then
			ChatDlgSend();
			
		-- 喇叭
		elseif nControlID == 2 then
			ChatDlgHornLayerShow()
			
		-- 选择国家频道
		elseif nControlID == 3 then
			SetFalse( m_uiNationSelectBtn )
			SetFalse( m_uiZoneSelectBtn )
			SetFalse( m_uiZoneBtn )
			SetTrue( m_uiNationBtn )
			SetText( m_uiTitleText, T(2057) )
			SetTrue( m_uiScrollView )
			SetFalse( m_uiScrollViewZone )
			ChatDlgSelectChannel( 0 )
			
		-- 选择区域频道
		elseif nControlID == 4 then
			SetFalse( m_uiNationSelectBtn )
			SetFalse( m_uiZoneSelectBtn )
			SetFalse( m_uiNationBtn )
			SetTrue( m_uiZoneBtn )
			SetText( m_uiTitleText, T(2058) )
			SetFalse( m_uiScrollView )
			SetTrue( m_uiScrollViewZone )
			ChatDlgSelectChannel( 1 )
			
		-- 点击国家频道
		elseif nControlID == 5 then
			SetFalse( m_uiNationSelectBtn )
			SetTrue( m_uiZoneSelectBtn )
		
		-- 点击区域频道
		elseif nControlID == 6 then
			SetFalse( m_uiZoneSelectBtn )
			SetTrue( m_uiNationSelectBtn )
		
		-- 黑名单
		elseif nControlID == 10 then
			BlackListDlgShow()
			
		-- 喇叭-购买并使用
		elseif nControlID == 11 then
			ChatDlgHornBuy()
			
		-- 喇叭-发送
		elseif nControlID == 12 then
			ChatDlgHornSend()
		
		-- 操作-查看信息
		elseif nControlID == 21 then
			ChatDlgOpSearch()
			
		-- 操作-加为好友
		elseif nControlID == 22 then
			ChatDlgOpFriend()
		-- 操作-屏蔽说话
		elseif nControlID == 23 then
			ChatDlgOpForbid()
			
		-- 点击玩家
		elseif nControlID > 1000 then
			ChatDlgClickActor( nControlID - 1000, value )
        end
		
	elseif nType == UI_EVENT_INPUTVALUECHANGED then
		if nControlID == 0 then
			local text = m_uiChatEdit.transform:Find("Input"):GetComponent( "UIInputField" ).text;
			if text == "" then
				SetTrue( m_uiChatEdit.transform:Find("Input/Hint") )
			else
				SetFalse( m_uiChatEdit.transform:Find("Input/Hint") )
			end
		elseif nControlID == 1 then
			local text = m_uiContentField.transform:GetComponent( "UIInputField" ).text;
			if text == "" then
				SetTrue( m_uiContentField.transform:Find("Hint") )
			else
				SetFalse( m_uiContentField.transform:Find("Hint") )
			end
		end
		
	elseif nType == UI_EVENT_SCROLLDRAG then
		if nControlID == 0 then
			if m_OpLayerShow == true then
				ChatDlgOpLayerHide()
			end
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
	m_uiNationBtn = objs[3];
	m_uiZoneBtn = objs[4];
	m_uiChatEdit = objs[5];
	m_uiUIP_ChatItemSelf = objs[6];
	m_uiInput = objs[7];
	m_uiUIP_ChatTime = objs[8];
	m_uiUIP_ChatSystalk = objs[9];
	m_uiNationSelectBtn = objs[10];
	m_uiZoneSelectBtn = objs[11];
	m_uiTitleText = objs[12];
	m_uiScrollViewZone = objs[13];
	m_uiContentZone = objs[14];
	m_uiHornLayer = objs[15];
	m_uiItem = objs[16];
	m_uiContentField = objs[17];
	m_uiBuyButton = objs[18];
	m_uiSendButton = objs[19];
	m_uiOpLayer = objs[20];
	m_uiOpForbidBtn = objs[21];
	
	-- 对象池
	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );
	m_ObjectPool:CreatePool("UIP_ChatItem", 5, 5, m_uiUIP_ChatItem);
	m_ObjectPool:CreatePool("UIP_ChatItemSelf", 1, 1, m_uiUIP_ChatItemSelf);
	m_ObjectPool:CreatePool("UIP_ChatSystalk", 1, 1, m_uiUIP_ChatSystalk);
	m_ObjectPool:CreatePool("UIP_ChatTime", 1, 1, m_uiUIP_ChatTime);
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
	if m_SelectChannel == 0 then
		ChatDlgScrollToBottom( m_uiScrollView );
	else
		ChatDlgScrollToBottom( m_uiScrollViewZone );
	end
	SetFalse( m_uiOpLayer )
end

-- 解析内容
function ChatDlgMakeMsg( recvValue )
	local msg = ""
	if recvValue.m_msgtype == CHAT_MSGTYPE_VS then
		local info = json.decode( recvValue.m_msg );
		msg = F( 1122, info["aname"], info["dname"], info["mailid"] );
		
	elseif recvValue.m_msgtype == CHAT_MSGTYPE_SPY then
		local info = json.decode( recvValue.m_msg );
		msg = F( 1123, info["aname"], info["dname"], info["mailid"] );
		
	elseif recvValue.m_msgtype == CHAT_MSGTYPE_ATTACK_ASKHELP then
		local info = json.decode( recvValue.m_msg );
		msg = F( 1124, NationEx(info["n"]), info["na"], info["pos"] );
		
	elseif recvValue.m_msgtype == CHAT_MSGTYPE_DEFENSE_ASKHELP then
		local info = json.decode( recvValue.m_msg );
		msg = F( 1125, info["pos"], NationEx(info["n"]), info["na"] );
	
	else
		local horn = ""
		if recvValue.m_channel == 1 then
			horn = "<icon=horn>"
		end
		msg = horn..recvValue.m_msg
	end
	return msg;
end

-- 添加聊天消息
-- m_actorid=0,m_shape=0,m_level=0,m_namelen=0,m_name="[m_namelen]",m_frame=0,m_zone=0,m_place=0,m_msglen=0,m_msg="[m_msglen]",m_optime=0,m_channel=0,m_nation=0,
function ChatDlgAddMsg( uiRoot, recvValue )
	local uiObj = nil;
	if recvValue.m_actorid == GetPlayer().m_actorid then
		uiObj = m_ObjectPool:Get( "UIP_ChatItemSelf" );
	else
		uiObj = m_ObjectPool:Get( "UIP_ChatItem" );
	end
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiHead = objs[0]
	local uiShape = objs[1]
	local uiName = objs[2]
	local uiArrow = objs[3]
	local uiBack = objs[4]
	local uiText = objs[5]
	
	SetControlID( uiHead, 1000 + recvValue.m_actorid )
	
	-- 玩家头像
	SetImage( uiShape, PlayerHeadSprite( recvValue.m_shape ) );
	
	-- 国家
	local nation = ""
	
	-- 世界
	if recvValue.m_channel == 1 then
		SetImage( uiArrow, LoadSprite("ui_chatdlg_talkarrow_2") );
		SetImage( uiBack, LoadSprite("ui_chatdlg_talkback_2") );
		SetRichTextColor( uiText, Hex2Color(0xf7f3bbff) )
		nation = " <color="..NationColorStr(recvValue.m_nation)..">["..Nation( recvValue.m_nation ).."]</color>"
		
	-- 国家
	else
		SetImage( uiArrow, LoadSprite("ui_chatdlg_talkarrow_1") );
		SetImage( uiBack, LoadSprite("ui_chatdlg_talkback_1") );
		SetRichTextColor( uiText, Hex2Color(0x685036FF) )
	end
	
	-- 名字等级区域等信息
	SetRichText( uiName, PlaceRichText( recvValue.m_place ).."<color=00FFC0FF>Lv."..recvValue.m_level.." "..recvValue.m_name.."</color> <color=FF00EDFF>"..MapZoneName( recvValue.m_zone ).."</color>"..nation );
	
	-- 内容
	if recvValue.m_msgtype == CHAT_MSGTYPE_VS or recvValue.m_msgtype == CHAT_MSGTYPE_SPY then
		SetRichText( uiText, ChatDlgMakeMsg( recvValue ), ChatDlgOnLinkClickMail );
	else
		SetRichText( uiText, ChatDlgMakeMsg( recvValue ), ChatDlgOnLinkClickPos );
	end
	uiObj.transform:SetParent( uiRoot.transform );
	uiObj.transform.localScale = Vector3.one;
	uiObj.gameObject:SetActive( true );	
end

-- 添加系统消息
function ChatDlgAddSysTalk( uiRoot, recvValue )
	local uiObj = m_ObjectPool:Get( "UIP_ChatSystalk" );
	SetRichText( uiObj.transform:Find("Talk/Text"), recvValue.m_msg, ChatDlgOnLinkClickPos )
	uiObj.transform:SetParent( uiRoot.transform );
	uiObj.transform.localScale = Vector3.one;
	uiObj.gameObject:SetActive( true );
end

-- 添加时间
function ChatDlgAddTime( uiRoot, optime )
	local uiObj = m_ObjectPool:Get( "UIP_ChatTime" );
	uiObj.transform:Find("Content/Text"):GetComponent( "UIText" ).text = os.date( "%m-%d %H:%M", optime )
	uiObj.transform:SetParent( uiRoot.transform );
	uiObj.transform.localScale = Vector3.one;
	uiObj.gameObject:SetActive( true );
end

-- 滑动到底部
function ChatDlgScrollToBottom( uiObj )
	--m_uiScrollView:GetComponent("UIScrollRect"):ResetScroll();
	--m_uiScrollView:GetComponent("UIScrollRect").normalizedPosition = Vector2.zero;
	uiObj:GetComponent("UIScrollRect"):ScrollToBottom();
end

-- 发送消息
function ChatDlgSend()
	local msg = m_uiInput:GetComponent( "UIInputField" ).text;
	if msg == "" then
		return;
	end
	if string.len(msg) >= 127 then
		return;
	end
	m_uiInput:GetComponent("UIInputField").text = "";
	
	-- GM指令检查
	local cmdsign = string.sub(msg, 1, 1);
    if cmdsign == "." then
        local str = string.sub(msg, 2);
        local strarray = string.split(str, " ");
        if GMCommandCheck(strarray) == true then
            return;
        end
    end
	
	-- 发送
	local sendValue = {};
	sendValue.m_channel = 0;
	sendValue.m_msg = msg;
	sendValue.m_msglen = string.len( msg );
	netsend_chat_C( sendValue );
end

-- 发送消息-喇叭
function ChatDlgSendHorn()
	local msg = m_uiContentField.transform:GetComponent( "UIInputField" ).text;
	if msg == "" then
		return;
	end
	if string.len(msg) >= 127 then
		return;
	end
	m_uiContentField.transform:GetComponent( "UIInputField" ).text = "";
		
	-- 发送
	local sendValue = {};
	sendValue.m_channel = 1;
	sendValue.m_msg = msg;
	sendValue.m_msglen = string.len( msg );
	netsend_chat_C( sendValue );
end

-- 接收消息
-- m_actorid=0,m_shape=0,m_level=0,m_namelen=0,m_name="[m_namelen]",m_frame=0,m_zone=0,m_place=0,m_msglen=0,m_msg="[m_msglen]",m_optime=0,m_channel=0,m_nation=0,
function ChatDlgRecv( recvValue )
	-- 缓存
	if m_ChatCacheCount >= 100 then
		table.remove( m_ChatCache, 1 );
		
		if m_ChatCacheObj[1] ~= nil then
			
		end
	end
	table.insert( m_ChatCache, recvValue );
	m_ChatCacheCount = m_ChatCacheCount + 1;
	
	if m_Dlg == nil then
		ChatDlgOpen()
		ChatDlgClose()
	end
	
	-- 超过10分钟添加时间
	if recvValue.m_optime - m_LastTime > 600 then
		ChatDlgAddTime( m_uiContent, recvValue.m_optime )
	end
	m_LastTime = recvValue.m_optime;
	
	-- 创建一条聊天
	if recvValue.m_actorid > 0 then
		ChatDlgAddMsg( m_uiContent, recvValue );
	elseif recvValue.m_actorid == -1 then
		ChatDlgAddSysTalk( m_uiContent, recvValue );
	end
	ChatDlgScrollToBottom( m_uiScrollView )
end

-- 频道选择
function ChatDlgSelectChannel( channel )
	if m_SelectChannel == channel then
		return
	end
	m_SelectChannel = channel;	
	
	if m_SelectChannel == 0 then
		local objs={}
		for i = 0 ,m_uiContentZone.transform.childCount - 1 do
			table.insert( objs, m_uiContentZone.transform:GetChild(i).gameObject )
		end
		for k, v in pairs(objs) do
			local obj = v;
			if obj.name == "UIP_ChatItem(Clone)" then
				m_ObjectPool:Release( "UIP_ChatItem", obj );
			elseif obj.name == "UIP_ChatItemSelf(Clone)" then
				m_ObjectPool:Release( "UIP_ChatItemSelf", obj );
			elseif obj.name == "UIP_ChatSystalk(Clone)" then
				m_ObjectPool:Release( "UIP_ChatSystalk", obj );
			elseif obj.name == "UIP_ChatTime(Clone)" then
				m_ObjectPool:Release( "UIP_ChatTime", obj );
			end
		end
		ChatDlgScrollToBottom( m_uiScrollView )
	elseif m_SelectChannel == 1 then
		local lastTime = 0
		for i= 1, #m_ChatCache, 1 do		
			-- 超过10分钟添加时间
			if GetPlayer().m_zone == m_ChatCache[i].m_zone then
				if m_ChatCache[i].m_optime - lastTime > 600 then
					ChatDlgAddTime( m_uiContentZone, m_ChatCache[i].m_optime )
				end
				lastTime = m_ChatCache[i].m_optime;
			end
	
			-- 创建一条聊天
			if m_ChatCache[i].m_actorid > 0 then
				if GetPlayer().m_zone == m_ChatCache[i].m_zone then
					ChatDlgAddMsg( m_uiContentZone, m_ChatCache[i] );
				end
			elseif m_ChatCache[i].m_actorid == -1 then
				ChatDlgAddSysTalk( m_uiContentZone, m_ChatCache[i] );
			end
		end
		Invoke( function()
			ChatDlgScrollToBottom( m_uiScrollViewZone )
		end, 1/30 )
	end
end

-- 喇叭页
function ChatDlgHornLayerShow()
	SetTrue( m_uiHornLayer )
	m_horn_itemnum = GetItem():GetCount(171);
	SetImage( m_uiItem.transform:Find("Shape"), ItemSprite(171) )
	SetText( m_uiItem.transform:Find("Num"), "x"..m_horn_itemnum );
	if m_horn_itemnum == 0 then
		SetTrue( m_uiBuyButton )
		SetFalse( m_uiSendButton )
	else
		SetFalse( m_uiBuyButton )
		SetTrue( m_uiSendButton )
	end
end

function ChatDlgHornLayerHide()
	SetFalse( m_uiHornLayer )
end

function ChatDlgHornBuy()
	local cost = item_gettoken(171)
	MsgBox( F( 2063, cost ), function() 
		if GetPlayer().m_token < cost then
			JumpToken()
		end
		ChatDlgHornSend()
	end )
end

function ChatDlgHornSend()
	ChatDlgSendHorn();
	ChatDlgHornLayerHide()
end

-- 点击分享的邮件
function ChatDlgOnLinkClickMail( str )
	local sendValue = {};
	sendValue.m_op = 8;
	sendValue.m_mailid = int64.new(str);
	if sendValue.m_mailid == nil then
		return
	end
	netsend_mailop_C( sendValue )
end

-- 点击坐标
function ChatDlgOnLinkClickPos( str )
	MailOnLinkClick(str)
end

-- 点击玩家
function ChatDlgClickActor( actorid, gameObject )
	m_clickActorid = actorid
	if GetPlayer().m_actorid == m_clickActorid then
		ActorSearchDlgShow( m_clickActorid )
	elseif GetPlayer():CheckBlacklist( m_clickActorid ) == true then
		ChatDlgOpLayerShow( gameObject, 1 )
	else
		ChatDlgOpLayerShow( gameObject, 0 )
	end
end

function ChatDlgOpLayerShow( gameObject, isblack )
	m_OpLayerShow = true
	SetTrue( m_uiOpLayer )
	m_uiOpLayer.transform:SetParent( gameObject.transform )
	m_uiOpLayer.transform.anchoredPosition = Vector2( gameObject.transform.anchoredPosition.x*2, -49 )
	m_uiOpLayer.transform:SetParent( m_Dlg.transform )
	
	if isblack == 1 then
		SetFalse( m_uiOpForbidBtn )
	else
		SetTrue( m_uiOpForbidBtn )
	end
end

function ChatDlgOpLayerHide()
	SetFalse( m_uiOpLayer )
	m_OpLayerShow = false
end

-- 操作-查看信息
function ChatDlgOpSearch()
	ChatDlgOpLayerHide()
	-- 自己
	if GetPlayer().m_actorid == m_clickActorid then
		ActorSearchDlgShow( m_clickActorid )
	else
		ActorSearchDlgShow( m_clickActorid )
	end
end

-- 操作-加为好友
function ChatDlgOpFriend()
	ChatDlgOpLayerHide()
	local sendValue = {}
	sendValue.m_op = 1;
	sendValue.m_target_actorid = m_clickActorid;
	sendValue.m_target_cityindex = -1;
	sendValue.m_target_namelen = 0;
	sendValue.m_target_name = "";
	netsend_friendop_C( sendValue )
end

-- 操作-屏蔽说话
function ChatDlgOpForbid()
	ChatDlgOpLayerHide()
	system_askinfo( ASKINFO_ACTOR, "", 4, m_clickActorid );
end

