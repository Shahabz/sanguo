CREATE TABLE `map_res` (
  `index` int(11) NOT NULL DEFAULT '0' COMMENT '索引',
  `posx` smallint(6) NOT NULL DEFAULT '0' COMMENT '位置',
  `posy` smallint(6) NOT NULL DEFAULT '0' COMMENT '位置',
  `kind` tinyint(3) NOT NULL DEFAULT '0' COMMENT '资源种类',
  PRIMARY KEY (`index`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
