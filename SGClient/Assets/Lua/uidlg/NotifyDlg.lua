-- 消息通知界面
local m_Dlg = nil;

local m_PopWindow_T;      -- UIPopWindow;
local m_PopWindow_M;      -- UIPopWindow;
local m_PopWindow_B;      -- UIPopWindow;

-- 消息缓存
local m_Data_T = {};
local m_Data_M = {};
local m_Data_B = {};
-- 打开界面
function NotifyDlgOpen()
	m_Dlg = eye.uiManager:Open( "NotifyDlg" );
end

-- 隐藏界面
function NotifyDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "NotifyDlg" );
end

-- 删除界面
function NotifyDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end



----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function NotifyDlgOnEvent( nType, nControlID, value )
    if nType == UI_EVENT_PUSHEVENT then
        NotifyNext( nControlID );
	end
end

-- 载入时调用
function NotifyDlgOnAwake( gameObject )	
	-- 控件赋值	
    local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
    m_PopWindow_T = objs[0]:GetComponent( "UIPopWindow" );
    m_PopWindow_M = objs[1]:GetComponent( "UIPopWindow" );
    m_PopWindow_B = objs[2]:GetComponent( "UIPopWindow" );
end

-- 界面初始化时调用
function NotifyDlgOnStart()
end

-- 界面显示时调用
function NotifyDlgOnEnable()
	
end

-- 界面隐藏时调用
function NotifyDlgOnDisable()
	
end

-- 界面删除时调用
function NotifyDlgOnDestroy()
	m_Dlg = nil;
end

-- 每帧调用
function NotifyDlgOnLogic()
	
end


----------------------------------------
-- 自定
----------------------------------------
-- 弹出消息
local function NotifyAdd( msg, popWindow, data, param )
    local window = popWindow:Pop();
    if window == nil then
        table.insert( data, { msg = msg, param = param } );
    else
        -- 字
        local text = window:Find( "Text" ):GetComponent( typeof(UIText) );
		text.text = msg;
        -- 形象
        local shape = window:Find( "Icon" ):Find( "Shape" ):GetComponent( typeof(Image) )
        -- 背景
        local back = window:Find( "Icon" ):Find( "Back" ):GetComponent( typeof(Image) );
		
        -- 直接发文字, 不带图标     
        if param == nil then
            text.transform.anchoredPosition = Vector2.New( 0, 0 );
            shape.gameObject:SetActive( false );
            back.gameObject:SetActive( false );
        else        
            -- back
            if param.back == nil then
                back.gameObject:SetActive( false );
            else
                back.gameObject:SetActive( true );
				back.sprite = param.back;
            end

            -- shape
            if param.shape == nil then
				text.transform.anchoredPosition = Vector2.New( 0, 0 ); -- 矫正下字的位置
                shape.gameObject:SetActive( false );
            else
				text.transform.anchoredPosition = Vector2.New( 64, 0 ); -- 矫正下字的位置
                shape.gameObject:SetActive( true );
                shape.sprite = param.shape;
                --shape:SetNativeSize();
            end
        end
    end
end

-- 通用
function Notify( pos, msg, param )
    NotifyDlgOpen();

    -- 看位置
    if pos == 0 then
	    NotifyAdd( msg, m_PopWindow_T, m_Data_T, param );
    elseif pos == 1 then
	    NotifyAdd( msg, m_PopWindow_M, m_Data_M, param );
    elseif pos == 2 then
	    NotifyAdd( msg, m_PopWindow_B, m_Data_B, param );
    end
end

-- 中间
function NotifyMiddle( msg, param )
    NotifyDlgOpen();

	NotifyAdd( msg, m_PopWindow_M, m_Data_M, param );
end

-- 上面
function NotifyTop( msg, param )
    NotifyDlgOpen();

	NotifyAdd( msg, m_PopWindow_T, m_Data_T, param );
end

-- 下面
function NotifyBottom( msg, param )
    NotifyDlgOpen();

	NotifyAdd( msg, m_PopWindow_B, m_Data_B, param );
end

-- 下一条
function NotifyNext( mode )
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
        NotifyAdd( data[1].msg, popWindow, data, data[1].param );
        table.remove( data, 1 );

        -- 如果缓存还有，手动加速
        if #data > 0 then
            popWindow:Pop();
        end
    else
        -- 没有显示的界面了，关闭
        if m_PopWindow_T.currCount == 0 and m_PopWindow_M.currCount == 0 and m_PopWindow_B.currCount == 0 then
            NotifyDlgClose();
        end
    end
end

-- 重置
function NotifyReset()
    
end