-- 界面
local m_Dlg = nil;
local m_DialogFrameMod = nil;
local m_uiScroll = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiUIP_Make = nil; --UnityEngine.GameObject
local m_uiMakeInfo = nil; --UnityEngine.GameObject

local m_pBuilding = nil;
local m_ObjectPool = nil;
local m_uiItemList = {};
local m_CacheItemList = {};
local m_recvValue = nil;
local m_makeing_count = 0;
local m_id = 0;
local m_itemkind = 0;

-- 打开界面
function MaterialMakeWillDlgOpen()
	m_Dlg = eye.uiManager:Open( "MaterialMakeWillDlg" );
	m_DialogFrameMod = DialogFrameModOpen( m_Dlg, T(924), 8, MaterialMakeWillDlgClose );
end

-- 隐藏界面
function MaterialMakeWillDlgClose()
	if m_Dlg == nil then
		return;
	end
	DialogFrameModClose( m_DialogFrameMod );
	m_DialogFrameMod = nil;
	eye.uiManager:Close( "MaterialMakeWillDlg" );
end

-- 删除界面
function MaterialMakeWillDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function MaterialMakeWillDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            MaterialMakeWillDlgClose();
		elseif nControlID == -2 then
			SetFalse( m_uiMakeInfo );
		elseif nControlID == 1 then
			MaterialMakeWillDlgMakeInfo();
		elseif nControlID == 3 then
			MaterialMakeWillDlgMake();
		elseif nControlID >= 10 and nControlID < 20 then
			MaterialMakeWillDlgSelect( nControlID-10 )
		elseif nControlID >= 100 and nControlID < 110 then
			MaterialMakeWillDlgSelectItem( nControlID-100 )
        end
	end
end

-- 载入时调用
function MaterialMakeWillDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiScroll = objs[0];
	m_uiContent = objs[1];
	m_uiUIP_Make = objs[2];
	m_uiMakeInfo = objs[3];

	-- 对象池
	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );
	m_ObjectPool:CreatePool("UIP_Make", 3, 3, m_uiUIP_Make);
end

-- 界面初始化时调用
function MaterialMakeWillDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function MaterialMakeWillDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function MaterialMakeWillDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function MaterialMakeWillDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function MaterialMakeWillDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function MaterialMakeWillDlgShow()
	if Utils.get_int_sflag( GetPlayer().m_actor_sflag, ACTOR_SFLAG_MATERIAL_MAKEWILL ) == 0 then
		AlertMsg( T(2320) )
		return
	end
	m_pBuilding = GetPlayer():GetBuilding( BUILDING_Craftsman );
	MaterialMakeWillDlgOpen()
	-- 请求信息
	system_askinfo( ASKINFO_MATERIALMAKE, "", 2 );
end

-- 接收
-- m_count=0,m_list={m_matkind=0,m_matsec=0,m_matkind_will=0,m_matnum_will=0,[m_count]},
function MaterialMakeWillDlgRecv( recvValue )
	m_recvValue = recvValue;
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	
	local pMainBuilding = GetPlayer():GetBuilding( BUILDING_Main );
	local minvalue = g_building_upgrade[BUILDING_Main][pMainBuilding.m_level].value0
	local maxvalue = g_building_upgrade[BUILDING_Main][pMainBuilding.m_level].value1
	
	MaterialMakeWillDlgClear()
	
	for i=1, recvValue.m_count, 1 do
		local info = recvValue.m_list[i]
		local uiObj = m_ObjectPool:Get( "UIP_Make" );
		uiObj.transform:SetParent( m_uiContent.transform );
		
		if info.m_matkind > 0 then
			-- 可以预定生产
			if info.m_matkind_will > 0 then
				-- 已经预定生产
				SetControlID( uiObj.transform:Find("Content"), 0 )
				MaterialMakeWillDlgSetObject( uiObj, 
				ItemSprite(info.m_matkind_will), 
				ItemColorSprite(item_getcolor(info.m_matkind_will)), 
				"x"..info.m_matnum_will, 
				info.m_matsec, 
				nil, 
				nil );
					
			else
				-- 点击预定生产
				SetControlID( uiObj.transform:Find("Content"), 1 )
				MaterialMakeWillDlgSetObject( uiObj, 
				LoadSprite("ui_icon_back_4"), 
				nil, 
				nil, 
				nil, 
				T(925), 
				1 );
				
			end
		else
			-- 不能预定生产
			SetControlID( uiObj.transform:Find("Content"), 0 )
			MaterialMakeWillDlgSetObject( uiObj, 
			LoadSprite("ui_icon_back_4"), 
			nil, 
			nil, 
			nil, 
			T(926), 
			nil );
		end
		
	end
	
end

-- 设置对象
function MaterialMakeWillDlgSetObject( uiObj, shape, color, num, sec, wait, makebtn )
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiShape = objs[0];
	local uiColor = objs[1];
	local uiNum = objs[2];
	local uiTimer = objs[3];
	local uiWait = objs[4];
	local uiMakeBtn = objs[5];
	local uiNameBack = objs[6];
	if shape == nil then
		SetFalse( uiShape )
	else
		SetTrue( uiShape )
		SetImage( uiShape, shape );
	end
	
	if color == nil then
		SetFalse( uiColor )
	else
		SetTrue( uiColor )
		SetImage( uiColor, color );
	end
	
	if num == nil then
		SetFalse( uiNum )
		SetFalse( uiNameBack )
	else
		SetTrue( uiNum )
		SetText( uiNum, num );
		SetTrue( uiNameBack )
	end
	
	if sec == nil then
		SetFalse( uiTimer )
	else
		SetTrue( uiTimer )
		SetTimer( uiTimer, sec, sec, 1, T(923) );
	end
	
	if wait == nil then
		SetFalse( uiWait )
	else
		SetTrue( uiWait )
		SetText( uiWait, wait );
	end
	
	if makebtn == nil then
		SetFalse( uiMakeBtn )
	else
		SetTrue( uiMakeBtn )
	end
end
	
-- 清空
function MaterialMakeWillDlgClear()
	local objs = {};
	for i = 0 ,m_uiContent.transform.childCount - 1 do
		table.insert( objs, m_uiContent.transform:GetChild(i).gameObject )
    end
	for k, v in pairs(objs) do
		local obj = v;
		m_ObjectPool:Release( "UIP_Make", obj );
	end
end

-- 生产界面
function MaterialMakeWillDlgMakeInfo()
	SetTrue( m_uiMakeInfo );
	m_uiMakeInfo.transform:SetSiblingIndex(1000);
	MaterialMakeWillDlgSelect( 1 )
end

-- 选择生产内容
local colorsprite = { "ui_color_2", "ui_color_3", "ui_color_4", "ui_color_5" }
local colorname = { 898, 899, 900,1684 }
local matname = { 913, 914, 915,916 }
local drawingname = { 901, 902, 903,904 }
function MaterialMakeWillDlgSelect( id )
	m_id = id;
	local objs = m_uiMakeInfo.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiTags = objs[0];
	local uiInfo = objs[1];
	local uiItems = objs[2];
	local uiItemWarn = objs[3];
	local uiMakeBtn = objs[4];
	
	-- 选择颜色
	for i = 1, 4, 1 do
		local uiObj = uiTags.transform:GetChild(i-1).gameObject;
		if i <= m_pBuilding.m_level then
			SetControlID( uiObj, 10+i )
			SetImage( uiObj.transform:Find("Shape"), LoadSprite( colorsprite[i] ) )
			SetText( uiObj.transform:Find("Name"), T(colorname[i]), NameColor(i+1)  )
		else
			SetImage( uiObj.transform:Find("Shape"), LoadSprite( "ui_icon_back_2" ) )
			SetText( uiObj.transform:Find("Name"), F(911, i), Hex2Color(0x7D6C5AFF) )
		end
		if i == id then
			SetTrue( uiObj.transform:Find("Select") )
		else
			SetFalse( uiObj.transform:Find("Select") )
		end
	end
	
	-- 名称
	SetText( uiInfo.transform:Find("Name"), T(colorname[id]), NameColor(id+1) )
	
	-- 消耗
	local cost = "";
	cost = cost..F( matname[id], 1 ).."\n"
	
	if g_material_make[id].silver > GetPlayer().m_silver then
		cost = cost..T(121).."：<color=#E80017FF>"..knum(g_material_make[id].silver).."</color>/"..knum(GetPlayer().m_silver).."\n"
	else
		cost = cost..T(121).."："..knum(g_material_make[id].silver).."/"..knum(GetPlayer().m_silver).."\n"
	end
	
	if g_material_make[id].wood > GetPlayer().m_wood then
		cost = cost..T(122).."：<color=#E80017FF>"..knum(g_material_make[id].wood).."</color>/"..knum(GetPlayer().m_wood)..""
	else
		cost = cost..T(122).."："..knum(g_material_make[id].wood).."/"..knum(GetPlayer().m_wood)..""
	end
	SetText( uiInfo.transform:Find("Cost"), T(912)..": "..cost )
	
	-- 耗时
	local sec = MaterialMakeDlgSecNeed(id);
	SetText( uiInfo.transform:Find("Sec"), T(918)..": "..secnum(sec) )
	
	-- 图纸
	m_uiItemList = {};
	m_CacheItemList = GetItem():GetItemsByTypeWithMinColor( ITEM_TYPE_EQUIP_DRAWING, m_id, m_id );
	if #m_CacheItemList > 0 then
		SetTrue( uiItems )
		SetFalse( uiItemWarn )
		SetButtonTrue( uiMakeBtn )
		for i=1, 6, 1 do
			local uiObj = uiItems.transform:GetChild(i-1).gameObject;
			if i <= #m_CacheItemList then
				SetTrue( uiObj )
				SetControlID( uiObj, 100+i )
				SetImage( uiObj.transform:Find("Shape"), ItemSprite( m_CacheItemList[i].m_itemkind ) )
				SetImage( uiObj.transform:Find("Color"), ItemColorSprite( item_getcolor( m_CacheItemList[i].m_itemkind ) ) )
				SetText( uiObj.transform:Find("Name"), item_getname( m_CacheItemList[i].m_itemkind ), NameColor(item_getcolor( m_CacheItemList[i].m_itemkind )) )
				SetText( uiObj.transform:Find("Num"), "x"..m_CacheItemList[i].m_itemcount )
				SetImage( uiObj.transform:Find("Select"), LoadSprite("ui_button_select3") )
			else
				SetFalse( uiObj )
			end
		end
		MaterialMakeWillDlgSelectItem( 1 )
	else
		SetButtonFalse( uiMakeBtn )
		SetFalse( uiItems )
		SetTrue( uiItemWarn )
		SetText( uiItemWarn, F(910, T(drawingname[m_id]) ) )
	end
end

-- 选择道具
function MaterialMakeWillDlgSelectItem( index )
	m_itemkind = m_CacheItemList[index].m_itemkind;
	if m_itemkind == nil then
		m_itemkind = 0;
		return
	end
	
	local objs = m_uiMakeInfo.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiItems = objs[2];
	
	for i=1, 6, 1 do
		local uiObj = uiItems.transform:GetChild(i-1).gameObject;
		if i == index then
			SetImage( uiObj.transform:Find("Select"), LoadSprite("ui_button_select4") )
		else
			SetImage( uiObj.transform:Find("Select"), LoadSprite("ui_button_select3") )
		end
	end
end

-- 生产
function MaterialMakeWillDlgMake()
	if m_id <= 0 or m_itemkind <= 0 then
		pop(T(919))
		return
	end
	if GetPlayer().m_silver < g_material_make[m_id].silver then
		JumpRes(1,g_material_make[m_id].silver-GetPlayer().m_silver)
		return
	end
	if GetPlayer().m_wood < g_material_make[m_id].wood then
		JumpRes(2,g_material_make[m_id].wood-GetPlayer().m_wood)
		return
	end
	system_askinfo( ASKINFO_MATERIALMAKE, "", 5, m_id, m_itemkind );
	SetFalse( m_uiMakeInfo );
end


