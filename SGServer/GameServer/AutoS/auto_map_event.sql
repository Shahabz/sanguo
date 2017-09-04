CREATE TABLE `map_event` (
  `index` int(11) NOT NULL DEFAULT '0' COMMENT '事件索引',
  `posx` smallint(6) NOT NULL DEFAULT '0' COMMENT '位置',
  `posy` smallint(6) NOT NULL DEFAULT '0' COMMENT '位置',
  `kind` smallint(6) NOT NULL DEFAULT '0' COMMENT '种类',
  `waitsec` int(11) NOT NULL DEFAULT '0' COMMENT '倒计时',
  `actorid` int(11) NOT NULL DEFAULT '0' COMMENT '属于角色',
  `state` tinyint(3) NOT NULL DEFAULT '0' COMMENT '状态',
  PRIMARY KEY (`index`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
