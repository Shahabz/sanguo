
local notificationConfig = {
["SETTING_PUSH_TRAIN"] 				= {id=1,title=5901,trigger_time=0}, -- 募兵完成通知
["SETTING_PUSH_ENEMYATTACK"]		= {id=2,title=5902,trigger_time=0}, -- 被人攻打通知
["SETTING_PUSH_ENEMYDETECT"]		= {id=3,title=5903,trigger_time=0}, -- 被人侦查通知
["SETTING_PUSH_KINGWARSTART"]		= {id=4,title=5904,trigger_time=0}, -- 血战皇城开启通知
["SETTING_PUSH_BODYFULL"]      		= {id=5,title=5905,trigger_time=0}, -- 体力值满通知
["SETTING_PUSH_EQUIPCOMPLETE"] 		= {id=6,title=5906,trigger_time=0}, -- 装备打造完成通知
["SETTING_PUSH_TECHCOMPLETE"]		= {id=7,title=5907,trigger_time=0}, -- 科技研发完成通知
["SETTING_PUSH_BUILDCOMPLETE"]		= {id=8,title=5908,trigger_time=0}, -- 建筑升级完成通知
["SETTING_PUSH_MATERIALCOMPLETE"] 	= {id=9,title=5909,trigger_time=0}, -- 作坊生产完成通知
["SETTING_PUSH_HEROWASHFULL"] 		= {id=10,title=5910,trigger_time=0}, -- 洗髓次数满通知
["SETTING_PUSH_WISHING"] 			= {id=11,title=5911,trigger_time=0}, -- 聚宝盆冷却完成通知
["SETTING_PUSH_GATHERCOMPLETE"] 	= {id=12,title=5912,trigger_time=0}, -- 资源采集完成通知
["SETTING_PUSH_LEVYFULL"] 			= {id=13,title=5913,trigger_time=0}, -- 征收次数满通知
["SETTING_PUSH_GUARD"] 				= {id=14,title=5914,trigger_time=0}, -- 城防军冷却完成通知
}

-- 设置时间
function notification_set( name, title, trigger_time )
	if notificationConfig[name] == nil then
		return
	end
	notificationConfig[name].title = title;
	notificationConfig[name].trigger_time = trigger_time + os.time();
	--print( "notification_set:"..title.." trigger_time:"..trigger_time )
end

-- 切到后台和退出游戏调用
function notification_register_all()
	gamelog("notification_register_all")
	
	-- 体力值什么时候满(6分钟一点)
	local left = global.body_max - GetPlayer().m_body;
	local trigger_time = 0;
	if left > 0 then
		trigger_time = global.body_sec * left;
	end
	notification_set( "SETTING_PUSH_BODYFULL", T(5905), trigger_time )
		
	-- 资源采集完成
	--notification_set( "SETTING_PUSH_GATHERCOMPLETE", T(5912), )
	
	-- 征收次数已满
	notification_set( "SETTING_PUSH_LEVYFULL", T(5913), (global.levy_max-GetPlayer().m_levynum)*global.levy_sec )
	
	for k,v in pairs(notificationConfig) do
		local onORoff = GameManager.ini( k, "" )
		local trigger_time = v.trigger_time - os.time();
		if onORoff == "1" and trigger_time > 0 then
			NotificationManager.Instance:Register( v.id, v.title, trigger_time, false );
			gamelog( "notification_register_all:"..v.title.." trigger_time:"..trigger_time )
		end
	end
end

-- 切到前台调用	
function notification_unregister_all()
	for k,v in pairs(notificationConfig) do
		NotificationManager.Instance:Unregister( v.id );
	end
	NotificationManager.Instance:ClearAll ();
end
