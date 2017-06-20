CREATE TABLE `city_building_res` (
  `cityid` int(11) NOT NULL DEFAULT '0' COMMENT '',
  `offset` tinyint(4) NOT NULL DEFAULT '0' COMMENT '',
  `kind` tinyint(4) NOT NULL DEFAULT '0' COMMENT '资源建筑种类',
  `level` tinyint(4) NOT NULL DEFAULT '0' COMMENT '资源建筑等级',
  PRIMARY KEY (`cityid`)
  KEY (`offset`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
