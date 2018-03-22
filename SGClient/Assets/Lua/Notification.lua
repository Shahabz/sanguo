SETTING_PUSH_TRAIN         		= 1	-- 募兵完成通知
SETTING_PUSH_ENEMYATTACK		= 2 -- 被人攻打通知
SETTING_PUSH_ENEMYDETECT		= 3 -- 被人侦查通知
SETTING_PUSH_KINGWARSTART		= 4 -- 血战皇城开启通知
SETTING_PUSH_BODYFULL      		= 5 -- 体力值满通知
SETTING_PUSH_EQUIPCOMPLETE 		= 6 -- 装备打造完成通知
SETTING_PUSH_TECHCOMPLETE 		= 7 -- 科技研发完成通知
SETTING_PUSH_BUILDCOMPLETE 		= 8 -- 建筑升级完成通知
SETTING_PUSH_MATERIALCOMPLETE 	= 9 -- 作坊生产完成通知
SETTING_PUSH_HEROWASHFULL 		= 10 -- 洗髓次数满通知
SETTING_PUSH_WISHING 			= 11 -- 聚宝盆冷却完成通知
SETTING_PUSH_GATHERCOMPLETE 	= 12 -- 资源采集完成通知
SETTING_PUSH_LEVYFULL 			= 13 -- 征收次数满通知
SETTING_PUSH_GUARD 				= 14 -- 城防军冷却完成通知

-- 募兵完成
function push_train( triggerTime )
	NotificationManager.Instance:Register( SETTING_PUSH_TRAIN, T(5901), triggerTime, false );
end



