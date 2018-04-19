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
			GirlShopDlgOnBtnRefresh();
		elseif nControlID >= 10 and nControlID <= 20 then				
			GirlShopDlgOnBtnBuy( nControlID - 10 )
        end
	elseif nType == UI_EVENT_TIMECOUNTEND then
		if nControlID == 1 then
			system_askinfo( ASKINFO_GIRLSHOP, "", 0 ); 
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
		if data.m_awardkind ~= 0 then
			GirlShopDlgCreatGrid(i,data);
		end
	end
	GirlShopDlgCDTitle(GetServerTime() + m_InfoRecv.m_update_lefttime)-- 界面
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
	local uiSellOut = objs[6];	
	
	local sprite, color, name, c, desc = AwardInfo( data.m_awardkind );		
	SetImage(uiShape,sprite);
	SetImage(uiColor,color);
	SetText(uiName,F(4203,name,data.m_awardnum));
	
	local Csprite, Ccolor, Cname, Cc, Cdesc = AwardInfo( data.m_cost_awardkind );	
	SetImage(uiCostBack,Csprite);
	SetText(uiCostNum,"x"..data.m_cost_awardnum);
	
	if data.m_cost_awardkind > AWARDKIND_GIRLSOULBASE then
		if GirlShopDlgCheckCost( data ) == true then 
			SetTextColor(uiCostNum,Hex2Color(0xFFFFFFFF));
		else
			SetTextColor(uiCostNum,Hex2Color(0xE80017FF));
		end
	end
	
	-- 是否售罄 
	if data.m_isbuy == 1 then
		SetTrue(uiSellOut);
	else
		SetFalse(uiSellOut);
	end
	
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
	if GirlShopDlgCheckRefresh() == true then 
		system_askinfo( ASKINFO_GIRLSHOP, "", 2 ); 
	end
end

-- 刷新判断
function GirlShopDlgCheckRefresh()
	-- 判断次数
	if m_InfoRecv.m_update_num < m_InfoRecv.m_update_nummax then 
		-- 判断vip
		if m_InfoRecv.m_update_viplevel <= GetPlayer().m_viplevel then 
			-- 判断元宝
			if m_InfoRecv.m_update_token <= GetPlayer().m_token then 
				return true;
			else
				JumpToken();
				return false;
			end
		else
			pop(F(4201,m_InfoRecv.m_update_viplevel));
			return false;
		end		
	else
		pop(T(3414));
		return false;
	end	
end

-- 购买
function GirlShopDlgOnBtnBuy( index )
	local info = m_InfoRecv.m_list[index];
	if info == nil then
		return;
	end
	if info.m_isbuy == 1 then
		return;
	end
	
	local Csprite, Ccolor, Cname, Cc, Cdesc = AwardInfo( info.m_cost_awardkind );	
	local sprite, color, name, c, desc = AwardInfo( info.m_awardkind );
	local costName = nil;
	local buyName = F(4203,name,info.m_awardnum);	
	--弹窗提示
	if info.m_cost_awardkind > AWARDKIND_GIRLSOULBASE then 	
		costName = F(4203,Cname,info.m_cost_awardnum);
	else
		costName = F(4204,info.m_cost_awardnum);
	end
	MsgBox( F(4205, costName,buyName ), function()
			GirlShopDlgCheckBuy(info,index)
		end)
end

function GirlShopDlgCheckBuy(info,index)
	if GirlShopDlgCheckCost( info ) == true then 	
		warn("购买第"..index.."个")
		system_askinfo( ASKINFO_GIRLSHOP, "", 1, index, info.m_id ); 				
	else
		warn("道具或元宝不足")
		if info.m_cost_awardkind > AWARDKIND_GIRLSOULBASE then
			pop(T(4202));
		else
			JumpToken();
		
		end
	end
end
--判断消耗
function GirlShopDlgCheckCost( info )
	local CostKind = info.m_cost_awardkind;
	local CostNum = info.m_cost_awardnum;
	if CostKind > AWARDKIND_GIRLSOULBASE then 			--女将碎片
		local HaveNum = GetGirl():GetSoul(CostKind-71000);
		if CostNum <= HaveNum then 
			return true;
		else			
			return false;
		end
	else												--元宝
		if CostNum <= GetPlayer().m_token then 
			return true;
		else			
			return false;
		end
	end
	return false
end