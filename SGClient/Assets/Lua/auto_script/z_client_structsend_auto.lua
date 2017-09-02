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
end

