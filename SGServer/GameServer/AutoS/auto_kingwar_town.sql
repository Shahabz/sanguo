CREATE TABLE `kingwar_town` (
  `id` int(11) NOT NULL DEFAULT '0' COMMENT 'id',
  `nation` tinyint(3) NOT NULL DEFAULT '0' COMMENT '占领国',
  `attack_total` int(11) NOT NULL DEFAULT '0' COMMENT '攻击方总兵力',
  `defense_total` int(11) NOT NULL DEFAULT '0' COMMENT '防御方总兵力',
  `sec` int(11) NOT NULL DEFAULT '0' COMMENT '战斗倒计时',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
