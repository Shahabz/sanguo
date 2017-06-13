CREATE TABLE `actor_item` (
  `itemid` bigint(20) NOT NULL DEFAULT '0' COMMENT '物品id',
  `actorid` int(11) NOT NULL DEFAULT '0' COMMENT '角色ID',
  `offset` smallint(6) NOT NULL DEFAULT '0' COMMENT '物品所在位置',
  `kind` int(11) NOT NULL DEFAULT '0' COMMENT '物品种类',
  `num` smallint(6) NOT NULL DEFAULT '0' COMMENT '物品个数',
  `ability0` smallint(6) NOT NULL DEFAULT '0' COMMENT '属性',
  `ability1` smallint(6) NOT NULL DEFAULT '0' COMMENT '属性',
  `ability2` smallint(6) NOT NULL DEFAULT '0' COMMENT '属性',
  `ability3` smallint(6) NOT NULL DEFAULT '0' COMMENT '属性',
  `value0` int(11) NOT NULL DEFAULT '0' COMMENT '属性值',
  `value1` int(11) NOT NULL DEFAULT '0' COMMENT '属性值',
  `value2` int(11) NOT NULL DEFAULT '0' COMMENT '属性值',
  `value3` int(11) NOT NULL DEFAULT '0' COMMENT '属性值',
  `color_level` tinyint(4) NOT NULL DEFAULT '0' COMMENT '颜色等级',
  PRIMARY KEY (`itemid`)
  KEY (`actorid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
