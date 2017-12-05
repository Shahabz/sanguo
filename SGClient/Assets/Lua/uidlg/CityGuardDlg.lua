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
local m_uiHeroSortLayer = nil; --UnityEngine.GameObject

local m_level = 1;
local m_nextlevel = 1;
local m_ObjectPool = nil;
local m_dataCache = {};
local m_uiCache = {};
local m_num = 0;
local m_guardup_toggle = false;

local m_HeroList = {}
local m_HeroObjList = {}
local m_HeroSortList = {}
-- 打开界面
function CityGuardDlgOpen()
	m_Dlg = eye.uiManager:Open( "CityGuardDlg" );
	m_DialogFrameMod = DialogFrameModOpen( m_Dlg, BuildingName(BUILDING_Wall).." Lv."..m_level, HELP_CityGuardDlg, CityGuardDlgClose );
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
	m_HeroList = {}
	m_HeroObjList = {}
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
		elseif nControlID == -2 then
			CityGuardDlgHeroSortLayerHide()
			
		-- 招募按钮
		elseif nControlID == 101 then
			system_askinfo( ASKINFO_CITYGUARD, "", 1 );
			
		-- 清除CD
		elseif nControlID == 102 then
			CityGuardDlgClearCD()
			
		-- 武将防守顺序
		elseif nControlID == 103 then
			CityGuardDlgHeroSortLayerShow()
			
		-- 友军驻防武将
		elseif nControlID == 104 then
			if m_level < global.cityhelp_level then
				pop( F(1247, global.cityhelp_level ) )
			else
				CityArmyHelpDlgShow()
			end
		
		-- 防守顺序调整确定
		elseif nControlID == 105 then
			CityGuardDlgHeroSortOK()
			
		-- 防守顺序调整
		elseif nControlID >= 110 and nControlID <= 114 then
			CityGuardDlgHeroSortUp( nControlID-110 )
				
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
	m_uiHeroSortLayer = objs[7];
	
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
	
	-- 设置御林卫
	m_HeroList = {}
	m_HeroObjList = {}
	for i=8,11,1 do -- 羽林卫武将位置8-11
		if GetHero().m_CityHero[i].m_kind > 0 then
			table.insert( m_HeroList, GetHero().m_CityHero[i] );
			
			local uiObj = m_ObjectPool:Get( "UIP_Guard" );
			table.insert( m_HeroObjList, uiObj )
			uiObj.transform:SetParent( m_uiContent.transform );
		end
	end 
	CityGuardDlgSetHero();
	CityGuardDlgCreateEmpty();
	system_askinfo( ASKINFO_CITYGUARD, "", 0 );
end

function CityGuardDlgSetHero()
	for i=1, #m_HeroList, 1 do
		local pHero = m_HeroList[i];
		local uiObj = m_HeroObjList[i]
		SetControlID( uiObj, 0 )
		CityGuardDlgSetObj( uiObj, -pHero.m_kind, pHero.m_color, pHero.m_corps, pHero.m_level, pHero.m_soldiers, pHero.m_troops, nil, false, true )
	end
end

-- m_count=0,m_list={m_corps=0,m_color=0,m_shape=0,m_level=0,m_soldiers=0,m_troops=0,m_offset=0,[m_count]},m_guardsec=0,
function CityGuardDlgRecv( recvValue )
	m_dataCache = {};
	for i=1, recvValue.m_count, 1 do
		CityGuardDlgSet( recvValue.m_list[i] );
	end
	
	-- 城防军数量
	m_num = recvValue.m_count;
	GetPlayer().m_guardnum = m_num
	City.GuardCallMod( nil, true )
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
		CityGuardDlgSetObj( uiObj, nil, nil, nil, nil, nil, nil, false, true, false )
	else
		
		-- 已解锁的
		for i=0, m_level-1, 1 do
			local uiObj = m_ObjectPool:Get( "UIP_Guard" );
			uiObj.transform:SetParent( m_uiContent.transform );
			CityGuardDlgSetObj( uiObj, nil, nil, nil, nil, nil, nil, false, false, false )
			SetControlID( uiObj, 1000+i )
			m_uiCache[i] = uiObj;
		end
	
		-- 在创建一个未解锁的
		local uiObj = m_ObjectPool:Get( "UIP_Guard" );
		uiObj.transform:SetParent( m_uiContent.transform );
		m_nextlevel = m_level + 1;
		CityGuardDlgSetObj( uiObj, nil, nil, nil, nil, nil, nil, false, true, false )
		
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
		GetPlayer().m_guardnum = m_num
		City.GuardCallMod( nil, true )
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
		if color >= maxcolor then
			m_dataCache[offset].m_arrow = false
		end
	end
	CityGuardDlgSetObj( uiObj, m_dataCache[offset].m_shape, m_dataCache[offset].m_color, m_dataCache[offset].m_corps, m_dataCache[offset].m_level, m_dataCache[offset].m_soldiers, m_dataCache[offset].m_troops, m_dataCache[offset].m_arrow, false, false )
end

-- 设置对象
function CityGuardDlgSetObj( uiObj, shape, color, corps, level, hp, hpmax, arrow, lock, flag )
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiShape = objs[0];
	local uiColor = objs[1];
	local uiCorps = objs[2];
	local uiLevel = objs[3];
	local uiArrow = objs[4];
	local uiProgress = objs[5];
	local uiFlag = objs[6];
	-- 
	if shape == nil then
		if lock == true then
			SetImage( uiShape, LoadSprite("ui_icon_back_2") )
		else
			SetImage( uiShape, LoadSprite("ui_icon_back_4") )
		end
	else
		if shape > 0 then
			SetImage( uiShape, GuardSprite( shape ) )
		else
			SetImage( uiShape, HeroHeadSprite( -shape ) )
		end
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
	
	--
	if flag == true then
		SetTrue( uiFlag );
	else
		SetFalse( uiFlag );
	end
end

function CityGuardDlgChangeSec( recvValue )
	-- 冷却时间
	if recvValue.m_guardsec > 0 then
		SetTrue( m_uiTimerText );
		SetTrue( m_uiDescText );
		SetTrue( m_uiCDButton );
		SetFalse( m_uiCallButton );
		SetTimer( m_uiTimerText, recvValue.m_guardsec, recvValue.m_guardsec, 1, T(1463) )
	else
		SetFalse( m_uiTimerText );
		SetFalse( m_uiDescText );
		SetFalse( m_uiCDButton );
		if m_level < global.city_guard_level or GetPlayer().m_guardnum >= m_level then
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

-- 武将防守顺序调整
function CityGuardDlgHeroSortLayerShow()
	SetTrue( m_uiHeroSortLayer )
	local uiGrid = m_uiHeroSortLayer.transform:Find("Panel/Grid");
	for i=0,3,1 do
		local uiObj = uiGrid.transform:GetChild( i )
		SetFalse( uiObj )
	end
	-- 设置御林卫
	m_HeroSortList = {}
	local index = 0;
	for i=8,11,1 do -- 羽林卫武将位置8-11
		if GetHero().m_CityHero[i].m_kind > 0 then
			table.insert( m_HeroSortList, GetHero().m_CityHero[i] );
		end
	end 
	CityGuardDlgHeroSortSet()
end

function CityGuardDlgHeroSortLayerHide()
	SetFalse( m_uiHeroSortLayer )
end

function CityGuardDlgHeroSortSet()
	local uiGrid = m_uiHeroSortLayer.transform:Find("Panel/Grid");
	-- 设置御林卫
	for index=1,#m_HeroSortList,1 do
		local pHero = m_HeroSortList[index]
		local uiObj = uiGrid.transform:GetChild( index-1 )
		SetTrue( uiObj )
		local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
		local uiShape = objs[0];
		local uiColor = objs[1];
		local uiCorps = objs[2];
		local uiName = objs[3];
		local uiSortText = objs[4];
		local uiUpBtn = objs[5];
		SetControlID( uiUpBtn, 110+index )
		SetImage( uiShape, HeroHeadSprite(pHero.m_kind) )
		SetImage( uiColor, ItemColorSprite(pHero.m_color) )
		SetImage( uiCorps, CorpsSprite(pHero.m_corps) )
		SetText( uiName, HeroNameLv( pHero.m_kind, pHero.m_level ) )
		SetText( uiSortText, index )
		if index == 1 then
			SetFalse( uiUpBtn )
		else
			SetTrue( uiUpBtn )
		end
	end 
end

function CityGuardDlgHeroSortUp( index )
	if index == 1 then
		return;
	end
	m_HeroSortList[index-1], m_HeroSortList[index] = m_HeroSortList[index], m_HeroSortList[index-1]
	CityGuardDlgHeroSortSet()
end

function CityGuardDlgHeroSortOK()
	CityGuardDlgHeroSortLayerHide()
	
	-- m_herokind={[4]},
	local sendValue = {}
	sendValue.m_herokind = { 0, 0, 0, 0 }
	
	for i=1, #m_HeroSortList, 1 do
		m_HeroList[i] = m_HeroSortList[i]
		sendValue.m_herokind[i] = m_HeroSortList[i].m_kind
	end	
	netsend_heroguardsort_C( sendValue )
	CityGuardDlgSetHero()
end