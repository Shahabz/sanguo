CREATE TABLE `city_guard` (
  `actorid` int(11) NOT NULL DEFAULT '0' COMMENT '',
  `offset` tinyint(3) NOT NULL DEFAULT '0' COMMENT '',
  `shape` tinyint(3) NOT NULL DEFAULT '0' COMMENT '',
  `level` tinyint(3) NOT NULL DEFAULT '0' COMMENT '',
  `color` tinyint(3) NOT NULL DEFAULT '0' COMMENT '',
  `monsterid` smallint(6) NOT NULL DEFAULT '0' COMMENT '',
  `soldiers` int(11) NOT NULL DEFAULT '0' COMMENT '',
  PRIMARY KEY (`actorid`)
  KEY (`offset`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
