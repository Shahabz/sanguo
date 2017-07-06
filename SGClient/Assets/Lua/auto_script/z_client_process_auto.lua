-- m_result=0,m_username_length=0,m_username="[m_username_length]",m_token_length=0,m_token="[m_token_length]",m_usertype=0,
function proc_login_C( recvValue )
	-- process.
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
	-- 无角色
	if recvValue.m_actor_num <= 0 then
		CreateDlgOpen();
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
	gamelog( "proc_create_C" )
	-- process.
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
	Const.NetStatus = 3;
	Const.serverid = recvValue.m_serverid;
	Const.actorid = recvValue.m_actorid;
	GetPlayer().m_createtime = recvValue.m_createtime;
	gamelog( "proc_enterinfo_C" )
	
	-- 加载主城
	LoadPrefabAsyn( "MainCity", function( obj )
		
		-- 缓存主城
		GameManager.MainCity = GameObject.Instantiate( obj );
		City.Init();
		
		-- 打开主界面
		MainDlgOpen();
		
		-- 获取进入游戏所需信息
		local sendValue = {};
		sendValue.m_value = 0;
		netsend_entercity_C( sendValue );
	end );
end

-- m_actorid=0,m_name="[22]",m_nation=0,m_shape=0,m_level=0,m_exp=0,m_exp_max=0,m_token=0,m_viplevel=0,m_vipexp=0,m_vipexp_max=0,m_body=0,m_place=0,m_official=0,m_zone=0,m_battlepower=0,m_silver=0,m_wood=0,m_food=0,m_iron=0,m_infantry_num=0,m_cavalry_num=0,m_archer_num=0,m_mokilllv=0,m_sflag=0,m_cityid=0,
function proc_actorinfo_C( recvValue )
	-- process.
	GetPlayer():Set( recvValue );
	MainDlgSetHead()
	MainDlgSetLevel()
	MainDlgSetSilver()
	MainDlgSetWood()
	MainDlgSetFood()
	MainDlgSetIron()
	MainDlgSetBody()
	MainDlgSetInfantry()
	MainDlgSetCavalry()
	MainDlgSetArcher()
	MainDlgSetToken()
	MainDlgSetVipLevel()
end

-- m_count=0,m_building={m_kind=0,m_level=0,m_sec=0,m_quick=0,[m_count]},m_count=0,m_barracks={m_kind=0,m_level=0,m_sec=0,m_quick=0,[m_count]},m_count=0,m_res={m_kind=0,m_level=0,m_offset=0,[m_count]},m_levynum=0,m_worker_kind=0,m_worker_offset=0,m_worker_sec=0,m_worker_kind_ex=0,m_worker_offset_ex=0,m_worker_sec_ex=0,m_worker_expire_ex=0,
function proc_buildinglist_C( recvValue )
	-- process.
	-- 设置建筑
	for i=1, recvValue.m_building_count, 1 do
		proc_building_C( recvValue.m_building[i] );	
	end
	
	for i=1, recvValue.m_barracks_count, 1 do
		proc_buildingbarracks_C( recvValue.m_barracks[i] )
	end
	
	for i=1, recvValue.m_res_count, 1 do
		proc_buildingres_C( recvValue.res[i] )
	end
	
	
	GetPlayer():SetBuilding( 1,{ m_kind = 1,m_offset = 0,m_level = 1,m_sec = 100,m_needsec = 100,m_quick = 0, m_overvalue=0 } )
	GetPlayer():SetBuilding( 2,{ m_kind = 2,m_offset = 1,m_level = 1,m_sec = 60,m_needsec = 60,m_quick = 0, m_overvalue=0 } )
	GetPlayer():SetBuilding( 3,{ m_kind = 3,m_offset = 2,m_level = 1,m_sec = 30,m_needsec = 30,m_quick = 0, m_overvalue=0 } )
	GetPlayer():SetBuilding( 4,{ m_kind = 4,m_offset = 3,m_level = 1,m_sec = 0,m_needsec = 0,m_quick = 0, m_overvalue=0 } )
	GetPlayer():SetBuilding( 5,{ m_kind = 5,m_offset = 4,m_level = 1,m_sec = 0,m_needsec = 0,m_quick = 0, m_overvalue=0 } )
	GetPlayer():SetBuilding( 6,{ m_kind = 6,m_offset = 5,m_level = 1,m_sec = 100,m_needsec = 100,m_quick = 0, m_overvalue=0 } )

	GetPlayer():SetBuilding( 11,{ m_kind = 11,m_offset = 0,m_level = 1,m_sec = 0,m_needsec = 0,m_quick = 0, m_overvalue=0 } )
	GetPlayer():SetBuilding( 12,{ m_kind = 12,m_offset = 1,m_level = 1,m_sec = 0,m_needsec = 0,m_quick = 0, m_overvalue=0 } )
	GetPlayer():SetBuilding( 13,{ m_kind = 13,m_offset = 2,m_level = 1,m_sec = 0,m_needsec = 0,m_quick = 0, m_overvalue=0 } )
	
	GetPlayer():SetBuildingRes( 21,0,{ m_kind = 21,m_offset = 0,m_level = 1,m_sec = 0,m_needsec = 0,m_quick = 0 } )
	GetPlayer():SetBuildingRes( 21,1,{ m_kind = 21,m_offset = 1,m_level = 1,m_sec = 0,m_needsec = 0,m_quick = 0 } )
	GetPlayer():SetBuildingRes( 21,2,{ m_kind = 21,m_offset = 2,m_level = 1,m_sec = 0,m_needsec = 0,m_quick = 0 } )
	GetPlayer():SetBuildingRes( 21,3,{ m_kind = 21,m_offset = 3,m_level = 1,m_sec = 0,m_needsec = 0,m_quick = 0 } )
	GetPlayer():SetBuildingRes( 21,4,{ m_kind = 21,m_offset = 4,m_level = 1,m_sec = 0,m_needsec = 0,m_quick = 0 } )
	
	GetPlayer():SetBuildingRes( 22,32,{ m_kind = 22,m_offset = 32,m_level = 1,m_sec = 0,m_needsec = 0,m_quick = 0 } )
	GetPlayer():SetBuildingRes( 22,33,{ m_kind = 22,m_offset = 33,m_level = 1,m_sec = 0,m_needsec = 0,m_quick = 0 } )
	GetPlayer():SetBuildingRes( 22,34,{ m_kind = 22,m_offset = 34,m_level = 1,m_sec = 0,m_needsec = 0,m_quick = 0 } )
	GetPlayer():SetBuildingRes( 22,35,{ m_kind = 22,m_offset = 35,m_level = 1,m_sec = 0,m_needsec = 0,m_quick = 0 } )
	GetPlayer():SetBuildingRes( 22,36,{ m_kind = 22,m_offset = 36,m_level = 1,m_sec = 0,m_needsec = 0,m_quick = 0 } )
	recvValue.m_levynum = 3
	recvValue.m_worker_kind = 21;
	recvValue.m_worker_offset = 2;
	recvValue.m_worker_sec = 200;
	recvValue.m_worker_needsec = 200;
	recvValue.m_worker_free = 1;
	recvValue.m_worker_kind_ex = 1;
	recvValue.m_worker_offset_ex = 0;
	recvValue.m_worker_sec_ex = 3600;
	recvValue.m_worker_needsec_ex = 3600;
	recvValue.m_worker_expire_ex = 3000;
	
	GetPlayer():SetBuildingWorker( recvValue )
	GetPlayer():SetBuildingLevy( recvValue.m_levynum )
	-- 关闭加载面板
	GameObject.FindWithTag( "UpdateManager" ):SetActive( false );
	LoginModClose();
end

-- m_kind=0,m_offset=0,m_level=0,m_sec=0,m_needsec=0,m_quick=0,m_overvalue=0,
function proc_building_C( recvValue )
	-- process.
	GetPlayer():SetBuilding( recvValue.m_kind, { 
		m_kind = recvValue.m_kind,
		m_offset = recvValue.m_offset,
		m_level = recvValue.m_level,
		m_sec = recvValue.m_sec,
		m_needsec = recvValue.m_needsec,
		m_quick = recvValue.m_quick,
		m_overvalue = recvValue.m_overvalue,
		} )
end

-- m_kind=0,m_offset=0,m_level=0,m_sec=0,m_needsec=0,m_quick=0,m_overvalue=0,
function proc_buildingbarracks_C( recvValue )
	-- process.
	GetPlayer():SetBuilding( recvValue.m_kind, { 
		m_kind = recvValue.m_kind,
		m_offset = recvValue.m_offset,
		m_level = recvValue.m_level,
		m_sec = recvValue.m_sec,
		m_needsec = recvValue.m_needsec,
		m_quick = recvValue.m_quick,
		m_overvalue = recvValue.m_overvalue,
		} )
end

-- m_kind=0,m_offset=0,m_level=0,
function proc_buildingres_C( recvValue )
	-- process.
	GetPlayer():SetBuildingRes( recvValue.m_kind, recvValue.m_offset,{ 
		m_kind = recvValue.m_kind,
		m_offset = recvValue.m_offset,
		m_level = recvValue.m_level,
		} )
end

-- m_worker_kind=0,m_worker_offset=0,m_worker_op=0,m_worker_sec=0,m_worker_needsec=0,m_worker_free=0,m_worker_kind_ex=0,m_worker_offset_ex=0,m_worker_op_ex=0,m_worker_sec_ex=0,m_worker_needsec_ex=0,m_worker_free_ex=0,m_worker_expire_ex=0,
function proc_worker_C( recvValue )
	-- process.
	GetPlayer():SetBuildingWorker( recvValue )
end

-- m_result=0,m_actorid=0,
function proc_delete_C( recvValue )
	-- process.
end

-- m_value={[2]},
function proc_heart_C( recvValue )
	-- process.
end

-- m_msgid=0,m_valuenum=0,m_value={}[m_valuenum],m_msg_length=0,m_msg="[m_msg_length]",
function proc_notify_C( recvValue )
	-- process.
	RecvActorNotify( recvValue );
end

-- m_itemoffset=0,m_usenum=0,m_effres=0,
function proc_itemuse_C( recvValue )
	-- process.

end

-- m_res_offset=0,m_res_num=0,m_target_offset=0,m_target_num=0,
function proc_itemput_C( recvValue )
	-- process.

end

-- m_itemnum=0,m_itemoffset={}[m_itemnum],
function proc_itemsettle_C( recvValue )
	-- process.

end

-- m_itemoffset=0,m_itemnum=0,m_targetid=0,m_path=0,
function proc_lostitem_C( recvValue )
	-- process.

end

-- m_itemoffset=0,m_kind=0,m_type=0,m_color=0,m_num=0,m_targetid=0,m_path=0,m_situation=0,
function proc_getitem_C( recvValue )
	-- process.

end

-- m_itemext=0,m_kindnum=0,m_item={m_offset=0,m_kind=0,m_num=0,m_situation=0,m_color_level=0,[m_kindnum]},
function proc_itemlist_C( recvValue )
	-- process.

end

-- m_itemoffset=0,m_itemkind=0,m_type=0,m_level=0,m_color_level=0,m_price=0,m_attr_num=0,m_attr={m_type=0,m_ability=0,m_value=0,m_addvalue=0,[m_attr_num]},
function proc_iteminfo_C( recvValue )
	-- process.

end

-- m_count=0,m_list={m_kind=0,m_num=0,[m_count]},m_callback_code=0,
function proc_awardinfolist_C( recvValue )
	-- process.

end

-- m_type=0,m_state=0,m_posx=0,m_unit_index=0,m_posy=0,m_namelen=0,m_name="[m_namelen]",m_char_value_count=0,m_char_value={}[m_char_value_count],m_short_value_count=0,m_short_value={}[m_short_value_count],m_int_value_count=0,m_int_value={}[m_int_value_count],m_prefixlen=0,m_prefix="[m_prefixlen]",
function proc_addmapunit_C( recvValue )
	-- process.

end

-- m_unit_index=0,
function proc_delmapunit_C( recvValue )
	-- process.

end

-- m_info={m_type=0,m_state=0,m_posx=0,m_unit_index=0,m_posy=0,m_namelen=0,m_name="[m_namelen]",m_char_value_count=0,m_char_value={}[m_char_value_count],m_short_value_count=0,m_short_value={}[m_short_value_count],m_int_value_count=0,m_int_value={}[m_int_value_count],m_prefixlen=0,m_prefix="[m_prefixlen]",},
function proc_updatemapunit_C( recvValue )
	-- process.

end

-- m_type=0,m_state=0,m_posx=0,m_unit_index=0,m_posy=0,m_namelen=0,m_name="[m_namelen]",m_char_value_count=0,m_char_value={}[m_char_value_count],m_short_value_count=0,m_short_value={}[m_short_value_count],m_int_value_count=0,m_int_value={}[m_int_value_count],m_prefixlen=0,m_prefix="[m_prefixlen]",
function proc_mapunitsingle_C( recvValue )
	-- process.
end

-- m_map_width=0,m_map_height=0,m_area_width=0,m_area_height=0,m_map_area_xnum=0,m_map_area_ynum=0,m_my_city_posx=0,m_my_city_posy=0,m_my_city_unit_index=0,m_citystate=0,m_target_posx=0,m_target_posy=0,
function proc_worldmapinfo_C( recvValue )
	-- process.
end

-- m_unit_index=0,m_posx=0,m_posy=0,
function proc_mapunitcorrdinate_C( recvValue )
	-- process.
end

-- m_from_type=0,m_from_posx=0,m_from_posy=0,m_to_type=0,m_to_posx=0,m_to_posy=0,m_state=0,m_from_cityid=0,m_from_clubid=0,m_to_cityid=0,m_to_clubid=0,m_army_index=0,m_action=0,
function proc_addmarchroute_C( recvValue )
	-- process.

end


-- m_army_index=0,
function proc_delmarchroute_C( recvValue )
	-- process.
end

-- m_addexp=0,m_curexp=0,m_isup=0,m_level, m_expmax, m_path=0,
function proc_experience_C( recvValue )
	-- process.
	pop( T(120)..": "..T(128).."x"..recvValue.m_addexp );
	GetPlayer().m_level = recvValue.m_level;
	GetPlayer().m_exp = recvValue.m_curexp;
	GetPlayer().m_exp_max = recvValue.m_expmax;
end

-- m_total=0,m_add=0,m_path=0,
function proc_body_C( recvValue )
	-- process.
	if recvValue.m_path ~= PATH_SYSTEM then
		pop( T(120)..": "..T(126).."x"..recvValue.m_add );
	end
	
	GetPlayer().m_body = recvValue.m_total;
	MainDlgSetBody()
end

-- m_total=0,m_add=0,m_path=0,
function proc_changelevy_C( recvValue )
	-- process.
	GetPlayer():SetBuildingLevy( recvValue.m_total )
end

-- m_total=0,m_add=0,m_path=0,
function proc_changesilver_C( recvValue )
	-- process.
	pop( T(120)..": "..T(121).."x"..recvValue.m_add );
	GetPlayer().m_silver = recvValue.m_total;
	MainDlgSetSilver();
end

-- m_total=0,m_add=0,m_path=0,
function proc_changewood_C( recvValue )
	-- process.
	pop( T(120)..": "..T(122).."x"..recvValue.m_add );
	GetPlayer().m_wood = recvValue.m_total;
	MainDlgSetWood();
end

-- m_total=0,m_add=0,m_path=0,
function proc_changefood_C( recvValue )
	-- process.
	pop( T(120)..": "..T(123).."x"..recvValue.m_add );
	GetPlayer().m_food = recvValue.m_total;
	MainDlgSetFood();
end

-- m_total=0,m_add=0,m_path=0,
function proc_changeiron_C( recvValue )
	-- process.
	pop( T(120)..": "..T(124).."x"..recvValue.m_add );
	GetPlayer().m_iron = recvValue.m_total;
	MainDlgSetIron();
end

-- m_total=0,m_add=0,m_path=0,
function proc_changepeople_C( recvValue )
	-- process.
end

-- m_total=0,m_add=0,m_path=0,
function proc_changeprestige_C( recvValue )
	-- process.
end

-- m_total=0,m_add=0,m_path=0,
function proc_changefriendship_C( recvValue )
	-- process.
end

-- m_addexp=0,m_curexp=0,m_expmax=0,m_isup=0,m_level=0,m_path=0,
function proc_changevip_C( recvValue )
	-- process.
	GetPlayer().m_viplevel		=	recvValue.m_level;
	GetPlayer().m_vipexp		=	recvValue.m_curexp;
	GetPlayer().m_vipexp_max	=	recvValue.m_expmax;
	MainDlgSetVipLevel();
end

-- m_total=0,m_add=0,m_path=0,
function proc_changtoken_C( recvValue )
	-- process.
	pop( T(120)..": "..T(125).."x"..recvValue.m_add );
	GetPlayer().m_token = recvValue.m_total;
	MainDlgSetToken()
end

-- m_info={m_type=0,m_state=0,m_posx=0,m_unit_index=0,m_posy=0,m_namelen=0,m_name="[m_namelen]",m_char_value_count=0,m_char_value={}[m_char_value_count],m_short_value_count=0,m_short_value={}[m_short_value_count],m_int_value_count=0,m_int_value={}[m_int_value_count],m_prefixlen=0,m_prefix="[m_prefixlen]",},
function proc_equiplist_C( recvValue )
	-- process.
end

-- m_offset=0,m_kind=0,m_washid={[4]},
function proc_equip_C( recvValue )
	-- process.
end

-- m_offset=0,m_kind=0,m_path=0,
function proc_equipget_C( recvValue )
	-- process.
end

-- m_offset=0,m_kind=0,m_path=0,
function proc_equiplost_C( recvValue )
	-- process.
end

-- m_kind=0,m_color=0,m_level=0,m_corps=0,m_exp=0,m_exp_max=0,m_soldiers=0,m_state=0,m_attack_base=0,m_attack_wash=0,m_defense_base=0,m_defense_wash=0,m_troops_base=0,m_troops_wash=0,m_attack=0,m_defense=0,m_troops=0,m_offset=0,
function proc_hero_C( recvValue )
	-- process.
	local type, offset = GetHero():GetIndex( recvValue.m_kind );
	if type == "CityHero" then
		GetHero().m_CityHero[offset]:Set( recvValue );
	elseif type == "Hero" then
		GetHero().m_Hero[offset]:Set( recvValue );
	end
	HeroDlgUpdate();
end

-- m_count=0,m_list={m_kind=0,m_color=0,m_level=0,m_corps=0,m_exp=0,m_exp_max=0,m_soldiers=0,m_state=0,m_attack_base=0,m_attack_wash=0,m_defense_base=0,m_defense_wash=0,m_troops_base=0,m_troops_wash=0,m_attack=0,m_defense=0,m_troops=0,m_offset=0,[m_count]},m_type=0,
function proc_herolist_C( recvValue )
	-- process.
	if recvValue.m_type == 0 then
		for i=1, recvValue.m_count, 1 do
			local pHero = SLK_Hero.new();
			pHero:Set( recvValue.m_list[i] );
			GetHero():SetCityHero( recvValue.m_list[i].m_offset, pHero );
		end
	else
		for i=1, recvValue.m_count, 1 do
			local pHero = SLK_Hero.new();
			pHero:Set( recvValue.m_list[i] );
			GetHero():SetHero( recvValue.m_list[i].m_offset, pHero );
		end
	end
end

-- m_kind=0,m_exp=0,m_exp_max=0,m_add=0,m_level,m_isup=0,m_path=0,
function proc_heroexp_C( recvValue )
	-- process.
	local type, offset = GetHero():GetIndex( recvValue.m_kind );
	if type == "CityHero" then
		GetHero().m_CityHero[offset].m_exp = recvValue.m_exp;
		GetHero().m_CityHero[offset].m_exp_max = recvValue.m_exp_max;
		GetHero().m_CityHero[offset].m_level = recvValue.m_level;
	elseif type == "Hero" then
		GetHero().m_Hero[offset].m_exp = recvValue.m_exp;
		GetHero().m_Hero[offset].m_exp_max = recvValue.m_exp_max;
		GetHero().m_Hero[offset].m_level = recvValue.m_level;
	end
	
end

-- m_kind=0,m_add=0,m_soldiers=0,m_soldiers_max=0,m_path=0,
function proc_herosoldiers_C( recvValue )
	-- process.
	local type, offset = GetHero():GetIndex( recvValue.m_kind );
	if type == "CityHero" then
		GetHero().m_CityHero[offset].m_soldiers = recvValue.m_soldiers;
	elseif type == "Hero" then
		GetHero().m_Hero[offset].m_soldiers = recvValue.m_soldiers;
	end
	HeroDlgUpdate();
end

-- m_kind=0,m_state=0,
function proc_herostate_C( recvValue )
	-- process.
	local type, offset = GetHero():GetIndex( recvValue.m_kind );
	if type == "CityHero" then
		GetHero().m_CityHero[offset].m_state = recvValue.m_state;
	elseif type == "Hero" then
		GetHero().m_Hero[offset].m_state = recvValue.m_state;
	end
	HeroDlgUpdate();
end

-- m_up_kind=0,m_down_kind=0,
function proc_heroreplace_C( recvValue )
	-- process.
	local up_type, up_offset = GetHero():GetIndex( recvValue.m_up_kind );
	local down_type, down_offset = GetHero():GetIndex( recvValue.m_down_kind );
	
	-- 
	if up_type == down_type and up_type == "CityHero" then
		GetHero().m_CityHero[up_offset], GetHero().m_CityHero[down_offset] = GetHero().m_CityHero[down_offset], GetHero().m_CityHero[up_offset];
		return;
	end
	
	--
	if up_type == down_type and up_type == "Hero" then
		GetHero().m_Hero[up_offset], GetHero().m_Hero[down_offset] = GetHero().m_Hero[down_offset], GetHero().m_Hero[up_offset];
		return;
	end
	
	--
	if up_type == "Hero" then
		GetHero().m_Hero[up_offset], GetHero().m_CityHero[down_offset] = GetHero().m_CityHero[down_offset], GetHero().m_Hero[up_offset];
		return;
	end
	
	--
	if up_type == "CityHero" then
		GetHero().m_CityHero[up_offset], GetHero().m_Hero[down_offset] = GetHero().m_Hero[down_offset], GetHero().m_CityHero[up_offset];
		return;
	end

end

-- m_kind=0,m_path=0,m_hero={m_kind=0,m_color=0,m_level=0,m_corps=0,m_exp=0,m_exp_max=0,m_soldiers=0,m_state=0,m_attack_base=0,m_attack_wash=0,m_defense_base=0,m_defense_wash=0,m_troops_base=0,m_troops_wash=0,m_attack=0,m_defense=0,m_troops=0,m_offset=0,},
function proc_heroget_C( recvValue )
	-- process.
	local pHero = SLK_Hero.new();
	pHero:Set( recvValue.m_hero );
	GetHero():SetHero( recvValue.m_hero.m_offset, pHero );
	HeroGetDlgShow( recvValue );
end

-- m_citylevel=0,m_actorlevel=0,m_silver=0,m_wood=0,m_food=0,m_iron=0,m_sec=0,m_old_value={[8]},m_new_value={[8]},m_maxlevel=0,
function proc_buildingupgradeinfo_C( recvValue )
	-- process.
	BuildingUpgradeDlgRecv( recvValue );
end


-- m_path=0,m_building={m_kind=0,m_offset=0,m_level=0,m_sec=0,m_needsec=0,m_quick=0,m_overvalue=0,},
function proc_buildingget_C( recvValue )
	-- process.
	BuildingGetDlgShow( recvValue.m_building.m_kind, recvValue.m_building.m_offset, recvValue.m_building );
end

-- m_path=0,m_barracks={m_kind=0,m_offset=0,m_level=0,m_sec=0,m_needsec=0,m_quick=0,m_overvalue=0,},
function proc_buildingbarracksget_C( recvValue )
	-- process.
	BuildingGetDlgShow( recvValue.m_building.m_kind, recvValue.m_building.m_offset, recvValue.m_building );
end

-- m_path=0,m_res={m_kind=0,m_offset=0,m_level=0,},
function proc_buildingresGet_C( recvValue )
	-- process.
	BuildingGetDlgShow( recvValue.m_building.m_kind, recvValue.m_building.m_offset, recvValue.m_building );
end

