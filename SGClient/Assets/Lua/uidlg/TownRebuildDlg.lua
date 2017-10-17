-- 界面
local m_Dlg = nil;
local m_uiTitle = nil; --UnityEngine.GameObject
local m_uiPic = nil; --UnityEngine.GameObject
local m_uiDesc1 = nil; --UnityEngine.GameObject
local m_uiDesc2 = nil; --UnityEngine.GameObject
local m_uiCost = nil; --UnityEngine.GameObject
local m_uiList = nil; --UnityEngine.GameObject
local m_uiGrid = nil; --UnityEngine.GameObject
local m_uiButtons = nil; --UnityEngine.GameObject
local m_uiAskTimer = nil; --UnityEngine.GameObject

local m_townid = 0;
local m_towninfo = nil;

-- 打开界面
function TownRebuildDlgOpen()
	m_Dlg = eye.uiManager:Open( "TownRebuildDlg" );
end

-- 隐藏界面
function TownRebuildDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "TownRebuildDlg" );
end

-- 删除界面
function TownRebuildDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function TownRebuildDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            TownRebuildDlgClose();
		elseif nControlID == 1 then
			TownRebuildDlgAsk();
        end
	
	elseif nType == UI_EVENT_TIMECOUNTEND then
		TownRebuildDlgClose()
	end
end

-- 载入时调用
function TownRebuildDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiTitle = objs[0];
	m_uiPic = objs[1];
	m_uiDesc1 = objs[2];
	m_uiDesc2 = objs[3];
	m_uiCost = objs[4];
	m_uiList = objs[5];
	m_uiGrid = objs[6];
	m_uiButtons = objs[7];
	m_uiAskTimer = objs[8];
end

-- 界面初始化时调用
function TownRebuildDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function TownRebuildDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function TownRebuildDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function TownRebuildDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function TownRebuildDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function TownRebuildDlgShow( townid, towninfo )
	if towninfo == nil then
		return
	end
	m_townid = townid;
	m_towninfo = towninfo;
	TownRebuildDlgOpen()
	MapTownDlgClose()
	SetFalse( m_uiDesc2 )
	SetFalse( m_uiList )
	SetFalse( m_uiButtons )
	SetFalse( m_uiAskTimer )
	
	if m_towninfo.m_protect_sec > 0 then
		if m_towninfo.m_myask == 1 then
			-- 我在申请列表里，那么我要请求申请列表
			system_askinfo( ASKINFO_MAPTOWN, "", 1, m_townid )
		else
			SetTrue( m_uiDesc2 )
			SetTrue( m_uiButtons )
		end
	else
		SetTrue( m_uiDesc2 )
		SetTrue( m_uiButtons )
	end
	
	SetText( m_uiDesc1.transform:Find("Text"), F( 1301, MapTownName( townid ) ) )
	SetText( m_uiDesc2.transform:Find("Text"), F( 1302, global.town_owner_award ) )
	local askcost = "";
	if g_towninfo[townid].ask_silver > 0 then
		askcost = askcost .. "<color=f7f3bbff><icon=silver>"..knum(g_towninfo[townid].ask_silver).."</color>  "
	end
	if g_towninfo[townid].ask_wood > 0 then
		askcost = askcost .. "<color=f7f3bbff><icon=wood>"..knum(g_towninfo[townid].ask_wood).."</color>  "
	end
	if g_towninfo[townid].ask_food > 0 then
		askcost = askcost .. "<color=f7f3bbff><icon=food>"..knum(g_towninfo[townid].ask_food).."</color>  "
	end
	if g_towninfo[townid].ask_iron > 0 then
		askcost = askcost .. "<color=f7f3bbff><icon=iron>"..knum(g_towninfo[townid].ask_iron).."</color>  "
	end
	SetRichText( m_uiCost, T(1304)..askcost )
end

-- 申请城主
function TownRebuildDlgAsk()
	if m_townid <= 0 then
		return
	end
	local townid = m_townid;
	
	if m_towninfo.m_own_actorid > 0 then
		return
	end
	
	local askcost = "";
	if g_towninfo[townid].ask_silver > 0 then
		askcost = askcost .. T(121) ..knum(g_towninfo[townid].ask_silver).." "
	end
	if g_towninfo[townid].ask_wood > 0 then
		askcost = askcost .. T(122) ..knum(g_towninfo[townid].ask_wood).." "
	end
	if g_towninfo[townid].ask_food > 0 then
		askcost = askcost .. T(123) ..knum(g_towninfo[townid].ask_food).." "
	end
	if g_towninfo[townid].ask_iron > 0 then
		askcost = askcost .. T(124) ..knum(g_towninfo[townid].ask_iron).." "
	end
	
	local msg = ""
	if m_towninfo.m_protect_sec > 0 then
		msg = F( 1306, MapTownName(townid), askcost )
	else
		msg = F( 1305, MapTownName(townid), askcost )
	end
	
	MsgBox( msg, function()
		if g_towninfo[townid].ask_silver > 0 then
			if GetPlayer().m_silver < g_towninfo[townid].ask_silver then
				JumpRes(1)
				return
			end
		end
		if g_towninfo[townid].ask_wood > 0 then
			if GetPlayer().m_wood < g_towninfo[townid].ask_wood then
				JumpRes(2)
				return
			end
		end
		if g_towninfo[townid].ask_food > 0 then
			if GetPlayer().m_food < g_towninfo[townid].ask_food then
				JumpRes(3)
				return
			end
		end
		if g_towninfo[townid].ask_iron > 0 then
			if GetPlayer().m_iron < g_towninfo[townid].ask_iron then
				JumpRes(4)
				return
			end
		end
		system_askinfo( ASKINFO_MAPTOWN, "", 0, m_townid )
		TownRebuildDlgClose()
	end)

end

-- 申请城主的列表
-- m_count=0,m_list={m_name_len=0,m_name="[m_name_len]",m_place=0,[m_count]},m_sec=0,
function TownRebuildDlgAskList( recvValue )
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		TownRebuildDlgShow( m_townid, m_towninfo )
	end
	SetFalse( m_uiDesc2 )
	SetFalse( m_uiButtons )
	SetTrue( m_uiList )
	SetTrue( m_uiAskTimer )
	SetTimer( m_uiAskTimer, recvValue.m_sec, recvValue.m_sec, 1, T(1310) );
	
	for tmpi=1, 5, 1 do
		local uiActor = m_uiGrid.transform:GetChild(tmpi-1).gameObject;
		if tmpi <= recvValue.m_count then
			SetText( uiActor.transform:Find("Name"), recvValue.m_list[tmpi].m_name )
			SetText( uiActor.transform:Find("Place"), PlaceName( recvValue.m_list[tmpi].m_place ) )
		else
			SetText( uiActor.transform:Find("Name"), "" )
			SetText( uiActor.transform:Find("Place"), "" )
		end
	end
end
