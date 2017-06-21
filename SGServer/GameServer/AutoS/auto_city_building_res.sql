CREATE TABLE `city_building_res` (
  `actorid` int(11) NOT NULL DEFAULT '0' COMMENT '',
  `offset` tinyint(3) NOT NULL DEFAULT '0' COMMENT '',
  `kind` tinyint(3) NOT NULL DEFAULT '0' COMMENT '资源建筑种类',
  `level` tinyint(3) NOT NULL DEFAULT '0' COMMENT '资源建筑等级',
  PRIMARY KEY (`actorid`)
  KEY (`offset`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
