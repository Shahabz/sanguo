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

-- 服务器列表
local m_ServerList = {};

-- 当前选择的服务器ID
local m_selectServerID = 0;

-- 用户名密码
local m_UserName = "";
local m_PassWord = "";

-- 服务器名对象池
local m_objectPoolServer = {};

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
		
		-- 账户登陆	
		elseif nControlID == 2 then
			LoginModLogin();
		
		-- 显示服务器列表	
		elseif nControlID == 10 then
			LoginModOpenServerList();
				
		-- 选择服务器
		elseif nControlID >= 100 then
			LoginModSelectServer( nControlID - 100 );
			
		end
	elseif nType == UI_EVENT_TWEENFINISH then
		if nControlID == 1 then
			m_uiWarning.gameObject:SetActive( false );
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
	-- 版本号
	m_uiVersion:GetComponent( typeof(UIText) ).text = "v "..Application.version.."("..Global.GetValue("RESOURCE_VERSION")..")  ".."lang："..DeviceHelper.getLanguage().."-"..DeviceHelper.getCountry();
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
	-- 需要显示的
	m_uiAccountEdit.gameObject:SetActive( true );
	m_uiPasswordEdit.gameObject:SetActive( true );
	m_uiServerInfo.gameObject:SetActive( true );
	
	-- 需要隐藏的

	
	-- 读取上次登陆过的账户密码
	m_uiAccountEdit.Find("Input"):GetComponent( "UIInputField" ).text = GameManager.ini( "USERNAME", "" );
	m_uiPasswordEdit.Find("Input"):GetComponent( "UIInputField" ).text = GameManager.ini( "PASSTOKEN", "" );
	 
	-- 获取服务器列表
	HttpRequest.GetServerList( function( response )
		local json = require "cjson"
		local info = json.decode( response );
		if info == nil then
			netlog( response );
			return;
		end
		-- 设置缓存，ID传字符串
		m_ServerList = info;	
		
		-- 检查是否有登陆过的服务器
		m_selectServerID = GameManager.ini( "LASTSERVERID", "0" );
		if m_selectServerID == "0" then
			LoginModOpenServerList();
		else
			LoginModSelectServer( m_selectServerID );
		end 
	
	end );
	
	-- test
--[[	for i=1, 100, 1 do
		Invoke( function() 
						local prefab = LoadPrefab( "UIU_Damage" ) 
						local obj = GameObject.Instantiate( prefab );
						obj.transform:SetParent( m_Dlg.transform );
						--obj.transform.localScale = Vector3( 1, 1, 1 );
						obj.transform:Find("Text"):GetComponent("UIText").text = custom.rand(10, 100);
		end, i, obj)
	end--]]
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
end

-- 打开服务器列表
function LoginModOpenServerList()
	m_uiFastEnter.gameObject:SetActive( false );
	m_uiRegEnter.gameObject:SetActive( false );
	m_uiServerList.gameObject:SetActive( true );
	-- 创建服务器名对象
	for k, v in pairs( m_ServerList ) do
		LoginModAddServer( v["sid"], v["n"] );
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
	
	-- 需要显示的
	m_uiLoading.gameObject:SetActive( true );
	m_uiLoading.transform:GetComponent( "Slider" ).value = 0.0;
	
    -- 读取所有AssetBundle
    eye.resourceManager:LoadAllAssetBundle();	
end

-- 读取AssetBundle的回调
function LoginModOnLoadAssetBundleProc( assetBundleName, progress, totalProgress )
	if progress == totalProgress then
		m_uiLoading.transform:GetComponent( "Slider" ).value = progress/totalProgress;
		-- 加载主城
		LoadPrefabAsyn( "MainCityScence", function( obj )
			GameManager.MainCityScence = GameObject.Instantiate( obj );
			MainDlgOpen();
			LoginModClose();
			netsend_list_C( 0 );
			
			coroutine.start( function()
			-- 测试对象
			eye.objectPoolManager:CreatePool("hero_brute_dragon", 5, 10, LoadPrefab(heroprefab[1]))

			eye.objectPoolManager:CreatePool("hero_centaur", 5, 10, LoadPrefab(heroprefab[2]))

			eye.objectPoolManager:CreatePool("hero_faith_healer", 5, 10, LoadPrefab(heroprefab[3]))

			eye.objectPoolManager:CreatePool("hero_orc_monk", 5, 10, LoadPrefab(heroprefab[4]))

			eye.objectPoolManager:CreatePool("hero_pirate", 5, 10, LoadPrefab(heroprefab[5]))

			eye.objectPoolManager:CreatePool("hero_polemaster", 5, 10, LoadPrefab(heroprefab[6]))

			eye.objectPoolManager:CreatePool("hero_shadow_assassin", 5, 10, LoadPrefab(heroprefab[7]))
			--coroutine.wait( 1/30 );
			end)
	
		end );
--[[		LoginModClose();
		FightDlgOpen();
		GetFightRoom():Create( { m_fightid = 1, m_side = 1, m_maxtime = 150 } );--]]
	end
end

-- 添加服务器列表
function LoginModAddServer( id, name )
	
	-- 先检查对象缓存池是否有空余的
	local uiObj = nil;
	for index, obj in pairs( m_objectPoolServer ) do
		if obj and obj.gameObject.activeSelf == false then
			uiObj = obj;
			break;
		end
	end
	
	-- 没有空余的就新创建一个
	if uiObj == nil then
		uiObj = GameObject.Instantiate( m_uiServerName );
		uiObj.transform:SetParent( m_uiContent.transform );
		uiObj.transform.localScale = Vector3.one;
		table.insert( m_objectPoolServer, uiObj );
	end
	uiObj:SetActive( true );
	uiObj.transform:GetComponent( "UIButton" ).controlID = 100 + id;
	uiObj.transform:GetChild( 0 ):GetChild( 0 ):GetComponent( "UIText" ).text = name;
end

-- 设置当前已经选择的服务器
function LoginModSelectServer( id )
	local serverinfo = m_ServerList[tostring(id)];
	if serverinfo == nil then
		return;
	end
	m_selectServerID = serverinfo["sid"];
	m_uiServerInfo.transform:Find("Text"):GetComponent( "UIText" ).text = serverinfo["n"];
	-- 对象缓存池
	for index, obj in pairs( m_objectPoolServer ) do
		if obj then
			obj.gameObject:SetActive( false );
		end
	end
	m_uiServerList:SetActive( false );
	m_uiFastEnter.gameObject:SetActive( true );
	m_uiRegEnter.gameObject:SetActive( true );
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
	Network.SDKConnectServer( m_ServerList[m_selectServerID]["h"], m_ServerList[m_selectServerID]["p"] );
end

-- 用户密码登陆
function LoginModLogin()
	LoginModCloseTestLogin();
	LoginModLoginQueue( true );
	m_UserName = m_uiAccountEdit.Find("Input"):GetComponent( "UIInputField" ).text;
	m_PassWord = m_uiPasswordEdit.Find("Input"):GetComponent( "UIInputField" ).text;
	GameManager.writeini( "LASTSERVERID", m_selectServerID );
	GameManager.writeini( "LASTLOGINTYPE", 1 );
	Network.SDKConnectServer( m_ServerList[m_selectServerID]["h"], m_ServerList[m_selectServerID]["p"] );
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
	