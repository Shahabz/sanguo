-- 渠道SDK
SDK = SDK or {};
	
-- SDK初始化
function SDK.init()
	ChannelSDK.Instance:init("");
end

-- SDK初始化回调
function SDK.onInit( jsonResult )
	local json = require "cjson"
	local info = json.decode( jsonResult );   
	local result =  info["result"];
	if result == "1" then
		Const.sdk_isinit = true;
		SDK.login()
	end
end

-- SDK登陆
function SDK.login()
	ChannelSDK.Instance:login("");
end

-- SDK登陆回调
function SDK.onLogin( jsonResult )
	local json = require "cjson"
	local info = json.decode( jsonResult );   
	
	local result 	    = info["result"];
	if result == "1" then
		Const.sdk_uid 		= info["uid"];
		Const.sdk_token 	= info["token"];
		Const.sdk_isverify 	= info["isverify"];
	else
		SDK.login()
	end
end

-- SDK登出
function SDK.logout()
	ChannelSDK.Instance:logout("");
end

-- SDK登出回调
function SDK.onLogout( jsonResult )
	Const.sdk_uid 		= "";
	Const.sdk_token 	= "";
	SDK.login()
end

-- 切换账号回调
function SDK.onSwitchAccount( jsonResult )
	GameManager.restart = true;
	GameManager.Logout( 0 );
	SDK.login()
end

-- 传额外参数
function SDK.setExtendData()
	if Const.platid == 12 then -- 在野
		local json = require "cjson"
		local info = {}
		info["actorname"] = GetPlayer().m_name
		info["actorid"] = GetPlayer().m_actorid
		info["actorlevel"]= GetPlayer().m_level
		info["serverid"] = Const.serverid
		info["servername"] = "s"..Const.serverid
		local jsonMsg = json.encode( info );   
		ChannelSDK.Instance:setExtendData( jsonMsg );
	else
	end
end

