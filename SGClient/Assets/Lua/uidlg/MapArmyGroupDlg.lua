-- 界面
local m_Dlg = nil;
local m_uiScroll = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiUIP_ArmyGroup = nil; --UnityEngine.GameObject
local m_uiButtons = nil; --UnityEngine.GameObject
local m_uiCloseButton = nil; --UnityEngine.GameObject
local m_uiCItyFightButton = nil; --UnityEngine.GameObject

local m_ObjectPool = nil;
local m_nation = 0;
local m_unit_index = -1
local m_cache = {};

local UIASKHELPBTN_EVENT_BASE = 1000000
local UIADDATTACKBTN_EVENT_BASE = 2000000
local UIADDDEFENSEBTN_EVENT_BASE = 3000000

-- 打开界面
function MapArmyGroupDlgOpen()
	m_Dlg = eye.uiManager:Open( "MapArmyGroupDlg" );
end

-- 隐藏界面
function MapArmyGroupDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "MapArmyGroupDlg" );
end

-- 删除界面
function MapArmyGroupDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function MapArmyGroupDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            MapArmyGroupDlgClose();
		elseif nControlID == 1 then
			MapClickModOpenCityFight( m_unit_index )
		
		-- 请求援助
		elseif nControlID >= UIASKHELPBTN_EVENT_BASE and nControlID < UIADDATTACKBTN_EVENT_BASE then
			MapArmyGroupDlgAskHelp( nControlID-UIASKHELPBTN_EVENT_BASE )
			
		-- 加入攻击方
		elseif nControlID >= UIADDATTACKBTN_EVENT_BASE and nControlID < UIADDDEFENSEBTN_EVENT_BASE then	
			MapArmyGroupDlgAddAttack( nControlID-UIADDATTACKBTN_EVENT_BASE )
			
		-- 加入防御方
		elseif nControlID >= UIADDDEFENSEBTN_EVENT_BASE then	
			MapArmyGroupDlgAddDefense( nControlID-UIADDDEFENSEBTN_EVENT_BASE )
			
        end
	elseif nType == UI_EVENT_TIMECOUNTEND then
		--MapArmyGroupDlgShow( m_unit_index )
		MapArmyGroupDlgClose()
	end
end

-- 载入时调用
function MapArmyGroupDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiScroll = objs[0];
	m_uiContent = objs[1];
	m_uiUIP_ArmyGroup = objs[2];
	m_uiButtons = objs[3];
	m_uiCloseButton = objs[4];
	m_uiCItyFightButton = objs[5];
	
	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );
	m_ObjectPool:CreatePool("UIP_ArmyGroup", 4, 4, m_uiUIP_ArmyGroup);
end

-- 界面初始化时调用
function MapArmyGroupDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function MapArmyGroupDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function MapArmyGroupDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function MapArmyGroupDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function MapArmyGroupDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function MapArmyGroupDlgShow( unit_index )
	MapArmyGroupDlgOpen()
	MapArmyGroupDlgClear()
	SetFalse( m_uiButtons );
	m_cache = {};
	m_unit_index = unit_index;
	system_askinfo( ASKINFO_CITYARMYGROUP, "", 0, unit_index )
end

function MapArmyGroupDlgBegin( nation, unit_index )
	m_nation = nation;
	m_unit_index = unit_index;
	m_cache = {};
	SetTrue( m_uiButtons );
	if m_nation == GetPlayer().m_nation then
		SetFalse( m_uiCItyFightButton )
	else
		SetTrue( m_uiCItyFightButton )
	end
end

-- m_count=0,m_list={m_group_index=0,m_group_id=0,m_attack=0,m_statetime=0,m_stateduration=0,m_nation=0,m_t_nation=0,m_level=0,m_t_level=0,m_name_length=0,m_name="[m_name_length]",m_t_name_length=0,m_t_name="[m_t_name_length]",m_posx=0,m_posy=0,m_t_posx=0,m_t_posy=0,m_actorid=0,m_t_actorid=0,m_total=0,m_t_total=0,[m_count]},
function MapArmyGroupDlgAddRecvValue( recvValue )
	table.insert( m_cache, recvValue );
	local uiObj = m_ObjectPool:Get( "UIP_ArmyGroup" );
	uiObj.transform:SetParent( m_uiContent.transform );
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
	
	if recvValue.m_actorid == GetPlayer().m_actorid or recvValue.m_t_actorid == GetPlayer().m_actorid then
		SetTrue( uiAskHelpBtn )
		SetControlID( uiAskHelpBtn, UIASKHELPBTN_EVENT_BASE + #m_cache )
		SetFalse( m_uiCItyFightButton )
		SetFalse( uiAddDefenseBtn )
	else
		SetFalse( uiAskHelpBtn )
	end
	
	-- 这个人是本国玩家，那么都显示在左面
	if m_nation == GetPlayer().m_nation then
		
		-- 攻击方
		if recvValue.m_attack == 1 then
			SetTrue( uiArrowAttack )
			SetFalse( uiArrowDefense )
			SetText( uiFlagText, T(1264), Hex2Color(0xffde00ff) )
			SetText( uiTFlagText, T(1265), Hex2Color(0x03de27ff) )
			
			-- 守备兵力
			SetText( uiTotal, T(1259).."\n"..recvValue.m_total )
			SetText( uiTTotal, T(1262).."\n"..recvValue.m_t_total )
			
			-- 参与按钮
			if recvValue.m_type == 1 then -- 闪电战
				SetFalse( uiAskHelpBtn )
				SetFalse( uiAddAttackBtn )
				SetTrue( uiWarn )
				SetText( uiWarn, T(1267) )
			else
				SetTrue( uiAddAttackBtn )
				SetControlID( uiAddAttackBtn, UIADDATTACKBTN_EVENT_BASE + #m_cache )
				SetFalse( uiWarn )
			end
			SetFalse( uiAddDefenseBtn )
		
		else
			SetFalse( uiArrowAttack )
			SetTrue( uiArrowDefense )
			SetText( uiFlagText, T(1265), Hex2Color(0x03de27ff) )
			SetText( uiTFlagText, T(1264), Hex2Color(0xffde00ff) )
			
			-- 守备兵力
			SetText( uiTotal, T(1261).."\n"..recvValue.m_total )
			SetText( uiTTotal, T(1260).."\n"..recvValue.m_t_total )
			
			-- 参与按钮
			SetFalse( uiWarn )
			SetFalse( uiAddAttackBtn )
			
			if recvValue.m_actorid == GetPlayer().m_actorid or recvValue.m_t_actorid == GetPlayer().m_actorid then
				SetFalse( uiAddDefenseBtn )
			else
				SetTrue( uiAddDefenseBtn )
				SetControlID( uiAddDefenseBtn, UIADDDEFENSEBTN_EVENT_BASE + #m_cache )
			end
		end
		
		-- 倒计时
		SetTimer( uiTimer, recvValue.m_stateduration-recvValue.m_statetime, recvValue.m_stateduration )
		
		-- 形象
		SetImage( uiShape, LoadSprite( MapUnitCityShapeList[recvValue.m_level].."_"..recvValue.m_nation ) )
		SetImage( uiTShape, LoadSprite( MapUnitCityShapeList[recvValue.m_t_level].."_"..recvValue.m_t_nation ) )
		
		-- 名称
		SetText( uiName, recvValue.m_name )
		SetText( uiTName, recvValue.m_t_name )
		
		-- 坐标
		SetRichText( uiPos, F( 1266, recvValue.m_posx, recvValue.m_posy ), MailOnLinkClick )
		SetRichText( uiTPos, F( 1266, recvValue.m_t_posx, recvValue.m_t_posy ), MailOnLinkClick )
		
	
	-- 这个人是敌国玩家，那么都显示在右面
	else
		if recvValue.m_attack == 1 then
			SetFalse( uiArrowAttack )
			SetTrue( uiArrowDefense )
			SetText( uiFlagText, T(1265), Hex2Color(0x03de27ff) )
			SetText( uiTFlagText, T(1264), Hex2Color(0xffde00ff) )
			
			-- 守备兵力
			SetText( uiTotal, T(1261).."\n"..recvValue.m_total )
			SetText( uiTTotal, T(1260).."\n"..recvValue.m_t_total )
			
			-- 参与按钮
			SetFalse( uiWarn )
			SetFalse( uiAddAttackBtn )
			SetTrue( uiAddDefenseBtn )
			SetControlID( uiAddDefenseBtn, UIADDDEFENSEBTN_EVENT_BASE + #m_cache )
			
		else
			SetTrue( uiArrowAttack )
			SetFalse( uiArrowDefense )
			SetText( uiFlagText, T(1264), Hex2Color(0xffde00ff) )
			SetText( uiTFlagText, T(1265), Hex2Color(0x03de27ff) )
			
			-- 守备兵力
			SetText( uiTTotal, T(1259).."\n"..recvValue.m_total )
			SetText( uiTotal, T(1262).."\n"..recvValue.m_t_total )
			
			-- 参与按钮
			if recvValue.m_type == 1 then -- 闪电战
				SetFalse( uiAskHelpBtn )
				SetFalse( uiAddAttackBtn )
				SetTrue( uiWarn )
				SetText( uiWarn, T(1267) )
			else
				SetTrue( uiAddAttackBtn )
				SetControlID( uiAddAttackBtn, UIADDATTACKBTN_EVENT_BASE + #m_cache )
				SetFalse( uiWarn )
			end
			SetFalse( uiAddDefenseBtn )
		end
		
		-- 倒计时
		SetTimer( uiTimer, recvValue.m_stateduration-recvValue.m_statetime, recvValue.m_stateduration )
		
		-- 形象
		SetImage( uiTShape, LoadSprite( MapUnitCityShapeList[recvValue.m_level].."_"..recvValue.m_nation ) )
		SetImage( uiShape, LoadSprite( MapUnitCityShapeList[recvValue.m_t_level].."_"..recvValue.m_t_nation ) )
		
		-- 名称
		SetText( uiTName, recvValue.m_name )
		SetText( uiName, recvValue.m_t_name )
		
		-- 坐标
		SetRichText( uiTPos, F( 1266, recvValue.m_posx, recvValue.m_posy ), MailOnLinkClick )
		SetRichText( uiPos, F( 1266, recvValue.m_t_posx, recvValue.m_t_posy ), MailOnLinkClick )
		
	end
end

function MapArmyGroupDlgOver()
	
end

-- 更新
function MapArmyGroupDlgUpdate()
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	MapArmyGroupDlgShow( m_unit_index )
end

-- 清空
function MapArmyGroupDlgClear()
	local objs = {};
	for i = 0 ,m_uiContent.transform.childCount - 1 do
		table.insert( objs, m_uiContent.transform:GetChild(i).gameObject )
    end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "UIP_ArmyGroup(Clone)" then
			m_ObjectPool:Release( "UIP_ArmyGroup", obj );
		end
    end
end


-- 请求援助
function MapArmyGroupDlgAskHelp( index )
	local recvValue = m_cache[index]
	if recvValue == nil then
		return
	end
	system_askinfo( ASKINFO_CITYARMYGROUP, "", 1, recvValue.m_group_index, recvValue.m_group_id );
end

-- 加入攻击方
function MapArmyGroupDlgAddAttack( index )
	local recvValue = m_cache[index]
	if recvValue == nil then
		return
	end
	
	local unitRecvValue = WorldMap.m_nMapUnitList[m_unit_index];
	if unitRecvValue == nil then
		return
	end
	if unitRecvValue.m_type ~= MAPUNIT_TYPE_CITY then
		return
	end

	
	MapBattleDlgShow( unitRecvValue, ARMY_ACTION_GROUP_ATTACK, recvValue.m_group_index );
	MapBattleDlgID( recvValue.m_group_id )
	MapArmyGroupDlgClose()
end

-- 加入防守方
function MapArmyGroupDlgAddDefense( index )
	local recvValue = m_cache[index]
	if recvValue == nil then
		return
	end
	
	local unitRecvValue = WorldMap.m_nMapUnitList[m_unit_index];
	if unitRecvValue == nil then
		return
	end
	if unitRecvValue.m_type ~= MAPUNIT_TYPE_CITY then
		return
	end
	
	MapBattleDlgShow( unitRecvValue, ARMY_ACTION_GROUP_DEFENSE, recvValue.m_group_index );
	MapBattleDlgID( recvValue.m_group_id )
	MapArmyGroupDlgClose()
end

