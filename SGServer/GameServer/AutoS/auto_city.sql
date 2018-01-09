CREATE TABLE `city` (
  `actorid` int(11) NOT NULL DEFAULT '0' COMMENT '角色ID',
  `index` int(11) NOT NULL DEFAULT '0' COMMENT '索引',
  `name` varbinary(22) NOT NULL DEFAULT '' COMMENT '名称',
  `type` tinyint(3) NOT NULL DEFAULT '0' COMMENT '城池类型',
  `shape` tinyint(3) NOT NULL DEFAULT '0' COMMENT '领主形象(选取)',
  `headid` int(11) NOT NULL DEFAULT '0' COMMENT '领主头像(自定义)',
  `country` smallint(6) NOT NULL DEFAULT '0' COMMENT '选择的国家(真实)',
  `ipcountry` varbinary(3) NOT NULL DEFAULT '' COMMENT 'IP地址所在国家',
  `language` tinyint(3) NOT NULL DEFAULT '0' COMMENT '选择语言',
  `os` tinyint(3) NOT NULL DEFAULT '0' COMMENT '系统1ios 2android 3win',
  `platid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '平台',
  `createtime` int(11) NOT NULL DEFAULT '0' COMMENT '创建时间',
  `lastlogin` int(11) NOT NULL DEFAULT '0' COMMENT '上次登陆时间',
  `lastlogout` int(11) NOT NULL DEFAULT '0' COMMENT '上次登出时间',
  `state` tinyint(3) NOT NULL DEFAULT '0' COMMENT '城池当前状态',
  `posx` smallint(6) NOT NULL DEFAULT '0' COMMENT '位置',
  `posy` smallint(6) NOT NULL DEFAULT '0' COMMENT '位置',
  `sflag` int(11) NOT NULL DEFAULT '0' COMMENT '标志位',
  `level` smallint(6) NOT NULL DEFAULT '0' COMMENT '玩家等级',
  `exp` int(11) NOT NULL DEFAULT '0' COMMENT '玩家经验',
  `viplevel` tinyint(3) NOT NULL DEFAULT '0' COMMENT 'VIP等级',
  `vipexp` int(11) NOT NULL DEFAULT '0' COMMENT 'VIP经验',
  `nation` tinyint(3) NOT NULL DEFAULT '0' COMMENT '国家(魏蜀吴)',
  `official` tinyint(3) NOT NULL DEFAULT '0' COMMENT '官职',
  `place` tinyint(3) NOT NULL DEFAULT '0' COMMENT '爵位',
  `zone` tinyint(3) NOT NULL DEFAULT '0' COMMENT '区域',
  `battlepower` int(11) NOT NULL DEFAULT '0' COMMENT '战力',
  `mokilllv` tinyint(3) NOT NULL DEFAULT '0' COMMENT '击杀野怪最高级别',
  `body` smallint(6) NOT NULL DEFAULT '0' COMMENT '体力',
  `bodysec` smallint(6) NOT NULL DEFAULT '0' COMMENT '体力恢复时间(每秒-1)',
  `silver` int(11) NOT NULL DEFAULT '0' COMMENT '银币',
  `wood` int(11) NOT NULL DEFAULT '0' COMMENT '木材',
  `food` int(11) NOT NULL DEFAULT '0' COMMENT '粮草',
  `iron` int(11) NOT NULL DEFAULT '0' COMMENT '镔铁',
  `levynum` tinyint(3) NOT NULL DEFAULT '0' COMMENT '当前征收次数',
  `levysec` smallint(6) NOT NULL DEFAULT '0' COMMENT '下次征收时间(每秒-1)',
  `people` int(11) NOT NULL DEFAULT '0' COMMENT '人口',
  `prestige` int(11) NOT NULL DEFAULT '0' COMMENT '威望值',
  `friendship` int(11) NOT NULL DEFAULT '0' COMMENT '友谊积分',
  `function` int(11) NOT NULL DEFAULT '0' COMMENT '功能是否开启',
  `equip_washnum` tinyint(3) NOT NULL DEFAULT '0' COMMENT '装备洗练免费次数',
  `equip_washsec` smallint(6) NOT NULL DEFAULT '0' COMMENT '装备下次免费洗练时间',
  `forgingkind` smallint(6) NOT NULL DEFAULT '0' COMMENT '打造装备种类',
  `forgingsec` int(11) NOT NULL DEFAULT '0' COMMENT '打造剩余时间',
  `hero_washnum` tinyint(3) NOT NULL DEFAULT '0' COMMENT '英雄洗练次数',
  `hero_washsec` smallint(6) NOT NULL DEFAULT '0' COMMENT '英雄下次洗练免费时间',
  `autobuild` tinyint(3) NOT NULL DEFAULT '0' COMMENT '自动建造剩余次数',
  `autobuildopen` tinyint(3) NOT NULL DEFAULT '0' COMMENT '自动建造是否开启',
  `ptsec` int(11) NOT NULL DEFAULT '0' COMMENT '城池保护状态剩余时间',
  `peoplesec` smallint(6) NOT NULL DEFAULT '0' COMMENT '城池人口回复时间',
  `eventsec` int(11) NOT NULL DEFAULT '0' COMMENT '事件倒计时',
  `questid` varbinary(64) NOT NULL DEFAULT '' COMMENT '任务ID',
  `questvalue` varbinary(64) NOT NULL DEFAULT '' COMMENT '任务数值',
  `techlevel` varbinary(160) NOT NULL DEFAULT '' COMMENT '科技',
  `techprogress` varbinary(160) NOT NULL DEFAULT '' COMMENT '科技进度',
  `data_record` varbinary(64) NOT NULL DEFAULT '' COMMENT '数据记录',
  `matkind` varbinary(32) NOT NULL DEFAULT '' COMMENT '材料生产kind',
  `matnum` varbinary(32) NOT NULL DEFAULT '' COMMENT '材料生产数量',
  `matsec` varbinary(32) NOT NULL DEFAULT '' COMMENT '材料已经生产时间',
  `matquenum` tinyint(3) NOT NULL DEFAULT '0' COMMENT '材料生产队列数量',
  `matkind_will` varbinary(32) NOT NULL DEFAULT '' COMMENT '材料预定生产kind',
  `matnum_will` varbinary(32) NOT NULL DEFAULT '' COMMENT '材料预定生产数量',
  `matkind_over` varbinary(32) NOT NULL DEFAULT '' COMMENT '材料生产完毕',
  `matnum_over` varbinary(32) NOT NULL DEFAULT '' COMMENT '材料生产完毕',
  `hv_fcd` int(11) NOT NULL DEFAULT '0' COMMENT '良将寻访免费CD',
  `hv_hsec` int(11) NOT NULL DEFAULT '0' COMMENT '神将寻访解锁时长',
  `hv_hf` tinyint(3) NOT NULL DEFAULT '0' COMMENT '神将寻访免费次数',
  `kw_co` tinyint(3) NOT NULL DEFAULT '0' COMMENT '恭贺是否完毕',
  `kw_px` smallint(6) NOT NULL DEFAULT '0' COMMENT '挖宝坐标',
  `kw_py` smallint(6) NOT NULL DEFAULT '0' COMMENT '挖宝坐标',
  `kw_tn` tinyint(3) NOT NULL DEFAULT '0' COMMENT '挖宝第几次',
  `kw_has` tinyint(3) NOT NULL DEFAULT '0' COMMENT '挖宝已经挖到图纸',
  `kw_totalkill` int(11) NOT NULL DEFAULT '0' COMMENT '本次血战累杀',
  `kw_point` int(11) NOT NULL DEFAULT '0' COMMENT '血战总击杀',
  `rb_num` smallint(6) NOT NULL DEFAULT '0' COMMENT '重建家园次数',
  `rb_silver` int(11) NOT NULL DEFAULT '0' COMMENT '给予的重建家园资源',
  `rb_wood` int(11) NOT NULL DEFAULT '0' COMMENT '给予的重建家园资源',
  `rb_food` int(11) NOT NULL DEFAULT '0' COMMENT '给予的重建家园资源',
  `rb_df` smallint(6) NOT NULL DEFAULT '0' COMMENT '给予的重建家园资源',
  `neq_lv` varbinary(24) NOT NULL DEFAULT '' COMMENT '国器等级',
  `neq_exp` varbinary(24) NOT NULL DEFAULT '' COMMENT '国器经验',
  `neq_pro` varbinary(24) NOT NULL DEFAULT '' COMMENT '国器改造的进度次数',
  `neq_sec` varbinary(24) NOT NULL DEFAULT '' COMMENT '国器升级或改造时间',
  `neq_star` varbinary(24) NOT NULL DEFAULT '' COMMENT '国器改造星级',
  `guardsec` smallint(6) NOT NULL DEFAULT '0' COMMENT '城墙守卫冷却时间',
  `worker_op` tinyint(3) NOT NULL DEFAULT '0' COMMENT '建造队列操作',
  `worker_sec` int(11) NOT NULL DEFAULT '0' COMMENT '建造剩余时间(每秒-1)',
  `worker_kind` tinyint(3) NOT NULL DEFAULT '0' COMMENT '当前升级建筑类型',
  `worker_offset` tinyint(3) NOT NULL DEFAULT '0' COMMENT '当前升级建筑索引',
  `worker_free` tinyint(3) NOT NULL DEFAULT '0' COMMENT '是否使用过免费',
  `worker_op_ex` tinyint(3) NOT NULL DEFAULT '0' COMMENT '建造队列操作(商用)',
  `worker_sec_ex` int(11) NOT NULL DEFAULT '0' COMMENT '建造剩余时间(每秒-1)(商用)',
  `worker_kind_ex` tinyint(3) NOT NULL DEFAULT '0' COMMENT '当前升级建筑类型(商用)',
  `worker_offset_ex` tinyint(3) NOT NULL DEFAULT '0' COMMENT '当前升级建筑索引(商用)',
  `worker_free_ex` tinyint(3) NOT NULL DEFAULT '0' COMMENT '是否使用过免费',
  `worker_expire_ex` int(11) NOT NULL DEFAULT '0' COMMENT '商用建造队列到期时间',
  `wnsec` int(11) NOT NULL DEFAULT '0' COMMENT '建造队列所需时间',
  `wnsec_ex` int(11) NOT NULL DEFAULT '0' COMMENT '建造队列所需时间',
  `wnquick` int(11) NOT NULL DEFAULT '0' COMMENT '建造队列奖励的加速',
  `wnquick_ex` int(11) NOT NULL DEFAULT '0' COMMENT '建造队列奖励的加速',
  `ofkind0` smallint(6) NOT NULL DEFAULT '0' COMMENT '建筑官员种类',
  `ofkind1` smallint(6) NOT NULL DEFAULT '0' COMMENT '建筑官员种类',
  `ofkind2` smallint(6) NOT NULL DEFAULT '0' COMMENT '建筑官员种类',
  `ofsec0` int(11) NOT NULL DEFAULT '0' COMMENT '建筑官员剩余秒',
  `ofsec1` int(11) NOT NULL DEFAULT '0' COMMENT '建筑官员剩余秒',
  `ofsec2` int(11) NOT NULL DEFAULT '0' COMMENT '建筑官员剩余秒',
  `offree0` int(11) NOT NULL DEFAULT '0' COMMENT '建筑官员免费情况',
  `offree1` int(11) NOT NULL DEFAULT '0' COMMENT '建筑官员免费情况',
  `offree2` int(11) NOT NULL DEFAULT '0' COMMENT '建筑官员免费情况',
  `ofquick0` int(11) NOT NULL DEFAULT '0' COMMENT '建筑官员已经使用的加速时间',
  `ofquick1` int(11) NOT NULL DEFAULT '0' COMMENT '建筑官员已经使用的加速时间',
  `ofquick2` int(11) NOT NULL DEFAULT '0' COMMENT '建筑官员已经使用的加速时间',
  `signature` varbinary(128) NOT NULL DEFAULT '' COMMENT '个性签名',
  `wcard` smallint(6) NOT NULL DEFAULT '0' COMMENT '周卡天数',
  `mcard` smallint(6) NOT NULL DEFAULT '0' COMMENT '月卡天数',
  `bag_gid` varbinary(32) NOT NULL DEFAULT '' COMMENT '推送的礼包',
  `bag_time` varbinary(32) NOT NULL DEFAULT '' COMMENT '推送的礼包剩余时间',
  `bag_num` varbinary(32) NOT NULL DEFAULT '' COMMENT '推送的礼包剩余次数',
  `buffsec` varbinary(32) NOT NULL DEFAULT '' COMMENT 'buff',
  `atgu` tinyint(3) NOT NULL DEFAULT '0' COMMENT '城防补充次数',
  `atgu_op` tinyint(3) NOT NULL DEFAULT '0' COMMENT '城防补充是否开启',
  PRIMARY KEY (`actorid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
