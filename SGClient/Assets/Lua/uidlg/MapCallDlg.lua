-- 界面
local m_Dlg = nil;
local m_uiShape = nil; --UnityEngine.GameObject
local m_uiColor = nil; --UnityEngine.GameObject
local m_uiNum = nil; --UnityEngine.GameObject
local m_uiDesc = nil; --UnityEngine.GameObject
local m_uiCost = nil; --UnityEngine.GameObject
local m_uiHas = nil; --UnityEngine.GameObject
local m_uiLimitLevelEdit = nil; --UnityEngine.GameObject
local m_uiTodayNum = nil; --UnityEngine.GameObject

-- 打开界面
function MapCallDlgOpen()
	m_Dlg = eye.uiManager:Open( "MapCallDlg" );

end

-- 隐藏界面
function MapCallDlgClose()
	if m_Dlg == nil then
		return;
	end

	eye.uiManager:Close( "MapCallDlg" );
end

-- 删除界面
function MapCallDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function MapCallDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            MapCallDlgClose();
		elseif nControlID == 1 then
			MapCallDlgSend();
        end
	
	elseif nType == UI_EVENT_INPUTVALUECHANGED then
		if nControlID == 0 then
			local text = m_uiLimitLevelEdit.transform:Find("Input"):GetComponent( "UIInputField" ).text;
			if text == "" then
				SetTrue( m_uiLimitLevelEdit.transform:Find("Input/Hint") )
			else
				SetFalse( m_uiLimitLevelEdit.transform:Find("Input/Hint") )
			end
		end
	end
end

-- 载入时调用
function MapCallDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiShape = objs[0];
	m_uiColor = objs[1];
	m_uiNum = objs[2];
	m_uiDesc = objs[3];
	m_uiCost = objs[4];
	m_uiHas = objs[5];
	m_uiLimitLevelEdit = objs[6];
	m_uiTodayNum = objs[7];
end

-- 界面初始化时调用
function MapCallDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function MapCallDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function MapCallDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function MapCallDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function MapCallDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function MapCallDlgShow()
	MapCallDlgOpen()
	system_askinfo( ASKINFO_WORLDMAP, "", 14 );
end

-- 接收
function MapCallDlgRecv( actornum, todaynum, freenum )
	local itemkind = 131
	local itemnum = GetItem():GetCount(itemkind)
	SetImage( m_uiShape, ItemSprite( itemkind ) )
	SetImage( m_uiColor, ItemColorSprite( item_getcolor(itemkind) ) )
	SetText( m_uiNum, "x"..itemnum )
	SetText( m_uiDesc, F( 1860, actornum ) )
	if freenum > 0 then
		SetText( m_uiCost, F( 1865, freenum ) )
		SetFalse( m_uiHas )
	else
		SetText( m_uiCost, F( 1861, 4 ) )
		SetTrue( m_uiHas )
		SetText( m_uiHas, F( 1862, itemnum ) )
	end
	
	SetText( m_uiTodayNum, F( 1864, todaynum ) );
end

function MapCallDlgSend()
	local level = m_uiLimitLevelEdit.transform:Find("Input"):GetComponent( "UIInputField" ).text;
	if level == "" then
		level = 1;
	end
	m_uiLimitLevelEdit.transform:Find("Input"):GetComponent( "UIInputField" ).text = "";
	system_askinfo( ASKINFO_WORLDMAP, "", 11, level );
	MapCallDlgClose()
end