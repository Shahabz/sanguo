-- 界面
local m_Dlg = nil;
local m_DialogFrameMod = nil;

local m_uiNationBtn = nil; --UnityEngine.GameObject
local m_uiCityBtn = nil; --UnityEngine.GameObject
local m_uiNationScroll = nil; --UnityEngine.GameObject
local m_uiNationContent = nil; --UnityEngine.GameObject
local m_uiUIP_NationArmy = nil; --UnityEngine.GameObject
local m_uiCityScroll = nil; --UnityEngine.GameObject
local m_uiCityContent = nil; --UnityEngine.GameObject
local m_uiUIP_CityArmy = nil; --UnityEngine.GameObject

local m_ObjectPool = nil
local m_SelectType = 0;
local m_NationRecvValue = nil
local m_CityRecvValue = nil

local UIADD_NATION_EVENT_BASE = 1000

local UIASKHELPBTN_EVENT_BASE = 10000
local UIADDATTACKBTN_EVENT_BASE = 20000
local UIADDDEFENSEBTN_EVENT_BASE = 30000

-- 打开界面
function NationWarDlgOpen()
	m_Dlg = eye.uiManager:Open( "NationWarDlg" );
	m_DialogFrameMod = DialogFrameModOpen( m_Dlg, T(1947), HELP_NationDlg, NationWarDlgClose );
end

-- 隐藏界面
function NationWarDlgClose()
	if m_Dlg == nil then
		return;
	end
	DialogFrameModClose( m_DialogFrameMod );
	m_DialogFrameMod = nil;
	eye.uiManager:Close( "NationWarDlg" );
	m_NationRecvValue = nil
	m_CityRecvValue = nil
	m_SelectType = 0;
end

-- 删除界面
function NationWarDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
	m_NationRecvValue = nil
	m_CityRecvValue = nil
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function NationWarDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            NationWarDlgClose();
		elseif nControlID == 1 then
			NationWarDlgSelect( 1 )
		elseif nControlID == 2 then
			NationWarDlgSelect( 2 )
			
		-- 加入国战
		elseif nControlID >= UIADD_NATION_EVENT_BASE and nControlID < UIASKHELPBTN_EVENT_BASE then
			NationWarDlgDlgNationJoin( nControlID-UIADD_NATION_EVENT_BASE )
			
		-- 请求援助
		elseif nControlID >= UIASKHELPBTN_EVENT_BASE and nControlID < UIADDATTACKBTN_EVENT_BASE then
			NationWarDlgDlgCityAskHelp( nControlID-UIASKHELPBTN_EVENT_BASE )
			
		-- 加入攻击方
		elseif nControlID >= UIADDATTACKBTN_EVENT_BASE and nControlID < UIADDDEFENSEBTN_EVENT_BASE then	
			NationWarDlgDlgCityAddAttack( nControlID-UIADDATTACKBTN_EVENT_BASE )
			
		-- 加入防御方
		elseif nControlID >= UIADDDEFENSEBTN_EVENT_BASE then	
			NationWarDlgDlgCityAddDefense( nControlID-UIADDDEFENSEBTN_EVENT_BASE )
			
        end
	end
end

-- 载入时调用
function NationWarDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	
	m_uiNationBtn = objs[0];
	m_uiCityBtn = objs[1];
	m_uiNationScroll = objs[2];
	m_uiNationContent = objs[3];
	m_uiUIP_NationArmy = objs[4];
	m_uiCityScroll = objs[5];
	m_uiCityContent = objs[6];
	m_uiUIP_CityArmy = objs[7];
	
	-- 对象池
	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );
	m_ObjectPool:CreatePool("UIP_NationArmy", 1, 1, m_uiUIP_NationArmy);
	m_ObjectPool:CreatePool("UIP_CityArmy", 1, 1, m_uiUIP_CityArmy);
end

-- 界面初始化时调用
function NationWarDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function NationWarDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function NationWarDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function NationWarDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function NationWarDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function NationWarDlgShow()
	NationWarDlgOpen()
	NationWarDlgSelect( 1 )
end

function NationWarDlgNationUpdate()
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	if m_SelectType == 2 then
		return
	end
	NationWarDlgSelect( 1 )
end

function NationWarDlgCityUpdate()
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	if m_SelectType == 1 then
		return
	end
	NationWarDlgSelect( 2 )
end

function NationWarDlgSelect( type )	
	m_SelectType = type;
    -- 读取缓存
	if type == 1 then
		SetTrue( m_uiNationScroll )
		SetFalse( m_uiCityScroll )
		SetImage( m_uiNationBtn.transform:Find("Back"), LoadSprite("ui_button_18") )
		SetImage( m_uiCityBtn.transform:Find("Back"), LoadSprite("ui_button_17") )
		system_askinfo( ASKINFO_NATION, "", 6 )
		
	elseif type == 2 then
		SetFalse( m_uiNationScroll )
		SetTrue( m_uiCityScroll )
		SetImage( m_uiCityBtn.transform:Find("Back"), LoadSprite("ui_button_18") )
		SetImage( m_uiNationBtn.transform:Find("Back"), LoadSprite("ui_button_17") )
		system_askinfo( ASKINFO_NATION, "", 7 )
		
	end
end

-- 清空
function NationWarDlgNationClear()
	local objs = {};
	for i = 0 ,m_uiNationContent.transform.childCount - 1 do
		table.insert( objs, m_uiNationContent.transform:GetChild(i).gameObject )
    end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "UIP_NationArmy(Clone)" then
			m_ObjectPool:Release( "UIP_NationArmy", obj );
		end
    end
end
-- 清空
function NationWarDlgCityClear()
	local objs = {};
	for i = 0 ,m_uiCityContent.transform.childCount - 1 do
		table.insert( objs, m_uiCityContent.transform:GetChild(i).gameObject )
    end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "UIP_CityArmy(Clone)" then
			m_ObjectPool:Release( "UIP_CityArmy", obj );
		end
    end
end

-- 国战接收开始
function NationWarDlgNationRecvBegin()
	NationWarDlgNationClear()
	m_NationRecvValue = {}
end

-- 国战接收过程
-- m_count=0,m_list={m_group_index=0,m_group_id=0,m_attack=0,m_statetime=0,m_stateduration=0,m_nation=0,m_t_nation=0,m_total=0,m_t_total=0,m_type=0,m_townid=0,m_unit_index=0,m_town_nation=0[m_count]},m_op=0,
function NationWarDlgNationRecv( recvValue )
	for i=1, recvValue.m_count, 1 do
		table.insert( m_NationRecvValue, recvValue.m_list[i] )
	end
end

-- 国战接收完毕
function NationWarDlgNationRecvEnd()
	if #m_NationRecvValue == 0 then
		pop(T(1782))
		return
	end
	for i=1, #m_NationRecvValue, 1 do
		NationWarDlgNationCreate( i, m_NationRecvValue[i] )
	end
end

-- 创建
function NationWarDlgNationCreate( index, info )
	local uiObj = m_ObjectPool:Get( "UIP_NationArmy" );
	uiObj.transform:SetParent( m_uiNationContent.transform );
	
	local objs = uiObj:GetComponent( typeof(Reference) ).relatedGameObject;
	uiFlagText = objs[0];
	uiTotal = objs[1];
	uiName = objs[2];
	uiShape = objs[3];
	uiPos = objs[4];
	uiTimer = objs[5];
	uiArrowAttack = objs[6];
	uiArrowDefense = objs[7];
	uiTFlagText = objs[8];
	uiTTotal = objs[9];
	uiNation = objs[10];
	uiTNation = objs[11];
	uiAddBtn = objs[12];
		
	-- 形象
	local type 	= g_towninfo[info.m_townid].type
	SetImage( uiShape, LoadSprite( MapUnitTownShapeList[type].."_"..info.m_town_nation ) )
	
	-- 名称
	SetText( uiName, MapTownName( info.m_townid ) )
	
	-- 坐标
	SetRichText( uiPos, F( 1266, g_towninfo[info.m_townid].posx, g_towninfo[info.m_townid].posy ), MailOnLinkClick )
	
	-- 倒计时
	SetTimer( uiTimer, info.m_stateduration-info.m_statetime, info.m_stateduration )
	
	-- 国旗
	SetImage( uiNation, NationSprite( info.m_nation ) )
	SetImage( uiTNation, NationSprite( info.m_t_nation ) )
	
				
	-- 属于攻击方
	if info.m_attack == 1 then
		SetTrue( uiArrowAttack )
		SetFalse( uiArrowDefense )
		SetText( uiFlagText, T(1264), Hex2Color(0xffde00ff) )
		SetText( uiTFlagText, T(1265), Hex2Color(0x03de27ff) )
		-- 守备兵力
		SetText( uiTotal, T(1259).."\n"..info.m_total )
		SetText( uiTTotal, T(1262).."\n"..info.m_t_total )
	
	-- 属于防御方
	elseif info.m_attack == 2 then
	
		SetFalse( uiArrowAttack )
		SetTrue( uiArrowDefense )
		SetText( uiFlagText, T(1265), Hex2Color(0x03de27ff) )
		SetText( uiTFlagText, T(1264), Hex2Color(0xffde00ff) )
		
		-- 守备兵力
		SetText( uiTotal, T(1261).."\n"..info.m_total )
		SetText( uiTTotal, T(1260).."\n"..info.m_t_total )
	
	-- 第三方	
	elseif info.m_attack == 3 then
		SetTrue( uiArrowAttack )
		SetFalse( uiArrowDefense )
		SetText( uiFlagText, T(1264), Hex2Color(0xffde00ff) )
		SetText( uiTFlagText, T(1265), Hex2Color(0x03de27ff) )
		SetFalse( uiAddBtn )
	end
	
	-- 加入国战按钮	
	SetControlID( uiAddBtn, UIADD_NATION_EVENT_BASE + index )
end

-- 加入国战
function NationWarDlgDlgNationJoin( index )
	local info = m_NationRecvValue[index]
	if info == nil then
		return
	end
	if GetPlayer().m_level < global.nationfight_actorlevel then
		AlertMsg( F( 1344, global.nationfight_actorlevel ) )
		return
	end
	
	local unitRecvValue = {}
	unitRecvValue.m_char_value = {}
	unitRecvValue.m_short_value = {}
	unitRecvValue.m_unit_index = info.m_unit_index
	unitRecvValue.m_posx = g_towninfo[info.m_townid].posx;
	unitRecvValue.m_posy = g_towninfo[info.m_townid].posy;
	unitRecvValue.m_name = "";
	unitRecvValue.m_namelen = 0
	unitRecvValue.m_char_value[1] = info.m_town_nation
	unitRecvValue.m_short_value[1] = info.m_townid
	unitRecvValue.m_type = MAPUNIT_TYPE_TOWN
			
	if info.m_attack == 2 then
		MapBattleDlgShow( unitRecvValue, ARMY_ACTION_NATION_DEFENSE, info.m_group_index );
	else
		MapBattleDlgShow( unitRecvValue, ARMY_ACTION_NATION_ATTACK, info.m_group_index );
	end
	MapBattleDlgID( info.m_group_id )
end

-- 城战接收开始
function NationWarDlgCityRecvBegin()
	NationWarDlgCityClear()
	m_CityRecvValue = {}
end

-- 城战接收过程
-- m_count=0,m_list={m_group_index=0,m_group_id=0,m_attack=0,m_statetime=0,m_stateduration=0,m_nation=0,m_t_nation=0,m_level=0,m_t_level=0,m_name_length=0,m_name="[m_name_length]",m_t_name_length=0,m_t_name="[m_t_name_length]",m_posx=0,m_posy=0,m_t_posx=0,m_t_posy=0,m_actorid=0,m_t_actorid=0,m_total=0,m_t_total=0,m_type=0,m_unit_index=0,[m_count]},m_op=0
function NationWarDlgCityRecv( recvValue )
	for i=1, recvValue.m_count, 1 do
		table.insert( m_CityRecvValue, recvValue.m_list[i] )
	end
end

-- 城战接收完毕
function NationWarDlgCityRecvEnd()
	if #m_CityRecvValue == 0 then
		pop(T(1782))
		return
	end
	for i=1, #m_CityRecvValue, 1 do
		NationWarDlgCityCreate( i, m_CityRecvValue[i] )
	end
end

-- 创建
function NationWarDlgCityCreate( index, info )
	local uiObj = m_ObjectPool:Get( "UIP_CityArmy" );
	uiObj.transform:SetParent( m_uiCityContent.transform );
	
	local objs = uiObj:GetComponent( typeof(Reference) ).relatedGameObject;
	uiFlagText = objs[0];
	uiTotal = objs[1];
	uiName = objs[2];
	uiShape = objs[3];
	uiPos = objs[4];
	uiTimer = objs[5];
	uiArrowAttack = objs[6];
	uiArrowDefense = objs[7];
	uiTFlagText = objs[8];
	uiTTotal = objs[9];
	uiTName = objs[10];
	uiTShape = objs[11];
	uiTPos = objs[12];
	uiAskHelpBtn = objs[13];
	uiAddAttackBtn = objs[14];
	uiAddDefenseBtn = objs[15];
	uiWarn = objs[16];
	
	if info.m_actorid == GetPlayer().m_actorid or info.m_t_actorid == GetPlayer().m_actorid then
		SetTrue( uiAskHelpBtn )
		SetControlID( uiAskHelpBtn, UIASKHELPBTN_EVENT_BASE + index )
		SetFalse( uiAddDefenseBtn )
	else
		SetFalse( uiAskHelpBtn )
	end
	
	-- 攻击方
	if info.m_attack == 1 then
		SetTrue( uiArrowAttack )
		SetFalse( uiArrowDefense )
		SetText( uiFlagText, T(1264), Hex2Color(0xffde00ff) )
		SetText( uiTFlagText, T(1265), Hex2Color(0x03de27ff) )
		
		-- 守备兵力
		SetText( uiTotal, T(1259).."\n"..info.m_total )
		SetText( uiTTotal, T(1262).."\n"..info.m_t_total )
		
		-- 参与按钮
		if info.m_type == 1 then -- 闪电战
			SetFalse( uiAskHelpBtn )
			SetFalse( uiAddAttackBtn )
			SetTrue( uiWarn )
			SetText( uiWarn, T(1267) )
		else
			SetTrue( uiAddAttackBtn )
			SetControlID( uiAddAttackBtn, UIADDATTACKBTN_EVENT_BASE + index )
			SetFalse( uiWarn )
		end
		SetFalse( uiAddDefenseBtn )
	
	else
		SetFalse( uiArrowAttack )
		SetTrue( uiArrowDefense )
		SetText( uiFlagText, T(1265), Hex2Color(0x03de27ff) )
		SetText( uiTFlagText, T(1264), Hex2Color(0xffde00ff) )
		
		-- 守备兵力
		SetText( uiTotal, T(1261).."\n"..info.m_total )
		SetText( uiTTotal, T(1260).."\n"..info.m_t_total )
		
		-- 参与按钮
		SetFalse( uiWarn )
		SetFalse( uiAddAttackBtn )
		
		if info.m_actorid == GetPlayer().m_actorid or info.m_t_actorid == GetPlayer().m_actorid then
			SetFalse( uiAddDefenseBtn )
		else
			SetTrue( uiAddDefenseBtn )
			SetControlID( uiAddDefenseBtn, UIADDDEFENSEBTN_EVENT_BASE + index )
		end
	end
	
	-- 倒计时
	SetTimer( uiTimer, info.m_stateduration-info.m_statetime, info.m_stateduration )
	
	-- 形象
	SetImage( uiShape, LoadSprite( MapUnitCityShapeList[info.m_level].."_"..info.m_nation ) )
	SetImage( uiTShape, LoadSprite( MapUnitCityShapeList[info.m_t_level].."_"..info.m_t_nation ) )
	
	-- 名称
	SetText( uiName, info.m_name )
	SetText( uiTName, info.m_t_name )
	
	-- 坐标
	SetRichText( uiPos, F( 1266, info.m_posx, info.m_posy ), MailOnLinkClick )
	SetRichText( uiTPos, F( 1266, info.m_t_posx, info.m_t_posy ), MailOnLinkClick )
end

-- 请求援助
function NationWarDlgDlgCityAskHelp( index )
	local info = m_CityRecvValue[index]
	if info == nil then
		return
	end
	system_askinfo( ASKINFO_CITYARMYGROUP, "", 1, info.m_group_index, info.m_group_id );
end

-- 加入攻击方
function NationWarDlgDlgCityAddAttack( index )
	local info = m_CityRecvValue[index]
	if info == nil then
		return
	end
	
	local unitRecvValue = {}
	unitRecvValue.m_char_value = {}
	unitRecvValue.m_short_value = {}
	unitRecvValue.m_unit_index = info.m_unit_index
	unitRecvValue.m_type = MAPUNIT_TYPE_CITY
	if info.m_attack == 1 then
		unitRecvValue.m_posx = info.m_t_posx;
		unitRecvValue.m_posy = info.m_t_posy;
		unitRecvValue.m_name = info.m_t_name;
		unitRecvValue.m_char_value[1] = info.m_t_level
		unitRecvValue.m_char_value[2] = info.m_t_nation
	else
		unitRecvValue.m_posx = info.m_posx;
		unitRecvValue.m_posy = info.m_posy;
		unitRecvValue.m_name = info.m_name;
		unitRecvValue.m_char_value[1] = info.m_level
		unitRecvValue.m_char_value[2] = info.m_nation
	end

	MapBattleDlgShow( unitRecvValue, ARMY_ACTION_GROUP_ATTACK, info.m_group_index );
	MapBattleDlgID( info.m_group_id )
end

-- 加入防守方
function NationWarDlgDlgCityAddDefense( index )
	local info = m_CityRecvValue[index]
	if info == nil then
		return
	end
	
	local unitRecvValue = {}
	unitRecvValue.m_char_value = {}
	unitRecvValue.m_short_value = {}
	unitRecvValue.m_unit_index = info.m_unit_index
	unitRecvValue.m_type = MAPUNIT_TYPE_CITY
	if info.m_attack == 1 then
		unitRecvValue.m_posx = info.m_t_posx;
		unitRecvValue.m_posy = info.m_t_posy;
		unitRecvValue.m_name = info.m_t_name;
		unitRecvValue.m_char_value[1] = info.m_t_level
		unitRecvValue.m_char_value[2] = info.m_t_nation
	else
		unitRecvValue.m_posx = info.m_posx;
		unitRecvValue.m_posy = info.m_posy;
		unitRecvValue.m_name = info.m_name;
		unitRecvValue.m_char_value[1] = info.m_level
		unitRecvValue.m_char_value[2] = info.m_nation
	end
	
	MapBattleDlgShow( unitRecvValue, ARMY_ACTION_GROUP_DEFENSE, info.m_group_index );
	MapBattleDlgID( info.m_group_id )
end
