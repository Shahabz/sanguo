CREATE TABLE `actor_girl` (
  `actorid` int(11) NOT NULL DEFAULT '0' COMMENT '角色',
  `kind` tinyint(3) NOT NULL DEFAULT '0' COMMENT '种类',
  `color` tinyint(3) NOT NULL DEFAULT '0' COMMENT '颜色',
  `sflag` tinyint(3) NOT NULL DEFAULT '0' COMMENT '标记',
  `love_level` tinyint(3) NOT NULL DEFAULT '0' COMMENT '亲密等级',
  `soul` smallint(6) NOT NULL DEFAULT '0' COMMENT '碎片',
  `love_exp` smallint(6) NOT NULL DEFAULT '0' COMMENT '亲密度',
  `love_today` smallint(6) NOT NULL DEFAULT '0' COMMENT '今天获取亲密度',
  `herokind` smallint(6) NOT NULL DEFAULT '0' COMMENT '关联男将',
  PRIMARY KEY (`actorid`)
  KEY (`kind`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
