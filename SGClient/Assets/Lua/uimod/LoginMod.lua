-- 界面
local m_Dlg = nil;
local m_uiVersion = nil; --UnityEngine.GameObject
local m_uiAccountEdit = nil; --UnityEngine.GameObject
local m_uiPasswordEdit = nil; --UnityEngine.GameObject
local m_uiServerInfo = nil; --UnityEngine.GameObject
local m_uiServerList = nil; --UnityEngine.GameObject
local m_uiWarning = nil; --UnityEngine.GameObject
local m_uiLoading = nil; --UnityEngine.GameObject
local m_uiWaiting = nil; --UnityEngine.GameObject
local m_uiMsgBox = nil; --UnityEngine.GameObject
local m_uiGameEnter = nil; --UnityEngine.GameObject
local m_uiButtonList = nil; --UnityEngine.GameObject
local m_uiNoticeLayer = nil; --UnityEngine.GameObject
local m_uiNoticeText = nil; --UnityEngine.GameObject
local m_uiNoticeTogBtn = nil; --UnityEngine.GameObject
local m_uiGroupContent = nil; --UnityEngine.GameObject
local m_uiServerContent = nil; --UnityEngine.GameObject
local m_uiUIP_Group = nil; --UnityEngine.GameObject
local m_uiUIP_Server = nil; --UnityEngine.GameObject
local m_uiUsedGroup = nil; --UnityEngine.GameObject
local m_uiNewGroup = nil; --UnityEngine.GameObject
local m_uiLoginLayer = nil; --UnityEngine.GameObject
local m_uiRegLayer = nil; --UnityEngine.GameObject
local m_uiLoginType = nil; --UnityEngine.GameObject
local m_uiRegUserEdit = nil; --UnityEngine.GameObject
local m_uiRegPwdEdit = nil; --UnityEngine.GameObject
local m_uiRegPwdReEdit = nil; --UnityEngine.GameObject

local m_uiSelectGroup = nil -- 选择的服务器分组
local m_uiGroupCache = {} -- 服务器组对象缓存
local m_nSelectGroupID = 0 -- 当前选择的服务器组

local GROUPLIST_CONTROLID = 1000
local SERVERLIST_CONTROLID = 10000

-- 服务器分组列表
local m_GroupList = nil;
-- 所有服务器列表
local m_ServerList = nil;
-- 常用服务器列表
local m_LoginUsedServerList = nil;

-- 用户名密码
local m_UserName = "";
local m_PassWord = "";

local m_MsgBoxCallBack = nil

-- 打开界面
function LoginModOpen()
	if m_Dlg == nil then
		m_Dlg = eye.uiManager:GetLayer( 3 ):Find( "LoginMod" ).gameObject;
	end
	m_Dlg:SetActive( true );
end

-- 隐藏界面
function LoginModClose()
	if m_Dlg == nil then
		return;
	end
	m_Dlg:SetActive( false );
    LoginModDestroy();
end

-- 删除界面
function LoginModDestroy()
	GameObject.Destroy( m_Dlg );
	ResourceManager.UnloadAssetBundleImmediately( "launchload" )
	m_Dlg = nil;
	m_objectPoolServer = {};
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function LoginModOnEvent( nType, nControlID, value )
	if nType == UI_EVENT_CLICK then
			
		--关闭服务器列表
		if nControlID == -1  then
		    LoginModCloseServerList();
		
		-- 关闭公告	
		elseif nControlID == -2 then
			SetFalse( m_uiNoticeLayer )
			
		elseif nControlID == -3 then
			SetFalse( m_uiLoginLayer )
			
		elseif nControlID == -4 then
			SetFalse( m_uiRegLayer )
		
		-- 快速试玩
		elseif nControlID == 1 then
			LoginModQuickLogin();
				
		-- 打开账户登陆页
		elseif nControlID == 2 then
			LoginModLoginLayer()
			
		-- 关闭每组的小列表 返回大列表
		elseif nControlID == 3 then
			LoginModCloseGroup();
			
		-- 切换账号
		elseif nControlID == 4 then
			if Const.platid == 18 or Const.platid == 19 then
				LoginModLoginLayer()
			elseif Const.platid > 11 then
				SDK.logout()
			else
				LoginModLoginLayer()
			end
			
		-- 游戏公告
		elseif nControlID == 5 then
			 LoginModNoticeShow(0)
			
		--服务器列表
		elseif nControlID == 6 then
			if IsActive(m_uiServerList)	then
				LoginModCloseServerList();
			else
				LoginModAskServerListRe()
			end
			
		-- 联系客服
		elseif nControlID == 7 then
			SDK.gmbug()
		
		-- 进入游戏
		elseif nControlID == 9 then	
			LoginModEnterGame()
			
		-- 显示服务器列表
		elseif nControlID == 10 then
			LoginModOpenServerList();
		
		-- 账户登陆	
		elseif nControlID == 11 then
			LoginModLogin();
		
		-- 打开账户注册页
		elseif nControlID == 12 then
			LoginModRegLayer();
		
		-- 账户注册
		elseif nControlID == 13 then
			LoginModReg();
		
		-- MsgBox回调
		elseif nControlID == 14 then
			if m_MsgBoxCallBack then
				m_MsgBoxCallBack()
			end
			
		-- 点击推荐分组	
		elseif nControlID == 1000 then	
			LoginModSelectGroup( nControlID );
			
		-- 选择组
		elseif nControlID > GROUPLIST_CONTROLID and nControlID < SERVERLIST_CONTROLID then
		    LoginModSelectGroup( nControlID - GROUPLIST_CONTROLID )
			
		-- 选择服务器
		elseif nControlID > SERVERLIST_CONTROLID and nControlID < SERVERLIST_CONTROLID*2 then
			LoginModSelectServer( nControlID - SERVERLIST_CONTROLID );
		end
		
	elseif nType == UI_EVENT_TWEENFINISH then
		if nControlID == 1 then
			m_uiWarning.gameObject:SetActive( false );
			
		elseif nControlID == 2 then
		    --m_selectObj.transform:SetParent( m_selectTransform.transform, false );
			LoginModGroupAddServer( m_nSelectGroupID );
			
		end
		
	elseif nType == UI_EVENT_TOGGLEVALUECHANGE then
		if nControlID == 1 then
			if value == true then
				GameManager.writeini( "NOTICE_TODAYSHOW", 1 )
			else
				GameManager.writeini( "NOTICE_TODAYSHOW", 0 )
			end
			
		end
		
	elseif nType == UI_EVENT_INPUTVALUECHANGED then
		if nControlID == 1 then
			local text = m_uiAccountEdit.transform:Find("Input"):GetComponent( "UIInputField" ).text;
			if text == "" then
				SetTrue( m_uiAccountEdit.transform:Find("Input/Hint") )
			else
				SetFalse( m_uiAccountEdit.transform:Find("Input/Hint") )
			end
		elseif nControlID == 2 then
			local text = m_uiPasswordEdit.transform:Find("Input"):GetComponent( "UIInputField" ).text;
			if text == "" then
				SetTrue( m_uiPasswordEdit.transform:Find("Input/Hint") )
			else
				SetFalse( m_uiPasswordEdit.transform:Find("Input/Hint") )
			end
			
		elseif nControlID == 3 then
			local text = m_uiRegUserEdit.transform:Find("Input"):GetComponent( "UIInputField" ).text;
			if text == "" then
				SetTrue( m_uiRegUserEdit.transform:Find("Input/Hint") )
			else
				SetFalse( m_uiRegUserEdit.transform:Find("Input/Hint") )
			end
			
		elseif nControlID == 4 then
			local text = m_uiRegPwdEdit.transform:Find("Input"):GetComponent( "UIInputField" ).text;
			if text == "" then
				SetTrue( m_uiRegPwdEdit.transform:Find("Input/Hint") )
			else
				SetFalse( m_uiRegPwdEdit.transform:Find("Input/Hint") )
			end
			
		elseif nControlID == 5 then
			local text = m_uiRegPwdReEdit.transform:Find("Input"):GetComponent( "UIInputField" ).text;
			if text == "" then
				SetTrue( m_uiRegPwdReEdit.transform:Find("Input/Hint") )
			else
				SetFalse( m_uiRegPwdReEdit.transform:Find("Input/Hint") )
			end
		end
	end
end

-- 载入时调用
function LoginModOnAwake( gameObject )
    m_Dlg = gameObject;
	local objs = gameObject:GetComponent( "UIMod" ).relatedGameObject;
	m_uiVersion = objs[0];
	m_uiAccountEdit = objs[1];
	m_uiPasswordEdit = objs[2];
	m_uiServerInfo = objs[3];
	m_uiServerList = objs[4];
	m_uiWarning = objs[5];
	m_uiLoading = objs[6];
	m_uiWaiting = objs[7];
	m_uiMsgBox = objs[8];
	m_uiGameEnter = objs[9];
	m_uiButtonList = objs[10];
	m_uiNoticeLayer = objs[11];
	m_uiNoticeText = objs[12];
	m_uiNoticeTogBtn = objs[13];
	m_uiGroupContent = objs[14];
	m_uiServerContent = objs[15];
	m_uiUIP_Group = objs[16];
	m_uiUIP_Server = objs[17];
	m_uiUsedGroup = objs[18];
	m_uiNewGroup = objs[19];
	m_uiLoginLayer = objs[20];
	m_uiRegLayer = objs[21];
	m_uiLoginType = objs[22];
	m_uiRegUserEdit = objs[23];
	m_uiRegPwdEdit = objs[24];
	m_uiRegPwdReEdit = objs[25];

	-- 对象池
	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );
	m_ObjectPool:CreatePool("UIP_Group", 3, 3, m_uiUIP_Group);
	m_ObjectPool:CreatePool("UIP_Server", 3, 3, m_uiUIP_Server);
end

-- 界面初始化时调用
function LoginModOnStart()
	eye.audioManager:Play(201);
	
	-- 版本号
	m_uiVersion:GetComponent( typeof(UIText) ).text = "v "..Application.version.."("..Global.GetValue("RESOURCE_VERSION")..")"--[[.."lang："..DeviceHelper.getLanguage().."-"..DeviceHelper.getCountry()--]];
	-- 平台
	if Const.platid == 18 or Const.platid == 19 then
		LoginModOpenTestLogin();
	elseif Const.platid > 11 then
		SDK.init()
		LoginModOpenSDKLogin();
	else
		LoginModOpenTestLogin();
	end
end

-- 界面显示时调用
function LoginModOnEnable()
	
end

-- 界面隐藏时调用
function LoginModOnDisable()
	
end

-- 界面删除时调用
function LoginModOnDestroy()
	m_Dlg = nil;
end

-- 每帧调用
function LoginModOnLogic()
	
end


----------------------------------------
-- 自定
----------------------------------------
-- 打开测试模式登陆
function LoginModOpenTestLogin()
	if m_Dlg == nil then
		return;
	end
	-- 需要隐藏的
	LoginModCloseTestLogin()
	
	-- 读取上次登陆过的账户密码
	local username = GameManager.ini( "USERNAME", "" );
	local pwd = GameManager.ini( "PASSTOKEN", "" );
	m_uiAccountEdit.transform:Find("Input"):GetComponent( "UIInputField" ).text = username
	m_uiPasswordEdit.transform:Find("Input"):GetComponent( "UIInputField" ).text = pwd
	if username == "" then
		SetTrue( m_uiAccountEdit.transform:Find("Input/Hint") )
	else
		SetFalse( m_uiAccountEdit.transform:Find("Input/Hint") )
	end
	if pwd == "" then
		SetTrue( m_uiPasswordEdit.transform:Find("Input/Hint") )
	else
		SetFalse( m_uiPasswordEdit.transform:Find("Input/Hint") )
	end
			
	-- 账户
	local loginType = GameManager.ini( "LASTLOGINTYPE", 0 );
	m_uiLoginType.gameObject:SetActive( true );
	if loginType == "2" then
		SetText( m_uiLoginType, T(414)..":"..T(435) )
	else
		SetText( m_uiLoginType, T(414)..":"..GameManager.ini( "USERNAME", "" ) )
	end

	-- 请求服务器列表	
	LoginModAskServerList()
end

-- 关闭测试模式登陆
function LoginModCloseTestLogin()
	if m_Dlg == nil then
		return;
	end
	-- 需要隐藏的
	m_uiLoginType.gameObject:SetActive( false );
	m_uiLoginLayer.gameObject:SetActive( false );
	m_uiRegLayer.gameObject:SetActive( false );
	m_uiServerInfo.gameObject:SetActive( false );
	m_uiButtonList.gameObject:SetActive( false );
	m_uiGameEnter.gameObject:SetActive( false );
end

-- 测试模式登陆页
function LoginModLoginLayer()
	m_uiLoginLayer.gameObject:SetActive( true );
	m_uiRegLayer.gameObject:SetActive( false );
	-- 读取上次登陆过的账户密码
	m_uiAccountEdit.transform:Find("Input"):GetComponent( "UIInputField" ).text = GameManager.ini( "USERNAME", "" );
	m_uiPasswordEdit.transform:Find("Input"):GetComponent( "UIInputField" ).text = GameManager.ini( "PASSTOKEN", "" );
end

-- 测试模式注册页
function LoginModRegLayer()
	m_uiRegLayer.gameObject:SetActive( true );
	m_uiLoginLayer.gameObject:SetActive( false );
end

-- 注册
function LoginModReg()
	local userName = m_uiRegUserEdit.transform:Find("Input"):GetComponent( "UIInputField" ).text
	local passWord = m_uiRegPwdEdit.transform:Find("Input"):GetComponent( "UIInputField" ).text
	local passWordRe = m_uiRegPwdReEdit.transform:Find("Input"):GetComponent( "UIInputField" ).text
	-- 非法检查
	local len = string.len( userName );
	if len < 4 or len > 64 then
		LoginModWarning( T(417) );
		return
	end
	if string.checksign( userName ) == false then
		LoginModWarning( T(417) );
		return
	end
	
	local pwdlen = string.len( passWord );
	if passWord == "" or pwdlen < 4 or pwdlen > 64 then
		LoginModWarning( T(419) );
		return
	end
	
	if passWord ~= passWordRe then
		LoginModWarning( T(416) );
		return
	end
	
	LoginModWaitOpen()
	HttpRequest.RegisterUser( userName, passWord, function( response ) 
		LoginModWaitClose()
		local json = require "cjson"
		local info = json.decode( response );
		if info == nil then
			netlog( response );
			LoginModWarning( T(418) )
			return;
		end
		if info["result"] == -1 then
			LoginModWarning( T(418) )
			return
		end
		if info["result"] == -2 then
			LoginModWarning( T(417) )
			return
		end
		if info["result"] == -3 then
			LoginModWarning( T(419) )
			return
		end
		LoginModWarning( T(447) );
		m_uiAccountEdit.transform:Find("Input"):GetComponent( "UIInputField" ).text = info["u"];
		m_uiPasswordEdit.transform:Find("Input"):GetComponent( "UIInputField" ).text = info["p"];
		LoginModLogin()
	end )
end

-- 进入游戏
function LoginModEnterGame()
	if Const.platid == 18 or Const.platid == 19 or Const.platid <= 11 then
		local loginType = GameManager.ini( "LASTLOGINTYPE", 0 );
		if loginType == "2" then
			LoginModQuickLogin();
		else
			local userName = m_uiAccountEdit.transform:Find("Input"):GetComponent( "UIInputField" ).text;
			local passWord = m_uiPasswordEdit.transform:Find("Input"):GetComponent( "UIInputField" ).text;
			if userName == "" or passWord == "" then
				LoginModLoginLayer()
			else
				LoginModLogin()
			end
		end
	else
		LoginModSDKLogin()
	end
end

-- 显示SDK模式登陆
function LoginModOpenSDKLogin()
	if m_Dlg == nil then
		return;
	end
	-- 需要隐藏的
	LoginModCloseTestLogin()
	
	-- 请求服务器列表
	LoginModAskServerList()
end

-- 关闭SDK模式登陆
function LoginModCloseSDKLogin()
	if m_Dlg == nil then
		return;
	end
	-- 需要隐藏的
	LoginModCloseTestLogin()
end

-- 请求服务器列表
function LoginModAskServerList()
	-- 获取服务器列表
	LoginModWaitOpen()
	HttpRequest.GetServerList( function( response )
		LoginModWaitClose()
		local json = require "cjson"
		local info = json.decode( response );
		if info == nil then
			netlog( response );
			LoginModMsgBox( T(413), LoginModAskServerList )
			return;
		end

		-- 服务器分组
		m_GroupList = info["grouplist"];
		-- 所有服务器
		m_ServerList = info["serverlist"];
		
		-- 玩家登陆过的服务器列表
		local loginUsedServerJson = GameManager.ini("LOGIN_USED_SERVER_LIST", "");
		if loginUsedServerJson == "" then
			-- 没有登陆过的服务器就选择默认推荐的新服
			m_LoginUsedServerList = nil;
			local newserver = LoginModGetNewServer();
			if newserver == nil then
				LoginModOpenServerList(); 
			else
				LoginModSelectServer( newserver["sid"] );	
			end
		else
			--有登陆过就选择最后一次登陆的服务器
			m_LoginUsedServerList = json.decode( loginUsedServerJson )
			LoginModSelectServer( m_LoginUsedServerList[#m_LoginUsedServerList]["sid"] );
		end 
			
		-- 需要显示的
		m_uiServerInfo.gameObject:SetActive( true );
		m_uiButtonList.gameObject:SetActive( true );
		m_uiGameEnter.gameObject:SetActive( true );

		-- 显示公告
		LoginModNoticeShow(1)
	end );
end

-- 再次请求服务器列表
function LoginModAskServerListRe()
	-- 获取服务器列表
	LoginModWaitOpen()
	HttpRequest.GetServerList( function( response )
	LoginModWaitClose()
		local json = require "cjson"
		local info = json.decode( response );
		if info == nil then
			netlog( response );
			LoginModMsgBox( T(413), LoginModAskServerListRe )
			return;
		end
		-- 服务器分组
		m_GroupList = info["grouplist"];
		-- 所有服务器
		m_ServerList = info["serverlist"];	
		LoginModOpenServerList(); 
	end );
end

-- 打开服务器列表界面
function LoginModOpenServerList()
	m_uiGameEnter.gameObject:SetActive( false );
	m_uiServerInfo.gameObject:SetActive( false );
	m_uiServerList.gameObject:SetActive( true );
	m_uiGroupCache[1000] = m_uiNewGroup
	LoginModGroupClear( m_uiGroupContent )
	LoginModServerClear( m_uiGroupContent )
	
	-- 默认添加常用服务器
	if m_LoginUsedServerList ~= nil then
		local index = 1;
		for k, v in pairs( m_LoginUsedServerList ) do
			local serverinfo = LoginModGetServerInfo( v["sid"] );
			if serverinfo ~= nil then
				LoginModAddServer( serverinfo, m_uiGroupContent, index );
				index = index + 1;
			end
		end
	end
	
	-- 创建服务器分组
	for k, v in pairs( m_GroupList ) do
		LoginModAddGroup( v )
	end 
end

-- 关闭服务器列表界面
function LoginModCloseServerList()
	-- 关闭选择的分组
	if m_uiSelectGroup and IsActive( m_uiSelectGroup ) == true then
		LoginModCloseGroup();
	end
	m_uiGroupCache = {}
	
	m_uiGameEnter.gameObject:SetActive( true );
	m_uiServerInfo.gameObject:SetActive( true );
	m_uiServerList.gameObject:SetActive( false );
end

-- 释放服务器组对象
function LoginModGroupClear( uiRoot )
	local objs = {};
	for i=0, uiRoot.transform.childCount-1 do
		table.insert(objs,uiRoot.transform:GetChild(i).gameObject);
	end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "UIP_Group(Clone)" then
			m_ObjectPool:Release( "UIP_Group", obj );
		end
	end
end

-- 释放服务器对象
function LoginModServerClear( uiRoot )
	local objs = {};
	for i=0, uiRoot.transform.childCount-1 do
		table.insert(objs,uiRoot.transform:GetChild(i).gameObject);
	end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "UIP_Server(Clone)" then
			m_ObjectPool:Release( "UIP_Server", obj );
		end
	end
end

-- 添加服务器分组
function LoginModAddGroup( group )
	local groupid = tonumber(group["g"])
	local groupname = group["n"]
	
	local uiObj = m_ObjectPool:Get("UIP_Group");	
	uiObj.transform:SetParent( m_uiGroupContent.transform );
	uiObj.transform.localScale = Vector3.one;	

	SetTrue( uiObj )
	SetControlID( uiObj, GROUPLIST_CONTROLID + groupid )
	SetText( uiObj.transform:Find("Back/Name"), groupname )
	m_uiGroupCache[groupid] = uiObj;
end

-- 添加服务器列表
function LoginModAddServer( serverinfo, parent, siblingIndex )
	local serverid = serverinfo["sid"]
	local servername = serverinfo["n"]
	
	local uiObj = m_ObjectPool:Get("UIP_Server");	
	uiObj.transform:SetParent( parent.transform );
	uiObj.transform.localScale = Vector3.one;
		
	SetTrue( uiObj )
	SetControlID( uiObj, SERVERLIST_CONTROLID + serverid )
	SetText( uiObj.transform:Find("Back/Name"), LoginModStateText( serverinfo["new"], serverinfo["cl"] ) .. servername )
	
	if Const.serverid == serverid then
		SetTrue( uiObj.transform:Find("Back/Select") )
	else
		SetFalse( uiObj.transform:Find("Back/Select") )
	end
	if siblingIndex ~= nil then
		uiObj.transform:SetSiblingIndex( siblingIndex );
	end
end

-- 选择一个分组
function LoginModSelectGroup( groupid )
	m_nSelectGroupID = groupid;
	local uiOriginalObj = m_uiGroupCache[groupid];
	if uiOriginalObj == nil then
		return
	end
	
	m_uiSelectGroup = m_ObjectPool:Get("UIP_Group");	
	m_uiSelectGroup.transform:SetParent( m_uiServerContent.transform, false );
	m_uiSelectGroup.transform.localScale = Vector3.one;
	
	SetFalse( m_uiGroupContent );
	SetTrue( m_uiServerContent );
	
	SetControlID( m_uiSelectGroup, 3 );
	SetText( m_uiSelectGroup.transform:Find("Back/Name"), uiOriginalObj.transform:Find("Back/Name"):GetComponent("UIText").text )
	
	m_uiSelectGroup.transform:GetComponent( "UITweenRectPosition" ).from = Vector2( 224, uiOriginalObj.transform.localPosition.y )
	m_uiSelectGroup.transform:GetComponent( "UITweenRectPosition" ).to = Vector2( 224, -8 ); 
	m_uiSelectGroup.transform:GetComponent( "UITweenRectPosition" ):Play( true );
end

-- 往选择的分组里添加服务器
function LoginModGroupAddServer( groupid )
	-- 推荐组
	if groupid == 1000 then
		local recommendList = {}
		for k,v in pairs( m_ServerList ) do
			if math.floor( tonumber( v["new"] )/10 ) == 1 then
				table.insert( recommendList, m_ServerList[k] )
			end
		end
		for  i=1,#recommendList  do
			LoginModAddServer( recommendList[i], m_uiServerContent );
		end
		
	-- 其他组
	else
		for k,v in pairs( m_ServerList ) do
			if tonumber(v["g"]) == groupid then
				LoginModAddServer( v, m_uiServerContent );
			end
		end
	end
end

-- 关闭选择的服务器分组
function LoginModCloseGroup()
	SetTrue( m_uiGroupContent );
	SetFalse( m_uiServerContent );
	LoginModGroupClear( m_uiServerContent )
	LoginModServerClear( m_uiServerContent )

	m_uiSelectGroup = nil
	m_nSelectGroupID = 0
end

-- 设置当前已经选择的服务器
function LoginModSelectServer( id )
	local serverinfo = LoginModGetServerInfo( id )
	if serverinfo == nil then
		serverinfo= LoginModGetNewServer();
		id = serverinfo["sid"]
		LoginModSelectServer( id );	
		return;
	end
	
	-- 系统维护中
	if tonumber(serverinfo["cl"]) == 1 then
		LoginModWarning( T(411) );
		return
	end
	
	Const.servername = serverinfo["n"];
	Const.serverid = serverinfo["sid"];
	
	-- 服务器名称
	SetText( m_uiServerInfo.transform:Find("SeverName"), serverinfo["n"] )
	
	-- 服务器状态
	SetText( m_uiServerInfo.transform:Find("StateName"), LoginModStateText( serverinfo["new"],serverinfo["cl"] ) );
	local newR = tonumber(serverinfo["new"]) % 10
	if newR == 0 or newR == 1 then
		newR = 2
	end	
	SetImage( m_uiServerInfo.transform:Find("StateIcon"), LoadSprite( "ui_login_serverstate_"..newR ) );
	
	-- 关闭服务器列表
	LoginModCloseServerList()
end

-- 获取服务器状态文字
function LoginModStateText( server_new, server_close )
	local srvname = ""
	local close = tonumber( server_close );
	local newR = tonumber( server_new ) % 10 ;
	if close == 1 then
		srvname = T(433);
	else
		if newR == 2 then
			srvname = T(428);
		elseif newR == 3 then
			srvname = T(429);
		elseif newR == 4 then 
			srvname = T(430);
		elseif newR == 5 then 
			srvname = T(431);		
		elseif newR == 6 then 
			srvname = T(432);	
		end
	end
	return srvname
end

-- 获取推荐的新服务器
function LoginModGetNewServer()
	local serverinfo = nil;
	for k, v in pairs(m_ServerList) do
		local close = tonumber( v.cl );
		local newR = tonumber( v.new ) % 10 ;
		if close == 0 and newR == 2 then
			serverinfo = v;
			break;
		end
	end
	return serverinfo;
end

-- 获取服务器信息
function LoginModGetServerInfo( sid )
	local serverinfo = nil;
	local sid = tonumber(sid);
	for k, v in pairs(m_ServerList) do
		if tonumber(v.sid) == sid then
			serverinfo = v;
			break;
		end
	end
	return serverinfo;
end

-- 保存常用服务器信息
function LoginModUsedServerSave()
	local json_used = require "cjson"
	
	-- 第一次保存
	if m_LoginUsedServerList == nil then
		m_LoginUsedServerList = {};
		table.insert( m_LoginUsedServerList, { sid=Const.serverid } )
	else
		-- 先移除刚才选的服务器
		for k, v in pairs( m_LoginUsedServerList ) do 
			if v["sid"] == Const.serverid then
				table.remove( m_LoginUsedServerList, k )
			end
		end
		
		if #m_LoginUsedServerList > 5 then
			table.remove( m_LoginUsedServerList, 1 )
			table.insert( m_LoginUsedServerList, { sid=Const.serverid } )
		else
			table.insert( m_LoginUsedServerList, { sid=Const.serverid } )
		end
	end
	
	local jsonMsg = json_used.encode( m_LoginUsedServerList )
	GameManager.writeini( "LOGIN_USED_SERVER_LIST", jsonMsg )
end

-- 打开进度条
function LoginModOpenLoading()
	if m_Dlg == nil then
		return;
	end
	LoginModLoginQueue( false );
	
	-- 需要隐藏的
	m_uiLoginLayer.gameObject:SetActive( false );
	m_uiServerInfo.gameObject:SetActive( false );
	m_uiGameEnter.gameObject:SetActive( false );
	m_uiButtonList.gameObject:SetActive( false );
	
	-- 需要显示的
	m_uiLoading.gameObject:SetActive( true );
	m_uiLoading.transform:GetComponent( "Slider" ).value = 0.0;
	
    -- 读取所有AssetBundle
    eye.resourceManager:LoadAllAssetBundle();
end

-- 读取AssetBundle的回调
function LoginModOnLoadAssetBundleProc( assetBundleName, progress, totalProgress )
	m_uiLoading.transform:GetComponent( "Slider" ).value = progress/totalProgress;
	gamelog( "LoadAllAssetBundle:"..assetBundleName.."("..progress.."/"..totalProgress..")" )
	if progress == totalProgress then
		-- 资源初始化类放在这
		-- 初始化提示文字对象池
		PopTextModPoolInit();
		NotifyModPoolInit();
		MapZoneTipsModPoolInit();
		DialogFrameModPoolInit();
		netsend_list_C( 0 );
	end
end

-- 快速登陆
function LoginModQuickLogin()
	LoginModCloseTestLogin();
	LoginModLoginQueue( true );
	m_UserName = "";
	m_PassWord = "";
	GameManager.writeini( "LASTSERVERID", Const.serverid );
	GameManager.writeini( "LASTLOGINTYPE", 2 );	
	local serverinfo = LoginModGetServerInfo( Const.serverid )
	if serverinfo == nil then
		AlertMsg( T(411) )
		LoginModOpenTestLogin()
		return
	end
	Network.SDKConnectServer(serverinfo["h"], serverinfo["p"] );
	LoginModUsedServerSave()
end

-- 用户密码登陆
function LoginModLogin()
	LoginModCloseTestLogin();
	LoginModLoginQueue( true );
	m_UserName = m_uiAccountEdit.transform:Find("Input"):GetComponent( "UIInputField" ).text;
	m_PassWord = m_uiPasswordEdit.transform:Find("Input"):GetComponent( "UIInputField" ).text;
	GameManager.writeini( "LASTSERVERID", Const.serverid );
	GameManager.writeini( "LASTLOGINTYPE", 1 );	
	local serverinfo = LoginModGetServerInfo( Const.serverid )
	if serverinfo == nil then
		AlertMsg( T(411) )
		LoginModOpenTestLogin()
		return
	end
	Network.SDKConnectServer( serverinfo["h"], serverinfo["p"] );
	LoginModUsedServerSave()
end

-- SDK登陆
function LoginModSDKLogin()
	if Const.sdk_uid == "" then
		SDK.login()
		return
	end
	
	LoginModCloseSDKLogin();
	LoginModLoginQueue( true );
	
	GameManager.writeini( "LASTSERVERID", Const.serverid );
	GameManager.writeini( "LASTLOGINTYPE", 3 );	
	local serverinfo = LoginModGetServerInfo( Const.serverid )
	if serverinfo == nil then
		AlertMsg( T(411) )
		LoginModOpenSDKLogin()
		return
	end
	Network.SDKConnectServer( serverinfo["h"], serverinfo["p"] );
	LoginModUsedServerSave()
end

-- 测试模式登陆游戏
function LoginModTestLoginProc()
	gamelog("LoginModTestLoginProc");
		
	-- 登陆
	local sendValue = {};
	sendValue.m_username = m_UserName;
	sendValue.m_username_length = string.len( sendValue.m_username );
	
	sendValue.m_password = m_PassWord;
	sendValue.m_password_length = string.len( sendValue.m_password );
	
	sendValue.m_deviceid = Utils.deviceUniqueIdentifier;
	sendValue.m_deviceid_length = string.len( sendValue.m_deviceid );
	sendValue.m_isnew = 0;
	
	sendValue.m_ipcountry = g_ipCountryStr;
	sendValue.m_ipcountry_length = string.len( sendValue.m_ipcountry );
	netsend_login_C( sendValue )
end

-- SDK模式登陆游戏
function LoginModSDKLoginProc()
	gamelog("LoginModSDKLoginProc");
	
	-- 登陆
	local sendValue = {};
	sendValue.m_username = Const.sdk_uid
	sendValue.m_username_length = string.len( sendValue.m_username );
	
	sendValue.m_password = Const.sdk_token.."_"..Const.sdk_timestamp
	sendValue.m_password_length = string.len( sendValue.m_password );
	
	sendValue.m_deviceid = Utils.deviceUniqueIdentifier;
	sendValue.m_deviceid_length = string.len( sendValue.m_deviceid );
	
	sendValue.m_isnew = tonumber(Const.sdk_isnewuser);
	sendValue.m_ipcountry = g_ipCountryStr;
	sendValue.m_ipcountry_length = string.len( sendValue.m_ipcountry );
	netsend_login_C( sendValue );
end

-- 警告字符串
function LoginModWarning( text )
	m_uiWarning.transform:Find("Text"):GetComponent("UIText").text = text;
	m_uiWarning.gameObject:SetActive( true );
end

-- 等待框
function LoginModWaitOpen()
	SetTrue( m_uiWaiting )
end
function LoginModWaitClose()
	SetFalse( m_uiWaiting )
end

-- 提示框
function LoginModMsgBox( msg, callback )
	SetTrue( m_uiMsgBox )
	SetText( m_uiMsgBox.transform:Find("Back/Msg"), msg )
	m_MsgBoxCallBack = callback;
end

-- 登陆队列显示
local LoginQueueFrame = 0;
function LoginModLoginQueue( show )
--[[	m_LoginQueue.gameObject:SetActive( show );
	if show == true then
		LoginQueueFrame = 0;
		-- begin 修改进度条显示时候加的，主要是为了平时不显示登录中...，有排队再显示
		m_LoginQueue.gameObject:SetActive( false );
		-- end
		m_LoginQueue.transform:FindChild("Text"):GetComponent("UIText").text = T(410);
		Invoke( LoginModLoginQueueLogic, 0.5, 0, "LoginModLoginQueueLogic" );
	else
		InvokeStop( "LoginModLoginQueueLogic" );
	end--]] 
end
function LoginModLoginQueueLogic()
--[[	if m_Dlg == nil then
		return;
	end

    if m_LoginQueue == nil then
        return;
    end

	local QueueNumText = "";
	if Network.LoginUserNum > 0 then
		m_LoginQueue.gameObject:SetActive( true );
		QueueNumText = "("..Network.LoginUserNum..")"
	end
	if LoginQueueFrame == 0 then
		m_LoginQueue.transform:FindChild("Text"):GetComponent("UIText").text = T(410)..QueueNumText;
	elseif LoginQueueFrame == 1 then
		m_LoginQueue.transform:FindChild("Text"):GetComponent("UIText").text = T(410)..QueueNumText..".";
	elseif LoginQueueFrame == 2 then
		m_LoginQueue.transform:FindChild("Text"):GetComponent("UIText").text = T(410)..QueueNumText.."..";
	elseif LoginQueueFrame == 3 then
		m_LoginQueue.transform:FindChild("Text"):GetComponent("UIText").text = T(410)..QueueNumText.."...";
		LoginQueueFrame = 0;
	end
	LoginQueueFrame = LoginQueueFrame + 1;
	InvokeStop( "LoginModLoginQueueLogic" );
	Invoke( LoginModLoginQueueLogic, 0.5, 0, "LoginModLoginQueueLogic" );--]]
end

-- 显示公告
function LoginModNoticeShow( check )
	local onORoff = GameManager.ini( "NOTICE_TODAYSHOW", "" )
	if onORoff == "" then
		onORoff = 0;
		GameManager.writeini( "NOTICE_TODAYSHOW", 0 )
	end
	
	if tonumber(onORoff) == 1 then
		m_uiNoticeTogBtn.transform:GetComponent( "UIToggle" ).isOn = true
	else
		m_uiNoticeTogBtn.transform:GetComponent( "UIToggle" ).isOn = false
	end
	
	if check == 1 then
		local today = math.floor(os.time()/86400);
		local fday = GameManager.ini( "NOTICE_TODAY", "" )
		if fday == "" then
			GameManager.writeini( "NOTICE_TODAY", today )
		end
		
		-- 今日不在提示
		if tonumber(onORoff) == 1 and fday ~= "" and tonumber(fday) == tonumber(today) then
			return
		end
		if tonumber(fday) ~= tonumber(today) then
			GameManager.writeini( "NOTICE_TODAYSHOW", 0 )
			GameManager.writeini( "NOTICE_TODAY", today )
			m_uiNoticeTogBtn.transform:GetComponent( "UIToggle" ).isOn = false
		end
	end
	
	SetTrue( m_uiNoticeLayer )
	HttpRequest.GetFile( "notice.cfg", function( response )
		SetText(m_uiNoticeText, response)
	end )
end



	