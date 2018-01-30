-- 界面
local m_Dlg = nil;
local m_uiShape = nil; --UnityEngine.GameObject
local m_uiPrefix = nil; --UnityEngine.GameObject
local m_uiNation = nil; --UnityEngine.GameObject
local m_uiName = nil; --UnityEngine.GameObject
local m_uiOfficial = nil; --UnityEngine.GameObject
local m_uiPlace = nil; --UnityEngine.GameObject
local m_uiZone = nil; --UnityEngine.GameObject
local m_uiSignText = nil; --UnityEngine.GameObject
local m_uiBattlepower = nil; --UnityEngine.GameObject
local m_uiKillCity = nil; --UnityEngine.GameObject
local m_uiHeroInfoBtn = nil; --UnityEngine.GameObject
local m_uiBattleCmpBtn = nil; --UnityEngine.GameObject
local m_uiGridTitle = nil; --UnityEngine.GameObject
local m_uiHeroGrid = nil; --UnityEngine.GameObject
local m_uiBattlepowerGrid = nil; --UnityEngine.GameObject
local m_uiSendMailBtn = nil; --UnityEngine.GameObject
local m_recvValue = nil
-- 打开界面
function ActorSearchDlgOpen()
	m_Dlg = eye.uiManager:Open( "ActorSearchDlg" );
end

-- 隐藏界面
function ActorSearchDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "ActorSearchDlg" );
	m_recvValue = nil
end

-- 删除界面
function ActorSearchDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function ActorSearchDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            ActorSearchDlgClose();
			
		-- 发送邮件
		elseif nControlID == 1 then
			if m_recvValue ~= nil then
				MailSendDlgShow( m_recvValue.m_actorid, m_recvValue.m_name, m_recvValue.m_nation )
			end
		-- 武将信息
		elseif nControlID == 2 then
			ActorSearchDlgShowHero()
		
		-- 战力信息
		elseif nControlID == 3 then
			ActorSearchDlgShowBattlepower()
        end
	end
end

-- 载入时调用
function ActorSearchDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiShape = objs[0];
	m_uiPrefix = objs[1];
	m_uiNation = objs[2];
	m_uiName = objs[3];
	m_uiOfficial = objs[4];
	m_uiPlace = objs[5];
	m_uiZone = objs[6];
	m_uiSignText = objs[7];
	m_uiBattlepower = objs[8];
	m_uiKillCity = objs[9];
	m_uiHeroInfoBtn = objs[10];
	m_uiBattleCmpBtn = objs[11];
	m_uiGridTitle = objs[12];
	m_uiHeroGrid = objs[13];
	m_uiBattlepowerGrid = objs[14];
	m_uiSendMailBtn = objs[15];
end

-- 界面初始化时调用
function ActorSearchDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function ActorSearchDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function ActorSearchDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function ActorSearchDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function ActorSearchDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function ActorSearchDlgShow( actorid, city_index )
	ActorSearchDlgOpen()		
	if city_index == nil then
		system_askinfo( ASKINFO_ACTOR, "", 2, actorid, -1 );
	else
		system_askinfo( ASKINFO_ACTOR, "", 2, actorid, city_index );
	end
	if actorid == GetPlayer().m_actorid then
		SetFalse( m_uiSendMailBtn )
	else
		SetTrue( m_uiSendMailBtn )
	end
	SetText( m_uiGridTitle.transform:Find("Text"), T(692) )
	SetFalse( m_uiHeroInfoBtn )
	SetTrue( m_uiBattleCmpBtn )
	SetTrue( m_uiHeroGrid )
	SetFalse( m_uiBattlepowerGrid )
	for i=0, 3, 1 do
		SetFalse( m_uiHeroGrid.transform:GetChild( i ) )
	end
end

-- m_actorid=0,m_city_index=0,m_namelen=0,m_name="[m_namelen]",m_signlen=0,m_sign="[m_signlen]",m_level=0,m_official=0,m_place=0,m_zone=0,m_nation=0,m_battlepower=0,m_killcity=0,m_herocount=0,m_hero={m_kind=0,m_color=0,m_level=0,m_pre_exp=0,m_exp=0,[m_herocount]},
function ActorSearchDlgRecv( recvValue )
	m_recvValue = recvValue
	SetImage( m_uiShape, PlayerHeadSprite( recvValue.m_shape ) )
	SetImage( m_uiNation, NationSprite( recvValue.m_nation ) )
	SetText( m_uiName, "Lv."..recvValue.m_level.." "..recvValue.m_name )
	SetText( m_uiPrefix, "" )
	SetText( m_uiOfficial, T(131)..": <color=#ECC244FF>"..OfficialName( recvValue.m_official ).."</color>" );
	SetText( m_uiPlace, T(130)..": <color=#ECC244FF>"..PlaceName( recvValue.m_place ).."</color>" )
	SetText( m_uiZone, T(132)..": <color=#25C9FFFF>"..MapZoneName( recvValue.m_zone ).."</color>" )
	if recvValue.m_signlen > 0 then
		SetText( m_uiSignText, T(2067)..recvValue.m_sign )
	else
		SetText( m_uiSignText, T(2067)..T(2069) )
	end
	SetText( m_uiBattlepower, T(672)..recvValue.m_battlepower )
	SetText( m_uiKillCity, T(2064)..":"..recvValue.m_killcity )
	ActorSearchDlgShowHero()
end

-- 显示武将页
function ActorSearchDlgShowHero()
	SetTrue( m_uiBattleCmpBtn )
	SetFalse( m_uiHeroInfoBtn )
	SetTrue( m_uiHeroGrid )
	SetFalse( m_uiBattlepowerGrid )
	SetText( m_uiGridTitle.transform:Find("Text"), T(692) )
	for i=1, m_recvValue.m_herocount, 1 do
		local pHero = m_recvValue.m_hero[i]
		if pHero.m_kind > 0 then
			local uiHero = m_uiHeroGrid.transform:GetChild( i-1 )
			SetTrue( uiHero )
			local objs = uiHero.transform:GetComponent( typeof(Reference) ).relatedGameObject;
			local uiShape = objs[0];
			local uiColor = objs[1];
			local uiCorps = objs[2];
			local uiName = objs[3];
			local uiType = objs[4];
		
			SetImage( uiShape, HeroHeadSprite( pHero.m_kind ) )
			SetImage( uiColor, ItemColorSprite( pHero.m_color ) )
			SetImage( uiCorps, CorpsSprite( g_heroinfo[pHero.m_kind][pHero.m_color].corps ) )
			SetText( uiName, HeroNameLv( pHero.m_kind, pHero.m_level ) )
			
			-- 是否无双，神将，无双神将
			if pHero.m_type == 1 then
				SetTrue( uiType )
				SetText( uiType, T(2357) )
			elseif pHero.m_type == 2 then
				SetTrue( uiType )
				SetText( uiType, T(2358) )
			elseif pHero.m_type == 3 then
				SetTrue( uiType )
				SetText( uiType, T(2359) )
			else
				SetFalse( uiType )
			end
			
		end
	end
end

-- 显示战力页
function ActorSearchDlgShowBattlepower()
	SetTrue( m_uiHeroInfoBtn )
	SetFalse( m_uiBattleCmpBtn )
	SetFalse( m_uiHeroGrid )
	SetTrue( m_uiBattlepowerGrid )
	SetText( m_uiGridTitle.transform:Find("Text"), T(2066) )
	
	if m_recvValue.m_actorid == GetPlayer().m_actorid then
		local offset = 0;
		-- 武将
		if m_recvValue.m_bp_hero > 0 then
			local uiObj = m_uiBattlepowerGrid.transform:GetChild( offset )
			ActorSearchDlgSetMyBattlepower( uiObj, 2072, m_recvValue.m_bp_hero )
			offset = offset + 1;
		end
		
		-- 装备
		if m_recvValue.m_bp_equip > 0 then
			local uiObj = m_uiBattlepowerGrid.transform:GetChild( offset )
			ActorSearchDlgSetMyBattlepower( uiObj, 2073, m_recvValue.m_bp_equip )
			offset = offset + 1;
		end
		
		-- 科技
		if m_recvValue.m_bp_tech > 0 then
			local uiObj = m_uiBattlepowerGrid.transform:GetChild( offset )
			ActorSearchDlgSetMyBattlepower( uiObj, 2074, m_recvValue.m_bp_tech )
			offset = offset + 1;
		end
		
		-- 国器
		if m_recvValue.m_bp_nequip > 0 then
			local uiObj = m_uiBattlepowerGrid.transform:GetChild( offset )
			ActorSearchDlgSetMyBattlepower( uiObj, 2075, m_recvValue.m_bp_nequip )
			offset = offset + 1;
		end
		
		-- 爵位
		if m_recvValue.m_bp_place > 0 then
			local uiObj = m_uiBattlepowerGrid.transform:GetChild( offset )
			ActorSearchDlgSetMyBattlepower( uiObj, 2076, m_recvValue.m_bp_place )
			offset = offset + 1;
		end
		
		-- 女将
		if m_recvValue.m_bp_girl > 0 then
			local uiObj = m_uiBattlepowerGrid.transform:GetChild( offset )
			ActorSearchDlgSetMyBattlepower( uiObj, 2077, m_recvValue.m_bp_girl )
			offset = offset + 1;
		end
		
		for i=0, m_uiBattlepowerGrid.transform.childCount-1, 1 do
			if i < offset then
				SetTrue( m_uiBattlepowerGrid.transform:GetChild( i ) )
			else
				SetFalse( m_uiBattlepowerGrid.transform:GetChild( i ) )
			end
		end
	else
		
		-- 表头
		local uiObj = m_uiBattlepowerGrid.transform:GetChild( 0 );
		SetTrue( uiObj.transform:Find("Name1") )
		SetTrue( uiObj.transform:Find("Name2") )
		SetTrue( uiObj.transform:Find("Name3") )
		SetFalse( uiObj.transform:Find("Name4") )
		SetFalse( uiObj.transform:Find("Name5") )
		SetFalse( uiObj.transform:Find("IconUp") )
		SetFalse( uiObj.transform:Find("IconDown") )
		SetText( uiObj.transform:Find("Name1"), T(2070), NameColor(4) )
		SetText( uiObj.transform:Find("Name2"), "VS" )
		SetText( uiObj.transform:Find("Name3"), T(2071), NameColor(1) )
				
		ActorSearchDlgSetBattlepower( m_uiBattlepowerGrid.transform:GetChild( 1 ), 2072, m_recvValue.m_bp_hero, m_recvValue.m_my_bp_hero )
		ActorSearchDlgSetBattlepower( m_uiBattlepowerGrid.transform:GetChild( 2 ), 2073, m_recvValue.m_bp_equip, m_recvValue.m_my_bp_equip )
		ActorSearchDlgSetBattlepower( m_uiBattlepowerGrid.transform:GetChild( 3 ), 2074, m_recvValue.m_bp_tech, m_recvValue.m_my_bp_tech )
		ActorSearchDlgSetBattlepower( m_uiBattlepowerGrid.transform:GetChild( 4 ), 2075, m_recvValue.m_bp_nequip, m_recvValue.m_my_bp_nequip )
		ActorSearchDlgSetBattlepower( m_uiBattlepowerGrid.transform:GetChild( 5 ), 2076, m_recvValue.m_bp_place, m_recvValue.m_my_bp_place )
		ActorSearchDlgSetBattlepower( m_uiBattlepowerGrid.transform:GetChild( 6 ), 2077, m_recvValue.m_bp_girl, m_recvValue.m_my_bp_girl )
	end

end

-- 设置我的战力
function ActorSearchDlgSetMyBattlepower( uiObj, textid, value )
	SetText( uiObj.transform:Find("Name5"), T(textid)..":"..value );
	SetTrue( uiObj.transform:Find("Name5") )
	SetFalse( uiObj.transform:Find("Name1") )
	SetFalse( uiObj.transform:Find("Name2") )
	SetFalse( uiObj.transform:Find("Name3") )
	SetFalse( uiObj.transform:Find("Name4") )
	SetFalse( uiObj.transform:Find("IconUp") )
	SetFalse( uiObj.transform:Find("IconDown") )
end

-- 设置战力对比
function ActorSearchDlgSetBattlepower( uiObj, textid, value, myvalue )
	SetTrue( uiObj )
	SetTrue( uiObj.transform:Find("Name1") )
	SetTrue( uiObj.transform:Find("Name2") )
	SetTrue( uiObj.transform:Find("Name3") )
	SetTrue( uiObj.transform:Find("Name4") )
	SetFalse( uiObj.transform:Find("Name5") )
	
	SetText( uiObj.transform:Find("Name1"), value )
	SetText( uiObj.transform:Find("Name2"), T(textid) )
	SetText( uiObj.transform:Find("Name3"), myvalue )
	
	local cmpnum =  myvalue - value
	if cmpnum > 0 then
		SetText( uiObj.transform:Find("Name4"), cmpnum )
		SetTrue( uiObj.transform:Find("IconUp") )
		SetFalse( uiObj.transform:Find("IconDown") )
	elseif cmpnum < 0 then
		SetText( uiObj.transform:Find("Name4"), -cmpnum )
		SetFalse( uiObj.transform:Find("IconUp") )
		SetTrue( uiObj.transform:Find("IconDown") )
	else
		SetFalse( uiObj.transform:Find("Name4") )
		SetFalse( uiObj.transform:Find("IconUp") )
		SetFalse( uiObj.transform:Find("IconDown") )
	end
end

