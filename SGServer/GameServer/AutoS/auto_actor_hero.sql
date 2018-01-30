CREATE TABLE `actor_hero` (
  `id` bigint(20) NOT NULL DEFAULT '0' COMMENT '主索引',
  `actorid` int(11) NOT NULL DEFAULT '0' COMMENT '城池',
  `offset` int(11) NOT NULL DEFAULT '0' COMMENT '索引',
  `kind` smallint(6) NOT NULL DEFAULT '0' COMMENT '英雄种类',
  `color` tinyint(3) NOT NULL DEFAULT '0' COMMENT '颜色',
  `level` smallint(6) NOT NULL DEFAULT '0' COMMENT '等级',
  `exp` int(11) NOT NULL DEFAULT '0' COMMENT '经验',
  `soldiers` int(11) NOT NULL DEFAULT '0' COMMENT '当前兵力',
  `attack_wash` smallint(6) NOT NULL DEFAULT '0' COMMENT '洗髓攻击资质',
  `defense_wash` smallint(6) NOT NULL DEFAULT '0' COMMENT '洗髓防御资质',
  `troops_wash` smallint(6) NOT NULL DEFAULT '0' COMMENT '洗髓兵力资质',
  `colorup` smallint(6) NOT NULL DEFAULT '0' COMMENT '良将突破值',
  `god` tinyint(3) NOT NULL DEFAULT '0' COMMENT '是否神级突破',
  PRIMARY KEY (`id`)
  KEY (`actorid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
