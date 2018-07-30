-- 界面
local m_Dlg = nil;
local m_uiPlaceName = nil; --UnityEngine.GameObject
local m_uiNextPlaceName = nil; --UnityEngine.GameObject
local m_uiPlaceDesc = nil; --UnityEngine.GameObject
local m_uiPlaceAttr = nil; --UnityEngine.GameObject
local m_uiCondGroup = nil; --UnityEngine.GameObject
local m_uiUpgradeBtn = nil; --UnityEngine.GameObject

-- 打开界面
function NationPlaceDlgOpen()
	m_Dlg = eye.uiManager:Open( "NationPlaceDlg" );
end

-- 隐藏界面
function NationPlaceDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "NationPlaceDlg" );
end

-- 删除界面
function NationPlaceDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function NationPlaceDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            NationPlaceDlgClose();
		elseif nControlID == 1 then
			NationPlaceDlgUpgrade()
		elseif nControlID >= 100 and nControlID < 200 then
			NationPlaceDlgGoto( nControlID-100 )
        end
	end
end

-- 载入时调用
function NationPlaceDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiPlaceName = objs[0];
	m_uiNextPlaceName = objs[1];
	m_uiPlaceDesc = objs[2];
	m_uiPlaceAttr = objs[3];
	m_uiCondGroup = objs[4];
	m_uiUpgradeBtn = objs[5];
end

-- 界面初始化时调用
function NationPlaceDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function NationPlaceDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function NationPlaceDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function NationPlaceDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function NationPlaceDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function NationPlaceDlgShow()
	NationPlaceDlgOpen()
	NationPlaceDlgRecv()
end

function NationPlaceDlgRecv()
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	local place = GetPlayer().m_place
	local nextplace = GetPlayer().m_place+1
	SetRichText( m_uiPlaceName, F(1773, PlaceRichText( place ), PlaceName( place )) )
	-- 满级
	if place >= global.nation_place_max then
		SetRichText( m_uiNextPlaceName, F(1772, T(1757), "") )
		SetText( m_uiPlaceDesc, PlaceDesc(place) )
		SetRichText( m_uiPlaceAttr, F( 1776, g_nation_place[place].value ) )
		SetFalse( m_uiUpgradeBtn )
		SetFalse( m_uiCondGroup )
		return
	else
		SetRichText( m_uiNextPlaceName, F(1772, PlaceRichText( nextplace ), PlaceName( nextplace )) )
		SetText( m_uiPlaceDesc, PlaceDesc(nextplace) )
		SetRichText( m_uiPlaceAttr,  F( 1776, g_nation_place[place].value ).."<icon=arrow>"..g_nation_place[nextplace].value )
		SetTrue( m_uiUpgradeBtn )
		SetTrue( m_uiCondGroup )
	end
	
	-- 消耗
	local index = 0;
	local info = g_nation_place[nextplace];
	
	-- 威望
	if GetPlayer().m_prestige < info.prestige then
		NationPlaceDlgSetCond( index, LoadSprite("ui_opration_prestige"), T(152).." <color=#03de27ff>"..knum(info.prestige).."</color><color=#e80017ff>/"..knum(GetPlayer().m_prestige).."</color>", 0 )
	else
		NationPlaceDlgSetCond( index, LoadSprite("ui_opration_prestige"), T(152).." <color=#03de27ff>"..knum(info.prestige).."</color><color=#03de27ff>/"..knum(GetPlayer().m_prestige).."</color>", 1 )
	end
	index = index+1
	
	-- 银币
	if GetPlayer().m_silver < info.silver then
		NationPlaceDlgSetCond( index, LoadSprite("ui_opration_silver"), T(121).." <color=#03de27ff>"..knum(info.silver).."</color><color=#e80017ff>/"..knum(GetPlayer().m_silver).."</color>", 0 )
	else
		NationPlaceDlgSetCond( index, LoadSprite("ui_opration_silver"), T(121).." <color=#03de27ff>"..knum(info.silver).."</color><color=#03de27ff>/"..knum(GetPlayer().m_silver).."</color>", 1 )
	end
	index = index+1
	
	-- 资源
	local costkind = { info.cost_kind0, info.cost_kind1, info.cost_kind2, info.cost_kind3, }
	local costnum = { info.cost_num0, info.cost_num1, info.cost_num2, info.cost_num3, }
	for i=1, 4, 1 do
		if costkind[i] > 0 then
			local itemnum = GetItem():GetCount( costkind[i] )
			if itemnum < costnum[i] then
				NationPlaceDlgSetCond( index, LoadSprite("ui_opration_item_"..costkind[i]), item_getname(costkind[i]).." <color=#03de27ff>"..costnum[i].."</color><color=#e80017ff>/"..itemnum.."</color>", 0 )
			else
				NationPlaceDlgSetCond( index, LoadSprite("ui_opration_item_"..costkind[i]), item_getname(costkind[i]).." <color=#03de27ff>"..costnum[i].."</color><color=#03de27ff>/"..itemnum.."</color>", 1 )
			end
			index = index+1
		end
	end
	
	for i=0, 5 do
		if i < index then
			SetTrue( m_uiCondGroup.transform:GetChild( i ) )
		else
			SetFalse( m_uiCondGroup.transform:GetChild( i ) )
		end
	end
end

-- 设置条件
function NationPlaceDlgSetCond( index, shape, name, has )
	local uiObj = m_uiCondGroup.transform:GetChild( index );
	SetImage( uiObj.transform:Find("Shape"), shape )
	SetText( uiObj.transform:Find("Name"), name )
	if has == 1 then
		SetTrue( uiObj.transform:Find("Yes") )
		SetFalse( uiObj.transform:Find("No") )
		SetFalse( uiObj.transform:Find("GetBtn") )
	else
		SetFalse( uiObj.transform:Find("Yes") )
		SetTrue( uiObj.transform:Find("No") )
		SetTrue( uiObj.transform:Find("GetBtn") )
		SetControlID( uiObj.transform:Find("GetBtn"), 100+index )
	end
end

-- 前往获取
function NationPlaceDlgGoto( index )
	local nextplace = GetPlayer().m_place+1
	local info = g_nation_place[nextplace];
	if index == 0 then
		ShopDlgShow()
	elseif index == 1 then
		MaterialGetDlgShow( 120 )
	else
		local i = index - 1
		local costkind = { info.cost_kind0, info.cost_kind1, info.cost_kind2, info.cost_kind3, }
		MaterialGetDlgShow( costkind[i] )
	end
end

-- 升级爵位
function NationPlaceDlgUpgrade()
	local nextplace = GetPlayer().m_place+1
	local info = g_nation_place[nextplace];
	if GetPlayer().m_prestige < info.prestige then
		pop( T(1777)..":"..T(152).."x"..info.prestige )
		return
	end
	if GetPlayer().m_silver < info.silver then
		pop( T(1777)..":"..T(121).."x"..info.silver )
		return
	end
	
	-- 资源
	local costkind = { info.cost_kind0, info.cost_kind1, info.cost_kind2, info.cost_kind3, }
	local costnum = { info.cost_num0, info.cost_num1, info.cost_num2, info.cost_num3, }
	for i=1, 4, 1 do
		if costkind[i] > 0 then
			local itemnum = GetItem():GetCount( costkind[i] )
			if itemnum < costnum[i] then
				pop( T(1777)..":"..item_getname(costkind[i]).."x"..costnum[i] )
				return
			end
		end
	end
	system_askinfo( ASKINFO_NATION, "", 2 )
end
