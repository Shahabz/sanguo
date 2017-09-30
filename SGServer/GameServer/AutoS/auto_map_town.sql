CREATE TABLE `map_town` (
  `townid` int(11) NOT NULL DEFAULT '0' COMMENT '城镇id',
  `nation` tinyint(3) NOT NULL DEFAULT '0' COMMENT '所属',
  `name` varbinary(22) NOT NULL DEFAULT '' COMMENT '修改后的名称',
  `protect_sec` int(11) NOT NULL DEFAULT '0' COMMENT '保护时间',
  `produce_num` smallint(6) NOT NULL DEFAULT '0' COMMENT '征收次数',
  `produce_sec` int(11) NOT NULL DEFAULT '0' COMMENT '生产时间',
  `own_actorid` int(11) NOT NULL DEFAULT '0' COMMENT '城主角色id',
  `own_sec` int(11) NOT NULL DEFAULT '0' COMMENT '城主任期倒计时',
  `join_actorid` varbinary(512) NOT NULL DEFAULT '' COMMENT '参战人员',
  `ask_actorid` varbinary(512) NOT NULL DEFAULT '' COMMENT '申请人员',
  `soldier` varbinary(64) NOT NULL DEFAULT '' COMMENT '守卫血量存档',
  `soldier_update` tinyint(3) NOT NULL DEFAULT '0' COMMENT '守卫血量存档更新',
  PRIMARY KEY (`townid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
