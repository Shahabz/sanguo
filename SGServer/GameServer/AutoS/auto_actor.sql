CREATE TABLE `actor` (
  `actorid` int(11) NOT NULL DEFAULT '0' COMMENT '角色编号',
  `userid` bigint(20) NOT NULL DEFAULT '0' COMMENT '账号编号',
  `username` varbinary(51) NOT NULL DEFAULT '' COMMENT '账号名',
  `admin` smallint(6) NOT NULL DEFAULT '0' COMMENT '管理员级别',
  `lastip` varbinary(16) NOT NULL DEFAULT '' COMMENT '上次登陆ip',
  `createtime` int(11) NOT NULL DEFAULT '0' COMMENT '创建时间',
  `forbidtime` int(11) NOT NULL DEFAULT '0' COMMENT '禁言时间',
  `name` varbinary(22) NOT NULL DEFAULT '' COMMENT '角色名称',
  `nation` tinyint(3) NOT NULL DEFAULT '0' COMMENT '角色国家',
  `shape` tinyint(3) NOT NULL DEFAULT '0' COMMENT '形象',
  `level` smallint(6) NOT NULL DEFAULT '0' COMMENT '级别',
  `token` int(11) NOT NULL DEFAULT '0' COMMENT '钻石',
  `total_charge` int(11) NOT NULL DEFAULT '0' COMMENT '总充值',
  `charge_dollar` double NOT NULL DEFAULT '0' COMMENT '总消费多少钱',
  `itemext` tinyint(3) NOT NULL DEFAULT '0' COMMENT '扩展的背包栏位数',
  `equipext` tinyint(3) NOT NULL DEFAULT '0' COMMENT '扩展的装备栏位数',
  `quest_talkid` int(11) NOT NULL DEFAULT '0' COMMENT '任务对话',
  `worldquest_complete` int(11) NOT NULL DEFAULT '0' COMMENT '世界任务领取情况',
  `blacklist` varbinary(200) NOT NULL DEFAULT '' COMMENT '黑名单',
  `sflag` int(11) NOT NULL DEFAULT '0' COMMENT '特定标志位',
  `fdate` int(11) NOT NULL DEFAULT '0' COMMENT '刷新的日子是哪天',
  `today_char` varbinary(512) NOT NULL DEFAULT '' COMMENT 'char类型每日次数',
  `today_short` varbinary(128) NOT NULL DEFAULT '' COMMENT 'short类型每日次数',
  `today_int0` int(11) NOT NULL DEFAULT '0' COMMENT 'int类型每日次数',
  `today_int1` int(11) NOT NULL DEFAULT '0' COMMENT 'int类型每日次数',
  `today_int2` int(11) NOT NULL DEFAULT '0' COMMENT 'int类型每日次数',
  `today_int3` int(11) NOT NULL DEFAULT '0' COMMENT 'int类型每日次数',
  `today_int4` int(11) NOT NULL DEFAULT '0' COMMENT 'int类型每日次数',
  `today_int5` int(11) NOT NULL DEFAULT '0' COMMENT 'int类型每日次数',
  `today_int6` int(11) NOT NULL DEFAULT '0' COMMENT 'int类型每日次数',
  `today_int7` int(11) NOT NULL DEFAULT '0' COMMENT 'int类型每日次数',
  `cd0` int(11) NOT NULL DEFAULT '0' COMMENT '通用CD时间',
  `cd1` int(11) NOT NULL DEFAULT '0' COMMENT '通用CD时间',
  `cd2` int(11) NOT NULL DEFAULT '0' COMMENT '通用CD时间',
  `cd3` int(11) NOT NULL DEFAULT '0' COMMENT '通用CD时间',
  `cd4` int(11) NOT NULL DEFAULT '0' COMMENT '通用CD时间',
  `cd5` int(11) NOT NULL DEFAULT '0' COMMENT '通用CD时间',
  `cd6` int(11) NOT NULL DEFAULT '0' COMMENT '通用CD时间',
  `cd7` int(11) NOT NULL DEFAULT '0' COMMENT '通用CD时间',
  `config` varbinary(32) NOT NULL DEFAULT '' COMMENT '玩家配置',
  `permission` int(11) NOT NULL DEFAULT '0' COMMENT '权限是否开启',
  `storyid` smallint(6) NOT NULL DEFAULT '0' COMMENT '副本进度章节',
  `story_star` varbinary(512) NOT NULL DEFAULT '' COMMENT '精英副本星级',
  `story_hero` varbinary(128) NOT NULL DEFAULT '' COMMENT '招募副本',
  `story_resnum` varbinary(128) NOT NULL DEFAULT '' COMMENT '资源副本次数',
  `story_restime` varbinary(128) NOT NULL DEFAULT '' COMMENT '资源副本时间',
  `story_resreset` varbinary(128) NOT NULL DEFAULT '' COMMENT '资源副本重置次数',
  `story_itemnum` varbinary(256) NOT NULL DEFAULT '' COMMENT '道具副本掉落次数',
  `story_drawing` varbinary(64) NOT NULL DEFAULT '' COMMENT '装备图纸副本是否购买',
  `story_sweephero` varbinary(16) NOT NULL DEFAULT '' COMMENT '扫荡武将',
  `lastpeople_c` int(11) NOT NULL DEFAULT '0' COMMENT '上次本城人口',
  `lastpeople_n` int(11) NOT NULL DEFAULT '0' COMMENT '上次国家人口',
  `cdkey1` int(11) NOT NULL DEFAULT '0' COMMENT '是否领取该批次兑换码',
  `cdkey2` int(11) NOT NULL DEFAULT '0' COMMENT '是否领取该批次兑换码',
  `cdkey3` int(11) NOT NULL DEFAULT '0' COMMENT '是否领取该批次兑换码',
  `cdkey4` int(11) NOT NULL DEFAULT '0' COMMENT '是否领取该批次兑换码',
  `hv_ln` tinyint(3) NOT NULL DEFAULT '0' COMMENT '良将寻访次数',
  `hv_hn` tinyint(3) NOT NULL DEFAULT '0' COMMENT '神将寻访次数',
  `hv_pro` smallint(6) NOT NULL DEFAULT '0' COMMENT '当前寻访进度',
  `vipbag` int(11) NOT NULL DEFAULT '0' COMMENT 'vip礼包是否购买',
  `shop_useitem` tinyint(3) NOT NULL DEFAULT '0' COMMENT '商店是否优先使用道具购买符',
  `charge_point` int(11) NOT NULL DEFAULT '0' COMMENT '充值积分',
  `pay_maxtier` smallint(6) NOT NULL DEFAULT '0' COMMENT '单笔付费最大级别',
  `shop_saleitem` varbinary(24) NOT NULL DEFAULT '' COMMENT '商店打折物品',
  `neq_crit` varbinary(24) NOT NULL DEFAULT '' COMMENT '国器暴击状态',
  `mapcallfree` tinyint(3) NOT NULL DEFAULT '0' COMMENT '免费召唤次数',
  `wishingid` varbinary(36) NOT NULL DEFAULT '' COMMENT '聚宝盆道具列表',
  `wishingopen` varbinary(36) NOT NULL DEFAULT '' COMMENT '聚宝盆道具开启',
  `wishingday` int(11) NOT NULL DEFAULT '0' COMMENT '聚宝盆道具哪天的',
  `wishingcd` int(11) NOT NULL DEFAULT '0' COMMENT '聚宝盆寻宝冷却',
  `shape_bag` int(11) NOT NULL DEFAULT '0' COMMENT '形象背包',
  `student` varbinary(120) NOT NULL DEFAULT '' COMMENT '徒弟',
  `te_award` varbinary(40) NOT NULL DEFAULT '' COMMENT '师徒奖励可领取次数',
  `te_awarded` varbinary(40) NOT NULL DEFAULT '' COMMENT '师徒奖励已经领取次数',
  `te_shop` int(11) NOT NULL DEFAULT '0' COMMENT '师徒商店',
  `act02_state` int(11) NOT NULL DEFAULT '0' COMMENT '主城等级活动领取奖励',
  `act03_state` tinyint(3) NOT NULL DEFAULT '0' COMMENT '攻城掠地活动领取奖励',
  `act04_state` varbinary(512) NOT NULL DEFAULT '' COMMENT '七日狂欢活动状态',
  `act04_value` varbinary(160) NOT NULL DEFAULT '' COMMENT '七日狂欢活动数值',
  `act05_item` varbinary(24) NOT NULL DEFAULT '' COMMENT '七星拜将活动-道具列表',
  `act05_isbuy` tinyint(3) NOT NULL DEFAULT '0' COMMENT '七星拜将活动-道具是否购买',
  `act05_buynum` tinyint(3) NOT NULL DEFAULT '0' COMMENT '七星拜将活动-道具购买次数',
  `act05_xw` tinyint(3) NOT NULL DEFAULT '0' COMMENT '七星拜将活动-信物',
  `act05_upnum` tinyint(3) NOT NULL DEFAULT '0' COMMENT '七星拜将活动-剩余刷新次数',
  `act05_upstamp` int(11) NOT NULL DEFAULT '0' COMMENT '七星拜将活动-刷新时间',
  `act08_state` smallint(6) NOT NULL DEFAULT '0' COMMENT '成长计划活动领取奖励',
  `fs_nodeid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '坊市节点',
  `fs_awardkind` varbinary(56) NOT NULL DEFAULT '' COMMENT '坊市寻访奖励',
  `fs_awardnum` varbinary(56) NOT NULL DEFAULT '' COMMENT '坊市寻访奖励',
  `fs_awardfday` int(11) NOT NULL DEFAULT '0' COMMENT '坊市寻访奖励日期',
  `fs_weekkind` varbinary(20) NOT NULL DEFAULT '' COMMENT '皇宫内院奖励',
  `fs_weeknum` varbinary(20) NOT NULL DEFAULT '' COMMENT '皇宫内院奖励',
  `fs_fweek` int(11) NOT NULL DEFAULT '0' COMMENT '坊市周数',
  `girlshop` varbinary(36) NOT NULL DEFAULT '' COMMENT '女将商店',
  `girlshop_buy` smallint(6) NOT NULL DEFAULT '0' COMMENT '女将商店是否购买',
  `girlshop_fday` int(11) NOT NULL DEFAULT '0' COMMENT '女将商店刷新天数',
  `maidname` varbinary(22) NOT NULL DEFAULT '' COMMENT '侍女名称',
  `token_sale` int(11) NOT NULL DEFAULT '0' COMMENT '元宝抵扣点数',
  `token_ret` int(11) NOT NULL DEFAULT '0' COMMENT '返利次数',
  PRIMARY KEY (`actorid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
