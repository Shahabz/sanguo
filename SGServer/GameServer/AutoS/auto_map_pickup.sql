CREATE TABLE `map_pickup` (
  `index` int(11) NOT NULL DEFAULT '0' COMMENT '拾取物索引',
  `posx` smallint(6) NOT NULL DEFAULT '0' COMMENT '位置',
  `posy` smallint(6) NOT NULL DEFAULT '0' COMMENT '位置',
  `kind` smallint(6) NOT NULL DEFAULT '0' COMMENT '种类',
  `value` int(11) NOT NULL DEFAULT '0' COMMENT '数值',
  PRIMARY KEY (`index`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
