CREATE TABLE `map_pickup` (
  `index` int(11) NOT NULL DEFAULT '0' COMMENT 'ʰȡ������',
  `posx` smallint(6) NOT NULL DEFAULT '0' COMMENT 'λ��',
  `posy` smallint(6) NOT NULL DEFAULT '0' COMMENT 'λ��',
  `kind` smallint(6) NOT NULL DEFAULT '0' COMMENT '����',
  `value` int(11) NOT NULL DEFAULT '0' COMMENT '��ֵ',
  PRIMARY KEY (`index`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
