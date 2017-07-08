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

-- 打开界面
function BuildingUpgradeDlgOpen()
	m_Dlg = eye.uiManager:Open( "BuildingUpgradeDlg" );
	m_DialogFrameMod = DialogFrameModOpen( m_Dlg, "主城建设", 8, BuildingUpgradeDlgClose );
end

-- 隐藏界面
function BuildingUpgradeDlgClose()
	if m_Dlg == nil then
		return;
	end
	DialogFrameModClose( m_DialogFrameMod );
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
		elseif nControlID == 5 then
		elseif nControlID == 6 then
		elseif nControlID == 7 then
		elseif nControlID == 100 then
			Notify( "木材厂升级完毕" )
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
	
	local m_pBuilding = GetPlayer():GetBuilding( kind, offset )
	SetImage( m_uiShape, BuildingSprite( kind ) );
	SetText( m_uiName, BuildingNameLv( kind, (offset%16+1), m_pBuilding.m_level ) );
	SetLevel( m_uiLevel, m_pBuilding.m_level+1 );
	
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
	local uiObj = nil;
	local flag = true;
	-- 官府等级是否满足
	if recvValue.m_citylevel > 0 then
		uiObj = addChild( m_uiCondition, m_uiUIP_UpgradeCond );
		flag = true;
		if GetPlayer().CityLevel() < recvValue.m_citylevel then
			flag = false;
		end
		BuildingUpgradeCondSet( uiObj, T(1).." "..F( 98, recvValue.m_citylevel ), flag, 2 );
	end
	-- 主公等级是否满足
	if recvValue.m_actorlevel > 0 then
		uiObj = addChild( m_uiCondition, m_uiUIP_UpgradeCond );
		flag = true;
		if GetPlayer().m_level() < recvValue.m_actorlevel then
			flag = false;
		end
		BuildingUpgradeCondSet( uiObj, T(100).." "..F( 98, recvValue.m_actorlevel ), flag, 3 );
	end
	
	-- 银币是否满足
	if recvValue.m_silver > 0 then
		uiObj = addChild( m_uiCondition, m_uiUIP_UpgradeCond );
		flag = true;
		if GetPlayer().m_silver < recvValue.m_silver then
			flag = false;
		end
		BuildingUpgradeCondSet( uiObj, T(121).." "..knum(recvValue.m_silver), flag, 4 );
	end
	
	-- 木材是否满足
	if recvValue.m_wood > 0 then
		uiObj = addChild( m_uiCondition, m_uiUIP_UpgradeCond );
		flag = true;
		if GetPlayer().m_wood < recvValue.m_wood then
			flag = false;
		end
		BuildingUpgradeCondSet( uiObj, T(122).." "..knum(recvValue.m_wood), flag, 5 );
	end
	
	-- 粮草是否满足
	if recvValue.m_food > 0 then
		uiObj = addChild( m_uiCondition, m_uiUIP_UpgradeCond );
		flag = true;
		if GetPlayer().m_food < recvValue.m_food then
			flag = false;
		end
		BuildingUpgradeCondSet( uiObj, T(123).." "..knum(recvValue.m_food), flag, 6 );
	end
	
	-- 镔铁是否满足
	if recvValue.m_iron > 0 then
		uiObj = addChild( m_uiCondition, m_uiUIP_UpgradeCond );
		flag = true;
		if GetPlayer().m_iron < recvValue.m_iron then
			flag = false;
		end
		BuildingUpgradeCondSet( uiObj, T(124).." "..knum(recvValue.m_iron), flag, 7 );
	end
end

function BuildingUpgradeCondSet( uiObj, text, flag, type )
	local color = Color.white;
	local flagname = "ui_cond_yes";
	if flag == false then
		color = Color.red;
		flagname = "ui_cond_no";
	end
	
	-- 条件描述
	local uiObjText = uiObj.transform:Find("Text");
	SetText( uiObjText, text, color );
	SetTrue( uiObjText );
	
	-- 对号错号
	local uiFlag = uiObj.transform:Find("Flag");
	if type == 2 or type == 3 then
		SetImage( uiFlag, flagname );
		SetTrue( uiFlag );
	else
		SetFalse( uiFlag );
	end
	
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
				SetText( uiObjText.transform:Find("Back/Text"), T(608) ); -- 获取
			end
			SetControlID( uiButton, type )
			SetTrue( uiButton );
		end
	else
		SetFalse( uiButton );
	end
end

