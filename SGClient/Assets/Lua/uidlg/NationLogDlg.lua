-- 界面
local m_Dlg = nil;
local m_DialogFrameMod = nil;

local m_uiScrollView = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_IsShowLoadingItem = nil
local m_PageDirection = 0;
local m_page = 0;
NATION_LOG_TYPE_DEFENSE_LOSE	=1
NATION_LOG_TYPE_DEFENSE_WIN		=2
NATION_LOG_TYPE_REBUILD			=3
NATION_LOG_TYPE_LEAVE			=4
NATION_LOG_TYPE_ATTACK_GIVEUP	=5
NATION_LOG_TYPE_ATTACK_WIN		=6

-- 打开界面
function NationLogDlgOpen()
	m_Dlg = eye.uiManager:Open( "NationLogDlg" );
	m_DialogFrameMod = DialogFrameModOpen( m_Dlg, T(1538), HELP_NationDlg, NationLogDlgClose );
end

-- 隐藏界面
function NationLogDlgClose()
	if m_Dlg == nil then
		return;
	end
	DialogFrameModClose( m_DialogFrameMod );
	m_DialogFrameMod = nil;
	eye.uiManager:Close( "NationLogDlg" );
end

-- 删除界面
function NationLogDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function NationLogDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            NationLogDlgClose();
        end
	elseif nType == UI_EVENT_SCROLLDRAG then
	
		if value == 0 then
            -- 判断列表是否允许滑动
            m_IsShowLoadingItem = m_uiScrollView:GetComponent("UIScrollRect"):IsShowLoading();
		
		-- 滚动条超过一定的范围,这个时候访问数据,读取更多数据	
        elseif value == 1 and m_IsShowLoadingItem == false then
            local vn = m_uiScrollView:GetComponent("UIScrollRect").verticalNormalizedPosition;
            local rectTrans = m_uiContent:GetComponent("RectTransform");
            local scrollHeight = m_uiContent:GetComponent("RectTransform").rect.height;
            local maxHeight =(math.max(scrollHeight, rectTrans.rect.height) - rectTrans.offsetMax.y);
            if maxHeight < 0.9 * scrollHeight then
                -- 增加一个提示用户读取数据的 一行
                --m_uiScrollView:GetComponent("UIScrollRect"):ShowLoading(false, 2.5);
                m_IsShowLoadingItem = true;
				m_PageDirection = 1; -- 下一页
				
			elseif vn >= 1 and math.abs(rectTrans.offsetMax.y) > 0.1 * scrollHeight then
                -- 增加一个提示用户读取数据的 一行
                --m_uiScrollView:GetComponent("UIScrollRect"):ShowLoading(false, 2.5);
                m_IsShowLoadingItem = true;
				m_PageDirection = 2; -- 上一页
            end
         
        -- 拖动放开
        elseif value == 2 then
            m_uiScrollView:GetComponent("UIScrollRect").enableDrag = true;                  
			m_uiScrollView:GetComponent("UIScrollRect").enabled = false;
			m_uiScrollView:GetComponent("UIScrollRect").enabled = true;
			-- 加载更多数据
			
			-- 下一页
			if m_PageDirection == 1 then
				NationLogDlgNext();
				
			-- 上一页
			elseif m_PageDirection == 2 then 
				NationLogDlgPre();
			end
        end
	end
end

-- 载入时调用
function NationLogDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiScrollView = objs[0];
	m_uiContent = objs[1];
end

-- 界面初始化时调用
function NationLogDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function NationLogDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function NationLogDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function NationLogDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function NationLogDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function NationLogDlgShow()
	NationLogDlgOpen()
	for i=0, 9, 1 do
		SetFalse( m_uiContent.transform:GetChild(i) )
	end
	m_page = 0;
	system_askinfo( ASKINFO_NATIONLOG, "", 0 )
end

-- m_count=0,m_list={m_type=0,m_townid=0,m_namelen=0,m_name="[m_namelen]",m_target_nation=0,m_optime=0,[m_count]},
function NationLogDlgRecv( recvValue )
	if recvValue.m_count == 0 then
		if m_page > 0 then
			m_page = m_page - 1;
		end
		pop( T(1881) )
		return
	end
	for i=1, 10, 1 do
		local uiObj =  m_uiContent.transform:GetChild(i-1)
		if i <= recvValue.m_count then
			SetTrue( uiObj )
			NationLogDlgCreate( uiObj, recvValue.m_list[i] )
		else
			SetFalse( uiObj )
		end
	end
end

-- 创建
function NationLogDlgCreate( uiObj, info )
	SetText( uiObj.transform:Find( "Time" ), os.date( "%m-%d %H:%M", info.m_optime ) )
	
	local posx = g_towninfo[info.m_townid].posx
	local posy = g_towninfo[info.m_townid].posy
	local pos = posx..","..posy
	local townname = MapTownName( info.m_townid )
	local zoneid = g_towninfo[info.m_townid].zoneid
	local zonename = MapZoneName( zoneid )
	
	if info.m_type == NATION_LOG_TYPE_DEFENSE_LOSE then
		SetText(  uiObj.transform:Find( "Text" ), F( 1870, zonename, townname, pos, NationEx( info.m_target_nation ), info.m_name ) )
		
	elseif info.m_type == NATION_LOG_TYPE_DEFENSE_WIN then
		SetText(  uiObj.transform:Find( "Text" ), F( 1871, zonename, townname, pos, NationEx( info.m_target_nation ), info.m_name ) )	
		
	elseif info.m_type == NATION_LOG_TYPE_REBUILD then
		SetText(  uiObj.transform:Find( "Text" ), F( 1872, info.m_name, zonename, townname, pos ) )	
		
	elseif info.m_type == NATION_LOG_TYPE_LEAVE then
		SetText(  uiObj.transform:Find( "Text" ), F( 1873, info.m_name, zonename, townname, pos ) )	
		
	elseif info.m_type == NATION_LOG_TYPE_ATTACK_GIVEUP then
		SetText(  uiObj.transform:Find( "Text" ), F( 1874, zonename, townname, pos, info.m_name ) )
		
	elseif info.m_type == NATION_LOG_TYPE_ATTACK_WIN then
		SetText(  uiObj.transform:Find( "Text" ), F( 1875, zonename, townname, pos, info.m_name ) )
	end
end

-- 上一页
function NationLogDlgPre()
	m_page = m_page - 1;
	if m_page < 0 then
		m_page = 0;
		return
	end
	system_askinfo( ASKINFO_NATIONLOG, "", m_page )
end

-- 下一页
function NationLogDlgNext()
	m_page = m_page + 1;
	system_askinfo( ASKINFO_NATIONLOG, "", m_page )
end