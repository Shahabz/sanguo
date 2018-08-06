-- 界面
local m_Dlg = nil;
local m_uiScrollView = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiUIP_ChatItem = nil; --UnityEngine.GameObject
local m_uiUIP_ChatItemSelf = nil; --UnityEngine.GameObject
local m_uiUIP_ChatTime = nil; --UnityEngine.GameObject
local m_uiInput = nil; --UnityEngine.GameObject
local m_uiSendBtn = nil; --UnityEngine.GameObject
local m_uiWaiting = nil; --UnityEngine.GameObject
local m_optime = 0;


-- 打开界面
function BugReportDlgOpen()
	m_Dlg = eye.uiManager:Open( "BugReportDlg" );
end

-- 隐藏界面
function BugReportDlgClose()
	if m_Dlg == nil then
		return;
	end
	eye.uiManager:Close( "BugReportDlg" );
end

-- 删除界面
function BugReportDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
	m_optime = 0
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function BugReportDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            BugReportDlgClose();
			
		-- 发送
		elseif nControlID == 1 then
			BugReportDlgSend();
        end
		
	elseif nType == UI_EVENT_INPUTVALUECHANGED then
		if nControlID == 0 then
			local text = m_uiInput:GetComponent( "UIInputField" ).text;
			if text == "" then
				SetTrue( m_uiInput.transform:Find("Hint") )
			else
				SetFalse( m_uiInput.transform:Find("Hint") )
			end
		end
	end
end

-- 载入时调用
function BugReportDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;
	m_uiScrollView = objs[0];
	m_uiContent = objs[1];
	m_uiUIP_ChatItem = objs[2];
	m_uiUIP_ChatItemSelf = objs[3];
	m_uiUIP_ChatTime = objs[4];
	m_uiInput = objs[5];
	m_uiSendBtn = objs[6];
	m_uiWaiting = objs[7];
	
	-- 对象池
	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );
	m_ObjectPool:CreatePool("UIP_ChatItem", 5, 5, m_uiUIP_ChatItem);
	m_ObjectPool:CreatePool("UIP_ChatItemSelf", 1, 1, m_uiUIP_ChatItemSelf);
	m_ObjectPool:CreatePool("UIP_ChatTime", 1, 1, m_uiUIP_ChatTime);
end

-- 界面初始化时调用
function BugReportDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function BugReportDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function BugReportDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function BugReportDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
local m_nFrame = 0;
function BugReportDlgOnLogic( gameObject )
	if m_nFrame > 150 then
		m_nFrame = 0;
		BugReportDlgGet()
	end
	m_nFrame = m_nFrame + 1;
end


----------------------------------------
-- 自定
----------------------------------------
function BugReportDlgShow()
	BugReportDlgOpen()
	SetTrue( m_uiWaiting )
	BugReportDlgGet()
end

function BugReportDlgGet()
	local paramString = "c=report&m=getlist"
	.."&serverid="..Const.serverid
	.."&actorid="..Const.actorid
	.."&optime="..m_optime

	HttpRequest.Get( paramString, function( response )
		SetFalse( m_uiWaiting )
		--print( response )
		
		local json = require "cjson"
		local info = json.decode( response );
		if info == nil then
			return;
		end
		local list = info["list"];
		for i=1, #list, 1 do
			local actorlevel = tonumber(list[i]["actorlevel"]);
			local msg = list[i]["msg"];
			local optime = tonumber(list[i]["optime"]);
			BugReportDlgAddMsg( m_uiContent, actorlevel, msg, optime )
		end
		m_uiScrollView:GetComponent("UIScrollRect"):ScrollToBottom();
	end );
end

-- 发送消息
function BugReportDlgSend()
	local msg = m_uiInput:GetComponent( "UIInputField" ).text;
	if msg == "" then
		return;
	end
	if string.len(msg) >= 1024 then
		return;
	end
	m_uiInput:GetComponent("UIInputField").text = "";
	
	SetTrue( m_uiWaiting )
	local paramString = "c=report&m=question"
	.."&serverid="..Const.serverid
	.."&platid="..Const.platid
	.."&actorid="..Const.actorid
	.."&actorname="..WWW.EscapeURL(GetPlayer().m_name)
	.."&actorlevel="..GetPlayer().m_level
	.."&nation="..GetPlayer().m_nation
	.."&vip="..GetPlayer().m_viplevel
	.."&posx="..GetPlayer().m_posx
	.."&posy="..GetPlayer().m_posy
	.."&msg="..WWW.EscapeURL(msg)
	.."&version="..Application.version.."."..Global.GetValue("RESOURCE_VERSION")

	HttpRequest.Get( paramString, function( response )
		--print( response )
		SetFalse( m_uiWaiting )
		local json = require "cjson"
		local info = json.decode( response );
		if info == nil then
			return;
		end
		local actorlevel = tonumber(info["actorlevel"]);
		local msg = info["msg"];
		local optime = tonumber(info["optime"]);
		BugReportDlgAddMsg( m_uiContent, actorlevel, msg, optime )
		m_uiScrollView:GetComponent("UIScrollRect"):ScrollToBottom();
	end );
end

function BugReportDlgAddMsg( uiRoot, actorlevel, msg, optime )
	local uiObj = nil;
	if actorlevel > 0 then
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
	
	if actorlevel > 0 then
		SetImage( uiShape, PlayerHeadSprite( GetPlayer().m_shape ) );
		SetRichText( uiName, GetPlayer().m_name );
	else
		SetImage( uiShape, LoadSprite("playerhead_normal") );
		SetRichText( uiName, T(303) );
	end
	SetRichText( uiText, msg, nil );

	uiObj.transform:SetParent( uiRoot.transform );
	uiObj.transform.localScale = Vector3.one;
	uiObj.gameObject:SetActive( true );	
	
	if optime > m_optime then
		m_optime = optime;
	end 
end