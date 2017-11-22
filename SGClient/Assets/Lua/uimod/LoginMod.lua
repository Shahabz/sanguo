-- 界面
local m_Dlg = nil;
local m_uiVersion; --UnityEngine.GameObject
local m_uiAccountEdit; --UnityEngine.GameObject
local m_uiPasswordEdit; --UnityEngine.GameObject
local m_uiServerInfo; --UnityEngine.GameObject
local m_uiServerList; --UnityEngine.GameObject
local m_uiContent; --UnityEngine.GameObject
local m_uiServerName; --UnityEngine.GameObject
local m_uiWarning; --UnityEngine.GameObject
local m_uiLoading; --UnityEngine.GameObject
local m_uiFastEnter; --UnityEngine.GameObject
local m_uiRegEnter; --UnityEngine.GameObject
local m_uiArrayName;  --实例服务器组的GameObject
local m_uiServerContent
local m_servertab = nil;
local m_uiArrayObjs = {}
local m_uiServerObjs = {}
local m_uiRencommed;
local m_selectObj=nill --点击服务器数组列表时 实例的移动物体
local m_selectTransform --实例移动在此物体下
local m_selectArrayID=nil   --记录点击服务器数组列表的ID
local m_uiServerStateImage;   --服务器信息栏状态图标
local m_uiButtonList = nil; --UnityEngine.GameObject
local m_uiNoticeLayer = nil; --UnityEngine.GameObject
local m_uiNoticeText = nil; --UnityEngine.GameObject
-- 组列表
local m_GroupList = nil;
-- 服务器列表
local m_ServerList = nil;

-- 当前选择的服务器ID
local m_selectServerID = 0;

--服务器的数量
local m_serverNumber=0;

-- 用户名密码
local m_UserName = "";
local m_PassWord = "";

-- 服务器名对象池
local m_objectPoolServer = {};
--服务器组名对象池
local m_objectPoolArray = {};

local GROUPLIST_CONTROLID = 1000
local SERVERLIST_CONTROLID = 10000

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
	ResourceManager.UnloadAssetBundle( "_ab_ui_static_notice1" )
	ResourceManager.UnloadAssetBundle( "_ab_ui_static_notice2" )
	m_Dlg = nil;
	m_objectPoolServer = {};
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function LoginModOnEvent( nType, nControlID, value )
	if nType == UI_EVENT_CLICK then
		-- 快速试玩
		if nControlID == 1 then 
			LoginModQuickLogin();
			
		--关闭服务器列表
		elseif nControlID == -1  then
		    LoginModCloseServerList();
			
		elseif nControlID == -2 then
			SetFalse( m_uiNoticeLayer )
			
		-- 账户登陆	
		elseif nControlID == 2 then
			LoginModLogin();
			
		--关闭每组的小列表 返回大列表
		elseif nControlID == 3 then
			LoginModCloseArray();
			
		--切换账号
		elseif nControlID == 4 then
			return;
			
		--游戏公告
		elseif nControlID == 5 then
			 LoginModNoticeShow()
			
		--服务器列表
		elseif nControlID == 6 then
			if IsActive(m_uiServerList)	then
				LoginModCloseServerList();
			else
				LoginModOpenServerList();
			end
			
		--联系客服
		elseif nControlID == 7 then
			return;
			
		-- 显示服务器列表	
		elseif nControlID == 10 then
			LoginModOpenServerList();
			
		elseif nControlID == 1000 then	
			LoginModSelectArrayList( nControlID );
			
		-- 选择组	
		elseif nControlID > GROUPLIST_CONTROLID and nControlID < SERVERLIST_CONTROLID then
		    LoginModSelectArrayList( nControlID - GROUPLIST_CONTROLID )
			
		-- 选择服务器
		elseif nControlID > SERVERLIST_CONTROLID and nControlID < SERVERLIST_CONTROLID*2 then
			LoginModSelectServer( nControlID - SERVERLIST_CONTROLID );
		end
		
	elseif nType == UI_EVENT_TWEENFINISH then
		if nControlID == 1 then
			m_uiWarning.gameObject:SetActive( false );
		elseif nControlID ==2 then
		    m_selectObj.transform:SetParent(m_selectTransform.transform,false);
			LoginModOpenSelectArrayServer(m_selectArrayID);
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
	m_uiContent = objs[5];
	m_uiServerName = objs[6];
	m_uiWarning = objs[7];
	m_uiLoading = objs[8];
	m_uiFastEnter = objs[9];
	m_uiRegEnter = objs[10];
	m_uiArrayName = objs[11];
	m_uiArrayObjs[1000] = objs[12];
	m_selectTransform = objs[13]
	m_uiServerStateImage = objs[14];
	m_uiButtonList = objs[15];
	m_uiNoticeLayer = objs[16];
	m_uiNoticeText = objs[17];

	-- 版本号
	m_uiVersion:GetComponent( typeof(UIText) ).text = "v "..Application.version.."("..Global.GetValue("RESOURCE_VERSION")..")"--[[.."lang："..DeviceHelper.getLanguage().."-"..DeviceHelper.getCountry()--]];
	LoginModCloseTestLogin();
	LoginModOpenTestLogin();
end

-- 界面初始化时调用
function LoginModOnStart()
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
	
	-- 读取上次登陆过的账户密码
	m_uiAccountEdit.transform:Find("Input"):GetComponent( "UIInputField" ).text = GameManager.ini( "USERNAME", "" );
	m_uiPasswordEdit.transform:Find("Input"):GetComponent( "UIInputField" ).text = GameManager.ini( "PASSTOKEN", "" );
	
	-- 获取服务器列表
	HttpRequest.GetServerList( function( response )
		local json = require "cjson"
		local info = json.decode( response );
		if info == nil then
			netlog( response );
			return;
		end
		m_GroupList = info["grouplist"];
		m_ServerList = info["serverlist"];
		
		-- 检查是否有登陆过的服务器
		m_selectServerID = GameManager.ini( "USED_SERVER_TAB", "0" );
		if m_selectServerID == "0" then
			 --没有登陆过的服务器就打开 服务器列表
			LoginModOpenServerList(); 
		else
			--有登陆过 就读取并选择最后一次登陆的服务器
			local m_selectServerTab = json.decode(m_selectServerID)
			local index = #m_selectServerTab			
			LoginModSelectServer( m_selectServerTab[index][1] );
		end 
		
		-- 需要显示的
		m_uiAccountEdit.gameObject:SetActive( true );
		m_uiPasswordEdit.gameObject:SetActive( true );
		m_uiServerInfo.gameObject:SetActive( true );
		m_uiButtonList.gameObject:SetActive( true );
	end );
end

-- 关闭测试模式登陆
function LoginModCloseTestLogin()
	if m_Dlg == nil then
		return;
	end
	-- 需要隐藏的
	m_uiAccountEdit.gameObject:SetActive( false );
	m_uiPasswordEdit.gameObject:SetActive( false );
	m_uiFastEnter.gameObject:SetActive( false );
	m_uiRegEnter.gameObject:SetActive( false );
	m_uiServerInfo.gameObject:SetActive( false );
	m_uiButtonList.gameObject:SetActive( false );
end

-- 打开服务器列表
function LoginModOpenServerList()
	m_uiAccountEdit.gameObject:SetActive( false);
	m_uiPasswordEdit.gameObject:SetActive( false );
	m_uiFastEnter.gameObject:SetActive( false );
	m_uiRegEnter.gameObject:SetActive( false );
	m_uiServerInfo.gameObject:SetActive( false );
	m_uiServerList.gameObject:SetActive( true );
	--m_uiButtonList.gameObject:SetActive( false );
	
	
	-- 常用服务器
	local localServerList = GameManager.ini( "USED_SERVER_TAB", "false" );
	if localServerList ~= "false" then 
		local json2 = require "cjson"
		local localServerTable = json2.decode( localServerList );	
		local index = 1;
		for k, v in pairs( localServerTable ) do
			local serverinfo = LoginModGetServerInfo( v[1] );
			if serverinfo ~= nil then
				LoginModAddServer( serverinfo["sid"], serverinfo["n"],serverinfo["new"], serverinfo["cl"],m_uiContent,index);
				index=index+1;
			end
		end
	end
	
	-- 服务器组列表
	for index, obj in pairs( m_objectPoolArray ) do
		SetFalse( obj );
	end
	
	for k, v in pairs(m_GroupList) do
		LoginModAddArray( v.g, v.n )
	end 
end

-- 打开进度条
function LoginModOpenLoading()
	if m_Dlg == nil then
		return;
	end
	LoginModLoginQueue( false );
	
	-- 需要隐藏的
	m_uiAccountEdit.gameObject:SetActive( false );
	m_uiPasswordEdit.gameObject:SetActive( false );
	m_uiServerInfo.gameObject:SetActive( false );
	m_uiFastEnter.gameObject:SetActive( false );
	m_uiRegEnter.gameObject:SetActive( false );
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

--添加服务器组列表
function LoginModAddArray(id,name)
	local uiObj=nil;
	for index, obj in pairs( m_objectPoolArray ) do
		if obj and obj.gameObject.activeSelf == false then
			uiObj = obj;
			break;
		end
	end 
	if uiObj == nil then
		uiObj = GameObject.Instantiate( m_uiArrayName );
		uiObj.transform:SetParent( m_uiContent.transform );
		uiObj.transform.localScale = Vector3.one;
		table.insert( m_objectPoolArray, uiObj );
	end
	uiObj:SetActive( true );
	uiObj.transform:GetComponent( "UIButton" ).controlID = id+GROUPLIST_CONTROLID;
	uiObj.transform:GetChild( 0 ):GetChild( 0 ):GetComponent( "UIText" ).text =name
	m_uiArrayObjs[id+0] = uiObj;
end

-- 添加服务器列表
function LoginModAddServer( id, name, new, close, setparent,siblingIndex )
	if setparent == nil then
		return
	end
	-- 先检查对象缓存池是否有空余的
	local uiObj = nil;
	for index, obj in pairs( m_objectPoolServer ) do
		if obj.gameObject~=nil and obj.gameObject.activeSelf == false then
			uiObj = obj;
			break;
		end
	end
	
	-- 没有空余的就新创建一个
	if uiObj == nil then
		uiObj = GameObject.Instantiate( m_uiServerName );
		uiObj.transform:SetParent( setparent.transform );
		uiObj.transform.localScale = Vector3.one;
		table.insert( m_objectPoolServer, uiObj );
	end
	uiObj:SetActive( true );
	local statename = LoginModTextSet(new,close)
	uiObj.transform:GetComponent( "UIButton" ).controlID = SERVERLIST_CONTROLID + id;
	uiObj.transform:Find("Back/Text"):GetComponent( "UIText" ).text = statename..name;
	local selectBack=uiObj.transform:Find("SelectBack");
	if m_selectServerID == id then
		if IsActive(selectBack) == false then
			SetTrue(selectBack);
		end
	else
		if IsActive(selectBack) == true then
			SetFalse(selectBack);
		end
	end
	if siblingIndex~=nil then
		uiObj.transform:SetSiblingIndex(siblingIndex);
	end
	
end

function LoginModTextSet(snew,sclose )
	local srvname = ""
	local close = tonumber(sclose);
	local newR = tonumber(snew)%10 ;
	if close == 1 then
		srvname = T(433);
	else
		if  newR == 2 then
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

-- 设置当前已经选择的服务器
function LoginModSelectServer( id )
	local serverinfo =LoginModGetServerInfo( id )
	if serverinfo == nil then
		return;
	end
	if tonumber(serverinfo["cl"]) == 1 then
		local text = T(411)
		LoginModWarning(text);
		return
	end
	Const.servername = serverinfo["n"];
	m_selectServerID = serverinfo["sid"];
	local statename = LoginModTextSet(serverinfo["new"],serverinfo["cl"])
	m_uiServerInfo.transform:Find("SeverNameTexxt"):GetComponent( "UIText" ).text = serverinfo["n"];
	m_uiServerInfo.transform:Find("StateText"):GetComponent( "UIText" ).text = statename ;
	local newnumber = tonumber(serverinfo["new"]) % 10
	if newnumber == 0 or newnumber == 1	then
		newnumber = 2
	end	
	local image = LoadSprite( "ui_login_serverstate_"..newnumber );
	SetImage( m_uiServerStateImage, image );
	
	-- 对象缓存池
	for index, obj in pairs( m_objectPoolArray ) do
		if obj then
			obj.gameObject:SetActive( false );
		end
	end
	for index, obj in pairs(m_objectPoolServer ) do
		if obj.gameObject~=nil then
			obj.gameObject:SetActive( false );
		end
	end

    if IsActive( m_selectTransform ) == true then
		LoginModCloseArray();
	end
	m_uiServerList:SetActive( false );
	m_uiFastEnter.gameObject:SetActive( true );
	m_uiRegEnter.gameObject:SetActive( true );
	m_uiAccountEdit.gameObject:SetActive( true );
	m_uiPasswordEdit.gameObject:SetActive( true );
	m_uiServerInfo.gameObject:SetActive( true );
end

-- 自动登录
function LoginModAutoLogin()
--[[	m_UserNameEdit:GetComponent( "UIInputField" ).text = Data.userini:ReadValue("USERNAME", "")
	m_PassWordEdit:GetComponent( "UIInputField" ).text = Data.userini:ReadValue("PASSTOKEN", "")
	m_nCurrServerID = Data.userini:ReadValue( "LASTSERVERID", "0" );
	if m_nCurrServerID == "0" then
		return -1;
	end
	if m_UserNameEdit:GetComponent( "UIInputField" ).text == "" then
		return -1;
	end
	if m_PassWordEdit:GetComponent( "UIInputField" ).text == "" then
		return -1;
	end
	if Global.GetValue("ISLOGOUT" ) == "1" then
		return -1;
	end
	local loginType = tonumber(Data.userini:ReadValue( "LASTLOGINTYPE", "0" ));
	if loginType == 0 then
		return -1;
	end
	-- 登录
	if loginType == 1 then
		LoginModLogin()

	-- 快速试玩
	elseif loginType == 2 then
		LoginModQuickLogin()
		
	end
	return 0;--]]
end

-- 快速登陆
function LoginModQuickLogin()
	LoginModCloseTestLogin();
	LoginModLoginQueue( true );
	m_UserName = "";
	m_PassWord = "";
	GameManager.writeini( "LASTSERVERID", m_selectServerID );
	GameManager.writeini( "LASTLOGINTYPE", 2 );	
	local serverinfo = LoginModGetServerInfo( m_selectServerID )
	if serverinfo == nil then
		AlertMsg( T(406) )
		LoginModOpenTestLogin()
		m_uiFastEnter.gameObject:SetActive( true );
		m_uiRegEnter.gameObject:SetActive( true );
		return
	end
	Network.SDKConnectServer(serverinfo["h"], serverinfo["p"] );
end

-- 用户密码登陆
function LoginModLogin()
	LoginModCloseTestLogin();
	LoginModLoginQueue( true );
	m_UserName = m_uiAccountEdit.transform:Find("Input"):GetComponent( "UIInputField" ).text;
	m_PassWord = m_uiPasswordEdit.transform:Find("Input"):GetComponent( "UIInputField" ).text;
	--GameManager.writeini( "LASTSERVERID", m_selectServerID );
	--GameManager.writeini( "LASTLOGINTYPE", 2);
	LoginModWriteServer(2)
	local serverinfo = LoginModGetServerInfo( m_selectServerID )
	if serverinfo == nil then
		AlertMsg( T(406) )
		LoginModOpenTestLogin()
		m_uiFastEnter.gameObject:SetActive( true );
		m_uiRegEnter.gameObject:SetActive( true );
		return
	end
	Network.SDKConnectServer( serverinfo["h"], serverinfo["p"] );
end

-- SDK模式登陆游戏
function LoginModSDKLoginProc()
	gamelog("LoginModSDKLoginProc");
		
	-- 登陆
	local sendValue = {};
	sendValue.m_username = Const.sdk_uid
	sendValue.m_username_length = string.len( sendValue.m_username );
	
	sendValue.m_password = Const.sdk_sign.."_"..Const.sdk_timestamp
	sendValue.m_password_length = string.len( sendValue.m_password );
	
	sendValue.m_deviceid = Utils.deviceUniqueIdentifier;
	sendValue.m_deviceid_length = string.len( sendValue.m_deviceid );
	
	sendValue.m_isnew = tonumber(Const.sdk_isnewuser);
	sendValue.m_ipcountry = g_ipCountryStr;
	sendValue.m_ipcountry_length = string.len( sendValue.m_ipcountry );
	netsend_login_C( sendValue );
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

-- 警告字符串
function LoginModWarning( text )
	m_uiWarning.transform:Find("Text"):GetComponent("UIText").text = text;
	m_uiWarning.gameObject:SetActive( true );
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

function LoginModWriteServer(loginType)	
	local isServerTable=GameManager.ini( "USED_SERVER_TAB", "false" );
	local json_used = require "cjson"
	m_servertab={}
	if isServerTable=="false" then		
		m_servertab[1]={m_selectServerID,loginType}
		--print("m_selectServerID:"..m_selectServerID.."   tabletype:"..type(m_servertab).."m_servertab[1][1]:"..m_servertab[1][1])
		local jsonServerID=json_used.encode(m_servertab)
		GameManager.writeini("USED_SERVER_TAB",jsonServerID)		
	else
		local m_servertab=json_used.decode(isServerTable)
		for k,v in pairs(m_servertab)do 
			if v[1]==m_selectServerID then
				table.remove(m_servertab,k)
				table.insert(m_servertab,v)
				local jsonServerID=json_used.encode(m_servertab)
				GameManager.writeini("USED_SERVER_TAB",jsonServerID)	
				return
			end
		end		
		if table.getn(m_servertab)<3 then
			table.insert(m_servertab,{m_selectServerID,loginType})		 
			local jsonServerID=json_used.encode(m_servertab)
			GameManager.writeini("USED_SERVER_TAB",jsonServerID)				
		else
			table.remove(m_servertab,1)
			 table.insert(m_servertab,{m_selectServerID,loginType})
			local jsonServerID=json_used.encode(m_servertab)
			GameManager.writeini("USED_SERVER_TAB",jsonServerID)		
		end		
	end	
end

--点击组
function LoginModSelectArrayList( buttonid )
	m_selectArrayID = buttonid;
	local selectObj = m_uiArrayObjs[buttonid];
	m_selectObj = GameObject.Instantiate( m_uiArrayName )
	m_selectObj.transform:SetParent( m_uiContent.transform.parent,false);
	m_selectObj.transform.localScale = Vector3.one;
	SetFalse( m_uiContent );
	m_selectObj.transform:GetComponent( "UIButton" ).controlID = 3;
	local text = selectObj.transform:Find("Back"):Find("Text"):GetComponent("UIText").text;
	m_selectObj.transform:Find("Back"):Find("Text"):GetComponent("UIText").text=text
	--uiObj.transform.localPosition = Vector3.New(222,selectObj.transform.localPosition.y,selectObj.transform.localPosition.z)
	m_selectObj.transform:GetComponent( "UITweenRectPosition" ).from=Vector2(224,selectObj.transform.localPosition.y)
	m_selectObj.transform:GetComponent( "UITweenRectPosition" ).to = Vector2( 224, -8 ); 
	m_selectObj.transform:GetComponent( "UITweenRectPosition" ):Play( true );
end

-- 关闭组
function LoginModCloseArray()
	SetTrue( m_uiContent );
	clearChild( m_selectTransform )
	m_selectObj = nil
	m_selectArrayID = 0
end

function LoginModOpenSelectArrayServer(serverid) 
	if serverid==1000 then
		local recommendList={}
		for k,v in pairs(m_ServerList) do
			if math.floor(tonumber(v["new"])/10)==1 then
				table.insert(recommendList,m_ServerList[k])
			end
		end
		for  i=1,#recommendList  do
			LoginModAddServer( recommendList[i]["sid"], recommendList[i]["n"],recommendList[i]["new"],recommendList[i]["cl"],m_selectTransform);
		end
	else
		local arrayid=serverid;
		for k,v in pairs(m_ServerList) do
			if tonumber(v["g"]) == arrayid then
				LoginModAddServer( v.sid, v.n,v.new,v.cl,m_selectTransform );
			end
		end
	end
end
-- 关闭服务器列表
function LoginModCloseServerList()
	if IsActive( m_selectTransform ) == true then
		LoginModCloseArray();
	end
	m_uiAccountEdit.gameObject:SetActive( true );
	m_uiPasswordEdit.gameObject:SetActive( true );
	m_uiServerInfo.gameObject:SetActive( true );
	m_uiFastEnter.gameObject:SetActive( true );
	m_uiRegEnter.gameObject:SetActive( true );
	m_uiServerList.gameObject:SetActive( false );
	--m_uiButtonList.gameObject:SetActive( true );
	for index, obj in pairs(m_objectPoolServer ) do
		if obj.gameObject~=nil then
			obj.gameObject:SetActive( false );
		end
	end
end

-- 显示公告
function LoginModNoticeShow()
	SetTrue( m_uiNoticeLayer )
	ResourceManager.LoadAssetBundle( "_ab_ui_static_notice1" )
	ResourceManager.LoadAssetBundle( "_ab_ui_static_notice2" )
	HttpRequest.GetFile( "notice.cfg", function( response )
		SetText(m_uiNoticeText, response)
	end )
end



	