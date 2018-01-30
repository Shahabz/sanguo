-- 界面
local m_Dlg = nil;
local m_DialogFrameMod = nil;
local m_uiLButton = nil; --UnityEngine.GameObject
local m_uiRButton = nil; --UnityEngine.GameObject
local m_uiVipProgress = nil; --UnityEngine.GameObject
local m_uiVipLevel = nil; --UnityEngine.GameObject
local m_uiVipNext = nil; --UnityEngine.GameObject
local m_uiVipTitle = nil; --UnityEngine.GameObject
local m_uiBagTitle = nil; --UnityEngine.GameObject
local m_uiAttrGrid = nil; --UnityEngine.GameObject
local m_uiUIP_Attr = nil; --UnityEngine.GameObject
local m_uiBagList = nil; --UnityEngine.GameObject
local m_uiUIP_Award = nil; --UnityEngine.GameObject

local m_curVipLevel = 0;

-- 打开界面
function VipDlgOpen()
	m_Dlg = eye.uiManager:Open( "VipDlg" );
	m_DialogFrameMod = DialogFrameModOpen( m_Dlg, T(570), HELP_VipDlg, VipDlgClose );
end

-- 隐藏界面
function VipDlgClose()
	if m_Dlg == nil then
		return;
	end
	DialogFrameModClose( m_DialogFrameMod );
	m_DialogFrameMod = nil;
	eye.uiManager:Close( "VipDlg" );
end

-- 删除界面
function VipDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function VipDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            VipDlgClose();
			
		elseif nControlID == -2 then
			VipDlgHideDesc()
			
		-- 充值	
		elseif nControlID == 1 then
			PayDlgShow()
		
		-- 上一页
		elseif nControlID == 2 then	
			VipDlgPre()
			
		-- 下一页
		elseif nControlID == 3 then
			VipDlgNext()
		
		-- 前往商店
		elseif nControlID == 4 then
			ShopDlgShowByType( 2 )
			
		-- 点击奖励
		elseif nControlID > 10000 then
			VipDlgClickAward( nControlID-10000, value )
        end
	end
end

-- 载入时调用
function VipDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiLButton = objs[0];
	m_uiRButton = objs[1];
	m_uiVipProgress = objs[2];
	m_uiVipLevel = objs[3];
	m_uiVipNext = objs[4];
	m_uiVipTitle = objs[5];
	m_uiBagTitle = objs[6];
	m_uiAttrGrid = objs[7];
	m_uiUIP_Attr = objs[8];
	m_uiBagList = objs[9];
	m_uiUIP_Award = objs[10];
end

-- 界面初始化时调用
function VipDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function VipDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function VipDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function VipDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function VipDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function VipDlgShow()
	VipDlgOpen()
	VipDlgUpdateVip()
	m_curVipLevel = GetPlayer().m_viplevel
	VipDlgSet( GetPlayer().m_viplevel )
end

-- 设置信息
function VipDlgSet( level )
	m_curVipLevel = level
	if level == 0 then
		SetFalse( m_uiLButton )
		SetTrue( m_uiRButton )
	elseif level == 14 then
		SetTrue( m_uiLButton )
		SetFalse( m_uiRButton )
	else
		SetTrue( m_uiLButton )
		SetTrue( m_uiRButton )
	end
	
	SetText( m_uiVipTitle.transform:Find("Text"), F( 2206, level ) )
	SetText( m_uiBagTitle.transform:Find("Text"), F( 2207, level ) )
	
	-- 提供权限
	local offset = 0;
	if g_vip[level].buildfree > 0 then
		VipDlgSetAttrObj( offset, "item_icon_471", 2211, 2231, g_vip[level].buildfree )
		offset = offset + 1;
	end
	if g_vip[level].bodybuy > 0 then
		VipDlgSetAttrObj( offset, "item_icon_126", 2212, 2232, g_vip[level].bodybuy )
		offset = offset + 1;
	end
	if g_vip[level].autobuild > 0 then
		VipDlgSetAttrObj( offset, "item_icon_470", 2213, 2233, g_vip[level].autobuild )
		offset = offset + 1;
	end
	if g_vip[level].skipfight > 0 then
		VipDlgSetAttrObj( offset, "item_icon_173", 2214, 2234, g_vip[level].skipfight )
		offset = offset + 1;
	end
	if g_vip[level].marchcall > 0 then
		VipDlgSetAttrObj( offset, "item_icon_139", 2215, 2235, g_vip[level].marchcall )
		offset = offset + 1;
	end
	if g_vip[level].train > 0 then
		VipDlgSetAttrObj( offset, "item_icon_474", 2216, 2236, g_vip[level].train )
		offset = offset + 1;
	end
	if g_vip[level].palace > 0 then
		VipDlgSetAttrObj( offset, "item_icon_125", 2217, 2237, g_vip[level].palace )
		offset = offset + 1;
	end
	if g_vip[level].storysweep > 0 then
		VipDlgSetAttrObj( offset, "item_icon_138", 2218, 2238, g_vip[level].storysweep )
		offset = offset + 1;
	end
	for i = offset, 5 do
		SetFalse( m_uiAttrGrid.transform:GetChild( i ) )
	end
	
	-- 礼包
	local p = g_vipbag[level]
	local awardkind = { p.awardkind0,p.awardkind1,p.awardkind2,p.awardkind3,p.awardkind4,p.awardkind5 }
	local awardnum = { p.awardnum0,p.awardnum1,p.awardnum2,p.awardnum3,p.awardnum4,p.awardnum5 }
	for i=0, 4, 1 do
		local awardObj = m_uiBagList.transform:GetChild(i);
		if awardkind[i+1] > 0 then
			SetControlID( awardObj, 10000+awardkind[i+1] );
			local sprite, color, name, c, desc = AwardInfo( awardkind[i+1] )
			SetTrue( awardObj )
			SetImage( awardObj.transform:Find("Shape"), sprite );
			SetImage( awardObj.transform:Find("Color"), color );
			if awardnum[i+1] > 1 then
				SetText( awardObj.transform:Find("Num"), awardnum[i+1] );
			else
				SetText( awardObj.transform:Find("Num"), "" );
			end
			if awardkind[i+1] > AWARDKIND_HEROBASE and awardkind[i+1] < AWARDKIND_BUILDINGBASE then
				SetTrue( awardObj.transform:Find("Corps") )
				
				local herokind = awardkind[i+1] - AWARDKIND_HEROBASE
				local color = hero_getnormalcolor( herokind )
				SetImage( awardObj.transform:Find("Corps"), CorpsSprite( g_heroinfo[herokind][color].corps ) )
			else
				SetFalse( awardObj.transform:Find("Corps") )
			end
		else
			SetFalse( awardObj )
		end
		SetFalse( awardObj.transform:Find("Desc") )
	end
end

-- 设置一个属性对象
function VipDlgSetAttrObj( offset, icon, nameid, descid, value )
	local uiObj = m_uiAttrGrid.transform:GetChild( offset )
	SetTrue( uiObj )
	SetImage( uiObj.transform:Find("Shape"), LoadSprite( icon ) )
	SetText( uiObj.transform:Find("Name"), T( nameid ) )
	SetText( uiObj.transform:Find("Desc"), F( descid, value ) )
end

-- 上一页
function VipDlgPre()
	if m_curVipLevel > 0 then
		m_curVipLevel = m_curVipLevel - 1
	end
	VipDlgSet( m_curVipLevel )
end

-- 下一页
function VipDlgNext()
	if m_curVipLevel < 14 then
		m_curVipLevel = m_curVipLevel + 1
	end
	VipDlgSet( m_curVipLevel )
end

-- 更新VIP
function VipDlgUpdateVip()
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	local viplevel = GetPlayer().m_viplevel
	SetText( m_uiVipLevel, "vip"..viplevel )
	if viplevel >= 14 then
		-- 已经满级
		SetFalse( m_uiVipNext )
		SetProgress( m_uiVipProgress, 1 );
		SetText( m_uiVipProgress.transform:Find("Text"), T(2098) );
	else
		local needexp = g_vip[viplevel].exp
		SetTrue( m_uiVipNext )
		SetText( m_uiVipNext, F(2096, needexp-GetPlayer().m_vipexp, viplevel+1 ) )
		SetProgress( m_uiVipProgress, GetPlayer().m_vipexp/needexp );
		SetText( m_uiVipProgress.transform:Find("Text"), GetPlayer().m_vipexp.."/"..needexp );
	end
end

-- 隐藏描述
function VipDlgHideDesc()
	for i=0, 4, 1 do
		local awardObj = m_uiBagList.transform:GetChild(i);
		SetFalse( awardObj.transform:Find("Desc") )
	end
end

-- 点击奖励
function VipDlgClickAward( awardkind, uiObj )
	VipDlgHideDesc()
	if awardkind > AWARDKIND_HEROBASE and awardkind < AWARDKIND_BUILDINGBASE then
		local herokind = awardkind - AWARDKIND_HEROBASE
		local color = hero_getnormalcolor( herokind )
		HeroConfigDlgShow( g_heroinfo[herokind][color], 0 );
	else
		local _, _, name, c, desc = AwardInfo( awardkind )
		local uiDesc = uiObj.transform:Find("Desc")
		SetTrue( uiDesc )
		SetText( uiDesc.transform:Find("Name"), name, NameColor(c) )	
		local _desc = string.split( desc, "\n")
		if _desc ~= nil and _desc[1] ~= nil then
			SetText( uiDesc.transform:Find("Desc"), _desc[1] )
		else
			SetText( uiDesc.transform:Find("Desc"), desc )
		end
		SetText( uiDesc.transform:Find("Warn"), "" )
	end
end
