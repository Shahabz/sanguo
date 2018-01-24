CREATE TABLE `city_nation_hero` (
  `actorid` int(11) NOT NULL DEFAULT '0' COMMENT '',
  `offset` tinyint(3) NOT NULL DEFAULT '0' COMMENT '',
  `herokind` tinyint(3) NOT NULL DEFAULT '0' COMMENT '名将英雄',
  `state` tinyint(3) NOT NULL DEFAULT '0' COMMENT '1关押,2已经劝降',
  `forever` tinyint(3) NOT NULL DEFAULT '0' COMMENT '是否永久拥有',
  `loyal` tinyint(3) NOT NULL DEFAULT '0' COMMENT '忠诚度',
  PRIMARY KEY (`actorid`)
  KEY (`offset`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
