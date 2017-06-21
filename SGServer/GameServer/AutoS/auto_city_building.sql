CREATE TABLE `city_building` (
  `actorid` int(11) NOT NULL DEFAULT '0' COMMENT '',
  `offset` tinyint(3) NOT NULL DEFAULT '0' COMMENT '',
  `kind` tinyint(3) NOT NULL DEFAULT '0' COMMENT '建筑种类',
  `level` tinyint(3) NOT NULL DEFAULT '0' COMMENT '建筑等级',
  `officialkind` tinyint(3) NOT NULL DEFAULT '0' COMMENT '正在使用官员',
  `officialfree` int(11) NOT NULL DEFAULT '0' COMMENT '是否使用过免费(按位)',
  `officialsec` int(11) NOT NULL DEFAULT '0' COMMENT '官员剩余时间',
  `value` int(11) NOT NULL DEFAULT '0' COMMENT '通用值',
  `sec` int(11) NOT NULL DEFAULT '0' COMMENT '时间',
  `quicksec` smallint(6) NOT NULL DEFAULT '0' COMMENT '加速时间',
  PRIMARY KEY (`actorid`)
  KEY (`offset`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
