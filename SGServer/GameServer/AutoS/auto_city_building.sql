CREATE TABLE `city_building` (
  `cityid` int(11) NOT NULL DEFAULT '0' COMMENT '',
  `offset` tinyint(4) NOT NULL DEFAULT '0' COMMENT '',
  `kind` tinyint(4) NOT NULL DEFAULT '0' COMMENT '建筑种类',
  `level` tinyint(4) NOT NULL DEFAULT '0' COMMENT '建筑等级',
  `official` tinyint(4) NOT NULL DEFAULT '0' COMMENT '正在使用官员级别',
  `officialfree` tinyint(4) NOT NULL DEFAULT '0' COMMENT '官员体验级别',
  `officialsec` int(11) NOT NULL DEFAULT '0' COMMENT '剩余时间',
  `value` int(11) NOT NULL DEFAULT '0' COMMENT '通用值',
  PRIMARY KEY (`cityid`)
  KEY (`offset`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
