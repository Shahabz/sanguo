CREATE TABLE `map_enemy` (
  `index` int(11) NOT NULL DEFAULT '0' COMMENT '流寇id',
  `posx` smallint(6) NOT NULL DEFAULT '0' COMMENT '位置',
  `posy` smallint(6) NOT NULL DEFAULT '0' COMMENT '位置',
  `kind` smallint(6) NOT NULL DEFAULT '0' COMMENT '种类',
  `deltime` int(11) NOT NULL DEFAULT '0' COMMENT '删除时间',
  `actorid` int(11) NOT NULL DEFAULT '0' COMMENT '属于角色',
  PRIMARY KEY (`index`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
