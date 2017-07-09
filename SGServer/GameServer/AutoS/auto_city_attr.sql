CREATE TABLE `city_attr` (
  `actorid` int(11) NOT NULL DEFAULT '0' COMMENT '角色ID',
  `silver_per` smallint(6) NOT NULL DEFAULT '0' COMMENT '银币产量增加%',
  `wood_per` smallint(6) NOT NULL DEFAULT '0' COMMENT '木材产量增加%',
  `food_per` smallint(6) NOT NULL DEFAULT '0' COMMENT '粮食产量增加%',
  `iron_per` smallint(6) NOT NULL DEFAULT '0' COMMENT '镔铁产量增加%',
  `protectres_per` smallint(6) NOT NULL DEFAULT '0' COMMENT '仓库保护增加%',
  `buildingsec_per` smallint(6) NOT NULL DEFAULT '0' COMMENT '升级时间减少%',
  `infantry_attack` smallint(6) NOT NULL DEFAULT '0' COMMENT '步兵武将增加攻击力',
  `cavalry_attack` smallint(6) NOT NULL DEFAULT '0' COMMENT '骑兵武将增加攻击力',
  `archer_attack` smallint(6) NOT NULL DEFAULT '0' COMMENT '弓兵武将增加攻击力',
  PRIMARY KEY (`actorid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
