-- 界面
local m_Dlg = nil;

local m_List;           -- Transform
local m_Item;           -- GameObject
local m_Tweens;         -- UITween

-- 数据
local m_ShowTimer = 0;
local m_ItemList = {};
local m_ItemData = {};

-- 打开界面
function AwardGainDlgOpen()
	m_Dlg = eye.uiManager:Open( "AwardGainDlg" );
end

-- 隐藏界面
function AwardGainDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "AwardGainDlg" );
end

-- 删除界面
function AwardGainDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function AwardGainDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            AwardGainDlgClose();
        end
	elseif nType == UI_EVENT_TWEENFINISH then
		if nControlID == -1 then
			AwardGainDlgClose();
		end
	end
end

-- 载入时调用
function AwardGainDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;

    m_Item = objs[0];
    m_List = objs[1].transform;
    m_Tweens = m_List.parent:GetComponents( typeof(UITween) );
end

-- 界面初始化时调用
function AwardGainDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function AwardGainDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function AwardGainDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function AwardGainDlgOnDestroy( gameObject )
	m_Dlg = nil;
    m_ItemList = {};
    m_ItemData = {};
end

-- 每帧调用
function AwardGainDlgOnLogic( gameObject )
	if m_ShowTimer > 0 then        
        m_ShowTimer = m_ShowTimer - UnityEngine.Time.deltaTime;
        if m_ShowTimer <= 0 then
            AwardGainDlgShow();
        end
    end
end


----------------------------------------
-- 自定
----------------------------------------
-- 打开
function AwardGainDlgShow()
    for i = 1, #m_ItemData, 1 do
        local obj;
        if m_ItemList[i] == nil then
            obj = GameObject.Instantiate( m_Item );
            obj.transform:SetParent( m_List );
            obj.transform.localScale = Vector3.one;
            
            m_ItemList[i] = obj;
        else
            obj = m_ItemList[i];
        end

        obj:SetActive( true );
		
		local kind = m_ItemData[i].kind
		local num = m_ItemData[i].num
		local sprite, color, name, c, desc = AwardInfo( kind );
		SetImage(obj.transform:Find("Shape"),sprite)
		SetImage(obj.transform:Find("Color"),color)
		SetText(obj.transform:Find("Name"),name,NameColor(c));
		SetText(obj.transform:Find("Count"),"+"..num);
    end

    -- 隐藏多余
    for i = #m_ItemData + 1, #m_ItemList, 1 do
        m_ItemList[i]:SetActive( false );
    end

    -- 设置高度
    local size = m_List.parent.sizeDelta;
    m_List.parent.sizeDelta = Vector2.New( size.x, 120 + ( #m_ItemData - 1 ) * 60 );

    -- 播放
    m_Tweens[0]:Play( true );
    m_Tweens[1]:Play( true );

    m_ItemData = {};
end

--
function AwardGainDlgGain( kind, num )
    m_ShowTimer = 0.5;
    AwardGainDlgOpen();

    m_Tweens[0]:ToInit();
    
    table.insert( m_ItemData, { kind = kind, num = num } );
end