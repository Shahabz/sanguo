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

-- m_value={[2]},
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

