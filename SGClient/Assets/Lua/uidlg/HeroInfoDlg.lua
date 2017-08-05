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

local m_CacheHeroCache = {}
local m_CacheHeroList = {}

local m_ObjectPool = nil;
local m_pCacheHero = nil;

-- 打开界面
function HeroInfoDlgOpen()
	m_Dlg = eye.uiManager:Open( "HeroInfoDlg" );
	m_DialogFrameMod = DialogFrameModOpen( m_Dlg, "武将", 3, HeroInfoDlgClose );
end

-- 隐藏界面
function HeroInfoDlgClose()
	if m_Dlg == nil then
		return;
	end
	local objs = {};
	for i = 0 ,m_uiContent.transform.childCount - 1 do
		table.insert( objs, m_uiContent.transform:GetChild(i).gameObject )
    end
	for k, v in pairs(objs) do
		local obj = v;
		m_ObjectPool:Release( "UIP_HeroHead", obj );
    end
	m_CacheHeroCache = {};
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
			
		-- 快速升级
		elseif nControlID == 1 then
		
		-- 武将洗髓
		elseif nControlID == 2 then
		
		-- 上阵
		elseif nControlID == 3 then
			HeroInfoDlgHeroUp();
		
		-- 神级突破
		elseif nControlID == 4 then
		
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

function HeroInfoDlgShow( pHero, up )
	m_pCacheHero = pHero;
	if pHero == nil or pHero.m_kind <= 0 then
		return
	end
	HeroInfoDlgOpen();
	local uiPolygonChart = m_uiAbilityArea.transform:GetComponent( typeof(UIPolygonChart) );
	uiPolygonChart.VerticesDistances[0] = 0.5
	uiPolygonChart.VerticesDistances[1] = 0.5
	uiPolygonChart.VerticesDistances[2] = 0.5
	
	SetImage( m_uiShape, HeroFaceSprite( pHero.m_kind ) )
	SetText( m_uiAttackBase, T(143).." "..pHero.m_attack_base );
	SetText( m_uiDefenseBase, T(144).." "..pHero.m_defense_base );
	SetText( m_uiTroopsBase, T(145).." "..pHero.m_troops_base );
	
	local total = pHero.m_attack_base + pHero.m_defense_base + pHero.m_troops_base;
	local total_wash = pHero.m_attack_wash + pHero.m_defense_wash + pHero.m_troops_wash;
	SetText( m_uiTotalGrowth, T(149)..":"..(total).."+"..(total_wash) );
	SetText( m_uiAttack,  T(146)..":"..pHero.m_attack );
	SetText( m_uiDefense,  T(147)..":"..pHero.m_defense )
	SetText( m_uiSoldier,  T(148)..":"..pHero.m_troops )
	
	
	-- 先放进临时缓存
    for offset = 0, 3, 1 do
        local pHero = GetHero().m_CityHero[offset];
        if pHero ~= nil and pHero.m_kind > 0 then
            table.insert(m_CacheHeroCache, offset, { m_kind = pHero.m_kind, m_color = pHero.m_color, m_level = pHero.m_level, m_corps = pHero.m_corps, m_offset = offset });
        end
    end
	
	-- 先放进临时缓存			
    for offset = 0, MAX_HERONUM-1, 1 do
        local pHero = GetHero().m_Hero[offset];
        if pHero ~= nil and pHero.m_kind > 0 then
            table.insert(m_CacheHeroCache, 100+offset, { m_kind = pHero.m_kind, m_color = pHero.m_color, m_level = pHero.m_level, m_corps = pHero.m_corps, m_offset = 100+offset });
        end
    end
	
	-- 创建对象
	for i, v in pairs( m_CacheHeroCache ) do
		local pHero = v;
        if pHero ~= nil and pHero.m_kind > 0 then
			local uiHeroObj = m_ObjectPool:Get( "UIP_HeroHead" );
			uiHeroObj.transform:SetParent( m_uiContent.transform );
				
			uiHeroObj:GetComponent("UIButton").controlID = 1000 + pHero.m_offset;
			local objs = uiHeroObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;

			local uiShape = objs[0];
			local uiColor = objs[1];
			local uiCorps = objs[2];
			local uiName = objs[3];
			
			SetImage( uiShape, HeroHeadSprite(pHero.m_kind) )
			SetImage( uiColor, ItemColorSprite(pHero.m_color) )
			SetImage( uiCorps, CorpsSprite(pHero.m_corps) )
			SetText( uiName, HeroNameLv( pHero.m_kind, pHero.m_level ) )
			m_CacheHeroList[pHero.m_offset] = uiHeroObj;
		end
	end
	
	-- 已经上阵的
	if up == true then
		SetTrue( m_uiUpgradeBtn );
		SetTrue( m_uiWashBtn );
		SetFalse( m_uiUpBtn );
		SetFalse( m_uiGoldBtn );
	else
		SetFalse( m_uiUpgradeBtn );
		SetFalse( m_uiWashBtn );
		SetTrue( m_uiUpBtn );
		SetFalse( m_uiGoldBtn );
	end
	
end

-- 上阵
function HeroInfoDlgHeroUp()
	if m_pCacheHero == nil or m_pCacheHero.m_kind <= 0 then
		return
	end
end
