g_ipCountryStr = "us";

--游戏管理器--
GameManager = {};
GameManager.restart = false; -- 强制重启
GameManager.userini = nil;
GameManager.MainCity = nil;
GameManager.WorldMap = nil;
GameManager.currentScence = "city"

-- 全局天气
g_game_day = 0;
g_game_weather = 0;
g_game_day_loop = 0;
		
-- 游戏脚本主逻辑入口
function GameManager.OnStart()
	--local x = int64.new('78962871035984074')
    --print('int64 is: '..x)
	
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
	-- 初始化系统设置
	SettingInit()
	
	-- 读取平台渠道信息
	local platini = IniUtil.New();
	if platini:OpenFromData( "plat.txt" ) == true then
		Const.platid = tonumber(platini:ReadValue("PLATID", "1")); 
	end
		
	print( OpenUDIDPhone.OpenUDID.value );
	
	-- 打开登陆界面
	LoginModOpen();

	gamelog("GameManager.OnStart();end");
end

-- 游戏销毁
function GameManager.OnDestroy()	
	-- 存档邮件缓存
	GetMail():SaveCache();
end



-- 请求退出账号
function GameManager.Logout( voluntary )
	-- 1主动 0被动
	Global.AddValue("ISLOGOUT" , voluntary );
	eye.networkManager:Logout();
end

function GameManager.Restart()
	Const.NetStatus = 0;
	eye.uiManager:Clear();
	LoginModDestroy();
	SceneManager.LoadScene( "launcher" );
end
	
-- 主逻辑秒
GameManager.LastGameLogicTime = 0;
GameManager.HeartTime = 0;
GameManager.CityUpgradeTime = 0;
function GameManager.GameLogic()
	--gamelog( "GameLogic:"..os.time() )
	-- 建造队列
	if GetPlayer().m_worker_sec > 0 then
		GetPlayer().m_worker_sec = GetPlayer().m_worker_sec - 1;
	end
	
	-- 商用建造队列
	if GetPlayer().m_worker_sec_ex > 0 then
		GetPlayer().m_worker_sec_ex = GetPlayer().m_worker_sec_ex - 1;
	end
	
	-- 商用建造队到期时间
--[[	if GetPlayer().m_worker_expire_ex > 0 then
		GetPlayer().m_worker_expire_ex = GetPlayer().m_worker_expire_ex - 1;
	end--]]
	
	-- 保护时间
	if GetPlayer().m_ptsec > 0 then
		GetPlayer().m_ptsec = GetPlayer().m_ptsec - 1
	end
	
	-- 体力回复时间
	if GetPlayer().m_bodysec > 0 then
		GetPlayer().m_bodysec = GetPlayer().m_bodysec - 1
	end
	
	-- 太学院
	local pBuilding = GetPlayer():GetBuilding( BUILDING_Tech, -1 );
	if pBuilding and pBuilding.m_sec > 0 then
		pBuilding.m_sec = pBuilding.m_sec - 1;
	end
	
	-- 铁匠铺
	if GetPlayer().m_forgingsec > 0 then
		GetPlayer().m_forgingsec = GetPlayer().m_forgingsec - 1
	end
	
	-- 兵营
	for kind = BUILDING_Infantry, BUILDING_Militiaman_Archer, 1 do
		local pBuilding = GetPlayer():GetBuilding( kind, -1 );
		if pBuilding and pBuilding.m_sec > 0 then
			pBuilding.m_sec = pBuilding.m_sec - 1;
		end
	end	
	
	-- 雇佣官员
	for i=0, 2, 1 do
		if GetPlayer().m_officialhire[i] ~= nil and GetPlayer().m_officialhire[i].m_ofsec > 0 then
			GetPlayer().m_officialhire[i].m_ofsec = GetPlayer().m_officialhire[i].m_ofsec - 1
		end
	end
	
	-- 自动洗练
	EquipWashDlgAuto()
	
	-- 记录上一次logic的时间
	GameManager.LastGameLogicTime = os.time();
	
	-- 心跳
	GameManager.HeartTime = GameManager.HeartTime + 1;
	if GameManager.HeartTime > 120 then
		local sendValue = {};
		sendValue.m_value = 0;
		netsend_heart_C( sendValue )
		GameManager.HeartTime = 0;
	end
	
	-- 可升级箭头
	GameManager.CityUpgradeTime = GameManager.CityUpgradeTime + 1
	if GameManager.CityUpgradeTime > 60 then
		City.UpgradeArrow()
		GameManager.CityUpgradeTime = 0
	end
end

-- 请求退出游戏
function GameManager.OnAskQuit()
   
    -- 没进游戏
    if Const.NetStatus < 3 then
        return;
    end
	
	-- 非战斗状态
	if g_fight.m_playing == false then
		-- 先关闭最后打开的界面
		if eye.uiManager:CloseLast() then
			return;
		end
	end
    -- 
    MsgBox( T( 518 ),function()
		GameManager.Restart();
		GameManager.Logout( 1 );
	end )
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
		notification_register_all();
	-- 从后台进入前台时
	else
		notification_unregister_all()
		
		local PauseTime = os.time() - GameManager.OnApplicationPauseTime;
		gamelog( "PauseTime:"..PauseTime );
		
		-- 进入前台后，检查主逻辑运转
		local gamelogicTime = os.time() - GameManager.LastGameLogicTime;
		gamelog( "gamelogicTime:"..gamelogicTime );
		for i=1, gamelogicTime, 1 do
			GameManager.GameLogic();
		end
		-- 检测是否断线
		if GameManager.OnApplicationPauseTime > 0 and PauseTime > 300 then
			
				if Const.NetStatus > 2 or GameManager.restart == true then
					Invoke( function() 
							GameManager.Restart()
							GameManager.Logout( 0 );
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

-- 改变场景
function GameManager.ChangeScence( scence )
	
	if scence == "city" then
		
		if GameManager.MainCity == nil then
			-- 加载主城
			LoadPrefabAsyn( "MainCity", function( obj )
				
				-- 缓存主城
				GameManager.MainCity = GameObject.Instantiate( obj );
				City.Init();

				-- 获取进入游戏所需信息
				local sendValue = {};
				sendValue.m_value = 0;
				netsend_entercity_C( sendValue );
						
			end );
			
		else
			GameManager.MainCity.gameObject:SetActive( true )

		end
		
		-- 隐藏世界地图
		if GameManager.WorldMap ~= nil then
			GameObject.Destroy( GameManager.WorldMap.gameObject );
			GameManager.WorldMap = nil;
		end
		
		-- 主城bgm
		CityPlayAudio()
		GameManager.currentScence = scence
		
	elseif scence == "worldmap" then
		if GameManager.WorldMap == nil then
		-- 加载地图
			LoadPrefabAsyn( "WorldMapRoot", function( obj )
			
				-- 缓存地图
				GameManager.WorldMap = GameObject.Instantiate( obj );
			
			end );
		
		else
			GameManager.WorldMap.gameObject:SetActive( true )
		end
		
		if GameManager.MainCity ~= nil then
			GameManager.MainCity.gameObject:SetActive( false )
		end
		
		-- 地图bgm
		MapPlayAudio()
		GameManager.currentScence = scence
		
	elseif scence == "fight" then
		if GameManager.MainCity ~= nil then
			GameManager.MainCity.gameObject:SetActive( false )
		end
		if GameManager.WorldMap ~= nil then
			GameManager.WorldMap.gameObject:SetActive( false )
		end
		-- 战斗bgm
		eye.audioManager:Play(202);
	
	elseif scence == "current" then
		GameManager.ChangeScence( GameManager.currentScence )
	end
end

