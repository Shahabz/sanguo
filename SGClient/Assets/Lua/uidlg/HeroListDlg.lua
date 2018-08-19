-- 界面
local m_Dlg = nil;
local m_DialogFrameMod = nil;
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiUIP_HeroRow = nil; --UnityEngine.GameObject
local m_uiToggleRank = nil; --UnityEngine.GameObject toggle按钮
local m_uiTooglGroup = nil; --UnityEngine.GameObject
local m_uiCityHeroTitle = nil; --UnityEngine.GameObject
local m_uiOpenInfoBtu = nil; --UnityEngine.GameObject 显示资质
local m_uiCloseInfoBtu = nil; --UnityEngine.GameObject 隐藏资质
local m_uiRankWayText = nil; --UnityEngine.GameObjec 显示排序方法的Text
local m_uiEmptyHeroContent = nil; --UnityEngine.GameObject
local m_uiHeoVisitEnter = nil; --UnityEngine.GameObject
local m_uiUIP_NationHeroVisit = {nil,nil,nil,nil}; --UnityEngine.GameObject
local m_uiTextBack = nil; --UnityEngine.GameObject
local m_CacheHeroCache = {}
local m_CacheHeroList = {}
local m_SelectHeroKind = 0;
local m_SelectRankWay = 0;
local b_OpenCloseInfo = false;

local m_rankType = 1;
local m_path = 0;
local m_isUpdate = 0;

HEROLIST_PATH_HERO			= 0
HEROLIST_PATH_HERO_GATHER	= 1
HEROLIST_PATH_HERO_GUARD	= 2
HEROLIST_PATH_HERO_LIST		= 3

-- 打开界面
function HeroListDlgOpen()
	eye.audioManager:Play(317);
	m_Dlg = eye.uiManager:Open( "HeroListDlg" );
	m_DialogFrameMod = DialogFrameModOpen( m_Dlg, T(35), HELP_HeroListDlg, HeroListDlgClose );
end

-- 隐藏界面
function HeroListDlgClose()
	if m_Dlg == nil then
		return;
	end
	DialogFrameModClose( m_DialogFrameMod );
	m_DialogFrameMod = nil;
	eye.uiManager:Close( "HeroListDlg" );
	m_SelectHeroKind = 0;
	m_isUpdate = 0;
end

-- 删除界面
function HeroListDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function HeroListDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            HeroListDlgClose();
			HeroListDlgSelectCloseToggle();
			
		elseif nControlID == -2 then
			HeroListDlgSelectCloseToggle();
			
		elseif nControlID == 1 then
			HeroListDlgSelectRankWay(nControlID);
			HeroListDlgRankWay(nControlID);
			
		elseif nControlID == 2 then
			HeroListDlgSelectRankWay(nControlID);
			HeroListDlgRankWay(nControlID);
			
		elseif nControlID == 3 then
			HeroListDlgSelectRankWay(nControlID);
			HeroListDlgRankWay(nControlID);	
			
		elseif nControlID == 4 then
			HeroListDlgCloseInfo();
			HeroListDlgSelectCloseToggle();
			
		elseif nControlID == 5 then
			HeroListDlgOpenInfo();
			HeroListDlgSelectCloseToggle();
			
		--武将寻访
		elseif nControlID == 6 then
			HeroListDlgClose()
			HeroVisitDlgShow()
		
		-- 国家名将招募
		elseif nControlID >= 1000 and nControlID < 1500 then
			HeroListDlgNationHeroCall( nControlID-1000 )
			
		elseif nControlID >= 10000 and nControlID < 30000 then
		 	HeroListDlgSelect(nControlID-10000);
			HeroListDlgSelectCloseToggle();
		
		elseif nControlID == 30001 then
			pop( T(610) )
		elseif nControlID == 30002 then
			pop( T(611) )
        end
	elseif nType == UI_EVENT_TOGGLEVALUECHANGE then
		if nControlID == 1 then
			HeroListDlgSetTogGroup();
		end	
	elseif nType == UI_EVENT_SCROLLDRAG then
		if nControlID == -2 then
			HeroListDlgSelectCloseToggle();
		end
	end
end

-- 载入时调用
function HeroListDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiContent = objs[0];
	m_uiUIP_HeroRow = objs[1];
	m_uiToggleRank = objs[2];
	m_uiTooglGroup = objs[3];
	m_uiCityHeroTitle = objs[4];
	m_uiOpenInfoBtu = objs[5];
	m_uiCloseInfoBtu = objs[6];
	m_uiRankWayText = objs[7];
	m_uiEmptyHeroContent = objs[8];
	m_uiHeoVisitEnter = objs[9];
	m_uiUIP_NationHeroVisit[1] = objs[10];
	m_uiUIP_NationHeroVisit[2] = objs[11];
	m_uiUIP_NationHeroVisit[3] = objs[12];
	m_uiUIP_NationHeroVisit[4] = objs[13];
	m_uiTextBack = objs[14];
end

-- 界面初始化时调用
function HeroListDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function HeroListDlgOnEnable( gameObject )	
	if m_isUpdate == 1 then
		m_isUpdate = 0;
		HeroListDlgLoadHero()
	end
end

-- 界面隐藏时调用
function HeroListDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function HeroListDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function HeroListDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function HeroListDlgShow( path )
	m_path = path
	HeroListDlgOpen()
	if GetPlayer().m_level >= global.hero_visit_actorlevel and GetPlayer():CityLevel() >= global.hero_visit_mainlevel then
		SetFalse( m_uiTextBack )
		SetTrue( m_uiHeoVisitEnter );
	else
		SetTrue( m_uiTextBack )
		SetText( m_uiTextBack.transform:Find("Text"), F(1924,global.hero_visit_mainlevel) )
		SetFalse( m_uiHeoVisitEnter );
	end
	
	HeroListDlgCreateNationHeroVisit()
	HeroListDlgLoadHero();
end

function HeroListDlgUpdate()
	if m_SelectHeroKind > 0 then
		m_isUpdate = 1;
	end
end

-- 缓存排序
function HeroListCacheSort(a, b)
    if a ~= nil and b ~= nil then
        if a.m_kind < b.m_kind then
            return true;
        else
            return false;
        end
    else
        return false;
    end
end

-- 清空英雄对象
function HeroListDlgClear()
    if m_uiEmptyHeroContent ~= nil then
        local childCount = m_uiEmptyHeroContent.transform.childCount;
        for i = 0, childCount - 1, 1 do
            local child = m_uiEmptyHeroContent.transform:GetChild(i).gameObject;
            -- 不删除,改为隐藏
            child:SetActive(false);
        end
    end
    m_CacheHeroCache = {};
    m_CacheHeroList = {};
end

-- 清除一行的状态为初始的显示
function HeroListDlgClearHeroRow( row, up )
    local trans = row.transform;
    for tmpi = 1, trans.childCount - 1, 1 do
        local child = trans:GetChild(tmpi);
		local objs = child.transform:GetComponent( typeof(Reference) ).relatedGameObject;
		local uiBorder = objs[0];
		local uiShape = objs[1];
		local uiColor = objs[2];
		local uiCorps = objs[3];
		local uiName = objs[5];
		local uiBack = objs[6];
		
		SetFalse( uiBorder )
		SetFalse( uiShape )
		SetFalse( uiColor )
		SetFalse( uiCorps )
		SetFalse( uiName )
		SetFalse( uiBack )
		
		if up == true then
			SetTrue( uiBack )
			SetTrue( uiBorder )
		end
		child:GetComponent("UIButton").controlID = 0;
    end
	local RowBack = trans:GetChild(0);
	if up == true then
		SetTrue(RowBack);
	else
		SetFalse(RowBack);
	end
end

-- 读取缓存英雄
function HeroListDlgLoadHero()
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
    HeroListDlgClear()
    local heroCount = 0;
    local currHeroRow = nil;
    local rowCount = 0;
	
	local baseoffset = 0; 
	if m_path == HEROLIST_PATH_HERO or m_path == HEROLIST_PATH_HERO_LIST then -- 上阵武将
		SetText( m_uiCityHeroTitle.transform:Find("Text"), T(692) )
		baseoffset = 0;
		
	elseif m_path == HEROLIST_PATH_HERO_GATHER then -- 财赋署武将
		SetText( m_uiCityHeroTitle.transform:Find("Text"), T(1491) )
		baseoffset = 4
		
	elseif m_path == HEROLIST_PATH_HERO_GUARD then -- 御林卫武将
		SetText( m_uiCityHeroTitle.transform:Find("Text"), T(1492) )
		baseoffset = 8	
	end
	
    -- 先放进临时缓存
    for offset = 0+baseoffset, 3+baseoffset, 1 do
        local pHero = GetHero().m_CityHero[offset];
        if pHero ~= nil and pHero.m_kind > 0 then
        	--print("pHero.m_kind:"..pHero.m_kind)
			local base = pHero.m_attack_base+pHero.m_defense_base+pHero.m_troops_base;
			local wash = pHero.m_attack_wash+pHero.m_defense_wash+pHero.m_troops_wash;
            table.insert(m_CacheHeroCache, { m_kind = pHero.m_kind, m_color = pHero.m_color, m_level = pHero.m_level, m_corps = pHero.m_corps, m_god = pHero.m_god, m_offset = offset,
			m_qualtiy = base, 
			m_washqualtiy = wash,
			m_attack = pHero.m_attack_base + pHero.m_attack_wash,
			m_defense = pHero.m_defense_base + pHero.m_defense_wash,
			m_troops = pHero.m_troops_base + pHero.m_troops_wash} );
        end
    end
	
	-- 先放进临时缓存			
    for offset = 0, MAX_HERONUM-1, 1 do
        local pHero = GetHero().m_Hero[offset];
        if pHero ~= nil and pHero.m_kind > 0 then
			if GetHero():IsCanUse( pHero.m_kind ) == true then
				local base = pHero.m_attack_base + pHero.m_defense_base + pHero.m_troops_base;
				local wash = pHero.m_attack_wash + pHero.m_defense_wash + pHero.m_troops_wash
				table.insert(m_CacheHeroCache, { m_kind = pHero.m_kind, m_color = pHero.m_color, m_level = pHero.m_level, m_corps = pHero.m_corps, m_god = pHero.m_god, m_offset = 10000+offset,
				m_qualtiy = base, 
				m_washqualtiy = wash,
				m_attack = pHero.m_attack_base + pHero.m_attack_wash,
				m_defense = pHero.m_defense_base + pHero.m_defense_wash,
				m_troops = pHero.m_troops_base + pHero.m_troops_wash} );
			end
        end
    end

    -- 排序
	HeroListDlgSelectRankWay( m_rankType );
	if m_rankType == 1 then
	    table.sort(m_CacheHeroCache,HeroListDlgQualtiySort);
	elseif m_rankType == 2 then
	    table.sort(m_CacheHeroCache, HeroListDlgTalentSort);
	elseif m_rankType == 3 then
	    table.sort(m_CacheHeroCache, HeroListDlgLevelSort);
	end

    -- 创建上阵对象
	local upHeroRow = m_uiContent.transform:Find("UIP_HeroRow").gameObject;
	HeroListDlgClearHeroRow(upHeroRow,true);

	local tmpCache = {};
    for i=1, #m_CacheHeroCache, 1 do
		local pHero = m_CacheHeroCache[i];
        if pHero.m_offset < 10000 then
			table.insert( tmpCache, pHero );
			--m_CacheHeroList[pHero.m_offset] = upHeroRow.transform:GetChild(index);
            --HeroListDlgSetHero( m_CacheHeroList[pHero.m_offset], pHero, index )
			--index = index + 1
		end
	end
	table.sort( tmpCache, function(a,b) 
		if a.m_offset < b.m_offset then 
			return true 
		end
		return false;
	end )
	
	local index = 1;
	for i=1, #tmpCache, 1 do
		local pHero = tmpCache[i];
        if pHero.m_offset < 10000 then
			m_CacheHeroList[pHero.m_offset] = upHeroRow.transform:GetChild(index);
            HeroListDlgSetHero( m_CacheHeroList[pHero.m_offset], pHero, index )
			index = index + 1
		end
	end
	
	
	-- 创建锁定的对象
	if index <= 4 then
		for tmpi=index,4,1 do
			HeroListDlgSetHero( upHeroRow.transform:GetChild(tmpi), nil, tmpi )
		end
	end
	
	-- 创建空闲对象
	SetTrue(m_uiEmptyHeroContent);
	rowCount = 0
	for i, v in pairs( m_CacheHeroCache ) do
        local pHero = v;
        if pHero.m_offset >= 10000 then
            local _index = heroCount % 4;
            if _index == 0 then
                if rowCount >= m_uiEmptyHeroContent.transform.childCount then
                    currHeroRow = GameObject.Instantiate(m_uiUIP_HeroRow);
                    currHeroRow.transform:SetParent(m_uiEmptyHeroContent.transform);
                    currHeroRow.transform.localScale = Vector3.one;
                    currHeroRow:SetActive(true);
					if currHeroRow ~= nil then
					end
                else
                    currHeroRow = m_uiEmptyHeroContent.transform:GetChild(rowCount).gameObject;
                    currHeroRow:SetActive(true);
                end
				HeroListDlgClearHeroRow(currHeroRow,false);
				HeroListDlgSetBackTF(currHeroRow,false);
                rowCount = rowCount + 1;
            end
            local uiHeroObj = currHeroRow.transform:GetChild(_index+1);
            HeroListDlgSetHero( uiHeroObj, pHero, _index+1 )
			m_CacheHeroList[pHero.m_offset] = uiHeroObj;
			heroCount = heroCount + 1;
        end
    end
end

function HeroListDlgRankWay( rankType )
    local heroCount = 0;
    local currHeroRow = nil;
	m_rankType = rankType
	if m_rankType == 1 then
	    table.sort(m_CacheHeroCache,HeroListDlgQualtiySort);
	elseif m_rankType == 2 then
	    table.sort(m_CacheHeroCache, HeroListDlgTalentSort);
	elseif m_rankType == 3 then
	    table.sort(m_CacheHeroCache, HeroListDlgLevelSort);
	end
	SetTrue(m_uiEmptyHeroContent);
	rowCount = 0
	for i, v  in  pairs( m_CacheHeroCache )  do
        local pHero = v;
        if pHero.m_offset >= 10000 then
            local _index = heroCount % 4;
            if _index == 0 then
                if rowCount >= m_uiEmptyHeroContent.transform.childCount then
                    currHeroRow = GameObject.Instantiate(m_uiUIP_HeroRow);
                    currHeroRow.transform:SetParent(m_uiEmptyHeroContent.transform);
                    currHeroRow.transform.localScale = Vector3.one;
                    currHeroRow:SetActive(true);
					if currHeroRow ~= nil then
					end
                else
                    currHeroRow = m_uiEmptyHeroContent.transform:GetChild(rowCount).gameObject;
                    currHeroRow:SetActive(true);
                end
				HeroListDlgClearHeroRow(currHeroRow,false);
				HeroListDlgSetBackTF(currHeroRow,false);
                rowCount = rowCount + 1;
            end
            local uiHeroObj = currHeroRow.transform:GetChild(_index+1);
            HeroListDlgSetHero( uiHeroObj, pHero, _index+1 )
			m_CacheHeroList[pHero.m_offset] = uiHeroObj;
			heroCount = heroCount + 1;
        end
    end
end

function HeroListDlgQualtiySort(a,b)
	if a ~= nil and b ~= nil then
        if a.m_color > b.m_color then
            return true;
        elseif a.m_color == b.m_color then
			if a.m_level > b.m_level then
				return true;
			elseif a.m_level == b.m_level then
				if (a.m_qualtiy + a.m_washqualtiy) > (b.m_qualtiy + b.m_washqualtiy) then
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
function HeroListDlgTalentSort(a,b)
	if a ~= nil and b ~= nil then
        if (a.m_qualtiy + a.m_washqualtiy) > (b.m_qualtiy + b.m_washqualtiy) then
            return true;
        elseif (a.m_qualtiy + a.m_washqualtiy) == (b.m_qualtiy + b.m_washqualtiy) then
			if a.m_level > b.m_level then
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
function HeroListDlgLevelSort(a,b)
	if a ~= nil and b ~= nil then
        if a.m_level > b.m_level then
            return true;
        elseif a.m_level == b.m_level then
			if a.m_color > b.m_color then
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
	
function HeroListDlgSetBackTF(obj,b_tf)
	local back = obj.transform:Find("Back");
	SetShow(back,b_tf);
end

-- 设置单个对象
function HeroListDlgSetHero( uiHeroObj, pHero, index )
	local objs = uiHeroObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiColorBack = objs[0];
	local uiShape = objs[1];
	local uiNameBack = objs[2];
	local uiCorps = objs[3];
	local uiType = objs[4];
	local uiName = objs[5];
	local uiLevel = objs[6]
	local uiHidBack = objs[7];
	local uiQualtiyText = objs[8];
	local uiAttackText = objs[9];
	local uiDefenseText = objs[10];
	local uiTroopsText = objs[11];
	local uiLevelText = objs[12];
	
	-- 空对象
	if pHero == nil then
		SetTrue( uiColorBack )
		SetTrue( uiShape )
		SetTrue( uiNameBack )
		SetTrue( uiName )
		SetFalse( uiHidBack )
		SetFalse( uiLevel )
		SetFalse( uiCorps )
		SetFalse( uiType )
		SetImage( uiColorBack, HeroColorSprite(0) )
		SetImage( uiNameBack, HeroNameColorSprite(0) )
		SetImage( uiShape, HeroFaceSprite(0) )
		SetText( uiName, "" )
		
		if m_path == HEROLIST_PATH_HERO or m_path == HEROLIST_PATH_HERO_LIST then -- 上阵武将
			if index == 3 then
				if GetPlayer().m_attr.m_hero_up_num < 1 then -- 科技增加
					--SetText( uiName, T(610) )
					--SetImage( uiShape, LoadSprite("ui_icon_back_2") )
					uiHeroObj:GetComponent("UIButton").controlID = 30001;
				else
					SetText( uiName, "" )
					--SetImage( uiShape, LoadSprite("ui_icon_back_4") )
				end

			elseif index == 4 then
				if GetPlayer().m_attr.m_hero_up_num < 2 then -- 科技增加
					--SetText( uiName, T(611) )
					--SetImage( uiShape, LoadSprite("ui_icon_back_2") )
					uiHeroObj:GetComponent("UIButton").controlID = 30002;
				else
					--SetText( uiName, "" )
					--SetImage( uiShape, LoadSprite("ui_icon_back_4") )
				end
			end
			
		elseif m_path == HEROLIST_PATH_HERO_GATHER or m_path == HEROLIST_PATH_HERO_GUARD then -- 财赋署武将 -- 御林卫武将
			if index == 1 then
				if GetPlayer().m_level < global.hero_cabinet_level1 then -- 等级0开放
					SetText( uiName, F(1486,global.hero_cabinet_level1) )
					--SetImage( uiShape, LoadSprite("ui_icon_back_2") )
				else
					SetText( uiName, "" )
					--SetImage( uiShape, LoadSprite("ui_icon_back_4") )
				end
			elseif index == 2 then
				if GetPlayer().m_level < global.hero_cabinet_level2 then -- 等级100开放
					SetText( uiName, F(1486,global.hero_cabinet_level2) )
					--SetImage( uiShape, LoadSprite("ui_icon_back_2") )
				else
					SetText( uiName, "" )
					--SetImage( uiShape, LoadSprite("ui_icon_back_4") )
				end
			elseif index == 3 then
				if GetPlayer().m_level < global.hero_cabinet_level3 then -- 等级105开放
					SetText( uiName, F(1486,global.hero_cabinet_level3) )
					--SetImage( uiShape, LoadSprite("ui_icon_back_2") )
				else
					SetText( uiName, "" )
					SetImage( uiShape, LoadSprite("ui_icon_back_4") )
				end

			elseif index == 4 then
				if GetPlayer().m_level < global.hero_cabinet_level4 then -- 等级110开放
					SetText( uiName, F(1486,global.hero_cabinet_level4) )
					--SetImage( uiShape, LoadSprite("ui_icon_back_2") )
				else
					SetText( uiName, "" )
					SetImage( uiShape, LoadSprite("ui_icon_back_4") )
				end
			end
		end
		return
	end
	uiHeroObj:GetComponent("UIButton").controlID = 10000 + pHero.m_offset;
	SetTrue( uiColorBack )
	SetTrue( uiShape )
	SetTrue( uiNameBack )
	SetTrue( uiName )
	SetFalse( uiHidBack )
	SetTrue( uiLevel )
	SetTrue( uiCorps )

	SetImage( uiColorBack, HeroColorSprite(pHero.m_color) )
	SetImage( uiNameBack, HeroNameColorSprite(pHero.m_color) )
	SetImage( uiShape, HeroFaceSprite(pHero.m_kind) )
	SetImage( uiCorps, CorpsSprite( pHero.m_corps ) )
	
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
	
	SetText( uiName, HeroName( pHero.m_kind ) )
	SetLevel( uiLevelText, pHero.m_level )
	SetText( uiQualtiyText,F(1930,pHero.m_qualtiy,pHero.m_washqualtiy) );
	SetText( uiAttackText, pHero.m_attack );
	SetText( uiDefenseText, pHero.m_defense );
	SetText( uiTroopsText, pHero.m_troops );
	if b_OpenCloseInfo == true then
		SetTrue(uiHidBack);
		--SetFalse(uiCorps);
	else
		SetFalse(uiHidBack);
		--SetTrue(uiCorps);
	end
	
	if IsGuiding() then
		if GetCurrentGuideType() == GUIDE_ZJ then
			if pHero.m_kind == 12 then
				FindCmdTpye(uiShape.transform);
			end
		end
	end
end

-- 选择
function HeroListDlgSelect( offset )
	local up = true;
	local pHero = nil;
	if offset < 10000 then
		-- 上阵
		up = true;
		pHero = GetHero().m_CityHero[offset];
	else
		-- 空闲
		up = false
		pHero = GetHero().m_Hero[offset-10000];
	end
	if pHero == nil then
		return;
	end
	m_SelectHeroKind = pHero.m_kind;
	if m_SelectHeroKind <= 0 then
		return
	end
	if IsGuiding() then
		if GetCurrentGuideType() == GUIDE_ZJ then
			if m_SelectHeroKind == 12 then
				GuideNext();
			end
		end
	end
	m_isUpdate = 0;
	HeroInfoDlgShow( m_path, pHero, up )
end

--设置ToggleGroup的隐藏显示
function HeroListDlgSetTogGroup()
	local b_toggle = m_uiToggleRank.transform:GetComponent( "UIToggle" ).isOn;
	if b_toggle == true then
		SetTrue(m_uiTooglGroup);
	else
		SetFalse(m_uiTooglGroup);
	end
end
--选择排序方式
function HeroListDlgSelectRankWay(number)
	for i=1,m_uiTooglGroup.transform.childCount,1 do
		local obj = m_uiTooglGroup.transform:GetChild(i-1):Find("TogGround");
		if i == number then
			SetTrue(obj);
		else
			SetFalse(obj);
		end

		if  i == 1 then
			SetText(m_uiRankWayText,T(1925));
		elseif i == 2 then
			SetText(m_uiRankWayText,T(1926));
		elseif i == 3 then
			SetText(m_uiRankWayText,T(1927));
		end
	end
	if  number == 1 then
		SetText(m_uiRankWayText,T(1925));
	elseif number == 2 then
		SetText(m_uiRankWayText,T(1926));
	elseif number == 3 then
		SetText(m_uiRankWayText,T(1927));
	end	
	HeroListDlgSelectCloseToggle();
end
function HeroListDlgSelectCloseToggle()
	local b_toggle = m_uiToggleRank.transform:GetComponent( "UIToggle" );
	if b_toggle.isOn == true then
		b_toggle.isOn=false;
	end
end

-- 获取排序类型
function HeroListDlgGetRankType()
	return m_rankType
end

--显示资质
function HeroListDlgOpenInfo()
	local b_close = IsActive(m_uiCloseInfoBtu);
	if b_close == true then
		SetFalse(m_uiCloseInfoBtu);
	end
	local b_open = IsActive(m_uiOpenInfoBtu)
	if b_open == false then
		SetTrue(m_uiOpenInfoBtu);
	end
	b_OpenCloseInfo = true;
	for k,v in pairs(m_CacheHeroList) do
		local obj = v.transform:Find("HidBack");
		--local uiCorps = v.transform:Find("Corps");
		SetTrue(obj);
		--SetFalse(uiCorps);
	end
end
--隐藏资质 
function HeroListDlgCloseInfo()
	local b_open = IsActive(m_uiOpenInfoBtu)
	if b_open == true then
		SetFalse(m_uiOpenInfoBtu);
	end
	local b_close = IsActive(m_uiCloseInfoBtu);
	if b_close == false then
		SetTrue(m_uiCloseInfoBtu);
	end
	b_OpenCloseInfo = false;
		for k,v in pairs(m_CacheHeroList) do
		local obj = v.transform:Find("HidBack");
		--local uiCorps = v.transform:Find("Corps");
		SetFalse(obj);
		--SetTrue(uiCorps);
	end
end

-- 创建国家名将寻访
function HeroListDlgCreateNationHeroVisit()
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	SetFalse( m_uiUIP_NationHeroVisit[1] )
	SetFalse( m_uiUIP_NationHeroVisit[2] )
	SetFalse( m_uiUIP_NationHeroVisit[3] )
	SetFalse( m_uiUIP_NationHeroVisit[4] )
	
	local index = 1;
	for i=0,NATIONHERO_MAX-1,1 do
		local info = GetHero().m_NationHero[i];
		if info.m_state == NATIONHERO_STATE_CATCH then
			local uiObj = m_uiUIP_NationHeroVisit[index]
			SetTrue( m_uiUIP_NationHeroVisit[1] )
			local kind = info.m_kind
			local pHero = GetHero():GetPtr( kind )
			if pHero ~= nil then
				SetControlID( uiObj.transform:Find("CallBtn"), 1000+kind )
				SetImage( uiObj.transform:Find("Shape"), HeroHeadSprite(kind) )
				SetImage( uiObj.transform:Find("Corps"), CorpsSprite( hero_getcorps(kind) ) )
				SetImage( uiObj.transform:Find("Color"), ItemColorSprite(pHero.m_color) )
				SetText( uiObj.transform:Find("Name"), "Lv."..pHero.m_level.." "..Nation( g_nation_heroinfo[kind].nation ).."·"..HeroName(kind) )
				
				local cost = T(2350)
				if g_nation_heroinfo[kind].call_silver > 0 then
					cost = cost .. T(121).."x"..g_nation_heroinfo[kind].call_silver;
				end
				if g_nation_heroinfo[kind].call_itemkind0 > 0 then
					cost = cost .. "" ..item_getname(g_nation_heroinfo[kind].call_itemkind0).."x"..g_nation_heroinfo[kind].call_itemnum;
				end
				if g_nation_heroinfo[kind].call_itemkind1 > 0 then
					cost = cost .. "," ..item_getname(g_nation_heroinfo[kind].call_itemkind1).."x"..g_nation_heroinfo[kind].call_itemnum;
				end
				if g_nation_heroinfo[kind].call_itemkind2 > 0 then
					cost = cost .. "," ..item_getname(g_nation_heroinfo[kind].call_itemkind2).."x"..g_nation_heroinfo[kind].call_itemnum;
				end
				if g_nation_heroinfo[kind].call_itemkind3 > 0 then
					cost = cost .. "," ..item_getname(g_nation_heroinfo[kind].call_itemkind3).."x"..g_nation_heroinfo[kind].call_itemnum;
				end
				SetText( uiObj.transform:Find("Cost"), cost )
				
				local left = info.m_runstamp-GetServerTime()
				SetTimer( uiObj.transform:Find("Timer"), left, left, 0, T(2349) )
				index = index + 1;
			end
		end
	end 
end

-- 国家名将招募
function HeroListDlgNationHeroCall( kind )
	NationHeroCallDlgShow( kind )
end
