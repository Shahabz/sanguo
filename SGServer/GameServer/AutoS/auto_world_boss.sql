CREATE TABLE `world_boss` (
  `bossid` tinyint(3) NOT NULL DEFAULT '0' COMMENT 'bossid',
  `hp` int(11) NOT NULL DEFAULT '0' COMMENT '当前血量',
  `maxhp` int(11) NOT NULL DEFAULT '0' COMMENT '总血量',
  PRIMARY KEY (`bossid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
