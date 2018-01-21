CREATE TABLE `map_call` (
  `index` tinyint(3) NOT NULL DEFAULT '0' COMMENT '召唤事件索引',
  `actorid` int(11) NOT NULL DEFAULT '0' COMMENT '属于玩家',
  `posx` smallint(6) NOT NULL DEFAULT '0' COMMENT '位置',
  `posy` smallint(6) NOT NULL DEFAULT '0' COMMENT '位置',
  `endtimestamp` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `num` tinyint(3) NOT NULL DEFAULT '0' COMMENT '当前召唤人数',
  `maxnum` tinyint(3) NOT NULL DEFAULT '0' COMMENT '召唤人数上限',
  `invite` tinyint(3) NOT NULL DEFAULT '0' COMMENT '邀请次数',
  PRIMARY KEY (`index`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
