-- 引导事件
CMD_TALK = 1         --对话，随机点击跳过（目前修改通过触发的形式出现，暂时不做处理了）
CMD_SPECIAL = 2      --点击，同一界面上有多个的高亮，出现手指指示
CMD_CLICK = 3        --点击，同一区域没有高亮，做高亮处理

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
GUIDE_MAKE = 10          --打造古淀刀
GUIDE_CHOOSE_WEAPON = 11 --点击武器穿戴
GUIDE_CPOY = 12          --副本按钮手指


GUIDE_TASK_FINISH = 111  --任务完成，作为一个触发的标识


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
	m_uiFinger = objs[0];
	m_uiPos = objs[1];
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
        if GuideCheck( id ) and step == 999 then
			mIsGuiding = false;
            return;
        end   
    end
	
	mIsGuiding = true;
	
	mId = id;
	mStep = step;
	warn(mId);
	warn(step);
	
	if g_guide[mId][mStep].cmd == CMD_SPECIAL then FindCmdTpye(nil) end
end

function GuideNext()
	if table.getn(g_guide[mId]) == mStep then
		mStep = 0;
		Guide( mId + 1, 1);
	--	system_askinfo( ASKINFO_GUAID, "", mId + 1 );
	else
		Guide( mId, mStep + 1 );
	end
	HideGuideFinger();
end

function GuideCheck(id)
	if id == table.getn(g_guide) then 
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

function HideGuideFinger()
	SetFalse(m_uiFinger.transform);
end

function ShowGuideFinger()
	SetTrue(m_uiFinger.transform);
end

function HideCurrentFinger()
	if currentFinger ~= nil then
		SetFalse(currentFinger.transform);
	end
end

-- 设置当前指引位置
function FindCmdTpye(tran)
	cmd = g_guide[mId][mStep].cmd;
	HideCurrentFinger();
	if cmd == 2 then 
		if g_guide[mId][mStep].guideType == GUIDE_TOCLICKTASK then 
			ShowGuideFinger();
			m_uiFinger.transform.position = GetQuestDlgPos();
		elseif g_guide[mId][mStep].guideType == GUIDE_CPOY then 
			ShowGuideFinger();
			m_uiFinger.transform.position = GetCopyPos();
		end
	elseif cmd == 3 then
		HideGuideFinger();
		currentFinger = addChild(tran,m_uiFinger);
		SetParent(currentFinger,tran);
	elseif cmd == 1 then
		HideGuideFinger();
	end
end