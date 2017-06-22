# SQL-Front 5.1  (Build 4.16)

/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE */;
/*!40101 SET SQL_MODE='STRICT_TRANS_TABLES,NO_ENGINE_SUBSTITUTION' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES */;
/*!40103 SET SQL_NOTES='ON' */;


# Host: 119.29.56.213    Database: sg_dev_autoconfig
# ------------------------------------------------------
# Server version 5.6.36-log

#
# Source for table net_struct
#

CREATE TABLE `net_struct` (
  `structid` int(11) NOT NULL AUTO_INCREMENT,
  `name` varbinary(32) NOT NULL DEFAULT '',
  `context` varbinary(128) NOT NULL DEFAULT '',
  `struct_type` tinyint(3) NOT NULL DEFAULT '0' COMMENT '1server,2client,3logined',
  `sortindex` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`structid`)
) ENGINE=MyISAM AUTO_INCREMENT=91 DEFAULT CHARSET=gbk ROW_FORMAT=DYNAMIC;

#
# Dumping data for table net_struct
#

INSERT INTO `net_struct` VALUES (1,'NetC_Login','客户端请求登录',2,0);
INSERT INTO `net_struct` VALUES (2,'NetU_Logined','用户服务器返回登陆',3,0);
INSERT INTO `net_struct` VALUES (3,'NetS_Login','服务器端发送登录结果给客户端',1,0);
INSERT INTO `net_struct` VALUES (4,'ListInfo','单个角色的列表信息',1,0);
INSERT INTO `net_struct` VALUES (5,'NetS_List','服务器端发送角色列表',1,0);
INSERT INTO `net_struct` VALUES (6,'NetC_Create','客户端请求创建角色',2,0);
INSERT INTO `net_struct` VALUES (7,'NetS_Create','服务器返回创建角色结果',1,0);
INSERT INTO `net_struct` VALUES (8,'NetS_EnterInfo','服务器返回进入游戏后的信息',1,0);
INSERT INTO `net_struct` VALUES (9,'NetS_Delete','服务器返回删除角色',1,0);
INSERT INTO `net_struct` VALUES (10,'NetC_Heart','心跳',2,0);
INSERT INTO `net_struct` VALUES (11,'NetS_Heart','心跳',1,0);
INSERT INTO `net_struct` VALUES (12,'NetC_Gmcmd','GM指令',2,0);
INSERT INTO `net_struct` VALUES (13,'NetS_Notify','通用短消息',1,0);
INSERT INTO `net_struct` VALUES (14,'NetU_UserAwarded','用户服务器返回兑换奖励',3,0);
INSERT INTO `net_struct` VALUES (15,'NetU_Gmlocalcmd','本地GM信息',3,0);
INSERT INTO `net_struct` VALUES (16,'NetC_AskInfo','客户端发送短请求',2,0);
INSERT INTO `net_struct` VALUES (17,'NetC_EnterCity','请求进入城池所有信息',2,0);
INSERT INTO `net_struct` VALUES (25,'NetS_Building','普通建筑信息',1,0);
INSERT INTO `net_struct` VALUES (26,'NetS_BuildingBarracks','兵营建筑信息',1,0);
INSERT INTO `net_struct` VALUES (27,'NetS_BuildingRes','资源建筑信息',1,0);
INSERT INTO `net_struct` VALUES (28,'NetS_BuildingList','建筑列表',1,0);
INSERT INTO `net_struct` VALUES (29,'NetS_ActorInfo','角色基本信息',1,0);
INSERT INTO `net_struct` VALUES (30,'Corrdinate','坐标',1,0);
INSERT INTO `net_struct` VALUES (31,'NetS_ItemUse','道具使用|装备',1,0);
INSERT INTO `net_struct` VALUES (32,'NetS_ItemPut','道具放置|卸下',1,0);
INSERT INTO `net_struct` VALUES (33,'NetS_ItemSettle','道具整理',1,0);
INSERT INTO `net_struct` VALUES (34,'NetS_LostItem','丢失道具',1,0);
INSERT INTO `net_struct` VALUES (35,'NetS_GetItem','获得道具',1,0);
INSERT INTO `net_struct` VALUES (36,'ItemSmpInfo','道具简易信息',1,0);
INSERT INTO `net_struct` VALUES (37,'NetS_ItemList','道具列表',1,0);
INSERT INTO `net_struct` VALUES (38,'ItemAttr','道具属性',1,0);
INSERT INTO `net_struct` VALUES (39,'NetS_ItemInfo','道具信息',1,0);
INSERT INTO `net_struct` VALUES (40,'WalkPath','移动路径',1,0);
INSERT INTO `net_struct` VALUES (44,'NetS_AddMapUnit','显示单元进入地图',1,0);
INSERT INTO `net_struct` VALUES (45,'NetS_DelMapUnit','显示单元离开地图',1,0);
INSERT INTO `net_struct` VALUES (46,'NetS_WorldMapInfo','世界地图信息',1,0);
INSERT INTO `net_struct` VALUES (47,'NetS_MapUnitCorrdinate','地图显示单元位置',1,0);
INSERT INTO `net_struct` VALUES (48,'NetS_AddMarchRoute','行军路线',1,0);
INSERT INTO `net_struct` VALUES (49,'NetS_DelMarchRoute','删除行军路线',1,0);
INSERT INTO `net_struct` VALUES (50,'NetS_UpdateMapUnit','更新地图显示单元信息',1,0);
INSERT INTO `net_struct` VALUES (77,'NetS_AwardInfo','奖励信息',1,0);
INSERT INTO `net_struct` VALUES (78,'NetS_AwardInfoList','奖励信息列表',1,0);
INSERT INTO `net_struct` VALUES (79,'NetS_Experience','改变经验',1,0);
INSERT INTO `net_struct` VALUES (80,'NetS_Body','改变体力',1,0);
INSERT INTO `net_struct` VALUES (81,'NetS_Levy','改变征收次数',1,0);
INSERT INTO `net_struct` VALUES (82,'NetS_Silver','改变银币',1,0);
INSERT INTO `net_struct` VALUES (83,'NetS_Wood','改变木材',1,0);
INSERT INTO `net_struct` VALUES (84,'NetS_Food','改变粮食',1,0);
INSERT INTO `net_struct` VALUES (85,'NetS_Iron','改变镔铁',1,0);
INSERT INTO `net_struct` VALUES (86,'NetS_People','改变人口',1,0);
INSERT INTO `net_struct` VALUES (87,'NetS_Prestige','改变威望值',1,0);
INSERT INTO `net_struct` VALUES (88,'NetS_Friendship','改变友谊积分',1,0);
INSERT INTO `net_struct` VALUES (89,'NetS_Vip','改变VIP经验',1,0);
INSERT INTO `net_struct` VALUES (90,'NetS_Token','改变钻石',1,0);

#
# Source for table net_struct_conf
#

CREATE TABLE `net_struct_conf` (
  `dataid` int(11) NOT NULL AUTO_INCREMENT,
  `structid` int(11) DEFAULT NULL,
  `offset` smallint(6) NOT NULL DEFAULT '0',
  `name` varbinary(32) NOT NULL DEFAULT '',
  `data_type` smallint(6) NOT NULL DEFAULT '0',
  `data_count` smallint(6) NOT NULL DEFAULT '0',
  `data_size` smallint(6) NOT NULL DEFAULT '0',
  `context` varbinary(128) NOT NULL DEFAULT '',
  `notsave` tinyint(4) NOT NULL DEFAULT '0',
  `kind` tinyint(4) NOT NULL DEFAULT '0',
  `m_fix` tinyint(3) NOT NULL DEFAULT '1',
  PRIMARY KEY (`dataid`)
) ENGINE=MyISAM AUTO_INCREMENT=90003 DEFAULT CHARSET=gbk ROW_FORMAT=DYNAMIC;

#
# Dumping data for table net_struct_conf
#

INSERT INTO `net_struct_conf` VALUES (1000,1,0,'username_length',-2,0,0,'客户端请求-用户名长度',0,0,1);
INSERT INTO `net_struct_conf` VALUES (1001,1,1,'username',-1,-1,51,'客户端请求-用户名',0,0,1);
INSERT INTO `net_struct_conf` VALUES (1002,1,2,'password_length',-2,0,0,'客户端请求-密码长度',0,0,1);
INSERT INTO `net_struct_conf` VALUES (1003,1,3,'password',-1,-1,256,'客户端请求-密码',0,0,1);
INSERT INTO `net_struct_conf` VALUES (1004,1,4,'deviceid_length',-2,0,0,'客户端请求-设备标示长度',0,0,1);
INSERT INTO `net_struct_conf` VALUES (1005,1,5,'deviceid',-1,-1,65,'客户端请求-设备标示',0,0,1);
INSERT INTO `net_struct_conf` VALUES (1006,1,6,'isnew',-2,0,0,'客户端请求-是否新用户',0,0,1);
INSERT INTO `net_struct_conf` VALUES (1007,1,7,'ipcountry_length',-2,0,0,'客户端请求-IP地址所在国家',0,0,1);
INSERT INTO `net_struct_conf` VALUES (1008,1,8,'ipcountry',-1,-1,16,'客户端请求-IP地址所在国家',0,0,1);
INSERT INTO `net_struct_conf` VALUES (2000,2,0,'result',-3,0,0,'用户服务器返回-登录返回值',0,0,1);
INSERT INTO `net_struct_conf` VALUES (2001,2,1,'client_index',-3,0,0,'用户服务器返回-角色索引',0,0,1);
INSERT INTO `net_struct_conf` VALUES (2002,2,2,'authid',-3,0,0,'用户服务器返回-认证ID',0,0,1);
INSERT INTO `net_struct_conf` VALUES (2003,2,3,'userid',-1,21,0,'用户服务器返回-用户ID',0,0,1);
INSERT INTO `net_struct_conf` VALUES (2004,2,4,'username',-1,51,0,'用户服务器返回-用户名',0,0,1);
INSERT INTO `net_struct_conf` VALUES (2005,2,5,'usertype',-2,0,0,'用户服务器返回-用户类型',0,0,1);
INSERT INTO `net_struct_conf` VALUES (2006,2,6,'checkflag',-1,0,0,'用户服务器返回-检测标记',0,0,1);
INSERT INTO `net_struct_conf` VALUES (2007,2,7,'isdeep',-1,0,0,'用户服务器返回-要防沉迷',0,0,1);
INSERT INTO `net_struct_conf` VALUES (2008,2,8,'access_token',-1,65,0,'用户服务器返回-360的用户令牌',0,0,1);
INSERT INTO `net_struct_conf` VALUES (2009,2,9,'expires_in',-3,0,0,'用户服务器返回-360的令牌有效期',0,0,1);
INSERT INTO `net_struct_conf` VALUES (2010,2,10,'refresh_token',-1,65,0,'用户服务器返回-360的刷新令牌',0,0,1);
INSERT INTO `net_struct_conf` VALUES (2011,2,11,'deviceid',-1,65,0,'用户服务器返回-传回的设备ID',0,0,1);
INSERT INTO `net_struct_conf` VALUES (3000,3,0,'result',-3,0,0,'服务器返回-返回值',0,0,1);
INSERT INTO `net_struct_conf` VALUES (3001,3,1,'username_length',-2,0,0,'服务器返回-用户名长度',0,0,1);
INSERT INTO `net_struct_conf` VALUES (3002,3,2,'username',-1,-1,51,'服务器返回-用户名',0,0,1);
INSERT INTO `net_struct_conf` VALUES (3003,3,3,'token_length',-2,0,0,'服务器返回-token长度',0,0,1);
INSERT INTO `net_struct_conf` VALUES (3004,3,4,'token',-1,-1,256,'服务器返回-token',0,0,1);
INSERT INTO `net_struct_conf` VALUES (3005,3,5,'usertype',-2,0,0,'服务器返回-用户类型',0,0,1);
INSERT INTO `net_struct_conf` VALUES (4000,4,0,'actorid',-3,0,0,'角色ID',0,0,1);
INSERT INTO `net_struct_conf` VALUES (4001,4,1,'name',-1,22,0,'角色名字',0,0,1);
INSERT INTO `net_struct_conf` VALUES (4002,4,2,'nation',-1,0,0,'角色国家',0,0,1);
INSERT INTO `net_struct_conf` VALUES (4003,4,3,'level',-2,0,0,'角色等级',0,0,1);
INSERT INTO `net_struct_conf` VALUES (4004,4,4,'offset',-1,0,0,'角色位置',0,0,1);
INSERT INTO `net_struct_conf` VALUES (4005,4,5,'lockstat',-1,0,0,'锁定状态',0,0,1);
INSERT INTO `net_struct_conf` VALUES (4006,4,6,'delete_stoptime',-3,0,0,'删除时间',0,0,1);
INSERT INTO `net_struct_conf` VALUES (4007,4,7,'lock_endtime',-3,0,0,'锁定时间',0,0,1);
INSERT INTO `net_struct_conf` VALUES (5000,5,0,'actor_num',-1,0,0,'服务端返回-角色个数',0,0,1);
INSERT INTO `net_struct_conf` VALUES (5001,5,1,'listinfo',4,-1,8,'服务端返回-角色数组',0,0,1);
INSERT INTO `net_struct_conf` VALUES (6000,6,0,'nation',-1,0,0,'客户端请求-创建的角色国家',0,0,1);
INSERT INTO `net_struct_conf` VALUES (6001,6,1,'name_length',-2,0,0,'客户端请求-创建的角色名称长度',0,0,1);
INSERT INTO `net_struct_conf` VALUES (6002,6,2,'name',-1,-1,22,'客户端请求-创建的角色名称',0,0,1);
INSERT INTO `net_struct_conf` VALUES (7000,7,0,'result',-1,0,0,'服务端返回-创建角色的结果',0,0,1);
INSERT INTO `net_struct_conf` VALUES (7001,7,1,'actorid',-3,0,0,'服务端返回-创建角色的编号',0,0,1);
INSERT INTO `net_struct_conf` VALUES (7002,7,2,'createtime',-3,0,0,'服务端返回-创建角色的时间',0,0,1);
INSERT INTO `net_struct_conf` VALUES (8000,8,0,'actorid',-3,0,0,'服务端返回-进入游戏角色ID',0,0,1);
INSERT INTO `net_struct_conf` VALUES (8001,8,1,'data_int',-3,0,0,'服务端返回-进入游戏额外数据[备用]',0,0,1);
INSERT INTO `net_struct_conf` VALUES (8002,8,2,'data_short',-2,0,0,'服务端返回-进入游戏额外数据[备用]',0,0,1);
INSERT INTO `net_struct_conf` VALUES (8003,8,3,'data_char',-1,0,0,'服务端返回-进入游戏额外数据[备用]',0,0,1);
INSERT INTO `net_struct_conf` VALUES (8004,8,4,'serverid',-2,0,0,'服务端返回-进入的服务器ID',0,0,1);
INSERT INTO `net_struct_conf` VALUES (8005,8,5,'servertime',-3,0,0,'服务端返回-进入的服务器时间戳',0,0,1);
INSERT INTO `net_struct_conf` VALUES (8006,8,6,'createtime',-3,0,0,'服务端返回-创建角色时间',0,0,1);
INSERT INTO `net_struct_conf` VALUES (9000,9,0,'result',-3,0,0,'服务器返回-删除返回值',0,0,1);
INSERT INTO `net_struct_conf` VALUES (9001,9,1,'actorid',-3,0,0,'服务器返回-删除的角色编号',0,0,1);
INSERT INTO `net_struct_conf` VALUES (10000,10,0,'value',-3,2,0,'心跳-数值',0,0,1);
INSERT INTO `net_struct_conf` VALUES (11000,11,0,'value',-3,2,0,'心跳-数值',0,0,1);
INSERT INTO `net_struct_conf` VALUES (12000,12,0,'cmd',-2,0,0,'客户端发来-GM指令',0,0,1);
INSERT INTO `net_struct_conf` VALUES (12001,12,1,'value',-3,4,0,'客户端发来-GM指令数值参数',0,0,1);
INSERT INTO `net_struct_conf` VALUES (12002,12,2,'msg_length',-2,0,0,'客户端发来-GM指令字符串长度',0,0,1);
INSERT INTO `net_struct_conf` VALUES (12003,12,3,'msg',-1,-1,256,'客户端发来-GM指令字符串参数',0,0,1);
INSERT INTO `net_struct_conf` VALUES (13000,13,0,'msgid',-2,0,0,'服务器发送-短消息ID',0,0,1);
INSERT INTO `net_struct_conf` VALUES (13001,13,1,'valuenum',-2,0,0,'服务器发送-短消息参数个数',0,0,1);
INSERT INTO `net_struct_conf` VALUES (13002,13,2,'value',-3,-1,10,'服务器发送-短消息数值参数',0,0,1);
INSERT INTO `net_struct_conf` VALUES (13003,13,3,'msg_length',-2,0,0,'服务器发送-短消息字符串消息长度',0,0,1);
INSERT INTO `net_struct_conf` VALUES (13004,13,4,'msg',-1,-1,256,'服务器发送-短消息字符串参数',0,0,1);
INSERT INTO `net_struct_conf` VALUES (14000,14,0,'result',-3,0,0,'用户服务器返回-兑换结果返回值',0,0,1);
INSERT INTO `net_struct_conf` VALUES (14001,14,1,'client_index',-3,0,0,'用户服务器返回-角色索引',0,0,1);
INSERT INTO `net_struct_conf` VALUES (14002,14,2,'authid',-3,0,0,'用户服务器返回-认证ID',0,0,1);
INSERT INTO `net_struct_conf` VALUES (14003,14,3,'cdkey_index',-2,0,0,'用户服务器返回-兑换码存档索引',0,0,1);
INSERT INTO `net_struct_conf` VALUES (14004,14,4,'awardgroup',-3,0,0,'用户服务器返回-兑换码奖励组',0,0,1);
INSERT INTO `net_struct_conf` VALUES (14005,14,5,'cardnumber',-1,32,0,'用户服务器返回-兑换码卡号',0,0,1);
INSERT INTO `net_struct_conf` VALUES (15000,15,0,'cmd',-2,0,0,'指令',0,0,1);
INSERT INTO `net_struct_conf` VALUES (15001,15,1,'value',-3,4,0,'数值参数',0,0,1);
INSERT INTO `net_struct_conf` VALUES (15002,15,2,'msglen',-2,0,0,'字符串长度',0,0,1);
INSERT INTO `net_struct_conf` VALUES (15003,15,3,'msg',-1,-1,1024,'字符串参数',0,0,1);
INSERT INTO `net_struct_conf` VALUES (16000,16,0,'msgid',-2,0,0,'客户端发送-短请求ID',0,0,1);
INSERT INTO `net_struct_conf` VALUES (16001,16,1,'valuenum',-2,0,0,'客户端发送-短请求参数个数',0,0,1);
INSERT INTO `net_struct_conf` VALUES (16002,16,2,'value',-3,-1,10,'客户端发送-短请求参数',0,0,1);
INSERT INTO `net_struct_conf` VALUES (16003,16,3,'msg_length',-2,0,0,'客户端发送-短请求字符串长度',0,0,1);
INSERT INTO `net_struct_conf` VALUES (16004,16,4,'msg',-1,-1,256,'客户端发送-短请求字符串',0,0,1);
INSERT INTO `net_struct_conf` VALUES (17001,17,0,'value',-1,0,0,'客户端发送-我已初始化好，发过来玩家信息吧',0,0,1);
INSERT INTO `net_struct_conf` VALUES (25000,25,0,'kind',-1,0,0,'普通建筑-种类',0,0,1);
INSERT INTO `net_struct_conf` VALUES (25001,25,1,'offset',-1,0,0,'普通建筑-位置',0,0,1);
INSERT INTO `net_struct_conf` VALUES (25002,25,2,'level',-1,0,0,'普通建筑-等级',0,0,1);
INSERT INTO `net_struct_conf` VALUES (25003,25,3,'sec',-3,0,0,'普通建筑-操作剩余时间',0,0,1);
INSERT INTO `net_struct_conf` VALUES (25004,25,4,'quick',-1,0,0,'普通建筑-是否有加速(科技等)',0,0,1);
INSERT INTO `net_struct_conf` VALUES (26000,26,0,'kind',-1,0,0,'兵营建筑-种类',0,0,1);
INSERT INTO `net_struct_conf` VALUES (26001,26,1,'offset',-1,0,0,'兵营建筑-位置',0,0,1);
INSERT INTO `net_struct_conf` VALUES (26002,26,2,'level',-1,0,0,'兵营建筑-等级',0,0,1);
INSERT INTO `net_struct_conf` VALUES (26003,26,3,'sec',-3,0,0,'兵营建筑-募兵剩余时间',0,0,1);
INSERT INTO `net_struct_conf` VALUES (26004,26,4,'quick',-1,0,0,'兵营建筑-是否有加速',0,0,1);
INSERT INTO `net_struct_conf` VALUES (27000,27,0,'kind',-1,0,0,'资源建筑-种类',0,0,1);
INSERT INTO `net_struct_conf` VALUES (27001,27,1,'offset',-1,0,0,'资源建筑-索引(对应指定地块)',0,0,1);
INSERT INTO `net_struct_conf` VALUES (27002,27,2,'level',-1,0,0,'资源建筑-等级',0,0,1);
INSERT INTO `net_struct_conf` VALUES (28000,28,0,'building_count',-2,0,0,'服务器发送-普通建筑数量',0,0,1);
INSERT INTO `net_struct_conf` VALUES (28001,28,1,'building',25,-1,8,'服务器发送-普通建筑列表',0,0,1);
INSERT INTO `net_struct_conf` VALUES (28002,28,2,'barracks_count',-2,0,0,'服务器发送-兵营建筑数量',0,0,1);
INSERT INTO `net_struct_conf` VALUES (28003,28,3,'barracks',26,-1,8,'服务器发送-兵营建筑列表',0,0,1);
INSERT INTO `net_struct_conf` VALUES (28004,28,4,'res_count',-2,0,0,'服务器发送-资源建筑数量',0,0,1);
INSERT INTO `net_struct_conf` VALUES (28005,28,5,'res',27,-1,64,'服务器发送-资源建筑列表',0,0,1);
INSERT INTO `net_struct_conf` VALUES (28006,28,6,'levynum',-1,0,0,'服务器发送-当前可征收次数',0,0,1);
INSERT INTO `net_struct_conf` VALUES (28007,28,7,'worker_kind',-1,0,0,'服务器发送-建筑队列种类',0,0,1);
INSERT INTO `net_struct_conf` VALUES (28008,28,8,'worker_offset',-1,0,0,'服务器发送建筑队列索引',0,0,1);
INSERT INTO `net_struct_conf` VALUES (28009,28,9,'worker_op',-1,0,0,'服务器发送-建筑队列建筑操作',0,0,1);
INSERT INTO `net_struct_conf` VALUES (28010,28,10,'worker_sec',-3,0,0,'服务器发送-建筑队列剩余时间',0,0,1);
INSERT INTO `net_struct_conf` VALUES (28011,28,11,'worker_kind_ex',-1,0,0,'服务器发送-建筑队列种类(商用)',0,0,1);
INSERT INTO `net_struct_conf` VALUES (28012,28,12,'worker_offset_ex',-1,0,0,'服务器发送-建筑队列索引(商用)',0,0,1);
INSERT INTO `net_struct_conf` VALUES (28013,28,13,'worker_op_ex',-1,0,0,'服务器发送-建筑队列操作(商用)',0,0,1);
INSERT INTO `net_struct_conf` VALUES (28014,28,14,'worker_sec_ex',-3,0,0,'服务器发送-建筑队列剩余时间(商用)',0,0,1);
INSERT INTO `net_struct_conf` VALUES (28015,28,15,'worker_expire_ex',-3,0,0,'服务器发送-商用建造队列到期时间',0,0,1);
INSERT INTO `net_struct_conf` VALUES (28016,28,16,'function',-3,0,0,'服务器发送-功能是否开启',0,0,1);
INSERT INTO `net_struct_conf` VALUES (29000,29,0,'actorid',-3,0,0,'服务器发送玩家基本信息-角色编号',0,0,1);
INSERT INTO `net_struct_conf` VALUES (29001,29,1,'name',-1,22,0,'服务器发送玩家基本信息-角色名称',0,0,1);
INSERT INTO `net_struct_conf` VALUES (29002,29,2,'nation',-1,0,0,'服务器发送玩家基本信息-国家',0,0,1);
INSERT INTO `net_struct_conf` VALUES (29003,29,3,'shape',-1,0,0,'服务器发送玩家基本信息-形象',0,0,1);
INSERT INTO `net_struct_conf` VALUES (29004,29,4,'level',-2,0,0,'服务器发送玩家基本信息-级别',0,0,1);
INSERT INTO `net_struct_conf` VALUES (29005,29,5,'exp',-4,0,0,'服务器发送玩家基本信息-经验',0,0,1);
INSERT INTO `net_struct_conf` VALUES (29006,29,6,'exp_max',-4,0,0,'服务器发送玩家基本信息-经验上限',0,0,1);
INSERT INTO `net_struct_conf` VALUES (29008,29,8,'token',-3,0,0,'服务器发送玩家基本信息-钻石',0,0,1);
INSERT INTO `net_struct_conf` VALUES (29009,29,9,'viplevel',-1,0,0,'服务器发送玩家基本信息-VIP等级',0,0,1);
INSERT INTO `net_struct_conf` VALUES (29010,29,10,'vipexp',-3,0,0,'服务器发送玩家基本信息-VIP经验',0,0,1);
INSERT INTO `net_struct_conf` VALUES (29011,29,11,'vipexp_max',-3,0,0,'服务器发送玩家基本信息-VIP经验上限',0,0,1);
INSERT INTO `net_struct_conf` VALUES (29012,29,12,'body',-2,0,0,'服务器发送玩家基本信息-体力',0,0,1);
INSERT INTO `net_struct_conf` VALUES (29013,29,13,'place',-1,0,0,'服务器发送玩家基本信息-爵位',0,0,1);
INSERT INTO `net_struct_conf` VALUES (29014,29,14,'official',-1,0,0,'服务器发送玩家基本信息-官职',0,0,1);
INSERT INTO `net_struct_conf` VALUES (29015,29,15,'zone',-2,0,0,'服务器发送玩家基本信息-区域',0,0,1);
INSERT INTO `net_struct_conf` VALUES (29016,29,16,'battlepower',-3,0,0,'服务器发送玩家基本信息-战力',0,0,1);
INSERT INTO `net_struct_conf` VALUES (29017,29,17,'silver',-2,0,0,'服务器发送玩家基本信息-银币',0,0,1);
INSERT INTO `net_struct_conf` VALUES (29018,29,18,'wood',-2,0,0,'服务器发送玩家基本信息-木材',0,0,1);
INSERT INTO `net_struct_conf` VALUES (29019,29,19,'food',-2,0,0,'服务器发送玩家基本信息-粮食',0,0,1);
INSERT INTO `net_struct_conf` VALUES (29020,29,20,'iron',-2,0,0,'服务器发送玩家基本信息-镔铁',0,0,1);
INSERT INTO `net_struct_conf` VALUES (29022,29,22,'infantry_num',-3,0,0,'服务器发送玩家基本信息-步兵数',0,0,1);
INSERT INTO `net_struct_conf` VALUES (29023,29,23,'cavalry_num',-3,0,0,'服务器发送玩家基本信息-骑兵数',0,0,1);
INSERT INTO `net_struct_conf` VALUES (29024,29,24,'archer_num',-3,0,0,'服务器发送玩家基本信息-弓兵数',0,0,1);
INSERT INTO `net_struct_conf` VALUES (29025,29,25,'mokilllv',-2,0,0,'服务器发送玩家基本信息-击杀野怪最高级别',0,0,1);
INSERT INTO `net_struct_conf` VALUES (29026,29,26,'sflag',-3,0,0,'服务器发送玩家基本信息-标志位',0,0,1);
INSERT INTO `net_struct_conf` VALUES (30000,30,0,'posx',-2,0,0,'坐标',0,0,1);
INSERT INTO `net_struct_conf` VALUES (30001,30,1,'posy',-2,0,0,'坐标',0,0,1);
INSERT INTO `net_struct_conf` VALUES (31000,31,0,'itemoffset',-2,0,0,'物品位置',0,0,1);
INSERT INTO `net_struct_conf` VALUES (31001,31,1,'usenum',-2,0,0,'使用个数或装备位置',0,0,1);
INSERT INTO `net_struct_conf` VALUES (31002,31,2,'effres',-3,0,0,'影响，一般用于特效显示',0,0,1);
INSERT INTO `net_struct_conf` VALUES (32000,32,0,'res_offset',-2,0,0,'物品源',0,0,1);
INSERT INTO `net_struct_conf` VALUES (32001,32,1,'res_num',-2,0,0,'物品源个数',0,0,1);
INSERT INTO `net_struct_conf` VALUES (32002,32,2,'target_offset',-2,0,0,'物品目标',0,0,1);
INSERT INTO `net_struct_conf` VALUES (32003,32,3,'target_num',-2,0,0,'目标个数',0,0,1);
INSERT INTO `net_struct_conf` VALUES (33000,33,0,'itemnum',-2,0,0,'整理背包的个数',0,0,1);
INSERT INTO `net_struct_conf` VALUES (33001,33,1,'itemoffset',-2,-1,100,'重置的物品位置',0,0,1);
INSERT INTO `net_struct_conf` VALUES (34000,34,0,'itemoffset',-2,0,0,'物品位置',0,0,1);
INSERT INTO `net_struct_conf` VALUES (34001,34,1,'itemnum',-2,0,0,'数量',0,0,1);
INSERT INTO `net_struct_conf` VALUES (34002,34,2,'targetid',-3,0,0,'赋予的对象id',0,0,1);
INSERT INTO `net_struct_conf` VALUES (34003,34,3,'path',-2,0,0,'途径',0,0,1);
INSERT INTO `net_struct_conf` VALUES (35000,35,0,'itemoffset',-2,0,0,'物品位置',0,0,1);
INSERT INTO `net_struct_conf` VALUES (35001,35,1,'kind',-2,0,0,'物品种类',0,0,1);
INSERT INTO `net_struct_conf` VALUES (35002,35,2,'type',-2,0,0,'物品类型',0,0,1);
INSERT INTO `net_struct_conf` VALUES (35003,35,3,'color',-1,0,0,'颜色',0,0,1);
INSERT INTO `net_struct_conf` VALUES (35004,35,4,'num',-2,0,0,'数量',0,0,1);
INSERT INTO `net_struct_conf` VALUES (35005,35,5,'targetid',-3,0,0,'赋予的对象id',0,0,1);
INSERT INTO `net_struct_conf` VALUES (35006,35,6,'path',-2,0,0,'途径',0,0,1);
INSERT INTO `net_struct_conf` VALUES (35007,35,7,'situation',-11,0,0,'使用状态',0,0,1);
INSERT INTO `net_struct_conf` VALUES (36000,36,0,'offset',-2,0,0,'物品位置',0,0,1);
INSERT INTO `net_struct_conf` VALUES (36001,36,1,'kind',-2,0,0,'物品种类',0,0,1);
INSERT INTO `net_struct_conf` VALUES (36002,36,2,'num',-2,0,0,'数量',0,0,1);
INSERT INTO `net_struct_conf` VALUES (36003,36,3,'situation',-11,0,0,'使用状态',0,0,1);
INSERT INTO `net_struct_conf` VALUES (36004,36,4,'color_level',-1,0,0,'颜色',0,0,1);
INSERT INTO `net_struct_conf` VALUES (37000,37,0,'itemext',-1,0,0,'扩展的格子',0,0,1);
INSERT INTO `net_struct_conf` VALUES (37001,37,1,'kindnum',-2,0,0,'物品数量',0,0,1);
INSERT INTO `net_struct_conf` VALUES (37002,37,2,'item',36,-1,300,'物品',0,0,1);
INSERT INTO `net_struct_conf` VALUES (38000,38,0,'type',-2,0,0,'物品属性所属类型',0,0,1);
INSERT INTO `net_struct_conf` VALUES (38001,38,1,'ability',-2,0,0,'物品属性',0,0,1);
INSERT INTO `net_struct_conf` VALUES (38002,38,2,'value',-3,0,0,'物品数据',0,0,1);
INSERT INTO `net_struct_conf` VALUES (38003,38,3,'addvalue',-3,0,0,'物品增加数据',0,0,1);
INSERT INTO `net_struct_conf` VALUES (39000,39,0,'itemoffset',-2,0,0,'物品位置',0,0,1);
INSERT INTO `net_struct_conf` VALUES (39001,39,1,'itemkind',-2,0,0,'装备kind,校验',0,0,1);
INSERT INTO `net_struct_conf` VALUES (39002,39,2,'type',-2,0,0,'物品类型',0,0,1);
INSERT INTO `net_struct_conf` VALUES (39003,39,3,'level',-2,0,0,'一般等级',0,0,1);
INSERT INTO `net_struct_conf` VALUES (39004,39,4,'color_level',-1,0,0,'颜色分级',0,0,1);
INSERT INTO `net_struct_conf` VALUES (39005,39,5,'price',-3,0,0,'售价',0,0,1);
INSERT INTO `net_struct_conf` VALUES (39006,39,6,'attr_num',-2,0,0,'属性个数',0,0,1);
INSERT INTO `net_struct_conf` VALUES (39007,39,7,'attr',38,-1,10,'属性',0,0,1);
INSERT INTO `net_struct_conf` VALUES (40000,40,0,'pathx',-2,100,0,'路径点结构',0,0,1);
INSERT INTO `net_struct_conf` VALUES (40001,40,1,'pathy',-2,100,0,'路径点结构',0,0,1);
INSERT INTO `net_struct_conf` VALUES (40002,40,2,'len',-2,0,0,'路径点结构',0,0,1);
INSERT INTO `net_struct_conf` VALUES (44000,44,4,'unit_index',-3,0,0,'地图单元-序号',0,0,1);
INSERT INTO `net_struct_conf` VALUES (44001,44,1,'type',-1,0,0,'地图单元-类型',0,0,1);
INSERT INTO `net_struct_conf` VALUES (44002,44,2,'state',-1,0,0,'地图单元-状态',0,0,1);
INSERT INTO `net_struct_conf` VALUES (44003,44,3,'posx',-2,0,0,'地图单元-当前坐标',0,0,1);
INSERT INTO `net_struct_conf` VALUES (44004,44,4,'posy',-2,0,0,'地图单元-当前坐标',0,0,1);
INSERT INTO `net_struct_conf` VALUES (44005,44,5,'namelen',-1,0,0,'地图单元-名称长度',0,0,1);
INSERT INTO `net_struct_conf` VALUES (44006,44,6,'name',-1,-1,32,'地图单元-名称',0,0,1);
INSERT INTO `net_struct_conf` VALUES (44007,44,7,'char_value_count',-1,0,0,'地图单元-通用字段长度',0,0,1);
INSERT INTO `net_struct_conf` VALUES (44008,44,8,'char_value',-7,-1,16,'地图单元-通用字段',0,0,1);
INSERT INTO `net_struct_conf` VALUES (44009,44,9,'short_value_count',-1,0,0,'地图单元-通用字段长度',0,0,1);
INSERT INTO `net_struct_conf` VALUES (44010,44,10,'short_value',-2,-1,8,'地图单元-通用字段',0,0,1);
INSERT INTO `net_struct_conf` VALUES (44011,44,11,'int_value_count',-1,0,0,'地图单元-通用字段长度',0,0,1);
INSERT INTO `net_struct_conf` VALUES (44012,44,12,'int_value',-3,-1,8,'地图单元-通用字段',0,0,1);
INSERT INTO `net_struct_conf` VALUES (44013,44,13,'prefixlen',-1,0,0,'地图单元-称号长度',0,0,1);
INSERT INTO `net_struct_conf` VALUES (44014,44,14,'prefix',-1,-1,22,'地图单元-称号',0,0,1);
INSERT INTO `net_struct_conf` VALUES (45000,45,0,'unit_index',-3,0,0,'地图单元-序号',0,0,1);
INSERT INTO `net_struct_conf` VALUES (46000,46,0,'map_width',-2,0,0,'世界地图大小（格子数）',0,0,1);
INSERT INTO `net_struct_conf` VALUES (46001,46,1,'map_height',-2,0,0,'世界地图大小（格子数）',0,0,1);
INSERT INTO `net_struct_conf` VALUES (46002,46,2,'area_width',-1,0,0,'区域大小（一个区域几个地图格子）',0,0,1);
INSERT INTO `net_struct_conf` VALUES (46003,46,3,'area_height',-1,0,0,'区域大小（一个区域几个地图格子）',0,0,1);
INSERT INTO `net_struct_conf` VALUES (46004,46,4,'map_area_xnum',-2,0,0,'一个地图有多少区域',0,0,1);
INSERT INTO `net_struct_conf` VALUES (46005,46,5,'map_area_ynum',-2,0,0,'一个地图有多少区域',0,0,1);
INSERT INTO `net_struct_conf` VALUES (46006,46,6,'my_city_posx',-2,0,0,'我的城池位置',0,0,1);
INSERT INTO `net_struct_conf` VALUES (46007,46,7,'my_city_posy',-2,0,0,'我的城池位置',0,0,1);
INSERT INTO `net_struct_conf` VALUES (46008,46,8,'my_city_unit_index',-3,0,0,'我的城池地图单元-序号',0,0,1);
INSERT INTO `net_struct_conf` VALUES (46010,46,10,'citystate',-1,0,0,'我的城池状态',0,0,1);
INSERT INTO `net_struct_conf` VALUES (46011,46,11,'target_posx',-2,0,0,'目的点',0,0,1);
INSERT INTO `net_struct_conf` VALUES (46012,46,12,'target_posy',-2,0,0,'目的点',0,0,1);
INSERT INTO `net_struct_conf` VALUES (47000,47,0,'unit_index',-3,0,0,'地图单元-序号',0,0,1);
INSERT INTO `net_struct_conf` VALUES (47001,47,1,'posx',-2,0,0,'地图单元-当前坐标',0,0,1);
INSERT INTO `net_struct_conf` VALUES (47002,47,2,'posy',-2,0,0,'地图单元-当前坐标',0,0,1);
INSERT INTO `net_struct_conf` VALUES (48000,48,0,'from_type',-1,0,0,'行军路线-出发单元类型',0,0,1);
INSERT INTO `net_struct_conf` VALUES (48001,48,1,'from_posx',-2,0,0,'行军路线-出发位置',0,0,1);
INSERT INTO `net_struct_conf` VALUES (48002,48,2,'from_posy',-2,0,0,'行军路线-出发位置',0,0,1);
INSERT INTO `net_struct_conf` VALUES (48003,48,3,'to_type',-1,0,0,'行军路线-到达单元类型',0,0,1);
INSERT INTO `net_struct_conf` VALUES (48004,48,4,'to_posx',-2,0,0,'行军路线-到达位置',0,0,1);
INSERT INTO `net_struct_conf` VALUES (48005,48,5,'to_posy',-2,0,0,'行军路线-到达位置',0,0,1);
INSERT INTO `net_struct_conf` VALUES (48006,48,6,'state',-1,0,0,'行军路线-状态',0,0,1);
INSERT INTO `net_struct_conf` VALUES (48007,48,7,'from_actorid',-3,0,0,'行军路线-城市',0,0,1);
INSERT INTO `net_struct_conf` VALUES (48008,48,8,'from_clubid',-3,0,0,'行军路线-联盟',0,0,1);
INSERT INTO `net_struct_conf` VALUES (48009,48,9,'to_actorid',-3,0,0,'行军路线-目标城市',0,0,1);
INSERT INTO `net_struct_conf` VALUES (48010,48,10,'to_clubid',-3,0,0,'行军路线-目标联盟',0,0,1);
INSERT INTO `net_struct_conf` VALUES (48011,48,11,'army_index',-3,0,0,'行军路线-所属部队',0,0,1);
INSERT INTO `net_struct_conf` VALUES (48012,48,12,'action',-1,0,0,'行军路线-行为',0,0,1);
INSERT INTO `net_struct_conf` VALUES (49000,49,0,'army_index',-3,0,0,'删除线-部队索引',0,0,1);
INSERT INTO `net_struct_conf` VALUES (50000,50,0,'info',44,0,0,'更新地图显示单元信息',0,0,1);
INSERT INTO `net_struct_conf` VALUES (77000,77,0,'kind',-3,0,0,'种类',0,0,1);
INSERT INTO `net_struct_conf` VALUES (77001,77,1,'num',-3,0,0,'数量',0,0,1);
INSERT INTO `net_struct_conf` VALUES (78000,78,0,'count',-2,0,0,'奖励数量',0,0,1);
INSERT INTO `net_struct_conf` VALUES (78001,78,1,'list',77,-1,32,'奖励列表',0,0,1);
INSERT INTO `net_struct_conf` VALUES (78002,78,2,'callback_code',-2,0,0,'哪个系统要显示，回传',0,0,1);
INSERT INTO `net_struct_conf` VALUES (79000,79,0,'addexp',-3,0,0,'服务器发送-获取经验',0,0,1);
INSERT INTO `net_struct_conf` VALUES (79001,79,1,'curexp',-4,0,0,'服务器发送-获取经验',0,0,1);
INSERT INTO `net_struct_conf` VALUES (79002,79,2,'isup',-1,0,0,'服务器发送-获取经验',0,0,1);
INSERT INTO `net_struct_conf` VALUES (79003,79,4,'path',-2,0,0,'服务器发送-获取经验',0,0,1);
INSERT INTO `net_struct_conf` VALUES (79004,79,5,'level',-2,0,0,'服务器发送-获取经验',0,0,1);
INSERT INTO `net_struct_conf` VALUES (79005,79,6,'expmax',-3,0,0,'服务器发送-获取经验',0,0,1);
INSERT INTO `net_struct_conf` VALUES (80000,80,0,'total',-2,0,0,'体力',0,0,1);
INSERT INTO `net_struct_conf` VALUES (80001,80,1,'add',-2,0,0,'获取体力',0,0,1);
INSERT INTO `net_struct_conf` VALUES (80002,80,2,'path',-2,0,0,'途径',0,0,1);
INSERT INTO `net_struct_conf` VALUES (81000,81,0,'total',-2,0,0,'改变征收次数',0,0,1);
INSERT INTO `net_struct_conf` VALUES (81001,81,1,'add',-2,0,0,'改变征收次数',0,0,1);
INSERT INTO `net_struct_conf` VALUES (81002,81,2,'max',-2,0,0,'改变征收次数',0,0,1);
INSERT INTO `net_struct_conf` VALUES (81003,81,3,'sec',-3,0,0,'改变征收次数',0,0,1);
INSERT INTO `net_struct_conf` VALUES (81004,81,4,'path',-2,0,0,'改变征收次数',0,0,1);
INSERT INTO `net_struct_conf` VALUES (82000,82,0,'total',-3,0,0,'改变银币',0,0,1);
INSERT INTO `net_struct_conf` VALUES (82001,82,1,'add',-3,0,0,'改变银币',0,0,1);
INSERT INTO `net_struct_conf` VALUES (82002,82,2,'path',-2,0,0,'改变银币',0,0,1);
INSERT INTO `net_struct_conf` VALUES (83000,83,0,'total',-3,0,0,'改变木材',0,0,1);
INSERT INTO `net_struct_conf` VALUES (83001,83,1,'add',-3,0,0,'改变木材',0,0,1);
INSERT INTO `net_struct_conf` VALUES (83002,83,2,'path',-2,0,0,'改变木材',0,0,1);
INSERT INTO `net_struct_conf` VALUES (84000,84,0,'total',-3,0,0,'改变粮食',0,0,1);
INSERT INTO `net_struct_conf` VALUES (84001,84,1,'add',-3,0,0,'改变粮食',0,0,1);
INSERT INTO `net_struct_conf` VALUES (84002,84,2,'path',-2,0,0,'改变粮食',0,0,1);
INSERT INTO `net_struct_conf` VALUES (85000,85,0,'total',-3,0,0,'改变镔铁',0,0,1);
INSERT INTO `net_struct_conf` VALUES (85001,85,1,'add',-3,0,0,'改变镔铁',0,0,1);
INSERT INTO `net_struct_conf` VALUES (85002,85,2,'path',-2,0,0,'改变镔铁',0,0,1);
INSERT INTO `net_struct_conf` VALUES (86000,86,0,'total',-3,0,0,'改变人口',0,0,1);
INSERT INTO `net_struct_conf` VALUES (86001,86,1,'add',-3,0,0,'改变人口',0,0,1);
INSERT INTO `net_struct_conf` VALUES (86002,86,2,'path',-2,0,0,'改变人口',0,0,1);
INSERT INTO `net_struct_conf` VALUES (87000,87,0,'total',-3,0,0,'改变威望值',0,0,1);
INSERT INTO `net_struct_conf` VALUES (87001,87,1,'add',-3,0,0,'改变威望值',0,0,1);
INSERT INTO `net_struct_conf` VALUES (87002,87,2,'path',-2,0,0,'改变威望值',0,0,1);
INSERT INTO `net_struct_conf` VALUES (88000,88,0,'total',-3,0,0,'改变友谊积分',0,0,1);
INSERT INTO `net_struct_conf` VALUES (88001,88,1,'add',-3,0,0,'改变友谊积分',0,0,1);
INSERT INTO `net_struct_conf` VALUES (88002,88,2,'path',-2,0,0,'改变友谊积分',0,0,1);
INSERT INTO `net_struct_conf` VALUES (89000,89,0,'addexp',-3,0,0,'改变VIP经验',0,0,1);
INSERT INTO `net_struct_conf` VALUES (89001,89,1,'curexp',-3,0,0,'改变VIP经验',0,0,1);
INSERT INTO `net_struct_conf` VALUES (89002,89,2,'expmax',-3,0,0,'改变VIP经验',0,0,1);
INSERT INTO `net_struct_conf` VALUES (89003,89,3,'isup',-1,0,0,'改变VIP经验',0,0,1);
INSERT INTO `net_struct_conf` VALUES (89004,89,4,'level',-1,0,0,'改变VIP经验',0,0,1);
INSERT INTO `net_struct_conf` VALUES (89005,89,5,'path',-2,0,0,'改变VIP经验',0,0,1);
INSERT INTO `net_struct_conf` VALUES (90000,90,0,'total',-3,0,0,'改变钻石',0,0,1);
INSERT INTO `net_struct_conf` VALUES (90001,90,1,'add',-3,0,0,'改变钻石',0,0,1);
INSERT INTO `net_struct_conf` VALUES (90002,90,2,'path',-2,0,0,'改变钻石',0,0,1);

#
# Source for table netccmd
#

CREATE TABLE `netccmd` (
  `cmdid` int(11) NOT NULL AUTO_INCREMENT,
  `cmdstr` varbinary(32) NOT NULL DEFAULT '',
  `structid` int(11) NOT NULL DEFAULT '0',
  `target_type` tinyint(3) NOT NULL DEFAULT '0' COMMENT '1queue',
  `context` varbinary(128) NOT NULL DEFAULT '',
  `unuse` tinyint(3) NOT NULL DEFAULT '0',
  PRIMARY KEY (`cmdid`)
) ENGINE=MyISAM AUTO_INCREMENT=11 DEFAULT CHARSET=gbk ROW_FORMAT=DYNAMIC;

#
# Dumping data for table netccmd
#

INSERT INTO `netccmd` VALUES (-4,'userawarded',14,0,'用户服务器发过来的奖励',0);
INSERT INTO `netccmd` VALUES (-3,'gmlocalcmd',15,0,'GM本地指令',0);
INSERT INTO `netccmd` VALUES (-2,'logined',2,0,'用户服务器发送登陆信息',0);
INSERT INTO `netccmd` VALUES (-1,'timer',-1,0,'服务器本地时间消息',0);
INSERT INTO `netccmd` VALUES (1,'login',1,0,'请求登录',0);
INSERT INTO `netccmd` VALUES (2,'create',6,1,'请求创建角色',0);
INSERT INTO `netccmd` VALUES (3,'list',-1,1,'请求角色列表',0);
INSERT INTO `netccmd` VALUES (4,'entergame',-3,1,'进入游戏',0);
INSERT INTO `netccmd` VALUES (5,'delete',-3,0,'删除角色',0);
INSERT INTO `netccmd` VALUES (6,'heart',10,0,'心跳',0);
INSERT INTO `netccmd` VALUES (7,'gmcmd',12,0,'GM指令',0);
INSERT INTO `netccmd` VALUES (9,'askinfo',16,0,'短请求信息',0);
INSERT INTO `netccmd` VALUES (10,'entercity',17,0,'进入城池所有信息',0);

#
# Source for table netgcmd
#

CREATE TABLE `netgcmd` (
  `cmdid` int(11) NOT NULL AUTO_INCREMENT,
  `cmdstr` varbinary(32) NOT NULL DEFAULT '',
  `structid` int(11) NOT NULL DEFAULT '0',
  `target_type` tinyint(3) NOT NULL DEFAULT '0' COMMENT '如果为1表示以队列的方式，通常为创建角色等数据操作',
  `context` varbinary(128) NOT NULL DEFAULT '',
  `unuse` tinyint(3) NOT NULL DEFAULT '0',
  PRIMARY KEY (`cmdid`)
) ENGINE=MyISAM DEFAULT CHARSET=gbk ROW_FORMAT=DYNAMIC;

#
# Dumping data for table netgcmd
#


#
# Source for table netscmd
#

CREATE TABLE `netscmd` (
  `cmdid` int(11) NOT NULL AUTO_INCREMENT,
  `cmdstr` varbinary(32) NOT NULL DEFAULT '',
  `structid` int(11) NOT NULL DEFAULT '0',
  `target_type` tinyint(4) NOT NULL DEFAULT '0',
  `context` varbinary(128) NOT NULL DEFAULT '',
  `nolua` tinyint(3) NOT NULL DEFAULT '0',
  `unuse` tinyint(3) NOT NULL DEFAULT '0',
  PRIMARY KEY (`cmdid`)
) ENGINE=MyISAM AUTO_INCREMENT=38 DEFAULT CHARSET=gbk ROW_FORMAT=DYNAMIC;

#
# Dumping data for table netscmd
#

INSERT INTO `netscmd` VALUES (1,'login',3,0,'登录信息',0,0);
INSERT INTO `netscmd` VALUES (2,'list',5,0,'角色列表',0,0);
INSERT INTO `netscmd` VALUES (3,'create',7,0,'返回创建角色信息',0,0);
INSERT INTO `netscmd` VALUES (4,'enterinfo',8,0,'进入游戏的信息',0,0);
INSERT INTO `netscmd` VALUES (5,'delete',9,0,'删除角色',0,0);
INSERT INTO `netscmd` VALUES (6,'heart',11,0,'心跳',0,0);
INSERT INTO `netscmd` VALUES (7,'notify',13,0,'短消息',0,0);
INSERT INTO `netscmd` VALUES (8,'actorinfo',29,0,'角色基本信息',0,0);
INSERT INTO `netscmd` VALUES (9,'itemuse',31,0,'道具使用',0,0);
INSERT INTO `netscmd` VALUES (10,'itemput',32,0,'道具放置|卸下',0,0);
INSERT INTO `netscmd` VALUES (11,'itemsettle',33,0,'道具整理',0,0);
INSERT INTO `netscmd` VALUES (12,'lostitem',34,0,'丢失道具',0,0);
INSERT INTO `netscmd` VALUES (13,'getitem',35,0,'获得道具',0,0);
INSERT INTO `netscmd` VALUES (14,'itemlist',37,0,'物品列表',0,0);
INSERT INTO `netscmd` VALUES (15,'iteminfo',39,0,'道具信息',0,0);
INSERT INTO `netscmd` VALUES (16,'addmapunit',44,10,'显示单元进入地图',0,0);
INSERT INTO `netscmd` VALUES (17,'delmapunit',45,10,'显示单元离开地图',0,0);
INSERT INTO `netscmd` VALUES (18,'updatemapunit',50,10,'更新地图显示单元信息',0,0);
INSERT INTO `netscmd` VALUES (19,'mapunitsingle',44,0,'地图单元 发送一个信息',0,0);
INSERT INTO `netscmd` VALUES (20,'worldmapinfo',46,0,'世界地图信息',0,0);
INSERT INTO `netscmd` VALUES (21,'mapunitcorrdinate',47,10,'地图显示单元位置',0,0);
INSERT INTO `netscmd` VALUES (22,'addmarchroute',48,10,'添加行军路线',0,0);
INSERT INTO `netscmd` VALUES (23,'delmarchroute',49,10,'删除行军路线',0,0);
INSERT INTO `netscmd` VALUES (24,'awardinfolist',78,0,'奖励信息列表',0,0);
INSERT INTO `netscmd` VALUES (25,'experience',79,0,'获得经验',0,0);
INSERT INTO `netscmd` VALUES (26,'body',80,0,'体力',0,0);
INSERT INTO `netscmd` VALUES (27,'buildinglist',28,0,'建筑列表',0,0);
INSERT INTO `netscmd` VALUES (28,'changelevy',81,0,'征收次数',0,0);
INSERT INTO `netscmd` VALUES (29,'changesilver',82,0,'银币',0,0);
INSERT INTO `netscmd` VALUES (30,'changewood',83,0,'木材',0,0);
INSERT INTO `netscmd` VALUES (31,'changefood',84,0,'粮食',0,0);
INSERT INTO `netscmd` VALUES (32,'changeiron',85,0,'镔铁',0,0);
INSERT INTO `netscmd` VALUES (33,'changepeople',86,0,'人口',0,0);
INSERT INTO `netscmd` VALUES (34,'changeprestige',87,0,'威望',0,0);
INSERT INTO `netscmd` VALUES (35,'changefriendship',88,0,'友谊积分',0,0);
INSERT INTO `netscmd` VALUES (36,'changevip',89,0,'VIP',0,0);
INSERT INTO `netscmd` VALUES (37,'changtoken',90,0,'改变钻石',0,0);

#
# Source for table scsrv_c_struct
#

CREATE TABLE `scsrv_c_struct` (
  `structid` int(11) NOT NULL AUTO_INCREMENT,
  `name` varbinary(64) NOT NULL DEFAULT '',
  `context` varbinary(128) NOT NULL DEFAULT '',
  `limitcount` smallint(6) NOT NULL DEFAULT '0',
  `t1` tinyint(3) NOT NULL DEFAULT '0',
  `arg1` varbinary(32) NOT NULL DEFAULT '',
  `t2` tinyint(3) NOT NULL DEFAULT '0',
  `arg2` varbinary(32) NOT NULL DEFAULT '',
  `t3` tinyint(3) NOT NULL DEFAULT '0',
  `arg3` varbinary(32) NOT NULL DEFAULT '',
  `t4` tinyint(3) NOT NULL DEFAULT '0',
  `arg4` varbinary(32) NOT NULL DEFAULT '',
  `t5` tinyint(3) NOT NULL DEFAULT '0',
  `arg5` varbinary(32) NOT NULL DEFAULT '',
  `t6` tinyint(3) NOT NULL DEFAULT '0',
  `arg6` varbinary(32) NOT NULL DEFAULT '',
  `t7` tinyint(3) NOT NULL DEFAULT '0',
  `arg7` varbinary(32) NOT NULL DEFAULT '',
  `t8` tinyint(3) NOT NULL DEFAULT '0',
  `arg8` varbinary(32) NOT NULL DEFAULT '',
  `t9` tinyint(3) NOT NULL DEFAULT '0',
  `arg9` varbinary(32) NOT NULL DEFAULT '',
  `t10` tinyint(3) NOT NULL DEFAULT '0',
  `arg10` varbinary(32) NOT NULL DEFAULT '',
  `t11` tinyint(3) NOT NULL DEFAULT '0',
  `arg11` varbinary(32) NOT NULL DEFAULT '',
  `t12` tinyint(3) NOT NULL DEFAULT '0',
  `arg12` varbinary(32) NOT NULL DEFAULT '',
  `t13` tinyint(3) NOT NULL DEFAULT '0',
  `arg13` varbinary(32) NOT NULL DEFAULT '',
  `t14` tinyint(3) NOT NULL DEFAULT '0',
  `arg14` varbinary(32) NOT NULL DEFAULT '',
  `t15` tinyint(3) NOT NULL DEFAULT '0',
  `arg15` varbinary(32) NOT NULL DEFAULT '',
  `t16` tinyint(3) NOT NULL DEFAULT '0',
  `arg16` varbinary(32) NOT NULL DEFAULT '',
  PRIMARY KEY (`structid`)
) ENGINE=MyISAM AUTO_INCREMENT=211 DEFAULT CHARSET=gbk ROW_FORMAT=DYNAMIC;

#
# Dumping data for table scsrv_c_struct
#

INSERT INTO `scsrv_c_struct` VALUES (1,'c_item_name','获取物品名称',0,-3,'itemkind',9,'itemname',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'\"\"');
INSERT INTO `scsrv_c_struct` VALUES (2,'c_item_hasone','判断是否拥有某道具',0,-3,'actor_index',-3,'itemkind',1,'rtn',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'\"\"');
INSERT INTO `scsrv_c_struct` VALUES (3,'c_item_emptycount','获得空余背包个数',0,-3,'actor_index',3,'count',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'\"\"');
INSERT INTO `scsrv_c_struct` VALUES (4,'c_item_lost','剥夺玩家的道具（根据物品kind）',3,-3,'actor_index',-3,'itemkind',-3,'count',-3,'path',3,'rtn',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'\"\"');
INSERT INTO `scsrv_c_struct` VALUES (5,'c_item_lostitem','丢掉玩家物品（根据背包索引）',0,-3,'actor_index',-3,'itemoffset',-3,'count',-3,'path',3,'rtn',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'\"\"');
INSERT INTO `scsrv_c_struct` VALUES (6,'c_item_getitem','获取物品（全参数）',0,-3,'actor_index',-3,'itemkind',-3,'count',-1,'color',-1,'path',3,'rtn',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'\"\"');
INSERT INTO `scsrv_c_struct` VALUES (7,'c_item_get_base_ability','获取道具指定位置的属性（基础）',0,-3,'itemkind',-1,'pos',2,'ability',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'\"\"');
INSERT INTO `scsrv_c_struct` VALUES (8,'c_item_get_base_value','获取道具指定位置的数值（基础）',0,-3,'itemkind',-1,'pos',3,'value',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'\"\"');
INSERT INTO `scsrv_c_struct` VALUES (9,'c_item_get_ability_value','根据ability获取道具value（基础+动态）',0,-3,'itemkind',-2,'ability',3,'value',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'\"\"');
INSERT INTO `scsrv_c_struct` VALUES (10,'c_item_set_abilityvalue_offset','根据背包索引获取物品属性',0,-3,'actor_index',-3,'item_offset',-1,'pos',-2,'ability',-3,'value',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'\"\"');
INSERT INTO `scsrv_c_struct` VALUES (11,'c_item_get_abilityvalue_offset','根据背包索引设置物品属性',0,-3,'actor_index',-3,'item_offset',-1,'pos',2,'rtn_ability',3,'rtn_value',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'\"\"');
INSERT INTO `scsrv_c_struct` VALUES (12,'c_item_getinfo','获取物品信息',0,-3,'itemkind',9,'name',2,'level',1,'color',3,'price',3,'type',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'\"\"');
INSERT INTO `scsrv_c_struct` VALUES (13,'c_actor_info','获得角色信息',0,-3,'actor_index',3,'actorid',3,'nation',3,'level',9,'name',2,'language',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'\"\"');
INSERT INTO `scsrv_c_struct` VALUES (14,'c_actor_notify','提示信息',2,-3,'actor_index',-9,'msg',-3,'msgid',0,'0',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'\"\"');
INSERT INTO `scsrv_c_struct` VALUES (15,'c_actor_notify_value','提示信息带变量',3,-3,'actor_index',-9,'msg',-3,'msgid',-3,'value1',-3,'value2',-3,'value3',-3,'value4',-3,'value5',-3,'value6',-3,'value7',-3,'value8',-3,'value9',-3,'value10',0,'',0,'',0,'\"\"');
INSERT INTO `scsrv_c_struct` VALUES (16,'c_actor_add_today_char_times','添加每日信息(char)',0,-3,'actor_index',-2,'offset',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'');
INSERT INTO `scsrv_c_struct` VALUES (17,'c_actor_get_today_char_times','获得每日信息(char)',0,-3,'actor_index',-2,'offset',3,'rtn',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'\"\"');
INSERT INTO `scsrv_c_struct` VALUES (18,'c_actor_set_today_char_times','设置每日信息(char)',0,-3,'actor_index',-2,'offset',-3,'value',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'\"\"');
INSERT INTO `scsrv_c_struct` VALUES (19,'c_actor_add_today_int_times','添加每日次数(int)',0,-3,'actor_index',-2,'offset',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'\"\"');
INSERT INTO `scsrv_c_struct` VALUES (20,'c_actor_get_today_int_times','获得每日次数(int)',0,-3,'actor_index',-2,'offset',3,'rtn',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'\"\"');
INSERT INTO `scsrv_c_struct` VALUES (21,'c_actor_set_today_int_times','设置每日次数(int)',0,-3,'actor_index',-2,'offset',-3,'value',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'');
INSERT INTO `scsrv_c_struct` VALUES (22,'c_actor_set_sflag','0,1的标志操作',0,-3,'actor_index',-2,'offset',-2,'value',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'\"\"');
INSERT INTO `scsrv_c_struct` VALUES (23,'c_actor_get_sflag','0,1的标志操作',0,-3,'actor_index',-2,'offset',2,'value',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'\"\"');
INSERT INTO `scsrv_c_struct` VALUES (24,'c_actor_set_uselimit_cd','设置玩家CD限制',0,-3,'actor_index',-1,'index',-3,'cdtime',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'\"\"');
INSERT INTO `scsrv_c_struct` VALUES (25,'c_actor_get_uselimit_cd','检查玩家CD限制',0,-3,'actor_index',-1,'index',3,'rtn',0,'0',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'\"\"');
INSERT INTO `scsrv_c_struct` VALUES (26,'c_actor_add_week_times','周数据',0,-3,'actor_index',-3,'offset',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'\"\"');
INSERT INTO `scsrv_c_struct` VALUES (27,'c_actor_get_week_times','周数据',0,-3,'actor_index',-3,'offset',3,'value',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'\"\"');
INSERT INTO `scsrv_c_struct` VALUES (60,'c_map_addobject','添加地图格子数据',0,-1,'type',-2,'posx',-2,'posy',-1,'unittype',3,'rtn',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'');
INSERT INTO `scsrv_c_struct` VALUES (61,'c_map_delobject','删除地图格子数据',0,-1,'type',-2,'posx',-2,'posy',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'');
INSERT INTO `scsrv_c_struct` VALUES (62,'c_map_getrandpos','随机一个空白位置',0,-1,'type',2,'posx',2,'posy',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'');
INSERT INTO `scsrv_c_struct` VALUES (190,'c_world_data_get','世界数据',0,-3,'id',3,'value',9,'strvalue',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'');
INSERT INTO `scsrv_c_struct` VALUES (191,'c_world_data_set','世界数据',2,-3,'id',-3,'value',-9,'strvalue',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'');
INSERT INTO `scsrv_c_struct` VALUES (200,'c_system_getruntime','获取开服流逝时间',0,3,'time',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'');
INSERT INTO `scsrv_c_struct` VALUES (201,'c_system_getopentime','获取开服时间',0,3,'time',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'');
INSERT INTO `scsrv_c_struct` VALUES (202,'c_system_getfday','获取天数',0,3,'value',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'');
INSERT INTO `scsrv_c_struct` VALUES (203,'c_system_gettoday','获取日期天',0,3,'value',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'');
INSERT INTO `scsrv_c_struct` VALUES (204,'c_system_getweek','获取日期周',0,3,'value',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'');
INSERT INTO `scsrv_c_struct` VALUES (205,'c_system_getmonth','获取日期月',0,3,'value',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'');
INSERT INTO `scsrv_c_struct` VALUES (210,'c_award_getaward','给奖励',0,-3,'actor_index',-3,'kind',-3,'count',-1,'color',-1,'path',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'');

#
# Source for table scsrv_in_struct
#

CREATE TABLE `scsrv_in_struct` (
  `structid` int(11) NOT NULL AUTO_INCREMENT,
  `name` varbinary(32) NOT NULL DEFAULT '',
  `context` varbinary(128) NOT NULL DEFAULT '',
  `limitcount` smallint(6) NOT NULL DEFAULT '0',
  `t1` tinyint(3) NOT NULL DEFAULT '0',
  `arg1` varbinary(32) NOT NULL DEFAULT '',
  `t2` tinyint(3) NOT NULL DEFAULT '0',
  `arg2` varbinary(32) NOT NULL DEFAULT '',
  `t3` tinyint(3) NOT NULL DEFAULT '0',
  `arg3` varbinary(32) NOT NULL DEFAULT '',
  `t4` tinyint(3) NOT NULL DEFAULT '0',
  `arg4` varbinary(32) NOT NULL DEFAULT '',
  `t5` tinyint(3) NOT NULL DEFAULT '0',
  `arg5` varbinary(32) NOT NULL DEFAULT '',
  `t6` tinyint(3) NOT NULL DEFAULT '0',
  `arg6` varbinary(32) NOT NULL DEFAULT '',
  `t7` tinyint(3) NOT NULL DEFAULT '0',
  `arg7` varbinary(32) NOT NULL DEFAULT '',
  `t8` tinyint(3) NOT NULL DEFAULT '0',
  `arg8` varbinary(32) NOT NULL DEFAULT '',
  `t9` tinyint(3) NOT NULL DEFAULT '0',
  `arg9` varbinary(32) NOT NULL DEFAULT '',
  `t10` tinyint(3) NOT NULL DEFAULT '0',
  `arg10` varbinary(32) NOT NULL DEFAULT '',
  `t11` tinyint(3) NOT NULL DEFAULT '0',
  `arg11` varbinary(32) NOT NULL DEFAULT '',
  `t12` tinyint(3) NOT NULL DEFAULT '0',
  `arg12` varbinary(32) NOT NULL DEFAULT '',
  `t13` tinyint(3) NOT NULL DEFAULT '0',
  `arg13` varbinary(32) NOT NULL DEFAULT '',
  `t14` tinyint(3) NOT NULL DEFAULT '0',
  `arg14` varbinary(32) NOT NULL DEFAULT '',
  `t15` tinyint(3) NOT NULL DEFAULT '0',
  `arg15` varbinary(32) NOT NULL DEFAULT '',
  `t16` tinyint(3) NOT NULL DEFAULT '0',
  `arg16` varbinary(32) NOT NULL DEFAULT '',
  PRIMARY KEY (`structid`)
) ENGINE=MyISAM AUTO_INCREMENT=19 DEFAULT CHARSET=gbk ROW_FORMAT=DYNAMIC;

#
# Dumping data for table scsrv_in_struct
#

INSERT INTO `scsrv_in_struct` VALUES (1,'Script_Init','脚本初始化',0,0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'\"\"');
INSERT INTO `scsrv_in_struct` VALUES (2,'Script_Timer','脚本逻辑每分钟一次',0,0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'\"\"');
INSERT INTO `scsrv_in_struct` VALUES (3,'Script_Command','脚本GM命令',0,-3,'nValue1',-3,'nValue2',-3,'nValue3',-3,'nValue4',-9,'pMsg',-3,'actor_index',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'\"\"');
INSERT INTO `scsrv_in_struct` VALUES (4,'Script_Exec','脚本执行',0,-3,'id',-3,'value1',-3,'value2',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'');
INSERT INTO `scsrv_in_struct` VALUES (5,'Script_Hour','脚本逻辑每小时一次',0,0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'');
INSERT INTO `scsrv_in_struct` VALUES (6,'OnClockProcess','整点执行',0,-3,'hour',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'');
INSERT INTO `scsrv_in_struct` VALUES (7,'ClientMsg','来自客户端的脚本请求',0,-3,'nPlayerIndex',-3,'nCmd',-3,'nValue1',-3,'nValue2',-3,'nValue3',-3,'nValue4',-9,'pMsg',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'\"\"');
INSERT INTO `scsrv_in_struct` VALUES (8,'ActivityOnOpen','活动开启',0,-3,'activityid',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'');
INSERT INTO `scsrv_in_struct` VALUES (9,'ActivityOnLogic','活动逻辑',0,-3,'activityid',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'');
INSERT INTO `scsrv_in_struct` VALUES (10,'ActivityOnEnd','活动结束',0,-3,'activityid',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'\"\"');
INSERT INTO `scsrv_in_struct` VALUES (11,'ActivityOnClose','活动关闭',0,-3,'activityid',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'');
INSERT INTO `scsrv_in_struct` VALUES (12,'UseItem','使用物品',0,-3,'nPlayerIndex',-3,'nMember',-3,'nItemIndex',-3,'nItemKind',-3,'nUseNum',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'\"\"');
INSERT INTO `scsrv_in_struct` VALUES (13,'OnActorIn','当角色进入游戏',0,-3,'nPlayerIndex',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'\"\"');
INSERT INTO `scsrv_in_struct` VALUES (14,'OnMailReadAttach','读取附件',0,-3,'nPlayerIndex',-9,'szAttach',3,'rtn',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'');
INSERT INTO `scsrv_in_struct` VALUES (15,'TimeString','时间戳转换时间字符串',0,-3,'timestamp',9,'out',3,'len',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'');
INSERT INTO `scsrv_in_struct` VALUES (16,'OnWorldMapInit','地图初始化',0,-3,'nMaxWidth',-3,'nMaxHeight',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'');
INSERT INTO `scsrv_in_struct` VALUES (17,'OnWorldMapBlock','地图阻挡',0,-2,'posx',-2,'posy',3,'rtn',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'');
INSERT INTO `scsrv_in_struct` VALUES (18,'OnWorldMapBrush','刷地图',0,0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'',0,'');

#
# Source for table st_actor_conf
#

CREATE TABLE `st_actor_conf` (
  `dataid` int(11) NOT NULL AUTO_INCREMENT,
  `offset` int(11) NOT NULL DEFAULT '0',
  `name` varbinary(32) NOT NULL DEFAULT '',
  `data_type` smallint(6) NOT NULL DEFAULT '0',
  `data_count` smallint(6) NOT NULL DEFAULT '0',
  `data_size` smallint(6) NOT NULL DEFAULT '0',
  `context` varbinary(128) NOT NULL DEFAULT '',
  `notsave` tinyint(4) NOT NULL DEFAULT '0',
  `kind` tinyint(4) NOT NULL DEFAULT '0',
  `m_fix` tinyint(4) NOT NULL DEFAULT '0',
  PRIMARY KEY (`dataid`)
) ENGINE=MyISAM AUTO_INCREMENT=125 DEFAULT CHARSET=gbk ROW_FORMAT=DYNAMIC;

#
# Dumping data for table st_actor_conf
#

INSERT INTO `st_actor_conf` VALUES (1,1,'actorid',-3,0,0,'角色编号',0,1,0);
INSERT INTO `st_actor_conf` VALUES (2,2,'accountid',-3,0,0,'登陆id',1,0,0);
INSERT INTO `st_actor_conf` VALUES (3,3,'userid',-4,0,0,'账号编号',3,0,0);
INSERT INTO `st_actor_conf` VALUES (4,4,'username',-1,51,0,'账号名',3,0,0);
INSERT INTO `st_actor_conf` VALUES (5,5,'admin',-2,0,0,'管理员级别',2,0,0);
INSERT INTO `st_actor_conf` VALUES (6,6,'lastip',-1,16,0,'上次登陆ip',0,0,0);
INSERT INTO `st_actor_conf` VALUES (7,7,'createtime',-3,0,0,'创建时间',0,0,0);
INSERT INTO `st_actor_conf` VALUES (8,8,'forbidtime',-3,0,0,'禁言时间',0,0,0);
INSERT INTO `st_actor_conf` VALUES (9,9,'savetime',-3,0,0,'上次保存时间',1,0,0);
INSERT INTO `st_actor_conf` VALUES (10,10,'isexit',-11,0,0,'是否离开游戏中',1,0,0);
INSERT INTO `st_actor_conf` VALUES (12,12,'view_areaindex',-2,0,0,'当前视口的地图区域',1,0,0);
INSERT INTO `st_actor_conf` VALUES (13,13,'name',-1,22,0,'角色名称',0,0,0);
INSERT INTO `st_actor_conf` VALUES (14,14,'nation',-1,0,0,'角色国家',0,0,0);
INSERT INTO `st_actor_conf` VALUES (15,15,'shape',-11,0,0,'形象',0,0,0);
INSERT INTO `st_actor_conf` VALUES (16,16,'level',-2,0,0,'级别',0,0,0);
INSERT INTO `st_actor_conf` VALUES (20,20,'city_index',-3,0,0,'城池索引',1,0,0);
INSERT INTO `st_actor_conf` VALUES (21,21,'token',-3,0,0,'钻石',0,0,0);
INSERT INTO `st_actor_conf` VALUES (22,22,'total_charge',-3,0,0,'总充值',0,0,0);
INSERT INTO `st_actor_conf` VALUES (23,23,'charge_dollar',-5,0,0,'总消费多少钱',0,0,0);
INSERT INTO `st_actor_conf` VALUES (24,24,'itemext',-11,0,0,'扩展的背包栏位数',0,0,0);
INSERT INTO `st_actor_conf` VALUES (25,25,'item',-102,300,0,'角色携带的道具',1,0,0);
INSERT INTO `st_actor_conf` VALUES (26,26,'equip',-109,100,0,'角色携带的装备',1,0,0);
INSERT INTO `st_actor_conf` VALUES (27,27,'equipext',-11,0,0,'扩展的装备栏位数',0,0,0);
INSERT INTO `st_actor_conf` VALUES (28,28,'hero',-108,128,0,'未上阵英雄',1,0,0);
INSERT INTO `st_actor_conf` VALUES (37,37,'sflag',-3,0,0,'特定标志位',0,0,0);
INSERT INTO `st_actor_conf` VALUES (38,38,'fdate',-3,0,0,'刷新的日子是哪天',0,0,0);
INSERT INTO `st_actor_conf` VALUES (39,39,'today_char',-7,-1,128,'char类型每日次数',0,0,0);
INSERT INTO `st_actor_conf` VALUES (40,40,'today_int',-3,8,0,'int类型每日次数',0,0,0);
INSERT INTO `st_actor_conf` VALUES (41,41,'cd',-3,8,0,'通用CD时间',0,0,0);
INSERT INTO `st_actor_conf` VALUES (42,42,'config',-7,-1,8,'玩家配置',0,0,0);
INSERT INTO `st_actor_conf` VALUES (43,43,'function',-3,0,0,'功能是否开启',0,0,0);
INSERT INTO `st_actor_conf` VALUES (64,64,'view_lastframe',-3,0,0,'上次发送的区域信息时间',1,0,0);
INSERT INTO `st_actor_conf` VALUES (65,65,'subscribe_cmd',-2,8,0,'订阅的消息ID',1,0,0);
INSERT INTO `st_actor_conf` VALUES (80,80,'cdkey1',-3,0,0,'是否领取该批次兑换码',0,0,0);
INSERT INTO `st_actor_conf` VALUES (81,81,'cdkey2',-3,0,0,'是否领取该批次兑换码',0,0,0);
INSERT INTO `st_actor_conf` VALUES (82,82,'cdkey3',-3,0,0,'是否领取该批次兑换码',0,0,0);
INSERT INTO `st_actor_conf` VALUES (83,83,'cdkey4',-3,0,0,'是否领取该批次兑换码',0,0,0);
INSERT INTO `st_actor_conf` VALUES (122,122,'talkspeed_frame',-3,0,0,'说话延迟',1,0,0);
INSERT INTO `st_actor_conf` VALUES (124,124,'cdkeywait',-1,0,0,'CDKEY等待',1,0,0);

#
# Source for table st_actor_equip_conf
#

CREATE TABLE `st_actor_equip_conf` (
  `dataid` int(11) NOT NULL AUTO_INCREMENT,
  `offset` int(11) NOT NULL DEFAULT '0',
  `name` varbinary(32) NOT NULL DEFAULT '',
  `data_type` smallint(6) NOT NULL DEFAULT '0',
  `data_count` smallint(6) NOT NULL DEFAULT '0',
  `data_size` smallint(6) NOT NULL DEFAULT '0',
  `context` varbinary(128) NOT NULL DEFAULT '',
  `notsave` tinyint(4) NOT NULL DEFAULT '0',
  `kind` tinyint(4) NOT NULL DEFAULT '0',
  `m_fix` tinyint(4) NOT NULL DEFAULT '0',
  PRIMARY KEY (`dataid`)
) ENGINE=MyISAM AUTO_INCREMENT=25 DEFAULT CHARSET=gbk ROW_FORMAT=DYNAMIC;

#
# Dumping data for table st_actor_equip_conf
#

INSERT INTO `st_actor_equip_conf` VALUES (1,1,'id',-4,0,0,'主索引',0,1,0);
INSERT INTO `st_actor_equip_conf` VALUES (2,2,'actorid',-3,0,0,'角色ID',0,2,0);
INSERT INTO `st_actor_equip_conf` VALUES (3,3,'offset',-3,0,0,'装备所在背包位置',0,3,0);
INSERT INTO `st_actor_equip_conf` VALUES (4,4,'kind',-3,0,0,'物品种类',0,0,0);
INSERT INTO `st_actor_equip_conf` VALUES (5,5,'ability',-2,4,0,'属性',0,0,0);
INSERT INTO `st_actor_equip_conf` VALUES (6,6,'value',-3,4,0,'属性值',0,0,0);

#
# Source for table st_actor_hero_conf
#

CREATE TABLE `st_actor_hero_conf` (
  `dataid` int(11) NOT NULL AUTO_INCREMENT,
  `offset` int(11) NOT NULL DEFAULT '0',
  `name` varbinary(32) NOT NULL DEFAULT '',
  `data_type` smallint(6) NOT NULL DEFAULT '0',
  `data_count` smallint(6) NOT NULL DEFAULT '0',
  `data_size` smallint(6) NOT NULL DEFAULT '0',
  `context` varbinary(128) NOT NULL DEFAULT '',
  `notsave` tinyint(4) NOT NULL DEFAULT '0',
  `kind` tinyint(4) NOT NULL DEFAULT '0',
  `m_fix` tinyint(4) NOT NULL DEFAULT '0',
  PRIMARY KEY (`dataid`)
) ENGINE=MyISAM AUTO_INCREMENT=22 DEFAULT CHARSET=gbk ROW_FORMAT=DYNAMIC;

#
# Dumping data for table st_actor_hero_conf
#

INSERT INTO `st_actor_hero_conf` VALUES (1,1,'id',-4,0,0,'主索引',0,1,0);
INSERT INTO `st_actor_hero_conf` VALUES (2,2,'actorid',-3,0,0,'城池',0,2,0);
INSERT INTO `st_actor_hero_conf` VALUES (3,3,'offset',-3,0,0,'索引',0,3,0);
INSERT INTO `st_actor_hero_conf` VALUES (4,4,'kind',-2,0,0,'英雄种类',0,0,0);
INSERT INTO `st_actor_hero_conf` VALUES (5,5,'level',-2,0,0,'等级',0,0,0);
INSERT INTO `st_actor_hero_conf` VALUES (6,6,'exp',-3,0,0,'经验',0,0,0);
INSERT INTO `st_actor_hero_conf` VALUES (7,7,'soldiers',-3,0,0,'兵力',0,0,0);
INSERT INTO `st_actor_hero_conf` VALUES (8,8,'attack',-2,0,0,'攻击资质',0,0,0);
INSERT INTO `st_actor_hero_conf` VALUES (9,9,'defense',-2,0,0,'防御资质',0,0,0);
INSERT INTO `st_actor_hero_conf` VALUES (10,10,'troops',-2,0,0,'兵力资质',0,0,0);
INSERT INTO `st_actor_hero_conf` VALUES (11,11,'growth',-2,0,0,'总资质',0,0,0);
INSERT INTO `st_actor_hero_conf` VALUES (12,12,'equip',-109,6,0,'装备',1,0,0);

#
# Source for table st_actor_item_conf
#

CREATE TABLE `st_actor_item_conf` (
  `dataid` int(11) NOT NULL AUTO_INCREMENT,
  `offset` int(11) NOT NULL DEFAULT '0',
  `name` varbinary(32) NOT NULL DEFAULT '',
  `data_type` smallint(6) NOT NULL DEFAULT '0',
  `data_count` smallint(6) NOT NULL DEFAULT '0',
  `data_size` smallint(6) NOT NULL DEFAULT '0',
  `context` varbinary(128) NOT NULL DEFAULT '',
  `notsave` tinyint(4) NOT NULL DEFAULT '0',
  `kind` tinyint(4) NOT NULL DEFAULT '0',
  `m_fix` tinyint(4) NOT NULL DEFAULT '0',
  PRIMARY KEY (`dataid`)
) ENGINE=MyISAM AUTO_INCREMENT=9 DEFAULT CHARSET=gbk ROW_FORMAT=DYNAMIC;

#
# Dumping data for table st_actor_item_conf
#

INSERT INTO `st_actor_item_conf` VALUES (1,1,'id',-4,0,0,'物品id',0,1,0);
INSERT INTO `st_actor_item_conf` VALUES (2,2,'actorid',-3,0,0,'角色ID',0,2,0);
INSERT INTO `st_actor_item_conf` VALUES (3,3,'offset',-2,0,0,'物品所在位置',0,3,0);
INSERT INTO `st_actor_item_conf` VALUES (4,4,'kind',-3,0,0,'物品种类',0,0,1);
INSERT INTO `st_actor_item_conf` VALUES (5,5,'num',-2,0,0,'物品个数',0,0,1);
INSERT INTO `st_actor_item_conf` VALUES (6,6,'ability',-2,4,0,'属性',0,0,1);
INSERT INTO `st_actor_item_conf` VALUES (7,7,'value',-3,4,0,'属性值',0,0,1);
INSERT INTO `st_actor_item_conf` VALUES (8,8,'color_level',-1,0,0,'颜色等级',0,0,1);

#
# Source for table st_army_conf
#

CREATE TABLE `st_army_conf` (
  `dataid` int(11) NOT NULL AUTO_INCREMENT,
  `offset` int(11) NOT NULL DEFAULT '0',
  `name` varbinary(32) NOT NULL DEFAULT '',
  `data_type` smallint(6) NOT NULL DEFAULT '0',
  `data_count` smallint(6) NOT NULL DEFAULT '0',
  `data_size` smallint(6) NOT NULL DEFAULT '0',
  `context` varbinary(128) NOT NULL DEFAULT '',
  `notsave` tinyint(4) NOT NULL DEFAULT '0',
  `kind` tinyint(4) NOT NULL DEFAULT '0',
  `m_fix` tinyint(4) NOT NULL DEFAULT '0',
  PRIMARY KEY (`dataid`)
) ENGINE=MyISAM AUTO_INCREMENT=219 DEFAULT CHARSET=gbk ROW_FORMAT=DYNAMIC;

#
# Dumping data for table st_army_conf
#

INSERT INTO `st_army_conf` VALUES (1,1,'index',-3,0,0,'部队索引',0,1,0);
INSERT INTO `st_army_conf` VALUES (2,2,'id',-3,0,0,'部队唯一ID,合法性验证',0,0,0);
INSERT INTO `st_army_conf` VALUES (3,3,'unit_index',-3,0,0,'显示索引',1,0,0);
INSERT INTO `st_army_conf` VALUES (4,4,'state',-1,0,0,'状态',0,0,0);
INSERT INTO `st_army_conf` VALUES (5,5,'statetime',-3,0,0,'状态时间戳',0,0,0);
INSERT INTO `st_army_conf` VALUES (6,6,'stateduration',-3,0,0,'状态持续多少秒',0,0,0);
INSERT INTO `st_army_conf` VALUES (7,7,'action',-3,0,0,'行为',0,0,0);
INSERT INTO `st_army_conf` VALUES (8,8,'from_type',-2,0,0,'出发的对象类型',0,0,0);
INSERT INTO `st_army_conf` VALUES (9,9,'from_id',-3,0,0,'出发的对象ID',0,0,0);
INSERT INTO `st_army_conf` VALUES (10,10,'from_index',-3,0,0,'出发的对象索引',0,0,0);
INSERT INTO `st_army_conf` VALUES (11,11,'from_posx',-2,0,0,'出发的对象坐标',0,0,0);
INSERT INTO `st_army_conf` VALUES (12,12,'from_posy',-2,0,0,'出发的对象坐标',0,0,0);
INSERT INTO `st_army_conf` VALUES (40,13,'to_type',-2,0,0,'目的对象类型',0,0,0);
INSERT INTO `st_army_conf` VALUES (41,14,'to_id',-3,0,0,'目的对象ID',0,0,0);
INSERT INTO `st_army_conf` VALUES (42,15,'to_index',-3,0,0,'目的对象索引',0,0,0);
INSERT INTO `st_army_conf` VALUES (50,16,'to_posx',-2,0,0,'目的对象坐标',0,0,0);
INSERT INTO `st_army_conf` VALUES (60,17,'to_posy',-2,0,0,'目的对象坐标',0,0,0);
INSERT INTO `st_army_conf` VALUES (70,18,'posx',-2,0,0,'当前坐标',0,0,0);
INSERT INTO `st_army_conf` VALUES (80,19,'posy',-2,0,0,'当前坐标',0,0,0);
INSERT INTO `st_army_conf` VALUES (90,90,'herokind',-2,4,0,'武将',0,0,0);

#
# Source for table st_city_building_barracks_conf
#

CREATE TABLE `st_city_building_barracks_conf` (
  `dataid` int(11) NOT NULL AUTO_INCREMENT,
  `offset` int(11) NOT NULL DEFAULT '0',
  `name` varbinary(32) NOT NULL DEFAULT '',
  `data_type` smallint(6) NOT NULL DEFAULT '0',
  `data_count` smallint(6) NOT NULL DEFAULT '0',
  `data_size` smallint(6) NOT NULL DEFAULT '0',
  `context` varbinary(128) NOT NULL DEFAULT '',
  `notsave` tinyint(4) NOT NULL DEFAULT '0',
  `kind` tinyint(4) NOT NULL DEFAULT '0',
  `m_fix` tinyint(4) NOT NULL DEFAULT '0',
  PRIMARY KEY (`dataid`)
) ENGINE=MyISAM AUTO_INCREMENT=81 DEFAULT CHARSET=gbk ROW_FORMAT=DYNAMIC;

#
# Dumping data for table st_city_building_barracks_conf
#

INSERT INTO `st_city_building_barracks_conf` VALUES (1,1,'actorid',-3,0,0,'',-1,1,0);
INSERT INTO `st_city_building_barracks_conf` VALUES (2,2,'offset',-1,0,0,'',-1,2,0);
INSERT INTO `st_city_building_barracks_conf` VALUES (3,3,'kind',-1,0,0,'兵营种类',0,0,0);
INSERT INTO `st_city_building_barracks_conf` VALUES (4,4,'level',-1,0,0,'兵营等级',0,0,0);
INSERT INTO `st_city_building_barracks_conf` VALUES (5,5,'soldiers',-3,0,0,'本营兵力',0,0,0);
INSERT INTO `st_city_building_barracks_conf` VALUES (6,6,'trainnum',-3,0,0,'当前训练士兵数',0,0,0);
INSERT INTO `st_city_building_barracks_conf` VALUES (7,7,'trainsec',-3,0,0,'当前训练剩余时间(秒)',0,0,0);
INSERT INTO `st_city_building_barracks_conf` VALUES (9,9,'queuenum',-3,8,0,'招募队列',0,0,0);
INSERT INTO `st_city_building_barracks_conf` VALUES (10,10,'queuesec',-3,8,0,'招募队列',0,0,0);
INSERT INTO `st_city_building_barracks_conf` VALUES (11,11,'queue',-1,0,0,'扩容队列数量',0,0,0);
INSERT INTO `st_city_building_barracks_conf` VALUES (12,12,'trainlong',-1,0,0,'募兵加时级别',0,0,0);
INSERT INTO `st_city_building_barracks_conf` VALUES (13,13,'quicksec',-2,0,0,'募兵加速时间秒',0,0,0);

#
# Source for table st_city_building_conf
#

CREATE TABLE `st_city_building_conf` (
  `dataid` int(11) NOT NULL AUTO_INCREMENT,
  `offset` int(11) NOT NULL DEFAULT '0',
  `name` varbinary(32) NOT NULL DEFAULT '',
  `data_type` smallint(6) NOT NULL DEFAULT '0',
  `data_count` smallint(6) NOT NULL DEFAULT '0',
  `data_size` smallint(6) NOT NULL DEFAULT '0',
  `context` varbinary(128) NOT NULL DEFAULT '',
  `notsave` tinyint(4) NOT NULL DEFAULT '0',
  `kind` tinyint(4) NOT NULL DEFAULT '0',
  `m_fix` tinyint(4) NOT NULL DEFAULT '0',
  PRIMARY KEY (`dataid`)
) ENGINE=MyISAM AUTO_INCREMENT=13 DEFAULT CHARSET=gbk ROW_FORMAT=DYNAMIC;

#
# Dumping data for table st_city_building_conf
#

INSERT INTO `st_city_building_conf` VALUES (1,1,'actorid',-3,0,0,'',-1,1,0);
INSERT INTO `st_city_building_conf` VALUES (2,2,'offset',-1,0,0,'',-1,2,0);
INSERT INTO `st_city_building_conf` VALUES (3,3,'kind',-1,0,0,'建筑种类',0,0,0);
INSERT INTO `st_city_building_conf` VALUES (4,4,'level',-1,0,0,'建筑等级',0,0,0);
INSERT INTO `st_city_building_conf` VALUES (5,5,'officialkind',-1,0,0,'正在使用官员',0,0,0);
INSERT INTO `st_city_building_conf` VALUES (6,6,'officialfree',-3,0,0,'是否使用过免费(按位)',0,0,0);
INSERT INTO `st_city_building_conf` VALUES (7,7,'officialsec',-3,0,0,'官员剩余时间',0,0,0);
INSERT INTO `st_city_building_conf` VALUES (8,8,'value',-3,0,0,'通用值',0,0,0);
INSERT INTO `st_city_building_conf` VALUES (9,9,'sec',-3,0,0,'时间',0,0,0);
INSERT INTO `st_city_building_conf` VALUES (10,10,'quicksec',-2,0,0,'加速时间',0,0,0);

#
# Source for table st_city_building_res_conf
#

CREATE TABLE `st_city_building_res_conf` (
  `dataid` int(11) NOT NULL AUTO_INCREMENT,
  `offset` int(11) NOT NULL DEFAULT '0',
  `name` varbinary(32) NOT NULL DEFAULT '',
  `data_type` smallint(6) NOT NULL DEFAULT '0',
  `data_count` smallint(6) NOT NULL DEFAULT '0',
  `data_size` smallint(6) NOT NULL DEFAULT '0',
  `context` varbinary(128) NOT NULL DEFAULT '',
  `notsave` tinyint(4) NOT NULL DEFAULT '0',
  `kind` tinyint(4) NOT NULL DEFAULT '0',
  `m_fix` tinyint(4) NOT NULL DEFAULT '0',
  PRIMARY KEY (`dataid`)
) ENGINE=MyISAM AUTO_INCREMENT=9 DEFAULT CHARSET=gbk ROW_FORMAT=DYNAMIC;

#
# Dumping data for table st_city_building_res_conf
#

INSERT INTO `st_city_building_res_conf` VALUES (1,1,'actorid',-3,0,0,'',-1,1,0);
INSERT INTO `st_city_building_res_conf` VALUES (2,2,'offset',-1,0,0,'',-1,2,0);
INSERT INTO `st_city_building_res_conf` VALUES (3,3,'kind',-1,0,0,'资源建筑种类',0,0,0);
INSERT INTO `st_city_building_res_conf` VALUES (4,4,'level',-1,0,0,'资源建筑等级',0,0,0);

#
# Source for table st_city_conf
#

CREATE TABLE `st_city_conf` (
  `dataid` int(11) NOT NULL AUTO_INCREMENT,
  `offset` int(11) NOT NULL DEFAULT '0',
  `name` varbinary(32) NOT NULL DEFAULT '',
  `data_type` smallint(6) NOT NULL DEFAULT '0',
  `data_count` smallint(6) NOT NULL DEFAULT '0',
  `data_size` smallint(6) NOT NULL DEFAULT '0',
  `context` varbinary(128) NOT NULL DEFAULT '',
  `notsave` tinyint(4) NOT NULL DEFAULT '0',
  `kind` tinyint(4) NOT NULL DEFAULT '0',
  `m_fix` tinyint(4) NOT NULL DEFAULT '0',
  PRIMARY KEY (`dataid`)
) ENGINE=MyISAM AUTO_INCREMENT=204 DEFAULT CHARSET=gbk ROW_FORMAT=DYNAMIC;

#
# Dumping data for table st_city_conf
#

INSERT INTO `st_city_conf` VALUES (1,1,'actorid',-3,0,0,'角色ID',0,1,0);
INSERT INTO `st_city_conf` VALUES (2,2,'index',-3,0,0,'索引',0,0,0);
INSERT INTO `st_city_conf` VALUES (4,4,'name',-1,22,0,'名称',0,0,0);
INSERT INTO `st_city_conf` VALUES (5,5,'type',-1,0,0,'城池类型',0,0,0);
INSERT INTO `st_city_conf` VALUES (6,6,'shape',-1,0,0,'领主形象(选取)',0,0,0);
INSERT INTO `st_city_conf` VALUES (7,7,'headid',-3,0,0,'领主头像(自定义)',0,0,0);
INSERT INTO `st_city_conf` VALUES (8,8,'country',-2,0,0,'选择的国家(真实)',0,0,0);
INSERT INTO `st_city_conf` VALUES (9,9,'ipcountry',-1,3,0,'IP地址所在国家',0,0,0);
INSERT INTO `st_city_conf` VALUES (10,10,'language',-1,0,0,'选择语言',0,0,0);
INSERT INTO `st_city_conf` VALUES (11,11,'os',-1,0,0,'系统1ios 2android 3win',0,0,0);
INSERT INTO `st_city_conf` VALUES (12,12,'platid',-1,0,0,'平台',0,0,0);
INSERT INTO `st_city_conf` VALUES (13,13,'createtime',-3,0,0,'创建时间',0,0,0);
INSERT INTO `st_city_conf` VALUES (14,14,'lastlogin',-3,0,0,'上次登陆时间',0,0,0);
INSERT INTO `st_city_conf` VALUES (15,15,'lastlogout',-3,0,0,'上次登出时间',0,0,0);
INSERT INTO `st_city_conf` VALUES (16,16,'state',-1,0,0,'城池当前状态',0,0,0);
INSERT INTO `st_city_conf` VALUES (17,17,'posx',-2,0,0,'位置',0,0,0);
INSERT INTO `st_city_conf` VALUES (18,18,'posy',-2,0,0,'位置',0,0,0);
INSERT INTO `st_city_conf` VALUES (19,19,'sflag',-3,0,0,'标志位',0,0,0);
INSERT INTO `st_city_conf` VALUES (20,20,'level',-2,0,0,'玩家等级',0,0,0);
INSERT INTO `st_city_conf` VALUES (21,21,'exp',-3,0,0,'玩家经验',0,0,0);
INSERT INTO `st_city_conf` VALUES (22,22,'viplevel',-1,0,0,'VIP等级',1,0,0);
INSERT INTO `st_city_conf` VALUES (23,23,'vipexp',-3,0,0,'VIP经验',0,0,0);
INSERT INTO `st_city_conf` VALUES (24,24,'nation',-1,0,0,'国家(魏蜀吴)',0,0,0);
INSERT INTO `st_city_conf` VALUES (25,25,'official',-1,0,0,'官职',0,0,0);
INSERT INTO `st_city_conf` VALUES (26,26,'place',-1,0,0,'爵位',0,0,0);
INSERT INTO `st_city_conf` VALUES (27,27,'zone',-1,0,0,'区域',0,0,0);
INSERT INTO `st_city_conf` VALUES (28,28,'battlepower',-3,0,0,'战力',0,0,0);
INSERT INTO `st_city_conf` VALUES (29,29,'mokilllv',-1,0,0,'击杀野怪最高级别',0,0,0);
INSERT INTO `st_city_conf` VALUES (30,30,'body',-2,0,0,'体力',0,0,0);
INSERT INTO `st_city_conf` VALUES (31,31,'bodysec',-2,0,0,'体力恢复时间(每秒-1)',0,0,0);
INSERT INTO `st_city_conf` VALUES (32,32,'silver',-3,0,0,'银币',0,0,0);
INSERT INTO `st_city_conf` VALUES (33,33,'wood',-3,0,0,'木材',0,0,0);
INSERT INTO `st_city_conf` VALUES (34,34,'food',-3,0,0,'粮草',0,0,0);
INSERT INTO `st_city_conf` VALUES (35,35,'iron',-3,0,0,'镔铁',0,0,0);
INSERT INTO `st_city_conf` VALUES (36,36,'levynum',-11,0,0,'当前征收次数',0,0,0);
INSERT INTO `st_city_conf` VALUES (37,37,'levysec',-2,0,0,'下次征收时间(每秒-1)',0,0,0);
INSERT INTO `st_city_conf` VALUES (38,38,'people',-3,0,0,'人口',0,0,0);
INSERT INTO `st_city_conf` VALUES (39,39,'prestige',-3,0,0,'威望值',0,0,0);
INSERT INTO `st_city_conf` VALUES (40,40,'friendship',-3,0,0,'友谊积分',0,0,0);
INSERT INTO `st_city_conf` VALUES (41,41,'function',-3,0,0,'功能是否开启',0,0,0);
INSERT INTO `st_city_conf` VALUES (98,98,'hero',-108,4,0,'上阵英雄',0,0,0);
INSERT INTO `st_city_conf` VALUES (100,100,'building',-105,8,0,'普通建筑',0,0,0);
INSERT INTO `st_city_conf` VALUES (101,101,'building_barracks',-106,4,0,'兵营建筑',0,0,0);
INSERT INTO `st_city_conf` VALUES (102,102,'building_res',-107,64,0,'资源建筑',0,0,0);
INSERT INTO `st_city_conf` VALUES (103,103,'worker_op',-1,0,0,'建造队列操作',0,0,0);
INSERT INTO `st_city_conf` VALUES (104,104,'worker_sec',-3,0,0,'建造剩余时间(每秒-1)',0,0,0);
INSERT INTO `st_city_conf` VALUES (105,105,'worker_kind',-1,0,0,'当前升级建筑类型',0,0,0);
INSERT INTO `st_city_conf` VALUES (106,106,'worker_offset',-1,0,0,'当前升级建筑索引',0,0,0);
INSERT INTO `st_city_conf` VALUES (107,107,'worker_op_ex',-1,0,0,'建造队列操作(商用)',0,0,0);
INSERT INTO `st_city_conf` VALUES (108,108,'worker_sec_ex',-3,0,0,'建造剩余时间(每秒-1)(商用)',0,0,0);
INSERT INTO `st_city_conf` VALUES (109,109,'worker_kind_ex',-1,0,0,'当前升级建筑类型(商用)',0,0,0);
INSERT INTO `st_city_conf` VALUES (110,110,'worker_offset_ex',-1,0,0,'当前升级建筑索引(商用)',0,0,0);
INSERT INTO `st_city_conf` VALUES (111,111,'worker_expire_ex',-3,0,0,'商用建造队列到期时间',0,0,0);
INSERT INTO `st_city_conf` VALUES (201,201,'actor_index',-3,0,0,'角色索引',1,0,0);
INSERT INTO `st_city_conf` VALUES (202,202,'unit_index',-3,0,0,'显示索引',1,0,0);
INSERT INTO `st_city_conf` VALUES (203,203,'army_index',-3,5,0,'部队',1,0,0);

#
# Source for table struct_data
#

CREATE TABLE `struct_data` (
  `id` int(11) NOT NULL DEFAULT '0',
  `table` varchar(255) NOT NULL DEFAULT '' COMMENT '表名',
  `table_name` varchar(255) NOT NULL DEFAULT '' COMMENT '数据库表名',
  `table_struct` varchar(255) NOT NULL DEFAULT '' COMMENT '输出的结构体名',
  `load_key_dataid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '读取的键，0全读，对应每个表第几个键',
  `load_get_cb` tinyint(3) NOT NULL DEFAULT '0' COMMENT '读取之前的函数回调',
  `load_end_cb` tinyint(3) NOT NULL DEFAULT '0' COMMENT '读取之后的函数回调',
  `load_cb_dataid1` tinyint(3) NOT NULL DEFAULT '0' COMMENT '回调key',
  `load_cb_dataid2` tinyint(3) NOT NULL DEFAULT '0' COMMENT '回调key',
  `save_key_dataid` tinyint(3) NOT NULL DEFAULT '0' COMMENT '存档主键，0全存',
  `save_clear` tinyint(3) NOT NULL DEFAULT '0' COMMENT '存档前是否先清数据库',
  `save_batch` int(11) NOT NULL DEFAULT '0' COMMENT '>0使用replace的批量语法',
  `type` tinyint(3) NOT NULL DEFAULT '0' COMMENT '特殊类型，代码特殊处理一下',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=gbk;

#
# Dumping data for table struct_data
#

INSERT INTO `struct_data` VALUES (1,'st_actor_conf','actor','Actor',1,0,0,0,0,0,0,0,0);
INSERT INTO `struct_data` VALUES (2,'st_actor_item_conf','actor_item','Item',2,1,0,2,3,1,0,256,1);
INSERT INTO `struct_data` VALUES (3,'st_city_conf','city','City',0,1,1,2,0,0,0,0,0);
INSERT INTO `struct_data` VALUES (4,'st_army_conf','army','Army',0,1,1,1,0,2,1,256,0);
INSERT INTO `struct_data` VALUES (5,'st_city_building_conf','city_building','Building',1,1,0,1,2,3,0,64,2);
INSERT INTO `struct_data` VALUES (6,'st_city_building_barracks_conf','city_building_barracks','BuildingBarracks',1,1,0,1,2,3,0,64,2);
INSERT INTO `struct_data` VALUES (7,'st_city_building_res_conf','city_building_res','BuildingRes',1,1,0,1,2,3,0,64,2);
INSERT INTO `struct_data` VALUES (8,'st_actor_hero_conf','actor_hero','Hero',2,1,0,2,3,1,0,128,1);
INSERT INTO `struct_data` VALUES (9,'st_actor_equip_conf','actor_equip','Equip',2,1,0,2,3,1,0,256,1);

#
# Source for table struct_serv_data
#

CREATE TABLE `struct_serv_data` (
  `id` int(11) NOT NULL DEFAULT '0',
  `table` varchar(255) NOT NULL DEFAULT '' COMMENT '表名',
  `struct_name` varchar(255) NOT NULL DEFAULT '' COMMENT '结构体名',
  `variable_name` varchar(255) NOT NULL DEFAULT '' COMMENT '变量名',
  `skip_field1` varchar(32) NOT NULL DEFAULT '' COMMENT '跳过字段',
  `skip_field2` varchar(32) NOT NULL DEFAULT '',
  `skip_field3` varchar(32) NOT NULL DEFAULT '',
  `skip_field4` varchar(32) NOT NULL DEFAULT '',
  `fix` tinyint(3) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=gbk;

#
# Dumping data for table struct_serv_data
#

INSERT INTO `struct_serv_data` VALUES (1,'item','ItemKind','g_itemkind','used','','','',1);
INSERT INTO `struct_serv_data` VALUES (2,'platinfo','PlatInfo','g_platinfo','desc','','','',0);
INSERT INTO `struct_serv_data` VALUES (3,'upgrade','UpgradeInfo','g_upgradeinfo','','','','',0);
INSERT INTO `struct_serv_data` VALUES (4,'paycountry','PayCountry','g_paycountry','name','','','',0);
INSERT INTO `struct_serv_data` VALUES (5,'paygoods','PayGoods','g_paygoods','name','','','',0);
INSERT INTO `struct_serv_data` VALUES (6,'building_upgrade','BuildingUpgrade','g_building_upgrade','name','','','',0);
INSERT INTO `struct_serv_data` VALUES (7,'vip','VipInfo','g_vipinfo','','','','',0);

/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
