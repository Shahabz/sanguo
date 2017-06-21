CREATE TABLE `actor_hero` (
  `id` bigint(20) NOT NULL DEFAULT '0' COMMENT '主索引',
  `actorid` int(11) NOT NULL DEFAULT '0' COMMENT '城池',
  `offset` int(11) NOT NULL DEFAULT '0' COMMENT '索引',
  `kind` smallint(6) NOT NULL DEFAULT '0' COMMENT '英雄种类',
  `level` smallint(6) NOT NULL DEFAULT '0' COMMENT '等级',
  `exp` int(11) NOT NULL DEFAULT '0' COMMENT '经验',
  `soldiers` int(11) NOT NULL DEFAULT '0' COMMENT '兵力',
  `attack` smallint(6) NOT NULL DEFAULT '0' COMMENT '攻击资质',
  `defense` smallint(6) NOT NULL DEFAULT '0' COMMENT '防御资质',
  `troops` smallint(6) NOT NULL DEFAULT '0' COMMENT '兵力资质',
  `growth` smallint(6) NOT NULL DEFAULT '0' COMMENT '总资质',
  PRIMARY KEY (`id`)
  KEY (`actorid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
