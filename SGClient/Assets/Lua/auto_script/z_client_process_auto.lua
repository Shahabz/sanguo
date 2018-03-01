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
		if Const.platid > 11 then
			LoginModOpenSDKLogin();
		else
			LoginModOpenTestLogin();
		end
		if recvValue.m_result == -10000 then
			-- 队列满了的情况
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
		if GetPlayer().m_usertype >= 100 then
			SetTrue( GameObject.FindWithTag("UIRoot").transform:Find( "FPS" ) )
			SetTrue( GameObject.FindWithTag("UIRoot").transform:Find( "Mem" ) )
		else
			SetFalse( GameObject.FindWithTag("UIRoot").transform:Find( "FPS" ) )
			SetFalse( GameObject.FindWithTag("UIRoot").transform:Find( "Mem" ) )
		end
	
		-- 启动loading
		LoginModOpenLoading();
	end
end

-- m_actor_num=0,m_listinfo={m_actorid=0,m_name="[22]",m_aclass=0,m_level=0,m_offset=0,m_lockstat=0,m_delete_stoptime=0,m_lock_endtime=0,[m_actor_num]},
function proc_list_C( recvValue )
	-- process.
	-- 无角色
	if recvValue.m_actor_num <= 0 then
		CreateDlgShow( recvValue.m_nation_award );
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
	GetPlayer():SetServerTime( recvValue.m_servertime );
	gamelog( "proc_enterinfo_C" )
	
	-- 读取邮件缓存
	GetMail():LoadCache();
	
	-- 临时调整背景层级
	GameObject.FindWithTag( "UpdateManager" ).Find("Canvas"):GetComponent( "Canvas" ).sortingOrder = 1000
	
	-- 加载主城
	GameManager.ChangeScence( "city" )
end

-- m_actorid=0,m_name="[22]",m_nation=0,m_shape=0,m_level=0,m_exp=0,m_exp_max=0,m_token=0,m_viplevel=0,m_vipexp=0,m_vipexp_max=0,m_body=0,m_place=0,m_official=0,m_zone=0,m_battlepower=0,m_silver=0,m_wood=0,m_food=0,m_iron=0,m_infantry_num=0,m_cavalry_num=0,m_archer_num=0,m_mokilllv=0,m_sflag=0,m_cityid=0,
function proc_actorinfo_C( recvValue )
	-- process.
	-- 打开主界面
	MainDlgOpen();
	MainDlgCutScenesInit();
	
	GetPlayer():Set( recvValue );
	MainDlgSetHead()
	MainDlgSetLevel()
	MainDlgSetNation()
	MainDlgSetExp()
	MainDlgSetBody()
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
	MainDlgSetAutoBuild(0)
	MainDlgSetAutoGuard(0)
	MainDlgSetWeather( recvValue.m_game_day, recvValue.m_game_weather, recvValue.m_game_day_loop );
	SDK.setExtendData()
end

-- m_count=0,m_building={m_kind=0,m_level=0,m_sec=0,m_quick=0,[m_count]},m_count=0,m_barracks={m_kind=0,m_level=0,m_sec=0,m_quick=0,[m_count]},m_count=0,m_res={m_kind=0,m_level=0,m_offset=0,[m_count]},m_levynum=0,m_worker_kind=0,m_worker_offset=0,m_worker_sec=0,m_worker_kind_ex=0,m_worker_offset_ex=0,m_worker_sec_ex=0,m_worker_expire_ex=0,
function proc_buildinglist_C( recvValue )
	-- process.
	-- 设置建筑
	for i=1, recvValue.m_building_count, 1 do
		proc_building_C( recvValue.m_building[i] );	
	end
	
	-- 如果没有获得城墙
	if GetPlayer():GetBuilding( BUILDING_Wall, -1 ) == nil then
		City.BuildingLandShow( BUILDING_Wall, -1 )
		SetFalse( City.m_Fires )
	end
	
	for i=1, recvValue.m_barracks_count, 1 do
		proc_buildingbarracks_C( recvValue.m_barracks[i] )
	end
	
	for i=1, recvValue.m_res_count, 1 do
		proc_buildingres_C( recvValue.m_res[i] )
	end
		
	GetPlayer().m_function = recvValue.m_function;
	-- 铁匠铺
	if Utils.get_int_sflag( recvValue.m_function, CITY_FUNCTION_SMITHY ) == 1 then
		local equipkind = GetPlayer().m_forgingkind;
		if equipkind > 0 then
			local sec = GetPlayer().m_forgingsec;
			local needsec = g_equipinfo[equipkind].sec;
			if sec <= 0 then
				GetPlayer():SetBuilding( BUILDING_Smithy,{ m_kind = BUILDING_Smithy, m_offset = 0,m_level = 0,m_sec = 0, m_needsec = 0,m_quick = 0, m_overvalue=equipkind } )
			else
				GetPlayer():SetBuilding( BUILDING_Smithy,{ m_kind = BUILDING_Smithy, m_offset = 0,m_level = 0,m_sec=sec, m_needsec=needsec, m_quick = 0, m_overvalue=0 } )
			end
		else
			GetPlayer():SetBuilding( BUILDING_Smithy,{ m_kind = BUILDING_Smithy, m_offset = 0,m_level = 0,m_sec = 0, m_needsec = 0,m_quick = 0, m_overvalue=0 } )
		end
	end	
	-- 洗炼铺
	if Utils.get_int_sflag( recvValue.m_function, CITY_FUNCTION_WASH ) == 1 then
		GetPlayer():SetBuilding( BUILDING_Wash,{ m_kind = BUILDING_Wash, m_offset = 0,m_level = 0,m_sec = 0,m_needsec = 0,m_quick = 0, m_overvalue=0 } )
	end
	-- 坊市
	if Utils.get_int_sflag( recvValue.m_function, CITY_FUNCTION_FANGSHI ) == 1 then
		GetPlayer():SetBuilding( BUILDING_Fangshi,{ m_kind = BUILDING_Fangshi, m_offset = 0,m_level = 0,m_sec = 0,m_needsec = 0,m_quick = 0, m_overvalue=0 } )
	else
		-- 帮助
		GetPlayer():SetBuilding( BUILDING_Help,{ m_kind = BUILDING_Help, m_offset = 0,m_level = 0,m_sec = 0,m_needsec = 0,m_quick = 0, m_overvalue=0 } )
	end
	-- 商店
	if Utils.get_int_sflag( recvValue.m_function, CITY_FUNCTION_SHOP ) == 1 then
		GetPlayer():SetBuilding( BUILDING_Shop,{ m_kind = BUILDING_Shop, m_offset = 0,m_level = 0,m_sec = 0,m_needsec = 0,m_quick = 0, m_overvalue=0 } )
	end
	-- 聚贤馆
	if Utils.get_int_sflag( recvValue.m_function, CITY_FUNCTION_HERO ) == 1 then
		GetPlayer():SetBuilding( BUILDING_Hero,{ m_kind = BUILDING_Hero, m_offset = 0,m_level = 0,m_sec = 0,m_needsec = 0,m_quick = 0, m_overvalue=0 } )
	end
	-- 聚宝盆
	if Utils.get_int_sflag( recvValue.m_function, CITY_FUNCTION_WISHING ) == 1 then
		GetPlayer():SetBuilding( BUILDING_Wishing,{ m_kind = BUILDING_Wishing, m_offset = 0,m_level = 0,m_sec = 0,m_needsec = 0,m_quick = 0, m_overvalue=0 } )
	end
	-- 民兵营
	if Utils.get_int_sflag( recvValue.m_function, CITY_FUNCTION_Militiaman ) == 1 then
		local has = 0;
		for i=1, recvValue.m_barracks_count, 1 do
			if recvValue.m_barracks[i].m_kind == BUILDING_Militiaman_Infantry or 
			   recvValue.m_barracks[i].m_kind == BUILDING_Militiaman_Cavalry or 
			   recvValue.m_barracks[i].m_kind == BUILDING_Militiaman_Archer then
				has = 1;
				break;
			end
		end
		if has == 0 then
			City.BuildingLandShow( BUILDING_Militiaman_Infantry, -1 )
		end
	end
	MainDlgSetButtons( -1 );
	GetPlayer():SetBuildingWorker( recvValue )
	GetPlayer():SetBuildingLevy( recvValue.m_levynum )
	
	-- 战前补兵开启关闭
	if Utils.get_int_sflag( recvValue.m_function, CITY_FUNCTION_BATTLE_ADDHP ) == 1 then
		GameManager.writeini( "SETTING_AUTOSUPPLY", 1 )
	else
		GameManager.writeini( "SETTING_AUTOSUPPLY", 0 )
	end
	City.m_InitOver = 1;
	-- 可升级箭头
	City.UpgradeArrow()
	
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
		m_value = recvValue.m_value,
		m_overvalue = recvValue.m_overvalue,
		} )
	
	if recvValue.m_kind == BUILDING_Tech then
		CityTechDlgOnSet();
		if recvValue.m_sec <= 0 then
			CityTechDlgClose();
		end
	
	elseif recvValue.m_kind == BUILDING_Wall then
		if Utils.byteAndOp( GetPlayer().m_state, CITY_STATE_FIRE ) == CITY_STATE_FIRE then
			-- 着火
			City.BuildingSetShape( {m_kind=BUILDING_Wall,m_offset=-1}, "building_kind2_fire" )
			SetTrue( City.m_Fires )
		else
			City.BuildingSetShape( {m_kind=BUILDING_Wall,m_offset=-1}, "building_kind2" )
			SetFalse( City.m_Fires )
		end
	end
	
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
	GovInfoDlgSetBuffWorker();
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
	GetItem():ItemUsed( recvValue.m_itemoffset, recvValue.m_usenum, recvValue.m_effres );
end

-- m_res_offset=0,m_res_num=0,m_target_offset=0,m_target_num=0,
function proc_itemput_C( recvValue )
	-- process.
	--GetItem():ItemPut( recvValue.m_res_offset, recvValue.m_res_num, recvValue.m_target_offset, recvValue.m_target_num );
end

-- m_itemnum=0,m_itemoffset={}[m_itemnum],
function proc_itemsettle_C( recvValue )
	-- process.
	--GetItem():ItemSettle( recvValue.m_itemoffset, recvValue.m_itemnum );
end

-- m_itemoffset=0,m_itemnum=0,m_targetid=0,m_path=0,
function proc_lostitem_C( recvValue )
	-- process.
	GetItem():ItemLost( recvValue.m_itemoffset, recvValue.m_itemnum, recvValue.m_path );
end

-- m_itemoffset=0,m_kind=0,m_type=0,m_color=0,m_num=0,m_targetid=0,m_path=0,m_situation=0,
function proc_getitem_C( recvValue )
	-- process.
	GetItem():ItemGet( recvValue.m_itemoffset, recvValue.m_kind, recvValue.m_type, recvValue.m_num, recvValue.m_color, recvValue.m_situation, recvValue.m_path );
end

-- m_itemext=0,m_kindnum=0,m_item={m_offset=0,m_kind=0,m_num=0,m_situation=0,m_color_level=0,[m_kindnum]},
function proc_itemlist_C( recvValue )
	-- process.
    for tmpi = 1, recvValue.m_kindnum, 1 do
		if recvValue.m_item[tmpi].m_offset >= 0 then
			local tmpItem = SLK_Item.new();
			tmpItem.m_kind = recvValue.m_item[tmpi].m_kind;
			tmpItem.m_num = recvValue.m_item[tmpi].m_num;
			tmpItem.m_situation = recvValue.m_item[tmpi].m_situation;
            tmpItem.m_color_level = recvValue.m_item[tmpi].m_color_level;
			
			-- 类型和颜色级别从配置信息里拿
			local itemconfig = item_getinfo( tmpItem.m_kind );
			if itemconfig then
				tmpItem.m_type = itemconfig.type;
				tmpItem.m_color_level = itemconfig.color_level;
			end

			-- 背包栏
			if recvValue.m_item[tmpi].m_offset < MAX_ITEMNUM  then			
				GetItem():SetItem( recvValue.m_item[tmpi].m_offset, tmpItem );
			end
		end
	end
end

-- m_itemoffset=0,m_itemkind=0,m_type=0,m_level=0,m_color_level=0,m_price=0,m_attr_num=0,m_attr={m_type=0,m_ability=0,m_value=0,m_addvalue=0,[m_attr_num]},
function proc_iteminfo_C( recvValue )
	-- process.
    if recvValue.m_itemoffset >= 0 then
		GetItem():ItemInfo( recvValue, nil );
		local pItem = GetItem():GetAnyItem( recvValue.m_itemoffset );
		if pItem == nil then
			return;
		end
		-- 如果物品信息框正在显示，那么就更新到物品显示框
		BagDlgItemAbilityFormat( pItem );
	end
end

-- m_count=0,m_list={m_kind=0,m_num=0,[m_count]},m_callback_code=0,m_value
function proc_awardinfolist_C( recvValue )
	-- process.
	if recvValue.m_callback_code == 1 then
		MapEnemyDlgRecvAward( recvValue )
	elseif recvValue.m_callback_code == 2 then
		MapTownDlgRecvAward( recvValue )
	elseif recvValue.m_callback_code == 3 then
		WorldQuestInfoDlgRecvAward( recvValue )
	elseif recvValue.m_callback_code == 4 then
		MapNationHeroDlgRecvAward( recvValue )
	end
end

-- m_type=0,m_state=0,m_posx=0,m_unit_index=0,m_posy=0,m_namelen=0,m_name="[m_namelen]",m_char_value_count=0,m_char_value={}[m_char_value_count],m_short_value_count=0,m_short_value={}[m_short_value_count],m_int_value_count=0,m_int_value={}[m_int_value_count],m_prefixlen=0,m_prefix="[m_prefixlen]",
function proc_addmapunit_C( recvValue )
	-- process.
	if recvValue.m_type == MAPUNIT_TYPE_EVENT then
		if recvValue.m_int_value[1] ~= GetPlayer().m_actorid then
			return
		end
	end
	WorldMap.QueueAdd( 1, recvValue );
end

-- m_unit_index=0,
function proc_delmapunit_C( recvValue )
	-- process.
	WorldMap.QueueAdd( 2, recvValue );
end

-- m_info={m_type=0,m_state=0,m_posx=0,m_unit_index=0,m_posy=0,m_namelen=0,m_name="[m_namelen]",m_char_value_count=0,m_char_value={}[m_char_value_count],m_short_value_count=0,m_short_value={}[m_short_value_count],m_int_value_count=0,m_int_value={}[m_int_value_count],m_prefixlen=0,m_prefix="[m_prefixlen]",},
function proc_updatemapunit_C( recvValue )
	-- process.
	if recvValue.m_info.m_type == MAPUNIT_TYPE_EVENT then
		if recvValue.m_info.m_int_value[1] ~= GetPlayer().m_actorid then
			return
		end
	end
	WorldMap.QueueAdd( 3, recvValue.m_info );
end

-- m_type=0,m_state=0,m_posx=0,m_unit_index=0,m_posy=0,m_namelen=0,m_name="[m_namelen]",m_char_value_count=0,m_char_value={}[m_char_value_count],m_short_value_count=0,m_short_value={}[m_short_value_count],m_int_value_count=0,m_int_value={}[m_int_value_count],m_prefixlen=0,m_prefix="[m_prefixlen]",
function proc_mapunitsingle_C( recvValue )
	-- process.
end

-- m_area_width=0,m_area_height=0,m_map_area_xnum=0,m_map_area_ynum=0,m_my_city_posx=0,m_my_city_posy=0,m_my_city_unit_index=0,m_citystate=0,m_target_posx=0,m_target_posy=0,
function proc_worldmapinfo_C( recvValue )
	-- process.
	WorldMap.Recv( recvValue )
end

-- m_unit_index=0,m_posx=0,m_posy=0,
function proc_mapunitcorrdinate_C( recvValue )
	-- process.
	WorldMap.QueueAdd( 4, recvValue );
end

-- m_from_type=0,m_from_posx=0,m_from_posy=0,m_to_type=0,m_to_posx=0,m_to_posy=0,m_state=0,m_from_cityid=0,m_from_clubid=0,m_to_cityid=0,m_to_clubid=0,m_army_index=0,m_action=0,
function proc_addmarchroute_C( recvValue )
	-- process.
	print("proc_addmarchroute_C")
	WorldMap.QueueAdd( 5, recvValue );
end


-- m_army_index=0,
function proc_delmarchroute_C( recvValue )
	-- process.
	WorldMap.QueueAdd( 6, recvValue.m_army_index );
end

-- m_addexp=0,m_curexp=0,m_isup=0,m_level, m_expmax, m_path=0,
function proc_experience_C( recvValue )
	-- process.
	if recvValue.m_path ~= PATH_STORY and recvValue.m_path ~= PATH_STORY_SWEEP then
		if recvValue.m_addexp > 0 then
			pop( T(120)..": "..T(128).."x"..recvValue.m_addexp );
		end
	end
	GetPlayer().m_level = recvValue.m_level;
	GetPlayer().m_exp = recvValue.m_curexp;
	GetPlayer().m_exp_max = recvValue.m_expmax;
	MainDlgSetExp();
	if recvValue.m_isup == 1 then
		if recvValue.m_path ~= PATH_STORY and recvValue.m_path ~= PATH_STORY_SWEEP then
			pop( T(150) );
		end
		MainDlgSetLevel();
	end
end

-- m_total=0,m_add=0,m_path=0,
function proc_body_C( recvValue )
	-- process.
	if recvValue.m_path ~= PATH_SYSTEM then
		if recvValue.m_add > 0 then
			pop( T(120)..": "..T(126).."x"..recvValue.m_add );
		end
	end
	
	GetPlayer().m_body = recvValue.m_total;
	GetPlayer().m_bodysec = recvValue.m_bodysec;
	MainDlgSetBody()
end

-- m_total=0,m_add=0,m_path=0,
function proc_changelevy_C( recvValue )
	-- process.
	GetPlayer():SetBuildingLevy( recvValue.m_total )
	LevyDlgChangeSec( recvValue.m_sec )
end

-- m_total=0,m_add=0,m_path=0,
function proc_changesilver_C( recvValue )
	-- process.
	if recvValue.m_path ~= PATH_STORY and recvValue.m_path ~= PATH_STORY_SWEEP then
		if recvValue.m_add > 0 then
			pop( T(120)..": "..T(121).."x"..recvValue.m_add );
		end
	end
	GetPlayer().m_silver = recvValue.m_total;
	MainDlgSetSilver();
	if recvValue.m_add > 0 then
		NationUpgradeDlgSetCost()
		NationPlaceDlgRecv()
	end
end

-- m_total=0,m_add=0,m_path=0,
function proc_changewood_C( recvValue )
	-- process.
	if recvValue.m_path ~= PATH_STORY and recvValue.m_path ~= PATH_STORY_SWEEP then
		if recvValue.m_add > 0 then
			pop( T(120)..": "..T(122).."x"..recvValue.m_add );
		end
	end
	GetPlayer().m_wood = recvValue.m_total;
	MainDlgSetWood();
	if recvValue.m_add > 0 then
		NationUpgradeDlgSetCost()
	end
end

-- m_total=0,m_add=0,m_path=0,
function proc_changefood_C( recvValue )
	-- process.
	if recvValue.m_path ~= PATH_STORY and recvValue.m_path ~= PATH_STORY_SWEEP then
		if recvValue.m_add > 0 then
			pop( T(120)..": "..T(123).."x"..recvValue.m_add );
		end
	end
	GetPlayer().m_food = recvValue.m_total;
	MainDlgSetFood();
	HeroGuardDlgSetFood()
	if recvValue.m_add > 0 then
		NationUpgradeDlgSetCost()
	end
end

-- m_total=0,m_add=0,m_path=0,
function proc_changeiron_C( recvValue )
	-- process.
	if recvValue.m_path ~= PATH_STORY and recvValue.m_path ~= PATH_STORY_SWEEP then
		if recvValue.m_add > 0 then
			pop( T(120)..": "..T(124).."x"..recvValue.m_add );
		end
	end
	GetPlayer().m_iron = recvValue.m_total;
	MainDlgSetIron();
	NationEquipDlgSetIron( 1 );
end

-- m_total=0,m_add=0,m_path=0,
function proc_changepeople_C( recvValue )
	-- process.
end

-- m_total=0,m_add=0,m_path=0,
function proc_changeprestige_C( recvValue )
	-- process.
	if recvValue.m_path == PATH_FIGHT then
	else
		if recvValue.m_add > 0 then
			pop( T(120)..": "..T(152).."x"..recvValue.m_add );
		end
	end
	GetPlayer().m_prestige = recvValue.m_total;
	NationDlgChangePrestige()
	if recvValue.m_add > 0 then
		NationPlaceDlgRecv()
	end
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
	PayDlgUpdateVip()
	VipDlgUpdateVip()
	
	if recvValue.m_addexp > 0 then
		pop( T(120)..": "..T(190).."x"..recvValue.m_addexp );
	end
end

-- m_total=0,m_add=0,m_path=0,
function proc_changtoken_C( recvValue )
	-- process.
	if recvValue.m_add > 0 then
		if recvValue.m_path == PATH_WISHINGSHOP then
			ItemGetDlgShow( 124, recvValue.m_add )
		else
			pop( T(120)..": "..T(125).."x"..recvValue.m_add );
		end
	end
	GetPlayer().m_token = recvValue.m_total;
	MainDlgSetToken()
	DialogFrameModChangeToken()
	BloodyBattleDlgChangeToken()
	ActivityDlgChangeToken()
end

-- m_offset=0,m_kind=0,m_washid={[4]},
function proc_equip_C( recvValue )
	-- process.
	local tmpEquip = SLK_Equip.new();
	tmpEquip.m_kind = recvValue.m_kind;
	tmpEquip.m_offset = recvValue.m_offset;
	for i=1,4,1 do
		tmpEquip.m_washid[i] = recvValue.m_washid[i];
	end
	if recvValue.m_offset < 1000 then
		GetEquip():SetEquip( recvValue.m_offset, tmpEquip );
	else
		GetHero():SetEquip( recvValue.m_offset, tmpEquip );
	end
	EquipWashDlgSetWash( tmpEquip )
	EquipWashDlgUpdate( tmpEquip )
end

-- m_equipext=0,m_count=0,m_list={m_offset=0,m_kind=0,m_washid={[4]},[m_count]},
function proc_equiplist_C( recvValue )
	-- process.
	GetPlayer().m_equipext = recvValue.m_equipext;
	for tmpi = 1, recvValue.m_count, 1 do
		proc_equip_C( recvValue.m_list[tmpi] )
	end
end

-- m_offset=0,m_kind=0,m_path=0,m_washid={[4]}
function proc_equipget_C( recvValue )
	-- process.
	GetEquip():EquipGet( recvValue.m_offset, recvValue.m_kind, recvValue.m_washid, recvValue.m_path )
end

-- m_offset=0,m_kind=0,m_path=0,
function proc_equiplost_C( recvValue )
	-- process.
	if recvValue.m_offset < 1000 then
		GetEquip():EquipLost( recvValue.m_offset, recvValue.m_path );
	else
		GetHero():SetEquip( recvValue.m_offset, nil );
	end
end

-- m_kind=0,m_color=0,m_level=0,m_corps=0,m_exp=0,m_exp_max=0,m_soldiers=0,m_state=0,m_attack_base=0,m_attack_wash=0,m_defense_base=0,m_defense_wash=0,m_troops_base=0,m_troops_wash=0,m_attack=0,m_defense=0,m_troops=0,m_offset=0,
function proc_hero_C( recvValue )
	-- process.
	local type, offset = GetHero():GetIndex( recvValue.m_kind );
	--print( "kind:"..recvValue.m_kind.." type:"..type.." offset:"..offset )
	if type == "CityHero" then
		GetHero().m_CityHero[offset]:Set( recvValue );
	elseif type == "Hero" then
		GetHero().m_Hero[offset]:Set( recvValue );
	end
	HeroDlgUpdate();
	HeroGatherDlgUpdate();
	HeroGuardDlgUpdate();
	HeroInfoDlgUpdate( recvValue.m_kind );
	HeroListDlgUpdate();
	HeroWashDlgUpdate();
end

-- m_count=0,m_list={m_kind=0,m_color=0,m_level=0,m_corps=0,m_exp=0,m_exp_max=0,m_soldiers=0,m_state=0,m_attack_base=0,m_attack_wash=0,m_defense_base=0,m_defense_wash=0,m_troops_base=0,m_troops_wash=0,m_attack=0,m_defense=0,m_troops=0,m_offset=0,[m_count]},m_type=0,
function proc_herolist_C( recvValue )
	-- process.
	if recvValue.m_type == 0 then
		for i=1, recvValue.m_count, 1 do
			local pHero = SLK_Hero.new();
			pHero:Set( recvValue.m_list[i] );
			GetHero():SetCityHero( recvValue.m_list[i].m_offset-HERO_BASEOFFSET, pHero );
		end
	elseif recvValue.m_type == 1 then
		for i=1, recvValue.m_count, 1 do
			local pHero = SLK_Hero.new();
			pHero:Set( recvValue.m_list[i] );
			GetHero():SetHero( recvValue.m_list[i].m_offset, pHero );
		end
	
	-- 财赋署武将
	elseif recvValue.m_type == 11 then
		GetHero():ResetCityHero( 4, 7 )
		for i=1, recvValue.m_count, 1 do
			local pHero = SLK_Hero.new();
			pHero:Set( recvValue.m_list[i] );
			GetHero():SetCityHero( recvValue.m_list[i].m_offset-HERO_BASEOFFSET, pHero );
		end
	
	-- 御林卫武将
	elseif recvValue.m_type == 12 then
		GetHero():ResetCityHero( 8, 11 )
		for i=1, recvValue.m_count, 1 do
			local pHero = SLK_Hero.new();
			pHero:Set( recvValue.m_list[i] );
			GetHero():SetCityHero( recvValue.m_list[i].m_offset-HERO_BASEOFFSET, pHero );
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
	HeroInfoDlgUpdate( recvValue.m_kind );
	HeroExpDlgUpdate();
	
	if recvValue.m_add > 0 then
		if recvValue.m_path == PATH_ITEMUSE or recvValue.m_path == PATH_TOKENITEMUSE then
			pop( HeroName(recvValue.m_kind)..T(128).."+"..recvValue.m_add )
		end
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
	HeroGatherDlgUpdate();
	HeroGuardDlgUpdate();
	HeroInfoDlgUpdate( recvValue.m_kind )
	MapBattleDlgUpdate( recvValue.m_kind, recvValue.m_soldiers );
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
	HeroGatherDlgUpdate();
	HeroGuardDlgUpdate();
	HeroInfoDlgUpdate( recvValue.m_kind )
end

-- m_up_kind=0,m_down_kind=0,m_up_offset=0,m_down_offset=0
function proc_heroreplace_C( recvValue )
	-- process.
	local up_type, up_offset = GetHero():GetIndex( recvValue.m_up_kind );
	local down_type, down_offset = GetHero():GetIndex( recvValue.m_down_kind );
	--print( "up_type:"..up_type.." up_offset:"..up_offset.." down_type:"..down_type.." down_offset:"..down_offset )
	
	-- 两个上阵英雄之间的交换
	if up_type == down_type and up_type == "CityHero" then
		local tmp = clone( GetHero().m_CityHero[up_offset] )
		GetHero().m_CityHero[up_offset] = clone( GetHero().m_CityHero[down_offset] )
		GetHero().m_CityHero[down_offset] = tmp;
		HeroListDlgLoadHero();
		return;
	end
	
	-- 两个背包英雄之间的交换
	if up_type == down_type and up_type == "Hero" then
		local tmp = clone( GetHero().m_Hero[up_offset] )
		GetHero().m_Hero[up_offset] = clone( GetHero().m_Hero[down_offset] )
		GetHero().m_Hero[down_offset] = tmp;
		HeroListDlgLoadHero();
		return;
	end
	
	-- 背包英雄和上阵英雄的交换
	if up_type == "Hero" then
		-- 如果是单纯上阵，没有交换行为
		if recvValue.m_down_kind == 0 then 
			down_offset = recvValue.m_down_offset
		end
		local tmp = clone( GetHero().m_Hero[up_offset] )
		GetHero().m_Hero[up_offset] = clone( GetHero().m_CityHero[down_offset] )
		GetHero().m_CityHero[down_offset] = tmp;
		HeroListDlgLoadHero();
		return;
	end
	
	-- 处理单纯下阵
	-- 下阵up_type: up_offset:-1 down_type:CityHero down_offset:4
	if up_type == "" then
		local offset = -1;
		for i = 0, MAX_HERONUM-1, 1 do
			if GetHero().m_Hero[i].m_kind <= 0 then 
				offset = i
				break;
			end
		end
		if offset < 0 then
			return -1;
		end
		GetHero().m_Hero[offset] = clone( GetHero().m_CityHero[down_offset] )
		GetHero().m_CityHero[down_offset] = GetHero().m_CityHero[down_offset]:empty();
		HeroListDlgLoadHero();
	end
end

-- m_kind=0,m_path=0,m_itemnum=0,m_hero={m_kind=0,m_color=0,m_level=0,m_corps=0,m_exp=0,m_exp_max=0,m_soldiers=0,m_state=0,m_attack_base=0,m_attack_wash=0,m_defense_base=0,m_defense_wash=0,m_troops_base=0,m_troops_wash=0,m_attack=0,m_defense=0,m_troops=0,m_offset=0,},
function proc_heroget_C( recvValue )
	-- process.
	local pHero = SLK_Hero.new();
	pHero:Set( recvValue.m_hero );
	GetHero():SetHero( recvValue.m_hero.m_offset, pHero );
	if recvValue.m_path == PATH_HEROVISIT then
	elseif recvValue.m_path == PATH_NATIONHERO then
		NationHeroGetDlgShow( recvValue.m_kind )
	else
		HeroGetDlgShow( recvValue.m_hero );
	end
end

-- m_citylevel=0,m_actorlevel=0,m_silver=0,m_wood=0,m_food=0,m_iron=0,m_sec=0,m_old_value={[8]},m_new_value={[8]},m_maxlevel=0,
function proc_buildingupgradeinfo_C( recvValue )
	-- process.
	BuildingUpgradeDlgRecv( recvValue );
end


-- m_path=0,m_building={m_kind=0,m_offset=0,m_level=0,m_sec=0,m_needsec=0,m_quick=0,m_overvalue=0,},
function proc_buildingget_C( recvValue )
	-- process.
	if NpcTalkIsShow() == true then
		NpcTalkWait( BuildingGetDlgShow, recvValue.m_building )
	else
		BuildingGetDlgShow( recvValue.m_building );
	end
end

-- m_path=0,m_barracks={m_kind=0,m_offset=0,m_level=0,m_sec=0,m_needsec=0,m_quick=0,m_overvalue=0,},
function proc_buildingbarracksget_C( recvValue )
	-- process.
	if NpcTalkIsShow() == true then
		NpcTalkWait( BuildingGetDlgShow, recvValue.m_barracks )
	else
		BuildingGetDlgShow( recvValue.m_barracks );
	end
end

-- m_path=0,m_res={m_kind=0,m_offset=0,m_level=0,},
function proc_buildingresget_C( recvValue )
	-- process.
	if NpcTalkIsShow() == true then
		NpcTalkWait( BuildingGetDlgShow, recvValue.m_res )
	else
		BuildingGetDlgShow( recvValue.m_res );
	end
end

-- m_corps=0,m_soldiers=0,m_add=0,m_path=0,
function proc_soldiers_C( recvValue )
	-- process.
	if recvValue.m_corps == 0 then
		GetPlayer().m_infantry_num = recvValue.m_soldiers;
		MainDlgSetInfantry();
		HeroDlgSetInfantry();
		BloodyBattleDlgSetInfantry()
	elseif recvValue.m_corps == 1 then
		GetPlayer().m_cavalry_num = recvValue.m_soldiers;
		MainDlgSetCavalry();
		HeroDlgSetCavalry();
		BloodyBattleDlgSetCavalry()
	elseif recvValue.m_corps == 2 then
		GetPlayer().m_archer_num = recvValue.m_soldiers;
		MainDlgSetArcher();
		HeroDlgSetArcher();
		BloodyBattleDlgSetArcher()
	end
	
	if recvValue.m_add > 0 then
		pop( T(605)..": "..CorpsName(recvValue.m_corps).."x"..recvValue.m_add );
	end
end

-- m_soldiers=0,m_soldiers_max=0,m_trainnum=0,m_trainsec=0,m_trainsec_need=0,m_queuenum={[16]},m_queue=0,m_trainlong=0,m_train_confnum=0,m_train_confsec=0,
function proc_traininfo_C( recvValue )
	-- process.
	TrainDlgRecv( recvValue )
end

-- m_questid=0,m_flag=0,m_datatype=0,m_datakind=0,m_dataoffset=0,m_value=0,m_needvalue=0,m_awardkind={[5]},m_awardnum={[5]},m_nameid=0,
function proc_quest_C( recvValue )
	-- process.
end

-- m_count=0,m_list={m_questid=0,m_flag=0,m_datatype=0,m_datakind=0,m_dataoffset=0,m_value=0,m_needvalue=0,m_awardkind={[5]},m_awardnum={[5]},m_nameid=0,[m_count]},
function proc_questlist_C( recvValue )
	-- process.
	CacheQuestClear();
	CacheQuestSet( recvValue );
	MainDlgSetQuest();
end

-- m_questid=0,m_count=0,m_list={m_kind=0,m_num=0,[m_count]},m_datatype=0,m_datakind=0,m_dataoffset=0,m_value=0,m_needvalue=0,m_nameid=0,
function proc_questaward_C( recvValue )
	-- process.
	if FightDlgIsShow() == true then
		FightDlgWait( QuestAwardDlgShow, recvValue )
	elseif HeroGetDlgIsShow() == true then
		HeroGetDlgWait( QuestAwardDlgShow, recvValue )
	elseif NpcTalkIsShow() == true then
		NpcTalkWait( QuestAwardDlgShow, recvValue )
	else
		QuestAwardDlgShow( recvValue )
	end
end

-- m_function=0,m_openoffset=0,m_path=0,
function proc_function_C( recvValue )
	-- process.
	GetPlayer().m_function = recvValue.m_function;
	
	if recvValue.m_openoffset >= 0 and recvValue.m_openoffset <= 5 then
		local kindlist = {
		[0] = BUILDING_Smithy,
		[1] = BUILDING_Wash,
		[2] = BUILDING_Fangshi,
		[3] = BUILDING_Shop,
		[4] = BUILDING_Hero,
		[5] = BUILDING_Wishing }
		local kind = kindlist[ recvValue.m_openoffset ]
		BuildingGetDlgShow( { m_kind = kind, m_offset = 0,m_level = 0,m_sec = 0,m_needsec = 0,m_quick = 0, m_overvalue=0 } );
		
	elseif recvValue.m_openoffset == CITY_FUNCTION_Militiaman then
		City.BuildingLandShow( BUILDING_Militiaman_Infantry, -1 )
	else
		MainDlgSetButtons( recvValue.m_openoffset );
	end
end


-- m_corps=0,m_color=0,m_shape=0,m_level=0,m_soldiers=0,m_troops=0,m_offset=0,
function proc_cityguard_C( recvValue )
	-- process.
	CityGuardDlgSet( recvValue );
end

-- m_count=0,m_list={m_corps=0,m_color=0,m_shape=0,m_level=0,m_soldiers=0,m_troops=0,m_offset=0,[m_count]},m_guardsec=0,
function proc_cityguardlist_C( recvValue )
	-- process.
	GetPlayer().m_guardsec = recvValue.m_guardsec
	CityGuardDlgRecv( recvValue );
end

-- m_guardsec=0,
function proc_cityguardsec_C( recvValue )
	-- process.
	CityGuardDlgChangeSec( recvValue );
	GetPlayer().m_guardsec = recvValue.m_guardsec;
	if GetPlayer().m_guardsec > 0 then
		City.GuardCallMod( nil, false )
	else
		City.GuardCallMod( nil, true )
	end
end

-- m_forgingkind=0,m_forgingsec=0,m_forgingsec_need=0,
function proc_buildingsmithy_C( recvValue )
	-- process.
	GetPlayer().m_forgingkind = recvValue.m_forgingkind
	GetPlayer().m_forgingsec = recvValue.m_forgingsec
	local equipkind = GetPlayer().m_forgingkind;
	if equipkind > 0 then
		local sec = GetPlayer().m_forgingsec;
		local needsec = g_equipinfo[equipkind].sec;
		if sec <= 0 then
			GetPlayer():SetBuilding( BUILDING_Smithy,{ m_kind = BUILDING_Smithy, m_offset = 0,m_level = 0,m_sec = 0, m_needsec = 0,m_quick = 0, m_overvalue=equipkind } )
		else
			GetPlayer():SetBuilding( BUILDING_Smithy,{ m_kind = BUILDING_Smithy, m_offset = 0,m_level = 0,m_sec=sec, m_needsec=needsec, m_quick = 0, m_overvalue=0 } )
		end
	else
		GetPlayer():SetBuilding( BUILDING_Smithy,{ m_kind = BUILDING_Smithy, m_offset = 0,m_level = 0,m_sec = 0, m_needsec = 0,m_quick = 0, m_overvalue=0 } )
	end
	EquipForgingDlgTimer()
end

-- m_name_length=0,m_name="[m_name_length]",
function proc_changename_C( recvValue )
	-- process.
	GetPlayer().m_name = recvValue.m_name;
	ChangeNameDlgClose();
	PlayerDlgSet();
end

-- m_kind=0,m_offset=0,m_action=0,
function proc_buildingaction_C( recvValue )
	-- process.
	if recvValue.m_action == 1 then
		City.BuildingSetUpgradeing( recvValue.m_kind, recvValue.m_offset, 0, 0, 0 )
	elseif recvValue.m_action == 2 then
		City.BuildingDel( { m_kind=recvValue.m_kind, m_offset=recvValue.m_offset } )
	end
end

-- m_base={[4]},m_tech={[4]},m_weather={[4]},m_activity={[4]},m_offical={[4]},
function proc_levyinfo_C( recvValue )
	-- process.
	LevyDlgRecv( recvValue )
end



-- m_actorid=0,m_shape=0,m_level=0,m_namelen=0,m_name="[m_namelen]",m_frame=0,m_zone=0,m_place=0,m_msglen=0,m_msg="[m_msglen]",m_optime=0,m_channel=0,m_nation=0,
function proc_chat_C( recvValue )
	-- process.
	if GetPlayer():CheckBlacklist( recvValue.m_actorid ) == true then
		return
	end
	ChatDlgRecv( recvValue );
	MainDlgSetChat( recvValue );
	BloodyBattleDlgSetChat( recvValue );
end

-- m_count=0,m_list={m_actorid=0,m_shape=0,m_level=0,m_namelen=0,m_name="[m_namelen]",m_frame=0,m_zone=0,m_place=0,m_msglen=0,m_msg="[m_msglen]",m_optime=0,m_channel=0,m_nation=0,[m_count]},m_channel=0,
function proc_chatlist_C( recvValue )
	-- process.
	for i=1, recvValue.m_count, 1 do
		if GetPlayer():CheckBlacklist( recvValue.m_list[i].m_actorid ) == false then
			ChatDlgRecv( recvValue.m_list[i] );
			if i == recvValue.m_count then
				MainDlgSetChat( recvValue.m_list[i] );
				BloodyBattleDlgSetChat( recvValue.m_list[i] );
			end
		end
	end
end

-- m_count=0,m_msglist={m_vlen=0,m_v="[m_vlen]",[m_count]},m_textid=0,m_optime,
function proc_systalkid_C( recvValue )
	-- process.
end

-- m_msglen=0,m_msg="[m_msglen]",m_optime,
function proc_systalk_C( recvValue )
	-- process.
	local info = {}
	info.m_actorid = -1;
	info.m_name = T(564);
	info.m_msg = recvValue.m_msg;
	info.m_optime = recvValue.m_optime;
	MainDlgSetChat( info );
	BloodyBattleDlgSetChat( info );
	ChatDlgRecv( info );
	
	-- 有滚动消息
	if recvValue.m_roll == 1 then
		RollingMsgDlgShowMsg( recvValue.m_msg );
	end
end


-- m_total=0,m_path=0,
function proc_battlepower_C( recvValue )
	-- process.
	GetPlayer().m_battlepower = recvValue.m_total;
end

-- m_kind=0,m_level=0,m_progress=0,
function proc_techchange_C( recvValue )
	-- process.
	local kind = recvValue.m_kind;
	GetPlayer().m_techlevel[kind] = recvValue.m_level;
	GetPlayer().m_techprogress[kind] = recvValue.m_progress;
	if recvValue.m_progress > 0 then
		pop( F( 707, TechName(kind).."Lv"..GetPlayer().m_techlevel[kind], recvValue.m_progress ) )
	else
		pop( F( 706, TechName(kind).."Lv"..GetPlayer().m_techlevel[kind] ) )
	end
end

-- m_cevent_count=0,m_cevent_list={m_type=0,m_kind=0,m_value=0,m_optime=0,[m_cevent_count]},m_bevent_count=0,m_bevent_list={m_type=0,m_name="[22]",m_value=0,m_optime=0,[m_bevent_count]},
function proc_cityeventlist_C( recvValue )
	-- process.
	GovInfoDlgRecv( recvValue );
end


-- m_type=0,m_info={m_ofkind=0,m_ofsec=0,m_offree=0,m_offquick=0,},
function proc_officialhirechange_C( recvValue )
	-- process.
	GetPlayer().m_officialhire[recvValue.m_type] = recvValue.m_info;
	OfficialHireDlgSet( recvValue.m_type )
	if recvValue.m_type == 0 then
		EquipForgingDlgSetOfficial()
	elseif recvValue.m_type == 1 then
		LevyDlgSetOfficial();
	elseif recvValue.m_type == 2 then
		CityTechDlgSetOfficial();
	end
end

-- m_total=0,m_add=0,m_path=0,
function proc_cityprotect_C( recvValue )
	-- process.
	GetPlayer().m_ptsec = recvValue.m_total
	GovInfoDlgSetBuffProtect();
end

-- m_herokind=0,m_count=0,m_list={m_offset=0,m_kind=0,m_washid={[4]},[m_count]},
function proc_heroequip_C( recvValue )
	-- process.
	for tmpi = 1, recvValue.m_count, 1 do
		local tmpEquip = SLK_Equip.new();
		tmpEquip.m_kind = recvValue.m_list[tmpi].m_kind;
		tmpEquip.m_offset = recvValue.m_list[tmpi].m_offset;
		for i=1,4,1 do
			tmpEquip.m_washid[i] = recvValue.m_list[tmpi].m_washid[i];
		end
		GetHero():UpdateEquip( recvValue.m_herokind, tmpi-1, tmpEquip );
	end
end

-- m_hero_washnum=0,m_hero_washsec=0,
function proc_herowash_C( recvValue )
	-- process.
	HeroWashDlgWashInfo( recvValue )
end

-- m_value=0,
function proc_herocolorup_C( recvValue )
	-- process.
	HeroColorupDlgRecv( recvValue );
end

-- m_protectres_per=0,m_buildingsec_per=0,m_materialsec_per=0,m_hero_up_num=0,m_hero_row_num=0,m_everyday_body_buymax=0,m_everyday_autobuild_buynum=0,m_everyday_army_recall=0,m_ability_open_201=0,m_ability_open_203=0,m_ability_open_204=0,
function proc_cityattr_C( recvValue )
	-- process.
	GetPlayer():SetAttr( recvValue )
end

-- m_equip_washnum=0,m_equip_washsec=0,
function proc_equipwash_C( recvValue )
	-- process.
	EquipWashDlgRecv( recvValue )
end


-- m_count=0,m_list={m_matkind=0,m_matnum=0,m_matsec=0,m_matsec_need=0,[m_count]},m_matquenum=0,
function proc_materiallist_C( recvValue )
	-- process.
	MaterialMakeDlgRecv( recvValue );
end

-- m_count=0,m_list={m_matkind=0,m_matsec=0,m_matkind_will=0,m_matnum_will=0,[m_count]},
function proc_materialwilllist_C( recvValue )
	-- process.
	MaterialMakeWillDlgRecv( recvValue )
end

-- m_story_star={[128]},m_story_hero={[32]},m_story_restime={[32]},m_story_resnum={[32]},m_story_resreset={[32]},m_story_itemnum={[32]},m_story_drawing={[16]},m_storyid=0,
function proc_storylist_C( recvValue )
	-- process.
	StoryDlgRecv( recvValue )
end

-- m_count=0,m_list={m_monsterid=0,m_shape=0,m_level=0,m_color=0,m_corps=0,m_hp,[m_count]},m_exp=0,m_body=0,m_type=0
function proc_storyrank_C( recvValue )
	-- process.
	BattleDlgStoryRecv( recvValue )
end

-- m_storyid=0,m_state=0,m_savetype=0,m_saveoffset=0,m_actor_storyid=0
function proc_storystate_C( recvValue )
	-- process.
	StoryDlgStateRecv( recvValue )
end

-- m_storyid=0,m_num=0,m_saveoffset=0,
function proc_storyranknum_C( recvValue )
	-- process.
end

-- m_storyid=0,m_time=0,m_saveoffset=0,
function proc_storyranktime_C( recvValue )
	-- process.
end

-- m_zoneid=0,m_open=0,m_nation=0
function proc_mapzonechange_C( recvValue )
	-- process.
	local name = MapZoneName( recvValue.m_zoneid )
	if recvValue.m_open == 1 then
		MainDlgSetZoneName( name )
		MapZoneTipsModShow( name, recvValue.m_nation )
	else
		if recvValue.m_open == -1 then
			AlertMsg( T(2369) )
		else
			AlertMsg( T(937) )
		end
		MainDlgSetZoneName( name.."("..T(936)..")" )
	end
	WorldMap.m_nZoneID = recvValue.m_zoneid;
	MapMainDlgMiniMapChangeZone( recvValue.m_zoneid, recvValue.m_open )
end

-- m_count=0,m_list={m_posx=0,m_posy=0,m_nation=0,m_level=0,[m_count]},
function proc_mapzoneunitlist_C( recvValue )
	-- process.
	if WorldMapThumb.SetCityInfo( recvValue ) == false then
		for i=1, recvValue.m_count, 1 do
			MapMainDlgMiniMapAddCity( recvValue.m_list[i] )
		end
	end
end

-- m_posx=0,m_posy=0,m_nation=0,m_level=0,
function proc_mapzoneunit_C( recvValue )
	-- process.
end

-- m_unit_index=0,m_state=0,m_statetime=0,m_stateduration=0,
function proc_armyspeedupdate_C( recvValue )
	-- process.
	MapUnit.armySpeedUpdate( recvValue.m_unit_index, recvValue.m_state, recvValue.m_statetime, recvValue.m_stateduration )
end

-- m_count=0,m_list={m_army_index=0,m_unit_index=0,m_state=0,m_statetime=0,m_stateduration=0,m_action=0,m_to_posx=0,m_to_posy=0,m_herokind={[4]},[m_count]},m_unit_index=0,
function proc_battlelist_C( recvValue )
	-- process.
	MapMainDlgBattleRecv( recvValue )
	BloodyBattleDlgHeroRecv( recvValue )
end

-- m_army_index=0,m_unit_index=0,m_state=0,m_statetime=0,m_stateduration=0,m_action=0,m_to_posx=0,m_to_posy=0,m_herokind={[4]},m_to_type=0,
function proc_battleinfo_C( recvValue )
	-- process.
	MapMainDlgBattleUpdate( recvValue )
	BloodyBattleDlgHeroUpdate( recvValue )
end

-- m_kind=0,m_totalnum=0,m_totalsec=0,m_gathertime=0,m_herokind=0,m_herolevel=0,m_herohp=0,m_actorlevel=0,
function proc_mapresinfo_C( recvValue )
	-- process.
	MapResDlgRecv( recvValue )
end

-- m_game_day=0,m_game_weather=0,
function proc_weatherchange_C( recvValue )
	-- process.
	MainDlgSetWeather( recvValue.m_game_day, recvValue.m_game_weather, recvValue.m_game_day_loop )
end

-- m_mailid=0,m_type=0,m_title_len=0,m_title="[m_title_len]",m_content_len=0,m_content="[m_content_len]",m_attach_len=0,m_attach="[m_attach_len]",m_attachget=0,m_read=0,m_recvtime=0,m_fightid=0,
function proc_mail_C( recvValue )
	-- process.
	GetMail():Insert( recvValue )
end

-- m_op=0,m_mailid=0,
function proc_mailopresult_C( recvValue )
	-- process.
	if recvValue.m_op == 1 then
		MapCityDlgClose()
		MailInfoDlgByMailID( recvValue.m_mailid )
	end
end

-- m_mailid=0,m_flag=0,m_content_length=0,m_content="[m_content_length]",
function proc_mailfight_C( recvValue )
	-- process.
	if recvValue.m_flag == 0 then -- 准备发送
		MailFightContent = "";
		
	elseif recvValue.m_flag == 1 then -- 发送中
		MailFightContent = MailFightContent..recvValue.m_content;

		
	elseif recvValue.m_flag == 2 then -- 发送完毕
	    MailInfoDlgRecvFight( recvValue.m_mailid, MailFightContent )
		
	end

end

-- m_mail={m_mailid=0,m_type=0,m_title_len=0,m_title="[m_title_len]",m_content_len=0,m_content="[m_content_len]",m_attach_len=0,m_attach="[m_attach_len]",m_attachget=0,m_read=0,m_recvtime=0,m_fightid=0,m_lock=0,m_actorid=0,},
function proc_mailview_C( recvValue )
	-- process.
	local json = require "cjson";
	local m_content_json = json.decode( recvValue.m_mail.m_content );
	recvValue.m_mail.m_content_json = m_content_json;
	MailInfoDlgByRecvValue( recvValue.m_mail )
end
-- m_level=0,m_actorname_length=0,m_actorname="[m_actorname_length]",m_herokind=0,m_soldiers=0,m_actorid=0,
function proc_cityhelp_C( recvValue )
	-- process.
end

-- m_count=0,m_list={m_level=0,m_actorname_length=0,m_actorname="[m_actorname_length]",m_herokind=0,m_soldiers=0,m_actorid=0,m_army_index=0,[m_count]},
function proc_cityhelplist_C( recvValue )
	-- process.
	CityArmyHelpDlgRecv( recvValue )
end

-- m_count=0,m_list={m_level=0,m_actorname_length=0,m_actorname="[m_actorname_length]",m_herokind=0,m_soldiers=0,m_actorid=0,m_army_index=0,[m_count]},
function proc_mapcityhelplist_C( recvValue )
	-- process.
	MapCityHelpDlgRecv( recvValue )
end

-- m_state=0,m_change=0,
function proc_citystate_C( recvValue )
	-- process.
	GetPlayer().m_state = recvValue.m_state
	if Utils.byteAndOp( recvValue.m_state, CITY_STATE_FIRE ) == CITY_STATE_FIRE then
		-- 着火
		City.BuildingSetShape( {m_kind=BUILDING_Wall,m_offset=-1}, "building_kind2_fire" )
		SetTrue( City.m_Fires )
	else
		City.BuildingSetShape( {m_kind=BUILDING_Wall,m_offset=-1}, "building_kind2" )
		SetFalse( City.m_Fires )
	end
	
	if Utils.byteAndOp( recvValue.m_state, CITY_STATE_ARMYGROUP ) == CITY_STATE_ARMYGROUP then
		-- 国战告条
	end
end


-- m_group_index=0,m_group_id=0,m_attack=0,m_statetime=0,m_stateduration=0,m_nation=0,m_t_nation=0,m_level=0,m_t_level=0,m_name_length=0,m_name="[m_name_length]",m_t_name_length=0,m_t_name="[m_t_name_length]",m_posx=0,m_posy=0,m_t_posx=0,m_t_posy=0,m_actorid=0,m_t_actorid=0,m_total=0,m_t_total=0,
function proc_cityarmygroup_C( recvValue )
	-- process.
end

-- m_count=0,m_list={m_group_index=0,m_group_id=0,m_attack=0,m_statetime=0,m_stateduration=0,m_nation=0,m_t_nation=0,m_level=0,m_t_level=0,m_name_length=0,m_name="[m_name_length]",m_t_name_length=0,m_t_name="[m_t_name_length]",m_posx=0,m_posy=0,m_t_posx=0,m_t_posy=0,m_actorid=0,m_t_actorid=0,m_total=0,m_t_total=0,[m_count]},
function proc_cityarmygrouplist_C( recvValue )
	-- process.
	-- 开始
	if recvValue.m_flag == 0 then
		MapArmyGroupDlgBegin( recvValue.m_nation, recvValue.m_unit_index )
		
	-- 过程	
	elseif recvValue.m_flag == 1 then
		for i=1, recvValue.m_count, 1 do
			MapArmyGroupDlgAddRecvValue( recvValue.m_list[i] )
		end
		
	-- 结束
	else
		if recvValue.m_totalcount == 0 then
			MapArmyGroupDlgClose();
			if recvValue.m_nation == GetPlayer().m_nation then
				-- 本城没有城战
				AlertMsg( T(1263) )
			else
				MapClickModOpenCityFight( recvValue.m_unit_index );
			end
		else
			-- 添加结束
			MapArmyGroupDlgOver();
		end
	end
end

-- m_protect_sec=0,m_produce_sec=0,m_own_actorid=0,m_own_namelen=0,m_own_name="[m_own_namelen]",m_own_sec=0,m_hp=0,m_maxhp=0,,m_myask=0
function proc_maptowninfo_C( recvValue )
	-- process.
	MapTownDlgRecvValue( recvValue )
end


-- m_group_index=0,m_group_id=0,m_attack=0,m_statetime=0,m_stateduration=0,m_nation=0,m_t_nation=0,m_total=0,m_t_total=0,m_type=0,
function proc_townarmygroup_C( recvValue )
	-- process.
end

-- m_count=0,m_list={m_group_index=0,m_group_id=0,m_attack=0,m_statetime=0,m_stateduration=0,m_nation=0,m_t_nation=0,m_total=0,m_t_total=0,m_type=0,[m_count]},m_flag=0,m_nation=0,m_unit_index=0,m_totalcount=0,m_townid=0,
function proc_townarmygrouplist_C( recvValue )
	-- process.
	-- 开始
	if recvValue.m_flag == 0 then
		MapNationFightDlgBegin( recvValue.m_nation, recvValue.m_unit_index, recvValue.m_townid )
		
	-- 过程	
	elseif recvValue.m_flag == 1 then
		for i=1, recvValue.m_count, 1 do
			MapNationFightDlgAddRecvValue( recvValue.m_list[i] )
		end
		
	-- 结束
	else
		if recvValue.m_totalcount == 0 then
			MapNationFightDlgClose();
			if recvValue.m_nation == GetPlayer().m_nation then
				AlertMsg( T(1293) )
			else
				MapNationFightDlgCreate( recvValue.m_townid );
			end
		else
			-- 添加结束
			MapNationFightDlgOver();
		end
	end
end

-- m_msglen=0,m_msg="[m_msglen]",m_optime=0,
function proc_systalkjson_C( recvValue )
	-- process.
	local json = require "cjson"
	local msgjson = json.decode( recvValue.m_msg );
	local msg = "";
	if msgjson["v1"] ~= nil and msgjson["v2"] ~= nil and msgjson["v3"] ~= nil and msgjson["v4"] ~= nil and msgjson["v5"] ~= nil and msgjson["v6"] ~= nil then
		local v1_str = GetMail():GetString( msgjson["v1"] );
		local v2_str = GetMail():GetString( msgjson["v2"] );
		local v3_str = GetMail():GetString( msgjson["v3"] );
		local v4_str = GetMail():GetString( msgjson["v4"] );
		local v5_str = GetMail():GetString( msgjson["v5"] );
		local v6_str = GetMail():GetString( msgjson["v6"] );
		msg = F( msgjson["text"], v1_str, v2_str, v3_str, v4_str, v5_str, v6_str )
		
	elseif msgjson["v1"] ~= nil and msgjson["v2"] ~= nil and msgjson["v3"] ~= nil and msgjson["v4"] ~= nil and msgjson["v5"] ~= nil then
		local v1_str = GetMail():GetString( msgjson["v1"] );
		local v2_str = GetMail():GetString( msgjson["v2"] );
		local v3_str = GetMail():GetString( msgjson["v3"] );
		local v4_str = GetMail():GetString( msgjson["v4"] );
		local v5_str = GetMail():GetString( msgjson["v5"] );
		msg = F( msgjson["text"], v1_str, v2_str, v3_str, v4_str, v5_str )
		
	elseif msgjson["v1"] ~= nil and msgjson["v2"] ~= nil and msgjson["v3"] ~= nil and msgjson["v4"] ~= nil then
		local v1_str = GetMail():GetString( msgjson["v1"] );
		local v2_str = GetMail():GetString( msgjson["v2"] );
		local v3_str = GetMail():GetString( msgjson["v3"] );
		local v4_str = GetMail():GetString( msgjson["v4"] );
		msg = F( msgjson["text"], v1_str, v2_str, v3_str, v4_str )
		
	elseif msgjson["v1"] ~= nil and msgjson["v2"] ~= nil and msgjson["v3"] ~= nil then
		local v1_str = GetMail():GetString( msgjson["v1"] );
		local v2_str = GetMail():GetString( msgjson["v2"] );
		local v3_str = GetMail():GetString( msgjson["v3"] );
		msg = F( msgjson["text"], v1_str, v2_str, v3_str )
		
	elseif msgjson["v1"] ~= nil and msgjson["v2"] ~= nil then
		local v1_str = GetMail():GetString( msgjson["v1"] );
		local v2_str = GetMail():GetString( msgjson["v2"] );
		msg = F( msgjson["text"], v1_str, v2_str )
	
	elseif msgjson["v1"] ~= nil then
		local v1_str = GetMail():GetString( msgjson["v1"] );
		msg = F( msgjson["text"], v1_str )
		
	else
		msg = T( msgjson["text"] )
	end
			
	local info = {}
	info.m_actorid = -1;
	info.m_name = T(564);
	info.m_msg = msg
	info.m_optime = recvValue.m_optime;
	MainDlgSetChat( info );
	BloodyBattleDlgSetChat( info );
	ChatDlgRecv( info );
	
	-- 有滚动消息
	if recvValue.m_roll == 1 then
		RollingMsgDlgShowMsg( msg );
	end
end

-- m_msglen=0,m_msg="[m_msglen]",
function proc_rollmsgjson_C( recvValue )
	-- process.
	local json = require "cjson"
	local msgjson = json.decode( recvValue.m_msg );
	local msg = "";
	if msgjson["v1"] ~= nil and msgjson["v2"] ~= nil and msgjson["v3"] ~= nil and msgjson["v4"] ~= nil and msgjson["v5"] ~= nil and msgjson["v6"] ~= nil then
		local v1_str = GetMail():GetString( msgjson["v1"] );
		local v2_str = GetMail():GetString( msgjson["v2"] );
		local v3_str = GetMail():GetString( msgjson["v3"] );
		local v4_str = GetMail():GetString( msgjson["v4"] );
		local v5_str = GetMail():GetString( msgjson["v5"] );
		local v6_str = GetMail():GetString( msgjson["v6"] );
		msg = F( msgjson["text"], v1_str, v2_str, v3_str, v4_str, v5_str, v6_str )
		
	elseif msgjson["v1"] ~= nil and msgjson["v2"] ~= nil and msgjson["v3"] ~= nil and msgjson["v4"] ~= nil and msgjson["v5"] ~= nil then
		local v1_str = GetMail():GetString( msgjson["v1"] );
		local v2_str = GetMail():GetString( msgjson["v2"] );
		local v3_str = GetMail():GetString( msgjson["v3"] );
		local v4_str = GetMail():GetString( msgjson["v4"] );
		local v5_str = GetMail():GetString( msgjson["v5"] );
		msg = F( msgjson["text"], v1_str, v2_str, v3_str, v4_str, v5_str )
		
	elseif msgjson["v1"] ~= nil and msgjson["v2"] ~= nil and msgjson["v3"] ~= nil and msgjson["v4"] ~= nil then
		local v1_str = GetMail():GetString( msgjson["v1"] );
		local v2_str = GetMail():GetString( msgjson["v2"] );
		local v3_str = GetMail():GetString( msgjson["v3"] );
		local v4_str = GetMail():GetString( msgjson["v4"] );
		msg = F( msgjson["text"], v1_str, v2_str, v3_str, v4_str )
		
	elseif msgjson["v1"] ~= nil and msgjson["v2"] ~= nil and msgjson["v3"] ~= nil then
		local v1_str = GetMail():GetString( msgjson["v1"] );
		local v2_str = GetMail():GetString( msgjson["v2"] );
		local v3_str = GetMail():GetString( msgjson["v3"] );
		msg = F( msgjson["text"], v1_str, v2_str, v3_str )
		
	elseif msgjson["v1"] ~= nil and msgjson["v2"] ~= nil then
		local v1_str = GetMail():GetString( msgjson["v1"] );
		local v2_str = GetMail():GetString( msgjson["v2"] );
		msg = F( msgjson["text"], v1_str, v2_str )
	
	elseif msgjson["v1"] ~= nil then
		local v1_str = GetMail():GetString( msgjson["v1"] );
		msg = F( msgjson["text"], v1_str )
		
	else
		msg = T( msgjson["text"] )
	end
	RollingMsgDlgShowMsg( msg );
end

-- m_msglen=0,m_msg="[m_msglen]",
function proc_rollmsg_C( recvValue )
	-- process.
	RollingMsgDlgShowMsg( recvValue.m_msg );
end

-- m_count=0,m_list={m_name_len=0,m_name="[m_name_len]",m_place=0,[m_count]},m_sec=0,
function proc_townowneraskList_C( recvValue )
	-- process.
	TownRebuildDlgAskList( recvValue )
end

-- m_count=0,m_list={m_townid=0,m_statetime=0,m_attack=0,m_nation,[m_count]},
function proc_townfightlist_C( recvValue )
	-- process.
	MapMainDlgNationLayerRecv( recvValue )
end

-- m_ui=0,
function proc_dialogupdate_C( recvValue )
	-- process.
	dialogupdate_recv( recvValue.m_ui )
end

-- m_dev_level=0,m_dev_exp=0,m_dev_expmax=0,m_mytownid=0,m_dev_cd=0,m_attackcd=0
function proc_maptownexinfo_C( recvValue )
	-- process.
	MapTownExDlgRecvValue( recvValue )
end

-- m_townid=0,m_nation=0,m_protect_sec=0,m_from_nation="[4]",
function proc_mapzonetown_C( recvValue )
	-- process.
end

-- m_count=0,m_list={m_townid=0,m_nation=0,m_protect_sec=0,m_from_nation="[4]",[m_count]},m_zoneid=0,
function proc_mapzonetownlist_C( recvValue )
	-- process.
	if WorldMapThumb.SetTownInfo( recvValue ) == false then
		MapMainDlgMiniMapSetTown( recvValue )
	end
end

-- m_townid=0,m_nation=0,
function proc_mapcentertown_C( recvValue )
	-- process.
	MapUnit.createCenterTownRange( recvValue )
end

-- m_count=0,m_list={m_townid=0,m_nation=0,[m_count]},
function proc_mapcentertownlist_C( recvValue )
	-- process.
	for i=1, recvValue.m_count, 1 do
		MapUnit.createCenterTownRange( recvValue.m_list[i] )
	end
end


-- m_group_index=0,m_group_id=0,m_from_nation=0,m_from_posx=0,m_from_posy=0,m_namelen=0,m_name="[m_namelen]",m_statetime=0,m_stateduration=0,
function proc_citywarinfo_C( recvValue )
	-- process.
	MainDlgAddWar( recvValue )
end

-- m_count=0,m_list={m_group_index=0,m_group_id=0,m_from_nation=0,m_from_posx=0,m_from_posy=0,m_namelen=0,m_name="[m_namelen]",m_statetime=0,m_stateduration=0,[m_count]},
function proc_citywarlist_C( recvValue )
	-- process.
	MainDlgSetWarCache( recvValue )
end

-- m_group_index=0,
function proc_citywardel_C( recvValue )
	-- process.
	MainDlgAddDel( recvValue )
end

-- m_questid=0,m_value=0,m_maxvalue=0,m_complete=0,
function proc_worldquest_C( recvValue )
	-- process.
	MapMainDlgSetWorldQuest( recvValue )
end

-- m_bossid=0,m_hp=0,m_maxhp=0,m_isfight=0,m_actorlevel
function proc_worldboss_C( recvValue )
	-- process.
	WorldBossDlgRecvValue( recvValue )
end

-- m_bossid=0,m_herokind={[4]},
function proc_worldbossbattle_C( recvValue )
	-- process.
end

-- m_rb_silver=0,m_rb_wood=0,m_rb_food=0,m_rb_df=0,
function proc_lostrebuild_C( recvValue )
	-- process.
	LostRebuildDlgShow( recvValue )
end

-- m_id=0,m_attack_total=0,m_defense_total=0,m_nation=0,
function proc_kingwartown_C( recvValue )
	-- process.
	BloodyBattleDlgTownRecv( recvValue )
end

-- m_count=0,m_list={m_id=0,m_attack_total=0,m_defense_total=0,m_nation=0,[m_count]},m_leftstamp=0,m_losthp=0
function proc_kingwartownlist_C( recvValue )
	-- process.
	for i=1, recvValue.m_count, 1 do
		BloodyBattleDlgTownRecv( recvValue.m_list[i] )
	end
	BloodyBattleDlgRecvLeftStamp( recvValue.m_leftstamp )
	BloodyBattleDlgRecvLostHP( recvValue.m_losthp )
end


-- m_count=0,m_list={m_id=0,m_beginstamp=0,m_endstamp=0,m_value=0,m_state=0,[m_count]},
function proc_mapactivitylist_C( recvValue )
	-- process.
end

-- m_state=0,m_beginstamp=0,m_endstamp=0,m_nation=0,
function proc_kingwaractivity_C( recvValue )
	-- process.
	MapMainDlgActivityKingWar( recvValue )
end

-- m_a_heroid=0,m_a_color=0,m_a_name_len=0,m_a_name="[m_a_name_len]",m_a_losthp=0,m_a_nation=0,m_d_heroid=0,m_d_color=0,m_d_name_len=0,m_d_name="[m_d_name_len]",m_d_losthp=0,m_d_nation=0,m_result=0,m_id=0,
function proc_kingwarnotify_C( recvValue )
	-- process.
	BloodyBattleDlgSetFightLog( recvValue )
end

-- m_count=0,m_list={m_a_heroid=0,m_a_color=0,m_a_name_len=0,m_a_name="[m_a_name_len]",m_a_losthp=0,m_a_nation=0,m_d_heroid=0,m_d_color=0,m_d_name_len=0,m_d_name="[m_d_name_len]",m_d_losthp=0,m_d_nation=0,m_result=0,m_id=0,[m_count]},
function proc_kingwarnotifylist_C( recvValue )
	-- process.
	for i = 1, recvValue.m_count, 1 do
		BloodyBattleDlgSetPKLog( recvValue.m_list[i] );
	end
end

-- m_count=0,m_list={m_rank=0,m_kill=0,m_nation=0,m_name_len=0,m_name="[m_name_len]",m_herokind=0,m_herocolor=0,[m_count]},m_mypoint=0,m_totalkill=0,m_myrank=0
function proc_kingwarranklist_C( recvValue )
	-- process.
	KingWarRankDlgRecvValue( recvValue )
end

-- m_herokind=0,m_hp=0,m_id=0,
function proc_kingwarpk_C( recvValue )
	-- process.
	BloodyBattleDlgShowPKLayer( recvValue )
end

-- m_point=0,
function proc_kingwarpoint_C( recvValue )
	-- process.
	KingWarChangeDlgRecv( recvValue )
end

-- m_state=0,m_endstamp=0,m_nation=0,m_treasure_num=0,m_treasure_num_max=0,
function proc_treasureactivity_C( recvValue )
	-- process.
	MapMainDlgActivityTreasure( recvValue )
end

-- m_has=0,m_px=0,m_py=0,m_tn=0,
function proc_treasurehas_C( recvValue )
	-- process.
	TreasureDlgRecv( recvValue )
end

-- m_posx=0,m_posy=0,m_type=0,m_unit_index=0,m_op=0,
function proc_gotoasyn_C( recvValue )
	-- process.
	WorldMap.GotoUnitAsyn( recvValue )
end

-- m_count=0,m_list={m_itemkind=0,m_name_len=0,m_name="[m_name_len]",[m_count]},m_co=0,
function proc_treasureactorlist_C( recvValue )
	-- process.
	TreasureAwardDlgRecv( recvValue )
end

-- m_autobuild=0,m_autobuildopen=0,m_path
function proc_changeautobuild_C( recvValue )
	-- process.
	GetPlayer().m_autobuild		=	recvValue.m_autobuild;
	GetPlayer().m_autobuildopen	=	recvValue.m_autobuildopen;
	MainDlgSetAutoBuild( recvValue.m_path )
end

-- m_hv_free_cd=0,m_hv_high_sec=0,m_hv_high_free=0,m_hv_low_num=0,m_hv_high_num=0,m_hv_progress=0,
function proc_herovisitinfo_C( recvValue )
	-- process.
	HeroVisitDlgRecv( recvValue )
end

-- m_id={[10]},m_count=0,
function proc_herovisitaward_C( recvValue )
	-- process.
	HeroVisitDlgRecvAward( recvValue )
end

-- m_flag=0,m_content_length=0,m_content="[m_content_length]",
function proc_fightplay_C( recvValue )
	-- process.
	if recvValue.m_flag == 0 then -- 准备发送
		if recvValue.m_content_length > 0 then
			g_fight.infoJson = json.decode( recvValue.m_content )
		else
			g_fight.infoJson = ""
		end
		g_fight.contentJson = "";
		
	elseif recvValue.m_flag == 1 then -- 发送中
		g_fight.contentJson = g_fight.contentJson..recvValue.m_content;

	elseif recvValue.m_flag == 2 then -- 发送完毕
	
		local info = {}
		info.m_fight_content = g_fight.contentJson;
		info.m_content_json = g_fight.infoJson
		FightDlgShow( info )
		BattleDlgClose()
		StoryDlgClose()
	end
end

-- m_exp=0,m_silver=0,m_awardcount=0,m_award={m_kind=0,m_num=0,[m_awardcount]},m_herocount=0,m_hero={m_kind=0,m_color=0,m_level=0,m_pre_exp=0,m_exp=0,[m_herocount]},
function proc_storysweepresult_C( recvValue )
	-- process.
	FightDlgSweepResult( recvValue )
end

-- m_talkid=0,m_herokind=0,m_talk_textid=0,m_btn_textid=0,
function proc_questtalk_C( recvValue )
	-- process.
	if recvValue.m_talkid == -1 then
		ChangeNameDlgShow();
		return
	end
	local callback = function()
		local sendValue = {};
		sendValue.m_talkid = recvValue.m_talkid;
		sendValue.m_type = 0;
		netsend_questtalknext_C( sendValue )
	end
	
	if recvValue.m_herokind > 0 then
		if BuildingGetDlgIsShow() == true then
			BuildingGetDlgWait( HeroTalk, recvValue.m_herokind, Localization.text_quest(recvValue.m_talk_textid), callback )
		else
			HeroTalk( recvValue.m_herokind, Localization.text_quest(recvValue.m_talk_textid), callback )
		end
	else
		if recvValue.m_btn_textid > 0 then
			if BuildingGetDlgIsShow() == true then
				BuildingGetDlgWait( NpcTalkOne, Localization.text_quest(recvValue.m_talk_textid), Localization.text_quest(recvValue.m_btn_textid), callback )
			else
				NpcTalkOne( Localization.text_quest(recvValue.m_talk_textid), Localization.text_quest(recvValue.m_btn_textid), callback )
			end
		else
			if BuildingGetDlgIsShow() == true then
				BuildingGetDlgWait( NpcTalk, Localization.text_quest(recvValue.m_talk_textid), callback )
			else
				NpcTalk( Localization.text_quest(recvValue.m_talk_textid), callback )
			end
		end
	end
end

-- m_count=0,m_list={m_rank=0,m_namelen=0,m_name="[m_namelen]",m_nation=0,m_level=0,m_place=0,m_battlepower=0,m_actorid=0,[m_count]},m_myrank=0,m_type=0,m_page=0,m_myrank=0,
function proc_ranklist_C( recvValue )
	-- process.
	RankListDlgRecv( recvValue )
end

-- m_count=0,m_list={m_actorid=0,m_city_index=0,m_shape=0,m_namelen=0,m_name="[m_namelen]",m_level=0,m_place=0,m_battlepower=0,m_ask=0,[m_count]},
function proc_friendlist_C( recvValue )
	-- process.
	if recvValue.m_op == 1 then
		FriendDlgFriendBeginRecv()
	elseif recvValue.m_op == 2 then
		FriendDlgFriendRecv( recvValue )
	elseif recvValue.m_op == 3 then
		FriendDlgFriendEndRecv()
	end
end

-- m_actorid=0,m_city_index=0,m_namelen=0,m_name="[m_namelen]",m_signlen=0,m_sign="[m_signlen]",m_level=0,m_official=0,m_place=0,m_zone=0,m_nation=0,m_battlepower=0,m_killcity=0,m_herocount=0,m_hero={m_kind=0,m_color=0,m_level=0,m_pre_exp=0,m_exp=0,[m_herocount]},
function proc_actorsearch_C( recvValue )
	-- process.
	ActorSearchDlgRecv( recvValue )
end

-- m_actorid=0,m_namelen=0,m_name="[m_namelen]",m_level=0,m_nation=0,
function proc_blackinfo_C( recvValue )
	-- process.
	GetPlayer():AddBlacklist( recvValue.m_actorid )
	BlackListDlgAdd( recvValue )
end

-- m_count=0,m_list={m_actorid=0,m_namelen=0,m_name="[m_namelen]",m_level=0,m_nation=0,[m_count]},
function proc_blacklist_C( recvValue )
	-- process.
	if recvValue.m_op == 1 then
		BlackListDlgBegin()
	elseif recvValue.m_op == 2 then
		BlackListDlgRecv( recvValue )
	elseif recvValue.m_op == 3 then
		BlackListDlgEndRecv()
	end
end

-- m_count=0,m_actorid={}[m_count],
function proc_blacklistid_C( recvValue )
	-- process.
	GetPlayer():SetBlacklist( recvValue )
end



-- m_type=0,m_count=0,m_list={m_offset=0,m_awardkind=0,m_awardnum=0,m_token=0,m_sale=0,[m_count]},
function proc_shoplist_C( recvValue )
	-- process.
	ShopDlgRecvShop( recvValue )
end

-- m_count=0,m_list={m_goodsid=0,m_price=0,m_token=0,m_day=0,m_nameid=0,m_icon=0,[m_count]},
function proc_paystore_C( recvValue )
	-- process.
	PayDlgRecv( recvValue )
end

-- m_goodsid=0,m_price=0,m_nameid=0,m_descid=0,m_icon=0,m_sale=0,m_worth=0,m_bag_time=0,m_bag_num=0,m_awardcount=0,m_award={m_kind=0,m_num=0,[m_awardcount]},
function proc_goodsinfo_C( recvValue )
	-- process.
end

-- m_count=0,m_list={m_goodsid=0,m_price=0,m_nameid=0,m_descid=0,m_icon=0,m_sale=0,m_worth=0,m_bag_time=0,m_bag_num=0,m_awardcount=0,m_award={m_kind=0,m_num=0,[m_awardcount]},[m_count]},
function proc_paystoreactivity_C( recvValue )
	-- process.
	if recvValue.m_path == 0 then
		ShopDlgPayBagRecv( recvValue )
	elseif recvValue.m_path == 1 then
		Activity10ModRecv( recvValue )
	end
end

-- m_orderid_len=0,m_orderid="[m_orderid_len]",m_ext_len=0,m_ext="[m_ext_len]",m_goodsid=0,m_productid=0,m_nameid=0,m_descid=0,m_price=0,
function proc_payorder_C( recvValue )
	-- process.
	SDK.pay( recvValue )
end

-- m_count=0,m_list={m_awardkind=0,m_awardnum=0,m_token=0,m_itemkind=0,m_vip_token=0,m_vip_buynum=0,m_vip_buynum_max=0,[m_count]},m_useitem=0,
function proc_vipshop_C( recvValue )
	-- process.
	ShopDlgRecvVipShop( recvValue )
end

-- m_vipbag=0,,m_m_vipbag_count=0
function proc_vipbag_C( recvValue )
	-- process.
	ShopDlgRecvVipBag( recvValue )
end

-- m_buffkind=0,m_path=0,m_endtime=0,
function proc_buffchange_C( recvValue )
	-- process.
	GetPlayer().m_buff_endtime[recvValue.m_buffkind] = recvValue.m_endtime;
	GetPlayer().m_bufftrain = recvValue.m_bufftrain;
	if recvValue.m_endtime > 0 then
		pop( T(2400 + recvValue.m_buffkind) );
	end
	HeroDlgSetBuff()
end

-- m_autoguard=0,m_autoguardopen=0,m_path=0,
function proc_changeautoguard_C( recvValue )
	-- process.
	GetPlayer().m_autoguard		=	recvValue.m_autoguard;
	GetPlayer().m_autoguardopen	=	recvValue.m_autoguardopen;
	MainDlgSetAutoGuard( recvValue.m_path )
end

-- m_count=0,m_list={m_activityid=0,m_starttime=0,m_endtime=0,m_closetime=0,[m_count]},
function proc_activitylist_C( recvValue )
	-- process.
	ActivityDlgRecv( recvValue )
end

-- m_neq_lv=0,m_neq_exp=0,m_neq_pro=0,m_neq_sec=0,
function proc_nationequip_C( recvValue )
	-- process.
	NationEquipDlgRecv( recvValue )
end

-- m_count=0,m_list={m_neq_lv=0,m_neq_exp=0,m_neq_pro=0,m_neq_sec=0,[m_count]},
function proc_nationequiplist_C( recvValue )
	-- process.
	NationEquipDlgListRecv( recvValue )
end

-- m_level=0,m_exp=0,m_donate_num=0,m_myrank=0,m_notice_len=0,m_notice="[m_notice_len]",m_kingname_len=0,m_kingname="[m_kingname_len]",
function proc_nationinfo_C( recvValue )
	-- process.
	NationDlgRecv( recvValue )
end

-- m_level=0,m_exp=0,m_donate_num=0,m_myrank=0,
function proc_nationbase_C( recvValue )
	-- process.
	NationDlgRecvBase( recvValue )
	NationUpgradeDlgShowRecv( recvValue )
end

-- m_info={m_protect_sec=0,m_produce_sec=0,m_own_actorid=0,m_own_namelen=0,m_own_name="[m_own_namelen]",m_own_sec=0,m_hp=0,m_maxhp=0,m_myask=0,m_produce_num=0,m_townid=0,},
function proc_nationtown_C( recvValue )
	-- process.
	if recvValue.m_path == 1 then
		TownRebuildDlgShow( recvValue.m_info.m_townid, recvValue.m_info )
	elseif recvValue.m_path == 2 then
		NationTownDlgUpdate( recvValue.m_info )
	else
		NationTownDlgUpdate( recvValue.m_info )
	end
end

-- m_count=0,m_list={m_info={m_protect_sec=0,m_produce_sec=0,m_own_actorid=0,m_own_namelen=0,m_own_name="[m_own_namelen]",m_own_sec=0,m_hp=0,m_maxhp=0,m_myask=0,m_produce_num=0,m_townid=0,},[m_count]},m_op=0,
function proc_nationtownlist_C( recvValue )
	-- process.
	if recvValue.m_op == 1 then
		NationTownDlgBegin()
	elseif recvValue.m_op == 2 then
		NationTownDlgRecv( recvValue )
	elseif recvValue.m_op == 3 then
		NationTownDlgEndRecv()
	end
end

-- m_count=0,m_list={m_group_index=0,m_group_id=0,m_attack=0,m_statetime=0,m_stateduration=0,m_nation=0,m_t_nation=0,m_total=0,m_t_total=0,m_type=0,m_townid=0,m_unit_index=0,[m_count]},m_op=0,
function proc_nationwarlist_C( recvValue )
	-- process.
	if recvValue.m_op == 1 then
		NationWarDlgNationRecvBegin()
	elseif recvValue.m_op == 2 then
		NationWarDlgNationRecv( recvValue )
	elseif recvValue.m_op == 3 then
		NationWarDlgNationRecvEnd()
	end
end

-- m_count=0,m_list={m_group_index=0,m_group_id=0,m_attack=0,m_statetime=0,m_stateduration=0,m_nation=0,m_t_nation=0,m_level=0,m_t_level=0,m_name_length=0,m_name="[m_name_length]",m_t_name_length=0,m_t_name="[m_t_name_length]",m_posx=0,m_posy=0,m_t_posx=0,m_t_posy=0,m_actorid=0,m_t_actorid=0,m_total=0,m_t_total=0,m_type=0,m_unit_index=0,[m_count]},m_op=0,
function proc_nationcitywarlist_C( recvValue )
	-- process.
	if recvValue.m_op == 1 then
		NationWarDlgCityRecvBegin()
	elseif recvValue.m_op == 2 then
		NationWarDlgCityRecv( recvValue )
	elseif recvValue.m_op == 3 then
		NationWarDlgCityRecvEnd()
	end
end

-- m_count=0,m_list={m_level=0,m_kind=0,m_value=0,m_needvalue=0,m_awardkind={[5]},m_awardnum={[5]},m_other_awardkind=0,m_other_awardnum=0,[m_count]},
function proc_nationquestlist_C( recvValue )
	-- process.
	NationQuestDlgRecv( recvValue )
end

-- m_count=0,m_list={m_value={[3]},m_needvalue={[3]},m_awardkind={[2]},m_awardnum={[2]},m_isget=0,[m_count]},
function proc_nationmissionlist_C( recvValue )
	-- process.
	NationHonorDlgMissionRecv( recvValue )
end

-- m_buildrank={m_name_len=0,m_name="[m_name_len]",m_value=0,[5]},m_cityrank={m_name_len=0,m_name="[m_name_len]",m_value=0,[5]},m_townrank={m_name_len=0,m_name="[m_name_len]",m_value=0,[5]},
function proc_nationranklist_C( recvValue )
	-- process.
	NationHonorDlgRankRecv( recvValue )
end


-- m_count=0,m_list={m_official=0,m_namelen=0,m_name="[m_namelen]",m_level=0,m_battlepower=0,m_zone=0,[m_count]},m_endtime=0,
function proc_nationofficiallist_C( recvValue )
	-- process.
	NationOfficialDlgOfficialRecv( recvValue )
end

-- m_count=0,m_list={m_namelen=0,m_name="[m_namelen]",m_level=0,m_battlepower=0,m_ballot=0,m_actorid=0,[m_count]},m_endtime=0,m_myvote=0,m_isballot=0,m_tokenballot=0,
function proc_nationcandidatelist_C( recvValue )
	-- process.
	NationOfficialDlgCandidateRecv( recvValue )
end

-- m_open_town6=0,m_open_townking=0,
function proc_worlddataopen_C( recvValue )
	-- process.
	GetPlayer().m_open_town3 = recvValue.m_open_town3
	GetPlayer().m_open_town6 = recvValue.m_open_town6
	GetPlayer().m_open_townking = recvValue.m_open_townking
end

-- m_count=0,m_list={m_namelen=0,m_name="[m_namelen]",m_level=0,m_battlepower=0,m_place=0,m_official=0,m_actorid=0,[m_count]},m_op=0,
function proc_nationreplacelist_C( recvValue )
	-- process.
	if recvValue.m_op == 1 then
		NationOfficialReplaceDlgRecvBegin()
	elseif recvValue.m_op == 2 then
		NationOfficialReplaceDlgRecv( recvValue )
	elseif recvValue.m_op == 3 then
		NationOfficialReplaceDlgRecvEnd()
	end
end

-- m_count=0,m_list={m_type=0,m_townid=0,m_namelen=0,m_name="[m_namelen]",m_target_nation=0,m_optime=0,[m_count]},
function proc_nationloglist_C( recvValue )
	-- process.
	NationLogDlgRecv( recvValue )
end


-- m_count=0,m_list={m_kind=0,m_actorid=0,m_namelen=0,m_name="[m_namelen]",m_level=0,m_posx=0,m_posy=0,m_buypos=0,m_state=0,m_forever=0,m_nation=0,[m_count]},m_nationlevel=0,
function proc_nationherolist_C( recvValue )
	-- process.
	NationHeroDlgRecv( recvValue )
end

-- m_state=0,m_kind=0,m_forever=0,m_loyal=0,m_runstamp=0,
function proc_citynationhero_C( recvValue )
	-- process.
	local offset = g_nation_heroinfo[recvValue.m_kind].offset;
	if offset >= 0 and offset < NATIONHERO_MAX then
		GetHero().m_NationHero[offset]:Set( recvValue )
	end
	HeroListDlgCreateNationHeroVisit()
	HeroListDlgLoadHero();
	HeroInfoDlgUpdate( recvValue.m_kind )
end

-- m_count=0,m_list={m_state=0,m_kind=0,m_forever=0,m_loyal=0,m_runstamp=0,[m_count]},
function proc_citynationherolist_C( recvValue )
	-- process.
	for i=1,recvValue.m_count,1 do
		local kind = recvValue.m_list[i].m_kind
		local offset = g_nation_heroinfo[kind].offset;
		if offset >= 0 and offset < NATIONHERO_MAX then
			GetHero().m_NationHero[offset]:Set( recvValue.m_list[i] )
		end
	end
	HeroListDlgCreateNationHeroVisit()
	HeroListDlgLoadHero();
end

-- m_attr={m_kind=0,m_color=0,m_level=0,m_corps=0,m_exp=0,m_exp_max=0,m_soldiers=0,m_state=0,m_attack_base=0,m_attack_wash=0,m_defense_base=0,m_defense_wash=0,m_troops_base=0,m_troops_wash=0,m_attack=0,m_defense=0,m_troops=0,m_attack_increase=0,m_defense_increase=0,m_offset=0,m_god=0,},m_namelen=0,m_name="[m_namelen]",m_actorid=0,m_open=0,
function proc_nationheroattr_C( recvValue )
	-- process.
	HeroConfigDlgRecv( recvValue )
end

-- m_count=0,m_list={m_id=0,m_color=0,m_awardkind=0,m_awardnum=0,m_costkind=0,m_costnum=0,m_open=0,[m_count]},m_openstamp=0,
function proc_wishingshop_C( recvValue )
	-- process.
	WishingDlgWishingRecv( recvValue )
end

-- m_silver=0,m_wood=0,m_silver_to_wood=0,m_wood_to_silver=0,m_silver_to_food=0,m_wood_to_food=0,m_food=0,m_food_to_silver=0,m_food_to_wood=0,m_cd=0
function proc_wishingchange_C( recvValue )
	-- process.
	WishingDlgChangeRecv( recvValue )
end

-- m_count=0,m_list={m_awardkind=0,m_awardnum=0,m_costkind=0,m_costnum=0,m_id=0,m_token=0,[m_count]},
function proc_wishingpack_C( recvValue )
	-- process.
	WishingDlgPackRecv( recvValue )
end

-- m_tc_state=0,m_tc_kind=0,m_tc_num=0,m_tc_tech=0,m_nation_tiance_level=0,m_nation_tiance_point=0,
function proc_tiancequest_C( recvValue )
	-- process.
	HeroLevyDlgRecv( recvValue )
end

-- m_count=0,m_list={m_actorid=0,m_city_index=0,m_shape=0,m_namelen=0,m_name="[m_namelen]",m_level=0,m_place=0,m_battlepower=0,m_isteacher=0,[m_count]},m_te_award={[10]},m_te_awarded={[10]},m_teacheraward_count=0,m_teacheraward={m_kind=0,m_num=0,[m_teacheraward_count]},
function proc_studentlist_C( recvValue )
	-- process.
end

-- m_count=0,m_list={m_awardkind=0,m_buylevel=0,m_point=0,m_id=0,[m_count]},m_mypoint=0,
function proc_teachershoplist_C( recvValue )
	-- process.
end

