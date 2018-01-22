CREATE TABLE `map_call` (
  `index` tinyint(3) NOT NULL DEFAULT '0' COMMENT '召唤事件索引',
  `actorid` int(11) NOT NULL DEFAULT '0' COMMENT '属于玩家',
  `limitlevel` smallint(6) NOT NULL DEFAULT '0' COMMENT '绛夌骇闄愬埗',
  `endtimestamp` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `num` tinyint(3) NOT NULL DEFAULT '0' COMMENT '当前召唤人数',
  `maxnum` tinyint(3) NOT NULL DEFAULT '0' COMMENT '召唤人数上限',
  PRIMARY KEY (`index`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
