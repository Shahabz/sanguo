-- 界面
local m_Dlg = nil;
local m_DialogFrameMod = nil;

local m_uiUIP_ForgingEquip = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiEquipInfo = nil; --UnityEngine.GameObject
local m_uiShape = nil; --UnityEngine.GameObject
local m_uiName = nil; --UnityEngine.GameObject
local m_uiAbility = nil; --UnityEngine.GameObject
local m_uiSec = nil; --UnityEngine.GameObject
local m_uiMaterialGrid = nil; --UnityEngine.GameObject
local m_uiScroll = nil; --UnityEngine.GameObject
local m_uiUnLockDesc = nil; --UnityEngine.GameObject
local m_uiColor = nil; --UnityEngine.GameObject
local m_uiForgingBtn = nil; --UnityEngine.GameObject
local m_uiForgingInfo = nil; --UnityEngine.GameObject
local m_uiMaterialInfo = nil; --UnityEngine.GameObject
local m_uiOfficialEffect = nil; --UnityEngine.GameObject

local m_uiOfficialShape = nil; --UnityEngine.GameObject
local m_uiOfficialTimeBack = nil; --UnityEngine.GameObject
local m_uiOfficialTime = nil; --UnityEngine.GameObject
local m_uiOfficialName = nil; --UnityEngine.GameObject
local m_uiOfficialDesc = nil; --UnityEngine.GameObject
local m_uiOfficialBtn = nil; --UnityEngine.GameObject
local m_uiUpBtn = nil; --UnityEngine.GameObject

local m_ObjectPool = nil;
local m_uiEquipObj = {};
local m_selectkind = 0;
local m_material = {};
local m_selectmaterial = 0;
local m_isforging = {};
local m_scrollindex = 0;

local m_ForgingEffect = nil;
local m_guideShow = true;

-- 打开界面
function EquipForgingDlgOpen()
	m_Dlg = eye.uiManager:Open( "EquipForgingDlg" );
	m_DialogFrameMod = DialogFrameModOpen( m_Dlg, BuildingName(BUILDING_Smithy), HELP_EquipForgingDlg, EquipForgingDlgClose );
end

-- 隐藏界面
function EquipForgingDlgClose()
	if m_Dlg == nil then
		return;
	end
	DialogFrameModClose( m_DialogFrameMod );
	m_DialogFrameMod = nil;
	eye.uiManager:Close( "EquipForgingDlg" );
	HideSelect();
	m_selectkind = 0;
end

-- 删除界面
function EquipForgingDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function EquipForgingDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            EquipForgingDlgClose();
		elseif nControlID == -2 then
			EquipForgingDlgSelectMaterial(-1)
		
		elseif nControlID == 1 then
			m_uiScroll.transform:GetComponent("UIScrollRect"):ScrollToTop();
			
		elseif nControlID == 2 then
			m_uiScroll.transform:GetComponent("UIScrollRect"):ScrollToBottom();
		
		-- 打造
		elseif nControlID == 3 then
			EquipForgingDlgForging()
		
		-- 获取材料
		elseif nControlID == 4 then
			EquipForgingDlgMaterialGet()
			
		-- 雇佣
		elseif nControlID == 5 then
			OfficialHireDlgShow( 0 );
		
		-- 加速
		elseif nControlID == 11 then
			EquipForgingDlgQuick()
		
		-- 铁匠加速	
		elseif nControlID == 12 then
			EquipForgingDlgFreeQuick()
			
		-- 选择装备
		elseif nControlID > 1000 and nControlID < 2000 then
			EquipForgingDlgSelect( nControlID-1000 )
		
		-- 选择材料	
		elseif nControlID >= 2000 and nControlID < 2100 then
			EquipForgingDlgSelectMaterial( nControlID-2000 )
        end
		
	elseif nType == UI_EVENT_TIMECOUNTEND then
		if nControlID == 1 then
			-- 延迟1秒关闭
			Invoke(function()
				EquipForgingDlgClose();
			end, 1)
		end
	end
end

-- 载入时调用
function EquipForgingDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;
	m_uiUIP_ForgingEquip = objs[0];
	m_uiContent = objs[1];
	m_uiEquipInfo = objs[2];
	m_uiShape = objs[3];
	m_uiName = objs[4];
	m_uiAbility = objs[5];
	m_uiSec = objs[6];
	m_uiMaterialGrid = objs[7];
	m_uiScroll = objs[8];
	m_uiUnLockDesc = objs[9];
	m_uiColor = objs[10];
	m_uiForgingBtn = objs[11];
	m_uiForgingInfo = objs[12];
	m_uiOfficialShape = objs[13];
	m_uiOfficialTimeBack = objs[14];
	m_uiOfficialTime = objs[15];
	m_uiOfficialName = objs[16];
	m_uiOfficialDesc = objs[17];
	m_uiMaterialInfo = objs[18];
	m_uiOfficialEffect = objs[19];
	m_uiOfficialBtn = objs[20];
	m_uiUpBtn = objs[21];
	
	-- 对象池
	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );
	m_ObjectPool:CreatePool("UIP_ForgingEquip", 6, 6, m_uiUIP_ForgingEquip);
end

-- 界面初始化时调用
function EquipForgingDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function EquipForgingDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function EquipForgingDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function EquipForgingDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function EquipForgingDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function EquipForgingDlgShow()
	if Utils.get_int_sflag( GetPlayer().m_function, CITY_FUNCTION_SMITHY ) == 0 then
		AlertMsg( T(2427) )
		return
	end
	HeroInfoDlgClose();
	HeroListDlgClose();
	EquipForgingDlgOpen();
	EquipForgingDlgClear()
	
	if GetPlayer().m_forgingsec > 0 then
		EquipForgingDlgTimer();
		return
	end	
	EquipForgingDlgSetOfficial()
	
	-- 
	local EquipList = {};
	for kind, info in pairs(g_equipinfo) do
		if equip_getcolor( kind ) < 5 then
			table.insert( EquipList, { m_kind=kind, m_actorlevel=info.actorlevel } )
		end
	end
	table.sort( EquipList, EquipForgingDlgCacheSort )
	
	for key, info in pairsByKeys(EquipList) do
		local kind = info.m_kind;
		local uiObj = m_ObjectPool:Get( "UIP_ForgingEquip" );
		uiObj.transform:SetParent( m_uiContent.transform );
		m_uiEquipObj[kind] = uiObj
		local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
		local uiShape = objs[0];
		local uiColor = objs[1];
		local uiName = objs[2];
		local uiNum = objs[3];
		local uiLock = objs[4];
		local uiHammer = objs[5];
		local uiSelect = objs[6];
		SetControlID( uiObj, 1000+kind )
		-- 形象
		SetImage( uiShape, EquipSprite( kind ) );
		-- 颜色
		SetImage( uiColor, ItemColorSprite( g_equipinfo[kind].color ) );
		-- 名称
		SetText( uiName, EquipName( kind ), NameColor( g_equipinfo[kind].color ) );
		-- 已有数量
		SetText( uiNum, GetEquip():GetCount( kind ) )
		-- 锁
		if GetPlayer().m_level < g_equipinfo[kind].actorlevel then
			SetTrue( uiLock )
			SetFalse( uiNum )
			break; -- 此处注释就都显示出来
		else
			SetFalse( uiLock )
			SetTrue( uiNum )
		end
		
		-- 是否可以打造
		forging = true
		-- 材料
		local material ={}
		table.insert( material, {kind=-1, num=g_equipinfo[kind].silver} )
		table.insert( material, {kind=g_equipinfo[kind].material_kind0, num=g_equipinfo[kind].material_num0} ) 
		table.insert( material, {kind=g_equipinfo[kind].material_kind1, num=g_equipinfo[kind].material_num1} ) 
		table.insert( material, {kind=g_equipinfo[kind].material_kind2, num=g_equipinfo[kind].material_num2} ) 
		table.insert( material, {kind=g_equipinfo[kind].material_kind3, num=g_equipinfo[kind].material_num3} ) 
		table.insert( material, {kind=g_equipinfo[kind].material_kind4, num=g_equipinfo[kind].material_num4} ) 
		for i = 1, #material do
			local itemkind = material[i].kind
			local itemnum = material[i].num
			if itemkind == -1 then
				if GetPlayer().m_silver < itemnum then
					forging = false
					break
				end
			elseif itemkind > 0 then
				local has = GetItem():GetCount( itemkind )
				if has < itemnum then
					forging = false
				end
			end
		end
		if forging == true then
			SetTrue( uiHammer );
		else
			SetFalse( uiHammer );
		end
		
		-- 选择状态
		SetFalse( uiSelect )
		
		if IsGuiding() then
			if GetCurrentGuideType() == GUIDE_CHOOSE then 
				if kind ==1 then
					FindCmdTpye(m_uiUpBtn.transform);
				end
			elseif GetCurrentGuideType() == GUIDE_CHOOSE_MA then
				if kind ==11 and forging == true then
					FindCmdTpye(m_uiUpBtn.transform);
				end
			elseif GetCurrentGuideType() == GUIDE_CHOOSE_CLOUTH then
				if kind ==21 and forging == true then
					FindCmdTpye(m_uiUpBtn.transform);
				end
			elseif GetCurrentGuideType() == GUIDE_CHOOSE_HEAD then
				if kind ==31 and forging == true then
					FindCmdTpye(m_uiUpBtn.transform);
				end
			end
		end
    end 
	-- 任务检查
	QuestClickCheck( 4 )
end

-- 排序
function EquipForgingDlgCacheSort(a, b)
	if a.m_actorlevel < b.m_actorlevel then
		return true;
	elseif a.m_actorlevel == b.m_actorlevel then
		if a.m_kind < b.m_kind then
			return true
		else
			return false
		end
	else
		return false;
	end
end

-- 清空
function EquipForgingDlgClear()
	local objs = {};
	for i = 0 ,m_uiContent.transform.childCount - 1 do
		table.insert( objs, m_uiContent.transform:GetChild(i).gameObject )
    end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "UIP_ForgingEquip(Clone)" then
			m_ObjectPool:Release( "UIP_ForgingEquip", obj );
		end
    end
	m_uiScroll:GetComponent("UIScrollRect"):ResetScroll();
	m_uiContent.transform.localPosition = Vector2.zero
	m_uiEquipObj = {};
	m_scrollindex = 0;
	SetFalse( m_uiEquipInfo )
	SetFalse( m_uiForgingInfo )
	GameObject.Destroy(m_ForgingEffect)
	m_ForgingEffect = nil;
end

-- 选择要打造的装备
function EquipForgingDlgSelect( kind )
	if m_selectkind > 0 then
		local objs = m_uiEquipObj[m_selectkind].transform:GetComponent( typeof(Reference) ).relatedGameObject;
		local uiSelect = objs[6];
		SetFalse( uiSelect )
	end
	m_selectkind = kind;
	local objs = m_uiEquipObj[m_selectkind].transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiSelect = objs[6];
	SetTrue( uiSelect )
	SetTrue( m_uiEquipInfo )
	
	if GetPlayer().m_level < g_equipinfo[kind].actorlevel then
		SetTrue( m_uiUnLockDesc )
		SetText( m_uiUnLockDesc, F(765, g_equipinfo[kind].actorlevel ) )
		SetFalse( m_uiForgingBtn );
	else
		SetFalse( m_uiUnLockDesc )
		SetTrue( m_uiForgingBtn );

		if IsGuiding() then
			if GetCurrentGuideType() == GUIDE_CHOOSE then 
				if kind == 1 and forging == true then
					GuideNext();
				end
			elseif GetCurrentGuideType() == GUIDE_CHOOSE_MA then 
				if kind == 11 and forging == true then
					GuideNext();
				end
			elseif GetCurrentGuideType() == GUIDE_CHOOSE_CLOUTH then 
				if kind == 21 and forging == true then
					GuideNext();
				end
			elseif GetCurrentGuideType() == GUIDE_CHOOSE_HEAD then 
				if kind == 31 and forging == true then
					GuideNext();
				end
			end
		end		
		
		if IsGuiding() and GetCurrentGuideType() == GUIDE_MAKE then
			FindCmdTpye(m_uiForgingBtn.transform);
		end
	end
	
	-- 形象
	SetImage( m_uiShape, EquipSprite( kind ) );
	-- 颜色
	SetImage( m_uiColor, ItemColorSprite( g_equipinfo[kind].color ) );
	-- 名称
	SetText( m_uiName, EquipName( kind ), NameColor( g_equipinfo[kind].color ) );
	-- 属性
	SetText( m_uiAbility, equip_getabilityname( kind ) );	
	-- 耗时
	SetText( m_uiSec, T(766)..":"..secnum(g_equipinfo[kind].sec) );
	
	-- 材料
	m_material ={}
	table.insert( m_material, {kind=120, num=g_equipinfo[kind].silver} )
	table.insert( m_material, {kind=g_equipinfo[kind].material_kind0, num=g_equipinfo[kind].material_num0} ) 
	table.insert( m_material, {kind=g_equipinfo[kind].material_kind1, num=g_equipinfo[kind].material_num1} ) 
	table.insert( m_material, {kind=g_equipinfo[kind].material_kind2, num=g_equipinfo[kind].material_num2} ) 
	table.insert( m_material, {kind=g_equipinfo[kind].material_kind3, num=g_equipinfo[kind].material_num3} ) 
	table.insert( m_material, {kind=g_equipinfo[kind].material_kind4, num=g_equipinfo[kind].material_num4} ) 
	
	m_isforging = {true,true,true,true,true,true};
	for i = 0, m_uiMaterialGrid.transform.childCount - 1 do
		local materialObj = m_uiMaterialGrid.transform:GetChild(i).gameObject
		local objs = materialObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
		local uiShape = objs[0];
		local uiColor = objs[1];
		local uiNum = objs[2];
		local itemkind = m_material[i+1].kind
		local itemnum = m_material[i+1].num
		
		SetControlID( materialObj, 2000 + i )
		SetTrue( materialObj )
		SetImage( uiShape, ItemSprite(itemkind) );
		SetImage( uiColor, ItemColorSprite( item_getcolor(itemkind) ) );
			
		if itemkind == 120 then
			if GetPlayer().m_silver < itemnum then
				SetText( uiNum, knum(itemnum).."<color=#e80017>/"..knum(GetPlayer().m_silver).."</color>" )
				m_isforging[i] = false;
			else
				SetText( uiNum, knum(itemnum).."/"..knum(GetPlayer().m_silver) )
			end
		elseif itemkind > 0 then
			local has = GetItem():GetCount( itemkind )
			if has < itemnum then
				SetText( uiNum, itemnum.."<color=#e80017>/"..GetItem():GetCount( itemkind ).."</color>" )
				m_isforging[i] = false;
			else
				SetText( uiNum, itemnum.."/"..GetItem():GetCount( itemkind ) )
			end
		else
			SetFalse( materialObj )
		end
    end
end

function HideSelect()
	if m_selectkind > 0 then
		local objs = m_uiEquipObj[m_selectkind].transform:GetComponent( typeof(Reference) ).relatedGameObject;
		local uiSelect = objs[6];
		SetFalse( uiSelect )
	end
end

-- 更新
function EquipForgingDlgUpdate()
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	EquipForgingDlgSelect( m_selectkind )
end

-- 选择材料
function EquipForgingDlgSelectMaterial( index )
	if index < 0 then
		m_selectmaterial = 0
		SetFalse( m_uiMaterialInfo )
		return;
	end
	SetTrue( m_uiMaterialInfo )
	
	local itemkind = m_material[index+1].kind
	local itemnum = m_material[index+1].num
	m_selectmaterial = itemkind;
	
	local objs = m_uiMaterialInfo.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiShape = objs[0];
	local uiColor = objs[1];
	local uiName = objs[2];
	local uiDesc = objs[3];
	local uiPath = objs[4];
	local uiNeed = objs[5];
	local uiGetBtn = objs[6];
	
	SetImage( uiShape, ItemSprite(itemkind) );
	SetImage( uiColor, ItemColorSprite( item_getcolor(itemkind) ) );
	SetText( uiName, item_getname(itemkind) )
	SetText( uiDesc, item_getdesc(itemkind) )
	
	if itemkind == 120 then
		SetTrue( uiGetBtn )
		if GetPlayer().m_silver < itemnum then
			SetText( uiNeed, T(772)..": <color=#03de27>"..knum(itemnum).."</color><color=#e80017>/"..knum(GetPlayer().m_silver).."</color>" )
		else
			SetText( uiNeed, T(772)..": <color=#03de27>"..knum(itemnum).."/"..knum(GetPlayer().m_silver).."</color>" )
		end
	elseif itemkind > 0 then
		local has = GetItem():GetCount( itemkind )
		if has < itemnum then
			SetText( uiNeed, T(772)..": <color=#03de27>"..itemnum.."</color><color=#e80017>/"..GetItem():GetCount( itemkind ).."</color>" )
		else
			SetText( uiNeed, T(772)..": <color=#03de27>"..itemnum.."/"..GetItem():GetCount( itemkind ).."</color>" )
		end
		
		if item_gettype( itemkind ) == ITEM_TYPE_EQUIP_DRAWING then
			SetFalse( uiGetBtn )
		else
			SetTrue( uiGetBtn )
		end
	end
end

-- 获取材料
function EquipForgingDlgMaterialGet()
	if m_selectmaterial <= 0 then
		return
	end
	MaterialGetDlgShow( m_selectmaterial )
	EquipForgingDlgSelectMaterial( -1 )
end

-- 打造中页面
function EquipForgingDlgTimer()
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	SetFalse( m_uiEquipInfo )
	SetTrue( m_uiForgingInfo )
	
	local kind = GetPlayer().m_forgingkind
	local objs = m_uiForgingInfo.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiShape = objs[0];
	local uiColor = objs[1];
	local uiName = objs[2];
	local uiAbility = objs[3];
	local uiProgress = objs[4];
	local uiTimer = objs[5];
	local uiQuickBtn = objs[6];
	local uiFreeQuickBtn = objs[7];	
	local uiEffect = objs[8];

	
	SetFalse( uiQuickBtn )
	SetFalse( uiFreeQuickBtn )
	
	-- 形象
	SetImage( uiShape, EquipSprite( kind ) );
	-- 颜色
	SetImage( uiColor, ItemColorSprite( g_equipinfo[kind].color ) );
	-- 名称
	SetText( uiName, EquipName( kind ), NameColor( g_equipinfo[kind].color ) );
	-- 属性
	SetText( uiAbility, equip_getabilityname( kind ) );	
	-- 耗时
	SetTimer( uiTimer, GetPlayer().m_forgingsec, g_equipinfo[kind].sec, 1 );
	-- 铁匠
	EquipForgingDlgSetOfficial()
	-- 特效
	EquipForgingDlgForgingEffect(uiEffect)
end

-- 特效
function EquipForgingDlgForgingEffect(uiEffect)
	if m_ForgingEffect == nil then 
		m_ForgingEffect = GameObject.Instantiate( LoadPrefab( "Czdz" ) )
		m_ForgingEffect.transform:SetParent( uiEffect.transform );
		m_ForgingEffect.transform.localPosition = Vector3.New( 0, 0, 0 );		
		m_ForgingEffect.transform.localScale = Vector3.New( 400, 400, 400 );
	end
end

-- 打造
function EquipForgingDlgForging()
	if m_selectkind <= 0 then
		return;
	end
	for i=1, #m_isforging do
		if m_isforging[i] == false then
			pop( T(767) )
			return
		end
	end
	if IsGuiding() and GetCurrentGuideType() == GUIDE_MAKE then 
		GuideNext();
	end
	
	system_askinfo( ASKINFO_EQUIPFORGING, "", 0, m_selectkind );
end

-- 加速
function EquipForgingDlgQuick()
	if GetPlayer().m_forgingkind <= 0 or GetPlayer().m_forgingsec <= 0 then
		return
	end
	-- 加速消耗钻石
	local min = math.floor(GetPlayer().m_forgingsec/60) + 1
	local token = math.ceil( min*global.forgingquick_token)
	MsgBox( F(770, token ), function() 
		if GetPlayer().m_token < token then
			JumpToken();
		else
			system_askinfo( ASKINFO_EQUIPFORGING, "", 1 );
			EquipForgingDlgClose()
		end
	end )
	
	if IsGuiding() then
		if GetCurrentGuideType() == GUIDE_MAKE_UP then
			GuideNext();
		end
	end
end

-- 免费加速
function EquipForgingDlgFreeQuick()
	if GetPlayer().m_forgingkind <= 0 or GetPlayer().m_forgingsec <= 0 then
		return
	end
	system_askinfo( ASKINFO_EQUIPFORGING, "", 2 );
end

-- 设置雇佣官
function EquipForgingDlgSetOfficial()
	if m_Dlg == nil then
		return;
	end
	-- 免费加速按钮
	local objs = m_uiForgingInfo.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiQuickBtn = objs[6];
	local uiFreeQuickBtn = objs[7];
	
	local info = GetPlayer().m_officialhire[0];
	-- 已雇佣
	if info.m_ofkind > 0 then
		SetTrue( m_uiOfficialShape )
		SetFalse( m_uiOfficialEffect )
		SetTrue( m_uiOfficialTimeBack )
		SetTrue( m_uiOfficialTime )
		SetTimer( m_uiOfficialTime, info.m_ofsec, info.m_ofsec, 2 )
		if g_official_forging[info.m_ofkind].token > 0 then
			SetText( m_uiOfficialName, T(719).."Lv."..g_official_forging[info.m_ofkind].level..T(721), Hex2Color(0xD95DF4FF) )
		else
			SetText( m_uiOfficialName, T(719).."Lv."..g_official_forging[info.m_ofkind].level..T(721), Hex2Color(0xF7F3BBFF) )
		end
		SetText( m_uiOfficialDesc, F(769, zhtime(g_official_forging[info.m_ofkind].quick) ), Hex2Color(0x25C9FFFF) )
		SetImage( m_uiOfficialShape, OfSprite( g_official_forging[info.m_ofkind].shape ) )
		
		if info.m_ofquick >= 0 then
			SetTrue( uiFreeQuickBtn )
			SetFalse( uiQuickBtn )
		else
			SetFalse( uiFreeQuickBtn )
			SetTrue( uiQuickBtn )
		end
	else
		-- 未雇佣
		SetFalse( uiFreeQuickBtn )
		SetTrue( uiQuickBtn )
		SetFalse( m_uiOfficialShape )
		SetFalse( m_uiOfficialTimeBack )
		SetFalse( m_uiOfficialTime )
		local pBuilding = GetPlayer():GetBuilding( BUILDING_Main, -1 );
		if pBuilding == nil then
			return;
		end
		if pBuilding.m_level < g_official_forging[1].buildinglevel then
			-- 级解锁Lv.1铁匠
			SetText( m_uiOfficialName, F(726, BuildingName(BUILDING_Main), g_official_forging[1].buildinglevel, g_official_forging[1].level, T(721) ), Hex2Color(0x8A8A8AFF) )
			SetText( m_uiOfficialDesc, F(769, zhtime(g_official_forging[1].quick) ), Hex2Color(0x8A8A8AFF) )
			SetFalse( m_uiOfficialEffect )
			SetImage( m_uiOfficialBtn.transform:Find("Back"), LoadSprite("ui_icon_back_2") )
		else
			SetTrue( m_uiOfficialEffect )
			SetImage( m_uiOfficialBtn.transform:Find("Back"), LoadSprite("ui_icon_back_4") )
			SetText( m_uiOfficialName, T(721).."("..T(2338)..")", Hex2Color(0x8A8A8AFF) )
			SetText( m_uiOfficialDesc, T(2341), Hex2Color(0x8A8A8AFF) )
			-- 可雇佣
--[[			for kind=#g_official_forging, 1, -1 do
				if pBuilding.m_level >= g_official_forging[kind].buildinglevel then
					SetText( m_uiOfficialName, T(718).."Lv."..g_official_forging[kind].level..T(721) )
					SetText( m_uiOfficialDesc, F(769, zhtime(g_official_forging[kind].quick) ) )
					break
				end
			end--]]
		end
	end

end
