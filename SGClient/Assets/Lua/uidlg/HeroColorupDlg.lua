-- 界面
local m_Dlg = nil;
local m_uiMyItemNum = nil; --UnityEngine.GameObject
local m_uiColorDesc = nil; --UnityEngine.GameObject
local m_uiWashInfoPre = nil; --UnityEngine.GameObject
local m_uiWashInfoNext = nil; --UnityEngine.GameObject
local m_uiProgress = nil; --UnityEngine.GameObject
local m_uiUpValue = nil; --UnityEngine.GameObject
local m_uiCoatValue = nil; --UnityEngine.GameObject
local m_uiWarn = nil; --UnityEngine.GameObject

local m_pHero = nil;
local m_recvValue = nil;

-- 打开界面
function HeroColorupDlgOpen()
	m_Dlg = eye.uiManager:Open( "HeroColorupDlg" );
end

-- 隐藏界面
function HeroColorupDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "HeroColorupDlg" );
end

-- 删除界面
function HeroColorupDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function HeroColorupDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
			HeroColorupDlgAskClose()
		elseif nControlID == 1 then
			HeroColorupDlgColorup();
        end
	end
end

-- 载入时调用
function HeroColorupDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiMyItemNum = objs[0];
	m_uiColorDesc = objs[1];
	m_uiWashInfoPre = objs[2];
	m_uiWashInfoNext = objs[3];
	m_uiProgress = objs[4];
	m_uiUpValue = objs[5];
	m_uiCoatValue = objs[6];
	m_uiWarn = objs[7];
	SetProgress( m_uiProgress, 0 )
end

-- 界面初始化时调用
function HeroColorupDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function HeroColorupDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function HeroColorupDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function HeroColorupDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function HeroColorupDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function HeroColorupDlgShow( pHero )
	m_pHero = pHero
	local config = g_hero_colorup[pHero.m_color];
	if config == nil then
		return
	end
	HeroColorupDlgOpen()
	system_askinfo( ASKINFO_HERO, "", 7, pHero.m_kind );
	
	-- 将领
	HeroColorupDlgSetItem()
	-- 当前
	HeroColorupDlgSetHero( m_uiWashInfoPre, pHero, pHero.m_color )
	-- 突破后
	HeroColorupDlgSetHero( m_uiWashInfoNext, pHero, config.nextcolor )
	SetText( m_uiWarn, F( 852, ColorName(config.nextcolor) ) )
end

-- 设置道具
function HeroColorupDlgSetItem()
	local config = g_hero_colorup[m_pHero.m_color];
	local itemname = item_getname( config.itemkind )
	m_hasitemnum = GetItem():GetCount( config.itemkind )
	SetText( m_uiMyItemNum, T(848)..itemname.." "..knum(m_hasitemnum) )
	SetText( m_uiColorDesc, F( 853, ColorName(m_pHero.m_color), HeroName(m_pHero.m_kind), ColorName(config.nextcolor) ) )
	if m_hasitemnum >= config.itemnum then
		SetText( m_uiCoatValue, T(850)..itemname..":<color=#03de27ff>"..config.itemnum.."</color><color=#03de27ff>/"..m_hasitemnum.."</color>" )
	else
		SetText( m_uiCoatValue, T(850)..itemname..":<color=#03de27ff>"..config.itemnum.."</color><color=#e80017ff>/"..m_hasitemnum.."</color>" )
	end
end

-- 设置英雄信息
function HeroColorupDlgSetHero( uiObj, pHero, color )
	local config = g_heroinfo[pHero.m_kind][color];
	
	SetImage( uiObj.transform:Find("Shape"), HeroFaceSprite( pHero.m_kind )  );
	SetImage( uiObj.transform:Find("Color"),  ItemColorSprite( color )  );
	SetImage( uiObj.transform:Find("Corps"),  CorpsSprite( pHero.m_corps )  );
	SetImage( uiObj.transform:Find("Back"),  HeroColorSprite( color ) )
	local uiGrid = uiObj.transform:Find("Grid");
	
	-- 总资质
	local attrname = T(149)..":<color=#f7f3bbff>{0}</color> +<color=#03DE27FF>{1}</color>"
	local total = config.attack_base + config.defense_base + config.troops_base;
	local total_wash = pHero.m_attack_wash + pHero.m_defense_wash + pHero.m_troops_wash;
	
	local uiTotalGrowth = uiGrid.transform:GetChild(0).gameObject;
	SetText( uiTotalGrowth.transform:Find("Text"), Utils.StringFormat( attrname, total, total_wash ) )
	
	-- 攻
	local uiAttackBase = uiGrid.transform:GetChild(1).gameObject;
	SetText( uiAttackBase.transform:Find("Text"), F(138, (config.attack_base+pHero.m_attack_wash) ) );

	-- 防
	local uiDefenseBase = uiGrid.transform:GetChild(2).gameObject;
	SetText( uiDefenseBase.transform:Find("Text"), F(139, (config.defense_base+pHero.m_defense_wash)) );

	-- 兵
	local uiTroopsBase = uiGrid.transform:GetChild(3).gameObject;
	SetText( uiTroopsBase.transform:Find("Text"), F(140, (config.troops_base+pHero.m_troops_wash)) );
end

-- 获取突破数值
function HeroColorupDlgRecv( recvValue )
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	if recvValue.m_isup == 1 then
		pop( "突破成功" )
		HeroColorupDlgClose();
		return
	end
	m_recvValue = recvValue
	local config = g_hero_colorup[m_pHero.m_color];
	if config == nil then
		return
	end
	SetText( m_uiUpValue, T(849)..":"..recvValue.m_value )
	SetProgress( m_uiProgress, recvValue.m_value/config.needvalue )
	HeroColorupDlgSetItem();
end

-- 请求关闭
function HeroColorupDlgAskClose()
	if m_recvValue == nil or m_recvValue.m_value <= 0 then
		HeroColorupDlgClose();
		return
	end
	MsgBox( T(854), function()
		HeroColorupDlgClose();
	end )
end
-- 突破
function HeroColorupDlgColorup()
	local config = g_hero_colorup[m_pHero.m_color];
	if config == nil then
		return
	end
	if m_hasitemnum < config.itemnum then
		pop(T(779))
		return
	end
	system_askinfo( ASKINFO_HERO, "", 8, m_pHero.m_kind );
end

