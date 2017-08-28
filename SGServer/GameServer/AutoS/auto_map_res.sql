CREATE TABLE `map_res` (
  `index` int(11) NOT NULL DEFAULT '0' COMMENT '索引',
  `posx` smallint(6) NOT NULL DEFAULT '0' COMMENT '位置',
  `posy` smallint(6) NOT NULL DEFAULT '0' COMMENT '位置',
  `kind` smallint(6) NOT NULL DEFAULT '0' COMMENT '资源种类',
  `num` int(11) NOT NULL DEFAULT '0' COMMENT '资源量',
  `idlesec` int(11) NOT NULL DEFAULT '0' COMMENT '空闲时长',
  PRIMARY KEY (`index`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
