-- 界面
local m_Dlg = nil;
local m_uiTitle = nil; --UnityEngine.GameObject
local m_uiShape = nil; --UnityEngine.GameObject
local m_uiTalk = nil; --UnityEngine.GameObject
local m_uiBuyButton = nil; --UnityEngine.GameObject
local m_uiGotoButton = nil; --UnityEngine.GameObject
local m_uiTimer = nil; --UnityEngine.GameObject

local m_recvValue = nil;

-- 打开界面
function TreasureDlgOpen()
	ResourceManager.LoadAssetBundle( "_ab_ui_static_pic_3" )
	m_Dlg = eye.uiManager:Open( "TreasureDlg" );
end

-- 隐藏界面
function TreasureDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "TreasureDlg" );
end

-- 删除界面
function TreasureDlgDestroy()
	GameObject.Destroy( m_Dlg );
	ResourceManager.UnloadAssetBundle( "_ab_ui_static_pic_3" )
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function TreasureDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            TreasureDlgClose();
		elseif nControlID == 1 then
			TreasureDlgBuy();
		elseif nControlID == 2 then
			TreasureDlgGoTo();
        end
	elseif nType == UI_EVENT_TIMECOUNTEND then
		if nControlID == 1 then
			TreasureDlgClose()
		end
	end
end

-- 载入时调用
function TreasureDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiTitle = objs[0];
	m_uiShape = objs[1];
	m_uiTalk = objs[2];
	m_uiBuyButton = objs[3];
	m_uiGotoButton = objs[4];
	m_uiTimer = objs[5];
end

-- 界面初始化时调用
function TreasureDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function TreasureDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function TreasureDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function TreasureDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function TreasureDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
local m_Talks ={ 
[1] = { 1431, 1432, 1433 }, -- wei
[2] = { 1441, 1442, 1443 }, -- shu
[3] = { 1451, 1452, 1453 }, -- wu
 }

local m_PosTalks ={ 
[1] = { 1434, 1435, 1436 } , -- wei
[2] = { 1444, 1445, 1446 }, -- shu
[3] = { 1454, 1455, 1456 }, -- wu
 }

local m_OverTalks ={ 
[1] = { 1430 } , -- wei
[2] = { 1440 }, -- shu
[3] = { 1450 }, -- wu
 }
function TreasureDlgShow( recvValue )
	TreasureDlgOpen()
	SetFalse( m_uiBuyButton )
	SetFalse( m_uiGotoButton )
	
	-- 活动倒计时
	local leftstamp = MapMainDlgActivityTreasureEndStamp()-GetServerTime();
	SetTimer( m_uiTimer, leftstamp, leftstamp, 1, T(1462) )
		
	-- 神将形象
	if GetPlayer().m_nation == 1 then
		SetImage( m_uiShape, LoadSprite("heroface_72") )
	elseif GetPlayer().m_nation == 2 then
		SetImage( m_uiShape, LoadSprite("heroface_118") )
	elseif GetPlayer().m_nation == 3 then
		SetImage( m_uiShape, LoadSprite("heroface_119") )
	end
	
	-- 标题
	SetText( m_uiTitle, F(1420, recvValue.m_treasure_num_max, recvValue.m_treasure_num_max-recvValue.m_treasure_num[GetPlayer().m_nation] ) )
	TreasureDlgRecv( m_recvValue )
end

-- m_has=0,m_px=0,m_py=0,m_tn=0
function TreasureDlgRecv( recvValue )
	m_recvValue = recvValue;
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	-- 挖宝完毕
	if recvValue.m_has > 0 then
		local talk = m_OverTalks[GetPlayer().m_nation];
		local idx = math.random( 1, #talk )
		SetText( m_uiTalk, T( talk[idx] ) )
		SetFalse( m_uiBuyButton )
		SetFalse( m_uiGotoButton )
	else
		local talk = m_Talks[GetPlayer().m_nation];
		local idx = math.random( 1, #talk )
		
		if recvValue.m_px > 0 then
			local talk = m_PosTalks[GetPlayer().m_nation];
			local idx = math.random( 1, #talk )		
			SetText( m_uiTalk, F( talk[idx], recvValue.m_px, recvValue.m_py ) )
			SetFalse( m_uiBuyButton )
			SetTrue( m_uiGotoButton )
		else
			local talk = m_Talks[GetPlayer().m_nation];
			local idx = math.random( 1, #talk )
			SetText( m_uiTalk, T( talk[idx] ) )
			SetTrue( m_uiBuyButton )
			SetFalse( m_uiGotoButton )
		end
	end
end

-- 买酒
function TreasureDlgBuy()
	if m_recvValue.m_has > 0 then
		return
	end
	if m_recvValue.m_px > 0 then
		return
	end 
	local tn = m_recvValue.m_tn
	MsgBox( F( 1920, g_kingwar_config[tn+1].treasure_costtoken ), function() 
		if GetPlayer().m_token < g_kingwar_config[tn+1].treasure_costtoken then
			JumpToken();
			return
		end
		system_askinfo( ASKINFO_KINGWAR, "", 13 );
	end )
end

-- 前往
function TreasureDlgGoTo()
	system_askinfo( ASKINFO_KINGWAR, "", 15, m_recvValue.m_px, m_recvValue.m_py );
	TreasureDlgClose()
end

-- 挖宝
function TreasureDlgDo( GameCoorX, GameCoorY )
	local num = m_recvValue.m_tn
	if num <= 0 then
		num = 1
	end
	local point = g_kingwar_config[num].treasure_costpoint
	if m_recvValue.m_px == GameCoorX and m_recvValue.m_py == GameCoorY then
		point = g_kingwar_config[num].treasure_costpoint -- 买酒挖宝
	else
		point = g_kingwar_config[1].treasure_normalpoint -- 缺省挖宝
	end
	MsgBox( F(1922,point), function() 
		system_askinfo( ASKINFO_KINGWAR, "", 14, GameCoorX, GameCoorY );
	end )
end
