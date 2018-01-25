CREATE TABLE `city_nation_hero` (
  `actorid` int(11) NOT NULL DEFAULT '0' COMMENT '',
  `offset` tinyint(3) NOT NULL DEFAULT '0' COMMENT '',
  `herokind` smallint(6) NOT NULL DEFAULT '0' COMMENT '名将英雄',
  `state` tinyint(3) NOT NULL DEFAULT '0' COMMENT '1关押,2已经劝降',
  `forever` tinyint(3) NOT NULL DEFAULT '0' COMMENT '是否永久拥有',
  `loyal` tinyint(3) NOT NULL DEFAULT '0' COMMENT '忠诚度',
  `buypos` tinyint(3) NOT NULL DEFAULT '0' COMMENT '购买坐标次数',
  `posx` smallint(6) NOT NULL DEFAULT '0' COMMENT '购买的坐标',
  `posy` smallint(6) NOT NULL DEFAULT '0' COMMENT '购买的坐标',
  PRIMARY KEY (`actorid`)
  KEY (`offset`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
