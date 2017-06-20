-- m_result=0,m_username_length=0,m_username="[m_username_length]",m_token_length=0,m_token="[m_token_length]",m_usertype=0,
function proc_login_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_login_C", function( recvValue ) end )
	--EventProtocol.dispatchEvent( "proc_login_C", recvValue );
	gamelog( "[proc_login_C]result:"..recvValue.m_result );
	Global.AddValue("ISLOGOUT" , 0 );
	if recvValue.m_result < 0 then
		LoginModLoginQueue( false );
		-- 登陆失败
		print( "Login Result:"..recvValue.m_result );
		eye.networkManager:Logout();
--[[		if Const.platid > 10 then
			LoginModOpenSDKLoginActive( true );
		else
			LoginModOpenTestLogin();
		end--]]	
		LoginModOpenTestLogin();
		if recvValue.m_result == -10000 then
			-- 队列满了的情况
			LoginModOpenTestLogin();
			LoginModWarning( T(500) );
		else
			if recvValue.m_result == -1 or recvValue.m_result == -2 then
				LoginModWarning( T(505) );
			elseif recvValue.m_result == -4 then
				LoginModWarning( T(506) );
			end
		end
		
	else
		
		-- 登陆成功
		GameManager.writeini( "USERNAME", recvValue.m_username );
		GameManager.writeini( "PASSTOKEN", recvValue.m_token );
		GetPlayer().m_usertype = recvValue.m_usertype;
		
		-- GM号启动FPS
--[[		if GetPlayer().m_usertype >= 100 then
			FPSObject():SetActive( true );
		end--]]
	
		-- 启动loading
		LoginModOpenLoading();
	end
end

-- m_actor_num=0,m_listinfo={m_actorid=0,m_name="[22]",m_aclass=0,m_level=0,m_offset=0,m_lockstat=0,m_delete_stoptime=0,m_lock_endtime=0,[m_actor_num]},
function proc_list_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_list_C", function( recvValue ) end )
	-- EventProtocol.dispatchEvent( "proc_list_C", recvValue );
	-- 无角色
	if recvValue.m_actor_num <= 0 then
		
		CreateDlgOpen();
--[[		-- 创建角色
		local sendValue = {};
		sendValue.m_aclass = 0;
		sendValue.m_name = "xx"
		sendValue.m_name_length = string.len( sendValue.m_name );
		netsend_create_C( sendValue );--]]
		
	else
		if recvValue.m_listinfo[1].m_lockstat == 1 then
			Data.userini:WriteValue( "USERNAME", "" );
			Data.userini:WriteValue( "PASSTOKEN", "" );
			local lock_endtime = recvValue.m_listinfo[1].m_lock_endtime - os.time();
			if lock_endtime > 0 then
				lock_endtime = getTimeStringBySecond( lock_endtime );
			else
				lock_endtime = "";
			end
			--Alert( T(516).."\n"..lock_endtime, T(509), function() Application.Quit() end );
		else
			-- 进入游戏
			netsend_entergame_C( recvValue.m_listinfo[1].m_actorid );
			print( "Enter Game:"..recvValue.m_listinfo[1].m_actorid );
			gamelog( "Enter Game:"..recvValue.m_listinfo[1].m_actorid )
		end
	end
end

-- m_result=0,m_actorid=0,m_createtime=0,
function proc_create_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_create_C", function( recvValue ) end )
	-- EventProtocol.dispatchEvent( "proc_create_C", recvValue );
	if recvValue.m_result == 0 then
		-- 创建成功，进入游戏
		netsend_entergame_C( recvValue.m_actorid );
		print( "Create Actor Success:"..recvValue.m_actorid );
		gamelog( "Create Actor Success:"..recvValue.m_actorid );
	else
		print( "Create Actor Fail（Same Name）" )
		gamelog( "Create Actor Fail（Same Name）" )
	end
end

-- m_actorid=0,m_data_int=0,m_data_short=0,m_data_char=0,m_serverid=0,m_servertime=0,m_createtime=0,
function proc_enterinfo_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_enterinfo_C", function( recvValue ) end )
	--EventProtocol.dispatchEvent( "proc_enterinfo_C", recvValue );
	Const.NetStatus = 3;
	Const.serverid = recvValue.m_serverid;
	Const.actorid = recvValue.m_actorid;
	GetPlayer().m_createtime = recvValue.m_createtime;
end

-- m_result=0,m_actorid=0,
function proc_delete_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_delete_C", function( recvValue ) end )
	EventProtocol.dispatchEvent( "proc_delete_C", recvValue );
end

-- m_value={[2]},
function proc_heart_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_heart_C", function( recvValue ) end )
	EventProtocol.dispatchEvent( "proc_heart_C", recvValue );
end

-- m_msgid=0,m_valuenum=0,m_value={}[m_valuenum],m_msg_length=0,m_msg="[m_msg_length]",
function proc_notify_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_notify_C", function( recvValue ) end )
	-- EventProtocol.dispatchEvent( "proc_notify_C", recvValue );
	RecvActorNotify( recvValue );
end

-- m_actorid=0,m_name="[22]",m_aclass=0,m_shape=0,m_level=0,m_experience=0,m_experience_max=0,m_token=0,m_viplevel=0,
function proc_actorinfo_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_actorinfo_C", function( recvValue ) end )
	--EventProtocol.dispatchEvent( "proc_actorinfo_C", recvValue );
end

-- m_itemoffset=0,m_usenum=0,m_effres=0,
function proc_itemuse_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_itemuse_C", function( recvValue ) end )
	EventProtocol.dispatchEvent( "proc_itemuse_C", recvValue );
end

-- m_res_offset=0,m_res_num=0,m_target_offset=0,m_target_num=0,
function proc_itemput_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_itemput_C", function( recvValue ) end )
	EventProtocol.dispatchEvent( "proc_itemput_C", recvValue );
end

-- m_itemnum=0,m_itemoffset={}[m_itemnum],
function proc_itemsettle_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_itemsettle_C", function( recvValue ) end )
	EventProtocol.dispatchEvent( "proc_itemsettle_C", recvValue );
end

-- m_itemoffset=0,m_itemnum=0,m_targetid=0,m_path=0,
function proc_lostitem_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_lostitem_C", function( recvValue ) end )
	EventProtocol.dispatchEvent( "proc_lostitem_C", recvValue );
end

-- m_itemoffset=0,m_kind=0,m_type=0,m_color=0,m_num=0,m_targetid=0,m_path=0,m_situation=0,
function proc_getitem_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_getitem_C", function( recvValue ) end )
	EventProtocol.dispatchEvent( "proc_getitem_C", recvValue );
end

-- m_itemext=0,m_kindnum=0,m_item={m_offset=0,m_kind=0,m_num=0,m_situation=0,m_color_level=0,[m_kindnum]},
function proc_itemlist_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_itemlist_C", function( recvValue ) end )
	EventProtocol.dispatchEvent( "proc_itemlist_C", recvValue );
end

-- m_itemoffset=0,m_itemkind=0,m_type=0,m_level=0,m_color_level=0,m_price=0,m_attr_num=0,m_attr={m_type=0,m_ability=0,m_value=0,m_addvalue=0,[m_attr_num]},
function proc_iteminfo_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_iteminfo_C", function( recvValue ) end )
	EventProtocol.dispatchEvent( "proc_iteminfo_C", recvValue );
end

-- m_count=0,m_list={m_kind=0,m_num=0,[m_count]},m_callback_code=0,
function proc_awardinfolist_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_awardinfolist_C", function( recvValue ) end )
	EventProtocol.dispatchEvent( "proc_awardinfolist_C", recvValue );
end

-- m_addexp=0,m_curexp=0,m_isup=0,m_path=0,
function proc_experience_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_experience_C", function( recvValue ) end )
	EventProtocol.dispatchEvent( "proc_experience_C", recvValue );
end

-- m_body=0,m_addbody=0,m_path=0,
function proc_body_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_body_C", function( recvValue ) end )
	EventProtocol.dispatchEvent( "proc_body_C", recvValue );
end

-- m_attack_actornum=0,m_attack_actorinfo={m_name_length=0,m_name="[m_name_length]",m_level=0,m_actorid=0,m_headid=0,m_ready=0,[m_attack_actornum]},m_defense_actornum=0,m_defense_actorinfo={m_name_length=0,m_name="[m_name_length]",m_level=0,m_actorid=0,m_headid=0,m_ready=0,[m_defense_actornum]},m_fightid=0,m_pvpnum=0,
function proc_fightroominfo_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_fightroominfo_C", function( recvValue ) end )
	--EventProtocol.dispatchEvent( "proc_fightroominfo_C", recvValue );
	FightRoomInfoDlgRefurbish( recvValue );
end

-- m_fightid=0,m_actorid=0,
function proc_fightroomquit_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_fightroomquit_C", function( recvValue ) end )
	--EventProtocol.dispatchEvent( "proc_fightroomquit_C", recvValue );
end

-- m_fightid=0,m_actorid=0,
function proc_fightroomready_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_fightroomready_C", function( recvValue ) end )
	--EventProtocol.dispatchEvent( "proc_fightroomready_C", recvValue );
end

-- m_fightid=0,
function proc_fightroomsethero_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_fightroomsethero_C", function( recvValue ) end )
	--EventProtocol.dispatchEvent( "proc_fightroomsethero_C", recvValue );
	FightRoomInfoDlgSetHero( recvValue );
end

-- m_fightid=0,
function proc_fightroomstart_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_fightroomstart_C", function( recvValue ) end )
	--EventProtocol.dispatchEvent( "proc_fightroomstart_C", recvValue );
	LoginModClose();
	FightDlgOpen();
	GetFightRoom():Create( recvValue );
end

-- m_turns=0,m_count=0,m_list={m_side=0,m_cmd=0,m_kind=0,m_attr={m_life=0,m_attack=0,m_defence=0,m_precision=0,m_dodge=0,m_crit=0,m_crit_resist=0,m_crit_damage=0,m_crit_damage_resist=0,m_speed_attack=0,m_speed_move=0,m_ignore_defence=0,m_damage_increase=0,m_damage_reduce=0,m_skillid={[4]},},[m_count]},
function proc_fightturns_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_fightturns_C", function( recvValue ) end )
	--EventProtocol.dispatchEvent( "proc_fightturns_C", recvValue );
	--[[for i=1, recvValue.m_count, 1 do
		GetFightRoom():AddUnit( {m_side = recvValue.m_list[i].m_side, m_kind = recvValue.m_list[i].m_kind, m_offsetx = -200} )
	end
	GetFightRoom():Logic( recvValue );--]]
	GetFightRoom():AddTurnCommand( recvValue );
end

-- m_fightid=0,
function proc_fighttimeout_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_fighttimeout_C", function( recvValue ) end )
	EventProtocol.dispatchEvent( "proc_fighttimeout_C", recvValue );
	FightDlgClose();
	if GameManager.MainCityScence ~= nil then
		GameManager.MainCityScence.gameObject:SetActive( true );
	end
	if GameManager.FightScence ~= nil then
		GameObject.Destroy( GameManager.FightScence );
	end
	MainDlgOpen();
end

-- m_type=0,m_state=0,m_posx=0,m_unit_index=0,m_posy=0,m_namelen=0,m_name="[m_namelen]",m_char_value_count=0,m_char_value={}[m_char_value_count],m_short_value_count=0,m_short_value={}[m_short_value_count],m_int_value_count=0,m_int_value={}[m_int_value_count],m_prefixlen=0,m_prefix="[m_prefixlen]",
function proc_addmapunit_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_addmapunit_C", function( recvValue ) end )
	EventProtocol.dispatchEvent( "proc_addmapunit_C", recvValue );
end

-- m_unit_index=0,
function proc_delmapunit_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_delmapunit_C", function( recvValue ) end )
	EventProtocol.dispatchEvent( "proc_delmapunit_C", recvValue );
end

-- m_info={m_type=0,m_state=0,m_posx=0,m_unit_index=0,m_posy=0,m_namelen=0,m_name="[m_namelen]",m_char_value_count=0,m_char_value={}[m_char_value_count],m_short_value_count=0,m_short_value={}[m_short_value_count],m_int_value_count=0,m_int_value={}[m_int_value_count],m_prefixlen=0,m_prefix="[m_prefixlen]",},
function proc_updatemapunit_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_updatemapunit_C", function( recvValue ) end )
	EventProtocol.dispatchEvent( "proc_updatemapunit_C", recvValue );
end

-- m_type=0,m_state=0,m_posx=0,m_unit_index=0,m_posy=0,m_namelen=0,m_name="[m_namelen]",m_char_value_count=0,m_char_value={}[m_char_value_count],m_short_value_count=0,m_short_value={}[m_short_value_count],m_int_value_count=0,m_int_value={}[m_int_value_count],m_prefixlen=0,m_prefix="[m_prefixlen]",
function proc_mapunitsingle_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_mapunitsingle_C", function( recvValue ) end )
	EventProtocol.dispatchEvent( "proc_mapunitsingle_C", recvValue );
end

-- m_map_width=0,m_map_height=0,m_area_width=0,m_area_height=0,m_map_area_xnum=0,m_map_area_ynum=0,m_my_city_posx=0,m_my_city_posy=0,m_my_city_unit_index=0,m_citystate=0,m_target_posx=0,m_target_posy=0,
function proc_worldmapinfo_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_worldmapinfo_C", function( recvValue ) end )
	EventProtocol.dispatchEvent( "proc_worldmapinfo_C", recvValue );
end

-- m_unit_index=0,m_posx=0,m_posy=0,
function proc_mapunitcorrdinate_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_mapunitcorrdinate_C", function( recvValue ) end )
	EventProtocol.dispatchEvent( "proc_mapunitcorrdinate_C", recvValue );
end

-- m_from_type=0,m_from_posx=0,m_from_posy=0,m_to_type=0,m_to_posx=0,m_to_posy=0,m_state=0,m_from_cityid=0,m_from_clubid=0,m_to_cityid=0,m_to_clubid=0,m_army_index=0,m_action=0,
function proc_addmarchroute_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_addmarchroute_C", function( recvValue ) end )
	EventProtocol.dispatchEvent( "proc_addmarchroute_C", recvValue );
end

-- m_army_index=0,
function proc_delmarchroute_C( recvValue )
	-- process.
	-- EventProtocol.addEventListener( "proc_delmarchroute_C", function( recvValue ) end )
	EventProtocol.dispatchEvent( "proc_delmarchroute_C", recvValue );
end

