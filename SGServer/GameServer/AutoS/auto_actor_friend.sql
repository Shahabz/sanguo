CREATE TABLE `actor_friend` (
  `actorid` int(11) NOT NULL DEFAULT '0' COMMENT '角色编号',
  `offset` tinyint(3) NOT NULL DEFAULT '0' COMMENT '索引',
  `friend_actorid` int(11) NOT NULL DEFAULT '0' COMMENT '好友id',
  `jointime` int(11) NOT NULL DEFAULT '0' COMMENT '加入时间',
  PRIMARY KEY (`actorid`)
  KEY (`offset`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
