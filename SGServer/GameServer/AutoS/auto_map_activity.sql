CREATE TABLE `map_activity` (
  `index` int(11) NOT NULL DEFAULT '0' COMMENT '����',
  `posx` smallint(6) NOT NULL DEFAULT '0' COMMENT 'λ��',
  `posy` smallint(6) NOT NULL DEFAULT '0' COMMENT 'λ��',
  `kind` smallint(6) NOT NULL DEFAULT '0' COMMENT '����',
  `deltime` int(11) NOT NULL DEFAULT '0' COMMENT 'ɾ��ʱ��',
  `actorid` int(11) NOT NULL DEFAULT '0' COMMENT '���ڽ�ɫ',
  `hp` int(11) NOT NULL DEFAULT '0' COMMENT 'ʣ��Ѫ��',
  `maxhp` int(11) NOT NULL DEFAULT '0' COMMENT 'Ѫ��',
  `activityid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '�����',
  PRIMARY KEY (`index`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
