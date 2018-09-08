-- 界面
local m_Dlg = nil;
local m_uiScroll = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiUIP_Hero = nil; --UnityEngine.GameObject
local m_uiMyPower = nil; --UnityEngine.GameObject
local m_uiMyTeamList = nil; --UnityEngine.GameObject
local m_ObjectPool = nil;
local m_team = nil;
local m_CacheHeroCache = {};
local m_CacheHeroList = {}
-- 打开界面
function ColiseumTeamDlgOpen()
	m_Dlg = eye.uiManager:Open( "ColiseumTeamDlg" );
	
end

-- 隐藏界面
function ColiseumTeamDlgClose()
	if m_Dlg == nil then
		return;
	end
	m_CacheHeroCache = {}
	m_CacheHeroList = {}
	eye.uiManager:Close( "ColiseumTeamDlg" );
end

-- 删除界面
function ColiseumTeamDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function ColiseumTeamDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            ColiseumTeamDlgClose();
		elseif nControlID == 1 then
			ColiseumTeamDlgSave()
		-- 选择武将
		elseif nControlID > 1000 and nControlID < 2000 then
			ColiseumTeamDlgSelectHero( nControlID - 1000 )
        end
	end
end

-- 载入时调用
function ColiseumTeamDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiScroll = objs[0];
	m_uiContent = objs[1];
	m_uiUIP_Hero = objs[2];
	m_uiMyPower = objs[3];
	m_uiMyTeamList = objs[4];
	-- 对象池
	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );
	m_ObjectPool:CreatePool("UIP_Hero", 4, 4, m_uiUIP_Hero);
end

-- 界面初始化时调用
function ColiseumTeamDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function ColiseumTeamDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function ColiseumTeamDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function ColiseumTeamDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function ColiseumTeamDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function ColiseumTeamDlgShow( team )
	ColiseumTeamDlgOpen()
	m_team = team
	ColiseumTeamDlgCreateList()
	ColiseumTeamDlgCreateTeam()
end

-- 是否扫荡武将
local function _isUpHero( kind )
	if kind <= 0 then
		return 0
	end
	for i=1, 3, 1 do
		if m_team[i] == kind then
			return 1
		end
	end
	return 0
end

-- 创建武将列表
function ColiseumTeamDlgCreateList()
	ColiseumTeamDlgClear()
	-- 主将进入临时缓存
	--[[for offset = 0, 11, 1 do	
		local pHero = GetHero().m_CityHero[offset];
		if pHero ~= nil and pHero.m_kind > 0 then
			local base = pHero.m_attack_base+pHero.m_defense_base+pHero.m_troops_base;
			local wash = pHero.m_attack_wash+pHero.m_defense_wash+pHero.m_troops_wash;
			table.insert(m_CacheHeroCache, { m_kind = pHero.m_kind, m_pHero = pHero, total = base+wash });
		end
	end--]]
	
	-- 聚贤阁进临时缓存			
	for offset = 0, MAX_HERONUM-1, 1 do
		local pHero = GetHero().m_Hero[offset];
		if pHero ~= nil and pHero.m_kind > 0 then
			local base = pHero.m_attack_base+pHero.m_defense_base+pHero.m_troops_base;
			local wash = pHero.m_attack_wash+pHero.m_defense_wash+pHero.m_troops_wash;
			table.insert(m_CacheHeroCache, { m_kind = pHero.m_kind, m_pHero = pHero, total = base+wash });
		end
	end
	
	-- 排序
	table.sort( m_CacheHeroCache, HeroWashDlgQualtiySort );

	local num = #m_CacheHeroCache;
	for i=1, num, 1 do
		local pHero = m_CacheHeroCache[i].m_pHero;
		local uiHeroObj = m_ObjectPool:Get( "UIP_Hero" );
		uiHeroObj.transform:SetParent( m_uiContent.transform );
		SetTrue( uiHeroObj )
		SetControlID( uiHeroObj, 1000+pHero.m_kind )
		
		local objs = uiHeroObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
		local uiColor = objs[0];
		local uiShape = objs[1];
		local uiNameColor = objs[2];
		local uiName = objs[3];
		local uiSelect = objs[4];
		
		if _isUpHero( pHero.m_kind ) == 1 then
			SetTrue( uiSelect )
		else
			SetFalse( uiSelect )
		end
		
		SetImage( uiShape, HeroFaceSprite( pHero.m_kind ) );
		SetImage( uiColor,  HeroColorSprite( pHero.m_color ) );
		SetImage( uiNameColor,  HeroNameColorSprite( pHero.m_color )  );
		SetText( uiName, HeroNameLv( pHero.m_kind, pHero.m_level ), NameColor(pHero.m_color) );
		
		--[[if pHero.m_offset >= 1000 and pHero.m_offset < 1004 then
			SetText( uiFlag, T(2047) )
		elseif pHero.m_offset >= 1004 and pHero.m_offset < 1008 then
			SetText( uiFlag, T(2048) )
		elseif pHero.m_offset >= 1008 and pHero.m_offset < 1012 then
			SetText( uiFlag, T(2049) )
		else
			SetText( uiFlag, "" )
		end--]]
		m_CacheHeroList[pHero.m_kind] = uiHeroObj;
	end
	--m_uiScroll.transform:GetComponent( typeof(UIClippingLoop) ):Refresh()
end

-- 武将列表清空
function ColiseumTeamDlgClear()
	local objs = {};
	for i = 0 ,m_uiContent.transform.childCount - 1 do
		table.insert( objs, m_uiContent.transform:GetChild(i).gameObject )
    end
	for k, v in pairs(objs) do
		local obj = v;
		m_ObjectPool:Release( "UIP_Hero", obj );
    end
	m_CacheHeroCache = {};
	m_CacheHeroList = {};
end

-- 选择武将
function ColiseumTeamDlgSelectHero( kind )
	local uiHeroObj = m_CacheHeroList[kind];
	if uiHeroObj == nil then
		return
	end

	local objs = uiHeroObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiSelect = objs[4];
		
	if _isUpHero( kind ) == 1 then
		-- 已经选择这个武将，那么解除选择
		for i=1, 3, 1 do
			if m_team[i] == kind then
				m_team[i] = 0
				for tmpi=i+1, 3, 1 do
					m_team[tmpi-1] = m_team[tmpi]
					m_team[tmpi] = 0;
				end
				ColiseumTeamDlgCreateTeam()
				break
			end
		end
		SetFalse( uiSelect )
	else
		-- 没有选择这个武将，那么找到空位
		local full = 1;
		for i=1, 3, 1 do
			if m_team[i] <= 0 then
				m_team[i] = kind
				full = 0
				SetTrue( uiSelect )
				ColiseumTeamDlgCreateTeam()
				break
			end
		end
		if full == 1 then
			pop(T(4298))
		end
	end
end

-- 创建选择的武将
function ColiseumTeamDlgCreateTeam()
	-- 已经选择的武将信息
	for i=0, 2, 1 do
		local uiHero = m_uiMyTeamList.transform:GetChild(i);
		SetFalse( uiHero )
	end
	local bpower = 0;
	for i=1, 3, 1 do
		local uiHero = m_uiMyTeamList.transform:GetChild(i-1);
		local pHero = GetHero():GetPtr(m_team[i])
		if pHero ~= nil then
			SetTrue( uiHero )
			SetControlID( uiHero, 1000+pHero.m_kind )
			SetImage( uiHero.transform:Find("Shape"), HeroHeadSprite(pHero.m_kind) )
			SetImage( uiHero.transform:Find("Color"), ItemColorSprite(pHero.m_color) )
			--SetImage( uiHero.transform:Find("Corps"), CorpsSprite(pHero.m_corps) )
			SetText( uiHero.transform:Find("Name"), HeroName(pHero.m_kind), NameColor(pHero.m_color) )
			--[[if pHero.m_offset >= 1000 and pHero.m_offset < 1004 then
				SetText( uiHero.transform:Find("Flag"), T(2047) )
			elseif pHero.m_offset >= 1004 and pHero.m_offset < 1008 then
				SetText( uiHero.transform:Find("Flag"), T(2048) )
			elseif pHero.m_offset >= 1008 and pHero.m_offset < 1012 then
				SetText( uiHero.transform:Find("Flag"), T(2049) )
			else
				SetText( uiHero.transform:Find("Flag"), "" )
			end--]]
			bpower = bpower + pHero.m_bpower
		end
	end
	SetText( m_uiMyPower, F(4290,bpower) )
end

function ColiseumTeamDlgSave()
	system_askinfo( ASKINFO_COLISEUM, "", 4, m_team[1], m_team[2], m_team[3] )
end
