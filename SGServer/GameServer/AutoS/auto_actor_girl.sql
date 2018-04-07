CREATE TABLE `actor_girl` (
  `actorid` int(11) NOT NULL DEFAULT '0' COMMENT '角色',
  `kind` tinyint(3) NOT NULL DEFAULT '0' COMMENT '种类',
  `love_level` tinyint(3) NOT NULL DEFAULT '0' COMMENT '亲密等级',
  `love_num` tinyint(3) NOT NULL DEFAULT '0' COMMENT '今天亲密次数',
  `color` tinyint(3) NOT NULL DEFAULT '0' COMMENT '颜色',
  `soul` smallint(6) NOT NULL DEFAULT '0' COMMENT '碎片',
  `love_exp` smallint(6) NOT NULL DEFAULT '0' COMMENT '亲密度',
  `love_fday` int(11) NOT NULL DEFAULT '0' COMMENT '亲密时间',
  PRIMARY KEY (`actorid`)
  KEY (`kind`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
