-- 界面
local m_Dlg = nil;

local m_Template;       -- GameObject;
local m_List;
local m_Data = {};

-- 打开界面
function GainDlgOpen()
	m_Dlg = eye.uiManager:Open( "GainDlg" );
end

-- 隐藏界面
function GainDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "GainDlg" );
end

-- 删除界面
function GainDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function GainDlgOnEvent( nType, nControlID, value, gameObject )

end

-- 载入时调用
function GainDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;
    m_Template = objs[0];
    m_List = objs[1].transform;
end

-- 界面初始化时调用
function GainDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function GainDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function GainDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function GainDlgOnDestroy( gameObject )
	m_Dlg = nil;

    m_Data = {};
end

-- 每帧调用
function GainDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
-- type: 0 资源， 1 道具
function GainDlgAdd( type, kind, num )
    GainDlgOpen();

    -- 先获得一个空的
    local activeCount = 0;
    local trans = nil;
    for i = 0, m_List.childCount - 1, 1 do
        local obj = m_List:GetChild( i ).gameObject;
        if obj.activeSelf == false then
            trans = obj.transform;
        else
            activeCount = activeCount + 1;
        end
    end

    -- 多余缓存
    if activeCount >= 6 then
        table.insert( m_Data, { type = type, kind = kind, num = num } );
        return;
    end

    if trans == nil then
        trans = GameObject.Instantiate( m_Template ).transform;
        trans:SetParent( m_List );
        trans.localScale = Vector3.one;
    end

    local back = trans:FindChild( "Back" ).gameObject;
    local tween = trans:FindChild( "Item" ):GetComponent( UITweenPosition.GetClassType() );
    local icon = trans:FindChild( "Item" ):FindChild( "Icon" ):GetComponent( Image.GetClassType() );
    local count = trans:FindChild( "Item" ):FindChild( "Count" ):GetComponent( UIText.GetClassType() );
    local mask = trans:FindChild( "Item" ):FindChild( "Mask" ):GetComponent( Image.GetClassType() );
    local iconvalue = trans:FindChild( "Item" ):FindChild( "IconValue" ):GetComponent( UIText.GetClassType() );

    -- 根据controlID来判断是哪个结束了
    tween.controlID = trans:GetSiblingIndex();

    -- 还原
    icon.transform.localScale = Vector3.one;
    tween.transform.localPosition = Vector3.zero;
    if tween == nil then
        trans.gameObject:SetActive( false );
        return;
    end

    if type == 0 then
        -- 资源
        back:SetActive( true );

        -- 隐藏多余
        mask.gameObject:SetActive( false );
        iconvalue.gameObject:SetActive( false );

        -- 图标
        icon.sprite = GetResIconItem( kind );

        -- 数量
        count.text = "+" .. string.formatnumberthousands( num, kind == 101 );
    
        -- 隐藏
        Invoke( function() GainDlgOnFinish( trans ); end, 1.8 );

        -- 位移
        Invoke( function()
                    tween.from = tween.transform.position;
                    local target = MainDlgGetResIcon( kind );
                    -- 容错
                    if target ~= nil then
                        tween.to = target.position;
                    else
                        tween.to = tween.from + Vector3.New( 0, 10, 0 );
                    end
                    tween:Play( true );                
                end, 1.4 );
        
    elseif type == 1 then
        -- 道具
        back:SetActive( false );
        mask.gameObject:SetActive( true );

        -- 设置图标
        Item_SetItem( trans:FindChild( "Item" ).gameObject, kind, num );
        
        -- 加上+
        count.text = "+" .. count.text;

        -- 隐藏
        Invoke( function() GainDlgOnFinish( trans ); end, 1.8 );

        Invoke( function()
                    tween.from = tween.transform.position;
                    local target = MainDlgGetBagButton();
                    -- 容错
                    if target ~= nil then
                        tween.to = target.position;
                    else
                        tween.to = tween.from + Vector3.New( 0, 10, 0 );
                    end
                    tween:Play( true );                
                end, 1.4 );

    end

    -- 走起
    trans.gameObject:SetActive( true );
end

-- 动画播完
function GainDlgOnFinish( trans )
    trans.gameObject:SetActive( false );

    if #m_Data == 0 then
        local allClose = true;
        for i = 0, m_List.childCount - 1, 1 do
            if m_List:GetChild( i ).gameObject.activeSelf == true then
                allClose = false;
                break;
            end
        end

        if allClose then
            GainDlgClose();
        end
    else
        GainDlgAdd( m_Data[1].type, m_Data[1].kind, m_Data[1].num );
        table.remove( m_Data, 1 );
    end
end