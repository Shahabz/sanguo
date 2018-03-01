-- 引导事件
CMD_TALK = 1         --对话，随机点击跳过
CMD_CLICK = 2        --点击，固定位置点击

-- 引导的类型
GUIDE_GETTASKREWARD = 1   --领取奖励按钮高亮
GUIDE_GETNEW_BUILDING = 2 --获得新建筑
GUIDE_TOCLICKTASK = 3     --指向任务按钮手指
GUIDE_BUILDINGUPDATE = 4  --建筑升级按钮高亮
GUIDE_CREATENAME = 5      --名称创建按钮高亮
GUIDE_SOLVE = 6           --点击头像救治英雄（补高亮效果）
GUIDE_TALKCLICK = 7       --对话的点击按钮高亮
GUIDE_GETHERO = 8         --获得武将的点击按钮高亮
GUIDE_AUTOMATIC = 9       --自动建造图标高亮
GUIDE_TEACHER = 10        --研究院雇佣按钮手指
GUIDE_TECH = 11           --研究中心进入按钮
GUIDE_TECH_SURE = 12      --研究安抚民众确认
GUIDE_GOTO_SOCIETY = 13   --世界按钮手指
GUIDE_ENEMY_ATTACK = 14   --流寇进攻按钮高亮或手指
GUIDE_ADD_ARMY = 15       --补充兵力按钮高亮
GUIDE_HERO_ATTACK = 16    --武将出击按钮高亮
GUIDE_CLCLK_HEAD = 17     --点击马岱头像
GUIDE_CLCLK_DRESS = 18    --点击衣服穿戴
GUIDE_DRESS = 19          --穿戴
GUIDE_BACK = 20           --回城
GUIDE_UPGRADE = 21        --升级按钮
GUIDE_FREE = 22           --点击免费按钮
GUIDE_TALK = 23           --对话
GUIDE_ATTACK = 24         --围剿扰乱居民区的山贼
GUIDE_TEACH_GET = 25      --点击领取已学习的科技


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
        if GuideCheck( id ) and step == 1 then
            return;
        end   
    end
	
	mIsGuiding = true;
	
	mId = id;
	mStep = step;
	
	if g_guide[mId][mStep].cmd == CMD_TALK then FindCmdTpye(nil) end
	--[[" if then
		
		m_uiFinger.transform.position = GetQuestDlgPos();"]]
end

function GuideNext()
    Guide( mId, mStep + 1 );
	HideGuideFinger();
end

function GuideCheck(id)
	if id == 999 then 
		mIsGuiding = false;
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


-- 设置当前指引位置
function FindCmdTpye(pos)
	cmd = g_guide[mId][mStep].cmd;
	if cmd == 1 then 
		if g_guide[mId][mStep].guideType == GUIDE_TOCLICKTASK then 
			SetTrue(m_uiFinger);
			m_uiFinger.transform.position = GetQuestDlgPos();
		elseif g_guide[mId][mStep].guideType == GUIDE_GOTO_SOCIETY then
			SetTrue(m_uiFinger);
			m_uiFinger.transform.position =  GetWorldPos();
		else
			SetFalse(m_uiFinger);
			m_uiFinger.transform.localPosition = Vector3.New(0,0,0);
		end
	elseif cmd == 2 then
		SetTrue(m_uiFinger);
		m_uiFinger.transform.position = pos;
		--[["
		if g_guide[mId][mStep].guideType == GUIDE_GETTASKREWARD then
			m_uiFinger.transform.position = pos;
		elseif g_guide[mId][mStep].guideType == GUIDE_BUILDINGUPDATE then 
			m_uiFinger.transform.position = pos;
		elseif g_guide[mId][mStep].guideType == GUIDE_CREATENAME then
			m_uiFinger.transform.position = pos;   
		end
		"]]
	end
end