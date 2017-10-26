--client_structsend_auto.lua

function struct_NetC_Login_send( buf, sendValue )
	buf:WriteShort( sendValue.m_username_length );
	buf:WriteStringWithLength( sendValue.m_username );
	buf:WriteShort( sendValue.m_password_length );
	buf:WriteStringWithLength( sendValue.m_password );
	buf:WriteShort( sendValue.m_deviceid_length );
	buf:WriteStringWithLength( sendValue.m_deviceid );
	buf:WriteShort( sendValue.m_isnew );
	buf:WriteShort( sendValue.m_ipcountry_length );
	buf:WriteStringWithLength( sendValue.m_ipcountry );
end

function struct_NetC_Create_send( buf, sendValue )
	buf:WriteSByte( sendValue.m_nation );
	buf:WriteShort( sendValue.m_name_length );
	buf:WriteStringWithLength( sendValue.m_name );
end

function struct_NetC_Heart_send( buf, sendValue )
	for tmpi=1,2,1 do
		buf:WriteInt( sendValue.m_value[tmpi] );
	end
end

function struct_NetC_Gmcmd_send( buf, sendValue )
	buf:WriteShort( sendValue.m_cmd );
	for tmpi=1,4,1 do
		buf:WriteInt( sendValue.m_value[tmpi] );
	end
	buf:WriteShort( sendValue.m_msg_length );
	buf:WriteStringWithLength( sendValue.m_msg );
end

function struct_NetC_AskInfo_send( buf, sendValue )
	buf:WriteShort( sendValue.m_msgid );
	buf:WriteShort( sendValue.m_valuenum );
	for tmpi=1,sendValue.m_valuenum,1 do
		buf:WriteInt( sendValue.m_value[tmpi] );
	end
	buf:WriteShort( sendValue.m_msg_length );
	buf:WriteStringWithLength( sendValue.m_msg );
end

function struct_NetC_EnterCity_send( buf, sendValue )
	buf:WriteSByte( sendValue.m_value );
end

function struct_NetC_Chat_send( buf, sendValue )
	buf:WriteShort( sendValue.m_msglen );
	buf:WriteStringWithLength( sendValue.m_msg );
	buf:WriteSByte( sendValue.m_channel );
end

function struct_NetC_StoryBattle_send( buf, sendValue )
	buf:WriteInt( sendValue.m_storyid );
	for tmpi=1,4,1 do
		buf:WriteShort( sendValue.m_herokind[tmpi] );
	end
end

function struct_NetC_WorldMapAsk_send( buf, sendValue )
	buf:WriteShort( sendValue.m_to_posx );
	buf:WriteShort( sendValue.m_to_posy );
end

function struct_NetC_WorldMapAreaIndex_send( buf, sendValue )
	buf:WriteInt( sendValue.m_areaindex );
	buf:WriteShort( sendValue.m_posx );
	buf:WriteShort( sendValue.m_posy );
end

function struct_NetC_MapBattle_send( buf, sendValue )
	buf:WriteSByte( sendValue.m_to_unit_type );
	buf:WriteInt( sendValue.m_to_unit_index );
	buf:WriteInt( sendValue.m_id );
	for tmpi=1,4,1 do
		buf:WriteShort( sendValue.m_herokind[tmpi] );
	end
	buf:WriteShort( sendValue.m_to_posx );
	buf:WriteShort( sendValue.m_to_posy );
	buf:WriteInt( sendValue.m_appdata );
	buf:WriteSByte( sendValue.m_action );
	buf:WriteInt( sendValue.m_group_index );
end

function struct_NetC_MailAsk_send( buf, sendValue )
	buf:WriteLong( sendValue.m_minid );
	buf:WriteLong( sendValue.m_maxid );
	buf:WriteShort( sendValue.m_op );
end

function struct_NetC_MailOp_send( buf, sendValue )
	buf:WriteSByte( sendValue.m_op );
	buf:WriteLong( sendValue.m_mailid );
end

function struct_NetC_MailAllDel_send( buf, sendValue )
	buf:WriteShort( sendValue.m_count );
	for tmpi=1,sendValue.m_count,1 do
		buf:WriteLong( sendValue.m_mailid[tmpi] );
	end
end

function struct_NetS_MailShare_send( buf, sendValue )
	buf:WriteLong( sendValue.m_mailid );
	buf:WriteShort( sendValue.m_a_name_len );
	buf:WriteStringWithLength( sendValue.m_a_name );
	buf:WriteShort( sendValue.m_d_name_len );
	buf:WriteStringWithLength( sendValue.m_d_name );
	buf:WriteSByte( sendValue.m_type );
end

function struct_NetC_MailSend_send( buf, sendValue )
	buf:WriteInt( sendValue.m_unit_index );
	buf:WriteInt( sendValue.m_actorid );
	buf:WriteShort( sendValue.m_content_length );
	buf:WriteStringWithLength( sendValue.m_content );
end

function struct_NetC_MailReply_send( buf, sendValue )
	buf:WriteInt( sendValue.m_actorid );
	buf:WriteShort( sendValue.m_content_length );
	buf:WriteStringWithLength( sendValue.m_content );
	buf:WriteShort( sendValue.m_reply_length );
	buf:WriteStringWithLength( sendValue.m_reply );
	buf:WriteInt( sendValue.m_reply_recvtime );
end

function struct_NetC_WorldBossBattle_send( buf, sendValue )
	buf:WriteInt( sendValue.m_bossid );
	for tmpi=1,4,1 do
		buf:WriteShort( sendValue.m_herokind[tmpi] );
	end
end

