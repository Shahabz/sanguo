-- 界面
local m_Dlg = nil;
local m_uiTitle = nil; --UnityEngine.GameObject
local m_uiTimer = nil; --UnityEngine.GameObject
local m_uiAward1 = nil; --UnityEngine.GameObject
local m_uiAward2 = nil; --UnityEngine.GameObject
local m_uiAward3 = nil; --UnityEngine.GameObject
local m_uiAward4 = nil; --UnityEngine.GameObject
local m_uiAward5 = nil; --UnityEngine.GameObject
local m_uiAward6 = nil; --UnityEngine.GameObject
local m_uiExitButton = nil; --UnityEngine.GameObject
local m_uiCongratulateButton = nil; --UnityEngine.GameObject

local tableData = {}
	
-- 打开界面
function TreasureAwardDlgOpen()
	ResourceManager.LoadAssetBundle( "_ab_ui_static_pic_3" )
	ResourceManager.LoadAssetBundle( "_ab_ui_static_pic_4" )
	m_Dlg = eye.uiManager:Open( "TreasureAwardDlg" );
end

-- 隐藏界面
function TreasureAwardDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "TreasureAwardDlg" );
end

-- 删除界面
function TreasureAwardDlgDestroy()
	GameObject.Destroy( m_Dlg );
	ResourceManager.UnloadAssetBundle( "_ab_ui_static_pic_3" )
	ResourceManager.UnloadAssetBundle( "_ab_ui_static_pic_4" )
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function TreasureAwardDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            TreasureAwardDlgClose();
		elseif nControlID == 1 then
			TreasureAwardDlgCongratulate()
        end
	elseif nType == UI_EVENT_TIMECOUNTEND then
		if nControlID == 1 then
			TreasureAwardDlgClose()
		end
	end
end

-- 载入时调用
function TreasureAwardDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiTitle = objs[0];
	m_uiTimer = objs[1];
	m_uiAward1 = objs[2];
	m_uiAward2 = objs[3];
	m_uiAward3 = objs[4];
	m_uiAward4 = objs[5];
	m_uiAward5 = objs[6];
	m_uiAward6 = objs[7];
	m_uiExitButton = objs[8];
	m_uiCongratulateButton = objs[9];
end

-- 界面初始化时调用
function TreasureAwardDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function TreasureAwardDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function TreasureAwardDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function TreasureAwardDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function TreasureAwardDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function TreasureAwardDlgShow()
	TreasureAwardDlgOpen()
	SetButtonFalse( m_uiCongratulateButton )
	
	-- 标题
	SetText( m_uiTitle, F( 1682, NationEx( GetPlayer().m_nation ) ) )
	-- 活动倒计时
	local leftstamp = MapMainDlgActivityTreasureEndStamp()-GetServerTime();
	SetTimer( m_uiTimer, leftstamp, leftstamp, 1, T(1680) )
	
	-- 获取列表
	system_askinfo( ASKINFO_KINGWAR, "", 16 );
	
	tableData = { 
	[44]={ uiObj = m_uiAward1, num = 0 }, 
	[48]={ uiObj = m_uiAward2, num = 0 }, 
	[52]={ uiObj = m_uiAward3, num = 0 }, 
	[56]={ uiObj = m_uiAward4, num = 0 }, 
	[60]={ uiObj = m_uiAward5, num = 0 }, 
	[64]={ uiObj = m_uiAward6, num = 0 },
	}
	
	-- 道具信息
	for k, v in pairs( tableData ) do
		SetImage( v.uiObj.transform:Find("Shape"), ItemSprite( k ) );
		SetImage( v.uiObj.transform:Find("Color"), ItemColorSprite( item_getcolor(k) ) );
		SetText( v.uiObj.transform:Find("EquipName"), item_getname(k) );
		SetText( v.uiObj.transform:Find("Num"),"" );
	end
	
end

-- m_count=0,m_list={m_itemkind=0,m_name_len=0,m_name="[m_name_len]",[m_count]},m_co=0,
function TreasureAwardDlgRecv( recvValue )
	
	if recvValue.m_co == 1 then
		SetButtonFalse( m_uiCongratulateButton )
	else
		SetButtonTrue( m_uiCongratulateButton )
	end
	for i=1, recvValue.m_count, 1 do
		local itemkind = recvValue.m_list[i].m_itemkind;
		if tableData[itemkind] ~= nil then
			tableData[itemkind].num = tableData[itemkind].num + 1;
			if tableData[itemkind].num == 1 then
				SetText( tableData[itemkind].uiObj.transform:Find("Name1"), recvValue.m_list[i].m_name )
			elseif tableData[itemkind].num == 2 then
				SetText( tableData[itemkind].uiObj.transform:Find("Name2"), recvValue.m_list[i].m_name )
			end
		end
	end
	
	-- 设置数量
	for k, v in pairs( tableData ) do
		if v.num > 0 then
			SetText( v.uiObj.transform:Find("Num"),"x"..v.num );
		end
	end
	
end

-- 恭贺
function TreasureAwardDlgCongratulate()
	system_askinfo( ASKINFO_KINGWAR, "", 17 );
	SetButtonFalse( m_uiCongratulateButton )
end
