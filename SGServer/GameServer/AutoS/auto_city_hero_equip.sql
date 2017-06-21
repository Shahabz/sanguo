CREATE TABLE `city_hero_equip` (
  `actorid` int(11) NOT NULL DEFAULT '0' COMMENT '所属城池',
  `herokind` smallint(6) NOT NULL DEFAULT '0' COMMENT '所属英雄',
  `offset` smallint(6) NOT NULL DEFAULT '0' COMMENT '物品所在位置',
  `kind` int(11) NOT NULL DEFAULT '0' COMMENT '物品种类',
  `ability0` smallint(6) NOT NULL DEFAULT '0' COMMENT '属性',
  `ability1` smallint(6) NOT NULL DEFAULT '0' COMMENT '属性',
  `ability2` smallint(6) NOT NULL DEFAULT '0' COMMENT '属性',
  `ability3` smallint(6) NOT NULL DEFAULT '0' COMMENT '属性',
  `value0` int(11) NOT NULL DEFAULT '0' COMMENT '属性值',
  `value1` int(11) NOT NULL DEFAULT '0' COMMENT '属性值',
  `value2` int(11) NOT NULL DEFAULT '0' COMMENT '属性值',
  `value3` int(11) NOT NULL DEFAULT '0' COMMENT '属性值',
  PRIMARY KEY (`actorid`)
  KEY (`herokind`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
