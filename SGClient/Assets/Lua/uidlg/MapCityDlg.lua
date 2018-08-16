-- 界面
local m_Dlg = nil;
local m_uiShape = nil; --UnityEngine.GameObject
local m_uiLevel = nil; --UnityEngine.GameObject
local m_uiPos = nil; --UnityEngine.GameObject
local m_uiActorName = nil; --UnityEngine.GameObject
local m_uiNation = nil; --UnityEngine.GameObject
local m_uiAwardWarn = nil; --UnityEngine.GameObject
local m_uiAwardBack = nil; --UnityEngine.GameObject
local m_uiAwardGrid = nil; --UnityEngine.GameObject
local m_uiSpyWarn = nil; --UnityEngine.GameObject
local m_uiSpyBack = nil; --UnityEngine.GameObject
local m_uiSpyGrid = nil; --UnityEngine.GameObject
local m_uiFriendTitle = nil; --UnityEngine.GameObject
local m_uiFriendBack = nil; --UnityEngine.GameObject
local m_uiFriendWarn = nil; --UnityEngine.GameObject
local m_uiMailButton = nil; --UnityEngine.GameObject
local m_uiRelation = nil; --UnityEngine.GameObject

local m_recvValue = nil;
local m_SpyType = 0;

-- 打开界面
function MapCityDlgOpen()
	m_Dlg = eye.uiManager:Open( "MapCityDlg" );
end

-- 隐藏界面
function MapCityDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "MapCityDlg" );
end

-- 删除界面
function MapCityDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function MapCityDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            MapCityDlgClose();
		
		-- 发送邮件
		elseif nControlID == 1 then
			MailSendDlgSend( m_recvValue );
			
		-- 低级侦察
		elseif nControlID == 10 then
			MapCityDlgSpyAsk( 1 )
		
		-- 中级侦察
		elseif nControlID == 11 then
			MapCityDlgSpyAsk( 2 )
		
		-- 高级侦察
		elseif nControlID == 12 then
			MapCityDlgSpyAsk( 3 )
			
        end
	end
end

-- 载入时调用
function MapCityDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiShape = objs[0];
	m_uiLevel = objs[1];
	m_uiPos = objs[2];
	m_uiActorName = objs[3];
	m_uiNation = objs[4];
	m_uiAwardWarn = objs[5];
	m_uiAwardBack = objs[6];
	m_uiAwardGrid = objs[7];
	m_uiSpyWarn = objs[8];
	m_uiSpyBack = objs[9];
	m_uiSpyGrid = objs[10];
	m_uiFriendTitle = objs[11];
	m_uiFriendBack = objs[12];
	m_uiFriendWarn = objs[13];
	m_uiMailButton = objs[14];
	m_uiRelation = objs[15];
end

-- 界面初始化时调用
function MapCityDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function MapCityDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function MapCityDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function MapCityDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function MapCityDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function MapCityDlgShow( recvValue )
	MapCityDlgOpen()
	m_recvValue = recvValue;
	local name 		= recvValue.m_name;
	local posx 		= recvValue.m_posx;
	local posy 		= recvValue.m_posy;
	local level 	= recvValue.m_char_value[1];
	local nation	= recvValue.m_char_value[2];
	
	SetImage( m_uiShape, LoadSprite( MapUnitCityShapeList[level].."_"..nation ) );
	SetText( m_uiLevel, F(3041,level) )
	SetText( m_uiPos, F(1272, posx, posy) )
	SetText( m_uiActorName, name )
	SetImage( m_uiNation, NationSprite(nation) )
	
	-- 友方城池
	if nation == GetPlayer().m_nation then
		SetFalse( m_uiAwardWarn );
		SetFalse( m_uiAwardBack );
		SetFalse( m_uiAwardGrid );
		SetFalse( m_uiSpyWarn );
		SetFalse( m_uiSpyBack );
		SetFalse( m_uiSpyGrid );
		SetTrue( m_uiFriendTitle );
		SetTrue( m_uiFriendBack );
		SetTrue( m_uiFriendWarn );
		SetText( m_uiRelation, T(3043) )
	else
		SetTrue( m_uiAwardWarn );
		SetTrue( m_uiAwardBack );
		SetTrue( m_uiAwardGrid );
		SetTrue( m_uiSpyWarn );
		SetTrue( m_uiSpyBack );
		SetTrue( m_uiSpyGrid );
		SetFalse( m_uiFriendTitle );
		SetFalse( m_uiFriendBack );
		SetFalse( m_uiFriendWarn );
		SetText( m_uiRelation, T(3042) )
		
		-- 设置侦察花费
		-- 主城等级
		local pBuilding = GetPlayer():GetBuilding( BUILDING_Main, -1 );
		if pBuilding == nil then
			return;
		end
		local silver1 = g_building_upgrade[BUILDING_Main][pBuilding.m_level].value3;
		local silver2 = g_building_upgrade[BUILDING_Main][pBuilding.m_level].value4;
		local token = global.spy_token;
		SetText( m_uiSpyGrid.transform:GetChild(0).gameObject.transform:Find("Cost"), silver1 );
		SetText( m_uiSpyGrid.transform:GetChild(1).gameObject.transform:Find("Cost"), silver2 );
		SetText( m_uiSpyGrid.transform:GetChild(2).gameObject.transform:Find("Cost"), token );
	end
end

-- 侦查
function MapCityDlgSpyAsk( type )
	if m_recvValue == nil or m_recvValue.m_type ~= MAPUNIT_TYPE_CITY then
		return
	end
		
	-- 检查是否学习了侦察科技
	local techlevel = GetPlayer().m_techlevel[19];
	if techlevel <= 0 then
		AlertMsg( T(1218) )
		return
	end
	
	-- 主城等级
	local pBuilding = GetPlayer():GetBuilding( BUILDING_Main, -1 );
	if pBuilding == nil then
		return;
	end
	
	m_SpyType = type;
	if type == 1 then
		local silver1 = g_building_upgrade[BUILDING_Main][pBuilding.m_level].value3;
		if GetPlayer().m_silver < silver1 then
			JumpRes(1,silver1-GetPlayer().m_silver);
			return
		end
		MsgBox( F( 1219, techlevel, T(1204), global.spy_add_techlevel1 ), MapCityDlgSpy ) 
		
	elseif type == 2 then
		local silver2 = g_building_upgrade[BUILDING_Main][pBuilding.m_level].value3;
		if GetPlayer().m_silver < silver2 then
			JumpRes(1,silver2-GetPlayer().m_silver);
			return
		end
		MsgBox( F( 1219, techlevel, T(1205), global.spy_add_techlevel2 ), MapCityDlgSpy ) 
		
	elseif type == 3 then
		local token = global.spy_token;
		if GetPlayer().m_token < token then
			JumpToken();
			return
		end
		MsgBox( F( 1219, techlevel, T(1206), global.spy_add_techlevel3 ), MapCityDlgSpy ) 
		
	end
end

function MapCityDlgSpy()
	if m_SpyType <= 0 then
		return
	end
	system_askinfo( ASKINFO_SPY_CITY, "", 0, m_recvValue.m_unit_index, m_SpyType )
end

