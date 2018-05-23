-- 界面
local m_Dlg = nil;
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiHeroScroll = nil; --UnityEngine.GameObject
local m_uiFreeTitle = nil; --UnityEngine.GameObject
local m_uiNum = nil; --UnityEngine.GameObject
local m_uiTokenTitle = nil; --UnityEngine.GameObject
local m_uiShape = nil; --UnityEngine.GameObject
local m_uiColor = nil; --UnityEngine.GameObject
local m_uiCorps = nil; --UnityEngine.GameObject
local m_uiGrid = nil; --UnityEngine.GameObject
local m_uiUIP_HeroHead = nil; --UnityEngine.GameObject
local m_uiTimer = nil; --UnityEngine.GameObject
local m_uiColorBack = nil; --UnityEngine.GameObject
local m_uiType = nil; --UnityEngine.GameObject
local m_uiToken = nil; --UnityEngine.GameObject

local m_ObjectPool = nil;

local m_upHeroCache = {}	
local m_CacheHeroCache = {};
local m_CacheHeroList = {}
local m_recvValue = nil;
local m_herokind = 0;
local m_wash_toggle = false;
local m_path = 0;

-- 打开界面
function HeroWashDlgOpen()
	m_Dlg = eye.uiManager:Open( "HeroWashDlg" );
end

-- 隐藏界面
function HeroWashDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "HeroWashDlg" );
end

-- 删除界面
function HeroWashDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function HeroWashDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            HeroWashDlgClose();
			
		-- 免费洗髓
		elseif nControlID == 1 then
			HeroWashDlgFree()
		
		-- 至尊洗髓
		elseif nControlID == 2 then
			HeroWashDlgToken()
		
		-- 选择武将
		elseif nControlID > 1000 and nControlID < 2000 then
			HeroWashDlgSelectHero( nControlID - 1000 )
        end
	elseif nType == UI_EVENT_TIMECOUNTEND then
		if nControlID == 1 then
			system_askinfo( ASKINFO_HERO, "", 4 );
		end	
	end
end

-- 载入时调用
function HeroWashDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiContent = objs[0];
	m_uiHeroScroll = objs[1];
	m_uiFreeTitle = objs[2];
	m_uiNum = objs[3];
	m_uiTokenTitle = objs[4];
	m_uiShape = objs[5];
	m_uiColor = objs[6];
	m_uiCorps = objs[7];
	m_uiGrid = objs[8];
	m_uiUIP_HeroHead = objs[9];
	m_uiTimer = objs[10];
	m_uiColorBack = objs[11];
	m_uiType = objs[12];
	m_uiToken = objs[13];

	SetFalse( m_uiNum );
	SetFalse( m_uiTimer );
	
	-- 对象池
	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );
	m_ObjectPool:CreatePool("UIP_HeroHead", 4, 4, m_uiUIP_HeroHead);
end

-- 界面初始化时调用
function HeroWashDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function HeroWashDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function HeroWashDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function HeroWashDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function HeroWashDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function HeroWashDlgShow( kind, path )
	m_path = path
	HeroWashDlgOpen()
	system_askinfo( ASKINFO_HERO, "", 4 );
	HeroWashDlgSetHero()
	HeroWashDlgSelectHero( kind )
	SetText( m_uiToken, global.hero_wash_token )
end

-- 英雄
function HeroWashDlgSetHero()
	HeroWashDlgClear()
	local baseoffset = -1; 
	if m_path == HEROLIST_PATH_HERO_GATHER then -- 财赋署武将
		baseoffset = 4
	elseif m_path == HEROLIST_PATH_HERO_GUARD then -- 御林卫武将
		baseoffset = 8		
	end
					
	if baseoffset >= 0 then
		for offset = 0+baseoffset, 3+baseoffset, 1 do	
			local pHero = GetHero().m_CityHero[offset];
			if pHero ~= nil and pHero.m_kind > 0 and pHero.m_color > 0 then
				table.insert(m_upHeroCache, { m_kind = pHero.m_kind, m_pHero = pHero });
			end
		end
	else
		-- 主将进入临时缓存
		for offset = 0, 3, 1 do	
			local pHero = GetHero().m_CityHero[offset];
			if pHero ~= nil and pHero.m_kind > 0 and pHero.m_color > 0 then
				table.insert(m_upHeroCache, { m_kind = pHero.m_kind, m_pHero = pHero });
			end
		end
		
		-- 聚贤阁进临时缓存			
		for offset = 0, MAX_HERONUM-1, 1 do
			local pHero = GetHero().m_Hero[offset];
			if pHero ~= nil and pHero.m_kind > 0 and pHero.m_color > 0 then
				if GetHero():IsCanUse( pHero.m_kind ) == true then
					local base = pHero.m_attack_base+pHero.m_defense_base+pHero.m_troops_base;
					local wash = pHero.m_attack_wash+pHero.m_defense_wash+pHero.m_troops_wash;
					table.insert(m_CacheHeroCache, { m_kind = pHero.m_kind, m_pHero = pHero, total = base+wash });
				end
			end
		end
		
		-- 当前排序类型
		local rankType = HeroListDlgGetRankType()
		if rankType == 1 then
			table.sort( m_CacheHeroCache, HeroWashDlgQualtiySort );
		elseif rankType == 2 then
			table.sort( m_CacheHeroCache, HeroWashDlgTalentSort );
		elseif rankType == 3 then
			table.sort( m_CacheHeroCache, HeroWashDlgLevelSort );
		end
	end	
	
	local num = #m_upHeroCache+#m_CacheHeroCache;
	for i=1, num, 1 do
		local pHero = nil
		if i <= #m_upHeroCache then
			pHero = m_upHeroCache[i].m_pHero;
		else
			pHero = m_CacheHeroCache[i-#m_upHeroCache].m_pHero;
		end
		local uiHeroObj = m_ObjectPool:Get( "UIP_HeroHead" );
		uiHeroObj.transform:SetParent( m_uiContent.transform );
		SetTrue( uiHeroObj )
		
		SetControlID( uiHeroObj, 1000+pHero.m_kind )
		local objs = uiHeroObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
		local uiShape = objs[0];
		local uiColor = objs[1];
		local uiCorps = objs[2];
		local uiName = objs[3];
		local uiSelect = objs[4];
		local uiType = objs[5];
		SetTrue( uiShape )
		SetTrue( uiColor )
		SetTrue( uiCorps )
		SetTrue( uiName )
		SetFalse( uiSelect )
		
		SetImage( uiShape, HeroHeadSprite( pHero.m_kind )  );
		SetImage( uiColor,  ItemColorSprite( pHero.m_color )  );
		SetImage( uiCorps,  CorpsSprite( pHero.m_corps )  );
		SetText( uiName, HeroNameLv( pHero.m_kind, pHero.m_level ) );
		
		local only = GetHero():IsNationHeroOnly( pHero.m_kind )
		if only == true and pHero.m_god == 1 then
			SetTrue( uiType )
			SetText( uiType, T(2359) )
		elseif only == true then
			SetTrue( uiType )
			SetText( uiType, T(2357) )
		elseif pHero.m_god == 1 then
			SetTrue( uiType )
			SetText( uiType, T(2358) )
		else
			SetFalse( uiType )
		end
		m_CacheHeroList[pHero.m_kind] = uiHeroObj;	
	end
end

-- 选择英雄
function HeroWashDlgSelectHero( kind )
	m_herokind = kind
	
	local num = #m_upHeroCache+#m_CacheHeroCache;
	for i=1, num, 1 do
		local pHero = nil
		if i <= #m_upHeroCache then
			pHero = m_upHeroCache[i].m_pHero;
		else
			pHero = m_CacheHeroCache[i-#m_upHeroCache].m_pHero;
		end
		local uiHeroObj = m_CacheHeroList[pHero.m_kind];
		local objs = uiHeroObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
		local uiSelect = objs[4];
	
		if pHero.m_kind == kind then	
			SetTrue( uiSelect )
			SetImage( m_uiShape, HeroFaceSprite( pHero.m_kind )  );
			--SetImage( m_uiColor,  ItemColorSprite( pHero.m_color )  );
			SetImage( m_uiCorps,  CorpsSprite( pHero.m_corps )  );
			SetImage( m_uiColorBack, HeroColorSprite( pHero.m_color ) )
			
			local only = GetHero():IsNationHeroOnly( pHero.m_kind )
			if only == true and pHero.m_god == 1 then
				SetTrue( m_uiType )
				SetText( m_uiType, T(2359) )
			elseif only == true then
				SetTrue( m_uiType )
				SetText( m_uiType, T(2357) )
			elseif pHero.m_god == 1 then
				SetTrue( m_uiType )
				SetText( m_uiType, T(2358) )
			else
				SetFalse( m_uiType )
			end
		
			local config = g_heroinfo[pHero.m_kind][pHero.m_color];
			
			-- 总资质
			local total = pHero.m_attack_base + pHero.m_defense_base + pHero.m_troops_base;
			local total_wash = pHero.m_attack_wash + pHero.m_defense_wash + pHero.m_troops_wash;
			
			local attrname = T(149).."：<color=#f7f3bbff>{0}</color> +<color=#03DE27FF>{1}</color>"
			local uiTotalGrowth = m_uiGrid.transform:GetChild(0).gameObject;
			SetText( uiTotalGrowth.transform:Find("Text"), Utils.StringFormat( attrname, total, total_wash ) )
			SetProgress( uiTotalGrowth, total_wash/config.total_wash )
			if total_wash >= config.total_wash then
				SetTrue( uiTotalGrowth.transform:Find("Full") )
			else
				SetFalse( uiTotalGrowth.transform:Find("Full") )
			end
			
			-- 攻
			local uiAttackBase = m_uiGrid.transform:GetChild(1).gameObject;
			SetText( uiAttackBase.transform:Find("Text"), F(138, (pHero.m_attack_base+pHero.m_attack_wash) ) );
			SetProgress( uiAttackBase, (pHero.m_attack_base+pHero.m_attack_wash)/(pHero.m_attack_base+config.attack_wash_limit) )
			if pHero.m_attack_wash >= config.attack_wash_limit then
				SetTrue( uiAttackBase.transform:Find("Full") )
			else
				SetFalse( uiAttackBase.transform:Find("Full") )
			end
			
			-- 防
			local uiDefenseBase = m_uiGrid.transform:GetChild(2).gameObject;
			SetText( uiDefenseBase.transform:Find("Text"), F(139, (pHero.m_defense_base+pHero.m_defense_wash)) );
			SetProgress( uiDefenseBase, (pHero.m_defense_base+pHero.m_defense_wash)/(pHero.m_defense_base+config.defense_wash_limit) )
			if pHero.m_defense_wash >= config.defense_wash_limit then
				SetTrue( uiDefenseBase.transform:Find("Full") )
			else
				SetFalse( uiDefenseBase.transform:Find("Full") )
			end
			
			-- 兵
			local uiTroopsBase = m_uiGrid.transform:GetChild(3).gameObject;
			SetText( uiTroopsBase.transform:Find("Text"), F(140, (pHero.m_troops_base+pHero.m_troops_wash)) );
			SetProgress( uiTroopsBase, (pHero.m_troops_base+pHero.m_troops_wash)/(pHero.m_troops_base+config.troops_wash_limit) )
			if pHero.m_troops_wash >= config.troops_wash_limit then
				SetTrue( uiTroopsBase.transform:Find("Full") )
			else
				SetFalse( uiTroopsBase.transform:Find("Full") )
			end
			
		else
			SetFalse( uiSelect )
		end
	end

end

function HeroWashDlgClear()
	local objs = {};
	for i = 0 ,m_uiContent.transform.childCount - 1 do
		table.insert( objs, m_uiContent.transform:GetChild(i).gameObject )
    end
	for k, v in pairs(objs) do
		local obj = v;
		m_ObjectPool:Release( "UIP_HeroHead", obj );
    end
	m_upHeroCache = {}
	m_CacheHeroCache = {};
	m_CacheHeroList = {};
end

function HeroWashDlgWashInfo( recvValue )
	m_recvValue = recvValue;
	SetTrue( m_uiNum );
	SetText( m_uiNum, T(841)..":"..recvValue.m_hero_washnum.."/"..global.hero_wash_max )
	if recvValue.m_hero_washnum < global.hero_wash_max then
		SetTrue( m_uiTimer )
		SetTimer( m_uiTimer, recvValue.m_hero_washsec, recvValue.m_hero_washsec, 1 );
	else
		SetFalse( m_uiTimer )
	end
	-- 定时通知
	notification_set( "SETTING_PUSH_HEROWASHFULL", T(5910), (global.hero_wash_max-recvValue.m_hero_washnum)*global.hero_wash_sec )
end

function HeroWashDlgUpdate()
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	HeroWashDlgSelectHero( m_herokind )
end

function HeroWashDlgFree()
	if m_recvValue == nil then
		return
	end
	if m_recvValue.m_hero_washnum <= 0 then
		pop(T(843))
		return
	end
	system_askinfo( ASKINFO_HERO, "", 5, m_herokind );
end

function HeroWashDlgToken()
	local token = global.hero_wash_token;
	MsgBoxEx( F( 844, token ), function( sure, toggle ) 
		if sure == 1 then
			if GetPlayer().m_token < token then
				JumpToken()
			else
				system_askinfo( ASKINFO_HERO, "", 6, m_herokind );
			end
		end
		m_wash_toggle = toggle;
	end, m_wash_toggle ) 
end


function HeroWashDlgQualtiySort(a,b)
	if a ~= nil and b ~= nil then
        if a.m_pHero.m_color > b.m_pHero.m_color then
            return true;
        elseif a.m_pHero.m_color == b.m_pHero.m_color then
			if a.m_pHero.m_level > b.m_pHero.m_level then
				return true;
			elseif a.m_pHero.m_level == b.m_pHero.m_level then
				if a.total > b.total then
					return true
				else
					return false;
				end
			else
				return false
			end
		else
			return false
        end
    else
        return false;
    end	
end
function HeroWashDlgTalentSort(a,b)
	if a ~= nil and b ~= nil then
        if a.total > b.total then
            return true;
        elseif a.total == b.total then
			if a.m_pHero.m_level > b.m_pHero.m_level then
				return true;
			else
				return false;
			end
		else
			return false
        end
    else
        return false;
    end	
end
function HeroWashDlgLevelSort(a,b)
	if a ~= nil and b ~= nil then
        if a.m_pHero.m_level > b.m_pHero.m_level then
            return true;
        elseif a.m_pHero.m_level == b.m_pHero.m_level then
			if a.m_pHero.m_color > b.m_pHero.m_color then
				return true;
			else
				return false;
			end
		else
			return false
        end
    else
        return false;
    end	
end
