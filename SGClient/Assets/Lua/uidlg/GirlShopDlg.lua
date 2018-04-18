local m_Dlg = nil;
local m_DialogFrameMod = nil;

local m_uiCDTimer = nil; --UnityEngine.GameObject
local m_uiRefreshBtn = nil; --UnityEngine.GameObject
local m_uiItemContent = nil; --UnityEngine.GameObject
local m_uiUIP_ItemGrid = nil; --UnityEngine.GameObject
local m_uiCDTitle = nil; --UnityEngine.GameObject

local m_InfoRecv = nil;
-- 打开界面
function GirlShopDlgOpen()
	m_Dlg = eye.uiManager:Open( "GirlShopDlg" );
	m_DialogFrameMod = DialogFrameModOpen( m_Dlg, BuildingName(38), HELP_BagDlg, GirlShopDlgClose );
end

-- 隐藏界面
function GirlShopDlgClose()
	if m_Dlg == nil then
		return;
	end
	DialogFrameModClose( m_DialogFrameMod );
	m_DialogFrameMod = nil;
	m_InfoRecv = nil;
	eye.uiManager:Close( "GirlShopDlg" );
end

-- 删除界面
function GirlShopDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
	m_InfoRecv = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function GirlShopDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            GirlShopDlgClose();
		elseif nControlID == 1 then
			warn("刷新道具")
			GirlShopDlgOnBtnRefresh();
		elseif nControlID >= 10 and nControlID <= 20 then	
			warn("购买第"..(nControlID - 10).."个")
			GirlShopDlgOnBtnBuy( nControlID - 10 )
        end
	elseif nType == UI_EVENT_TIMECOUNTEND then
		if nControlID == 1 then
			SetFalse(m_uiCDTitle);
		end
	end
end

-- 载入时调用
function GirlShopDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiCDTimer = objs[0];
	m_uiRefreshBtn = objs[1];
	m_uiItemContent = objs[2];
	m_uiUIP_ItemGrid = objs[3];
	m_uiCDTitle = objs[4];
	
	-- 对象池
	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );	
	--巡游奖励Grid
	m_ObjectPool:CreatePool("UIP_ItemGrid", 9, 9, m_uiUIP_ItemGrid);
	
end

-- 界面初始化时调用
function GirlShopDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function GirlShopDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function GirlShopDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function GirlShopDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function GirlShopDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function GirlShopDlgShow()
	GirlShopDlgOpen()
	system_askinfo( ASKINFO_GIRLSHOP, "", 0 ); 
end

-- 女将商店数据
-- m_count=0,m_list={m_awardkind=0,m_awardnum=0,m_cost_awardkind=0,m_cost_awardnum=0,m_isbuy=0,m_id=0,[m_count]},m_update_lefttime=0,m_update_num=0,m_update_nummax=0,m_update_viplevel=0,m_up
function GirlShopDlgInfoRecv( recvValue )
	m_InfoRecv = recvValue;
	PrintTable(m_InfoRecv,"女将商店数据")
	GirlShopDlgClearGrid();
	for i = 1 , m_InfoRecv.m_count do 
		local data = m_InfoRecv.m_list[i];
		GirlShopDlgCreatGrid(i,data);
	end
	GirlShopDlgCDTitle(GetServerTime()+10)-- 界面
end

--设置标题或时间
function GirlShopDlgCDTitle(EndTime)
	local time = EndTime - GetServerTime();
	if time > 0 then 
		SetTimer( m_uiCDTimer, time , EndTime, 1 );
		SetTrue(m_uiCDTitle);
	else
		SetFalse(m_uiCDTitle);
	end
	-- 女将商店冷却完成通知
	notification_set( "SETTING_PUSH_GirlShop", T(5915), time )
end

-- 创建商店子控件
function GirlShopDlgCreatGrid(index,data)
	local uiObj = m_ObjectPool:Get( "UIP_ItemGrid" );
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiShape= objs[0];	
	local uiColor = objs[1];
	local uiName = objs[2];
	local uiCostBack = objs[3];
	local uiCostNum = objs[4];
	local uiBuyBtn = objs[5];
		
	local sprite, color, name, c, desc = AwardInfo( data.m_awardkind );		
	SetImage(uiShape,sprite)
	SetImage(uiColor,color)
	SetText(uiName,name.."x"..data.m_awardnum)
	
	local Csprite, Ccolor, Cname, Cc, Cdesc = AwardInfo( data.m_cost_awardkind );	
	SetImage(uiCostBack,Csprite)
	SetText(uiCostNum,"x"..data.m_cost_awardnum)
	uiObj.transform:SetParent( m_uiItemContent.transform );
	uiObj.gameObject:SetActive( true );
	SetControlID(uiBuyBtn,index + 10);
end

-- 清空商店子控件
function GirlShopDlgClearGrid()
	local objs = {};
	for i=0,m_uiItemContent.transform.childCount-1 do
		table.insert(objs,m_uiItemContent.transform:GetChild(i).gameObject);
	end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "UIP_ItemGrid(Clone)" then
			m_ObjectPool:Release( "UIP_ItemGrid", obj );
		end
	end
end

-- 刷新
function GirlShopDlgOnBtnRefresh()	
	-- 刷新元宝判断
	--m_InfoRecv.m_update_token
	system_askinfo( ASKINFO_GIRLSHOP, "", 2 ); 
end

-- 购买
function GirlShopDlgOnBtnBuy( index )
	local info = m_InfoRecv.m_list[index]
	if info == nil then
		return
	end
	if info.m_isbuy == 1 then
		return
	end
	system_askinfo( ASKINFO_GIRLSHOP, "", 1, index, info.m_id ); 
end