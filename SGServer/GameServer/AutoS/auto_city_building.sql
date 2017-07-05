CREATE TABLE `city_building` (
  `actorid` int(11) NOT NULL DEFAULT '0' COMMENT '',
  `offset` tinyint(3) NOT NULL DEFAULT '0' COMMENT '',
  `kind` tinyint(3) NOT NULL DEFAULT '0' COMMENT '建筑种类',
  `level` tinyint(3) NOT NULL DEFAULT '0' COMMENT '建筑等级',
  `sec` int(11) NOT NULL DEFAULT '0' COMMENT '时间',
  `value` int(11) NOT NULL DEFAULT '0' COMMENT '通用值',
  `overvalue` int(11) NOT NULL DEFAULT '0' COMMENT '完成的值',
  `quicksec` smallint(6) NOT NULL DEFAULT '0' COMMENT '加速时间',
  PRIMARY KEY (`actorid`)
  KEY (`offset`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
