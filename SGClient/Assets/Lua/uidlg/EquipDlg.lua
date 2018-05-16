-- 界面
local m_Dlg = nil;
local m_DialogFrameMod = nil;

local m_uiEquipScroll = nil; --UnityEngine.GameObject
local m_uiEquipContent = nil; --UnityEngine.GameObject
local m_uiUIP_EquipRow = nil; --UnityEngine.GameObject
local m_uiEquipInfo = nil; --UnityEngine.GameObject
local m_uiEquipExt = nil; --UnityEngine.GameObject
local m_uiEquipExtButton = nil; --UnityEngine.GameObject

local m_SelectEquipIndex = -1;
local m_SelectHeroKind = 0;
local m_equiptype = -1;
local m_EquipHeroIndex = -1;

-- 数据缓存
local m_CacheEquipCache = {};

-- 对象缓存
local m_CacheEquipList = {};

-- 打开界面
function EquipDlgOpen()
	m_Dlg = eye.uiManager:Open( "EquipDlg" );
	m_DialogFrameMod = DialogFrameModOpen( m_Dlg, T(650), HELP_BagDlg, EquipDlgClose );
end

-- 隐藏界面
function EquipDlgClose()
	if m_Dlg == nil then
		return;
	end
	DialogFrameModClose( m_DialogFrameMod );
	m_DialogFrameMod = nil;
	eye.uiManager:Close( "EquipDlg" );
	m_CacheEquipCache = {};
	m_CacheEquipList = {};
end

-- 删除界面
function EquipDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
	m_CacheEquipCache = {};
	m_CacheEquipList = {};
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function EquipDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            EquipDlgClose();
			
		-- 购买容量
		elseif nControlID == 10 then
			EquipDlgEquipBuyEquipExt()
		
		-- 分解
		elseif nControlID == 21 then
			EquipDlgEquipResolve()
		
		-- 装备
		elseif nControlID == 22 then
			EquipDlgEquipUp()
			
		-- 替换
		elseif nControlID == 23 then
			EquipDlgEquipUp()
			
		-- 洗炼
		elseif nControlID == 24 then
			EquipWashDlgShow()
			EquipDlgClose();
			
		-- 已装备
		elseif nControlID >= 31 and nControlID <= 34 then
				
		-- 选择装备
		elseif nControlID >= 2000 and nControlID < 3000 then
			EquipDlgSelectEquip( nControlID-2000 )
			
		-- 选择穿着装备
		elseif nControlID >= 3000 and nControlID < 4000 then
			EquipDlgSelectHeroEquip( nControlID-3000 )
        end
	end
end

-- 载入时调用
function EquipDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiEquipScroll = objs[0];
	m_uiEquipContent = objs[1];
	m_uiUIP_EquipRow = objs[2];
	m_uiEquipInfo = objs[3];
	m_uiEquipExt = objs[4];
	m_uiEquipExtButton = objs[5];
end

-- 界面初始化时调用
function EquipDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function EquipDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function EquipDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function EquipDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function EquipDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function EquipDlgShow()
	EquipDlgOpen()
	EquipDlgSelectEquip( -1 )
	EquipDlgLoadEquip()
end

-- 缓存排序
function EquipDlgEquipCacheSort(a, b)
    if a ~= nil and b ~= nil then
        if a.m_color < b.m_color then
            return true;
        else
            return false;
        end
    else
        return false;
    end
end

-- 清空装备对象
function EquipDlgClearEquip()
	local childCount = m_uiEquipContent.transform.childCount;
	for i = 0, childCount - 1, 1 do
		local child = m_uiEquipContent.transform:GetChild(i).gameObject;
		EquipDlgClearEquipRow(m_uiEquipContent.transform:GetChild(i).gameObject)
	end
    m_CacheEquipCache = { };
    m_CacheEquipList = { };
end

-- 清除一行的状态为初始的显示
function EquipDlgClearEquipRow(row)
    local trans = row.transform;
    for tmpi = 0, trans.childCount - 1, 1 do
        local child = trans:GetChild(tmpi);
		local objs = child.transform:GetComponent( typeof(Reference) ).relatedGameObject;
		local uiShape = objs[0];
		local uiColor = objs[1];
		local uiNew = objs[2];
		local uiWash = objs[3];
		local uiSelect = objs[4];
		SetImage( uiShape, LoadSprite("ui_icon_back_1") )
		SetTrue( uiShape )
		SetFalse( uiColor )
		SetFalse( uiNew )
		SetFalse( uiWash )
		SetFalse( uiSelect )
		child:GetComponent("UIButton").controlID = 0;
    end
end

-- 读取装备缓存
function EquipDlgLoadEquip()
	if m_Dlg == nil then
		return;
	end
	if IsActive( m_uiEquipScroll ) == false then
		return;
	end
    EquipDlgClearEquip()
	EquipDlgEquipExtSet();
    local equipCount = 0;
    local currEquipRow = nil;
    local rowCount = 0;
	
    -- 先放进临时缓存
    for nEquipIndex = 1, MAX_EQUIPNUM, 1 do
        local pEquip = GetEquip().m_Equip[nEquipIndex];
        if pEquip ~= nil and pEquip.m_kind > 0 then
            table.insert(m_CacheEquipCache, { m_kind = pEquip.m_kind, m_color = equip_getcolor(pEquip.m_kind), m_bIsNew = pEquip.m_bIsNew, m_offset = nEquipIndex, m_pEquip = pEquip });
        end
    end
	
	-- 一个装备都没有
	if #m_CacheEquipCache == 0 then
		return;
	end
	
    -- 排序
    table.sort(m_CacheEquipCache, EquipDlgEquipCacheSort);

    -- 创建排数对象
	local MaxEquipNum = GetPlayer().m_equipext+MAX_DEFAULT_EQUIPNUM
	local MaxRow = math.floor( MaxEquipNum/5 ); -- 最大排数
	local NowRow =  m_uiEquipContent.transform.childCount;-- 当前排数
	for i=1, MaxRow-NowRow, 1 do
		currEquipRow = GameObject.Instantiate(m_uiUIP_EquipRow);
		currEquipRow.transform:SetParent(m_uiEquipContent.transform);
		currEquipRow.transform.localScale = Vector3.one;
		currEquipRow:SetActive(true);
		EquipDlgClearEquipRow(currEquipRow);
	end
	
	-- 创建对象
    for nEquipIndex = 1, #m_CacheEquipCache, 1 do
		local equipIndex = equipCount % 5;
		if equipIndex == 0 then
			currEquipRow = m_uiEquipContent.transform:GetChild(rowCount).gameObject;
			EquipDlgClearEquipRow(currEquipRow);
			rowCount = rowCount + 1;
		end
			
        local pEquip = m_CacheEquipCache[nEquipIndex];
        if pEquip ~= nil and pEquip.m_kind > 0 then
            local uiEquipObj = currEquipRow.transform:GetChild(equipIndex);
            uiEquipObj:GetComponent("UIButton").controlID = 2000 + pEquip.m_offset;
			local objs = uiEquipObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
			local uiShape = objs[0];
			local uiColor = objs[1];
			local uiNew = objs[2];
			local uiWash = objs[3];
			local uiSelect = objs[4];
			
			SetTrue( uiShape )
			SetTrue( uiColor )
			SetShow( uiNew, pEquip.m_bIsNew )
			SetTrue( uiWash )
			SetFalse( uiSelect )
			
			SetImage( uiShape, EquipSprite(pEquip.m_kind) )
			SetImage( uiColor, ItemColorSprite(equip_getcolor(pEquip.m_kind)) )
			SetEquipWash( uiWash, pEquip.m_pEquip );
			
			m_CacheEquipList[pEquip.m_offset] = uiEquipObj;
			equipCount = equipCount + 1;
        end
    end
end

-- 选择装备
function EquipDlgSelectEquip( offset )
	-- 关掉上一个选择框
	local uiLastEquipObj = m_CacheEquipList[m_SelectEquipIndex];
    if uiLastEquipObj ~= nil then
        uiLastEquipObj.transform:Find("Select").gameObject:SetActive(false);
	end	
	m_uiEquipInfo.transform:SetParent( m_uiEquipContent.transform.parent.transform );
	
	m_EquipHeroIndex = -1
	if offset == -1 or offset == m_SelectEquipIndex then
		SetFalse( m_uiEquipInfo );
		m_SelectEquipIndex = -1
		return
	end
	m_SelectEquipIndex = offset;
		
	-- 获取装备
    local pEquip = GetEquip():GetAnyEquip(m_SelectEquipIndex - 1);
    if pEquip == nil then
        return;
    end
	
	-- 装备对象
	local uiEquipObj = m_CacheEquipList[m_SelectEquipIndex];
    if uiEquipObj == nil then
        return;
    end
	pEquip.m_bIsNew = false;
    SetFalse( uiEquipObj.transform:Find("New") );
	SetTrue( uiEquipObj.transform:Find("Select") )
	m_uiEquipInfo.transform:SetParent( m_uiEquipContent.transform );
	
	 -- 选择的道具位置
    local siblingIndex = uiEquipObj.parent:GetSiblingIndex();
    if m_uiEquipInfo.transform:GetSiblingIndex() < siblingIndex then
        m_uiEquipInfo.transform:SetSiblingIndex(siblingIndex);
    else
        m_uiEquipInfo.transform:SetSiblingIndex(siblingIndex + 1);
    end
	SetTrue( m_uiEquipInfo );
	-- 判断弹出菜单是否显示在最下面
    local uiEquipParent = uiEquipObj.parent.transform;
    local posY = - uiEquipParent.localPosition.y - m_uiEquipContent.transform.offsetMax.y;
    if posY + 400 > m_uiEquipScroll.transform.rect.height then
        local siblingIndex = m_uiEquipInfo.transform:GetSiblingIndex();
        m_uiEquipScroll:GetComponent("UIScrollRect"):ScrollToBottom(siblingIndex);
    end
	
	EquipDlgSetEquipInfo( pEquip )
end

function EquipDlgSetEquipInfo( pEquip )
	local objs = m_uiEquipInfo.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local EquipName = objs[0];
	local EquipDesc = objs[1];
	local WashLevel = objs[2];
	local SellButton = objs[3];
	local UseButton = objs[4];
	
	m_HeroEquip = {};
	m_HeroEquip[1] = objs[5];
	m_HeroEquip[2] = objs[6];
	m_HeroEquip[3] = objs[7];
	m_HeroEquip[4] = objs[8];
	local ReplaceButton = objs[9];
	local WashButton = objs[10];
			
	SetText( EquipName, equip_getname( pEquip.m_kind ), NameColor( equip_getcolor(pEquip.m_kind) ) );
	SetText( EquipDesc, equip_getabilityname( pEquip.m_kind ) );
	SetEquipWashLevel( WashLevel, pEquip );
	
	-- 装备类型1-6 变成0-5
	m_equiptype = equip_gettype( pEquip.m_kind )-1
	
	-- 上阵的英雄分别穿的同部位装备
	for i=0,3,1 do
		local objs = m_HeroEquip[i+1].transform:GetComponent( typeof(Reference) ).relatedGameObject;
		local uiShape = objs[0]
		local uiColor = objs[1]
		local uiHeroName = objs[2]
		local uiSelect = objs[3]
		local uiTips = objs[4]
		local uiWash = objs[5]
		
		SetControlID( m_HeroEquip[i+1], 3000 + i )
		SetFalse( uiSelect )
		SetFalse( uiTips )
		
		if GetHero().m_CityHero[i].m_kind <= 0 then
			SetFalse( uiShape )
			SetFalse( uiColor )
			SetFalse( uiHeroName )
			SetFalse( uiWash )
		else
			local hEquip = GetHero().m_CityHero[i].m_Equip[m_equiptype]
			if hEquip.m_kind <= 0 then
				SetFalse( uiShape )
				SetFalse( uiColor )
				SetFalse( uiWash )
			else
				SetTrue( uiShape )
				SetTrue( uiColor )
				SetTrue( uiWash )
				SetImage( uiShape, EquipSprite( hEquip.m_kind ) )
				SetImage( uiColor, ItemColorSprite( equip_getcolor( hEquip.m_kind ) ) )
				SetEquipWash( uiWash, hEquip );
			end
			SetTrue( uiHeroName )
			SetText( uiHeroName, HeroName( GetHero().m_CityHero[i].m_kind ), NameColor( GetHero().m_CityHero[i].m_color ) )
		end
		
	end
	
	-- 分解
	if equip_getprestige( pEquip.m_kind ) > 0 then
		SetTrue( SellButton );
	else
		SetFalse( SellButton );
	end
	
	SetFalse( UseButton ); -- 装备
	SetFalse( ReplaceButton ); -- 替换
	-- 洗炼
	if equip_getcolor(pEquip.m_kind) >= ITEM_COLOR_LEVEL_GREEN then
		SetTrue( WashButton );
	else
		SetFalse( WashButton );
	end
end

-- 选择穿着装备
function EquipDlgSelectHeroEquip( index )
	local objs = m_uiEquipInfo.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local SellButton = objs[3];
	local UseButton = objs[4];
	local ReplaceButton = objs[9];
	local WashButton = objs[10];
	SetFalse( ReplaceButton ); -- 替换
	SetFalse( UseButton ); -- 装备
	SetFalse( WashButton ); -- 洗练
	SetFalse( SellButton ); -- 分解
	
	if index == -1 then 
		local pEquip = GetEquip():GetAnyEquip(m_SelectEquipIndex - 1);
		if pEquip then -- 重新设置，偷懒
			EquipDlgSetEquipInfo( pEquip )
		end
		m_SelectHeroKind = 0;
		return;
	end
	
	if m_EquipHeroIndex == index then
		EquipDlgSelectHeroEquip( -1 );
		return
	end
	m_EquipHeroIndex = index;
	m_SelectHeroKind = GetHero().m_CityHero[index].m_kind;
	
	for i=0,3,1 do
		local objs = m_HeroEquip[i+1].transform:GetComponent( typeof(Reference) ).relatedGameObject;
		local uiSelect = objs[3]
		local uiTips = objs[4]
		if index == i then
			SetTrue( uiSelect )	
			SetTrue( uiTips )	
			
			if GetHero().m_CityHero[i].m_kind <= 0 or GetHero().m_CityHero[i].m_Equip[m_equiptype].m_kind <= 0 then
				SetText( uiTips.transform:Find( "Name" ), T(822) )
				SetText( uiTips.transform:Find( "Ability" ) )
				SetFalse( uiTips.transform:Find( "WashLevel" ) )
				SetFalse( ReplaceButton ); -- 替换
				SetTrue( UseButton ); -- 装备
			else
				local hEquip = GetHero().m_CityHero[i].m_Equip[m_equiptype]
				SetText( uiTips.transform:Find( "Name" ), EquipName( hEquip.m_kind ),NameColor( equip_getcolor(hEquip.m_kind) ) )
				SetText( uiTips.transform:Find( "Ability" ), equip_getabilityname( hEquip.m_kind ) )
				SetEquipWashLevel( uiTips.transform:Find( "WashLevel" ), hEquip );
				SetFalse( UseButton ); -- 装备
				SetTrue( ReplaceButton ); -- 替换
			end
	
		else
			SetFalse( uiSelect )
			SetFalse( uiTips )	
		end
	end
end

-- 装备分解
function EquipDlgEquipResolve()
	local pEquip = GetEquip():GetAnyEquip(m_SelectEquipIndex - 1);
    if pEquip == nil then
        return;
    end
	local kind = pEquip.m_kind
	local msg = "<color=#03de27ff>"..T(152).."x"..equip_getprestige( kind ).."</color>"
	
	if g_equipinfo[kind].resolve_kind0 > 0 then
		msg = msg.." <color=#FFDE00FF>"..item_getname( g_equipinfo[kind].resolve_kind0 ).."("..g_equipinfo[kind].resolve_min0.."-"..g_equipinfo[kind].resolve_max0..")</color>"
	end
	if g_equipinfo[kind].resolve_kind1 > 0 then
		msg = msg.." <color=#FFDE00FF>"..item_getname( g_equipinfo[kind].resolve_kind1 ).."("..g_equipinfo[kind].resolve_min1.."-"..g_equipinfo[kind].resolve_max1..")</color>"
	end
	if g_equipinfo[kind].resolve_kind2 > 0 then
		msg = msg.." <color=#FFDE00FF>"..item_getname( g_equipinfo[kind].resolve_kind2 ).."("..g_equipinfo[kind].resolve_min2.."-"..g_equipinfo[kind].resolve_max2..")</color>"
	end
	if g_equipinfo[kind].resolve_kind3 > 0 then
		msg = msg.." <color=#FFDE00FF>"..item_getname( g_equipinfo[kind].resolve_kind3 ).."("..g_equipinfo[kind].resolve_min3.."-"..g_equipinfo[kind].resolve_max3..")</color>"
	end
	-- 2322	50%几率返还{0}图纸
	if Utils.get_int_sflag( GetPlayer().m_actor_sflag, ACTOR_SFLAG_EQUPIPDRAWING ) == 1 then
		if g_equipinfo[kind].color > 0 then
			msg = msg.." <color=#03de27ff>"..F( 2322, equip_getname( kind ) ).."</color>"
		end
	end
	
	MsgBox( F( 689, msg ), function()
		system_askinfo( ASKINFO_EQUIP, "", 1, m_SelectEquipIndex - 1 );
		EquipDlgSelectEquip( -1 )
	end )
end

-- 装备上
function EquipDlgEquipUp()
	local pEquip = GetEquip():GetAnyEquip(m_SelectEquipIndex - 1);
    if pEquip == nil then
        return;
    end
	if m_SelectHeroKind <= 0 then
		return
	end
	system_askinfo( ASKINFO_EQUIP, "", 2, m_SelectHeroKind, m_SelectEquipIndex - 1 );
	EquipDlgSelectEquip( -1 )
end

-- 购买装备容量
function EquipDlgEquipBuyEquipExt()
	MsgBox( F( 685, global.equipext_token ), function()
		if GetPlayer().m_token < global.equipext_token then
			JumpToken()
		else
			system_askinfo( ASKINFO_EQUIP, "", 4 );
		end
	end )
end

-- 装备容量
function EquipDlgEquipExtSet()	
	SetText( m_uiEquipExt, T(683) .." ".. GetEquip():GetTotal() .."/" ..(GetPlayer().m_equipext+MAX_DEFAULT_EQUIPNUM) )
	if GetPlayer().m_equipext >= (MAX_EXT_EQUIPNUM-MAX_DEFAULT_EQUIPNUM) then
		SetFalse( m_uiEquipExtButton );
	else
		SetTrue( m_uiEquipExtButton )
	end
end
