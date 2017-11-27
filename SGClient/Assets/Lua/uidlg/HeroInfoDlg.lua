-- 界面
local m_Dlg = nil;
local m_DialogFrameMod = nil;

local m_uiShape = nil; --UnityEngine.GameObject
local m_uiEquip = {}; --UnityEngine.GameObject
local m_uiAbilityArea = nil; --UnityEngine.GameObject
local m_uiAttackBase = nil; --UnityEngine.GameObject
local m_uiDefenseBase = nil; --UnityEngine.GameObject
local m_uiTroopsBase = nil; --UnityEngine.GameObject
local m_uiTotalGrowth = nil; --UnityEngine.GameObject
local m_uiAttack = nil; --UnityEngine.GameObject
local m_uiDefense = nil; --UnityEngine.GameObject
local m_uiSoldier = nil; --UnityEngine.GameObject
local m_uiUpgradeBtn = nil; --UnityEngine.GameObject
local m_uiWashBtn = nil; --UnityEngine.GameObject
local m_uiHeroScroll = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiUIP_HeroHead = nil; --UnityEngine.GameObject
local m_uiUpBtn = nil; --UnityEngine.GameObject
local m_uiGoldBtn = nil; --UnityEngine.GameObject
local m_uiExpPanel = nil; --UnityEngine.GameObject
local m_uiSoldierPanel = nil; --UnityEngine.GameObject
local m_uiName = nil; --UnityEngine.GameObject
local m_uiEquipInfo = nil; --UnityEngine.GameObject
local m_uiAttackIncreaseIcon = nil; --UnityEngine.GameObject
local m_uiAttackIncrease = nil; --UnityEngine.GameObject
local m_uiDefenseIncreaseIcon = nil; --UnityEngine.GameObject
local m_uiDefenseIncrease = nil; --UnityEngine.GameObject

local m_CacheHeroCache = {}
local m_CacheHeroList = {}

local m_ObjectPool = nil;
local m_pCacheHero = nil;
local m_sys = 0;
local m_up = false
local m_MatchEquipList = {};

-- 打开界面
function HeroInfoDlgOpen()
	m_Dlg = eye.uiManager:Open( "HeroInfoDlg" );
	m_DialogFrameMod = DialogFrameModOpen( m_Dlg, T(550), 3, HeroInfoDlgClose );
end

-- 隐藏界面
function HeroInfoDlgClose()
	if m_Dlg == nil then
		return;
	end
	DialogFrameModClose( m_DialogFrameMod );
	m_DialogFrameMod = nil;
	eye.uiManager:Close( "HeroInfoDlg" );
end

-- 删除界面
function HeroInfoDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function HeroInfoDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            HeroInfoDlgClose();
		elseif nControlID == -2 then
			HeroInfoDlgSelectEquip( -1 )
			
		-- 快速升级
		elseif nControlID == 1 then
			HeroExpDlgShow( m_pCacheHero.m_kind )
			
		-- 武将洗髓
		elseif nControlID == 2 then
			HeroWashDlgShow( m_pCacheHero.m_kind )
		
		-- 上阵
		elseif nControlID == 3 then
			HeroInfoDlgHeroUp();
		
		-- 武将突破
		elseif nControlID == 4 then
			HeroColorupDlgShow( m_pCacheHero )
		
		-- 打造装备
		elseif nControlID == 5 then
			EquipForgingDlgShow();
			HeroInfoDlgClose();
			HeroDlgClose()
		
		-- 卸下装备
		elseif nControlID >= 10 and nControlID < 20 then
			HeroInfoDlgEquipDown( nControlID-10 )
		
		-- 洗练装备
		elseif nControlID >= 100 and nControlID < 110 then
			HeroInfoDlgEquipWash( nControlID-100 )
			
		-- 选择装备
		elseif nControlID >= 1000 and nControlID < 2000 then
			HeroInfoDlgSelectEquip( nControlID-1000 )
		
		-- 穿上
		elseif nControlID >= 2000 and nControlID < 3000 then
			HeroInfoDlgEquipUp( nControlID-2000 )
			
		-- 选择英雄
		elseif nControlID >= 10000 and nControlID < 20000 then
			HeroInfoDlgSelectHero( nControlID-10000 )
        end
	end
end

-- 载入时调用
function HeroInfoDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiShape = objs[0];
	m_uiEquip[0] = objs[1];
	m_uiEquip[1] = objs[2];
	m_uiEquip[2] = objs[3];
	m_uiEquip[3] = objs[4];
	m_uiEquip[4] = objs[5];
	m_uiEquip[5] = objs[6];
	m_uiAbilityArea = objs[7];
	m_uiAttackBase = objs[8];
	m_uiDefenseBase = objs[9];
	m_uiTroopsBase = objs[10];
	m_uiTotalGrowth = objs[11];
	m_uiAttack = objs[12];
	m_uiDefense = objs[13];
	m_uiSoldier = objs[14];
	m_uiUpgradeBtn = objs[15];
	m_uiWashBtn = objs[16];
	m_uiHeroScroll = objs[17];
	m_uiContent = objs[18];
	m_uiUIP_HeroHead = objs[19];
	m_uiUpBtn = objs[20];
	m_uiGoldBtn = objs[21];
	m_uiExpPanel = objs[22];
	m_uiSoldierPanel = objs[23];
	m_uiName = objs[24];
	m_uiEquipInfo = objs[25];
	m_uiAttackIncreaseIcon = objs[26];
	m_uiAttackIncrease = objs[27];
	m_uiDefenseIncreaseIcon = objs[28];
	m_uiDefenseIncrease = objs[29];

	-- 对象池
	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );
	m_ObjectPool:CreatePool("UIP_HeroHead", 4, 4, m_uiUIP_HeroHead);
end

-- 界面初始化时调用
function HeroInfoDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function HeroInfoDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function HeroInfoDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function HeroInfoDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function HeroInfoDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------

function HeroInfoDlgShow( sys, pHero, up )
	if pHero == nil or pHero.m_kind <= 0 then
		return
	end
	HeroInfoDlgOpen();
	HeroInfoDlgSelectEquip( -1 )
	HeroInfoDlgSet( sys, pHero, up )
end

function HeroInfoDlgSet( sys, pHero, up )
	m_pCacheHero = pHero;
	m_sys = sys;
	m_up = up
	HeroInfoDlgClear()
	
	-- 形象
	SetImage( m_uiShape, HeroFaceSprite( pHero.m_kind ) )
	
	-- 名称
	SetText( m_uiName, HeroNameLv(pHero.m_kind, pHero.m_level ) )
	
	-- 经验
	SetProgress( m_uiExpPanel.transform:Find("Progress"), pHero.m_exp/pHero.m_exp_max )
	SetText( m_uiExpPanel.transform:Find("Text"), knum(pHero.m_exp).."/"..knum(pHero.m_exp_max) )
	
	-- 兵力
	SetProgress( m_uiSoldierPanel.transform:Find("Progress"), pHero.m_soldiers/pHero.m_troops )
	SetText( m_uiSoldierPanel.transform:Find("Text"), knum(pHero.m_soldiers).."/"..knum(pHero.m_troops) )
	SetImage( m_uiSoldierPanel.transform:Find("Corps"), CorpsSprite( pHero.m_corps ) )
	
	-- 属性
	SetText( m_uiAttackBase, T(143).." "..(pHero.m_attack_base+pHero.m_attack_wash) );
	SetText( m_uiDefenseBase, T(144).." "..(pHero.m_defense_base+pHero.m_defense_wash) );
	SetText( m_uiTroopsBase, T(145).." "..(pHero.m_troops_base+pHero.m_troops_wash) );
	
	local total = pHero.m_attack_base + pHero.m_defense_base + pHero.m_troops_base;
	local total_wash = pHero.m_attack_wash + pHero.m_defense_wash + pHero.m_troops_wash;
	SetText( m_uiTotalGrowth, T(149)..":"..(total).."+"..(total_wash) );
	SetText( m_uiAttack,  T(146)..":"..pHero.m_attack );
	SetText( m_uiDefense,  T(147)..":"..pHero.m_defense )
	SetText( m_uiSoldier,  T(148)..":"..pHero.m_troops )
	
	-- 强攻
	if pHero.m_attack_increase > 0 then
		SetTrue( m_uiAttackIncreaseIcon )
		SetTrue( m_uiAttackIncrease )
		SetText( m_uiAttackIncrease, T(165).."+"..pHero.m_attack_increase );
	else
		SetFalse( m_uiAttackIncreaseIcon )
		SetFalse( m_uiAttackIncrease )
	end
	
	-- 强防
	if pHero.m_defense_increase > 0 then
		SetTrue( m_uiDefenseIncreaseIcon )
		SetTrue( m_uiDefenseIncrease )
		SetText( m_uiDefenseIncrease, T(166).."+"..pHero.m_defense_increase );
	else
		SetFalse( m_uiDefenseIncreaseIcon )
		SetFalse( m_uiDefenseIncrease )
	end
	
	SetFalse( m_uiAbilityArea )
	SetTrue( m_uiAbilityArea )
	-- 能力范围图
	local uiPolygonChart = m_uiAbilityArea.transform:GetComponent( typeof(UIPolygonChart) );
	uiPolygonChart.VerticesDistances[0] = pHero.m_attack_base/total
	uiPolygonChart.VerticesDistances[1] = pHero.m_defense_base/total
	uiPolygonChart.VerticesDistances[2] = pHero.m_troops_base/total
	
	-- 装备
	for i=0,5,1 do
		local objs = m_uiEquip[i].transform:GetComponent( typeof(Reference) ).relatedGameObject;
		local uiShape = objs[0];
		local uiColor = objs[1];
		local uiWash = objs[2];
		local uiAdd = objs[4];
		SetControlID( m_uiEquip[i], 1000+i )

		if pHero.m_Equip[i].m_kind > 0 then
			SetTrue( uiShape )
			SetTrue( uiColor )
			SetTrue( uiWash )
			SetFalse( uiAdd )
			SetImage( uiShape, EquipSprite(pHero.m_Equip[i].m_kind) )
			SetImage( uiColor, ItemColorSprite(equip_getcolor(pHero.m_Equip[i].m_kind)) )
			SetEquipWash( uiWash, pHero.m_Equip[i] );
		else
			SetTrue( uiShape )
			SetFalse( uiColor )
			SetFalse( uiWash )
			if GetEquip():GetCountBuyType( i+1 ) > 0 then
				SetTrue( uiAdd )
			else
				SetFalse( uiAdd )
			end
			SetImage( uiShape, EquipNormalSprite(i+1) )
		end
	end
	
	
	-- 先放进临时缓存
    for offset = 0, 3, 1 do
        local pHero = GetHero().m_CityHero[offset];
        if pHero ~= nil and pHero.m_kind > 0 then
            table.insert(m_CacheHeroCache, { m_kind = pHero.m_kind, m_color = pHero.m_color, m_level = pHero.m_level, m_corps = pHero.m_corps, m_offset = offset });
        end
    end
	
	-- 先放进临时缓存
	if sys == 1 then --1全放			
		for offset = 0, MAX_HERONUM-1, 1 do
			local pHero = GetHero().m_Hero[offset];
			if pHero ~= nil and pHero.m_kind > 0 then
				table.insert(m_CacheHeroCache, { m_kind = pHero.m_kind, m_color = pHero.m_color, m_level = pHero.m_level, m_corps = pHero.m_corps, m_offset = 1000+offset });
			end
		end
	end
	
	-- 创建对象
	--local tmp = nil
	for i=1, #m_CacheHeroCache, 1 do
		local pHero = m_CacheHeroCache[i];
        if pHero ~= nil and pHero.m_kind > 0 then
			local uiHeroObj = m_ObjectPool:Get( "UIP_HeroHead" );
			uiHeroObj.transform:SetParent( m_uiContent.transform );
				
			uiHeroObj:GetComponent("UIButton").controlID = 10000 + pHero.m_offset;
			local objs = uiHeroObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;

			local uiShape = objs[0];
			local uiColor = objs[1];
			local uiCorps = objs[2];
			local uiName = objs[3];
			local uiSelect = objs[4];
			
			if pHero.m_kind == m_pCacheHero.m_kind then
				SetTrue( uiSelect )
				--tmp = uiHeroObj;
			else
				SetFalse( uiSelect )
			end
		
			SetImage( uiShape, HeroHeadSprite(pHero.m_kind) )
			SetImage( uiColor, ItemColorSprite(pHero.m_color) )
			SetImage( uiCorps, CorpsSprite(pHero.m_corps) )
			SetText( uiName, HeroNameLv( pHero.m_kind, pHero.m_level ) )
			m_CacheHeroList[pHero.m_offset] = uiHeroObj;
		end
	end
	
	--local siblingIndex = tmp.transform:GetSiblingIndex();
    --m_uiHeroScroll.transform:GetComponent("UIScrollRect"):ScrollToBottom(siblingIndex);
	
	-- 已经上阵的
	if up == true then
		SetTrue( m_uiUpgradeBtn );
		SetFalse( m_uiUpBtn );
	else
		SetFalse( m_uiUpgradeBtn );
		SetTrue( m_uiUpBtn );
	end
	
	-- 洗髓
	if pHero.m_color < 1 then
		SetFalse( m_uiWashBtn );
	else
		SetTrue( m_uiWashBtn );
	end
	
	-- 武将突破
	if pHero.m_color < 2 or pHero.m_color > 4 then 
		SetFalse( m_uiGoldBtn );
	else
		SetTrue( m_uiGoldBtn );
	end
end

function HeroInfoDlgClear()
	local objs = {};
	for i = 0 ,m_uiContent.transform.childCount - 1 do
		table.insert( objs, m_uiContent.transform:GetChild(i).gameObject )
    end
	for k, v in pairs(objs) do
		local obj = v;
		m_ObjectPool:Release( "UIP_HeroHead", obj );
    end
	m_CacheHeroCache = {};
	m_CacheHeroList = {};
end

-- 选择英雄
function HeroInfoDlgSelectHero( herooffset )
	local uiHeroObj = m_CacheHeroList[herooffset];
	if uiHeroObj == nil then
		return
	end
	
	local objs = uiHeroObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiSelect = objs[4];
	
	for i=1, #m_CacheHeroCache, 1 do
		local pHero = m_CacheHeroCache[i];
		if pHero.m_offset == herooffset then	
			local pHero = GetHero():GetPtr( pHero.m_kind );
			if pHero == nil then
				return;
			end
			local up = true;
			if herooffset >= 1000 then
				up = false;
			end
			HeroInfoDlgSet( m_sys, pHero, up )
			break;
		end
	end
end

-- 选择装备
function HeroInfoDlgSelectEquip( offset )
	if offset < 0 then
		SetFalse( m_uiEquipInfo )
		return
	end
	SetTrue( m_uiEquipInfo );
	
	local kind = m_pCacheHero.m_Equip[offset].m_kind;
	local objs = m_uiEquipInfo.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiShape = objs[0];
	local uiColor = objs[1];
	local uiName = objs[2];
	local uiAbility = objs[3];
	local uiDesc = objs[4];
	local uiDownBtn = objs[5];
	local uiWashBtn = objs[6];
	local uiScroll = objs[7];
	local uiContent = objs[8];
	local uiUIP_Equip = objs[9];
	local uiForgingBtn = objs[10];
	local uiWash = objs[11];
	local uiWashLevel = objs[12];
	local uiScrollBack = objs[13];
	local uiEmpty = objs[14];
	
	SetControlID( uiDownBtn, 10 + offset );
	SetControlID( uiWashBtn, 100 + offset );
	-- 已经装备的信息
	if kind > 0 then
		SetTrue( uiShape )
		SetTrue( uiColor )
		SetTrue( uiName )
		SetTrue( uiAbility )
		SetFalse( uiDesc )
		SetTrue( uiDownBtn )
		SetTrue( uiWashBtn )
		SetTrue( uiWash )
		SetTrue( uiWashLevel )
		
		SetImage( uiShape, EquipSprite( kind ) )
		SetImage( uiColor, ItemColorSprite( equip_getcolor( kind ) ) )
		SetText( uiName, equip_getname( kind ), NameColor( equip_getcolor( kind ) ) );
		SetText( uiAbility, equip_getabilityname( kind ) );
		SetEquipWash( uiWash, m_pCacheHero.m_Equip[offset] );
		SetEquipWashLevel( uiWashLevel, m_pCacheHero.m_Equip[offset] );
	else
	
		SetTrue( uiShape )
		SetFalse( uiColor )
		SetFalse( uiName )
		SetFalse( uiAbility )
		SetTrue( uiDesc )
		SetFalse( uiWash )
		SetFalse( uiWashLevel )
		SetFalse( uiDownBtn )
		SetFalse( uiWashBtn )
		SetImage( uiShape, EquipNormalSprite(offset+1) )
		SetText( uiDesc, T(823+offset) )
	end
	
	
	-- 同部位装备列表
	m_MatchEquipList = GetEquip():GetEquipsByType( offset+1 )
	
	--排序
	table.sort( m_MatchEquipList, function( a, b )
		if equip_getcolor(a.m_kind) > equip_getcolor(b.m_kind) then
			return true
		else
			return false
		end
	end )
	-- 加入列表
	clearChild( uiContent.transform )
	for i=1, #m_MatchEquipList, 1 do
		local equipoffset = m_MatchEquipList[i].m_offset;
		local pEquip = GetEquip():GetAnyEquip( equipoffset );
		local uiEquip = GameObject.Instantiate( uiUIP_Equip );
		uiEquip.transform:SetParent( uiContent.transform );
        uiEquip.transform.localScale = Vector3.one;
        uiEquip:SetActive(true);
		
		local objs = uiEquip.transform:GetComponent( typeof(Reference) ).relatedGameObject;
		local uiShape = objs[0];
		local uiName = objs[1];
		local uiAbility = objs[2];
		local uiUpBtn = objs[3];
		local uiColor = objs[4];
		local uiWash = objs[5];
		local uiWashLevel = objs[6];
		SetControlID( uiUpBtn, 2000+equipoffset )
		SetImage( uiShape, EquipSprite( pEquip.m_kind ) )
		SetImage( uiColor, ItemColorSprite( equip_getcolor( pEquip.m_kind ) ) )
		SetText( uiName, equip_getname( pEquip.m_kind ), NameColor( equip_getcolor( pEquip.m_kind ) ) );
		SetText( uiAbility, equip_getabilityname( pEquip.m_kind ) );
		SetEquipWash( uiWash, pEquip );
		SetTrue( uiWash )
		SetEquipWashLevel( uiWashLevel, pEquip );
		SetTrue( uiWashLevel )
	end
	
    if #m_MatchEquipList == 0 then
		SetTrue( uiForgingBtn )
		SetFalse( uiScrollBack )
		SetTrue( uiEmpty )
	else
		SetFalse( uiForgingBtn )
		SetTrue( uiScrollBack )
		SetFalse( uiEmpty )
	end       
end

-- 穿上
function HeroInfoDlgEquipUp( offset )
	if m_pCacheHero.m_kind <= 0 then
		return
	end
	if offset < 0 then
		return
	end
	system_askinfo( ASKINFO_EQUIP, "", 2, m_pCacheHero.m_kind, offset );
	HeroInfoDlgSelectEquip( -1 )
end

-- 卸下
function HeroInfoDlgEquipDown( index )
	if m_pCacheHero.m_kind <= 0 then
		return
	end
	if index < 0 then
		return
	end
	system_askinfo( ASKINFO_EQUIP, "", 3, m_pCacheHero.m_kind, index );
	HeroInfoDlgSelectEquip( -1 )
end

-- 洗练
function HeroInfoDlgEquipWash( index )
	if m_pCacheHero.m_kind <= 0 then
		return
	end
	if index < 0 then
		return
	end
	EquipWashDlgShow()
	if m_up == true then
		EquipWashDlgSelectHero( m_pCacheHero.m_kind )
		EquipWashDlgSelectItem( m_pCacheHero.m_Equip[index].m_offset )
	end
end

-- 上阵
function HeroInfoDlgHeroUp()
	if m_pCacheHero == nil or m_pCacheHero.m_kind <= 0 then
		return
	end
	-- 检查是否有空位
	local heromax = 2 + GetPlayer().m_attr.m_hero_up_num;
	for i=0,heromax-1,1 do
		local pHero = GetHero().m_CityHero[i]
		if pHero and pHero.m_kind <= 0 then
			system_askinfo( ASKINFO_HERO, "", 3, 0, m_pCacheHero.m_kind, 0 );
			HeroInfoDlgClose();
			return
		end
	end

	-- 没空位打开替换界面
	HeroReplaceDlgShow( m_pCacheHero.m_kind )
end

function HeroInfoDlgUpdate( herokind )
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	if herokind ~= m_pCacheHero.m_kind then
		return;
	end
	local pHero = GetHero():GetPtr( herokind );
	if pHero == nil then
		return;
	end
	HeroInfoDlgSet( m_sys, pHero, m_up )
end