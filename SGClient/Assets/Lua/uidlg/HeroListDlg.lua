-- 界面
local m_Dlg = nil;
local m_DialogFrameMod = nil;
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiUIP_HeroRow = nil; --UnityEngine.GameObject

local m_CacheHeroCache = {}
local m_CacheHeroList = {}

local m_SelectHeroKind = 0;
	
-- 打开界面
function HeroListDlgOpen()
	m_Dlg = eye.uiManager:Open( "HeroListDlg" );
	m_DialogFrameMod = DialogFrameModOpen( m_Dlg, T(35), 2, HeroListDlgClose );
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
		elseif nControlID >= 1000 then
			HeroListDlgSelect(nControlID-1000);
        end
	end
end

-- 载入时调用
function HeroListDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiContent = objs[0];
	m_uiUIP_HeroRow = objs[1];
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
    if m_uiContent ~= nil then
        local childCount = m_uiContent.transform.childCount;
        for i = 3, childCount - 1, 1 do
            local child = m_uiContent.transform:GetChild(i).gameObject;
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
		local uiBack = objs[0];
		local uiShape = objs[1];
		local uiColor = objs[2];
		local uiCorps = objs[3];
		local uiNameBack = objs[4];
		local uiName = objs[5];
		
		SetFalse( uiBack )
		SetFalse( uiShape )
		SetFalse( uiColor )
		SetFalse( uiCorps )
		SetFalse( uiNameBack )
		SetFalse( uiName )
		
		if up == true then
			SetTrue( uiBack )
		end
		child:GetComponent("UIButton").controlID = -1;
    end
end

-- 读取缓存英雄
function HeroListDlgLoadHero()
	if m_Dlg == nil then
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

    -- 排序
    table.sort(m_CacheHeroCache, HeroListCacheSort);

    -- 创建上阵对象
	local index = 0;
    for i, v in pairs( m_CacheHeroCache ) do
		local pHero = v;
        if pHero ~= nil and pHero.m_kind > 0 and pHero.m_offset < 100 then
			local currHeroRow = m_uiContent.transform:GetChild(1).gameObject;
			HeroListDlgClearHeroRow(currHeroRow,true);
			local uiHeroObj = currHeroRow.transform:GetChild(index+1);
            HeroListDlgSetHero( uiHeroObj, pHero )
			m_CacheHeroList[pHero.m_offset] = uiHeroObj;
		end
	end
	
	-- 创建空闲对象
	rowCount = 3
	for i, v in pairs( m_CacheHeroCache ) do
        local pHero = v;
        if pHero ~= nil and pHero.m_kind > 0 and pHero.m_offset >= 100 then
            local _index = heroCount % 4;
            if _index == 0 then
                if rowCount >= m_uiContent.transform.childCount then
                    currHeroRow = GameObject.Instantiate(m_uiUIP_HeroRow);
                    currHeroRow.transform:SetParent(m_uiContent.transform);
                    currHeroRow.transform.localScale = Vector3.one;
                    currHeroRow:SetActive(true);
                else
                    currHeroRow = m_uiContent.transform:GetChild(rowCount).gameObject;
                    currHeroRow:SetActive(true);
                end
				HeroListDlgClearHeroRow(currHeroRow,false);
                rowCount = rowCount + 1;
            end
            local uiHeroObj = currHeroRow.transform:GetChild(_index+1);
            HeroListDlgSetHero( uiHeroObj, pHero )
			m_CacheHeroList[pHero.m_offset] = uiHeroObj;
			heroCount = heroCount + 1;
        end
    end
end

-- 设置单个对象
function HeroListDlgSetHero( uiHeroObj, pHero )
	uiHeroObj:GetComponent("UIButton").controlID = 1000 + pHero.m_offset;
	local objs = uiHeroObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiBack = objs[0];
	local uiShape = objs[1];
	local uiColor = objs[2];
	local uiCorps = objs[3];
	local uiNameBack = objs[4];
	local uiName = objs[5];
	
	SetTrue( uiBack )
	SetTrue( uiShape )
	SetTrue( uiColor )
	SetTrue( uiCorps )
	SetTrue( uiNameBack )
	SetTrue( uiName )
	
	SetImage( uiShape, HeroHeadSprite(pHero.m_kind) )
	SetImage( uiColor, ItemColorSprite(pHero.m_color) )
	SetImage( uiCorps, CorpsSprite(pHero.m_corps) )
	SetText( uiName, HeroNameLv( pHero.m_kind, pHero.m_level ) )
end

-- 选择
function HeroListDlgSelect( offset )
	local up = true;
	local pHero = nil;
	if offset < 100 then
		-- 上阵
		up = true;
		pHero = GetHero().m_CityHero[offset];
	else
		-- 空闲
		up = false
		pHero = GetHero().m_Hero[offset-100];
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

