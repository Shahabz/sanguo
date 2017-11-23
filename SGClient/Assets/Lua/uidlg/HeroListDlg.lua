-- 界面
local m_Dlg = nil;
local m_DialogFrameMod = nil;
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiUIP_HeroRow = nil; --UnityEngine.GameObject
local m_uiToggleRank = nil; --UnityEngine.GameObject toggle按钮
local m_uiTooglGroup = nil; --UnityEngine.GameObject
local m_uiGeneralBtu = nil; --UnityEngine.GameObject
local m_uiOpenInfoBtu = nil; --UnityEngine.GameObject 显示资质
local m_uiCloseInfoBtu = nil; --UnityEngine.GameObject 隐藏资质
local m_uiRankWayText = nil; --UnityEngine.GameObjec 显示排序方法的Text
local m_uiEmptyHeroContent = nil; --UnityEngine.GameObject
local m_CacheHeroCache = {}
local m_CacheHeroList = {}
local m_SelectHeroKind = 0;
local m_SelectRankWay = 0;
local b_OpenCloseInfo = false;
	
-- 打开界面
function HeroListDlgOpen()
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
		elseif nControlID == 6 then
			--武将寻访
		elseif nControlID >= 10000 then
			HeroListDlgSelect(nControlID-10000);
			HeroListDlgSelectCloseToggle();
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
	m_uiGeneralBtu = objs[4];
	m_uiOpenInfoBtu = objs[5];
	m_uiCloseInfoBtu = objs[6];
	m_uiRankWayText = objs[7];
	m_uiEmptyHeroContent = objs[8];

--[[
	if b_OpenCloseInfo == true then
		HeroListDlgCloseInfo();
	end
--]]
	--m_uiToggleRank.transform:GetComponent("UIToggle").ControlID = 1;
    --m_uiGeneralBtu.transform:GetComponent( "UIButton").ControlID = 1;
end

-- 界面初始化时调用
function HeroListDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function HeroListDlgOnEnable( gameObject )
	
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
function HeroListDlgShow()
	HeroListDlgOpen()
	HeroListDlgLoadHero();
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
		local uiNameBack = objs[4];
		local uiName = objs[5];
		local uiBack = objs[6];
		
		SetFalse( uiBorder )
		SetFalse( uiShape )
		SetFalse( uiColor )
		SetFalse( uiCorps )
		SetFalse( uiNameBack )
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

    -- 先放进临时缓存
    for offset = 0, 3, 1 do
        local pHero = GetHero().m_CityHero[offset];
        if pHero ~= nil and pHero.m_kind > 0 then
			local base = pHero.m_attack_base+pHero.m_defense_base+pHero.m_troops_base	;
			local wash = pHero.m_attack_wash+pHero.m_defense_wash+pHero.m_troops_wash	;
            table.insert(m_CacheHeroCache, { m_kind = pHero.m_kind, m_color = pHero.m_color, m_level = pHero.m_level, m_corps = pHero.m_corps, m_offset = offset,m_qualtiy = base,m_washqualtiy = wash,m_attack=pHero.m_attack,m_defense=pHero.m_defense,m_troops=pHero.m_troops} );
        end
    end
	
	-- 先放进临时缓存			
    for offset = 0, MAX_HERONUM-1, 1 do
        local pHero = GetHero().m_Hero[offset];
        if pHero ~= nil and pHero.m_kind > 0 then
			local base = pHero.m_attack_base+pHero.m_defense_base+pHero.m_troops_base	;
			local wash = pHero.m_attack_wash+pHero.m_defense_wash+pHero.m_troops_wash
            table.insert(m_CacheHeroCache, { m_kind = pHero.m_kind, m_color = pHero.m_color, m_level = pHero.m_level, m_corps = pHero.m_corps, m_offset = 10000+offset,m_qualtiy = base,m_washqualtiy = wash,m_attack=pHero.m_attack,m_defense=pHero.m_defense,m_troops=pHero.m_troops });
        end
    end

    -- 排序
    table.sort(m_CacheHeroCache, HeroListDlgLevelSort);

    -- 创建上阵对象
	local upHeroRow = m_uiContent.transform:GetChild(1).gameObject;
	HeroListDlgClearHeroRow(upHeroRow,true);
	local index = 1;
    for i=1, #m_CacheHeroCache, 1 do
		local pHero = m_CacheHeroCache[i];
        if pHero.m_offset < 10000 then
			m_CacheHeroList[pHero.m_offset] = upHeroRow.transform:GetChild(index);
            HeroListDlgSetHero( m_CacheHeroList[pHero.m_offset], pHero )
			index = index + 1
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
            HeroListDlgSetHero( uiHeroObj, pHero )
			m_CacheHeroList[pHero.m_offset] = uiHeroObj;
			heroCount = heroCount + 1;
        end
    end
end

function HeroListDlgRankWay( uiRanKum )
    local heroCount = 0;
    local currHeroRow = nil;
	
	if uiRanKum == 1 then
	    table.sort(m_CacheHeroCache,HeroListDlgQualtiySort);
	elseif uiRanKum == 2 then
	    table.sort(m_CacheHeroCache, HeroListDlgTalentSort);
	elseif uiRanKum == 3 then
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
            HeroListDlgSetHero( uiHeroObj, pHero )
			m_CacheHeroList[pHero.m_offset] = uiHeroObj;
			heroCount = heroCount + 1;
        end
    end
end

function HeroListDlgQualtiySort(a,b)
	if a ~= nil and b ~= nil then
        if a.m_color > b.m_color then
            return true;
        else
            return false;
        end
    else
        return false;
    end	
end
function HeroListDlgTalentSort(a,b)
	if a ~= nil and b ~= nil then
        if a.m_qualtiy > b.m_qualtiy then
            return true;
        else
            return false;
        end
    else
        return false;
    end	
end
function HeroListDlgLevelSort(a,b)
	if a ~= nil and b ~= nil then
        if a.m_level > b.m_level then
            return true;
        else
            return false;
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
function HeroListDlgSetHero( uiHeroObj, pHero )
	uiHeroObj:GetComponent("UIButton").controlID = 10000 + pHero.m_offset;
	local objs = uiHeroObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiBorder = objs[0];
	local uiShape = objs[1];
	local uiColor = objs[2];
	local uiCorps = objs[3];
	local uiNameBack = objs[4];
	local uiName = objs[5];
	local uiBack = objs[6]
	local uiHidBack = objs[7];
	local uiQualtiyText = objs[8];
	local uiAttackText = objs[9];
	local uiDefenseText = objs[10];
	local uiTroopsText = objs[11];
	
	SetTrue( uiBack )
	SetTrue( uiShape )
	SetTrue( uiColor )
	SetTrue( uiNameBack )
	SetTrue( uiName )
	SetTrue( uiBorder )
	SetFalse(uiHidBack)	

	SetImage( uiShape, HeroHeadSprite(pHero.m_kind) )
	SetImage( uiColor, ItemColorSprite(pHero.m_color) )
	SetImage( uiCorps, CorpsSprite(pHero.m_corps) )
	SetText( uiName, HeroNameLv( pHero.m_kind, pHero.m_level ) )
	print(".."..pHero.m_qualtiy..".."..pHero.m_washqualtiy);
	SetText(uiQualtiyText,F(1930,pHero.m_qualtiy,pHero.m_washqualtiy));
	SetText(uiAttackText,pHero.m_attack);
	SetText(uiDefenseText,pHero.m_defense);
	SetText(uiTroopsText,pHero.m_troops);
	if b_OpenCloseInfo == true then
		SetTrue(uiHidBack);
		SetFalse(uiCorps);
	else
		SetFalse(uiHidBack);
		SetTrue( uiCorps );
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
	HeroInfoDlgShow( 1, pHero, up )
end

--设置ToggleGroup的隐藏显示
function HeroListDlgSetTogGroup()
	local b_toggle = m_uiToggleRank.transform:GetComponent( "UIToggle"  ).isOn;
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
	local b_toggle = m_uiToggleRank.transform:GetComponent( "UIToggle"  );
	if b_toggle.isOn == true then
		b_toggle.isOn=false;
	end
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
		local uiCorps = v.transform:Find("Corps");
		SetTrue(obj);
		SetFalse(uiCorps);
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
		local uiCorps = v.transform:Find("Corps");
		SetFalse(obj);
		SetTrue(uiCorps);
	end
end
