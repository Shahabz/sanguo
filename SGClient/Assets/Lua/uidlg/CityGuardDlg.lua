-- 界面
local m_Dlg = nil;
local m_DialogFrameMod = nil;
local m_uiGuardNum = nil; --UnityEngine.GameObject
local m_uiUIP_Guard = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiTimerText = nil; --UnityEngine.GameObject
local m_uiDescText = nil; --UnityEngine.GameObject
local m_uiCDButton = nil; --UnityEngine.GameObject
local m_uiCallButton = nil; --UnityEngine.GameObject
local m_level = 1;
local m_nextlevel = 1;
local m_ObjectPool = nil;
local m_dataCache = {};
local m_uiCache = {};
local m_num = 0;
local m_guardup_toggle = false;
-- 打开界面
function CityGuardDlgOpen()
	m_Dlg = eye.uiManager:Open( "CityGuardDlg" );
	m_DialogFrameMod = DialogFrameModOpen( m_Dlg, BuildingName(BUILDING_Wall).." Lv."..m_level, 4, CityGuardDlgClose );
end

-- 隐藏界面
function CityGuardDlgClose()
	if m_Dlg == nil then
		return;
	end
	local objs = {};
	for i = 0 ,m_uiContent.transform.childCount - 1 do
		table.insert( objs, m_uiContent.transform:GetChild(i).gameObject )
    end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "UIP_Guard(Clone)" then
			m_ObjectPool:Release( "UIP_Guard", obj );
		end
    end
	m_dataCache = {};
	m_uiCache = {};
	DialogFrameModClose( m_DialogFrameMod );
	m_DialogFrameMod = nil;
	eye.uiManager:Close( "CityGuardDlg" );
end

-- 删除界面
function CityGuardDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function CityGuardDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            CityGuardDlgClose();
		
		-- 招募按钮
		elseif nControlID == 101 then
			system_askinfo( ASKINFO_CITYGUARD, "", 1 );
			
		-- 清除CD
		elseif nControlID == 102 then
			CityGuardDlgClearCD()
			
		-- 武将防守顺序
		elseif nControlID == 103 then
			
			
		-- 友军驻防武将
		elseif nControlID == 104 then
			if m_level < global.cityhelp_level then
				pop( F(1247, global.cityhelp_level ) )
			else
				CityArmyHelpDlgShow()
			end
			
		-- 选择守卫
		elseif nControlID >= 1000 and nControlID < 1100 then
			CityGuardDlgSelect( nControlID-1000 )
        end
	elseif nType == UI_EVENT_TIMECOUNTEND then
		if nControlID == 1 then
			CityGuardDlgChangeSec( {m_guardsec=0} )
		end
	end
end

-- 载入时调用
function CityGuardDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;
	m_uiGuardNum = objs[0];
	m_uiUIP_Guard = objs[1];
	m_uiContent = objs[2];
	m_uiTimerText = objs[3];
	m_uiDescText = objs[4];
	m_uiCDButton = objs[5];
	m_uiCallButton = objs[6];
	-- 对象池
	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );
	m_ObjectPool:CreatePool("UIP_Guard", m_level, m_level, m_uiUIP_Guard);
end

-- 界面初始化时调用
function CityGuardDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function CityGuardDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function CityGuardDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function CityGuardDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function CityGuardDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function CityGuardDlgShow()
	-- 城墙
	m_pBuilding = GetPlayer():GetBuilding( BUILDING_Wall, -1 );
	if m_pBuilding == nil then
		return;
	end 
	m_level = m_pBuilding.m_level;
	CityGuardDlgOpen();
	CityGuardDlgCreateEmpty();
	system_askinfo( ASKINFO_CITYGUARD, "", 0 );
end

-- m_count=0,m_list={m_corps=0,m_color=0,m_shape=0,m_level=0,m_soldiers=0,m_troops=0,m_offset=0,[m_count]},m_guardsec=0,
function CityGuardDlgRecv( recvValue )
	m_dataCache = {};
	for i=1, recvValue.m_count, 1 do
		CityGuardDlgSet( recvValue.m_list[i] );
	end
	
	-- 城防军数量
	m_num = recvValue.m_count;
	if m_level < global.city_guard_level then
		SetText( m_uiGuardNum, "" );
	else
		SetText( m_uiGuardNum, T(652).." "..m_num.."/"..m_level );
	end
	
	-- 冷却时间
	CityGuardDlgChangeSec( recvValue )
end

function CityGuardDlgCreateEmpty()
	
	if m_level < global.city_guard_level then
		-- 创建一个未解锁的
		local uiObj = m_ObjectPool:Get( "UIP_Guard" );
		uiObj.transform:SetParent( m_uiContent.transform );
		m_nextlevel = global.city_guard_level;
		CityGuardDlgSetObj( uiObj, nil, nil, nil, nil, nil, nil, false, true )
	else
		
		-- 已解锁的
		for i=0, m_level-1, 1 do
			local uiObj = m_ObjectPool:Get( "UIP_Guard" );
			uiObj.transform:SetParent( m_uiContent.transform );
			CityGuardDlgSetObj( uiObj, nil, nil, nil, nil, nil, nil, false, false )
			SetControlID( uiObj, 1000+i )
			m_uiCache[i] = uiObj;
		end
	
		-- 在创建一个未解锁的
		local uiObj = m_ObjectPool:Get( "UIP_Guard" );
		uiObj.transform:SetParent( m_uiContent.transform );
		m_nextlevel = m_level + 1;
		CityGuardDlgSetObj( uiObj, nil, nil, nil, nil, nil, nil, false, true )
		
	end
	
end

function CityGuardDlgSet( recvValue )
	local offset = recvValue.m_offset;
	if offset < 0 then
		return;
	end
	if m_dataCache[offset] == nil then
		-- 新加城防军数量
		m_num = m_num + 1;
		SetText( m_uiGuardNum, T(652).." "..m_num.."/"..m_level );
	end
	m_dataCache[offset] = recvValue;
	
	local uiObj = m_uiCache[offset];
	if uiObj == nil then
		uiObj = m_ObjectPool:Get( "UIP_Guard" );
		uiObj.transform:SetParent( m_uiContent.transform );
		m_uiCache[offset] = uiObj;	
	end
	
	local level = m_dataCache[offset].m_level;
	local color = m_dataCache[offset].m_color;
	m_dataCache[offset].m_arrow = true;
	if level >= g_building_upgrade[BUILDING_Wall][m_level].value1 then
		local maxcolor = 1;
		local colorminlist = { global.city_guard_color1_min, global.city_guard_color2_min, global.city_guard_color3_min, global.city_guard_color4_min, global.city_guard_color5_min }
		for i=5, 1, -1 do
			if level >= colorminlist[i] then
				maxcolor = i;
				break;
			end
		end
		print( color.."/"..maxcolor )
		if color >= maxcolor then
			m_dataCache[offset].m_arrow = false
		end
	end
	CityGuardDlgSetObj( uiObj, m_dataCache[offset].m_shape, m_dataCache[offset].m_color, m_dataCache[offset].m_corps, m_dataCache[offset].m_level, m_dataCache[offset].m_soldiers, m_dataCache[offset].m_troops, m_dataCache[offset].m_arrow, false )
end

-- 设置对象
function CityGuardDlgSetObj( uiObj, shape, color, corps, level, hp, hpmax, arrow, lock )
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiShape = objs[0];
	local uiColor = objs[1];
	local uiCorps = objs[2];
	local uiLevel = objs[3];
	local uiArrow = objs[4];
	local uiProgress = objs[5];
	
	-- 
	if shape == nil then
		if lock == true then
			SetImage( uiShape, LoadSprite("ui_icon_back_2") )
		else
			SetImage( uiShape, LoadSprite("ui_icon_back_4") )
		end
	else
		SetImage( uiShape, GuardSprite( shape ) )
	end
	
	--
	if color == nil then
		SetFalse( uiColor );
	else
		SetTrue( uiColor );
		SetImage( uiColor, ItemColorSprite( color ) )
	end
	
	--
	if corps == nil then
		SetFalse( uiCorps );
	else
		SetTrue( uiCorps );
		SetImage( uiCorps, CorpsSprite( corps ) )
	end
	
	--
	if level == nil then
		if lock == true then
			SetText( uiLevel, F(725, BuildingName(BUILDING_Wall), m_nextlevel ) )
		else
			SetText( uiLevel, T(794) )
		end
	else
		SetLevel( uiLevel, level )
	end
	
	if hp == nil then
		SetFalse( uiProgress );
	else
		SetTrue( uiProgress );
		SetProgress( uiProgress, hp/hpmax );
	end
	
	--
	if arrow == true then
		SetTrue( uiArrow );
	else
		SetFalse( uiArrow );
	end
end

function CityGuardDlgChangeSec( recvValue )
	-- 冷却时间
	if recvValue.m_guardsec > 0 then
		SetTrue( m_uiTimerText );
		SetTrue( m_uiDescText );
		SetTrue( m_uiCDButton );
		SetFalse( m_uiCallButton );
		SetTimer( m_uiTimerText, recvValue.m_guardsec, recvValue.m_guardsec, 1 )
	else
		SetFalse( m_uiTimerText );
		SetFalse( m_uiDescText );
		SetFalse( m_uiCDButton );
		if m_level < global.city_guard_level then
			SetFalse( m_uiCallButton );
		else
			SetTrue( m_uiCallButton );
		end
	end
end

function CityGuardDlgSelect( index )
	local offset = index;
	if m_dataCache[offset] == nil then
		-- 招募
		system_askinfo( ASKINFO_CITYGUARD, "", 1 );
	else
		if m_dataCache[offset].m_arrow == nil or m_dataCache[offset].m_arrow == false then
			pop(T(800))
			return
		end
		
		if m_dataCache[offset].m_soldiers < m_dataCache[offset].m_troops then
			-- 补血
			local token = global.city_guard_up_token;
			MsgBoxEx( F( 797, token ), function( sure, toggle ) 
				if sure == 1 then
					if GetPlayer().m_token < token then
						JumpToken()
					else
						system_askinfo( ASKINFO_CITYGUARD, "", 4, offset );
					end
				end
				m_guardup_toggle = toggle;
			end, m_guardup_toggle ) 
		else
			-- 升级
			local token = global.city_guard_up_token;
			MsgBoxEx( F( 798, token ), function( sure, toggle ) 
				if sure == 1 then
					if GetPlayer().m_token < token then
						JumpToken()
					else
						system_askinfo( ASKINFO_CITYGUARD, "", 2, offset );
					end
				end
				m_guardup_toggle = toggle;
			end, m_guardup_toggle ) 
		end
	end
end

function CityGuardDlgClearCD()
	local left = m_uiTimerText.transform:GetComponent( typeof(UITextTimeCountdown) ).LeftTime
	local token = math.ceil( left / global.city_guard_sec_token );
	MsgBox( F( 796, token ), function() 
		if GetPlayer().m_token < token then
			JumpToken()
		else
			system_askinfo( ASKINFO_CITYGUARD, "", 3 );
		end
	end ) 
end

