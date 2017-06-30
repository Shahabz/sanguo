local m_Mod;

local m_uiOprator = nil; --UnityEngine.GameObject
local m_uiUpgrade = nil; --UnityEngine.GameObject
local m_uiEnter = nil; --UnityEngine.GameObject
local m_uiTrain = nil; --UnityEngine.GameObject
local m_uiSpeed = nil; --UnityEngine.GameObject

local m_kind = 0;
local m_offset = -1;

local m_Closing = false;

function BuildingOpratorModOpen()
    BuildingOpratorModInit();
    m_Mod:SetActive( true );
end

function BuildingOpratorModClose()
    m_Mod:SetActive( false );
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function BuildingOpratorModOnEvent( nType, nControlID, value )
    if nType == UI_EVENT_CLICK then
        if nControlID == 1 then
            -- 升级
			BuildingUpgradeDlgShow( m_kind, m_offset )
            City.BuildingUnSelect();
        elseif nControlID == 2 then
            -- 升级
            --BuildingDlgOpenByIndex( m_offset );
            --BuildingUnSelect();
        end
    elseif nType == UI_EVENT_TWEENFINISH then
        --if value == 1 then
            --m_Mod:SetActive( false );
        --end
    elseif nType == UI_EVENT_TIMECOUNTEND then

    end
end

-- 载入时调用
function BuildingOpratorModOnAwake( gameObject )
    m_Mod = gameObject;
    local objs = gameObject:GetComponent( typeof(UIMod) ).relatedGameObject;

	m_uiOprator = objs[0];
	m_uiUpgrade = objs[1];
	m_uiEnter = objs[2];
	m_uiTrain = objs[3];
	m_uiSpeed = objs[4];
   
    BuildingOpratorModClose();
end

-- 界面初始化时调用
function BuildingOpratorModOnStart()
	
end

-- 界面显示时调用
function BuildingOpratorModOnEnable()

end

-- 界面隐藏时调用
function BuildingOpratorModOnDisable()
	
end

-- 界面删除时调用
function BuildingOpratorModOnDestroy()
end

-- 每帧调用
function BuildingOpratorModOnLogic()
	
end


----------------------------------------
-- 自定
----------------------------------------

function BuildingOpratorModShow( show, kind, offset, parent )

    if show then
		m_kind = kind;
        m_offset = offset;        
		
        -- 打开
        -- 如果已经打开，先关上
        if BuildingOpratorModIsOpen() and show then
            BuildingOpratorModClose();
        end

        -- 打开
        BuildingOpratorModOpen();

        -- 设置位置
        m_Mod.transform.position = parent.position;
        m_Mod.transform.localPosition = Vector3.New( m_Mod.transform.localPosition.x, m_Mod.transform.localPosition.y, 0 );
        
       
        -- 先全部隐藏了
        for tmpi = 0, m_uiOprator.transform.childCount - 1, 1 do
            local op = m_uiOprator.transform:GetChild(tmpi).gameObject:SetActive(false);
        end

        m_uiUpgrade:SetActive(true);
        m_uiEnter:SetActive(true);


        -- 刷新显示位置
        --[[local activeOP = {};
        for i = 0, m_uiOprator.childCount - 1, 1 do
            local op = m_uiOprator:GetChild( i );
            if op.gameObject.activeSelf then
                table.insert( activeOP, op );
            end
        end
        local space = 116;
        local start = -( space * ( #activeOP - 1 ) ) / 2;
        for i = 1, #activeOP, 1 do
            local pos = Vector3.zero;
            pos.x = start + ( i - 1 ) * space;
            local tweenerPos = activeOP[i]:GetComponent( UITweenLocalPosition.GetClassType() );
            tweenerPos.from = pos + Vector3.New( 0, 64, 0 );
            tweenerPos.to = pos;
            local tweenerFade = activeOP[i]:GetComponent( UITweenFade.GetClassType() );

            tweenerFade.delay = 0.2 + i * 0.05;
            tweenerPos.delay = 0.2 + i * 0.05;
            
            tweenerFade:Play( true );
            tweenerPos:Play( true );
        end--]]

        -- 是否可以升级？
        --[[if m_OPUpgrade.activeSelf then
            local showFX = true;
            for i = 1, 2, 1 do
                local info = Data.Building_Upgrade()[kind][level];
                local needKind = info[ "limit_kind" .. i ];
	            local needLevel = info[ "limit_lv" .. i ];
	            if needKind ~= nil and needKind ~= 0 then
		            local needBuilding = GetCity():GetBuildingByKind( needKind, true );
		            if needBuilding == nil or needBuilding.m_level < needLevel then
                        showFX = false;
                        break;
		            end
	            end
            end
            if showFX then
                Invoke( function() 
                            m_OPUpgrade.transform:Find( "FX" ).gameObject:SetActive( true );
                        end, 0.3 );
            else
                if kind >= BUILDING_Barracks and kind <= BUILDING_Magician and state == BUILDING_STATE_NORMAL then
                    Invoke( function() 
                                m_OPTrain.transform:Find( "FX" ).gameObject:SetActive( true );
                            end, 0.3 );
                end
            end
        end--]]

        -- 是否在屏幕适合范围内，不是微调摄像机
        --[[local camera2D = GameManager.MainCity.transform.camera;
        local camera = camera2D.camera;
        local pos = m_uiOprator.transform.position;
        -- 4个基准点
        local point1 = camera:ViewportToWorldPoint( Vector3.New( 0.6, 0.6, 0 ) );
        local point2 = camera:ViewportToWorldPoint( Vector3.New(  0.4, 0.4, 0 ) );
        -- 算偏差
        local move = false;
        local x, y = 0, 0;
        if pos.x > point1.x then
            x = pos.x - point1.x;
        elseif pos.x < point2.x then
            x = pos.x - point2.x;
        end
        if pos.y > point1.y then
            y = pos.y - point1.y;
        elseif pos.y < point2.y then
            y = pos.y - point2.y;
        end
        camera2D:TweenPosBy( Vector3.New( x, y, 0 ) , 0.25 );--]]

    else
        if BuildingOpratorModIsOpen() and m_Closing == false then
            --
            --m_OPUpgrade.transform:Find( "FX" ).gameObject:SetActive( false );
            --m_OPTrain.transform:Find( "FX" ).gameObject:SetActive( false );

            m_Closing = true;
            --m_Tweener[1]:Play( false );
            --m_Tweener[2]:Play( false );
                    BuildingOpratorModClose();
                    m_Closing = false;
					m_kind = 0;
					m_offset = -1;  
            -- 延时关闭
            --[[Invoke( function()
                    BuildingOpratorModClose();
                    m_Closing = false;
                end, 0.2 );---]]
        end
    end
end

function BuildingOpratorModIsOpen()
    if m_Mod == nil or m_Mod.gameObject.activeSelf == false then
        return false
    end
    return true;
end

function BuildingOpratorModGet()
    BuildingOpratorModInit();

	return m_Mod;
end

function BuildingOpratorModInit()
    if m_Mod ~= nil then
        return;
    end
	local trans = GameManager.MainCity.transform:Find( "BuildingUI/BuildingOpratorMod" ) 
	trans:SetActive(true);
	--local trans = GameObject.Instantiate( LoadPrefab( name ) ).transform;
    --trans:SetParent( GameManager.MainCityScence.transform:Find( "BuildingUI" ) );
    --trans.localScale = Vector3.one;
end

-- 获得各个按钮
function BuildingOpratorModGetOPUpgrade()
    return m_uiUpgrade:GetComponent( typedef(UIButton) );
end

