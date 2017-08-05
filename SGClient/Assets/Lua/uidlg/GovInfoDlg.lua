-- 界面
local m_Dlg = nil;
local m_DialogFrameMod = nil;

local m_uiUIP_Buff = {nil,nil}; --UnityEngine.GameObject
local m_uiUIP_BattleEvent = {nil,nil,nil,nil}; --UnityEngine.GameObject
local m_uiUIP_CityEvent = {nil,nil,nil,nil}; --UnityEngine.GameObject
local m_uiProtectBuy = nil; --UnityEngine.GameObject
local m_selectProtectKind = 0;
-- 打开界面
function GovInfoDlgOpen()
	m_Dlg = eye.uiManager:Open( "GovInfoDlg" );
	m_DialogFrameMod = DialogFrameModOpen( m_Dlg, T(730), 5, GovInfoDlgClose );
end

-- 隐藏界面
function GovInfoDlgClose()
	if m_Dlg == nil then
		return;
	end
	DialogFrameModClose( m_DialogFrameMod );
	m_DialogFrameMod = nil;
	eye.uiManager:Close( "GovInfoDlg" );
end

-- 删除界面
function GovInfoDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function GovInfoDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            GovInfoDlgClose();
			
		elseif nControlID == -2 then
			GovInfoDlgProtectBuySelect( -1 )
			
		-- 购买城池保护
		elseif nControlID == 1 then
			GovInfoDlgProtectBuyShow()
			
		-- 购买商用建造队伍
		elseif nControlID == 2 then
			BuyWorkerDlgShow()
		
		-- 使用	
		elseif nControlID == 10 then
			GovInfoDlgProtectUse()
			
		-- 购买并使用	
		elseif nControlID == 11 then
			GovInfoDlgProtectBuy()
			
		-- 选择
		elseif nControlID > 100 and nControlID < 110 then
			GovInfoDlgProtectBuySelect( nControlID-100 )
        end
	end
end

-- 载入时调用
function GovInfoDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiUIP_Buff[1] = objs[0];
	m_uiUIP_Buff[2] = objs[1];
	m_uiUIP_BattleEvent[1] = objs[2];
	m_uiUIP_BattleEvent[2] = objs[3];
	m_uiUIP_BattleEvent[3] = objs[4];
	m_uiUIP_BattleEvent[4] = objs[5];
	m_uiUIP_CityEvent[1] = objs[6];
	m_uiUIP_CityEvent[2] = objs[7];
	m_uiUIP_CityEvent[3] = objs[8];
	m_uiUIP_CityEvent[4] = objs[9];
	m_uiProtectBuy = objs[10];
end

-- 界面初始化时调用
function GovInfoDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function GovInfoDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function GovInfoDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function GovInfoDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function GovInfoDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function GovInfoDlgShow()
	GovInfoDlgOpen()
	system_askinfo( ASKINFO_GOV, "", 0 );
	GovInfoDlgSetBuffProtect()
	GovInfoDlgSetBuffWorker()
end

-- 城池保护
function GovInfoDlgSetBuffProtect()
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	SetTrue( m_uiUIP_Buff[1] )
	SetText( m_uiUIP_Buff[1].transform:Find("Name"), T(749) );
	if GetPlayer().m_ptsec > 0 then
		SetFalse( m_uiUIP_Buff[1].transform:Find("State") )
		SetTrue( m_uiUIP_Buff[1].transform:Find("Timer") )
		SetTimer( m_uiUIP_Buff[1].transform:Find("Timer"), GetPlayer().m_ptsec, GetPlayer().m_ptsec )
		SetTrue( m_uiUIP_Buff[1].transform:Find("Timer") )
		SetFalse( m_uiUIP_Buff[1].transform:Find("OpenBtn") )
	else
		SetTrue( m_uiUIP_Buff[1].transform:Find("State") )
		SetText( m_uiUIP_Buff[1].transform:Find("State"), T(599) );
		SetFalse( m_uiUIP_Buff[1].transform:Find("Timer") )
		SetTrue( m_uiUIP_Buff[1].transform:Find("OpenBtn") )
	end
end

-- 商用建造队伍
function GovInfoDlgSetBuffWorker()
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	SetTrue( m_uiUIP_Buff[2] )
	SetText( m_uiUIP_Buff[2].transform:Find("Name"), T(750) );
	if GetPlayer().m_worker_expire_ex > 0 then
		SetFalse( m_uiUIP_Buff[2].transform:Find("State") )
		SetTrue( m_uiUIP_Buff[2].transform:Find("Timer") )
		SetTimer( m_uiUIP_Buff[2].transform:Find("Timer"), GetPlayer().m_worker_expire_ex, GetPlayer().m_worker_expire_ex )
		SetFalse( m_uiUIP_Buff[2].transform:Find("OpenBtn") )
	else
		SetTrue( m_uiUIP_Buff[2].transform:Find("State") )
		SetText( m_uiUIP_Buff[2].transform:Find("State"), T(599) );
		SetFalse( m_uiUIP_Buff[2].transform:Find("Timer") )
		SetTrue( m_uiUIP_Buff[2].transform:Find("OpenBtn") )
	end
end

local function type2text_c( type, kind, value )
	if type == 1 then
		return F( 738, BuildingName(kind), value );
	elseif type == 2 then
		return F( 739, CorpsName(kind), value );
	elseif type == 3 then
		return F( 740, EquipName(kind), value );
	elseif type == 4 then
		local level = math.floor(value/100)
		local progress = value%100
		if progress > 0 then
			return F( 748, TechName(kind), level, progress );
		else
			return F( 741, TechName(kind), level );
		end
	end
end

local function type2text_b( type )
	if type == 1 then
		return 742
	elseif type == 2 then
		return 743
	elseif type == 3 then
		return 744
	elseif type == 4 then
		return 745
	elseif type == 5 then
		return 746
	elseif type == 6 then
		return 747
	end
end
-- -- m_cevent_count=0,m_cevent_list={m_type=0,m_kind=0,m_value=0,m_optime=0,[m_cevent_count]},m_bevent_count=0,m_bevent_list={m_type=0,m_name="[22]",m_value=0,m_optime=0,[m_bevent_count]},
function GovInfoDlgRecv( recvValue )
	for i=1, 4, 1 do
		if recvValue.m_cevent_list[i].m_type > 0 then
			SetTrue( m_uiUIP_CityEvent[i] )
			local timeText = os.date( "%m-%d %H:%M", recvValue.m_cevent_list[i].m_optime )
			local text = type2text_c( recvValue.m_cevent_list[i].m_type, recvValue.m_cevent_list[i].m_kind, recvValue.m_cevent_list[i].m_value )
			SetText( m_uiUIP_CityEvent[i].transform:Find("Text"), "["..timeText.."] "..text )
		else
			SetFalse( m_uiUIP_CityEvent[i] )
		end
		
		if recvValue.m_bevent_list[i].m_type > 0 then
			SetTrue( m_uiUIP_BattleEvent[i] )
		else
			SetFalse( m_uiUIP_BattleEvent[i] )
		end
	end	
end

--购买城池保护
local itemlist= { 140, 141 }
function GovInfoDlgProtectBuyShow()
	SetTrue( m_uiProtectBuy );
	local objs = m_uiProtectBuy.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiItem = {};
	uiItem[1] = objs[0];
	uiItem[2] = objs[1];
	local uiDesc = objs[2];
	local uiUseBtn = objs[3];
	local uiBuyBtn = objs[4];
	SetText( uiDesc, "" )
	
	for i=1, #itemlist, 1 do
		local objs = uiItem[i].transform:GetComponent( typeof(Reference) ).relatedGameObject; 
		local uiShape = objs[0];
		local uiNum = objs[1];
		local uiColor = objs[2];
		local uiSelect = objs[3];
			
		SetTrue( uiShape )
		SetTrue( uiNum )
		SetTrue( uiColor )
		SetFalse( uiSelect )
		
		local itemkind = itemlist[i]
		SetImage( uiShape, ItemSprite(itemkind) )
		SetText( uiNum, "x"..GetItem():GetCount( itemkind ) )
		SetImage( uiColor, ItemColorSprite(item_getcolor(itemkind)) )
		SetControlID( uiItem[i], 100+i )
	end
	
end

function GovInfoDlgProtectBuySelect( index )
	if index <= 0 or index > 2 then
		SetFalse( m_uiProtectBuy );
		return;
	end
	m_selectProtectKind = itemlist[index]
	if m_selectProtectKind == nil then
		SetFalse( m_uiProtectBuy );
		return;
	end
	
	local objs = m_uiProtectBuy.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiItem = {};
	uiItem[1] = objs[0];
	uiItem[2] = objs[1];
	local uiDesc = objs[2];
	local uiUseBtn = objs[3];
	local uiBuyBtn = objs[4];
	
	-- 选择状态重置
	for i=1, 2, 1 do
		local objs = uiItem[i].transform:GetComponent( typeof(Reference) ).relatedGameObject; 
		local uiSelect = objs[3];
		SetFalse( uiSelect )
	end
	
	-- 设置选择状态
	local objs = uiItem[index].transform:GetComponent( typeof(Reference) ).relatedGameObject; 
	local uiSelect = objs[3];
	SetTrue( uiSelect )
	
	-- 道具描述
	SetText( uiDesc, item_getname(m_selectProtectKind)..": "..item_getdesc( m_selectProtectKind ) )
	
	-- 有道具显示使用，无道具显示购买并使用
	if GetItem():GetCount( m_selectProtectKind ) > 0 then
		SetFalse( uiBuyBtn )
		SetTrue( uiUseBtn )
	else
		SetFalse( uiUseBtn )
		SetTrue( uiBuyBtn )
	end
end

-- 使用
function GovInfoDlgProtectUse()
	if m_selectProtectKind <= 0 then
		return
	end

	GetItem():UseWithCount( m_selectProtectKind , 1 );
	GovInfoDlgProtectBuySelect( -1 )
end

-- 购买并使用
function GovInfoDlgProtectBuy()
	if m_selectProtectKind <= 0 then
		return
	end
	MsgBox( F(754, item_gettoken(m_selectProtectKind), item_getname(m_selectProtectKind) ), function()
		GetItem():UseWithToken( m_selectProtectKind, 1  )
		GovInfoDlgProtectBuySelect( -1 )
	end )
end
