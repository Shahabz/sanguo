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
	
	-- 临时调整背景层级
	GameObject.FindWithTag( "UpdateManager" ).Find("Canvas"):GetComponent( "Canvas" ).sortingOrder = 1000
	
	-- 加载主城
	LoadPrefabAsyn( "MainCity", function( obj )
		
		-- 缓存主城
		GameManager.MainCity = GameObject.Instantiate( obj );
		City.Init();
		
		-- 获取进入游戏所需信息
		local sendValue = {};
		sendValue.m_value = 0;
		netsend_entercity_C( sendValue );
	end );
end

-- m_actorid=0,m_name="[22]",m_nation=0,m_shape=0,m_level=0,m_exp=0,m_exp_max=0,m_token=0,m_viplevel=0,m_vipexp=0,m_vipexp_max=0,m_body=0,m_place=0,m_official=0,m_zone=0,m_battlepower=0,m_silver=0,m_wood=0,m_food=0,m_iron=0,m_infantry_num=0,m_cavalry_num=0,m_archer_num=0,m_mokilllv=0,m_sflag=0,m_cityid=0,
function proc_actorinfo_C( recvValue )
	-- process.
	-- 打开主界面
	MainDlgOpen();
	
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
		proc_buildingres_C( recvValue.m_res[i] )
	end
		
	GetPlayer().m_function = recvValue.m_function;
	-- 铁匠铺
	if Utils.get_int_sflag( recvValue.m_function, CITY_FUNCTION_SMITHY ) == 1 then
		GetPlayer():SetBuilding( BUILDING_Smithy,{ m_kind = BUILDING_Smithy, m_offset = 0,m_level = 0,m_sec = 0,m_needsec = 0,m_quick = 0, m_overvalue=0 } )
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
	MainDlgSetButtons();
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
		m_value = recvValue.m_value,
		m_overvalue = recvValue.m_overvalue,
		} )
	
	if recvValue.m_kind == BUILDING_Tech then
		CityTechDlgOnSet();
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
				--tmpItem.m_color_level = itemconfig.color_level;
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
	MainDlgSetExp();
	if recvValue.m_isup == 1 then
		pop( T(150) );
		MainDlgSetLevel();
	end
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
	LevyDlgChangeSec( recvValue.m_sec )
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
	pop( T(120)..": "..T(152).."x"..recvValue.m_add );
	GetPlayer().m_prestige = recvValue.m_total;
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

-- m_offset=0,m_kind=0,m_washid={[4]},
function proc_equip_C( recvValue )
	-- process.
end

-- m_equipext=0,m_count=0,m_list={m_offset=0,m_kind=0,m_washid={[6]},[m_count]},
function proc_equiplist_C( recvValue )
	-- process.
	GetPlayer().m_equipext = recvValue.m_equipext;
	for tmpi = 1, recvValue.m_count, 1 do
		if recvValue.m_list[tmpi].m_offset >= 0 then
			local tmpEquip = SLK_Equip.new();
			tmpEquip.m_kind = recvValue.m_list[tmpi].m_kind;
			for i=1,6,1 do
				tmpEquip.m_washid[i] = recvValue.m_list[tmpi].m_washid[i];
			end
	
			GetEquip():SetEquip( recvValue.m_list[tmpi].m_offset, tmpEquip );
		end
	end
end

-- m_offset=0,m_kind=0,m_path=0,
function proc_equipget_C( recvValue )
	-- process.
	GetEquip():EquipGet( recvValue.m_offset, recvValue.m_kind, recvValue.path )
end

-- m_offset=0,m_kind=0,m_path=0,
function proc_equiplost_C( recvValue )
	-- process.
	GetEquip():EquipLost( recvValue.m_offset, recvValue.m_path );
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
	HeroGetDlgShow( recvValue.m_hero );
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
	elseif recvValue.m_corps == 1 then
		GetPlayer().m_cavalry_num = recvValue.m_soldiers;
		MainDlgSetCavalry();
	elseif recvValue.m_corps == 2 then
		GetPlayer().m_archer_num = recvValue.m_soldiers;
		MainDlgSetArcher();
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
	QuestAwardDlgShow( recvValue )
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
		
	else
		MainDlgSetButtons();
		
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
	CityGuardDlgRecv( recvValue );
end

-- m_gusrdsec=0,
function proc_cityguardsec_C( recvValue )
	-- process.
	CityGuardDlgChangeSec( recvValue );
end

-- m_forgingkind=0,m_forgingsec=0,m_forgingsec_need=0,
function proc_buildingsmithy_C( recvValue )
	-- process.
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
		City.BuildingSetUpgradeing( recvValue.m_kind, recvValue.m_offset, 0, 0 )
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
	ChatDlgRecv( recvValue );
	MainDlgSetChat( recvValue );
	ChatDlgScrollToBottom();
end

-- m_count=0,m_list={m_actorid=0,m_shape=0,m_level=0,m_namelen=0,m_name="[m_namelen]",m_frame=0,m_zone=0,m_place=0,m_msglen=0,m_msg="[m_msglen]",m_optime=0,m_channel=0,m_nation=0,[m_count]},m_channel=0,
function proc_chatlist_C( recvValue )
	-- process.
	for i=1, recvValue.m_count, 1 do
		ChatDlgRecv( recvValue.m_list[i] );
		if i == recvValue.m_count then
			MainDlgSetChat( recvValue.m_list[i] );
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
	ChatDlgRecv( info );
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

