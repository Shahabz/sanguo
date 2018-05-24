-- 消息通知界面
local m_Dlg = nil;

local m_PopWindow_T;      -- UIPopWindow;
local m_PopWindow_M;      -- UIPopWindow;

-- 消息缓存
local m_Data_T = {};
local m_Data_M = {};

-- 打开界面
function AwardNotifyDlgOpen()
	m_Dlg = eye.uiManager:Open( "AwardNotifyDlg" );
end

-- 隐藏界面
function AwardNotifyDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "AwardNotifyDlg" );
end

-- 删除界面
function AwardNotifyDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end



----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function AwardNotifyDlgOnEvent( nType, nControlID, value )
    if nType == UI_EVENT_PUSHEVENT then
        AwardNotifyNext( nControlID );
	end
end

-- 载入时调用
function AwardNotifyDlgOnAwake( gameObject )	
	-- 控件赋值	
    local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
    m_PopWindow_T = objs[0]:GetComponent( "UIPopWindow" );
    m_PopWindow_M = objs[1]:GetComponent( "UIPopWindow" );
end

-- 界面初始化时调用
function AwardNotifyDlgOnStart()
end

-- 界面显示时调用
function AwardNotifyDlgOnEnable()
	
end

-- 界面隐藏时调用
function AwardNotifyDlgOnDisable()
	
end

-- 界面删除时调用
function AwardNotifyDlgOnDestroy()
	m_Dlg = nil;
end

-- 每帧调用
function AwardNotifyDlgOnLogic()
	
end


----------------------------------------
-- 自定
----------------------------------------
-- 弹出消息
local function AwardNotifyAdd( kind, num, popWindow, data )
    local window = popWindow:Pop();
    if window == nil then
        table.insert( data, { kind = kind, num = num } );
    else
		if kind == AWARDKIND_SILVER then
		elseif kind == AWARDKIND_WOOD then
		elseif kind == AWARDKIND_FOOD then
		elseif kind == AWARDKIND_IRON then
		elseif kind == AWARDKIND_TOKEN then
		end
		local sprite, color, name, c, desc = AwardInfo( kind );
		SetImage(window.transform:Find("Shape"),sprite)
		SetImage(window.transform:Find("Color"),color)
		SetText(window.transform:Find("Name"),name,NameColor(c));
		SetText(window.transform:Find("Num"),"+"..num);
		
		
    end
end

-- 通用
function AwardNotify( kind, num )
    AwardNotifyDlgOpen();

	AwardNotifyAdd( kind, num, m_PopWindow_T, m_Data_T );
	--AwardNotifyAdd( kind, num, m_PopWindow_M, m_Data_M );
end

-- 中间
function AwardNotifyMiddle( kind, num )
    AwardNotifyDlgOpen();

	AwardNotifyAdd( kind, num, m_PopWindow_M, m_Data_M );
end

-- 上面
function AwardNotifyTop( kind, num )
    AwardNotifyDlgOpen();

	AwardNotifyAdd( kind, num, m_PopWindow_T, m_Data_T );
end

-- 下一条
function AwardNotifyNext( mode )
    local data;
    local popWindow;
    if mode == 0 then
        data = m_Data_T;
        popWindow = m_PopWindow_T;
    elseif mode == 1 then
        data = m_Data_M;
        popWindow = m_PopWindow_M;
    elseif mode == 2 then
        data = m_Data_B;
        popWindow = m_PopWindow_B;
    end

    if #data > 0 then
        -- 显示下一条
        AwardNotifyAdd( data[1].kind, data[1].num, popWindow, data );
        table.remove( data, 1 );

        -- 如果缓存还有，手动加速
        if #data > 0 then
            popWindow:Pop();
        end
    else
        -- 没有显示的界面了，关闭
        if m_PopWindow_T.currCount == 0 and m_PopWindow_M.currCount == 0 and m_PopWindow_B.currCount == 0 then
            AwardNotifyDlgClose();
        end
    end
end
