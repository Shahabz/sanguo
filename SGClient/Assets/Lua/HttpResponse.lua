json = require "cjson"

HttpResponse = {};

-- SDK获取用户信息
function HttpResponse.GetSDKUserInfo( response )	
	local info = json.decode( response );
	if info == nil then
		netlog( response );
		return;
	end
	local serverid = info["s"];
	local host = info["h"];
	local port = info["p"];
	Const.serverid = serverid;
	
	-- 入口是否关闭
	local close = info["close"];
	if close == nil then
		close = 0;
	end
	
	-- 入口关闭显示维护中
	if tonumber( close ) == 1 then
		Network.GetNotice();
	else
		Network.SDKConnectServer( host, port );
	end
	
	-- IP所在国家
	if info["ipcountry"] ~= nil and info["ipcountry"] ~= "" then
		g_ipCountryStr = string.lower( info["ipcountry"] );
	end
end
