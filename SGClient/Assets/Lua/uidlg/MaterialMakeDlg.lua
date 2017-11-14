-- 界面
local m_Dlg = nil;
local m_DialogFrameMod = nil;
local m_uiNationPeople = nil; --UnityEngine.GameObject
local m_uiCityPeople = nil; --UnityEngine.GameObject
local m_uiCityInfo = nil; --UnityEngine.GameObject
local m_uiScroll = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiUIP_Make = nil; --UnityEngine.GameObject
local m_uiWillBtn = nil; --UnityEngine.GameObject
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
function MaterialMakeDlgOpen()
	m_Dlg = eye.uiManager:Open( "MaterialMakeDlg" );
	m_DialogFrameMod = DialogFrameModOpen( m_Dlg, BuildingName(BUILDING_Craftsman).." Lv."..m_pBuilding.m_level, HELP_MaterialMakeDlg, MaterialMakeDlgClose );
end

-- 隐藏界面
function MaterialMakeDlgClose()
	if m_Dlg == nil then
		return;
	end
	DialogFrameModClose( m_DialogFrameMod );
	m_DialogFrameMod = nil;
	eye.uiManager:Close( "MaterialMakeDlg" );
end

-- 删除界面
function MaterialMakeDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function MaterialMakeDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            MaterialMakeDlgClose();
		elseif nControlID == -2 then
			SetFalse( m_uiMakeInfo );
		elseif nControlID == 1 then
			MaterialMakeDlgMakeInfo();
		elseif nControlID == 2 then
			MaterialMakeDlgQueue();
		elseif nControlID == 3 then
			MaterialMakeDlgMake();
		elseif nControlID == 4 then
			MaterialMakeWillDlgShow()
		elseif nControlID >= 10 and nControlID < 20 then
			MaterialMakeDlgSelect( nControlID-10 )
		elseif nControlID >= 100 and nControlID < 110 then
			MaterialMakeDlgSelectItem( nControlID-100 )
        end
	end
end

-- 载入时调用
function MaterialMakeDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiNationPeople = objs[0];
	m_uiCityPeople = objs[1];
	m_uiCityInfo = objs[2];
	m_uiScroll = objs[3];
	m_uiContent = objs[4];
	m_uiUIP_Make = objs[5];
	m_uiWillBtn = objs[6];
	m_uiMakeInfo = objs[7];

	-- 对象池
	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );
	m_ObjectPool:CreatePool("UIP_Make", 3, 3, m_uiUIP_Make);
end

-- 界面初始化时调用
function MaterialMakeDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function MaterialMakeDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function MaterialMakeDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function MaterialMakeDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function MaterialMakeDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function MaterialMakeDlgShow()
	m_pBuilding = GetPlayer():GetBuilding( BUILDING_Craftsman );
	MaterialMakeDlgOpen()
	m_uiWillBtn.transform:SetSiblingIndex(1000);
	
	-- 请求信息
	system_askinfo( ASKINFO_MATERIALMAKE, "", 0 );
end

-- 接收
-- m_count=0,m_list={m_matkind=0,m_matnum=0,m_matsec=0,m_matsec_need=0,[m_count]},m_matquenum=0,
function MaterialMakeDlgRecv( recvValue )
	m_recvValue = recvValue;
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	
	local pMainBuilding = GetPlayer():GetBuilding( BUILDING_Main );
	local minvalue = g_building_upgrade[BUILDING_Main][pMainBuilding.m_level].value0
	local maxvalue = g_building_upgrade[BUILDING_Main][pMainBuilding.m_level].value1
	
	-- 人口变动
	if recvValue.m_change_city_people ~= 0 or recvValue.m_change_nation_people ~= 0 then
		local change_city = "";
		if recvValue.m_change_city_people >= 0 then
			change_city = "<color=#03de27>+"..recvValue.m_change_city_people.."</color>"
		elseif recvValue.m_change_city_people < 0 then
			change_city = "<color=#e80017>-"..recvValue.m_change_city_people.."</color>"
		end
		
		local change_nation = "";
		if recvValue.m_change_nation_people >= 0 then
			change_nation = "<color=#03de27>+"..recvValue.m_change_nation_people.."</color>"
		elseif recvValue.m_change_nation_people < 0 then
			change_nation = "<color=#e80017>-"..recvValue.m_change_nation_people.."</color>"
		end
		local statemsg = T(931)
		if recvValue.m_change_city_people+recvValue.m_change_nation_people > 0 then
			statemsg = "<color=#03de27>"..T(930).."</color>"
		elseif recvValue.m_change_city_people+recvValue.m_change_nation_people < 0 then
			statemsg = "<color=#e80017>"..T(929).."</color>"
		end
		NpcTalk( T(927).."\n"..T(888)..":"..change_city.."\n"..T(887)..":"..change_nation.."\n"..T(928)..":"..statemsg );
	end
	
	-- 国家人口
	SetTrue( m_uiNationPeople )
	SetText( m_uiNationPeople.transform:Find("Text"), T(887)..": <color=#f7f3bb>"..recvValue.m_nation_people.."</color><color=#03de27>("..T(889).."+"..T(890)..")</color>" );
	
	-- 城池人口
	SetTrue( m_uiCityPeople )
	if recvValue.m_city_people < minvalue then 
		-- 人口增长中
		SetText( m_uiCityPeople.transform:Find("Text"), T(888)..": <color=#f7f3bb>"..recvValue.m_city_people.."</color><color=#03de27>("..T(896)..")</color>" )
		
	elseif recvValue.m_city_people > maxvalue then 
		-- 人口过剩
		SetText( m_uiCityPeople.transform:Find("Text"), T(888)..": <color=#f7f3bb>"..recvValue.m_city_people.."</color><color=#e80017>("..T(895)..")</color>" )
		
	else
		-- 人口饱和
		SetText( m_uiCityPeople.transform:Find("Text"), T(888)..": <color=#f7f3bb>"..recvValue.m_city_people.."("..T(894)..")</color>" )
	end
	
	-- 城池信息
	SetTrue( m_uiCityInfo )
	SetText( m_uiCityInfo.transform:Find("Text"), T(891)..": <color=#f7f3bb>"..F(893, pMainBuilding.m_level, minvalue, maxvalue ).."</color>" )

	MaterialMakeDlgClear()
	
	-- 先算出有多少队列参与生产
	m_makeing_count = 0;
	for i=1, recvValue.m_count, 1 do
		m_makeing_count = m_makeing_count + 1;
	end
	
	-- 正在生产中
	for i=1, recvValue.m_count, 1 do
		local info = recvValue.m_list[i]
		local uiObj = m_ObjectPool:Get( "UIP_Make" );
		uiObj.transform:SetParent( m_uiContent.transform );
		SetControlID( uiObj.transform:Find("Content"), 0 )
		MaterialMakeDlgSetObject( uiObj, 
		ItemSprite(info.m_matkind), 
		ItemColorSprite(item_getcolor(info.m_matkind)), 
		"x"..info.m_matnum, 
		F( 884, math.ceil(recvValue.m_city_people/m_makeing_count), math.ceil(recvValue.m_nation_people/m_makeing_count) ), 
		info.m_matsec, 
		info.m_matsec_need, 
		nil, 
		nil,
		nil );
	end
	
	-- 等待中
	for i=recvValue.m_count+1, recvValue.m_matquenum+2, 1 do
		local uiObj = m_ObjectPool:Get( "UIP_Make" );
		uiObj.transform:SetParent( m_uiContent.transform );
		SetControlID( uiObj.transform:Find("Content"), 1 )
		MaterialMakeDlgSetObject( uiObj, 
		LoadSprite("ui_icon_back_4"), 
		nil, 
		nil, 
		nil, 
		nil, 
		nil, 
		T(885), 
		1,
		nil );
	end
	
	-- 扩建
	if recvValue.m_matquenum < 4 then
		local uiObj = m_ObjectPool:Get( "UIP_Make" );
		uiObj.transform:SetParent( m_uiContent.transform );
		SetControlID( uiObj.transform:Find("Content"), 2 )
		MaterialMakeDlgSetObject( uiObj, 
		LoadSprite("ui_icon_back_4"), 
		nil, 
		nil, 
		nil, 
		nil, 
		nil, 
		"<color=#03de27ff>"..T(886).."</color>", 
		nil,
		1 );
	end
end

-- 设置对象
function MaterialMakeDlgSetObject( uiObj, shape, color, num, desc, sec, secneed, wait, makebtn, queuebtn )
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiShape = objs[0];
	local uiColor = objs[1];
	local uiNum = objs[2];
	local uiDesc = objs[3];
	local uiTimer = objs[4];
	local uiWait = objs[5];
	local uiProgress = objs[6];
	local uiMakeBtn = objs[7];
	local uiQueueBtn = objs[8];
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
	else
		SetTrue( uiNum )
		SetText( uiNum, num );
	end
	
	if desc == nil then
		SetFalse( uiDesc )
	else
		SetTrue( uiDesc )
		SetText( uiDesc, desc );
	end
	
	if sec == nil then
		SetFalse( uiTimer )
		SetFalse( uiProgress )
	else
		SetTrue( uiTimer )
		SetTimer( uiTimer, secneed-sec, secneed, 1, T(907) );
		SetTrue( uiProgress )
		uiProgress.transform:GetComponent( typeof(UITextAgent) )._formatText = T(920);
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
	
	if queuebtn == nil then
		SetFalse( uiQueueBtn )
	else
		SetTrue( uiQueueBtn )
	end
end
	
-- 清空
function MaterialMakeDlgClear()
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
function MaterialMakeDlgMakeInfo()
	SetTrue( m_uiMakeInfo );
	m_uiMakeInfo.transform:SetSiblingIndex(1000);
	MaterialMakeDlgSelect( 1 )
end

-- 选择生产内容
local colorsprite = { "ui_color_2", "ui_color_3", "ui_color_4" }
local colorname = { 898, 899, 900 }
local matname = { 913, 914, 915 }
local drawingname = { 901, 902, 903 }
function MaterialMakeDlgSelect( id )
	m_id = id;
	local objs = m_uiMakeInfo.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiTags = objs[0];
	local uiInfo = objs[1];
	local uiItems = objs[2];
	local uiItemWarn = objs[3];
	
	-- 选择颜色
	for i = 1, 3, 1 do
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
	cost = cost..F( matname[id], 1 )
	cost = cost.."<color=#f7f3bb>,"..T(121).."x"..g_material_make[id].silver..","
	cost = cost..T(122).."x"..g_material_make[id].wood.."</color>"
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
		for i=1, 6, 1 do
			local uiObj = uiItems.transform:GetChild(i-1).gameObject;
			if i <= #m_CacheItemList then
				SetTrue( uiObj )
				SetControlID( uiObj, 100+i )
				SetImage( uiObj.transform:Find("Shape"), ItemSprite( m_CacheItemList[i].m_itemkind ) )
				SetImage( uiObj.transform:Find("Color"), ItemColorSprite( item_getcolor( m_CacheItemList[i].m_itemkind ) ) )
				SetText( uiObj.transform:Find("Name"), item_getname( m_CacheItemList[i].m_itemkind ) )
				SetText( uiObj.transform:Find("Num"), "x"..m_CacheItemList[i].m_itemcount )
				SetImage( uiObj.transform:Find("Select"), LoadSprite("ui_button_select3") )
			else
				SetFalse( uiObj )
			end
		end
		MaterialMakeDlgSelectItem( 1 )
	else
		SetFalse( uiItems )
		SetTrue( uiItemWarn )
		SetText( uiItemWarn, F(910, T(drawingname[m_id]) ) )
	end
end

-- 选择道具
function MaterialMakeDlgSelectItem( index )
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
function MaterialMakeDlgMake()
	if m_id <= 0 or m_itemkind <= 0 then
		pop(T(919))
		return
	end
	if GetPlayer().m_silver < g_material_make[m_id].silver then
		JumpRes(1)
		return
	end
	if GetPlayer().m_wood < g_material_make[m_id].wood then
		JumpRes(2)
		return
	end
	system_askinfo( ASKINFO_MATERIALMAKE, "", 1, m_id, m_itemkind );
	SetFalse( m_uiMakeInfo );
end

-- 预计耗时，注意，数量m_makeing_count要+1
function MaterialMakeDlgSecNeed( id )
	local people = math.floor( (m_recvValue.m_city_people+m_recvValue.m_nation_people) / (m_makeing_count+1) );
	local needsec = math.ceil( math.max( g_material_make[id].sec * global.material_make_value1, g_material_make[id].sec - global.material_make_value2*people ) );
	needsec = math.ceil( needsec*(1.0-GetPlayer().m_attr.m_materialsec_per[1]/100)*(1.0-GetPlayer().m_attr.m_materialsec_per[2]/100) );
	return needsec
end

-- 扩容
local s_material_queue_token = { 500, 2000, 10000, 10000, 10000, 10000, 10000, 10000, 10000 };
function MaterialMakeDlgQueue()
	local token = s_material_queue_token[m_recvValue.m_matquenum+1]
	MsgBox( F(897, token ), function() 
		if GetPlayer().m_token < token then
			JumpToken()
		else
			system_askinfo( ASKINFO_MATERIALMAKE, "", 3 );
		end
	end )
end
