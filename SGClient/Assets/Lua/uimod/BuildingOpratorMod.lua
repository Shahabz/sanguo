local m_Mod;

local m_uiOprator = nil; --UnityEngine.GameObject
local m_uiUpgrade = nil; --UnityEngine.GameObject
local m_uiEnter = nil; --UnityEngine.GameObject
local m_uiTrain = nil; --UnityEngine.GameObject
local m_uiSpeed = nil; --UnityEngine.GameObject
local m_uiDelete = nil; --UnityEngine.GameObject

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
		-- 升级
        if nControlID == 1 then
			BuildingUpgradeDlgShow( m_kind, m_offset, 0 )		
		-- 进入
        elseif nControlID == 2 then
			if m_kind == BUILDING_Main then
				LevyDlgShow();
			elseif m_kind == BUILDING_Wall then
				eye.audioManager:Play(314);
				CityGuardDlgShow();
			elseif m_kind == BUILDING_StoreHouse then
				StoreDlgShow()
			elseif m_kind == BUILDING_Tech then
				CityTechDlgOnShow();
			elseif m_kind == BUILDING_Craftsman then
					eye.audioManager:Play(316);
				MaterialMakeDlgShow();
			elseif m_kind == BUILDING_Cabinet	then
				CabinetDlgShow();
			elseif m_kind == BUILDING_Smithy then
				EquipForgingDlgShow();
			elseif m_kind == BUILDING_Wash then
				EquipWashDlgShow();
			end
			
		-- 招募
		elseif nControlID == 3 then
			TrainDlgOnShow( m_kind );
			
		-- 加速
		elseif nControlID == 4 then
			if m_kind == BUILDING_Tech then
				CityTechDlgQuick();
			elseif m_kind >= BUILDING_Infantry and m_kind <= BUILDING_Militiaman_Archer then
				local pBuilding = GetPlayer():GetBuilding( m_kind, -1 );
				QuickItemDlgShow( 2, m_kind, -1, pBuilding.m_sec )
			end
			
		-- 改建
		elseif nControlID == 5 then
			if m_kind >= BUILDING_Militiaman_Infantry and m_kind <= BUILDING_Militiaman_Archer then
				BuildingCreateDlgShowByID( 1, m_kind )
			elseif m_kind >= BUILDING_Silver and m_kind <= BUILDING_Iron then
				BuildingCreateDlgShowByRes( m_kind, m_offset )
			end
        end
		City.BuildingUnSelect();
		BuildingOpratorModShow( false, 0, -1, nil );
		
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
	m_uiDelete = objs[5];
   
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
-------------------------------
-- 按钮表现
-------------------------------
-- 移动距离
local uiButtonOffset = 40;
local uiButtonOffsetX = 110;
local uiButtonOffsetY = 44;
local m_ButtonPos = 
{
    [1] = { Vector3.New( 0, -uiButtonOffsetY, 0 ) },
    [2] = { Vector3.New( -uiButtonOffsetX, -uiButtonOffsetY+uiButtonOffset, 0 ), Vector3.New( uiButtonOffsetX, -uiButtonOffsetY+uiButtonOffset, 0 ) },
    [3] = { Vector3.New( -uiButtonOffsetX, -uiButtonOffsetY+uiButtonOffset, 0 ), Vector3.New( 0, -uiButtonOffsetY+uiButtonOffset, 0 ), Vector3.New( uiButtonOffsetX, -uiButtonOffsetY+uiButtonOffset, 0 ) },
}
function BuildingOpratorModButton( buttonList )
    for i = 1, #buttonList, 1 do
        buttonList[i].gameObject:SetActive( true );
        buttonList[i]:GetComponent(typeof(UITweenLocalPosition)).to = m_ButtonPos[ #buttonList ][i];
        buttonList[i]:GetComponent(typeof(UITweenLocalPosition)):Play( true );
    end
end
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
        m_Mod.transform:SetSiblingIndex(100);
       
        -- 先全部隐藏了
        for tmpi = 0, m_uiOprator.transform.childCount - 1, 1 do
            local op = m_uiOprator.transform:GetChild(tmpi).gameObject:SetActive(false);
        end
		
		-- 兵营
		if m_kind >= BUILDING_Infantry and m_kind <= BUILDING_Militiaman_Archer then
			local pBuilding = GetPlayer():GetBuilding( m_kind, m_offset );
			-- 招募中
			if pBuilding.m_sec > 0 then
				m_uiSpeed:SetActive(true);
				m_uiTrain:SetActive(true);
			else
				if pBuilding.m_level < #g_building_upgrade[m_kind] then
					m_uiUpgrade:SetActive(true);
					m_uiTrain:SetActive(true);
				else
					m_uiTrain:SetActive(true);
				end
				
				-- 重建
				if m_kind >= BUILDING_Militiaman_Infantry and m_kind <= BUILDING_Militiaman_Archer then
					if GetPlayer().m_attr.m_ability_open_203 > 0 then
						m_uiDelete:SetActive(true);
					end
				end
				
			end
			
		
		-- 资源	
		elseif m_kind >= BUILDING_Silver and m_kind <= BUILDING_Iron then
			local pBuilding = GetPlayer():GetBuilding( m_kind, m_offset );
			if pBuilding.m_level <= 0 then
				 BuildingOpratorModClose();
				ResDrawingDlgShow( m_kind, m_offset );
				return
			end
			if pBuilding.m_level < #g_building_upgrade[m_kind] then
				m_uiUpgrade:SetActive(true);
			end
			-- 重建
			if GetPlayer().m_attr.m_ability_open_203 > 0 then
				m_uiDelete:SetActive(true);
			end
			
		-- 太学院	
		elseif m_kind == BUILDING_Tech then
			local pBuilding = GetPlayer():GetBuilding( m_kind, m_offset );
			if pBuilding.m_level < #g_building_upgrade[m_kind] then
				if pBuilding.m_sec > 0 then
					m_uiSpeed:SetActive(true);
					m_uiEnter:SetActive(true);
				else
					m_uiUpgrade:SetActive(true);
					m_uiEnter:SetActive(true);
				end
			else
				if pBuilding.m_sec > 0 then
					m_uiSpeed:SetActive(true);
					m_uiEnter:SetActive(true);
				else
					m_uiEnter:SetActive(true);
				end
			end
			
		else
			local pBuilding = GetPlayer():GetBuilding( m_kind, m_offset );
			if pBuilding.m_level < #g_building_upgrade[m_kind] then
				m_uiUpgrade:SetActive(true);
				m_uiEnter:SetActive(true);
			else
				m_uiEnter:SetActive(true);
			end
		end


        -- 刷新显示位置
        local activeOP = {};
        for i = 0, m_uiOprator.transform.childCount - 1, 1 do
            local op = m_uiOprator.transform:GetChild( i );
            if op.gameObject.activeSelf then
                table.insert( activeOP, op );
            end
        end
		BuildingOpratorModButton( activeOP )
        --[[local space = 160;
        local start = -( space * ( #activeOP - 1 ) ) / 2;
        for i = 1, #activeOP, 1 do
            local pos = Vector3.zero;
            pos.x = start + ( i - 1 ) * space;
            local tweenerPos = activeOP[i]:GetComponent( typeof(UITweenLocalPosition) );
            tweenerPos.from = pos + Vector3.New( 0, 64, 0 );
            tweenerPos.to = pos;
            local tweenerFade = activeOP[i]:GetComponent( typeof(UITweenFade) );

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
	BuildingOpratorModHideAllEffect();
	ShowBuildingModEffect();
end

function ShowBuildingModEffect()
	if IsGuiding() then
		if GetCurrentGuideType() == GUIDE_TECH or GetCurrentGuideType() == GUIDE_GET then
			BuildingOpratorModEffectShow(2);
		elseif GetCurrentGuideType() == GUIDE_UPGRADE then
			BuildingOpratorModEffectShow(1);
		elseif GetCurrentGuideType() == GUIDE_RECRUIT then
			BuildingOpratorModEffectShow(3);
		else
			ShowMod();
		end
	else
		ShowMod();
	end
end

function ShowMod()
	local pBuilding = GetPlayer():GetBuilding( m_kind, m_offset );
	if pBuilding ~= nil then
		if m_kind >= BUILDING_Infantry and m_kind <= BUILDING_Militiaman_Archer then
			BuildingOpratorModEffectShow(3);
		elseif m_kind == BUILDING_Tech then
			BuildingOpratorModEffectShow(2);
		elseif pBuilding.m_level < #g_building_upgrade[m_kind] then
			BuildingOpratorModEffectShow(1);
		end
	else
		BuildingOpratorModEffectShow(1);
	end
end

function BuildingOpratorModEffectShow(kind)
	BuildingOpratorModHideAllEffect()
	
	if kind == 1 then SetTrue(m_uiUpgrade.transform:Find("Effect"));
	elseif kind == 2 then SetTrue(m_uiEnter.transform:Find("Effect"));
	elseif kind == 3 then SetTrue(m_uiTrain.transform:Find("Effect"));
	elseif kind == 4 then SetTrue(m_uiSpeed.transform:Find("Effect"));
	end
end

function BuildingOpratorModHideAllEffect()
	SetFalse(m_uiUpgrade.transform:Find("Effect"));
	SetFalse(m_uiEnter.transform:Find("Effect"));
	SetFalse(m_uiTrain.transform:Find("Effect"));
	SetFalse(m_uiSpeed.transform:Find("Effect"));
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


