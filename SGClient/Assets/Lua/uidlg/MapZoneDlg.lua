-- 界面
local m_Dlg = nil;
local m_DialogFrameMod = nil;

local m_uiZoneName = nil; --UnityEngine.GameObject
local m_uiNation = nil; --UnityEngine.GameObject
local m_uiShowBtn = nil; --UnityEngine.GameObject
local m_uiHideBtn = nil; --UnityEngine.GameObject
local m_uiGlobalBtn = nil; --UnityEngine.GameObject
local m_uiWarn = nil; --UnityEngine.GameObject

-- 打开界面
function MapZoneDlgOpen()
	m_Dlg = eye.uiManager:Open( "MapZoneDlg" );
	m_DialogFrameMod = DialogFrameModOpen( m_Dlg, T(1003), 5, MapZoneDlgClose );
end

-- 隐藏界面
function MapZoneDlgClose()
	if m_Dlg == nil then
		return;
	end
	-- 删除缩略图
	WorldMapThumb.Delete();
	DialogFrameModClose( m_DialogFrameMod );
	m_DialogFrameMod = nil;
	eye.uiManager:Close( "MapZoneDlg" );
end

-- 删除界面
function MapZoneDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function MapZoneDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            MapZoneDlgClose();
			
		-- 世界地图
		elseif nControlID == 1 then
			if GetPlayer().m_open_town3 > 0 then
				MapZoneDlgClose()
				MapGlobalDlgShow()
			else
				WorldMapThumb.Alert( T(2367) )
			end
			
		-- 显示玩家
		elseif nControlID == 10 then
		
		-- 隐藏玩家
		elseif nControlID == 11 then
        end
		
	elseif nType == UI_EVENT_CLICKPOS then
		if nControlID == 1 then
			-- 点击地图位置
			MapZoneDlgOnClick( value )
		end
	end
end

-- 载入时调用
function MapZoneDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiZoneName = objs[0];
	m_uiNation = objs[1];
	m_uiShowBtn = objs[2];
	m_uiHideBtn = objs[3];
	m_uiGlobalBtn = objs[4];
	m_uiWarn = objs[5];
end

-- 界面初始化时调用
function MapZoneDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function MapZoneDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function MapZoneDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function MapZoneDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function MapZoneDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function MapZoneDlgShow( zoneid )
	MapZoneDlgOpen()
	if zoneid == nil then
		zoneid = WorldMap.m_nZoneID;
	end
	local name = MapZoneName( zoneid )
	SetText( m_uiZoneName.transform:Find("Text"), name )
	
	-- 显示缩略图
	WorldMapThumb.Create( zoneid );
	
	-- 郡城被攻克
	if GetPlayer().m_open_town3 > 0 then
		SetGray( m_uiGlobalBtn.transform:Find("Back"), false )
		SetText( m_uiGlobalBtn.transform:Find("Back/Text"), T(1004) )
	else
		SetGray( m_uiGlobalBtn.transform:Find("Back"), true )
		SetText( m_uiGlobalBtn.transform:Find("Back/Text"), T(601) )
	end
end
-- 设置所属国家
function MapZoneDlgSetNation( nation )
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return
	end
	if nation == 0 then
		SetFalse( m_uiNation )
		SetTrue( m_uiWarn )
		SetText( m_uiWarn, T(2368) )
	else
		SetTrue( m_uiNation )
		SetFalse( m_uiWarn )
		SetImage( m_uiNation.transform:Find("Icon"), NationSprite(nation) )
	end
end
