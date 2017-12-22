-- 界面
local m_Dlg = nil;

--------------------------------------------------------------
local m_uiIronNum = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiUIP_NationEquip = nil; --UnityEngine.GameObject
local m_uiAdvance = nil; --UnityEngine.GameObject


--临时服务器数据
local m_nation_equip={
[1] = {level=10,progress=15},
[2] = {level=100,progress=0,advanceprogress={0,3}},
[3] = {level=120,progress=20,advanceprogress={1,3},time=360,tataltime=1800},
[4] = nil,
[5] = nil,
[6] = nil,
}

-- 国器属性类型
CITY_ATTR_ABILITY_100 = 100	--所有武将攻击
CITY_ATTR_ABILITY_110 = 110	--所有武将防御
CITY_ATTR_ABILITY_120 = 120	--所有武将兵力


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
	
	eye.uiManager:Close( "NationEquipDlg" );
end

-- 删除界面
function NationEquipDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
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
	m_ObjectPool:CreatePool("UIP_NationEquip", 2, 2, m_uiUIP_NationEquip);
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
	NationEquipDlgClear()
end

function NationEquipDlgRecv( recvValue )
	SetText( m_uiIronNum, T(124)..":"..GetPlayer().m_iron )
	for i=1, #g_nation_equip do
		NationEquipDlgCreateEquip( i )
	end
end

function NationEquipDlgCreateEquip( i )
	local uiObj = m_ObjectPool:Get( "UIP_NationEquip" );
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
	
	uiObj.transform:SetParent( m_uiContent.transform );
	local playerlevel = GetPlayer().m_level
	local name = NationEquipName(i)
	--判断此国器是否解锁
	if playerlevel <  g_nation_equip[i][1].actorlevel then	--未解锁
		SetTrue(uiLock)
		SetText(uiEquipDesc,name,Hex2Color( 0x9B8869FF ))
		SetTrue(uiLockDesc)
		SetText(uiLockDesc,T(1744)..g_nation_equip[i][1].actorlevel..T(1747),Hex2Color( 0x9B8869FF ))
		SetFalse(uiProgressDesc)
		SetFalse(uiProgressBar)
		SetFalse(uiUpgradeBtn)
		SetFalse(uiCostIron)
		SetFalse(uiAdvanceTimer)
		SetFalse(uiAdvanceBtn)
		SetFalse(uiSpeedBtn)
		SetFalse(uiMakeCostText)
		SetFalse(uiMakeBtn)
		SetFalse(uiAdvanceDescText)
		SetFalse(uiAdvanceProgressText)
	else
		SetFalse(uiLock)
		SetFalse(uiLockDesc)
		SetTrue(uiEquipDesc)
		if m_nation_equip[i].level == 0 then--已解锁未打造
			SetImage(uiShape, NationEquipSprite(i) )
			SetText(uiEquipDesc,name)
			SetFalse(uiProgressDesc)
			SetTrue(uiMakeCostText)
			SetFalse(uiProgressBar)
			SetFalse(uiUpgradeBtn)
			SetFalse(uiCostIron)
			SetTrue(uiMakeBtn)
			uiMakeBtn.transform:GetComponent( typeof(UIButton) ).controlID = 100+i
	
		elseif m_nation_equip[i].level > 0 then	--已解锁已打造
			SetImage(uiShape, NationEquipSprite(i) )
			local attrname = nation_equip_getabilityname(i,m_nation_equip[i].level)
			SetText(uiEquipDesc,"Lv."..m_nation_equip[i].level.."  "..name.."  "..attrname,Hex2Color( 0xD95DF4FF ))
			if m_nation_equip[i].advanceprogress == nil  then		--升级中
				SetText(uiProgressDesc,T(1748)..":"..m_nation_equip[i].progress.."%")
				SetTrue(uiProgressBar)
				SetProgress( uiProgressBar, m_nation_equip[i].progress/100 );
				SetTrue(uiCostIron)
				SetTrue(uiUpgradeBtn)
				uiUpgradeBtn.transform:GetComponent( typeof(UIButton) ).controlID = 200+i
				if GetPlayer().m_iron < g_nation_equip[i][m_nation_equip[i].level].iron then 
					SetText(uiCostIron.transform:Find("CostNum"),g_nation_equip[i][m_nation_equip[i].level].iron,Hex2Color( 0xE80017FF ))
				end
			
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
					uiAdvanceBtn.transform:GetComponent( typeof(UIButton) ).controlID = 300+i
				else																				
					SetTrue(uiAdvanceTimer)	--已经在改造中
					SetTimer( uiAdvanceTimer, m_nation_equip[i].time, m_nation_equip[i].tataltime, 1, T(1463) )
					SetTrue(uiProgressBar)
					SetTrue(uiSpeedBtn)
					uiSpeedBtn.transform:GetComponent( typeof(UIButton) ).controlID = 400+i
				end
			end
		end
	end
end

-- 获取国器基础属性
function nation_equip_getabilityname( kind , level )
	if g_nation_equip[kind] == nil then
		return "";
	end
	local ability = g_nation_equip[kind][level].ability;
	local value = g_nation_equip[kind][level].value;
	local name = nil
	if ability == CITY_ATTR_ABILITY_100 then
		name = T(146).."+"..value;
	elseif ability == CITY_ATTR_ABILITY_110 then
		name = T(147).."+"..value;
	elseif ability == CITY_ATTR_ABILITY_120 then
		name = T(148).."+"..value;
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

--打造国器
function NationEquipDlgMake(kind)
	if kind <= 0 then
		return;
	end
	--system_askinfo( ASKINFO_NATIONEQUIPFORGING, "", 0, kind );
end

--升级国器
function NationEquipDlgUpgrade(kind)
	if kind <= 0 then
		return;
	end
	--system_askinfo( ASKINFO_NATIONEQUIPUPGRADE, "", 0, kind );
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
function NationEquipDlgSpeed(kind)
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

