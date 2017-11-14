-- 界面
local m_Dlg = nil;
local m_uiTitle = nil; --UnityEngine.GameObject
local m_uiShape = nil; --UnityEngine.GameObject
local m_uiTalk = nil; --UnityEngine.GameObject
local m_uiBuyButton = nil; --UnityEngine.GameObject

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

local m_OverTalks ={ 
[1] = { 1430 } , -- wei
[2] = { 1440 }, -- shu
[3] = { 1450 }, -- wu
 }
function TreasureDlgShow()
	TreasureDlgOpen()
	if GetPlayer().m_nation == 1 then
		SetImage( m_uiShape, LoadSprite("heroface_72") )
	elseif GetPlayer().m_nation == 2 then
		SetImage( m_uiShape, LoadSprite("heroface_118") )
	elseif GetPlayer().m_nation == 3 then
		SetImage( m_uiShape, LoadSprite("heroface_119") )
	end
	
	system_askinfo( ASKINFO_KINGWAR, "", 10 );
	
	local recvValue = {}
	recvValue.m_treasure_num = 8
	recvValue.m_treasure_num_ed = 3
	recvValue.m_isover = 1;
	TreasureDlgRecv( recvValue )
end

-- 接收服务器信息
function TreasureDlgRecv( recvValue )
	SetText( m_uiTitle, F(1420, recvValue.m_treasure_num, recvValue.m_treasure_num_ed)  )
	-- 挖宝完毕
	if recvValue.m_isover == 1 then
		local talk = m_OverTalks[GetPlayer().m_nation];
		local idx = math.random( 1, #talk )
		SetText( m_uiTalk, T( talk[idx] ) )
		SetFalse( m_uiBuyButton )
	else
		local talk = m_Talks[GetPlayer().m_nation];
		local idx = math.random( 1, #talk )
		SetText( m_uiTalk, T( talk[idx] ) )
		SetTrue( m_uiBuyButton )
	end
end

-- 买酒
function TreasureDlgBuy()
	
end
