-- cmd引导事件
CMD_TALK = 1         --隐藏所有手指
CMD_SPECIAL = 2      --主界面上的指引
CMD_CLICK = 3        --其他页面上的指引

-- guideType引导的类型
GUIDE_TOCLICKTASK = 1    --指向任务按钮手指
GUIDE_TECH_SURE = 2      --研究科技确认按钮
GUIDE_GOTO_SOCIETY = 3   --世界按钮手指
GUIDE_CLCLK_HEAD = 4     --点击马岱头像
GUIDE_CLCLK_DRESS = 5    --点击衣服穿戴
GUIDE_DRESS = 6          --装备穿戴
GUIDE_GET = 7            --征收
GUIDE_RECRUIT = 8        --招募士兵
GUIDE_CHOOSE = 9         --选择古淀刀
GUIDE_MAKE = 10          --打造
GUIDE_CHOOSE_WEAPON = 11 --点击武器穿戴
GUIDE_CPOY = 12          --副本按钮手指
GUIDE_BACK = 13          --回城按钮手指
GUIDE_UPGRADE = 14       --升级建筑
GUIDE_CHOOSE_MA = 15     --选择马
GUIDE_DRESS_MA = 16      --点击马穿戴
GUIDE_GET_HERO = 17      --招募武将按钮
GUIDE_CLCLK_ZL = 18      --点击张良头像
GUIDE_CHOOSE_CLOUTH = 19 --选择锁子甲
GUIDE_CHOOSE_HEAD = 20   --选择精铁盔
GUIDE_MAKE_CHOOSE = 21   --选择铁匠
GUIDE_MAKE_UP = 22       --铁匠加速
GUIDE_DRESS_HEAD = 23    --点击头盔穿戴
GUIDE_AUTOBUILDING = 24  --自动建造
GUIDE_HERODLG = 25       --武将按钮
GUIDE_TECH = 26          --科技特效
GUIDE_WORK = 27          --任务按钮
GUIDE_COPY_HERO = 28     --副本武将招募手指
GUIDE_DRESS_YIN = 29     --点击印章穿戴
GUIDE_DRESS_FU = 30      --点击符穿戴
GUIDE_GUOQI = 31         --打造国器1
GUIDE_DAZAO = 32         --点击开始打造国器

GUIDE_TASK_FINISH = 111  --任务完成，作为一个触发的标识
GUIDE_END = 999          --指引结束标识

local m_uiFinger = {nil,nil,nil,nil,nil,nil}; --UnityEngine.GameObject

-- 打开界面
function GuideDlgOpen()
	m_Dlg = eye.uiManager:Open( "GuideDlg" );
end

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
	m_uiFinger[5] = objs[4];
	m_uiFinger[6] = objs[5];
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

local mId = 0
local mStep = 0
local mIsGuiding = false


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
        if GuideCheck( id ) and mStep == 999 then
			mIsGuiding = false;
            return;
        end   
    end
	
	mIsGuiding = true;
	
	mId = id;
	mStep = step;
	warn(mId);
	warn(step);
	
	GuideClear();
	currentFinger = nil;
	currentMainFinger = nil;
	
	if g_guide[mId][mStep].cmd == CMD_SPECIAL or g_guide[mId][mStep].cmd == CMD_TALK then FindCmdTpye(nil) end
end

function GuideNext()
	if table.getn(g_guide[mId]) == mStep then
		GuideClear();
		system_askinfo( ASKINFO_GUAID, "", mId + 1 );
	else
		Guide( mId, mStep + 1 );
	end
end

function GuideEnd()
	GuideClear();
	mStep = 0;
	system_askinfo( ASKINFO_GUAID, "", mId + 1 );
end

function GuideCheck(id)
	if id == table.getn(g_guide) then 
		GuideClear();
		return true
	else 
		return false
	end
end

function IsGuiding()
	return mIsGuiding
end

function GuideClear()
	HideGuideFinger();
	HideCurrentFinger();
end

function GetCurrentGuideType()
	if mStep == 0 then
		return 0;
	end
	return g_guide[mId][mStep].guideType
end

function GetGuideSpecialEvent()
	return g_guide[mId][mStep].isSpecial
end
function HideGuideFinger()
	if m_Dlg ~= nil then
		for i = 1, table.getn(m_uiFinger), 1 do 
			if m_uiFinger[i] then
				SetFalse(m_uiFinger[i].transform);
			end
		end
	end
end

function ShowGuideFinger()
	HideGuideFinger();
	if currentMainFinger ~= nil then
		SetTrue(currentMainFinger.transform);
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
	if g_guide[mId] == nil or g_guide[mId][mStep] == nil then
		return
	end
	cmd = g_guide[mId][mStep].cmd;
	point = g_guide[mId][mStep].point;
	deviation = Vector3.New(g_guide[mId][mStep].x,g_guide[mId][mStep].y,0);
	if cmd == 2 then 
		currentMainFinger = m_uiFinger[point];
		ShowGuideFinger();
		if g_guide[mId][mStep].guideType == GUIDE_TOCLICKTASK then 
			m_uiFinger[point].transform.position = GetQuestDlgPos() + deviation;
		elseif g_guide[mId][mStep].guideType == GUIDE_CPOY then 
			m_uiFinger[point].transform.position = GetCopyPos() + deviation;
		elseif g_guide[mId][mStep].guideType == GUIDE_GOTO_SOCIETY then 
			m_uiFinger[point].transform.position = GetWorldPos() + deviation;
		elseif g_guide[mId][mStep].guideType == GUIDE_BACK then
			m_uiFinger[point].transform.position = GetBackPos() + deviation;
		elseif g_guide[mId][mStep].guideType == GUIDE_AUTOBUILDING then
			m_uiFinger[point].transform.position = GetAutoBuildPos() + deviation;
		elseif g_guide[mId][mStep].guideType == GUIDE_HERODLG then 
			m_uiFinger[point].transform.position = GetHeroDlgPos() + deviation;
		elseif g_guide[mId][mStep].guideType == GUIDE_WORK then 
			m_uiFinger[point].transform.position = GetWorkPos() + deviation;
		end
	elseif cmd == 3 then
		if currentFinger == nil and m_uiFinger[point] then
			currentFinger = addChild(tran,m_uiFinger[point]);
			currentFinger.transform.position = currentFinger.transform.position + deviation;
		end
	elseif cmd == 1 then
		
	end
end

