-- 界面
local m_Dlg = nil;
local m_DialogFrameMod = nil
--------------------------------------------------------------
local m_uiIronNum = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiUIP_NationEquip = nil; --UnityEngine.GameObject
local m_uiAdvance = nil; --UnityEngine.GameObject

local m_recvValue = nil;

-- 打开界面
function NationEquipDlgOpen()
	m_Dlg = eye.uiManager:Open( "NationEquipDlg" );
	m_DialogFrameMod = DialogFrameModOpen( m_Dlg, T(558), HELP_NationEquipDlg, NationEquipDlgClose );
end

-- 隐藏界面
function NationEquipDlgClose()
	if m_Dlg == nil then
		return;
	end	
	DialogFrameModClose( m_DialogFrameMod );
	m_DialogFrameMod = nil;
	eye.uiManager:Close( "NationEquipDlg" );
	m_recvValue = nil
end

-- 删除界面
function NationEquipDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
	m_recvValue = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function NationEquipDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            NationEquipDlgClose();
			
		elseif nControlID > 100 and nControlID < 200 then	--打造按钮
			NationEquipDlgMake(nControlID - 100)
		elseif nControlID > 200 and nControlID < 300 then	--升级按钮
			NationEquipDlgUpgrade(nControlID - 200)
		elseif nControlID > 300 and nControlID < 400 then	--改造界面显示
			NationEquipDlgAdvanceShow(nControlID - 300)
		elseif nControlID > 400 and nControlID < 500 then	--加速按钮
			NationEquipDlgSpeed(nControlID - 400)
		elseif nControlID > 500 and nControlID < 600 then	--确定改造按钮
			NationEquipDlgAdvance(nControlID - 500)
		
		elseif nControlID == -2  then	--改造界面关闭按钮
			SetFalse(m_uiAdvance)
		
		end
	end
end

-- 载入时调用
function NationEquipDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiIronNum = objs[0];
	m_uiContent = objs[1];
	m_uiUIP_NationEquip = objs[2];
	m_uiAdvance = objs[3];
	
	-- 对象池
	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );
	m_ObjectPool:CreatePool("UIP_NationEquip", 6, 6, m_uiUIP_NationEquip);
end

-- 界面初始化时调用
function NationEquipDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function NationEquipDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function NationEquipDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function NationEquipDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function NationEquipDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function NationEquipDlgShow()
	NationEquipDlgOpen()
	NationEquipDlgSetIron()
	system_askinfo( ASKINFO_NATIONEQUIP, "", 0 )
end

-- 接收列表
-- m_count=0,m_list={m_neq_lv=0,m_neq_exp=0,m_neq_pro=0,m_neq_sec=0,m_kind=0,[m_count]},
function NationEquipDlgListRecv( recvValue )
	NationEquipDlgClear()
	m_recvValue = {}
	for i=1, recvValue.m_count do
		table.insert( m_recvValue, recvValue.m_list[i] )
	end
	
	for i=1, #g_nation_equip do
		local uiObj = m_ObjectPool:Get( "UIP_NationEquip" );
		uiObj.transform:SetParent( m_uiContent.transform );
		NationEquipDlgCreateEquip( i, uiObj, m_recvValue[i] )
	end
end

-- 接收单个
-- m_neq_lv=0,m_neq_exp=0,m_neq_pro=0,m_neq_sec=0,m_kind=0,
function NationEquipDlgRecv( recvValue )
	local uiObj = m_recvValue[recvValue.m_kind].m_uiObj;
	m_recvValue[recvValue.m_kind] = recvValue;
	NationEquipDlgCreateEquip( recvValue.m_kind, uiObj, recvValue )
end

function NationEquipDlgCreateEquip( i, uiObj, info )
	info.m_index = i
	info.m_uiObj = uiObj;
	
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiShape = objs[0] ;
	local uiStarLv = objs[1] ;
	local uiStarImage = objs[2] ;
	local uiLock = objs[3] ;
	local uiEquipDesc = objs[4] ;
	local uiProgressDesc = objs[5] ;
	local uiAdvanceTimer = objs[6] ;
	local uiProgressBar = objs[7] ;
	local uiUpgradeBtn = objs[8] ;
	local uiAdvanceBtn = objs[9] ;
	local uiSpeedBtn = objs[10] ;
	local uiCostIron = objs[11] ;
	local uiCostNum = objs[12] ;
	local uiMakeCostText = objs[13] ;
	local uiMakeBtn = objs[14] ;
	local uiAdvanceDescText = objs[15] ;
	local uiAdvanceProgressText = objs[16] ;
	local uiLockDesc = objs[17] ;
	local name = NationEquipName( i )
	
	--判断此国器是否解锁
	if GetPlayer().m_level < g_nequip_open[i].actorlevel then
		-- 未解锁
		SetTrue( uiLock )
		SetTrue( uiLockDesc)
		SetFalse( uiProgressDesc )
		SetFalse( uiProgressBar )
		SetFalse( uiUpgradeBtn )
		SetFalse( uiCostIron )
		SetFalse( uiAdvanceTimer )
		SetFalse( uiAdvanceBtn )
		SetFalse( uiSpeedBtn )
		SetFalse( uiMakeCostText )
		SetFalse( uiMakeBtn )
		SetFalse( uiAdvanceDescText )
		SetFalse( uiAdvanceProgressText )
		
		SetText( uiEquipDesc, name, Hex2Color( 0x9B8869FF  ))
		SetText( uiLockDesc, T(1744)..g_nequip_open[i].actorlevel..T(1747), Hex2Color( 0x9B8869FF ) )
	else
		SetFalse( uiLock )
		SetFalse( uiLockDesc )
		-- 已解锁未开启
		if info.m_neq_lv == 0 then
			SetTrue( uiMakeCostText )
			SetTrue( uiMakeBtn )
			SetFalse( uiProgressDesc )
			SetFalse( uiProgressBar )
			SetFalse( uiUpgradeBtn )
			SetFalse( uiCostIron )
			
			SetImage( uiShape, NationEquipSprite(i) )
			SetText( uiEquipDesc, name )
			SetText( uiMakeCostText, T(121).."x"..g_nequip_open[i].actorlevel )
			SetControlID( uiMakeBtn, 100+i )
		
		--已解锁已开启
		elseif info.m_neq_lv > 0 then
			SetImage( uiShape, NationEquipSprite(i) )
			local attrname = nation_equip_getabilityname( i, info.m_neq_lv )
			SetText( uiEquipDesc, "Lv."..info.m_neq_lv.."  "..name.."  "..attrname, Hex2Color( 0xD95DF4FF ) )
			
			-- 可升级
			if info.m_neq_lv < 150 then
				SetTrue( uiProgressDesc )
				SetText( uiProgressDesc, T(1748)..":"..info.m_neq_exp.."%" )
				SetTrue( uiProgressBar )
				SetProgress( uiProgressBar, info.m_neq_exp/100 );
				SetTrue( uiCostIron )
				SetTrue( uiUpgradeBtn )
				SetControlID( uiUpgradeBtn, 200+i )
				if GetPlayer().m_iron < g_nation_equip[i][info.m_neq_lv].iron then 
					SetText(uiCostIron.transform:Find("CostNum"), g_nation_equip[i][info.m_neq_lv].iron, Hex2Color( 0xE80017FF ) )
				else
					SetText(uiCostIron.transform:Find("CostNum"), g_nation_equip[i][info.m_neq_lv].iron, Hex2Color( 0xECC244FF ) )
				end
			
			--[[-- 需要改造
			elseif m_nation_equip[i].advanceprogress[1] ~= nil  then	--改造中
				SetFalse(uiCostIron)
				SetFalse(uiProgressDesc)
				SetTrue(uiAdvanceProgressText)
				SetText(uiAdvanceProgressText,T(1742)..m_nation_equip[i].advanceprogress[1].."/"..m_nation_equip[i].advanceprogress[2])
				if m_nation_equip[i].advanceprogress[1] == 0 and m_nation_equip[i].progress == 0 then	--尚未开始改造
					SetTrue(uiAdvanceDescText)
					local maxlv = m_nation_equip[i].level+10
					SetText(uiAdvanceDescText,T(1743)..maxlv)
					SetFalse(uiProgressBar)
					SetTrue(uiAdvanceBtn)
					SetControlID( uiAdvanceBtn, 300+i )
				else
					--已经在改造中
					SetTrue(uiAdvanceTimer)
					SetTimer( uiAdvanceTimer, m_nation_equip[i].time, m_nation_equip[i].tataltime, 1, T(1463) )
					SetTrue(uiProgressBar)
					SetTrue(uiSpeedBtn)
					SetControlID( uiSpeedBtn, 400+i )
				end--]]
			end
		end
	end
end

-- 设置当前镔铁数量
function NationEquipDlgSetIron()
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	SetText( m_uiIronNum, T(124)..":"..knum(GetPlayer().m_iron) )
end

-- 获取国器基础属性
function nation_equip_getabilityname( kind , level )
	if g_nation_equip[kind] == nil then
		return "";
	end
	local attack = g_nation_equip[kind][level].attack;
	local defense = g_nation_equip[kind][level].defense;
	local troops = g_nation_equip[kind][level].troops;
	local name = nil
	if attack > 0 then
		name = T(146).."+"..attack;
	elseif defense > 0 then
		name = T(147).."+"..defense;
	elseif troops > 0 then
		name = T(148).."+"..troops;
	end
	return name
end

-- 清空
function NationEquipDlgClear()
	local objs = {};
	for i = 0 ,m_uiContent.transform.childCount - 1 do
		table.insert( objs, m_uiContent.transform:GetChild(i).gameObject )
    end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "UIP_NationEquip(Clone)" then
			m_ObjectPool:Release( "UIP_NationEquip", obj );
		end
    end
end

-- 开启国器
function NationEquipDlgMake( kind )
	if kind <= 0 then
		return;
	end
	system_askinfo( ASKINFO_NATIONEQUIP, "", 1, kind );
end

--升级国器
function NationEquipDlgUpgrade( kind )
	if kind <= 0 then
		return;
	end
	local level = m_recvValue[kind].m_neq_lv
	if GetPlayer().m_iron < g_nation_equip[kind][level].iron then
		JumpRes( 4 )
		return
	end
	system_askinfo( ASKINFO_NATIONEQUIP, "", 2, kind );
end

--改造国器界面
function NationEquipDlgAdvanceShow(kind)
	if kind <= 0 then
		return;
	end
	SetTrue(m_uiAdvance)
	local objs = m_uiAdvance.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiShape1 = objs[0] ;
	local uiColor1 = objs[1] ;
	local uiStarImage1 = objs[2] ;
	local uiMaxLv1 = objs[3] ;
	local uiShape2 = objs[4] ;
	local uiColor2 = objs[5] ;
	local uiStarImage2 = objs[6] ;
	local uiMaxLv2 = objs[7] ;
	local uiCloseBtn = objs[8] ;
	local uiImageGroup = objs[9] ;
	local uiCostTime = objs[10] ;
	local uiChangeBtn = objs[11] ;
	local uiCostText = objs[12] ;
	SetImage(uiShape1, NationEquipSprite(kind) )
	SetImage(uiShape2, NationEquipSprite(kind) )
	uiChangeBtn.transform:GetComponent( typeof(UIButton) ).controlID = 500+kind
end

--改造加速
function NationEquipDlgSpeed( kind )
	if kind <= 0 then
		return;
	end
	--system_askinfo( ASKINFO_NATIONEQUIPUPGRADE, "", 0, kind );	
end

--改造国器
function NationEquipDlgAdvance(kind)
	if kind <= 0 then
		return;
	end
	--system_askinfo( ASKINFO_NATIONEQUIPAdvance, "", 0, kind );
end

