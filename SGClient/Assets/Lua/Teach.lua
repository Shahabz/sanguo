-- 引导


local m_ID = 0;
local m_NextTeach = 0;
local m_Step = 0;
local m_Flag = 0;

function Teach_IsIn( id )
    if id ~= nil then
        if id == m_ID then
            return true;
        else
            return false;
        end
    elseif m_ID == 0 then
        return false;
    else
        return true;
    end
end

-- 引导开始或下一条
-- force = true强制进行引导
function Teach( id, step, force )
    if force ~= true then
        if m_ID == 0 then                   -- 不在引导状态
            if step ~= 1 then
                return;
            end
        else
            if m_ID ~= id then              -- 在进行其他引导
                return;
            elseif step ~= m_Step + 1 then  -- 步骤不对
                return;
            end
        end

        -- 检查引导是否完成
        if GetPlayer():TeachCheck( id ) and step == 1 then
            return;
        end   
    end

    -- 开始引导
    m_ID = id;
    m_Step = step;

    -- 引导分类
    if id == TEACH_FIGHT then
        Teach_Fight( step );
    elseif id == TEACH_GOBLIN then
        Teach_Goblin( step );
    elseif id == TEACH_MAPDESC then        
        Teach_MapDesc( step );
    elseif id == TEACH_TECH then
        Teach_Tech( step );
    elseif id == TEACH_WISHPOOL then
        Teach_WishPool( step );
    elseif id == TEACH_EMBASSY then
        Teach_Embassy( step );
    elseif id == TEACH_DRILLGROUND then
        Teach_DrillGround( step );
    elseif id == TEACH_HOSPITAL then
        Teach_Hospital( step );
    elseif id == TEACH_STOREHOUSE then
        Teach_StoreHouse( step );
    elseif id == TEACH_PROTECT then
        Teach_Protect( step );
    elseif id == TEACH_MISSION then
        Teach_Mission( step );
    elseif id == TEACH_GHOST then
        Teach_Ghost( step );
    elseif id == TEACH_SPACETIMECRACK then
        Teach_SpacetimeCrack( step );
    elseif id == TEACH_FORGE then
        Teach_Forge( step );
    elseif id == TEACH_WATERWHEEL then
        Teach_WaterWheel( step );
    elseif id == TEACH_PLANEWAR then
        Teach_PlaneWar( step );
    elseif id == TEACH_BUILDQUEUE then
        Teach_BuildQueue( step );
    elseif id == TEACH_SPEEDUP then
        Teach_SpeedUp( step );
    elseif id == TEACH_TROOPGROUP then
        Teach_TroopGroup( step );
    elseif id == TEACH_TROOPFORGING then 
        Teach_TroopForging( step );
    elseif id == TEACH_HEROGROUP then 
        Teach_HeroGroup(step);
    elseif id == TEACH_HEROCOLLECT then 
        Teach_HeroCollect(step);
    elseif id == TEACH_HEROOFFICE then 
        Teach_HeroOffice(step);
    elseif id == TEACH_TREASURE then 
        Teach_Treasure(step);
    elseif id == TEACH_TREASUREPANEL then 
        Teach_TreasurePanel(step)
    end
end

-- 自动下一条
function TeachNext()
    Teach( m_ID, m_Step + 1 );
end

-- 当前指引的指定步骤
function TeachStep( step, force )
    -- 强制执行步骤
    if force == true then
        if step <= 0 then
            return;
        end
        m_Step = step - 1;
    end

    Teach( m_ID, step );
end

-- 引导结束
function TeachUpdateServer()
    -- 告诉服务器引导完成
    --system_askinfo( ASKINFO_ACTOR, "", 7, m_ID );
end

-- 引导清空
function TeachEnd()
    --
    --MaskDlgClose();
    --TalkDlgClose();

    -- 更新缓存
    GetPlayer():TeachEnd( m_ID );

    -- 清空
    m_ID = 0;
    m_Step = 0;

    -- 如果有下一个引导，则进入下个引导
    if m_NextTeach ~= 0 then
        Teach( m_NextTeach, 1 );
        m_NextTeach = 0;
    end
end