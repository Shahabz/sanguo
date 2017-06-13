g_ipCountryStr = "us";

--游戏管理器--
GameManager = {};
GameManager.restart = false; -- 强制重启
GameManager.userini = nil;
GameManager.MainCityScence = nil;
GameManager.FightScence = nil;

-- 游戏脚本主逻辑入口
function GameManager.OnStart()
	gamelog("GameManager.OnStart();begin");
	
	-- 读取ini配置文件
	GameManager.userini = IniUtil.New();
	GameManager.userini:OpenFromTXT( PathUtil.ConfigPath() .. "user.txt" );
	if GameManager.userini:ReadValue("USERNAME", "") == "" then
		GameManager.userini:WriteValue("USERNAME","");
	end
	if GameManager.userini:ReadValue("PASSTOKEN", "") == "" then
		GameManager.userini:WriteValue("PASSTOKEN","");
	end
	if GameManager.userini:ReadValue("LASTSERVERID", "") == "" then
		GameManager.userini:WriteValue("LASTSERVERID", 0);
	end
	
	-- 读取平台渠道信息
	local platini = IniUtil.New();
	if platini:OpenFromData( "plat.txt" ) == true then
		Const.platid = tonumber(platini:ReadValue("PLATID", "1")); 
	end
		
	-- 与服务器通用的全局变量
	local _tableUtil = TableUtil.New();
	if _tableUtil:OpenFromData( "global.txt" ) == true then
		local _tableRecordsNum = _tableUtil:GetRecordsNum()-1;
		for records = 0, _tableRecordsNum, 1 do
			local id		 = _tableUtil:GetValue( records, 0 );
			local value 	 = _tableUtil:GetValue( records, 1 );
			local variable	 = _tableUtil:GetValue( records, 2 );
			Global.AddValue( variable, value );
		end
	end
	
	print( OpenUDIDPhone.OpenUDID.value );
	
	-- 打开登陆界面
	LoginModOpen();

	gamelog("GameManager.OnStart();end");
end

-- 游戏销毁
function GameManager.OnDestroy()	

end



-- 请求退出账号
function GameManager.Logout( voluntary )
	-- 1主动 0被动
	Global.AddValue("ISLOGOUT" , voluntary );
	--eye.networkManager:Logout();
end


-- 主逻辑，1分钟一次
function GameManager.GameLogic()
	
end

-- 请求退出游戏
function GameManager.OnAskQuit()
   
    -- 没进游戏
    if Const.NetStatus < 3 then
        return;
    end

    -- 先关闭最后打开的界面
    if eye.uiManager:CloseLast() then
        return;
    end

    
    --[[MsgBoxEx( GetLocalizeText( 507 ), GetLocalizeText( 508 ), nil, GetLocalizeText( 509 ), 
                function()
			        		Application.Quit() 
		        		end )--]]
end

-- 是否进入后台
GameManager.OnApplicationPauseTime = 0;
function GameManager.OnApplicationPause( paused )
	if paused == true then
		gamelog( "OnApplicationPause: paused == true" );
	else
		gamelog( "OnApplicationPause: paused == false" );
	end

	-- 进入后台
	if paused == true then

		GameManager.OnApplicationPauseTime = os.time();
	
	-- 从后台进入前台时
	else

		gamelog( "PauseTime:"..GameManager.OnApplicationPauseTime.." os.time():"..os.time() );
		-- 检测是否断线
		if GameManager.OnApplicationPauseTime > 0 and os.time() - GameManager.OnApplicationPauseTime > 180 then
			
				if Const.NetStatus > 2 or GameManager.restart == true then
					Invoke( function() 
							eye.uiManager:Clear();
							LoginModDestroy();
							SceneManager.LoadScene( "launcher" ); 
						end, 1/2 ); 
				end
				Const.NetStatus = 0;
		end
		GameManager.OnApplicationPauseTime = 0;
	end
	
end

-- 摇一摇
local m_WaveTime = 0;
function GameManager.GameWave()
	if os.time() - m_WaveTime <= 3 then
      return;
  end
  m_WaveTime = os.time();
end

-- 所有UI关闭事件
function GameManager.AllUIClose()
    --GetDialogQueue():Show();
end

-- 远程推送
function GameManager.PushKey( key )
	local sendValue = {}
	sendValue.m_pushkey = key;
	sendValue.m_pushkey_len = string.len( sendValue.m_pushkey );
	netsend_pushkey_C( sendValue )
end

-- 获取用户本地配置
function GameManager.ini( key, default )
	return GameManager.userini:ReadValue( key, default );
end
function GameManager.writeini( key, value )
	GameManager.userini:WriteValue( key, value );
end
