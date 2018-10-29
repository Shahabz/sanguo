-- 渠道SDK
SDK = SDK or {};

-- Google
SDK.logintype = ""
SDK.GoogleUserName = ""
SDK.FirebaseUserId = ""
	
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
		if Const.platid == 31 or Const.platid == 32 then
			
		else
			SDK.login()
		end
	end
end

-- SDK登陆
function SDK.login()
	ChannelSDK.Instance:login("");
end
function SDK.GoogleLogin()
	local json = require "cjson"
	local info = {}
	info["logintype"] = "google"
	local jsonMsg = json.encode( info ); 
	ChannelSDK.Instance:login(jsonMsg);
end
function SDK.FacebookLogin()
	local json = require "cjson"
	local info = {}
	info["logintype"] = "facebook"
	local jsonMsg = json.encode( info ); 
	ChannelSDK.Instance:login(jsonMsg);
end
function SDK.GuestLogin()

end

-- SDK登陆回调
function SDK.onLogin( jsonResult )
	local json = require "cjson"
	local info = json.decode( jsonResult );   

	local result 	    = info["result"];
	if result == "1" then
		if info["logintype"] ~= nil then
			if info["logintype"] == "google" then
				
			elseif info["logintype"] == "facebook" then
				
			end
		else
			if Const.platid == 22 then
				Const.sdk_timestamp = info["appId"];
				Const.sdk_channelId = info["ChannelId"];
				Const.sdk_uid 		= info["ChannelUserId"];
				Const.sdk_token 	= info["token"];
			elseif Const.platid == 26 then
				Const.sdk_timestamp = info["timestamp"];
				Const.sdk_channelId = info["ChannelId"];
				Const.sdk_uid 		= info["ChannelUserId"];
				Const.sdk_token 	= info["token"];
			else
				Const.sdk_uid 		= info["uid"];
				Const.sdk_token 	= info["token"];
				Const.sdk_isverify 	= info["isverify"];
				Const.sdk_timestamp = 0;
				Const.sdk_channelId = 0;
			end
		end
	else
		SDK.login()
	end
end
function SDK.onGoogleLogin( info )
	SDK.logintype = info["logintype"]
	SDK.GoogleUserName = info["UserName"]
	SDK.FirebaseUserId = info["UserId"]
end
function SDK.onFacebookLogin( info )
end

-- SDK登出
function SDK.logout()
	ChannelSDK.Instance:logout("");
end

-- SDK登出回调
function SDK.onLogout( jsonResult )
	Const.sdk_uid 		= "";
	Const.sdk_token 	= "";
	
	local json = require "cjson"
	local info = json.decode( jsonResult );   
	local result 	    = info["result"];
	if result == "1" then
		SDK.login()
	elseif result == "2" then
		GameManager.Restart();
		GameManager.Logout( 1 );
		SDK.login()
	end
end

-- 切换账号回调
function SDK.onSwitchAccount( jsonResult )
	--GameManager.restart = true;
	--GameManager.Logout( 0 );
	SDK.login()
end

-- 支付
-- m_orderid_len=0,m_orderid="[m_orderid_len]",m_ext_len=0,m_ext="[m_ext_len]",m_goodsid=0,m_productid=0,m_nameid=0,m_descid=0,m_price=0,
function SDK.pay( recvValue )
	local json = require "cjson"
	local info = {}
	info["product_id"] = recvValue.m_productid
	info["product_price"] = recvValue.m_price
	info["product_orider"] = recvValue.m_orderid
	info["product_ext"] = recvValue.m_ext
		
	if Const.platid == 13 or Const.platid == 15 or Const.platid == 11 or Const.platid == 1 then
		if recvValue.m_paymode == 0 then
			local jsonMsg = json.encode( info ); 
			ChannelSDK.Instance:pay( jsonMsg );
		else
			if Const.platid == 13 then
				IAppPay_sgbl( recvValue )
			elseif Const.platid == 15 then
				IAppPay_fysgz( recvValue )
			end
		end
	
	elseif Const.platid == 19 then
		if recvValue.m_paymode == 0 then
			local jsonMsg = json.encode( info ); 
			ChannelSDK.Instance:pay( jsonMsg );
		else
			local url = Global.GetValue("CLIENTACCESS_URL");
			Application.OpenURL( url.."wmcard.php"
							.."?product_id="..recvValue.m_productid
							.."&product_price="..(recvValue.m_price*100)
							.."&product_orider="..recvValue.m_orderid
							.."&product_ext="..WWW.EscapeURL(recvValue.m_ext)
							.."&product_name="..WWW.EscapeURL(T(recvValue.m_nameid)) )
		end
		
	elseif Const.platid == 14 then
		IAppPay_sgbl( recvValue )
		
	elseif Const.platid == 16 then
		IAppPay_fysgz( recvValue )
		
	elseif Const.platid == 17 then
		local url = Global.GetValue("SERVERACCESS_URL");
		info["product_price"] = recvValue.m_price*100
		info["product_actorid"] = GetPlayer().m_actorid
		info["product_name"] = T(recvValue.m_nameid)
		info["product_notifyurl"] = url.."iapppay_fysgz_trpay/trpay.php"
		info["product_notifyurl_params"] = "ext="..recvValue.m_ext
		local jsonMsg = json.encode( info );
		ChannelSDK.Instance:pay( jsonMsg );
		
	elseif Const.platid == 18 or Const.platid == 19 or Const.platid == 20 or Const.platid == 24 or Const.platid == 25 then
		local url = Global.GetValue("CLIENTACCESS_URL");
		Application.OpenURL( url.."wmcard.php"
							.."?product_id="..recvValue.m_productid
							.."&product_price="..(recvValue.m_price*100)
							.."&product_orider="..recvValue.m_orderid
							.."&product_ext="..WWW.EscapeURL(recvValue.m_ext)
							.."&product_name="..WWW.EscapeURL(T(recvValue.m_nameid)) )
							
	elseif Const.platid == 21 then
		local url = Global.GetValue("CLIENTACCESS_URL");
		Application.OpenURL( url.."wmcard.php"
							.."?product_id="..recvValue.m_productid
							.."&product_price="..(recvValue.m_price*100)
							.."&product_orider="..recvValue.m_orderid
							.."&product_ext="..WWW.EscapeURL(recvValue.m_ext)
							.."&product_name="..WWW.EscapeURL(T(recvValue.m_nameid)) )
	
	
	-- 易接android,可自由转化到完美数卡支付(单位：分)
	elseif Const.platid == 22 then	
		if recvValue.m_paymode == 0 then
			local url = Global.GetValue("SERVERACCESS_URL");
			info["product_ext"] = WWW.EscapeURL(recvValue.m_ext.."#"..recvValue.m_orderid)
			info["product_price"] = recvValue.m_price*100
			info["product_actorid"] = GetPlayer().m_actorid
			info["product_name"] = T(recvValue.m_nameid)
			--info["product_notifyurl"] = url.."tlsg_yj/pay.php"
			info["product_notifyurl"] = ""
			info["product_notifyurl_params"] = "";
			local jsonMsg = json.encode( info ); 
			ChannelSDK.Instance:pay( jsonMsg );
		else
			local url = Global.GetValue("CLIENTACCESS_URL");
			Application.OpenURL( url.."weizuo/wmcard.php"
							.."?product_id="..recvValue.m_productid
							.."&product_price="..(recvValue.m_price*100)
							.."&product_orider="..recvValue.m_orderid
							.."&product_ext="..WWW.EscapeURL(recvValue.m_ext)
							.."&product_name="..WWW.EscapeURL(T(recvValue.m_nameid)) )
		end
		
	-- 绿壳ios(单位：元)
	elseif Const.platid == 23 then	
		local url = Global.GetValue("SERVERACCESS_URL");
		info["product_ext"] = WWW.EscapeURL(recvValue.m_ext)
		info["product_actorid"] = tostring(GetPlayer().m_actorid)
		info["product_name"] = T(recvValue.m_nameid)
		info["product_sid"] = tostring(Const.serverid)
		info["product_sname"] = Const.servername;
		info["product_actorname"] = GetPlayer().m_name;
		local jsonMsg = json.encode( info ); 
		ChannelSDK.Instance:pay( jsonMsg );
		
	-- 绿壳android(单位：分)
	elseif Const.platid == 26 then	
		local url = Global.GetValue("SERVERACCESS_URL");
		info["product_price"] = tostring(recvValue.m_price*100)
		info["product_ext"] = string.gsub(recvValue.m_ext, "#", "_");
		info["product_actorid"] = tostring(GetPlayer().m_actorid)
		info["product_name"] = T(recvValue.m_nameid)
		info["product_sid"] = tostring(Const.serverid)
		info["product_sname"] = Const.servername;
		info["product_actorname"] = GetPlayer().m_name;
		local jsonMsg = json.encode( info ); 
		ChannelSDK.Instance:pay( jsonMsg );
	
	-- 海外繁体ios
	elseif Const.platid == 27 then
		local jsonMsg = json.encode( info ); 
		ChannelSDK.Instance:pay( jsonMsg );
	
	-- 海外繁体android	
	elseif Const.platid == 28 then
	end
end

-- 问题提交
function SDK.gmbug()
	BugReportDlgShow()
	--[[if Const.platid == 13 or Const.platid == 14 then -- sgbl-ios
		ChannelSDK.Instance:gmbug( '' );
	elseif Const.platid == 15 or Const.platid == 16 then -- fysgz-ios
		ChannelSDK.Instance:gmbug( '' );
	else
	end--]]
end

-- 用户中心
function SDK.userCenter()
	if Const.platid == 13 or Const.platid == 14 then -- sgbl-ios
		ChannelSDK.Instance:user_center( '' );
	elseif Const.platid == 15 or Const.platid == 16 then -- fysgz-ios
		ChannelSDK.Instance:user_center( '' );
	elseif Const.platid == 21 then -- fysgz-android 爱贝登录-完美舒卡支付
		ChannelSDK.Instance:user_center( '' );
	elseif Const.platid == 1 or Const.platid == 18 or Const.platid == 19 or Const.platid == 20 or Const.platid == 24 or Const.platid == 25 then
		UserInfoDlgShow()
	elseif Const.platid == 27 or Const.platid == 28 then
		UserInfoDlgShow()
	else
	end
end

-- 传额外参数
function SDK.setExtendData( step )
	if Const.platid == 13 or Const.platid == 14 or Const.platid == 15 or Const.platid == 16 then
		local json = require "cjson"
		local info = {}
		info["actorname"] = GetPlayer().m_name
		info["actorid"] = GetPlayer().m_actorid
		info["actorlevel"]= GetPlayer().m_level
		info["serverid"] = Const.serverid
		info["servername"] = Const.servername
		local jsonMsg = json.encode( info );   
		ChannelSDK.Instance:setExtendData( jsonMsg );
	
	-- 易接	
	elseif Const.platid == 22 then
		local json = require "cjson"
		local info = {}
		info["step"] = step
		info["actorname"] = GetPlayer().m_name
		info["actorid"] = tostring(GetPlayer().m_actorid)
		info["actorlevel"]= tostring(GetPlayer().m_level)
		info["actorvip"]= tostring(GetPlayer().m_viplevel)
		info["actortoken"]= tostring(GetPlayer().m_token)
		info["clubname"]= Nation(GetPlayer().m_nation)
		info["createtime"]= tostring(GetPlayer().m_createtime)
		info["levelmtime"]= "0"
		info["serverid"] = tostring(Const.serverid)
		info["servername"] = Const.servername
		local jsonMsg = json.encode( info );   
		ChannelSDK.Instance:setExtendData( jsonMsg );
		
	-- 绿壳	
	elseif Const.platid == 23 or Const.platid == 26 then
		local json = require "cjson"
		local info = {}
		if step == "enterServer" then
			step = "1"
		elseif step == "createrole" then
			step = "2"
		elseif step == "levelup" then
			step = "3"
		elseif step == "exit" then
			step = "4"
		end
		info["step"] = step
		info["actorname"] = GetPlayer().m_name
		info["actorid"] = tostring(GetPlayer().m_actorid)
		info["actorlevel"]= tostring(GetPlayer().m_level)
		info["actorvip"]= tostring(GetPlayer().m_viplevel)
		info["actortoken"]= tostring(GetPlayer().m_token)
		info["clubname"]= Nation(GetPlayer().m_nation)
		info["createtime"]= tostring(GetPlayer().m_createtime)
		info["levelmtime"]= "2"
		info["serverid"] = tostring(Const.serverid)
		info["servername"] = Const.servername
		local jsonMsg = json.encode( info );   
		ChannelSDK.Instance:setExtendData( jsonMsg );
	end
end

