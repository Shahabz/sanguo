HttpRequest = {};
local SECRET_KEY = "eye^sanguo"

-- 注册用户
function HttpRequest.RegisterUser( username, pwd, callback )
	local paramString = "c=userinfo&m=register"
	.."&username="..username
	.."&pwd="..pwd
	.."&lang="..DeviceHelper.getLanguage()
	.."&country="..DeviceHelper.getCountry()
	.."&channelId="..Const.sdk_channelId
	.."&os="..Const.sdk_sysType
	.."&version="..Application.version
	.."&platid="..Const.platid

	HttpRequest.Get( paramString, callback );
end

-- SDK获取用户信息
function HttpRequest.GetSDKUserInfo()
	local paramString = "c=userinfo&m=get"
	.."&uid="..Const.sdk_uid
	.."&lang="..DeviceHelper.getLanguage()
	.."&country="..DeviceHelper.getCountry()
	.."&channelId="..Const.sdk_channelId
	.."&os="..Const.sdk_sysType
	.."&version="..Application.version
	.."&platid="..Const.platid

	HttpRequest.Get( paramString, HttpResponse.GetSDKUserInfo );
end

-- 获取服务器信息列表
function HttpRequest.GetServerList( callback )
	local paramString = "c=serverinfo&m=getlist"
	.."&uid="..Const.sdk_uid
	.."&country="..DeviceHelper.getCountry()
	.."&channelId="..Const.sdk_channelId
	.."&os="..Const.sdk_sysType
	.."&platid="..Const.platid

	HttpRequest.Get( paramString, callback );
end

-- 获取文字
function HttpRequest.GetText( id, ext, callback )
	local paramString = "c=serverinfo&m=gettext"
	.."&id="..id
	.."&lang="..DeviceHelper.getLanguage()
	.."&cn="..DeviceHelper.getCountry()
	.."&ext="..ext

	HttpRequest.Get( paramString, callback );
end

-- 执行http get请求
-- 参数{c="serverinfo",m="getlist"}
function HttpRequest.Get( paramString, callBack )
	local url = Global.GetValue("CLIENTACCESS_URL");
	local sign = Utils.md5( paramString..SECRET_KEY );
	url = url.."api.php?"..paramString.."&sign="..sign;
	print( url )
	-- 启动协程
	coroutine.start( function()
		local request = WWW( url );
		coroutine.www( request );
		if callBack ~= nil then
			callBack( tolua.tolstring( request.bytes ) );
		end
	end)
end

-- 执行http get静态文件请求
function HttpRequest.GetFile( filename, callBack )
	local url = Global.GetValue("FILE_URL");
	url = url..filename;
	print( url )
	-- 启动协程
	coroutine.start( function()
		local request = WWW( url );
		coroutine.www( request );
		if callBack ~= nil then
			callBack( tolua.tolstring( request.bytes ) );
		end
	end)
end

-- 执行http post请求
function HttpRequest.Post( url, wwwform, callBack )
	-- 启动协程
	coroutine.start( function()
		local request = WWW( url, wwwform );
		coroutine.www( request );
		if callBack ~= nil then
			callBack( tolua.tolstring( request.bytes ) );
		end
	end)
end