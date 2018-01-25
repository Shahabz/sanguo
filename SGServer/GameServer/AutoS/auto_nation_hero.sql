CREATE TABLE `nation_hero` (
  `kind` smallint(6) NOT NULL DEFAULT '0' COMMENT '国家名将kind',
  `actorid` int(11) NOT NULL DEFAULT '0' COMMENT '当前所属玩家',
  `posx` smallint(6) NOT NULL DEFAULT '0' COMMENT '位置',
  `posy` smallint(6) NOT NULL DEFAULT '0' COMMENT '位置',
  `runsec` int(11) NOT NULL DEFAULT '0' COMMENT '逃跑倒计时',
  PRIMARY KEY (`kind`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
