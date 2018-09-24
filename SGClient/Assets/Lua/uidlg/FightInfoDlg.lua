-- 界面
local m_Dlg = nil;
local m_uiTitleText = nil; --UnityEngine.GameObject
local m_uiCountDown = nil; --UnityEngine.GameObject
local m_uiLeftName = nil; --UnityEngine.GameObject
local m_uiRightName = nil; --UnityEngine.GameObject
local m_uiLeftNation = nil; --UnityEngine.GameObject
local m_uiRightNation = nil; --UnityEngine.GameObject
local m_uiLeftName = nil; --UnityEngine.GameObject
local m_uiRightName = nil; --UnityEngine.GameObject
local m_uiLeftScroll = nil; --UnityEngine.GameObject
local m_uiLeftContent = nil; --UnityEngine.GameObject
local m_uiRightScroll = nil; --UnityEngine.GameObject
local m_uiRightContent = nil; --UnityEngine.GameObject
local m_uiUIP_Unit = nil; --UnityEngine.GameObject
local m_uiWarningText = nil; --UnityEngine.GameObject
local m_uiCenterContent = nil; --UnityEngine.GameObject
local m_uiFastBtn = nil; --UnityEngine.GameObject
local m_start = 0;
-- 打开界面
function FightInfoDlgOpen()
	ResourceManager.LoadAssetBundle( "_ab_ui_static_battle_back" );
	m_Dlg = eye.uiManager:Open( "FightInfoDlg" );
end

-- 隐藏界面
function FightInfoDlgClose()
	if m_Dlg == nil then
		return;
	end
	FightInfoDlgClearLeftUnit()
	FightInfoDlgClearRightUnit()
	eye.uiManager:Close( "FightInfoDlg" );
end

-- 删除界面
function FightInfoDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
	ResourceManager.UnloadAssetBundle( "_ab_ui_static_battle_back" );
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function FightInfoDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            FightInfoDlgClose();
		elseif nControlID == 1 then
			FightInfoDlgFast();
        end
	elseif nType == UI_EVENT_TIMECOUNTEND then
		if m_start == 0 then 
			FightInfoDlgFast();
		end
	end
end

-- 载入时调用
function FightInfoDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiTitleText = objs[0];
	m_uiCountDown = objs[1];
	m_uiLeftName = objs[2];
	m_uiRightName = objs[3];
	m_uiLeftNation = objs[4];
	m_uiRightNation = objs[5];
	m_uiLeftName = objs[6];
	m_uiRightName = objs[7];
	m_uiLeftScroll = objs[8];
	m_uiLeftContent = objs[9];
	m_uiRightScroll = objs[10];
	m_uiRightContent = objs[11];
	m_uiUIP_Unit = objs[12];
	m_uiWarningText = objs[13];	
	m_uiCenterContent = objs[14];
	m_uiFastBtn = objs[15];

	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );
	m_ObjectPool:CreatePool("UIP_Unit", 8, 8, m_uiUIP_Unit);
end

-- 界面初始化时调用
function FightInfoDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function FightInfoDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function FightInfoDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function FightInfoDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function FightInfoDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function FightInfoDlgShow( info )
	FightInfoDlgOpen()
	-- 战斗类型	
	local fighttype = info["ft"]
	if fighttype == nil then
		SetText( m_uiTitleText, "" )
		SetText( m_uiWarningText, "" )
	else
		SetText( m_uiTitleText, T(2000+fighttype-1) )
		if fighttype == FIGHTTYPE_STORY then			
			SetText( m_uiTitleText, StoryChapterName( math.floor(BattleDlgGetStoryid()/10) ) )
			SetText( m_uiWarningText, T(2019) )
		else
			SetText( m_uiWarningText, "" )
		end
	end
	SetTimer( m_uiCountDown, 10, 10, 1 )
	m_start = 0;
	
	-- 我是攻击方，攻击方显示左面
	local my = info["my"];
	if my == nil or my == 1 then
		-- 攻击方
		if info["a_type"] == 0 and info["a_name"] == "" then 
			SetText( m_uiLeftName, T(1360) );
		else
			SetText( m_uiLeftName, FightInfoDlgGetName( info["a_type"], info["a_name"], info["a_nation"] ) );
		end
		if info["a_nation"] > 0 then
			SetTrue( m_uiLeftNation )
			SetImage( m_uiLeftNation, NationSprite( info["a_nation"] ) );
		else
			SetFalse( m_uiLeftNation )			
		end
		-- 防御方
		if info["d_type"] == 0 and info["d_name"] == "" then 
			SetText( m_uiRightName, T(1361) );
		else
			SetText( m_uiRightName, FightInfoDlgGetName( info["d_type"], info["d_name"], info["d_nation"] ) );
		end
		if info["d_nation"] > 0 then
			SetTrue( m_uiRightNation )
			SetImage( m_uiRightNation, NationSprite( info["d_nation"] ) );
		else
			SetFalse( m_uiRightNation )			
		end
		
		-- 攻击方对象
		for i=1, #info["a_unit"], 1 do
			local unit = info["a_unit"][i];
			FightInfoDlgUnit( m_uiLeftContent, i, unit["t"], unit["kd"], unit["na"], unit["sp"], unit["cr"], unit["cs"], unit["lv"], unit["mhp"], unit["hp"] )
		end
		
		-- 防御方对象
		for i=1, #info["d_unit"], 1 do
			local unit = info["d_unit"][i];
			FightInfoDlgUnit( m_uiRightContent, i, unit["t"], unit["kd"], unit["na"], unit["sp"], unit["cr"], unit["cs"], unit["lv"], unit["mhp"], unit["hp"] )
		end
	
	-- 我是防御方，防御方显示左面
	else
		-- 攻击方
		SetText( m_uiLeftName, FightInfoDlgGetName( info["d_type"], info["d_name"], info["d_nation"] ) );
		if info["d_nation"] > 0 then
			SetTrue( m_uiLeftNation )
			SetImage( m_uiLeftNation, NationSprite( info["d_nation"] ) );
		else
			SetFalse( m_uiLeftNation )
		end
		-- 防御方
		SetText( m_uiRightName, FightInfoDlgGetName( info["a_type"], info["a_name"], info["a_nation"] ) );
		if info["a_nation"] > 0 then
			SetTrue( m_uiRightNation )
			SetImage( m_uiRightNation, NationSprite( info["a_nation"] ) );
		else
			SetFalse( m_uiRightNation )
		end
		
		
		-- 攻击方对象
		for i=1, #info["d_unit"], 1 do
			local unit = info["d_unit"][i];
			FightInfoDlgUnit( m_uiRightContent, i, unit["t"], unit["kd"], unit["na"], unit["sp"], unit["cr"], unit["cs"], unit["lv"], unit["mhp"], unit["hp"] )
		end
		
		-- 防御方对象
		for i=1, #info["a_unit"], 1 do
			local unit = info["a_unit"][i];
			FightInfoDlgUnit( m_uiLeftContent, i, unit["t"], unit["kd"], unit["na"], unit["sp"], unit["cr"], unit["cs"], unit["lv"], unit["mhp"], unit["hp"] )
		end
	end	
	
	FightDlgCenter(my,info["a_unit"],info["d_unit"]);
	SetTrue(m_uiFastBtn)
end

-- 设置一个unit信息
function FightInfoDlgUnit( root, index, unittype, kind, name, shape, color, corps, level, maxhp, hp )
	local uiObj = m_ObjectPool:Get( "UIP_Unit" );
	if uiObj == nil then
		gamelog( "FightInfoDlgUnit uiObj == nil" )
		return
	end
	uiObj.transform:SetParent( root.transform );
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiShape = objs[0];
	local uiColor = objs[1];
	local uiCorps = objs[2];
	local uiName = objs[3];
	local uiHp = objs[4];
	local uiSort = objs[5];	
	local uiUpButton = objs[6];
	local uiFristButton = objs[7];	
	
	SetFalse( uiUpButton )
	if hp == nil then
		SetFalse( uiHp )
	else
		SetTrue( uiHp )
		SetText( uiHp, maxhp )
	end
	-- 玩家英雄
	if unittype == FIGHT_UNITTYPE_LEADER_HERO or unittype == FIGHT_UNITTYPE_HERO then
		SetImage( uiShape, HeroHeadSprite( kind ) );
		SetImage( uiColor, ItemColorSprite( color ) );
		SetImage( uiCorps, CorpsSprite( corps ) );
		if name and name ~= "" then
			SetText( uiName, "["..name.."]".."Lv."..level.." "..HeroName( kind ) );
		else
			SetText( uiName, "Lv."..level.." "..HeroName( kind ) );
		end
		
	-- 玩家城墙守卫
	elseif unittype == FIGHT_UNITTYPE_GUARD then
		SetImage( uiShape, GuardSprite( shape ) );
		SetImage( uiColor, ItemColorSprite( color ) );
		SetImage( uiCorps, CorpsSprite( corps ) );
		SetText( uiName, "Lv."..level.." "..T( 1119 ) );
		
	-- 怪物表
	elseif unittype == FIGHT_UNITTYPE_MONSTER then
		SetImage( uiShape, EnemyHeadSprite( shape ) );
		SetImage( uiColor, ItemColorSprite( color ) );
		SetImage( uiCorps, CorpsSprite( corps ) );
		SetText( uiName, EnemyName( kind ) );
	end
	SetText( uiSort, index );
end

--设置兵种相克
function FightDlgCenter(my,a_unit,d_unit)
	local MinNum = FightDlgGetCenterNum(#a_unit,#d_unit)
	for i = 0 ,m_uiCenterContent.transform.childCount - 1 do
		local uiObj = m_uiCenterContent.transform:GetChild(i).gameObject		
		if i+1 > MinNum then 
			SetFalse(uiObj);
		else
			SetTrue(uiObj);
			local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
			local uiBack = objs[0]
			local a_Corps = a_unit[i+1]["cs"];
			local d_Corps = d_unit[i+1]["cs"];
			SetImage(uiBack,FightDlgGetCenterImage(a_Corps,d_Corps))
		end
	end	
end

-- 获得兵种相克数量
function FightDlgGetCenterNum(a_Num,d_Num)
	if a_Num > d_Num then 
		return d_Num
	else
		return a_Num
	end
	return a_Num
end

-- 获得兵种相克图片
function FightDlgGetCenterImage(a_Corps,d_Corps)
	if a_Corps == d_Corps then 
		return LoadSprite("ui_battle_icon_5")
	else
		if a_Corps == 0 then 
			if d_Corps == 1 then
				return LoadSprite("ui_battle_icon_4")
			elseif d_Corps == 2 then 
				return LoadSprite("ui_battle_icon_3")
			end
		elseif a_Corps == 1 then 
			if d_Corps == 0 then
				return LoadSprite("ui_battle_icon_3")
			elseif d_Corps == 2 then 
				return LoadSprite("ui_battle_icon_4")
			end
		elseif a_Corps == 2 then 
			if d_Corps == 0 then
				return LoadSprite("ui_battle_icon_4")
			elseif d_Corps == 1 then 
				return LoadSprite("ui_battle_icon_3")
			end
		end
	end
end

-- 设置双方主角信息
function FightInfoDlgGetName( type, name, nation )
	-- 玩家 部队 资源点
	if type == MAPUNIT_TYPE_CITY or type == MAPUNIT_TYPE_ARMY or type == MAPUNIT_TYPE_RES then
		return name;
			
	-- 城镇
	elseif type == MAPUNIT_TYPE_TOWN then
		return EnemyName( tonumber(name) )
		
	-- 流寇或国家名将
	elseif type == MAPUNIT_TYPE_ENEMY or type == MAPUNIT_TYPE_NATIONHERO or type == MAPUNIT_TYPE_ACTIVITY then
		return EnemyName( tonumber(name) )
	end
	
	return ""
end

-- 清空
function FightInfoDlgClearLeftUnit()
	local objs = {};
	for i = 0 ,m_uiLeftContent.transform.childCount - 1 do
		table.insert( objs, m_uiLeftContent.transform:GetChild(i).gameObject )
    end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "UIP_Unit(Clone)" then
			m_ObjectPool:Release( "UIP_Unit", obj );
		end
    end
end

-- 清空
function FightInfoDlgClearRightUnit()
	local objs = {};
	for i = 0 ,m_uiRightContent.transform.childCount - 1 do
		table.insert( objs, m_uiRightContent.transform:GetChild(i).gameObject )
    end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "UIP_Unit(Clone)" then
			m_ObjectPool:Release( "UIP_Unit", obj );
		end
    end
end

-- 快速开始
function FightInfoDlgFast()
	m_start = 1;
	SetFalse(m_uiFastBtn)
	FightDlgStart()
	FightInfoDlgClose()
end

function FightInfoDlgIsShow()
	if m_Dlg ~= nil then
		return IsActive( m_Dlg )
	end
	return false;
end