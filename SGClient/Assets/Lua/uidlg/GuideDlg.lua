-- 引导事件
CMD_TALK = 1         --隐藏所有手指
CMD_SPECIAL = 2      --主界面上的指引
CMD_CLICK = 3        --其他页面上的指引

-- 引导的类型
GUIDE_TOCLICKTASK = 1     --指向任务按钮手指
GUIDE_TECH_SURE = 2      --研究科技确认
GUIDE_GOTO_SOCIETY = 3   --世界按钮手指
GUIDE_CLCLK_HEAD = 4     --点击马岱头像
GUIDE_CLCLK_DRESS = 5    --点击衣服穿戴
GUIDE_DRESS = 6          --穿戴
GUIDE_GET = 7            --征收
GUIDE_RECRUIT = 8        --招募
GUIDE_CHOOSE = 9         --选择古淀刀
GUIDE_MAKE = 10          --打造
GUIDE_CHOOSE_WEAPON = 11 --点击武器穿戴
GUIDE_CPOY = 12          --副本按钮手指
GUIDE_BACK = 13          --回城按钮手指
GUIDE_UPGRADE = 14       --升级建筑
GUIDE_CHOOSE_MA = 15     --选择马
GUIDE_DRESS_MA = 16      --点击马穿戴
GUIDE_GET_HERO = 17      --招募武将
GUIDE_CLCLK_ZL = 18      --点击张良头像
GUIDE_CHOOSE_CLOUTH = 19 --选择锁子甲
GUIDE_CHOOSE_HEAD = 20   --选择精铁盔
GUIDE_MAKE_CHOOSE = 21   --选择铁匠
GUIDE_MAKE_UP = 22       --铁匠加速
GUIDE_DRESS_HEAD = 23    --点击头盔穿戴
GUIDE_AUTOBUILDING = 24  --自动建造


GUIDE_TASK_FINISH = 111  --任务完成，作为一个触发的标识
GUIDE_END = 999          --指引结束标识

local m_uiFinger = {nil,nil,nil,nil}; --UnityEngine.GameObject

-- 隐藏界面
function GuideDlgClose()
	if m_Dlg == nil then
		return;
	end
	eye.uiManager:Close( "GuideDlg" );
end

-- 删除界面
function GuideDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function GuideDlgOnEvent( nType, nControlID, value, gameObject )
end

-- 载入时调用
function GuideDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiFinger[1] = objs[0];
	m_uiFinger[2] = objs[1];
	m_uiFinger[3] = objs[2];
	m_uiFinger[4] = objs[3];	
end

-- 界面初始化时调用
function GuideDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function GuideDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function GuideDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function GuideDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function GuideDlgOnLogic( gameObject )
	
end

mId = 0
mStep = 0
mIsGuiding = false


-- 打开界面
function Guide(id, step, force)
	m_Dlg = eye.uiManager:Open( "GuideDlg" );	
	if force ~= true then
        if id == 0 then                   -- 不在引导状态
            if step ~= 1 then
                return;
            end
        else
            if id ~= id then              -- 在进行其他引导
                return;
            elseif step ~= mStep + 1 then  -- 步骤不对
                return;
            end
        end

        -- 检查引导是否完成
        if GuideCheck( id ) and g_guide[mId][mStep].guideType == 999 then
			mIsGuiding = false;
            return;
        end   
    end
	
	mIsGuiding = true;
	
	mId = id;
	mStep = step;
	warn(mId);
	warn(step);
	
	HideGuideFinger();
	HideCurrentFinger();
	
	if g_guide[mId][mStep].cmd == CMD_SPECIAL or g_guide[mId][mStep].cmd == CMD_TALK then FindCmdTpye(nil) end
end

function GuideNext()
	if table.getn(g_guide[mId]) == mStep then
		mStep = 0;
		system_askinfo( ASKINFO_GUAID, "", mId + 1 );
		Guide( mId + 1, 1);
	else
		Guide( mId, mStep + 1 );
	end
end

function ForceGuideNext()
	mStep = 0;
	system_askinfo( ASKINFO_GUAID, "", mId + 1 );
	Guide( mId + 1, 1);
end

function GuideCheck(id)
	if id == table.getn(g_guide) then 
		HideGuideFinger();
		HideCurrentFinger();
		return true
	else 
		return false
	end
end

function IsGuiding()
	return mIsGuiding
end

function GetCurrentGuideType()
	return g_guide[mId][mStep].guideType
end

function GetGuideSpecialEvent()
	return g_guide[mId][mStep].isSpecial
end
function HideGuideFinger()
	for i = 1, table.getn(m_uiFinger), 1 do 
		if m_uiFinger[i] then
			SetFalse(m_uiFinger[i].transform);
		end
	end
end

function ShowGuideFinger(pi)
	HideGuideFinger();
	if m_uiFinger[pi] then
		SetTrue(m_uiFinger[pi].transform);
	end
end

function HideCurrentFinger()
	if currentFinger ~= nil then
		SetFalse(currentFinger.transform);
		currentFinger = nil;
	end
end

-- 设置当前指引位置
function FindCmdTpye(tran)
	cmd = g_guide[mId][mStep].cmd;
	point = g_guide[mId][mStep].point;
	deviation = Vector3.New(g_guide[mId][mStep].x,g_guide[mId][mStep].y,0);
	if cmd == 2 then 
		if g_guide[mId][mStep].guideType == GUIDE_TOCLICKTASK then 
			ShowGuideFinger(point);
			m_uiFinger[point].transform.position = GetQuestDlgPos() + deviation;
		elseif g_guide[mId][mStep].guideType == GUIDE_CPOY then 
			ShowGuideFinger(point);
			m_uiFinger[point].transform.position = GetCopyPos() + deviation;
		elseif g_guide[mId][mStep].guideType == GUIDE_BACK then
			ShowGuideFinger(point);
			m_uiFinger[point].transform.position = GetBackPos() + deviation;
		elseif g_guide[mId][mStep].guideType == GUIDE_AUTOBUILDING then
			ShowGuideFinger(point);
			m_uiFinger[point].transform.position = GetAutoBuildPos() + deviation;
		end
	elseif cmd == 3 then
		if currentFinger == nil then
			currentFinger = addChild(tran,m_uiFinger[point]);
			currentFinger.transform.position = currentFinger.transform.position + deviation;
		end
	elseif cmd == 1 then
		
	end
end

