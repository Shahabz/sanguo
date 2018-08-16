-- 界面
local m_Dlg = nil;
local m_uiTitle = nil; --UnityEngine.GameObject
local m_uiShape = nil; --UnityEngine.GameObject
local m_uiName = nil; --UnityEngine.GameObject
local m_uiDesc = nil; --UnityEngine.GameObject
local m_uiCostButton = nil; --UnityEngine.GameObject

local m_recvValue = nil;

-- 打开界面
function MapEventDlgOpen()
	m_Dlg = eye.uiManager:Open( "MapEventDlg" );
end

-- 隐藏界面
function MapEventDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "MapEventDlg" );
end

-- 删除界面
function MapEventDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function MapEventDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            MapEventDlgClose();
		elseif nControlID == 1 then
			MapEventDlgGather()
        end
	end
end

-- 载入时调用
function MapEventDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiTitle = objs[0];
	m_uiShape = objs[1];
	m_uiName = objs[2];
	m_uiDesc = objs[3];
	m_uiCostButton = objs[4];
end

-- 界面初始化时调用
function MapEventDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function MapEventDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function MapEventDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function MapEventDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function MapEventDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function MapEventDlgShow( recvValue )	
	m_recvValue 	= recvValue;
	local state 	= recvValue.m_state;
	local posx 		= recvValue.m_posx;
	local posy 		= recvValue.m_posy;
	local type		= recvValue.m_char_value[1];
	local kind		= recvValue.m_short_value[1];
	local actorid	= recvValue.m_int_value[1];
	local waitsec	= recvValue.m_int_value[2];
	if waitsec > 0 then
		return
	end
	local info 		= g_mapeventinfo[kind]
	if info == nil then
		return
	end
	-- 已完成直接领取
	if state > 0 then
		MapEventDlgGetAward()
		return
	end
	MapEventDlgOpen()
	SetText( m_uiTitle, T( MapUnitEventNameList[type] ) )
	SetText( m_uiName, T( MapUnitEventNameList[type] ) )
	SetImage( m_uiShape, LoadSprite( MapUnitEventShapeList[type] ) )
	if type == 1 then -- 募兵加速
		SetText( m_uiDesc, F(1015, info.awardnum/60 ) )
	elseif type == 2 then -- 建造加速
		SetText( m_uiDesc, F(1016, info.awardnum/60 ) )
	elseif type == 3 then -- 运输车
		if info.awardkind == AWARDKIND_SILVER then
			SetText( m_uiDesc, T(120)..ResName(1).."x"..knum(info.awardnum) )
		elseif info.awardkind == AWARDKIND_WOOD then
			SetText( m_uiDesc, T(120)..ResName(2).."x"..knum(info.awardnum) )
		elseif info.awardkind == AWARDKIND_FOOD then
			SetText( m_uiDesc, T(120)..ResName(3).."x"..knum(info.awardnum) )
		elseif info.awardkind == AWARDKIND_IRON then
			SetText( m_uiDesc, T(120)..ResName(4).."x"..knum(info.awardnum) )
		elseif info.awardkind == AWARDKIND_TOKEN then
			SetText( m_uiDesc, T(120)..ResName(5).."x"..knum(info.awardnum) )
		end
	elseif type == 4 then -- 采集粮仓
		SetText( m_uiDesc, T(120)..ResName(3).."x"..knum(info.awardnum) )
	elseif type == 5 then -- 神秘矿洞
		SetText( m_uiDesc, T(1017) )
	end
	
	local buttonIcon = m_uiCostButton.transform:Find("Back/Layout/Icon")
	local buttonText = m_uiCostButton.transform:Find("Back/Layout/Text")
	if info.costtype > 0 then
		SetTrue( buttonIcon )
		SetImage( buttonIcon, ResIcon(info.costtype) );
		SetText( buttonText, info.costnum )
	else
		SetFalse( buttonIcon )
		SetText( buttonText, T(976) )
	end
end

-- 采集
function MapEventDlgGather()
	if m_recvValue == nil then
		return
	end
	local type		= m_recvValue.m_char_value[1];
	local kind		= m_recvValue.m_short_value[1];
	local info 		= g_mapeventinfo[kind]
	if info == nil then
		return
	end
	if info.costtype > 0 then
		if info.costtype == 1 then
			if GetPlayer().m_silver < info.costnum then
				JumpRes( 1,info.costnum-GetPlayer().m_silver )
				return
			end
		elseif info.costtype == 2 then
			if GetPlayer().m_wood < info.costnum then
				JumpRes( 2,info.costnum-GetPlayer().m_wood )
				return
			end
		elseif info.costtype == 3 then
			if GetPlayer().m_food < info.costnum then
				JumpRes( 3,info.costnum-GetPlayer().m_food )
				return
			end
		elseif info.costtype == 4 then
			if GetPlayer().m_iron < info.costnum then
				JumpRes( 4,info.costnum-GetPlayer().m_iron )
				return
			end
		elseif info.costtype == 5 then
			if GetPlayer().m_token < info.costnum then
				JumpToken()
				return
			end
		end
	end
	
	system_askinfo( ASKINFO_MAP_EVENT, "", 1, m_recvValue.m_unit_index );
	MapEventDlgClose()
end

-- 领取
function MapEventDlgGetAward()
	if m_recvValue == nil then
		return
	end
	system_askinfo( ASKINFO_MAP_EVENT, "", 2, m_recvValue.m_unit_index );
	MapEventDlgClose()
end
