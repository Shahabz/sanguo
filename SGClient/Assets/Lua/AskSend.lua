-- 通用短消息发送
ASKINFO_NORMAL		= 	0
ASKINFO_CHANGESHAPE	=	1 -- 修改头像
ASKINFO_CHANGENAME	=	2 -- 修改名字
ASKINFO_CITYGUARD	=	3 -- 城墙守卫
ASKINFO_QUEST		=	4 -- 任务
ASKINFO_BUILDING	=	5 -- 建筑
ASKINFO_LEVY		=	6 -- 征收
ASKINFO_TRAIN		=	7 -- 训练
ASKINFO_EQUIPFORGING	=	8  -- 装备打造
ASKINFO_EQUIPWASH		=	9  -- 装备洗炼
ASKINFO_EQUIP			=	10 -- 装备
ASKINFO_ITEM			=	11 -- 道具  
ASKINFO_HERO			=	12 -- 英雄
ASKINFO_TECH			=	13 -- 科技
ASKINFO_GOV				=	14 -- 官府 
ASKINFO_OFFICIALHIRE	=	15 -- 官职雇佣
ASKINFO_QUICK			=	16 -- 加速
ASKINFO_ACTOR			=	17 -- 角色操作
ASKINFO_MATERIALMAKE	=	18 -- 材料制造
ASKINFO_STORY			=	19 -- 副本
ASKINFO_WORLDMAP		=	20 -- 世界地图
ASKINFO_MAP_EVENT		=	21 -- 世界地图随机事件
ASKINFO_SPY_CITY		=	22 -- 侦察城池
ASKINFO_CITYHELP		=	23 -- 驻防信息
ASKINFO_CITYARMYGROUP	=	24 -- 城战
ASKINFO_NATIONARMYGROUP	=	25 -- 国战
ASKINFO_MAPTOWN			=	26 -- 城镇
ASKINFO_MAPZONE			=	27 -- 地区
ASKINFO_LOSTREBUILD		=	28 -- 家园重建
ASKINFO_KINGWAR			=	29 -- 皇城血战 
ASKINFO_MSGBOX_CALLBACK =	30 -- 消息框点击确定回调
ASKINFO_AUTOBUILD		=	31 -- 自动建造
ASKINFO_HERO_VISIT		=	32 -- 武将寻访
ASKINFO_CHANGESIGN		=	33 -- 个性签名
ASKINFO_SHOP			=	34 -- 商店
ASKINFO_PAY				=	35 -- 支付
ASKINFO_VIPSHOP			=	36 -- VIP商店
ASKINFO_VIPBAG			=	37 -- VIP礼包
ASKINFO_ACTIVITY		=	38 -- 活动
ASKINFO_NATIONEQUIP		=	39 -- 国器
ASKINFO_NATION			=	40 -- 国家
ASKINFO_NATIONLOG		=	41 -- 国家日志
ASKINFO_NATIONHERO		=   42 -- 国家名将
ASKINFO_WISHING			=	43 -- 聚宝盆
ASKINFO_TIANCEQUEST		=	44 -- 天策府任务

function system_askinfo( msgid, msg, ... )
	-- 未进入游戏不允许发送通用短协议
	if Const.NetStatus < 3 then
		return
	end
	-- m_msgid=0,m_valuenum=0,m_value={}[m_valuenum],m_msg_length=0,m_msg="[m_msg_length]",
	local arg={...}
	local sendValue = {};
	sendValue.m_msgid = msgid;
	sendValue.m_msg = msg;
	sendValue.m_msg_length = string.len( sendValue.m_msg );
	sendValue.m_valuenum = #arg;
	sendValue.m_value={};
	for i,v in ipairs(arg) do
		sendValue.m_value[i] = v;
   	end	
	netsend_askinfo_C( sendValue );
end



-- 订阅协议类型
-- 订阅一个消息，服务器只给订阅的用户发送消息
function SubscribeCmd( cmd )
	system_askinfo( ASKINFO_SUBSCRIBE, "", 1, cmd );
end

-- 取消订阅一个消息
function SubscribeCmdCancel( cmd )
	system_askinfo( ASKINFO_SUBSCRIBE, "", 2, cmd );
end
