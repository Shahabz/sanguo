-- 界面
local m_Dlg = nil;
local m_uiAwardInfo = nil; --UnityEngine.GameObject
local m_uiPoint = nil; --UnityEngine.GameObject

local m_point = 0;

-- 打开界面
function KingWarChangeDlgOpen()
	ResourceManager.LoadAssetBundle( "_ab_ui_static_pic_3" )
	ResourceManager.LoadAssetBundle( "_ab_ui_static_pic_4" )
	m_Dlg = eye.uiManager:Open( "KingWarChangeDlg" );
end

-- 隐藏界面
function KingWarChangeDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "KingWarChangeDlg" );
end

-- 删除界面
function KingWarChangeDlgDestroy()
	GameObject.Destroy( m_Dlg );
	ResourceManager.UnloadAssetBundle( "_ab_ui_static_pic_3" )
	ResourceManager.UnloadAssetBundle( "_ab_ui_static_pic_4" )
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function KingWarChangeDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            KingWarChangeDlgClose();
		elseif nControlID >= 1 and nControlID <= 6 then
			KingWarChangeDlgSelect( nControlID )
        end
	end
end

-- 载入时调用
function KingWarChangeDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiAwardInfo = objs[0];
	m_uiPoint = objs[1];
end

-- 界面初始化时调用
function KingWarChangeDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function KingWarChangeDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function KingWarChangeDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function KingWarChangeDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function KingWarChangeDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function KingWarChangeDlgShow()
	KingWarChangeDlgOpen()
	system_askinfo( ASKINFO_KINGWAR, "", 11 );
	for i = 0 ,m_uiAwardInfo.transform.childCount - 1 do
		local uiObj = m_uiAwardInfo.transform:GetChild(i).gameObject;
		local itemkind = g_kingwar_config[i+1].exchange_item;
		local point = g_kingwar_config[i+1].exchange_point;
		SetImage( uiObj.transform:Find("Shape"), ItemSprite(itemkind) )
		SetImage( uiObj.transform:Find("Color"), ItemColorSprite( item_getcolor(itemkind) ) )
		SetText( uiObj.transform:Find("Name"), item_getname( itemkind ), NameColor( item_getcolor(itemkind) ) )
		SetText( uiObj.transform:Find("ChangeBtn/Back/Text"), point..T(1908) )
		SetControlID( uiObj.transform:Find("ChangeBtn"), i+1 )
    end
end

-- m_point=0,
function KingWarChangeDlgRecv( recvValue )
	m_point = recvValue.m_point
	SetText( m_uiPoint, T(1915)..":"..m_point )
end

function KingWarChangeDlgSelect( index )
	local itemkind = g_kingwar_config[index].exchange_item;
	local point = g_kingwar_config[index].exchange_point;
	if m_point < point then
		AlertMsg( T(1918) )
		return
	end
	MsgBox( F(1919, point, item_getname(itemkind) ), function()
		system_askinfo( ASKINFO_KINGWAR, "", 12, index, itemkind );
	end )
end
