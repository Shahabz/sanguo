-- 界面
local m_Dlg = nil;
local m_uiTitle = nil; --UnityEngine.GameObject
local m_uiShape = nil; --UnityEngine.GameObject
local m_uiName = nil; --UnityEngine.GameObject
local m_uiPos = nil; --UnityEngine.GameObject
local m_uiOwn = nil; --UnityEngine.GameObject
local m_uiNation = nil; --UnityEngine.GameObject
local m_uiChangeNameBtn = nil; --UnityEngine.GameObject
local m_uiLayer1 = nil; --UnityEngine.GameObject
local m_uiLayer2 = nil; --UnityEngine.GameObject
local m_uiButtons = nil; --UnityEngine.GameObject
local m_uiFightButton = nil; --UnityEngine.GameObject
local m_uiUpgradeButton = nil; --UnityEngine.GameObject
local m_uiTimerButton = nil; --UnityEngine.GameObject
local m_uiAttackcd = nil; --UnityEngine.GameObject
local m_uiLayer3 = nil; --UnityEngine.GameObject
local m_uiKingButton = nil; --UnityEngine.GameObject

local m_LastRecvValue = nil;
local m_recvValue = nil;
local m_posx = 0;
local m_posy = 0;
local m_nation = 0;
local m_townid = 0;
local m_custom_name = "";

-- 打开界面
function MapTownExDlgOpen()
	ResourceManager.LoadAssetBundle( "_ab_ui_static_pic_2" )
	m_Dlg = eye.uiManager:Open( "MapTownExDlg" );
end

-- 隐藏界面
function MapTownExDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "MapTownExDlg" );
end

-- 删除界面
function MapTownExDlgDestroy()
	GameObject.Destroy( m_Dlg );
	ResourceManager.UnloadAssetBundle( "_ab_ui_static_pic_2" )
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function MapTownExDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            MapTownExDlgClose();
			
		-- 国战
		elseif nControlID == 1 then
			MapTownExDlgNationFight()
			
		-- 开发
		elseif nControlID == 2 then
			MapTownExDlgDev()
		
		-- 皇城密库
		elseif nControlID == 3 then
			KingWarChangeDlgShow();
			
		-- 改名
		elseif nControlID == 10 then	
			TownChangeNameDlgShow( m_townid, m_custom_name )
        end
	elseif nType == UI_EVENT_TIMECOUNTCHANGED then
		if nControlID == 101 then
			SetText( m_uiTimerButton.transform:Find("Back/Token"), MapTownExDlgDevTokenCalc() ) 
		end
	elseif nType == UI_EVENT_TIMECOUNTEND then
		if nControlID == 101 then
			if m_recvValue ~= nil and m_recvValue.m_dev_cd ~= nil then
				m_recvValue.m_dev_cd = 0;
				MapTownExDlgRecvValue( m_recvValue )
			else
				MapTownExDlgClose()
			end
		end
	end
end

-- 载入时调用
function MapTownExDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiTitle = objs[0];
	m_uiShape = objs[1];
	m_uiName = objs[2];
	m_uiPos = objs[3];
	m_uiOwn = objs[4];
	m_uiNation = objs[5];
	m_uiChangeNameBtn = objs[6];
	m_uiLayer1 = objs[7];
	m_uiLayer2 = objs[8];
	m_uiButtons = objs[9];
	m_uiFightButton = objs[10];
	m_uiUpgradeButton = objs[11];
	m_uiTimerButton = objs[12];
	m_uiAttackcd = objs[13];
	m_uiLayer3 = objs[14];
	m_uiKingButton = objs[15];

end

-- 界面初始化时调用
function MapTownExDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function MapTownExDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function MapTownExDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function MapTownExDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function MapTownExDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function MapTownExDlgShow( recvValue )
	MapTownExDlgOpen()
	SetFalse( m_uiChangeNameBtn )
	SetFalse( m_uiFightButton )
	SetFalse( m_uiUpgradeButton )
	SetFalse( m_uiTimerButton )
	SetFalse( m_uiLayer2.transform:Find("Warn3") )
	
	m_LastRecvValue = recvValue
	m_posx 				= recvValue.m_posx;
	m_posy 				= recvValue.m_posy;
	m_custom_name		= recvValue.m_name;
	local custom_namelen= recvValue.m_namelen;
	m_nation 			= recvValue.m_char_value[1];
	local dev_level 	= recvValue.m_char_value[2];
	m_townid 			= recvValue.m_short_value[1];
	local protect_sec	= recvValue.m_int_value[1];
	local produce_sec	= recvValue.m_int_value[2];
	local type 			= g_towninfo[m_townid].type
	local level 		= g_towninfo[m_townid].level
	
	-- 洛阳
	if type == MAPUNIT_TYPE_TOWN_LUOYANG then
		-- 名字
		SetText( m_uiName, MapTownName(m_townid) )
		-- 所属
		if m_nation == 0 then
			SetText( m_uiOwn, T(951) );
		else
			SetText( m_uiOwn, T(1211).." "..NationEx(m_nation) );
		end
		SetTrue( m_uiKingButton )
		SetTrue( m_uiLayer3 )
		SetFalse( m_uiLayer1 )
		SetFalse( m_uiLayer2 )
		-- 形象
		SetImage( m_uiShape, LoadSprite( MapUnitTownShapeList[type].."_"..m_nation ) )
	else
		-- 国都信息
		system_askinfo( ASKINFO_MAPTOWN, "", 7, m_townid );
		-- 名字
		if custom_namelen > 0 then
			SetText( m_uiName, "Lv."..(dev_level+1).." "..m_custom_name )
		else
			SetText( m_uiName, "Lv."..(dev_level+1).." "..MapTownName(m_townid) )
		end
		-- 所属
		if m_nation == 0 then
			SetText( m_uiOwn, T(1328)..":Lv.11 "..MapTownName(m_townid)..T(1328) );
		else
			SetText( m_uiOwn, T(1329) );
		end
		SetFalse( m_uiKingButton )
		SetFalse( m_uiLayer3 )
		-- 形象
	SetImage( m_uiShape, LoadSprite( MapUnitTownShapeList[type].."_"..m_nation ), true )
	end
	
	-- 位置
	SetText( m_uiPos, F(1272, m_posx, m_posy) )
	-- 国旗
	SetImage( m_uiNation, NationSprite(m_nation) )
end

-- m_dev_level=0,m_dev_exp=0,m_dev_expmax=0,m_mytownid=0,m_dev_cd=0,m_attackcd=0
function MapTownExDlgRecvValue( recvValue )
	m_recvValue = recvValue
	MapTownExDlgSetLayer2( recvValue );
	if m_nation == 0 then
		-- 群雄占领
		if recvValue.m_mytownid > 0 then
			-- 我国已经有国都
			SetFalse( m_uiLayer1 )
			SetTrue( m_uiLayer2 )
			SetTrue( m_uiLayer2.transform:Find("Warn3") )
		else
			-- 我国没国都
			SetTrue( m_uiLayer1 )
			SetFalse( m_uiLayer2 )
			SetTrue( m_uiFightButton )
		end
	else
		SetFalse( m_uiLayer1 )
		SetTrue( m_uiLayer2 )
		-- 玩家占领
		if recvValue.m_mytownid == m_townid then
			-- 我的国家占领
			if recvValue.m_dev_cd > 0 then
				SetFalse( m_uiUpgradeButton )
				SetTrue( m_uiTimerButton );
				SetTimer( m_uiTimerButton.transform:Find("Back/Timer"), recvValue.m_dev_cd, recvValue.m_dev_cd, 101 )
				SetText( m_uiTimerButton.transform:Find("Back/Token"), MapTownExDlgDevTokenCalc() ) 
			else
				SetFalse( m_uiTimerButton )
				SetTrue( m_uiUpgradeButton )
			end
			if GetPlayer().m_official > 0 then
				SetTrue( m_uiChangeNameBtn )
			else
				SetFalse( m_uiChangeNameBtn )
			end
		else
			-- 他国占领
			SetTrue( m_uiLayer2.transform:Find("Warn3") )
		end
	end
end

function MapTownExDlgSetLayer2( recvValue )
	SetProgress( m_uiLayer2.transform:Find("ExpProgress"), recvValue.m_dev_exp/recvValue.m_dev_expmax )
	SetText( m_uiLayer2.transform:Find("ExpProgress/Text"), T(127)..":"..recvValue.m_dev_exp.."/"..recvValue.m_dev_expmax )
	if recvValue.m_attackcd > 0 then
		SetTrue( m_uiAttackcd )
		SetTimer( m_uiAttackcd, recvValue.m_attackcd, recvValue.m_attackcd )
	else
		SetFalse( m_uiAttackcd )
	end
	local uiHeroList = m_uiLayer2.transform:Find("HeroList");
	for i = 1, 3, 1 do
		SetText( uiHeroList:GetChild(i-1):Find("Name"), "Lv."..i.." "..T(1330) )
		SetImage( uiHeroList:GetChild(i-1):Find("Color"), ItemColorSprite( i+2 ) )
	end
end

-- 国战
function MapTownExDlgNationFight()
	if GetPlayer().m_official == 0 or GetPlayer().m_official == 6 then
		AlertMsg( T(1852) )
		return
	end
	MapNationFightDlgShow( m_LastRecvValue.m_unit_index )
	MapTownExDlgClose()
end

-- 开发
function MapTownExDlgDev()
	if m_recvValue.m_dev_cd > 0 then
		local cost_token = MapTownExDlgDevTokenCalc()
		MsgBox( F(1339, cost_token ), function() 
			if GetPlayer().m_token < cost_token then
				JumpToken()
				return
			end	
			system_askinfo( ASKINFO_MAPTOWN, "", 9, m_townid );
		end )
		return
	end
	
	local cost_silver = global.town_dev_silver
	local cost_wood = global.town_dev_wood
	MsgBox( F(1338, T(121)..knum(cost_silver).." "..T(122)..knum(cost_wood) ), function() 
		if GetPlayer().m_silver < cost_silver then
			JumpRes(1)
			return
		end
		if GetPlayer().m_wood < cost_wood then
			JumpRes(2)
			return
		end	
		system_askinfo( ASKINFO_MAPTOWN, "", 8, m_townid );
	end )
end

-- 计算钻石加速
function MapTownExDlgDevTokenCalc()
	local uiTimer = m_uiTimerButton.transform:Find("Back/Timer")
	if uiTimer == nil then
		return 0;
	end
	local left = uiTimer.transform:GetComponent( typeof(UITextTimeCountdown) ).LeftTime
	local min = math.floor(left/60) + 1
	local token = math.ceil( min*global.upgradequick_token)
	return token
end

