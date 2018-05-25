-- 界面
local m_Dlg = nil;
local m_uiTownBtnList={};
local m_uiMaster={};

-- 打开界面
function MapGlobalDlgOpen()
	m_Dlg = eye.uiManager:Open( "MapGlobalDlg" );
	if GameManager.WorldMap ~= nil then
		GameManager.WorldMap.gameObject:SetActive( false );
	end
end

-- 隐藏界面
function MapGlobalDlgClose()
	if m_Dlg == nil then
		return;
	end
	if GameManager.WorldMap ~= nil then
		GameManager.WorldMap.gameObject:SetActive( true );
	end
	eye.uiManager:Close( "MapGlobalDlg" );
end

-- 删除界面
function MapGlobalDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function MapGlobalDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            MapGlobalDlgClose();
		elseif nControlID >= 1 and nControlID <= 30 then
			MapGlobalDlgSelect( nControlID )
        end
	end
end

-- 载入时调用
function MapGlobalDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiTownBtnList[1] = objs[0];
	m_uiTownBtnList[2] = objs[1];
	m_uiTownBtnList[3] = objs[2];
	m_uiTownBtnList[4] = objs[3];
	m_uiTownBtnList[5] = objs[4];
	m_uiTownBtnList[6] = objs[5];
	m_uiTownBtnList[7] = objs[6];
	m_uiTownBtnList[8] = objs[7];
	m_uiTownBtnList[9] = objs[8];
	m_uiMaster[1] = objs[9];
	m_uiMaster[2] = objs[10];
	m_uiMaster[3] = objs[11];
	m_uiMaster[4] = objs[12];
	m_uiMaster[5] = objs[13];
	m_uiMaster[6] = objs[14];
	m_uiMaster[7] = objs[15];
	m_uiMaster[8] = objs[16];
	m_uiMaster[9] = objs[17];
end

-- 界面初始化时调用
function MapGlobalDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function MapGlobalDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function MapGlobalDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function MapGlobalDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function MapGlobalDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function MapGlobalDlgShow()
	MapGlobalDlgOpen()
	
	--[[-- 皇城未开启
	if GetPlayer().m_open_townking <= 0 then
		-- 如果郡城已经攻克，只能看见对应州城和对应郡城
		if GetPlayer().m_open_town3 > 0 then
			for zoneid,v in pairs(m_uiTownBtnList) do
				if map_zone_ismovezone( GetPlayer().m_zone, zoneid ) == 1 or GetPlayer().m_zone == zoneid then
					SetButtonTrue( v )
					SetGray( v.transform:Find("Back"), false )
					SetTextColor( v.transform:Find("Text"), Hex2Color(0xFFDE00FF) )
				else
					SetButtonFalse( v )
					SetGray( v.transform:Find("Back"), true )
					SetTextColor( v.transform:Find("Text"), Hex2Color(0xC1C1C1FF) )
				end
			end
		else
			-- 只能看见自己的郡城
			for zoneid,v in pairs(m_uiTownBtnList) do
				if GetPlayer().m_zone == zoneid then
					SetButtonTrue( v )
					SetGray( v.transform:Find("Back"), false )
					SetTextColor( v.transform:Find("Text"), Hex2Color(0xFFDE00FF) )
				else
					SetButtonFalse( v )
					SetGray( v.transform:Find("Back"), true )
					SetTextColor( v.transform:Find("Text"), Hex2Color(0xC1C1C1FF) )
				end
			end
		end
	else
		-- 皇城已经开启
		for zoneid,v in pairs(m_uiTownBtnList) do
			SetButtonTrue( v )
			SetGray( v.transform:Find("Back"), false )
			SetTextColor( v.transform:Find("Text"), Hex2Color(0xFFDE00FF) )
		end
	end--]]
	
	-- 司隶没开启
	if GetPlayer().m_open_zone_sili == 0 then
		for zoneid,v in pairs(m_uiTownBtnList) do
			if zoneid == MAPZONE_CENTERID then
				SetButtonFalse( v )
				SetGray( v.transform:Find("Back"), true )
				SetTextColor( v.transform:Find("Text"), Hex2Color(0xC1C1C1FF) )
			else
				SetButtonTrue( v )
				SetGray( v.transform:Find("Back"), false )
				SetTextColor( v.transform:Find("Text"), Hex2Color(0xFFDE00FF) )
			end
		end
	else
		for zoneid,v in pairs(m_uiTownBtnList) do
			SetButtonTrue( v )
			SetGray( v.transform:Find("Back"), false )
			SetTextColor( v.transform:Find("Text"), Hex2Color(0xFFDE00FF) )	
		end
	end
	
	-- 所属国家
	for zoneid,v in pairs(m_uiTownBtnList) do
		if g_ZoneMaster[zoneid] and g_ZoneMaster[zoneid].m_nation > 0 then
			SetTrue( v.transform:Find("Nation") )
			SetImage( v.transform:Find("Nation"), NationSprite(g_ZoneMaster[zoneid].m_nation) )
		else
			SetFalse( v.transform:Find("Nation") )
		end	
	end
	
	-- 州牧皇帝信息
	for zoneid,v in pairs(m_uiMaster) do
		if g_ZoneMaster[zoneid] and g_ZoneMaster[zoneid].m_nation > 0 then
			SetTrue( v )
			SetImage( v.transform:Find("Shape"), PlayerHeadSprite(g_ZoneMaster[zoneid].m_shape) )
			SetText( v.transform:Find("Name/Text"), g_ZoneMaster[zoneid].m_name )
		else
			SetFalse( v )
			--SetImage( v.transform:Find("Shape"), LoadSprite("playerhead_normal") )
			--SetText( v.transform:Find("Name/Text"), T(3030) )
		end
	end
end

-- 选择区域
function MapGlobalDlgSelect( zoneid )
	if g_zoneinfo[zoneid] == nil then
		return
	end
	MapGlobalDlgClose();
	MapZoneDlgShow( zoneid )
	--WorldMap.GotoCoor( g_zoneinfo[zoneid].center_posx, g_zoneinfo[zoneid].center_posy )
end
