-- 界面
local m_Dlg = nil;
local m_DialogFrameMod = nil
--------------------------------------------------------------
local m_uiIronNum = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiUIP_NationEquip = nil; --UnityEngine.GameObject
local m_uiMakeLayer = nil; --UnityEngine.GameObject
local m_uiRemakeLayer = nil; --UnityEngine.GameObject
local m_uiPopCrit = nil; --UnityEngine.GameObject

local m_recvValue = nil;
local m_selectkind = 0;
local m_selectStoryid = 0;

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
		
		-- 打开副本
		elseif nControlID == 1 then
			SetFalse( m_uiMakeLayer )
			StoryDlgShow( m_selectStoryid )
			
		-- 打造
		elseif nControlID == 2 then
			NationEquipDlgMake()
			SetFalse( m_uiMakeLayer )
		
		-- 获取镔铁
		elseif nControlID == 3 then
			MaterialGetDlgShow( 123 )
			
		-- 打造页
		elseif nControlID > 100 and nControlID < 200 then
			NationEquipDlgMakeLayerShow( nControlID - 100 )
			
		-- 升级按钮
		elseif nControlID > 200 and nControlID < 300 then
			NationEquipDlgUpgrade( nControlID - 200, value )
			
		-- 改造界面显示
		elseif nControlID > 300 and nControlID < 400 then
			NationEquipDlgRemakeLayerShow( nControlID - 300 )
		
		-- 打造加速
		elseif nControlID > 400 and nControlID < 500 then
			NationEquipDlgMakeQuick( nControlID - 400 )
		
		-- 改造加速
		elseif nControlID > 500 and nControlID < 600 then
			NationEquipDlgRemakeQuick( nControlID - 500 )
			
		-- 确定改造
		elseif nControlID > 600 and nControlID < 700 then
			NationEquipDlgRemake( nControlID - 600 )
			
		end
	elseif nType == UI_EVENT_TWEENFINISH then
		if nControlID == 0 then
			SetFalse( m_uiPopCrit )
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
	m_uiPopCrit = objs[5];
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
	NationEquipDlgSetIron( 0 )
	SetFalse( m_uiMakeLayer )
	SetFalse( m_uiRemakeLayer )
	SetFalse( m_uiPopCrit )
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
	if m_recvValue == nil then
		return
	end
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
	local uiShape = objs[0]
	local uiStar = objs[1]
	local uiName = objs[2]
	local uiExp = objs[3]
	local uiExpProgress = objs[4]
	local uiLockShape = objs[5]
	local uiLockDesc = objs[6]
	local uiTimer = objs[7]
	local uiMakeCost = objs[8]
	local uiRemakeDesc = objs[9]
	local uiRemakeProgress = objs[10]
	local uiCost = objs[11]
	local uiCostIcon = objs[12]
	local uiCostNum = objs[13]
	local uiMakeBtn = objs[14]
	local uiUpgradeBtn = objs[15]
	local uiUpgradeCritBtn = objs[16]
	local uiRemakeBtn = objs[17]
	local uiSpeedBtn = objs[18]
	local uiGetIronBtn = objs[19]
	local uiFullBtn = objs[20];
	local uiTimerProgress = objs[21] ;
	local EquipName = NationEquipName( kind )
	
	--判断此国器是否解锁
	if GetPlayer().m_level < g_nequip_open[kind].actorlevel then
		-- 等级未到未解锁
		SetTrue( uiLockShape )
		SetTrue( uiLockDesc )
		
		SetFalse( uiStar )
		SetFalse( uiExp )
		SetFalse( uiExpProgress )
		SetFalse( uiTimer )
		SetFalse( uiTimerProgress )
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
		SetFalse( uiFullBtn )
		
		SetText( uiName, EquipName, Hex2Color( 0x9B8869FF  ) )
		SetText( uiLockDesc, F(1744, g_nequip_open[kind].actorlevel), Hex2Color( 0x9B8869FF ) )
	else
		-- 等级已到，已解锁
		SetFalse( uiLockShape )
		SetFalse( uiLockDesc )
		
		-- 未打造
		if info.m_neq_lv == 0 then
			SetFalse( uiStar )
			SetFalse( uiExp )
			SetFalse( uiExpProgress )
			SetFalse( uiRemakeDesc )
			SetFalse( uiRemakeProgress )
			SetFalse( uiCost )
			SetFalse( uiUpgradeBtn )
			SetFalse( uiUpgradeCritBtn )
			SetFalse( uiRemakeBtn )
			SetFalse( uiGetIronBtn )
			SetFalse( uiFullBtn )
			
			if info.m_neq_sec == 0 then
				SetTrue( uiMakeCost )
				SetTrue( uiMakeBtn )
				SetFalse( uiSpeedBtn )
				SetFalse( uiTimer )
				SetFalse( uiTimerProgress )
				
				SetText( uiMakeCost, T(121).."x"..g_nequip_open[kind].silver )
				SetControlID( uiMakeBtn, 100+kind )
			else
				--已经在打造中
				SetFalse( uiMakeCost )
				SetFalse( uiMakeBtn )
				SetTrue( uiSpeedBtn )
				SetTrue( uiTimer )
				SetTrue( uiTimerProgress )
				
				SetTimer( uiTimer, info.m_neq_sec, g_nequip_open[kind].sec, 1, T(1463) )
				SetControlID( uiSpeedBtn, 400+kind )
			end
				
			SetImage( uiShape, NationEquipSprite(kind) )
			SetText( uiName, EquipName, Hex2Color( 0xD95DF4FF ) )
			
			if IsGuiding() then
				if GetCurrentGuideType() == GUIDE_GUOQI then
					FindCmdTpye(uiMakeBtn.transform);
				end
			end
		
		-- 已经打造
		elseif info.m_neq_lv > 0 then
			SetImage( uiShape, NationEquipSprite(kind) )
			local AttrName = nation_equip_getabilityname( kind, info.m_neq_lv )
			SetText( uiName, "Lv."..info.m_neq_lv.."  "..EquipName.."  "..AttrName, Hex2Color( 0xD95DF4FF ) )
			NationEquipSetStar( uiStar, info.m_neq_star )
	
			-- 已经满级
			if g_nation_equip[kind][info.m_neq_lv].remake_star == -1 then
				SetTrue( uiFullBtn )
				
				SetFalse( uiExp )
				SetFalse( uiExpProgress )
				SetFalse( uiTimer )
				SetFalse( uiTimerProgress )
				SetFalse( uiMakeCost )
				SetFalse( uiMakeBtn )
				SetFalse( uiRemakeDesc )
				SetFalse( uiRemakeProgress )
				SetFalse( uiCost )
				SetFalse( uiUpgradeBtn )
				SetFalse( uiUpgradeCritBtn )
				SetFalse( uiRemakeBtn )
				SetFalse( uiSpeedBtn )
				SetFalse( uiGetIronBtn )
			
			-- 可升级	
			elseif g_nation_equip[kind][info.m_neq_lv].remake_star == 0 or info.m_neq_star >= g_nation_equip[kind][info.m_neq_lv].remake_star then
				SetTrue( uiExp )
				SetTrue( uiExpProgress )
				SetTrue( uiCost )
				
				SetFalse( uiTimer )
				SetFalse( uiTimerProgress )
				SetFalse( uiMakeCost )
				SetFalse( uiMakeBtn )
				SetFalse( uiRemakeDesc )
				SetFalse( uiRemakeProgress )
				SetFalse( uiRemakeBtn )
				SetFalse( uiSpeedBtn )
				SetFalse( uiGetIronBtn )
				
				if info.m_neq_crit > 0 then
					SetTrue( uiUpgradeCritBtn )
					SetFalse( uiUpgradeBtn )
					SetText( uiUpgradeCritBtn.transform:Find("Back/Text"), F(1752,info.m_neq_crit) )
					SetControlID( uiUpgradeCritBtn, 200+kind )
				else
					SetTrue( uiUpgradeBtn )
					SetFalse( uiUpgradeCritBtn )
					SetControlID( uiUpgradeBtn, 200+kind )
				end
				
				SetText( uiExp, F(1748, info.m_neq_exp) )
				SetProgress( uiExpProgress, info.m_neq_exp/100 );
				if GetPlayer().m_iron < g_nation_equip[kind][info.m_neq_lv].iron then 
					SetText( uiCostNum, g_nation_equip[kind][info.m_neq_lv].iron, Hex2Color( 0xE80017FF ) )
					SetFalse( uiUpgradeBtn )
					SetFalse( uiUpgradeCritBtn )
					SetTrue( uiGetIronBtn )
					SetControlID( uiGetIronBtn, 3 )
				else
					SetText( uiCostNum, g_nation_equip[kind][info.m_neq_lv].iron, Hex2Color( 0xECC244FF ) )
				end
			
			-- 需要改造
			else
				SetTrue( uiRemakeProgress )
				
				SetFalse( uiExp )
				SetFalse( uiExpProgress )
				SetFalse( uiMakeCost )
				SetFalse( uiMakeBtn )
				SetFalse( uiCost )
				SetFalse( uiUpgradeBtn )
				SetFalse( uiUpgradeCritBtn )
				SetFalse( uiSpeedBtn )
				SetFalse( uiGetIronBtn )
				
				local remake_star = g_nation_equip[kind][info.m_neq_lv].remake_star
				SetText( uiRemakeProgress, T(1742)..info.m_neq_pro.."/"..g_nequip_remake[kind][remake_star].progress )
				
				-- 尚未开始改造
				if info.m_neq_sec == 0 then
					SetTrue( uiRemakeDesc )
					SetTrue( uiRemakeBtn )
					SetFalse( uiSpeedBtn )
					SetFalse( uiTimer )
					SetFalse( uiTimerProgress )
					
					SetText( uiRemakeDesc, F(1743,g_nequip_remake[kind][remake_star].maxlevel) )
					SetControlID( uiRemakeBtn, 300+kind )
				else
					--已经在改造中
					SetFalse( uiRemakeDesc )
					SetFalse( uiRemakeBtn )
					SetTrue( uiSpeedBtn )
					SetTrue( uiTimer )
					SetTrue( uiTimerProgress )
					
					SetTimer( uiTimer, info.m_neq_sec, g_nequip_remake[kind][remake_star].sec, 1, T(1463) )
					SetControlID( uiSpeedBtn, 500+kind )
				end
			end
		end
	end
end

-- 设置当前镔铁数量
function NationEquipDlgSetIron( update )
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	SetText( m_uiIronNum, T(124)..":"..knum(GetPlayer().m_iron) )
	if update == 1 then
		for kind=1, 6, 1 do
			local uiObj = m_uiContent.transform:GetChild( kind-1 )
			local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
			local uiCostNum = objs[13] ;
			local uiUpgradeBtn = objs[15];
			local uiUpgradeCritBtn = objs[16];
			local uiGetIronBtn = objs[19];
			local info = m_recvValue[kind];
			if info.m_neq_lv > 0 then
				if g_nation_equip[kind][info.m_neq_lv].remake_star == 0 or info.m_neq_star >= g_nation_equip[kind][info.m_neq_lv].remake_star then
					if GetPlayer().m_iron < g_nation_equip[kind][info.m_neq_lv].iron then 
						SetText( uiCostNum, g_nation_equip[kind][info.m_neq_lv].iron, Hex2Color( 0xE80017FF ) )
						SetFalse( uiUpgradeBtn )
						SetFalse( uiUpgradeCritBtn )
						SetTrue( uiGetIronBtn )
						SetControlID( uiGetIronBtn, 3 )
					else
						SetText( uiCostNum, g_nation_equip[kind][info.m_neq_lv].iron, Hex2Color( 0xECC244FF ) )
					end
				end
			end
		end
	end
end

-- 设置国器改造星级
function NationEquipSetStar( uiStar, star )
	for i=1, 5, 1 do
		local uiLevel = uiStar.transform:GetChild(i-1).gameObject;
		if i <= star then
			SetTrue( uiLevel )
			has = 1;
		else	
			SetFalse( uiLevel )
		end
	end
	if has == 1 then
		SetTrue( uiStar )
	else
		SetFalse( uiStar )
	end
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
	m_selectkind = kind
	m_selectStoryid = 0;
	SetTrue( m_uiMakeLayer )
	
	local objs = m_uiMakeLayer.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiShape = objs[0];
	local uiName = objs[1];
	local uiDesc = objs[2];
	local uiStoryWarn = objs[3];
	local uiGetIronBtn = objs[4];
	local uiMakeBtn = objs[5];
	
	local info = m_recvValue[kind];
	SetImage( uiShape, NationEquipSprite( kind ) )
	SetText( uiName, NationEquipName( kind ) )
	
	local itemnum = GetItem():GetCount( g_nequip_open[kind].itemkind )
	SetText( uiDesc, F( 1753, itemnum, g_nequip_open[kind].itemnum ) )
	
	if itemnum < g_nequip_open[kind].itemnum then
		SetFalse( uiMakeBtn ) 
		if GetPlayer().m_storyid >= g_nequip_open[kind].storyid then
			SetTrue( uiGetIronBtn ) 
			SetFalse( uiStoryWarn ) 
			m_selectStoryid = g_nequip_open[kind].storyid;
		else
			SetTrue( uiStoryWarn ) 
			SetFalse( uiGetIronBtn )
			SetText( uiStoryWarn, F( 1756,   StoryChapterName( g_story[g_nequip_open[kind].storyid].chapter ), NationEquipName( kind ) ) ) 
		end
	else
		SetTrue( uiMakeBtn ) 
		SetFalse( uiStoryWarn ) 
		SetFalse( uiGetIronBtn ) 
	end
	
	if kind == 1 then
		if IsGuiding() then
			if GetCurrentGuideType() == GUIDE_GUOQI then
				GuideNext();
				FindCmdTpye(uiMakeBtn.transform);
			end
		end
	end
end

-- 打造国器
function NationEquipDlgMake()
	if m_selectkind <= 0 then
		return;
	end
	local info = m_recvValue[m_selectkind];
	if GetPlayer().m_silver < g_nequip_open[m_selectkind].silver then
		JumpRes( 1,g_nequip_open[m_selectkind].silver-GetPlayer().m_silver )
		return
	end
	if IsGuiding() then
		if GetCurrentGuideType() == GUIDE_DAZAO then
			GuideNext();
		end
	end
	system_askinfo( ASKINFO_NATIONEQUIP, "", 1, m_selectkind );
end

-- 打造加速
function NationEquipDlgMakeQuick( kind )
	if kind <= 0 then
		return;
	end
	local uiObj = m_uiContent.transform:GetChild( kind-1 )
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiTimer = objs[7] ;
	local left = uiTimer.transform:GetComponent( typeof(UITextTimeCountdown) ).LeftTime
	local min = math.floor(left/60) + 1
	local token = math.ceil( min*global.nequip_make_quick_token )
	MsgBox( F(1754,token), function()
		system_askinfo( ASKINFO_NATIONEQUIP, "", 4, kind, 0 );
	end )
end

-- 升级国器
function NationEquipDlgUpgrade( kind, uiButton )
	if kind <= 0 then
		return;
	end
	local level = m_recvValue[kind].m_neq_lv
	if GetPlayer().m_iron < g_nation_equip[kind][level].iron then
		JumpRes( 4,g_nation_equip[kind][level].iron-GetPlayer().m_iron )
		return
	end
	if level >= GetPlayer().m_level then
		AlertMsg( T(1767) )
		return
	end
	system_askinfo( ASKINFO_NATIONEQUIP, "", 2, kind );
	
	if m_recvValue[kind].m_neq_crit == 2 then
		pop( "<color=#25c9ffff>"..F( 1771, g_nation_equip[kind][level].exp * m_recvValue[kind].m_neq_crit ).."</color>" )
		NationEquipDlgPopCritShow( uiButton, m_recvValue[kind].m_neq_crit, Hex2Color(0x25c9ffff) )
		
	elseif m_recvValue[kind].m_neq_crit == 4 then
		pop( "<color=#03de27ff>"..F( 1771, g_nation_equip[kind][level].exp * m_recvValue[kind].m_neq_crit ).."</color>" )
		NationEquipDlgPopCritShow( uiButton, m_recvValue[kind].m_neq_crit, Hex2Color(0x03de27ff) )
		
	elseif m_recvValue[kind].m_neq_crit == 7 then
		pop( "<color=#ffde00ff>"..F( 1771, g_nation_equip[kind][level].exp * m_recvValue[kind].m_neq_crit ).."</color>" )
		NationEquipDlgPopCritShow( uiButton, m_recvValue[kind].m_neq_crit, Hex2Color(0xffde00ff) )
		
	elseif m_recvValue[kind].m_neq_crit == 10 then
		pop( "<color=#e80017ff>"..F( 1771, g_nation_equip[kind][level].exp * m_recvValue[kind].m_neq_crit ).."</color>" )
		NationEquipDlgPopCritShow( uiButton, m_recvValue[kind].m_neq_crit, Hex2Color(0xe80017ff) )
		
	else
		pop( "<color=#f7f3bbff>"..F( 1771, g_nation_equip[kind][level].exp ).."</color>" )
	end

end

-- 显示暴击
function NationEquipDlgPopCritShow( uiButton, crit, color )
	SetTrue( m_uiPopCrit )
	m_uiPopCrit.transform:SetParent( uiButton.transform )
	m_uiPopCrit.transform.anchoredPosition = Vector2( -uiButton.transform.anchoredPosition.x, uiButton.transform.anchoredPosition.y+60 )
	m_uiPopCrit.transform:SetParent( m_Dlg.transform )
	SetText( m_uiPopCrit.transform:Find("Text"), F(1752, crit), color )
end

-- 改造国器界面
function NationEquipDlgRemakeLayerShow( kind )
	if kind <= 0 then
		return;
	end
	local info = m_recvValue[kind];
	local remake_star = g_nation_equip[kind][info.m_neq_lv].remake_star
	local maxlevel = g_nequip_remake[kind][remake_star].maxlevel
	local progress = g_nequip_remake[kind][remake_star].progress
	
	SetTrue( m_uiRemakeLayer )
	local objs = m_uiRemakeLayer.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiFront = objs[0];
	local uiLater = objs[1];
	local uiProgressText = objs[2];
	local uiProgress = objs[3];
	local uiCostTime = objs[4];
	local uiCostText = objs[5];
	local uiRemakebtn = objs[6];
	
	SetImage( uiFront.transform:Find("Equip/Shape"), NationEquipSprite(kind) )
	SetImage( uiLater.transform:Find("Equip/Shape"), NationEquipSprite(kind) )
	NationEquipSetStar( uiFront.transform:Find("Star"), info.m_neq_star )
	NationEquipSetStar( uiLater.transform:Find("Star"), info.m_neq_star+1 )
	SetText( uiFront.transform:Find("MaxLv"), T(1749)..info.m_neq_lv )
	SetText( uiLater.transform:Find("MaxLv"), T(1749)..maxlevel )
	SetText( uiProgressText, F(1746,info.m_neq_pro, progress) )
	for i=0, 5, 1 do
		local uiObj = uiProgress.transform:GetChild( i )
		if i < progress then
			SetTrue( uiObj )
			if i < info.m_neq_pro then
				SetTrue( uiObj.transform:Find("Image") )
			else
				SetFalse( uiObj.transform:Find("Image") )
			end
		else
			SetFalse( uiObj )
		end
	end
	SetText( uiCostTime, secnum( g_nequip_remake[kind][remake_star].sec ) )
	SetText( uiCostText, T(121).." "..knum( g_nequip_remake[kind][remake_star].silver ) )
	SetControlID( uiRemakebtn, 600+kind )
end


-- 改造国器
function NationEquipDlgRemake( kind )
	if kind <= 0 then
		return;
	end
	SetFalse( m_uiRemakeLayer )
	local info = m_recvValue[kind];
	local remake_star = g_nation_equip[kind][info.m_neq_lv].remake_star
	if GetPlayer().m_silver < g_nequip_remake[kind][remake_star].silver then
		JumpRes( 1, g_nequip_remake[kind][remake_star].silver-GetPlayer().m_silver )
		return
	end
	system_askinfo( ASKINFO_NATIONEQUIP, "", 3, kind );
end

-- 改造加速
function NationEquipDlgRemakeQuick( kind )
	if kind <= 0 then
		return;
	end
	local uiObj = m_uiContent.transform:GetChild( kind-1 )
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiTimer = objs[7] ;
	local left = uiTimer.transform:GetComponent( typeof(UITextTimeCountdown) ).LeftTime
	local min = math.floor(left/60) + 1
	local token = math.ceil( min*global.nequip_remake_quick_token )
	MsgBox( F(1755,token), function()
		system_askinfo( ASKINFO_NATIONEQUIP, "", 4, kind, 1 );
	end )
end
