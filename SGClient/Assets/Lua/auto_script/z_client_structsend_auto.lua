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
	buf:WriteSByte( sendValue.m_aclass );
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

