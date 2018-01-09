-- 界面
local m_Dlg = nil;
local m_DialogFrameMod = nil
--------------------------------------------------------------
local m_uiIronNum = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiUIP_NationEquip = nil; --UnityEngine.GameObject
local m_uiMakeLayer = nil; --UnityEngine.GameObject
local m_uiRemakeLayer = nil; --UnityEngine.GameObject

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
		
		-- 打造界面关闭
		elseif nControlID == -2  then
			SetFalse( m_uiMakeLayer )
			
		-- 改造界面关闭
		elseif nControlID == -3  then
			SetFalse( m_uiRemakeLayer )	
		
		-- 打造	
		elseif nControlID > 100 and nControlID < 200 then
			NationEquipDlgMakeLayerShow( nControlID - 100 )
			
		-- 升级按钮
		elseif nControlID > 200 and nControlID < 300 then
			NationEquipDlgUpgrade( nControlID - 200 )
			
		-- 改造界面显示
		elseif nControlID > 300 and nControlID < 400 then
			NationEquipDlgRemakeLayerShow( nControlID - 300 )
		
		-- 加速按钮
		elseif nControlID > 400 and nControlID < 500 then
			NationEquipDlgSpeed( nControlID - 400 )
		
		-- 确定改造
		elseif nControlID > 500 and nControlID < 600 then
			NationEquipDlgRemake( nControlID - 500 )
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
	m_uiMakeLayer = objs[3];
	m_uiRemakeLayer = objs[4];
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
	SetFalse( m_uiMakeLayer )
	SetFalse( m_uiRemakeLayer )
	system_askinfo( ASKINFO_NATIONEQUIP, "", 0 )
end

-- 接收列表
-- m_count=0,m_list={m_neq_lv=0,m_neq_exp=0,m_neq_pro=0,m_neq_sec=0,m_neq_crit=0,m_neq_star=0,m_kind=0,[m_count]},
function NationEquipDlgListRecv( recvValue )
	m_recvValue = {}
	for i=1, recvValue.m_count do
		table.insert( m_recvValue, recvValue.m_list[i] )
	end
	NationEquipDlgSetList()
end

-- 接收单个
-- m_neq_lv=0,m_neq_exp=0,m_neq_pro=0,m_neq_sec=0,m_neq_crit=0,m_neq_star=0,m_kind=0,
function NationEquipDlgRecv( recvValue )
	m_recvValue[recvValue.m_kind] = recvValue;
	NationEquipDlgSetObj( recvValue.m_kind, recvValue )
end

-- 设置列表
function NationEquipDlgSetList()
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	for kind=1, #g_nation_equip do
		NationEquipDlgSetObj( kind, m_recvValue[kind] )
	end
end

-- 创建
function NationEquipDlgSetObj( kind, info )
	local uiObj = m_uiContent.transform:GetChild( kind-1 )
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiShape = objs[0] ;
	local uiStar = objs[1] ;
	local uiName = objs[2] ;
	local uiExp = objs[3] ;
	local uiExpProgress = objs[4] ;
	local uiLockShape = objs[5] ;
	local uiLockDesc = objs[6] ;
	local uiTimer = objs[7] ;
	local uiMakeCost = objs[8] ;
	local uiRemakeDesc = objs[9] ;
	local uiRemakeProgress = objs[10] ;
	local uiCost = objs[11] ;
	local uiCostIcon = objs[12] ;
	local uiCostNum = objs[13] ;
	local uiMakeBtn = objs[14] ;
	local uiUpgradeBtn = objs[15] ;
	local uiUpgradeCritBtn = objs[16] ;
	local uiRemakeBtn = objs[17] ;
	local uiSpeedBtn = objs[18] ;
	local uiGetIronBtn = objs[19] ;
	local EquipName = NationEquipName( kind )
	
	--判断此国器是否解锁
	if GetPlayer().m_level < g_nequip_open[kind].actorlevel then
		-- 等级未到未解锁
		SetTrue( uiLockShape )
		SetTrue( uiLockDesc )
		
		SetFalse( uiExp )
		SetFalse( uiExpProgress )
		SetFalse( uiTimer )
		SetFalse( uiMakeCost )
		SetFalse( uiRemakeDesc )
		SetFalse( uiRemakeProgress )
		SetFalse( uiCost )
		SetFalse( uiMakeBtn )
		SetFalse( uiUpgradeBtn )
		SetFalse( uiUpgradeCritBtn )
		SetFalse( uiRemakeBtn )
		SetFalse( uiSpeedBtn )
		SetFalse( uiGetIronBtn )
		
		SetText( uiName, EquipName, Hex2Color( 0x9B8869FF  ) )
		SetText( uiLockDesc, F(1744, g_nequip_open[kind].actorlevel), Hex2Color( 0x9B8869FF ) )
	else
		-- 等级已到，已解锁
		SetFalse( uiLockShape )
		SetFalse( uiLockDesc )
		
		-- 未打造
		if info.m_neq_lv == 0 then
			SetTrue( uiMakeCost )
			SetTrue( uiMakeBtn )
			
			SetFalse( uiExp )
			SetFalse( uiExpProgress )
			SetFalse( uiTimer )
			SetFalse( uiRemakeDesc )
			SetFalse( uiRemakeProgress )
			SetFalse( uiCost )
			SetFalse( uiUpgradeBtn )
			SetFalse( uiUpgradeCritBtn )
			SetFalse( uiRemakeBtn )
			SetFalse( uiSpeedBtn )
			SetFalse( uiGetIronBtn )
			
			SetImage( uiShape, NationEquipSprite(kind) )
			SetText( uiName, EquipName, Hex2Color( 0xD95DF4FF ) )
			SetText( uiMakeCost, T(121).."x"..g_nequip_open[kind].silver )
			SetControlID( uiMakeBtn, 100+kind )
		
		-- 已打造
		elseif info.m_neq_lv > 0 then
			SetImage( uiShape, NationEquipSprite(kind) )
			local AttrName = nation_equip_getabilityname( kind, info.m_neq_lv )
			SetText( uiName, "Lv."..info.m_neq_lv.."  "..EquipName.."  "..AttrName, Hex2Color( 0xD95DF4FF ) )
			
			-- 可升级
			if g_nation_equip[kind][info.m_neq_lv].remake_star >= 0 then
				SetTrue( uiExp )
				SetTrue( uiExpProgress )
				SetTrue( uiCost )
				SetTrue( uiUpgradeBtn )
				
				SetText( uiExp, F(1748, info.m_neq_exp) )
				SetProgress( uiExpProgress, info.m_neq_exp/100 );
				if GetPlayer().m_iron < g_nation_equip[kind][info.m_neq_lv].iron then 
					SetText( uiCostNum, g_nation_equip[kind][info.m_neq_lv].iron, Hex2Color( 0xE80017FF ) )
				else
					SetText( uiCostNum, g_nation_equip[kind][info.m_neq_lv].iron, Hex2Color( 0xECC244FF ) )
				end
				SetControlID( uiUpgradeBtn, 200+kind )
			
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

-- 打造国器页面
function NationEquipDlgMakeLayerShow( kind )
	if kind <= 0 then
		return;
	end
	
end

-- 打造国器
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

