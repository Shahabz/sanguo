CREATE TABLE `city_building` (
  `cityid` int(11) NOT NULL DEFAULT '0' COMMENT '',
  `offset` tinyint(4) NOT NULL DEFAULT '0' COMMENT '',
  `kind` tinyint(4) NOT NULL DEFAULT '0' COMMENT '资源建筑种类',
  `level` tinyint(4) NOT NULL DEFAULT '0' COMMENT '资源建筑等级',
  `finish` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `official` tinyint(4) NOT NULL DEFAULT '0' COMMENT '官员级别',
  `value` int(11) NOT NULL DEFAULT '0' COMMENT '通用值',
  PRIMARY KEY (`cityid`)
  KEY (`offset`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
