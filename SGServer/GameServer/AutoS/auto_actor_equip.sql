CREATE TABLE `actor_equip` (
  `id` bigint(20) NOT NULL DEFAULT '0' COMMENT '主索引',
  `actorid` int(11) NOT NULL DEFAULT '0' COMMENT '角色ID',
  `offset` int(11) NOT NULL DEFAULT '0' COMMENT '装备所在背包位置',
  `kind` smallint(6) NOT NULL DEFAULT '0' COMMENT '物品种类',
  `washid0` smallint(6) NOT NULL DEFAULT '0' COMMENT '洗练属性ID',
  `washid1` smallint(6) NOT NULL DEFAULT '0' COMMENT '洗练属性ID',
  `washid2` smallint(6) NOT NULL DEFAULT '0' COMMENT '洗练属性ID',
  `washid3` smallint(6) NOT NULL DEFAULT '0' COMMENT '洗练属性ID',
  `washid4` smallint(6) NOT NULL DEFAULT '0' COMMENT '洗练属性ID',
  `washid5` smallint(6) NOT NULL DEFAULT '0' COMMENT '洗练属性ID',
  PRIMARY KEY (`id`)
  KEY (`actorid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
