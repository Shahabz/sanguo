--client_netsend_auto.lua

-- m_username_length=0,m_username="[m_username_length]",m_password_length=0,m_password="[m_password_length]",m_deviceid_length=0,m_deviceid="[m_deviceid_length]",m_isnew=0,m_ipcountry_length=0,m_ipcountry="[m_ipcountry_length]",
function netsend_login_C( sendValue )
	local buf = ByteBuffer.New();
	buf:WriteShort( CMDC_LOGIN );
	struct_NetC_Login_send( buf, sendValue );
	eye.networkManager:SendMessage(buf);
end

-- m_nation=0,m_name_length=0,m_name="[m_name_length]",
function netsend_create_C( sendValue )
	local buf = ByteBuffer.New();
	buf:WriteShort( CMDC_CREATE );
	struct_NetC_Create_send( buf, sendValue );
	eye.networkManager:SendMessage(buf);
end

-- number
function netsend_list_C( sendValue )
	local buf = ByteBuffer.New();
	buf:WriteShort( CMDC_LIST );
	buf:WriteSByte( sendValue );
	eye.networkManager:SendMessage(buf);
end

-- number
function netsend_entergame_C( sendValue )
	local buf = ByteBuffer.New();
	buf:WriteShort( CMDC_ENTERGAME );
	buf:WriteInt( sendValue );
	eye.networkManager:SendMessage(buf);
end

-- number
function netsend_delete_C( sendValue )
	local buf = ByteBuffer.New();
	buf:WriteShort( CMDC_DELETE );
	buf:WriteInt( sendValue );
	eye.networkManager:SendMessage(buf);
end

-- m_value=0,
function netsend_heart_C( sendValue )
	local buf = ByteBuffer.New();
	buf:WriteShort( CMDC_HEART );
	struct_NetC_Heart_send( buf, sendValue );
	eye.networkManager:SendMessage(buf);
end

-- m_cmd=0,m_value={[4]},m_msg_length=0,m_msg="[m_msg_length]",
function netsend_gmcmd_C( sendValue )
	local buf = ByteBuffer.New();
	buf:WriteShort( CMDC_GMCMD );
	struct_NetC_Gmcmd_send( buf, sendValue );
	eye.networkManager:SendMessage(buf);
end

-- m_msgid=0,m_valuenum=0,m_value={}[m_valuenum],m_msg_length=0,m_msg="[m_msg_length]",
function netsend_askinfo_C( sendValue )
	local buf = ByteBuffer.New();
	buf:WriteShort( CMDC_ASKINFO );
	struct_NetC_AskInfo_send( buf, sendValue );
	eye.networkManager:SendMessage(buf);
end

-- m_value=0,
function netsend_entercity_C( sendValue )
	local buf = ByteBuffer.New();
	buf:WriteShort( CMDC_ENTERCITY );
	struct_NetC_EnterCity_send( buf, sendValue );
	eye.networkManager:SendMessage(buf);
end

-- m_msglen=0,m_msg="[m_msglen]",m_channel=0,
function netsend_chat_C( sendValue )
	local buf = ByteBuffer.New();
	buf:WriteShort( CMDC_CHAT );
	struct_NetC_Chat_send( buf, sendValue );
	eye.networkManager:SendMessage(buf);
end

-- m_storyid=0,m_herokind={[4]},
function netsend_storybattle_C( sendValue )
	local buf = ByteBuffer.New();
	buf:WriteShort( CMDC_STORYBATTLE );
	struct_NetC_StoryBattle_send( buf, sendValue );
	eye.networkManager:SendMessage(buf);
end

-- m_to_posx=0,m_to_posy=0,
function netsend_worldmapask_C( sendValue )
	local buf = ByteBuffer.New();
	buf:WriteShort( CMDC_WORLDMAPASK );
	struct_NetC_WorldMapAsk_send( buf, sendValue );
	eye.networkManager:SendMessage(buf);
end

-- m_areaindex=0,m_posx=0,m_posy=0,
function netsend_worldmapareaindex_C( sendValue )
	local buf = ByteBuffer.New();
	buf:WriteShort( CMDC_WORLDMAPAREAINDEX );
	struct_NetC_WorldMapAreaIndex_send( buf, sendValue );
	eye.networkManager:SendMessage(buf);
end

-- m_to_unit_type=0,m_to_unit_index=0,m_id=0,m_herokind={[4]},m_to_posx=0,m_to_posy=0,m_appdata=0,m_action=0,m_group_index=0,
function netsend_mapbattle_C( sendValue )
	local buf = ByteBuffer.New();
	buf:WriteShort( CMDC_MAPBATTLE );
	struct_NetC_MapBattle_send( buf, sendValue );
	eye.networkManager:SendMessage(buf);
end

-- m_minid=0,m_maxid=0,m_op=0,
function netsend_mailask_C( sendValue )
	local buf = ByteBuffer.New();
	buf:WriteShort( CMDC_MAILASK );
	struct_NetC_MailAsk_send( buf, sendValue );
	eye.networkManager:SendMessage(buf);
end

-- m_op=0,m_mailid=0,
function netsend_mailop_C( sendValue )
	local buf = ByteBuffer.New();
	buf:WriteShort( CMDC_MAILOP );
	struct_NetC_MailOp_send( buf, sendValue );
	eye.networkManager:SendMessage(buf);
end

-- m_count=0,m_mailid={}[m_count],
function netsend_mailalldel_C( sendValue )
	local buf = ByteBuffer.New();
	buf:WriteShort( CMDC_MAILALLDEL );
	struct_NetC_MailAllDel_send( buf, sendValue );
	eye.networkManager:SendMessage(buf);
end

-- m_mailid=0,m_a_name_len=0,m_a_name="[m_a_name_len]",m_d_name_len=0,m_d_name="[m_d_name_len]",m_type=0,
function netsend_mailshare_C( sendValue )
	local buf = ByteBuffer.New();
	buf:WriteShort( CMDC_MAILSHARE );
	struct_NetS_MailShare_send( buf, sendValue );
	eye.networkManager:SendMessage(buf);
end

-- m_unit_index=0,m_actorid=0,m_content_length=0,m_content="[m_content_length]",
function netsend_mailsend_C( sendValue )
	local buf = ByteBuffer.New();
	buf:WriteShort( CMDC_MAILSEND );
	struct_NetC_MailSend_send( buf, sendValue );
	eye.networkManager:SendMessage(buf);
end

-- m_actorid=0,m_content_length=0,m_content="[m_content_length]",m_reply_length=0,m_reply="[m_reply_length]",m_reply_recvtime=0,
function netsend_mailreply_C( sendValue )
	local buf = ByteBuffer.New();
	buf:WriteShort( CMDC_MAILREPLY );
	struct_NetC_MailReply_send( buf, sendValue );
	eye.networkManager:SendMessage(buf);
end

-- m_bossid=0,m_herokind={[4]},
function netsend_worldbossbattle_C( sendValue )
	local buf = ByteBuffer.New();
	buf:WriteShort( CMDC_WORLDBOSSBATTLE );
	struct_NetC_WorldBossBattle_send( buf, sendValue );
	eye.networkManager:SendMessage(buf);
end

-- m_herokind={[4]},
function netsend_heroguardsort_C( sendValue )
	local buf = ByteBuffer.New();
	buf:WriteShort( CMDC_HEROGUARDSORT );
	struct_NetC_HeroGuardSort_send( buf, sendValue );
	eye.networkManager:SendMessage(buf);
end

-- m_talkid=0,m_type=0,
function netsend_questtalknext_C( sendValue )
	local buf = ByteBuffer.New();
	buf:WriteShort( CMDC_QUESTTALKNEXT );
	struct_NetC_QuestTalkNext_send( buf, sendValue );
	eye.networkManager:SendMessage(buf);
end

-- m_type=0,m_page=0,m_myrange=0,
function netsend_ranklistask_C( sendValue )
	local buf = ByteBuffer.New();
	buf:WriteShort( CMDC_RANKLISTASK );
	struct_NetC_RankAsk_send( buf, sendValue );
	eye.networkManager:SendMessage(buf);
end

-- m_op=0,m_target_actorid=0,m_target_cityindex=0,m_target_namelen=0,m_target_name="[m_target_namelen]",
function netsend_friendop_C( sendValue )
	local buf = ByteBuffer.New();
	buf:WriteShort( CMDC_FRIENDOP );
	struct_NetC_FriendOp_send( buf, sendValue );
	eye.networkManager:SendMessage(buf);
end

