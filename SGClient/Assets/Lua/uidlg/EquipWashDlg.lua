-- 界面
local m_Dlg = nil;
local m_DialogFrameMod = nil;
local m_uiTags = nil; --UnityEngine.GameObject
local m_uiHeros = nil; --UnityEngine.GameObject
local m_uiEquipScroll = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiUIP_Equip = nil; --UnityEngine.GameObject
local m_uiShape = nil; --UnityEngine.GameObject
local m_uiColor = nil; --UnityEngine.GameObject
local m_uiName = nil; --UnityEngine.GameObject
local m_uiAbility = nil; --UnityEngine.GameObject
local m_uiFreeTitle = nil; --UnityEngine.GameObject
local m_uiTokenTitle = nil; --UnityEngine.GameObject
local m_uiButtons = nil; --UnityEngine.GameObject
local m_uiWarn = nil; --UnityEngine.GameObject
local m_uiWashList = nil; --UnityEngine.GameObject
local m_uiTokenBtn = nil; --UnityEngine.GameObject
local m_uiAutoBtn = nil; --UnityEngine.GameObject
local m_uiSuperBtn = nil; --UnityEngine.GameObject
local m_uiWash = nil; --UnityEngine.GameObject
local m_uiTips = nil; --UnityEngine.GameObject

local m_uiTagBtn = {};
local m_uiHeroBtn = {};
local m_uiWashIcon = {};
local m_uiWashIconAdd = nil;
local m_CacheWashid = {};

local m_ObjectPool = nil;
local m_CacheHeroCache = {};
local m_CacheEquipCache = {};
local m_uiEquipList = {};
local m_selectTag = 0;
local m_herokind = 0;
local m_equipoffset = -1;
local m_color = 0;
local m_recvValue = {};
local m_washtoken = 0; 
local m_wash_toggle = false;
local m_wash_count = 0;

local m_auto_wash_toggle = false;
local m_auto_wash_count = 0;

local m_level_allfull = 1;
local m_level_allfull_num = 0;
local m_hassuper = 0;
local m_super_wash_toggle = false;
local m_showEffect = false;

-- 打开界面
function EquipWashDlgOpen()
	m_Dlg = eye.uiManager:Open( "EquipWashDlg" );
	m_DialogFrameMod = DialogFrameModOpen( m_Dlg, BuildingName(32), HELP_EquipWashDlg, EquipWashDlgClose );
end

-- 隐藏界面
function EquipWashDlgClose()
	if m_Dlg == nil then
		return;
	end
	DialogFrameModClose( m_DialogFrameMod );
	m_DialogFrameMod = nil;
	eye.uiManager:Close( "EquipWashDlg" );
	EquipWashDlgStopAuto()
end

-- 删除界面
function EquipWashDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function EquipWashDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
		-- 自动洗练中
		if m_auto_wash_count > 0 and nControlID ~= 12 then
			return
		end
		
        if nControlID == -1 then
            EquipWashDlgClose();
		elseif nControlID == -2 then
			EquipWashDlgWashTips( -1 )
			
		-- 选择标签	
		elseif nControlID >= 1 and nControlID <= 3 then
			EquipWashDlgSelectTag( nControlID, 0, -1 );
			
		-- 免费洗练
		elseif nControlID == 10 then
			EquipWashDlgFree()
			
		-- 钻石洗练
		elseif nControlID == 11 then
			EquipWashDlgToken()
		
		-- 停止自动洗练
		elseif nControlID == 12 then
			EquipWashDlgStopAuto()
			
		-- 秘技洗练
		elseif nControlID == 13 then
			EquipWashDlgSuper()
		
		-- 点击秘技tips
		elseif nControlID > 100 and nControlID < 200 then
			EquipWashDlgWashTips( nControlID - 100 )
		
		-- 选择英雄
		elseif nControlID >= 1000 and nControlID < 2000 then
			EquipWashDlgSelectHero( nControlID-1000, -1 )
		
		-- 选择背包
		elseif nControlID == 2000 then
			EquipWashDlgSelectHero( 2000, -1 )
		
		-- 选择装备
		elseif nControlID >= 10000 then
			EquipWashDlgSelectItem( nControlID-10000 )
        end
	elseif nType == UI_EVENT_TIMECOUNTEND then
		if nControlID == 1 then
			system_askinfo( ASKINFO_EQUIP, "", 5 );
		end	
	end
end

-- 载入时调用
function EquipWashDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiTags = objs[0];
	m_uiHeros = objs[1];
	m_uiEquipScroll = objs[2];
	m_uiContent = objs[3];
	m_uiUIP_Equip = objs[4];
	m_uiShape = objs[5];
	m_uiColor = objs[6];
	m_uiName = objs[7];
	m_uiAbility = objs[8];
	m_uiFreeTitle = objs[9];
	m_uiTokenTitle = objs[10];
	m_uiButtons = objs[11];
	m_uiWarn = objs[12];
	m_uiWashList = objs[13];
	m_uiTokenBtn = objs[14];
	m_uiAutoBtn = objs[15];
	m_uiSuperBtn = objs[16];
	m_uiWash = objs[17];
	m_uiTips = objs[18];
	
	for i=1, 3, 1 do
		m_uiTagBtn[i] = m_uiTags.transform:GetChild(i-1).gameObject;
	end
	
	for i=1, 5, 1 do
		m_uiHeroBtn[i] = m_uiHeros.transform:GetChild(i-1).gameObject;
	end
	
	m_uiWashIcon[1] = m_uiWashList.transform:GetChild(0).gameObject;
	m_uiWashIcon[2] = m_uiWashList.transform:GetChild(1).gameObject;
	m_uiWashIcon[3] = m_uiWashList.transform:GetChild(2).gameObject;
	m_uiWashIcon[4] = m_uiWashList.transform:GetChild(4).gameObject;
	m_uiWashIconAdd = m_uiWashList.transform:GetChild(3).gameObject;

	-- 对象池
	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );
	m_ObjectPool:CreatePool("UIP_Equip", 5, 5, m_uiUIP_Equip);
end

-- 界面初始化时调用
function EquipWashDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function EquipWashDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function EquipWashDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function EquipWashDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function EquipWashDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function EquipWashDlgShow()
	EquipWashDlgOpen();
	SetText( m_uiTokenTitle.transform:Find("Num"), "5" );
	system_askinfo( ASKINFO_EQUIP, "", 5 );
	EquipWashDlgSelectTag( 1, 0, -1 )
	EquipWashDlgStopAuto()
end

-- 自动选择
function EquipWashDlgAutoSelect( heroindex, herokind, equipoffset )
	EquipWashDlgOpen();
	SetText( m_uiTokenTitle.transform:Find("Num"), "5" );
	system_askinfo( ASKINFO_EQUIP, "", 5 );
	
	local tag = 1
	if heroindex >= 0 and heroindex <= 3 then
		tag = 1
	elseif heroindex >= 4 and heroindex <= 7 then
		tag = 2
	elseif heroindex >= 8 and heroindex <= 11 then
		tag = 3
	else
		herokind = 2000
	end
	EquipWashDlgSelectTag( tag, herokind, equipoffset )
	EquipWashDlgStopAuto()
end

-- 选择标签
function EquipWashDlgSelectTag( index, herokind, equipoffset )
	m_selectTag = index;
	m_CacheHeroCache = {}
	m_showEffect = false;
	for tmpi = 1, 3, 1 do
		if tmpi == index then
			SetImage( m_uiTagBtn[tmpi].transform:Find("Back"), LoadSprite( "ui_button_page3" )  );
			
			-- 先放进临时缓存
			-- 主将
			if index == 1 then
				for offset = 0, 3, 1 do
					local pHero = GetHero().m_CityHero[offset];
					table.insert(m_CacheHeroCache, { m_kind = pHero.m_kind, m_pHero = pHero });
					SetControlID( m_uiHeroBtn[offset+1], 1000+pHero.m_kind )
				end
			
			-- 财赋署	
			elseif index == 2 then
				for offset = 4, 7, 1 do
					local pHero = GetHero().m_CityHero[offset];
					table.insert(m_CacheHeroCache, { m_kind = pHero.m_kind, m_pHero = pHero });
					SetControlID( m_uiHeroBtn[offset-3], 1000+pHero.m_kind )
				end
			
			-- 御林军
			elseif index == 3 then
				for offset = 8, 11, 1 do
					local pHero = GetHero().m_CityHero[offset];
					table.insert(m_CacheHeroCache, { m_kind = pHero.m_kind, m_pHero = pHero });
					SetControlID( m_uiHeroBtn[offset-7], 1000+pHero.m_kind )
				end
			end
		else
			SetImage( m_uiTagBtn[tmpi].transform:Find("Back"), LoadSprite( "ui_button_page4" )  );
		end
	end
	
	EquipWashDlgSetHero()
	if herokind > 0 then
		EquipWashDlgSelectHero( herokind, equipoffset );
	elseif m_CacheHeroCache[1] ~= nil and m_CacheHeroCache[1].m_kind > 0 then
		EquipWashDlgSelectHero( m_CacheHeroCache[1].m_kind, -1 );
	else
		EquipWashDlgSelectHero( 2000, -1 );
	end	
end

-- 设置英雄
function EquipWashDlgSetHero()
	for i=1, 4, 1 do
		local pHero = m_CacheHeroCache[i];
		if pHero.m_kind > 0 then
			SetText( m_uiHeroBtn[i].transform:Find("Back/Text"), HeroName(pHero.m_kind) )
		else
			SetText( m_uiHeroBtn[i].transform:Find("Back/Text"), "" )
		end
	end
end

-- 选择英雄
function EquipWashDlgSelectHero( kind, equipoffset )
	m_herokind = kind;
	if kind == 0 then
		return
	end
	m_CacheEquipCache = {};
	m_showEffect = false;
	
	-- 先全部设为未选
	for i=1, 4, 1 do
		SetImage( m_uiHeroBtn[i].transform:Find("Back"), LoadSprite( "ui_button_17" )  );
	end
	
	-- 点击的是背包装备
	if kind == 2000 then
		SetImage( m_uiHeroBtn[5].transform:Find("Back"), LoadSprite( "ui_button_18" )  );
		for tmpi=0, MAX_EQUIPNUM-1, 1 do
			local pEquip = GetEquip():GetAnyEquip( tmpi );
			if pEquip ~= nil and pEquip.m_kind > 0 then
				table.insert(m_CacheEquipCache, { m_kind = pEquip.m_kind, m_color=equip_getcolor(pEquip.m_kind), m_pEquip = pEquip });
			end
		end
	else
		SetImage( m_uiHeroBtn[5].transform:Find("Back"), LoadSprite( "ui_button_17" )  );
		-- 找到这个英雄的装备
		for i=1, 4, 1 do
			local pHero = m_CacheHeroCache[i].m_pHero;
			if pHero.m_kind == kind then
				SetImage( m_uiHeroBtn[i].transform:Find("Back"), LoadSprite( "ui_button_18" )  );
				for equipindex=0, 5, 1 do
					local pEquip = pHero.m_Equip[equipindex];
					if pEquip.m_kind > 0 then
						table.insert(m_CacheEquipCache, { m_kind = pEquip.m_kind, m_color=equip_getcolor(pEquip.m_kind), m_pEquip = pEquip });
					end
				end
				break;
			end
		end
	end
	
	EquipWashDlgSetItem()
	if equipoffset >= 0 then
		EquipWashDlgSelectItem( equipoffset )
	elseif #m_CacheEquipCache > 0 and m_CacheEquipCache[1] ~= nil then
		EquipWashDlgSelectItem( m_CacheEquipCache[1].m_pEquip.m_offset )
	else
		EquipWashDlgSelectItem( -1 )
	end
end

-- 装备排序
function EquipWashDlgItemSort( a, b )
	if a.m_color > b.m_color then
		return true
	else
		return false
	end
end

-- 设置装备
function EquipWashDlgClearItem()
	-- 先清空
	local objs = {};
	for i = 0 ,m_uiContent.transform.childCount - 1 do
		table.insert( objs, m_uiContent.transform:GetChild(i).gameObject )
    end
	for k, v in pairs(objs) do
		local obj = v;
		m_ObjectPool:Release( "UIP_Equip", obj );
    end
end

-- 设置装备
function EquipWashDlgSetItem()
	-- 先清空
	EquipWashDlgClearItem()
	
	-- 排序
	table.sort( m_CacheEquipCache, EquipWashDlgItemSort );
	
	-- 设置
	m_uiEquipList = {};
	for i=1, #m_CacheEquipCache do
		local pEquip = m_CacheEquipCache[i].m_pEquip
		local uiObj = m_ObjectPool:Get( "UIP_Equip" );
		uiObj.transform:SetParent( m_uiContent.transform );
		SetControlID( uiObj, 10000+pEquip.m_offset )
		local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
		local uiShape = objs[0]
		local uiColor = objs[1]
		local uiWash = objs[2]
		local uiSelect = objs[3]
		SetFalse( uiSelect )
		SetImage( uiShape, EquipSprite( pEquip.m_kind ) );
		SetImage( uiColor, ItemColorSprite( equip_getcolor( pEquip.m_kind ) ) );
		SetEquipWash( uiWash, pEquip );
		m_uiEquipList[i] = uiObj;
	end
end

-- 选择装备
function EquipWashDlgSelectItem( offset )
	m_equipoffset = offset;
	m_showEffect = false;
	if offset == -1 then
		EquipWashDlgClearItem()
		SetFalse( m_uiShape )
		SetFalse( m_uiColor )
		SetFalse( m_uiName )
		SetFalse( m_uiAbility )
		SetFalse( m_uiWash )
		SetFalse( m_uiButtons )
		SetFalse(m_uiWashList )
		SetFalse( m_uiTokenTitle )
		return;
	end
	
	SetTrue( m_uiShape )
	SetTrue( m_uiColor )
	SetTrue( m_uiName )
	SetTrue( m_uiAbility )
	SetTrue( m_uiWash )
	SetTrue( m_uiButtons )
	SetTrue(m_uiWashList )
	SetTrue( m_uiTokenTitle )
		
	for i=1, #m_CacheEquipCache do
		local uiObj = m_uiEquipList[i]
		local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
		local uiSelect = objs[3]
		
		local pEquip = m_CacheEquipCache[i].m_pEquip;
		if pEquip.m_offset == offset then
			-- 形象
			SetImage( m_uiShape, EquipSprite( pEquip.m_kind ) );
			-- 颜色
			local color = equip_getcolor( pEquip.m_kind );
			SetImage( m_uiColor, ItemColorSprite( color ) );
			-- 名称
			SetText( m_uiName, EquipName( pEquip.m_kind ), NameColor( equip_getcolor( pEquip.m_kind ) ) );
			-- 属性
			SetText( m_uiAbility, equip_getabilityname( pEquip.m_kind ) );
			SetEquipWash( m_uiWash, pEquip );
			
			-- 洗练技能
			EquipWashDlgSetWash( pEquip )
			
			-- 选择
			SetTrue( uiSelect )
			if color < 1 then
				SetTrue( m_uiWarn )
				SetFalse( m_uiButtons )
				SetFalse(m_uiWashList )
				SetFalse( m_uiTokenTitle )
			else
				SetTrue( m_uiButtons )
				SetFalse( m_uiWarn )
				SetTrue( m_uiWashList )
				SetTrue( m_uiTokenTitle )
				
			end
		else
			-- 选择
			SetFalse( uiSelect )
		end
	end
	m_wash_count = 0;
	m_auto_wash_toggle = false;	
end

-- 设置洗练技能
function EquipWashDlgSetWash( pEquip )
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	if m_equipoffset ~= pEquip.m_offset then
		return;
	end
	local color = equip_getcolor( pEquip.m_kind );
	if color <= ITEM_COLOR_LEVEL_WHITE then
		return;
	end
	
	m_color = color;
	m_level_allfull = 1;
	m_level_allfull_num = 0;
	m_hassuper = 0;
	m_CacheWashid = {}
	for i=1, 4, 1 do
		SetTrue( m_uiWashIcon[i] );
		local objs = m_uiWashIcon[i].transform:GetComponent( typeof(Reference) ).relatedGameObject;
		local uiShape = objs[0];
		local uiColor = objs[1];
		local uiName = objs[2];
		local uiIcon = objs[3];
		local uiLevel = objs[4];
		local uiWarn = objs[5];
		local uiNameBack = objs[6];
		if color >= i then
			SetText( uiWarn, T(870) ) -- 已开启
		else
			SetText( uiWarn, T(864+i-1).."\n"..T(609) ) -- X装开启
		end
		
		local washid = pEquip.m_washid[i];
		if washid > 0 then
			m_CacheWashid[i] = washid;
			SetControlID( m_uiWashIcon[i], 100 + i )
			SetTrue( uiShape )
			SetTrue( uiColor )
			SetTrue( uiName )
			SetTrue( uiIcon )
			SetTrue( uiLevel )
			SetTrue( uiNameBack )
			SetFalse( uiWarn )
			-- 装备技能信息
			SetImage( uiShape, EquipWashSprite( g_equip_wash[washid].ability ) )
			SetImage( uiColor, ItemColorSprite( color ) )
			SetText( uiName, equip_getwashname( g_equip_wash[washid].ability ) )
			-- 满级
			if g_equip_wash[washid].level >= g_equip_washrule[color].levellimit then
				SetImage( uiIcon, LoadSprite("ui_icon_star_1") )
				SetText( uiLevel, "Lv."..g_equip_wash[washid].level.."("..T(842)..")" )
				m_level_allfull_num = m_level_allfull_num + 1
			else
				SetImage( uiIcon, LoadSprite("ui_icon_star_2") )
				SetText( uiLevel, "Lv."..g_equip_wash[washid].level )
				m_level_allfull = 0;
			end
			
			if i == 4 then
				m_hassuper = 1;
			end
			if m_showEffect == true then 
				EquipWashDlgUpdateEffect(uiShape);
			end
		else
			SetFalse( uiShape )
			SetFalse( uiColor )
			SetFalse( uiName )
			SetFalse( uiIcon )
			SetFalse( uiLevel )
			SetFalse( uiNameBack )
			SetTrue( uiWarn )
		end
	end
	
	-- 紫装秘技
	local objs = m_uiWashIcon[4].transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiWarn = objs[5];
	if color < ITEM_COLOR_LEVEL_PURPLE then
		SetFalse( m_uiWashIconAdd );
		SetFalse( m_uiWashIcon[4] );
	else
		SetTrue( m_uiWashIconAdd );
		SetTrue( m_uiWashIcon[4] );
		if GetPlayer().m_attr.m_ability_open_201 > 0 then
			SetText( uiWarn, T(871) )
		else
			SetText( uiWarn, T(869) )
		end
	end
	
	-- 小于紫色，等级全满，就不自动洗练了
	if color < ITEM_COLOR_LEVEL_PURPLE and m_level_allfull == 1 then
		EquipWashDlgStopAuto()
			
	-- 金色装备，洗出秘技，就不自动洗练了
	elseif color >= ITEM_COLOR_LEVEL_PURPLE and m_hassuper == 1 then
		EquipWashDlgStopAuto()
		
	elseif color >= ITEM_COLOR_LEVEL_PURPLE and m_level_allfull == 1 then 
		EquipWashDlgStopAuto()
	end
	
	if m_auto_wash_count <= 0 then
		EquipWashDlgSetButton()
	end
end

-- 更新特效
function EquipWashDlgUpdateEffect(uiEffect)
	if m_showEffect == false then 
		return 
	end;
	local effect = GameObject.Instantiate( LoadPrefab( "Jssj" ) );
	effect.transform:SetParent( uiEffect.transform );
	effect.transform.localPosition = Vector3.New( 0, 0, 0 );		
	GameObject.Destroy(effect,1.5)
end

-- 
function EquipWashDlgSetButton()
	-- 开启秘技洗练
	if m_color >= ITEM_COLOR_LEVEL_PURPLE and GetPlayer().m_attr.m_ability_open_201 > 0 and m_level_allfull == 1 and m_level_allfull_num > 0 then
		SetFalse( m_uiTokenBtn )
		SetTrue( m_uiSuperBtn )
		-- 秘技洗练价格 
		m_washtoken = global.equip_wash_super_token
		SetText( m_uiTokenTitle.transform:Find("Num"), m_washtoken );
	else
		SetFalse( m_uiSuperBtn )
		SetTrue( m_uiTokenBtn )
		-- 元宝洗练价格 
		m_washtoken = g_equip_washrule[m_color].token
		SetText( m_uiTokenTitle.transform:Find("Num"), m_washtoken );
	end
end

-- 接收洗练次数
function EquipWashDlgRecv( recvValue )
	m_recvValue = recvValue;
	local uiNum = m_uiFreeTitle.transform:Find("Num")
	SetTrue( uiNum )
	SetText( uiNum, T(841)..":"..recvValue.m_equip_washnum.."/"..global.equip_wash_max )
	local uiTimer = m_uiFreeTitle.transform:Find("Timer");
	if recvValue.m_equip_washnum < global.equip_wash_max then
		SetTrue( uiTimer )
		SetTimer( uiTimer, recvValue.m_equip_washsec, recvValue.m_equip_washsec, 1 );
	else
		SetFalse( uiTimer )
	end
end

-- 更新
function EquipWashDlgUpdate( updateEquip )
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	SetEquipWash( m_uiWash, updateEquip );
	for i=1, #m_CacheEquipCache do
		local pEquip = m_CacheEquipCache[i].m_pEquip;
		if pEquip.m_offset == updateEquip.m_offset then
			-- 更新
			m_CacheEquipCache[i].m_pEquip = updateEquip;
			pEquip = m_CacheEquipCache[i].m_pEquip;
			-- 星星对象
			local uiObj = m_uiEquipList[i]
			local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
			local uiWash = objs[2]
			SetEquipWash( uiWash, pEquip );
			break;
		end
	end
end


-- 免费洗练
function EquipWashDlgFree()
	if m_recvValue.m_equip_washnum <= 0 then
		pop(T(843))
		return
	end
	
	if m_hassuper == 1 then
		MsgBox( T(878), function() 
			system_askinfo( ASKINFO_EQUIP, "", 6, m_equipoffset );
		end )
	else
		system_askinfo( ASKINFO_EQUIP, "", 6, m_equipoffset );
	end
	m_showEffect = true;
end

-- 钻石洗练
function EquipWashDlgToken()
	local token = m_washtoken;
	m_showEffect = true;
	local auto = true;
	-- 小于紫色色，等级全满，就不自动洗练了
	if m_color < ITEM_COLOR_LEVEL_PURPLE and m_level_allfull == 1 then
		auto = false;
	end
	
	--[[-- 紫色装备，洗出秘技，就不自动洗练了
	if auto == true then
		if m_color >= ITEM_COLOR_LEVEL_PURPLE and m_hassuper == 1 then
			auto = false;
		end
	end--]]
	
	-- 超过5次提示自动洗练
	if auto == true and m_wash_count >= 5 and m_auto_wash_toggle == false then
		
		local msg = ""
		if m_color >= ITEM_COLOR_LEVEL_PURPLE then
			msg = "\n"..T(880).."\n\n"..T(879)
		else
			msg = "\n\n"..T(879)
		end
		MsgBoxEx( F( 873, 100, token )..msg, function( sure, toggle ) 
			if sure == 1 then
				if GetPlayer().m_token < token then
					JumpToken()
				else
					-- 启动每秒自动洗练
					EquipWashDlgStartAuto()
				end
			end
			m_wash_count = 0;
			m_auto_wash_toggle = toggle;
		end, m_auto_wash_toggle )
		
	else
	
		MsgBoxEx( F( 872, token ), function( sure, toggle ) 
			if sure == 1 then
				if GetPlayer().m_token < token then
					JumpToken()
				else
					system_askinfo( ASKINFO_EQUIP, "", 7, m_equipoffset );
					m_wash_count = m_wash_count + 1
				end
			end
			m_wash_toggle = toggle;
		end, m_wash_toggle )
	end
end

-- 秘技洗练
function EquipWashDlgSuper()
	local token = m_washtoken;
	MsgBoxEx( F( 881, token ), function( sure, toggle ) 
			if sure == 1 then
				if GetPlayer().m_token < token then
					JumpToken()
				else
					m_showEffect = true;
					system_askinfo( ASKINFO_EQUIP, "", 8, m_equipoffset );				
				end
			end
			m_super_wash_toggle = toggle;
	end, m_super_wash_toggle )
end

-- 自动洗练
function EquipWashDlgAuto()
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	if m_auto_wash_count <= 0 then
		return
	end
	
	if GetPlayer().m_token < m_washtoken then
		JumpToken()
		EquipWashDlgStopAuto()
	else
		m_showEffect = true;
		system_askinfo( ASKINFO_EQUIP, "", 7, m_equipoffset );
	end
		
	m_auto_wash_count = m_auto_wash_count - 1;
	SetText( m_uiAutoBtn.transform:Find( "Back/Text" ), T(875)..":"..m_auto_wash_count )
	if m_auto_wash_count <= 0 then
		EquipWashDlgStopAuto()
	end
end

-- 启动自动洗练
function EquipWashDlgStartAuto()
	m_wash_count = 0
	m_auto_wash_count = 100;
	SetFalse( m_uiTokenBtn );
	SetFalse( m_uiSuperBtn );
	SetTrue( m_uiAutoBtn );
	SetText( m_uiAutoBtn.transform:Find( "Back/Text" ), T(875)..":"..m_auto_wash_count )
end

-- 停止自动洗练
function EquipWashDlgStopAuto()
	m_wash_count = 0
	m_auto_wash_count = 0;
	SetFalse( m_uiAutoBtn );
	EquipWashDlgSetButton()
end

function EquipWashDlgWashTips( index )
	if index == -1 then
		SetFalse( m_uiTips )
		return
	end

	local washid = m_CacheWashid[index];
	if index < 4 and washid == nil then
		SetFalse( m_uiTips )
		return
	end
	
	SetTrue( m_uiTips )
	local objs = m_uiTips.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiInfo = objs[0]
	local uiShape = objs[1];
	local uiColor = objs[2];
	local uiAbility = objs[3];
	local uiIcon = objs[4];
	local uiLevel = objs[5];
	local uiWarn = objs[6];
	local uiBack = objs[7];
	uiInfo.transform.position = Vector3.New( m_uiWashIcon[index].transform.position.x, m_uiWashIcon[index].transform.position.y+1.5, 0 )
	
	if index == 4 and washid == nil then
		SetFalse( uiShape );
		SetFalse( uiColor );
		SetFalse( uiAbility );
		SetFalse( uiIcon );
		SetFalse( uiLevel );
		SetFalse( uiBack );
		SetTrue( uiWarn );
		SetText( uiWarn, T(880) )
	else
		SetTrue( uiShape );
		SetTrue( uiColor );
		SetTrue( uiAbility );
		SetTrue( uiIcon );
		SetTrue( uiLevel );
		SetTrue( uiBack );
		SetFalse( uiWarn );
		SetImage( uiShape, EquipWashSprite( g_equip_wash[washid].ability ) )
		SetImage( uiColor, ItemColorSprite( m_color ) )
		SetText( uiAbility, equip_getwashname( g_equip_wash[washid].ability ).."+"..g_equip_wash[washid].value )
		SetText( uiLevel, "Lv."..g_equip_wash[washid].level )
		-- 满级
		if g_equip_wash[washid].level >= g_equip_washrule[m_color].levellimit then
			SetImage( uiIcon, LoadSprite("ui_icon_star_1") )
		else
			SetImage( uiIcon, LoadSprite("ui_icon_star_2") )
		end
	end
end