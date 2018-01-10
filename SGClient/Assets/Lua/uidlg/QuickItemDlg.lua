-- 界面
local m_Dlg = nil;

local m_uiShape = nil; --UnityEngine.GameObject
local m_uiName = nil; --UnityEngine.GameObject
local m_uiTargetName = nil; --UnityEngine.GameObject
local m_uiTimer = nil; --UnityEngine.GameObject
local m_uiUIP_QuickItem = {nil,nil,nil,nil}; --UnityEngine.GameObject

local m_buildingkind = 0;
local m_buildingoffset = 0;
local m_op = 0;

local m_itemlist = { 142, 134, 135, 136 }

-- 打开界面
function QuickItemDlgOpen()
	m_Dlg = eye.uiManager:Open( "QuickItemDlg" );
end

-- 隐藏界面
function QuickItemDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "QuickItemDlg" );
end

-- 删除界面
function QuickItemDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function QuickItemDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            QuickItemDlgClose();
	
		elseif nControlID >= 1 and nControlID <= 4 then
			QuickItemDlgSelect( nControlID )
        end
	end
end

-- 载入时调用
function QuickItemDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;
	m_uiShape = objs[0];
	m_uiName = objs[1];
	m_uiTargetName = objs[2];
	m_uiTimer = objs[3];
	m_uiUIP_QuickItem[1] = objs[4];
	m_uiUIP_QuickItem[2] = objs[5];
	m_uiUIP_QuickItem[3] = objs[6];
	m_uiUIP_QuickItem[4] = objs[7];
end

-- 界面初始化时调用
function QuickItemDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function QuickItemDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function QuickItemDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function QuickItemDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function QuickItemDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function QuickItemDlgShow( op, buildingkind, buildingoffset, sec )
	QuickItemDlgOpen()
	m_buildingkind = buildingkind;
	m_buildingoffset = buildingoffset;
	m_op = op;
	QuickItemDlgSet( sec )
end

function QuickItemDlgSet( sec, update )
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	local pBuilding = GetPlayer():GetBuilding( m_buildingkind, m_buildingoffset );
	if pBuilding == nil then
		return
	end
	
	-- 刷新模式
	if update ~= nil and m_uiTimer ~= nil and update == 1 then
		local m_uiTimerCountdown = m_uiTimer.transform:GetComponent( typeof(UITextTimeCountdown) );
		m_uiTimerCountdown.PassTime = m_uiTimerCountdown.PassTime + sec
		sec = m_uiTimerCountdown.LeftTime
		if sec <= 0 then
			QuickItemDlgClose();
			TrainDlgClose();
			return;
		end
	end
	
	-- 升级加速
	if m_op == 1 then
		SetImage( m_uiShape, BuildingSprite(m_buildingkind) );
		SetText( m_uiName, BuildingNameLv( m_buildingkind, m_buildingoffset, pBuilding.m_level ).."->".. BuildingNameLv( m_buildingkind, m_buildingoffset, pBuilding.m_level+1 ) )
		SetTimer( m_uiTimer, sec, sec, 0, T(702) )
		
	-- 募兵加速
	elseif m_op == 2 then
		SetImage( m_uiShape, CorpsFaceSpriteEx(m_buildingkind) );
		SetText( m_uiName, T(605)..CorpsNameEx(m_buildingkind) );
		SetTimer( m_uiTimer, sec, sec, 0, T(702) )
		
	-- 改建加速
	elseif m_op == 3 then			
		SetImage( m_uiShape, BuildingSprite(m_buildingkind) );
		SetText( m_uiName, BuildingNameLv( m_buildingkind, nil, pBuilding.m_level )..T(1474) )
		SetTimer( m_uiTimer, sec, sec, 0, T(702) )
	end
	
	for i=1, 4, 1 do
		local kind = m_itemlist[i]
		SetImage( m_uiUIP_QuickItem[i].transform:Find("Back/Shape"), ItemSprite(kind) );
		SetImage( m_uiUIP_QuickItem[i].transform:Find("Back/Color"), ItemColorSprite(item_getcolor(kind)) );
		SetText( m_uiUIP_QuickItem[i].transform:Find("Name"), item_getname(kind) );
		if kind == 142 then
			SetText( m_uiUIP_QuickItem[i].transform:Find("Back/Num"), T(125)..QuickItemDlgTokenCalc() );
		else
			SetText( m_uiUIP_QuickItem[i].transform:Find("Back/Num"), "x"..GetItem():GetCount(kind) );
		end
	end
end

-- 计算钻石加速
function QuickItemDlgTokenCalc()
	if m_uiTimer == nil then
		return 0;
	end
	local left = m_uiTimer.transform:GetComponent( typeof(UITextTimeCountdown) ).LeftTime
	local min = math.floor(left/60) + 1
	local token = 0
	if m_op == 1 or m_op == 3 then
		token = math.ceil( min*global.upgradequick_token)
	elseif m_op == 2 then
		token = math.ceil( min*global.upgradequick_token)
	end
	return token
end

function QuickItemDlgSelect( index )
	local kind = m_itemlist[index]
	if kind == 142 then
		local token = QuickItemDlgTokenCalc();
		if token <= 0 then
			QuickItemDlgClose();
			return;
		end
		if GetPlayer().m_token < token then
			-- 跳转
			JumpToken();
			return
		end
	else
		if GetItem():GetCount(kind) <= 0 then
			return
		end
	end
	system_askinfo( ASKINFO_QUICK, "", 0, kind, m_op, m_buildingkind, m_buildingoffset );
end
