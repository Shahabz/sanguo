-- 界面
local m_Dlg = nil;
local m_DialogFrameMod = nil;

local m_uiShape = nil; --UnityEngine.GameObject
local m_uiName = nil; --UnityEngine.GameObject
local m_uiLevel = nil; --UnityEngine.GameObject
local m_uiDesc1 = nil; --UnityEngine.GameObject
local m_uiDesc2 = nil; --UnityEngine.GameObject
local m_uiDesc3 = nil; --UnityEngine.GameObject
local m_uiSec = nil; --UnityEngine.GameObject
local m_uiCondition = nil; --UnityEngine.GameObject
local m_uiUIP_UpgradeCond = nil; --UnityEngine.GameObject

local m_kind = 0;
local m_offset = -1;
local m_pBuilding = nil;
local m_bUpgrade = true;
local m_recvValue = nil

-- 打开界面
function BuildingUpgradeDlgOpen()
	m_Dlg = eye.uiManager:Open( "BuildingUpgradeDlg" );
	m_DialogFrameMod = DialogFrameModOpen( m_Dlg, T(801), 8, BuildingUpgradeDlgClose );
	MainDlgClose()
	if City.m_Camera ~= nil then
		City.m_Camera.transform:GetComponent("ScreenBlur").enabled = true
	end
end

-- 隐藏界面
function BuildingUpgradeDlgClose()
	if City.m_Camera ~= nil then
		City.m_Camera.transform:GetComponent("ScreenBlur").enabled = false
	end
	MainDlgOpen()
	if m_Dlg == nil then
		return;
	end
	DialogFrameModClose( m_DialogFrameMod );
	m_DialogFrameMod = nil;
	eye.uiManager:Close( "BuildingUpgradeDlg" );
end

-- 删除界面
function BuildingUpgradeDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function BuildingUpgradeDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            BuildingUpgradeDlgClose();
		elseif nControlID == 1 then
			BuyWorkerDlgShow();
			BuildingUpgradeDlgClose();
		elseif nControlID == 4 then
			MaterialGetDlgShow( 120 )
		elseif nControlID == 5 then
			MaterialGetDlgShow( 121 )
		elseif nControlID == 6 then
			MaterialGetDlgShow( 122 )
		elseif nControlID == 7 then
			MaterialGetDlgShow( 123 )
		elseif nControlID == 100 then
			BuildingUpgradeDlgUpgrade()
        end
	end
end

-- 载入时调用
function BuildingUpgradeDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;
	m_uiShape = objs[0];
	m_uiName = objs[1];
	m_uiLevel = objs[2];
	m_uiDesc1 = objs[3];
	m_uiDesc2 = objs[4];
	m_uiDesc3 = objs[5];
	m_uiSec = objs[6];
	m_uiCondition = objs[7];
	m_uiUIP_UpgradeCond = objs[8];
end

-- 界面初始化时调用
function BuildingUpgradeDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function BuildingUpgradeDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function BuildingUpgradeDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function BuildingUpgradeDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function BuildingUpgradeDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function BuildingUpgradeDlgShow( kind, offset )
	m_kind = kind;
	m_offset = offset;
	BuildingUpgradeDlgOpen()
	clearChild( m_uiCondition )

	-- 请求数据
	system_askinfo( ASKINFO_BUILDING, "", 0, kind, offset );
	
	local m_pBuilding = GetPlayer():GetBuilding( kind, offset )
	SetImage( m_uiShape, BuildingSprite( kind ) );
	SetText( m_uiName, BuildingNameLv( kind, (offset%16), m_pBuilding.m_level ) );
	SetLevel( m_uiLevel, m_pBuilding.m_level+1 );
	SetText( m_uiSec, "" );
	
	-- 描述
	SetText( m_uiDesc1, "" )
	SetText( m_uiDesc2, "" )
	SetText( m_uiDesc3, "" )
	if kind == BUILDING_Main then
		SetText( m_uiDesc1, T(803) )
		
	elseif kind == BUILDING_Wall then
		SetText( m_uiDesc1, T(805) )
		
	elseif kind == BUILDING_StoreHouse then
		SetText( m_uiDesc1, T(806) )
	
	elseif kind == BUILDING_Tech then
		SetText( m_uiDesc1, T(807) )
	
	elseif kind == BUILDING_Craftsman then
		SetText( m_uiDesc1, T(809) )
	
	elseif kind == BUILDING_Cabinet then
		if m_pBuilding.m_level == 1 then
			SetText( m_uiDesc1, T(810) )
		elseif m_pBuilding.m_level == 2 then
			SetText( m_uiDesc1, T(811) )
		end
		
	elseif kind == BUILDING_Infantry then
		SetText( m_uiDesc1, T(812) )
		SetText( m_uiDesc2, T(791)..":"..g_building_upgrade[BUILDING_Infantry][m_pBuilding.m_level].value1.." +"..(g_building_upgrade[BUILDING_Infantry][m_pBuilding.m_level+1].value1-g_building_upgrade[BUILDING_Infantry][m_pBuilding.m_level].value1) )
		
	elseif kind == BUILDING_Cavalry then
		SetText( m_uiDesc1, T(813) )
		SetText( m_uiDesc2, T(791)..":"..g_building_upgrade[BUILDING_Cavalry][m_pBuilding.m_level].value1.." +"..(g_building_upgrade[BUILDING_Cavalry][m_pBuilding.m_level+1].value1-g_building_upgrade[BUILDING_Cavalry][m_pBuilding.m_level].value1) )
		
	elseif kind == BUILDING_Archer then
		SetText( m_uiDesc1, T(814) )
		SetText( m_uiDesc2, T(791)..":"..g_building_upgrade[BUILDING_Archer][m_pBuilding.m_level].value1.." +"..(g_building_upgrade[BUILDING_Archer][m_pBuilding.m_level+1].value1-g_building_upgrade[BUILDING_Archer][m_pBuilding.m_level].value1) )
		
	elseif kind == BUILDING_Militiaman_Infantry then
		SetText( m_uiDesc1, T(815) )
		SetText( m_uiDesc2, T(791)..":"..g_building_upgrade[BUILDING_Militiaman_Infantry][m_pBuilding.m_level].value1.." +"..(g_building_upgrade[BUILDING_Militiaman_Infantry][m_pBuilding.m_level+1].value1-g_building_upgrade[BUILDING_Militiaman_Infantry][m_pBuilding.m_level].value1) )
		
	elseif kind == BUILDING_Militiaman_Cavalry then
		SetText( m_uiDesc1, T(815) )
		SetText( m_uiDesc2, T(791)..":"..g_building_upgrade[BUILDING_Militiaman_Cavalry][m_pBuilding.m_level].value1.." +"..(g_building_upgrade[BUILDING_Militiaman_Cavalry][m_pBuilding.m_level+1].value1-g_building_upgrade[BUILDING_Militiaman_Cavalry][m_pBuilding.m_level].value1) )
		
	elseif kind == BUILDING_Militiaman_Archer then
		SetText( m_uiDesc1, T(815) )
		SetText( m_uiDesc2, T(791)..":"..g_building_upgrade[BUILDING_Militiaman_Archer][m_pBuilding.m_level].value1.." +"..(g_building_upgrade[BUILDING_Militiaman_Archer][m_pBuilding.m_level+1].value1-g_building_upgrade[BUILDING_Militiaman_Archer][m_pBuilding.m_level].value1) )
		
	elseif kind == BUILDING_Silver then
		SetText( m_uiDesc1, T(816) )
		SetText( m_uiDesc2, T(820)..":"..g_building_upgrade[BUILDING_Silver][m_pBuilding.m_level].value1.." +"..(g_building_upgrade[BUILDING_Silver][m_pBuilding.m_level+1].value1-g_building_upgrade[BUILDING_Silver][m_pBuilding.m_level].value1) )
		
	elseif kind == BUILDING_Wood then
		SetText( m_uiDesc1, T(817) )
		SetText( m_uiDesc2, T(820)..":"..g_building_upgrade[BUILDING_Wood][m_pBuilding.m_level].value1.." +"..(g_building_upgrade[BUILDING_Wood][m_pBuilding.m_level+1].value1-g_building_upgrade[BUILDING_Wood][m_pBuilding.m_level].value1) )
	
	elseif kind == BUILDING_Food then
		SetText( m_uiDesc1, T(818) )
		SetText( m_uiDesc2, T(820)..":"..g_building_upgrade[BUILDING_Food][m_pBuilding.m_level].value1.." +"..(g_building_upgrade[BUILDING_Food][m_pBuilding.m_level+1].value1-g_building_upgrade[BUILDING_Food][m_pBuilding.m_level].value1) )
	
	elseif kind == BUILDING_Iron then
		SetText( m_uiDesc1, T(819) )
		SetText( m_uiDesc2, T(820)..":"..g_building_upgrade[BUILDING_Iron][m_pBuilding.m_level].value1.." +"..(g_building_upgrade[BUILDING_Iron][m_pBuilding.m_level+1].value1-g_building_upgrade[BUILDING_Iron][m_pBuilding.m_level].value1) )
		
	end
	
	
	-- 空闲建造队是否满足
	local uiObj = addChild( m_uiCondition, m_uiUIP_UpgradeCond );
	local flag = true;
	if GetPlayer().m_worker_kind > 0 and GetPlayer().m_worker_kind_ex > 0 then
		flag = false;
	elseif GetPlayer().m_worker_kind > 0 and GetPlayer().m_worker_expire_ex <= 0 then
		flag = false;
	end
	BuildingUpgradeCondSet( uiObj, T(607), flag, 1 )
end

-- 数据返回
function BuildingUpgradeDlgRecv( recvValue )
	m_recvValue = recvValue;
	local uiObj = nil;
	local flag = true;
	m_bUpgrade = true;
	
	-- 时间
	SetText( m_uiSec, secnum( recvValue.m_sec ) );
	
	-- 官府等级是否满足
	if recvValue.m_citylevel > 0 then
		uiObj = addChild( m_uiCondition, m_uiUIP_UpgradeCond );
		flag = true;
		if GetPlayer():CityLevel() < recvValue.m_citylevel then
			flag = false;
			m_bUpgrade = false;
		end
		BuildingUpgradeCondSet( uiObj, T(1).." "..F( 98, recvValue.m_citylevel ), flag, 2 );
	end
	-- 主公等级是否满足
	if recvValue.m_actorlevel > 0 then
		uiObj = addChild( m_uiCondition, m_uiUIP_UpgradeCond );
		flag = true;
		if GetPlayer().m_level < recvValue.m_actorlevel then
			flag = false;
			m_bUpgrade = false;
		end
		BuildingUpgradeCondSet( uiObj, T(100).." "..F( 98, recvValue.m_actorlevel ), flag, 3 );
	end
	
	-- 银币是否满足
	if recvValue.m_silver > 0 then
		uiObj = addChild( m_uiCondition, m_uiUIP_UpgradeCond );
		flag = true;
		if GetPlayer().m_silver < recvValue.m_silver then
			flag = false;
			m_bUpgrade = false;
		end
		BuildingUpgradeCondSet( uiObj, T(121).." "..knum(recvValue.m_silver).."/"..knum(GetPlayer().m_silver), flag, 4 );
	end
	
	-- 木材是否满足
	if recvValue.m_wood > 0 then
		uiObj = addChild( m_uiCondition, m_uiUIP_UpgradeCond );
		flag = true;
		if GetPlayer().m_wood < recvValue.m_wood then
			flag = false;
			m_bUpgrade = false;
		end
		BuildingUpgradeCondSet( uiObj, T(122).." "..knum(recvValue.m_wood).."/"..knum(GetPlayer().m_wood), flag, 5 );
	end
	
	-- 粮草是否满足
	if recvValue.m_food > 0 then
		uiObj = addChild( m_uiCondition, m_uiUIP_UpgradeCond );
		flag = true;
		if GetPlayer().m_food < recvValue.m_food then
			flag = false;
			m_bUpgrade = false;
		end
		BuildingUpgradeCondSet( uiObj, T(123).." "..knum(recvValue.m_food).."/"..knum(GetPlayer().m_food), flag, 6 );
	end
	
	-- 镔铁是否满足
	if recvValue.m_iron > 0 then
		uiObj = addChild( m_uiCondition, m_uiUIP_UpgradeCond );
		flag = true;
		if GetPlayer().m_iron < recvValue.m_iron then
			flag = false;
			m_bUpgrade = false;
		end
		BuildingUpgradeCondSet( uiObj, T(124).." "..knum(recvValue.m_iron).."/"..knum(GetPlayer().m_iron), flag, 7 );
	end
end

function BuildingUpgradeDlgUpdate()
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	clearChild( m_uiCondition )
	BuildingUpgradeDlgRecv( m_recvValue )
end

function BuildingUpgradeCondSet( uiObj, text, flag, type )
	local color = Hex2Color( 0xf7f3bbff );
	local flagname = "ui_cond_yes";
	if flag == false then
		color = Hex2Color( 0xe80017ff );
		flagname = "ui_cond_no";
	end
	
	-- 条件描述
	local uiObjText = uiObj.transform:Find("Text");
	SetText( uiObjText, text, color );
	SetTrue( uiObjText );
	
	-- 对号错号
	local uiFlag = uiObj.transform:Find("Flag");
	--if type == 2 or type == 3 then
		SetImage( uiFlag, LoadSprite(flagname) );
		SetTrue( uiFlag );
	--else
		--SetFalse( uiFlag );
	--end
	
	-- 按钮
	local uiButton = uiObj.transform:Find("Button");
	if flag == false then
		if type == 1 and GetPlayer().m_worker_expire_ex > 0 then
			SetFalse( uiButton );
		elseif type == 2 or type == 3 then
			SetFalse( uiButton );
		else	
			if type == 1 then
				SetText( uiButton.transform:Find("Back/Text"), T(609) ); -- 开启
			else
				SetText( uiButton.transform:Find("Back/Text"), T(608) ); -- 获取
			end
			SetControlID( uiButton, type )
			SetTrue( uiButton );
		end
	else
		SetFalse( uiButton );
	end
end

-- 领取奖励
function BuildingUpgradeDlgUpgrade()
	if m_bUpgrade == false then
		pop(T(802))
		return
	end	
	system_askinfo( ASKINFO_BUILDING, "", 1, m_kind, m_offset );
	BuildingUpgradeDlgClose();
end

