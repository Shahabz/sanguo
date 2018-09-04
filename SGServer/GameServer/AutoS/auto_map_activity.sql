CREATE TABLE `map_activity` (
  `index` int(11) NOT NULL DEFAULT '0' COMMENT '索引',
  `posx` smallint(6) NOT NULL DEFAULT '0' COMMENT '位置',
  `posy` smallint(6) NOT NULL DEFAULT '0' COMMENT '位置',
  `kind` smallint(6) NOT NULL DEFAULT '0' COMMENT '种类',
  `deltime` int(11) NOT NULL DEFAULT '0' COMMENT '删除时间',
  `actorid` int(11) NOT NULL DEFAULT '0' COMMENT '属于角色',
  `hp` int(11) NOT NULL DEFAULT '0' COMMENT '剩余血量',
  `maxhp` int(11) NOT NULL DEFAULT '0' COMMENT '血量',
  `activityid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '所属活动',
  PRIMARY KEY (`index`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
