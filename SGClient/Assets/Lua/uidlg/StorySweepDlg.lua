-- 界面
local m_Dlg = nil;
local m_uiTitle = nil; --UnityEngine.GameObject
local m_uiDesc = nil; --UnityEngine.GameObject
local m_uiLayer1 = nil; --UnityEngine.GameObject
local m_uiLayer2 = nil; --UnityEngine.GameObject
local m_uiGrid = nil; --UnityEngine.GameObject
local m_uiBody = nil; --UnityEngine.GameObject
local m_uiSelectGrid = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiUIP_HeroHead = nil; --UnityEngine.GameObject
local m_uiScroll = nil; --UnityEngine.GameObject
local m_ObjectPool = nil;
local m_storyid = 0;
local m_sweep_herokind = {}
local m_upHeroCache = {}	
local m_CacheHeroCache = {};
local m_CacheHeroList = {}
-- 打开界面
function StorySweepDlgOpen()
	m_Dlg = eye.uiManager:Open( "StorySweepDlg" );
end

-- 隐藏界面
function StorySweepDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "StorySweepDlg" );
end

-- 删除界面
function StorySweepDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function StorySweepDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            StorySweepDlgClose();
		elseif nControlID == -2 then
			StorySweepDlgHideSelectLayer()
		elseif nControlID == 1 then
			StorySweepDlgFight()
		elseif nControlID == 2 then
			StorySweepDlgSweep()
		elseif nControlID == 3 then
			StorySweepDlgShowSelectLayer()
		elseif nControlID == 4 then
			StorySweepDlgHideSelectLayer()
		-- 选择武将
		elseif nControlID > 1000 and nControlID < 2000 then
			StorySweepDlgSelectHero( nControlID - 1000 )
        end
	end
end

-- 载入时调用
function StorySweepDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiTitle = objs[0];
	m_uiDesc = objs[1];
	m_uiLayer1 = objs[2];
	m_uiLayer2 = objs[3];
	m_uiGrid = objs[4];
	m_uiBody = objs[5];
	m_uiSelectGrid = objs[6];
	m_uiContent = objs[7];
	m_uiUIP_HeroHead = objs[8];
	m_uiScroll = objs[9];
	-- 对象池
	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );
	m_ObjectPool:CreatePool("UIP_HeroHead", 4, 4, m_uiUIP_HeroHead);
end

-- 界面初始化时调用
function StorySweepDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function StorySweepDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function StorySweepDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function StorySweepDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function StorySweepDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function StorySweepDlgShow( storyid, sweep_herokind )
	StorySweepDlgOpen()
	m_storyid = storyid;
	m_sweep_herokind = sweep_herokind
	SetTrue( m_uiLayer1 )
	SetFalse( m_uiLayer2 )
	SetText( m_uiTitle.transform:Find("Text"), StoryRankName( storyid ) )
	
	-- 当前体力
	if GetPlayer().m_body < g_story[storyid].body then
		SetRichText( m_uiBody, F(2045, GetPlayer().m_body ) )	
	else
		SetRichText( m_uiBody, F(2044, GetPlayer().m_body ) )	
	end
	StorySweepDlgCreateLayer1Hero()
end

function StorySweepDlgCreateLayer1Hero()
	for i=0, 3, 1 do
		local uiHero = m_uiGrid.transform:GetChild(i);
		SetFalse( uiHero )
	end
	-- 设置武将信息
	for i=1, 4, 1 do
		local uiHero = m_uiGrid.transform:GetChild(i-1);
		local pHero = GetHero():GetPtr(m_sweep_herokind[i])
		if pHero ~= nil then
			SetTrue( uiHero )
			SetImage( uiHero.transform:Find("Shape"), HeroHeadSprite(pHero.m_kind) )
			SetImage( uiHero.transform:Find("Color"), ItemColorSprite(pHero.m_color) )
			SetImage( uiHero.transform:Find("Corps"), CorpsSprite(pHero.m_corps) )
			SetText( uiHero.transform:Find("Name"), HeroNameLv(pHero.m_kind, pHero.m_level), NameColor(pHero.m_color) )
			if pHero.m_offset >= 1000 and pHero.m_offset < 1004 then
				SetText( uiHero.transform:Find("Flag"), T(2047) )
			elseif pHero.m_offset >= 1004 and pHero.m_offset < 1008 then
				SetText( uiHero.transform:Find("Flag"), T(2048) )
			elseif pHero.m_offset >= 1008 and pHero.m_offset < 1012 then
				SetText( uiHero.transform:Find("Flag"), T(2049) )
			else
				SetText( uiHero.transform:Find("Flag"), "" )
			end
		end
	end
end

-- 显示选择武将页
function StorySweepDlgShowSelectLayer()
	SetFalse( m_uiLayer1 )
	SetTrue( m_uiLayer2 )
	StorySweepDlgCreateSelect()
	StorySweepDlgCreateList()
end

-- 创建选择的武将
function StorySweepDlgCreateSelect()
	-- 已经选择的武将信息
	for i=0, 3, 1 do
		local uiHero = m_uiSelectGrid.transform:GetChild(i);
		SetFalse( uiHero )
	end
	for i=1, 4, 1 do
		local uiHero = m_uiSelectGrid.transform:GetChild(i-1);
		local pHero = GetHero():GetPtr(m_sweep_herokind[i])
		if pHero ~= nil then
			SetTrue( uiHero )
			SetImage( uiHero.transform:Find("Shape"), HeroHeadSprite(pHero.m_kind) )
			SetImage( uiHero.transform:Find("Color"), ItemColorSprite(pHero.m_color) )
			--SetImage( uiHero.transform:Find("Corps"), CorpsSprite(pHero.m_corps) )
			SetText( uiHero.transform:Find("Name"), HeroNameLv(pHero.m_kind, pHero.m_level), NameColor(pHero.m_color) )
			if pHero.m_offset >= 1000 and pHero.m_offset < 1004 then
				SetText( uiHero.transform:Find("Flag"), T(2047) )
			elseif pHero.m_offset >= 1004 and pHero.m_offset < 1008 then
				SetText( uiHero.transform:Find("Flag"), T(2048) )
			elseif pHero.m_offset >= 1008 and pHero.m_offset < 1012 then
				SetText( uiHero.transform:Find("Flag"), T(2049) )
			else
				SetText( uiHero.transform:Find("Flag"), "" )
			end
		end
	end
end

-- 是否扫荡武将
local function _isUpHero( kind )
	if kind <= 0 then
		return 0
	end
	for i=1, 4, 1 do
		if m_sweep_herokind[i] == kind then
			return 1
		end
	end
	return 0
end

-- 创建武将列表
function StorySweepDlgCreateList()
	StorySweepDlgListClear()
	-- 主将进入临时缓存
	for offset = 0, 11, 1 do	
		local pHero = GetHero().m_CityHero[offset];
		if pHero ~= nil and pHero.m_kind > 0 then
			table.insert(m_upHeroCache, { m_kind = pHero.m_kind, m_pHero = pHero });
		end
	end
	
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
		local uiFlag = objs[2];
		local uiName = objs[3];
		local uiSelect = objs[4];
		local uiCon = objs[5];
		SetTrue( uiCon )
		SetTrue( uiShape )
		SetTrue( uiColor )
		SetTrue( uiFlag )
		SetTrue( uiName )
		
		if _isUpHero( pHero.m_kind ) == 1 then
			SetTrue( uiSelect )
		else
			SetFalse( uiSelect )
		end
		
		SetImage( uiShape, HeroHeadSprite( pHero.m_kind )  );
		SetImage( uiColor,  ItemColorSprite( pHero.m_color )  );
		SetText( uiName, HeroNameLv( pHero.m_kind, pHero.m_level ), NameColor(pHero.m_color) );
		if pHero.m_offset >= 1000 and pHero.m_offset < 1004 then
			SetText( uiFlag, T(2047) )
		elseif pHero.m_offset >= 1004 and pHero.m_offset < 1008 then
			SetText( uiFlag, T(2048) )
		elseif pHero.m_offset >= 1008 and pHero.m_offset < 1012 then
			SetText( uiFlag, T(2049) )
		else
			SetText( uiFlag, "" )
		end
		m_CacheHeroList[pHero.m_kind] = uiHeroObj;
	end
	--m_uiScroll.transform:GetComponent( typeof(UIClippingLoop) ):Refresh()
end

-- 选择武将
function StorySweepDlgSelectHero( kind )
	local uiHeroObj = m_CacheHeroList[kind];
	if uiHeroObj == nil then
		return
	end

	local objs = uiHeroObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiSelect = objs[4];
		
	if _isUpHero( kind ) == 1 then
		-- 已经选择这个武将，那么解除选择
		for i=1, 4, 1 do
			if m_sweep_herokind[i] == kind then
				m_sweep_herokind[i] = 0
				StorySweepDlgCreateSelect()
				break
			end
		end
		SetFalse( uiSelect )
	else
		-- 没有选择这个武将，那么找到空位
		local empty = 0;
		for i=1, 4, 1 do
			if m_sweep_herokind[i] <= 0 then
				m_sweep_herokind[i] = kind
				SetTrue( uiSelect )
				StorySweepDlgCreateSelect()
				break
			end
		end
	end
	
end

-- 武将列表清空
function StorySweepDlgListClear()
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
	m_upHeroCache = {}
end

-- 隐藏选择武将页
function StorySweepDlgHideSelectLayer()
	SetTrue( m_uiLayer1 )
	SetFalse( m_uiLayer2 )
	StorySweepDlgListClear()
	StorySweepDlgCreateLayer1Hero()
end

-- 挑战
function StorySweepDlgFight()
	StorySweepDlgClose();
	BattleDlgShowByStory( m_storyid )
end

-- 扫荡
function StorySweepDlgSweep()
	-- 消耗体力
	if GetPlayer().m_body < g_story[m_storyid].body*5 then
		StorySweepDlgClose();
		JumpBody()
		return
	end	
	system_askinfo( ASKINFO_STORY, "", 8, m_storyid, m_sweep_herokind[1], m_sweep_herokind[2], m_sweep_herokind[3], m_sweep_herokind[4] );
	StorySweepDlgClose();
end

