-- 界面
local m_Dlg = nil;
local m_DialogFrameMod = nil;
local m_uiReelOpen = nil; --UnityEngine.GameObject
local m_uiDesc = nil; --UnityEngine.GameObject
local m_uiDesc1 = nil; --UnityEngine.GameObject
local m_uiDesc2 = nil; --UnityEngine.GameObject
local m_uiDesc3 = nil; --UnityEngine.GameObject
local m_uiOpenTip = nil; --UnityEngine.GameObject
local m_uiTaskTip = nil; --UnityEngine.GameObject
local m_uiTitleTip = nil; --UnityEngine.GameObject
local m_uiProTip = nil; --UnityEngine.GameObject
local m_uiFinishBack = nil; --UnityEngine.GameObject
local m_uiProContent = nil; --UnityEngine.GameObject
local m_uiGoButton = nil; --UnityEngine.GameObject
local m_uiNextButton = nil; --UnityEngine.GameObject
local m_uiOpenButton = nil; --UnityEngine.GameObject
local m_uiActiveButton = nil;
local m_uiTaskContent = nil; --UnityEngine.GameObject
local m_uiTaskScroll = nil; --UnityEngine.GameObject
local m_uiTaskScrollDot = nil; --UnityEngine.GameObject
local m_uiTaskScrollDotToggle = nil; --UnityEngine.GameObject
local m_uiUIP_Task = nil; --UnityEngine.GameObject
local m_uiButtons = nil;

local m_ObjectPool = nil;
local m_TaskPage = nil;
local m_recvValue = nil;
local m_bGoNext = false;

-- 打开界面
function HeroLevyDlgOpen()
	m_Dlg = eye.uiManager:Open( "HeroLevyDlg" );
	m_DialogFrameMod = DialogFrameModOpen( m_Dlg, T(3101), nil, HeroLevyDlgClose );
end

-- 隐藏界面
function HeroLevyDlgClose()
	if m_Dlg == nil then
		return;
	end
	DialogFrameModClose( m_DialogFrameMod );
	m_DialogFrameMod = nil;
	eye.uiManager:Close( "HeroLevyDlg" );
	m_recvValue = nil;
end

-- 删除界面
function HeroLevyDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function HeroLevyDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            HeroLevyDlgClose();
		elseif nControlID == 1 then
			HeroLevyDlgTaskPre();			
		elseif nControlID == 2 then
			HeroLevyDlgTaskNext();
		elseif nControlID == 11 then			--开启任务
			warn("开启任务：")
			HeroLevyDlgOnBtnOpenTask();
		elseif nControlID == 12 then			--前往任务
			warn("前往任务：")
			HeroLevyDlgOnBtnGoTask();
		elseif nControlID == 13 then			--激活兵术
			warn("激活兵术：")
			HeroLevyDlgOnBtnActiveTask();
		elseif nControlID == 14 then			--下一步
			warn("下一步：")
			HeroLevyDlgOnBtnNextTask();
		elseif nControlID == 15 then			--前往升级
			warn("前往升级")
			HeroLevyDlgOnBtnGoUpLevel();
        end
	elseif nType == UI_EVENT_SCROLLPAGE then
		if nControlID == 0 then
			m_TaskPage = value;
			HeroLevyDlgSetTaskPage();
			HeroLevyDlgTaskStateView();
		end
	end
end

-- 载入时调用
function HeroLevyDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiInfoLayer = objs[0];
	m_uiLeftBtn = objs[1];
	m_uiRightBtn = objs[2];
	m_uiTipLayer = objs[3];
	m_uiReelClose = objs[4];
	m_uiReelOpen = objs[5];
	m_uiNationGrade = objs[6];
	m_uiDesc1 = objs[7];
	m_uiDesc2 = objs[8];
	m_uiDesc3 = objs[9];
	m_uiOpenTip = objs[10];
	m_uiTaskTip = objs[11];
	m_uiTitleTip = objs[12];
	m_uiProTip = objs[13];
	m_uiProContent = objs[14];
	m_uiFinishBack = objs[15];
	m_uiGoButton = objs[16];
	m_uiNextButton = objs[17];
	m_uiActiveButton = objs[18];
	m_uiOpenButton = objs[19];
	m_uiTaskScroll = objs[20];
	m_uiTaskContent = objs[21];
	m_uiTaskScrollDot = objs[22];
	m_uiTaskScrollDotToggle = objs[23];
	m_uiUIP_Task = objs[24];
	m_uiButtons = objs[25];
end

-- 界面初始化时调用
function HeroLevyDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function HeroLevyDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function HeroLevyDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function HeroLevyDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function HeroLevyDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
-- 消息
-- system_askinfo( ASKINFO_TIANCEQUEST, "", 0 ); -- 获取玩家天策府任务信息
--system_askinfo( ASKINFO_TIANCEQUEST, "", 1, kind ); -- 领取开启一个任务
--system_askinfo( ASKINFO_TIANCEQUEST, "", 2, kind ); -- 完成一个任务
--system_askinfo( ASKINFO_TIANCEQUEST, "", 3, kind ); — 激活这个任务的点兵术

function HeroLevyDlgShow()
	HeroLevyDlgOpen()
	system_askinfo( ASKINFO_TIANCEQUEST, "", 0 )	
end

function HeroLevyDlgRecv(recvValue)
	m_RecvValue = recvValue;
	PrintTable(m_RecvValue,"天策府任务信息")
	HeroLevyDlgSetSelectPage();
	HeroLevyDlgTaskView();
	HeroLevyDlgTaskStateView();
	HeroLevyDlgNationGradeView();
end

-- 设置任务选择页
function HeroLevyDlgSetSelectPage()
	if m_TaskPage == nil then 
		if m_RecvValue.m_tc_kind ~= 0 then 
			m_TaskPage = m_RecvValue.m_tc_kind -1;
		else
			m_TaskPage = 0;
		end
	else
		if m_bGoNext == true then 
			m_TaskPage = m_TaskPage+1;
			m_bGoNext = false;
		end
	end
end
--点兵统领等级经验
function HeroLevyDlgNationGradeView()
	local Level = m_RecvValue.m_nation_tiance_level;
	local Exp = m_RecvValue.m_nation_tiance_point;
	local MaxExp = g_upgradeinfo[Level+1].tiance_point;
	SetText(m_uiNationGrade,F(3102,Level,Exp,MaxExp));
end

--当前选中的任务状态
function HeroLevyDlgTaskStateView() 
	local TaskIndex = m_TaskPage +1;
	--是否已完成
	if Utils.get_int_sflag( m_RecvValue.m_tc_tech, TaskIndex ) == 1  then 	
		HeroLevyDlgBsState(true);
		HeroLevyDlgReelState(false);
	else
		--是否能进行兵术激活
		if TaskIndex <= m_RecvValue.m_tc_kind then		
			--是否已任务完成
			if Utils.get_int_sflag( m_RecvValue.m_tc_state, TaskIndex ) == 1 then 
				HeroLevyDlgBsNext(true);
				HeroLevyDlgReelState(false);
			else
				--是否已开启兵术任务
				if TaskIndex == m_RecvValue.m_tc_kind then
					--任务是否完成
					if m_RecvValue.m_tc_num >= g_tiance_quest[m_RecvValue.m_tc_kind].brush_enemynum then 
						HeroLevyDlgBsTaskState(true);
					else
						HeroLevyDlgBsTaskState(false);
					end
					HeroLevyDlgReelState(false);					
				else
					--是否等级足够
					if GetPlayer().m_level >= g_tiance_quest[m_TaskPage+1].actor_level then 
						HeroLevyDlgBsTaskOpen(true);
					else
						HeroLevyDlgBsTaskOpen(false);
					end
					HeroLevyDlgReelState(true);
				end	
			end
		else
			if m_TaskPage == 0 and m_RecvValue.m_tc_kind == 0 then 
				HeroLevyDlgBsFristOpen();
			else
				-- 兵术未开启
				HeroLevyDlgBsState(false);
			end		
			HeroLevyDlgReelState(false);
		end
	end
end
-- 兵术初次开启
function HeroLevyDlgBsFristOpen()
	--标题
	SetFalse(m_uiOpenTip);		
	SetFalse(m_uiTaskTip);
	SetTrue(m_uiTitleTip);
	SetText(m_uiTitleTip,T(3105))
	SetFalse(m_uiProTip);
	SetFalse(m_uiFinishBack);
	SetFalse(m_uiProContent);
	SetTrue(m_uiButtons);
	--按钮	
	SetFalse(m_uiGoButton);
	SetFalse(m_uiNextButton);
	SetTrue(m_uiOpenButton);
	SetFalse(m_uiActiveButton);
end

-- 兵术任务开启
function HeroLevyDlgBsTaskOpen(bCanOpen)
	SetFalse(m_uiOpenTip);		
	SetFalse(m_uiTaskTip);
	SetTrue(m_uiTitleTip);
	SetText(m_uiTitleTip,F(3107,GetPlayer().m_level));
	SetTrue(m_uiProTip);
	SetText(m_uiProTip,F(3108,GetPlayer().m_level,g_tiance_quest[m_TaskPage+1].actor_level));
	SetFalse(m_uiFinishBack);
	SetFalse(m_uiProContent);
	SetTrue(m_uiButtons);
	--按钮	
	if bCanOpen == true then 
		SetFalse(m_uiGoButton);
		SetFalse(m_uiNextButton);
		SetTrue(m_uiOpenButton)
		SetFalse(m_uiActiveButton);
	else
		SetTrue(m_uiGoButton);
		SetControlID(m_uiGoButton,15);
		SetFalse(m_uiNextButton);
		SetFalse(m_uiOpenButton)
		SetFalse(m_uiActiveButton);
	end	
end

-- 兵术状态
function HeroLevyDlgBsState(bSuccess)
	SetTrue(m_uiOpenTip);	
	SetFalse(m_uiTaskTip);
	SetFalse(m_uiTitleTip);
	SetFalse(m_uiProTip);
	SetFalse(m_uiFinishBack);
	SetFalse(m_uiProContent);
	SetFalse(m_uiButtons);
	if bSuccess == true then 
		SetText(m_uiOpenTip,T(3103))
	else
		SetText(m_uiOpenTip,T(3104))
	end
end

-- 兵术任务描述
function HeroLevyDlgBsTaskState(bSuccess)
	SetFalse(m_uiOpenTip);	
	SetTrue(m_uiTaskTip);
	SetText(m_uiTaskTip,F(3106,g_tiance_quest[m_RecvValue.m_tc_kind].brush_enemynum));
	SetFalse(m_uiTitleTip);
	SetTrue(m_uiProTip);
	SetText(m_uiProTip,F(3108,m_RecvValue.m_tc_num,g_tiance_quest[m_RecvValue.m_tc_kind].brush_enemynum));
	SetTrue(m_uiProContent);
	HeroLevyDlgSetTaskPro(m_RecvValue.m_tc_num,g_tiance_quest[m_RecvValue.m_tc_kind].brush_enemynum);
	SetTrue(m_uiButtons);
	if bSuccess == true then 
		SetTrue(m_uiFinishBack);
		SetFalse(m_uiGoButton);
		SetFalse(m_uiNextButton);
		SetFalse(m_uiOpenButton);
		SetTrue(m_uiActiveButton);
	else
		SetFalse(m_uiFinishBack);
		SetTrue(m_uiGoButton);
		SetControlID(m_uiGoButton,12);
		SetFalse(m_uiNextButton);
		SetFalse(m_uiOpenButton);
		SetFalse(m_uiActiveButton);
	end
end

-- 下一个兵术任务
function HeroLevyDlgBsNext(bSuccess)
	SetFalse(m_uiOpenTip);	
	SetTrue(m_uiTaskTip);
	SetText(m_uiTaskTip,F(3106,g_tiance_quest[m_RecvValue.m_tc_kind].brush_enemynum));
	SetTrue(m_uiTitleTip);
	SetTrue(m_uiProTip);
	SetText(m_uiProTip,F(3108,m_RecvValue.m_tc_num,g_tiance_quest[m_RecvValue.m_tc_kind].brush_enemynum));
	SetTrue(m_uiFinishBack);
	SetTrue(m_uiProContent);
	HeroLevyDlgSetTaskPro(m_RecvValue.m_tc_num,g_tiance_quest[m_RecvValue.m_tc_kind].brush_enemynum);
	SetTrue(m_uiButtons);
	if bSuccess == true then 
		SetFalse(m_uiGoButton);
		SetTrue(m_uiNextButton);
		SetFalse(m_uiOpenButton)
		SetFalse(m_uiActiveButton);
	else
		SetTrue(m_uiGoButton);
		SetControlID(m_uiGoButton,12);
		SetFalse(m_uiNextButton);
		SetFalse(m_uiOpenButton)
		SetFalse(m_uiActiveButton);
	end
end

-- 兵术任务进度
function HeroLevyDlgSetTaskPro(value,max)
	for i = 1, m_uiProContent.transform.childCount ,1 do 
		local uiObj = m_uiProContent.transform:GetChild( i - 1 );
		local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
		local uiPro = objs[0];
		if max >= i then
			SetTrue(uiObj);
		else
			SetFalse(uiObj);
		end
		if value >= i then 
			SetTrue(uiPro);
		else
			SetFalse(uiPro);
		end
	end
end

-- 卷轴状态
function HeroLevyDlgReelState(bShow)
	if bShow == true then 
		SetTrue(m_uiReelOpen);
		SetFalse(m_uiReelClose);
		SetText(m_uiDesc1,F(3109,m_RecvValue.m_nation_tiance_level))	
		local BsName = T(tonumber("320"..m_TaskPage+1));
		local NeedLevel = g_tiance_quest[m_TaskPage+1].actor_level - m_RecvValue.m_nation_tiance_level; 
		SetText(m_uiDesc2,F(3110,BsName,NeedLevel))
	else
		SetFalse(m_uiReelOpen);
		SetTrue(m_uiReelClose);
	end
end

-- 点击按钮：开启任务
function HeroLevyDlgOnBtnOpenTask()
	system_askinfo( ASKINFO_TIANCEQUEST, "", 1, m_RecvValue.m_tc_kind ); -- 领取开启一个任务
end

-- 点击按钮：前往任务
function HeroLevyDlgOnBtnGoTask()
	HeroLevyDlgClose()
	CabinetDlgClose()
	WorldMap.GotoWorldMap(-1, -1)
end

-- 点击按钮：前往升级
function HeroLevyDlgOnBtnGoUpLevel()
	HeroLevyDlgClose()
	CabinetDlgClose()
end

-- 点击按钮：完成任务
function HeroLevyDlgOnBtnActiveTask()
	system_askinfo( ASKINFO_TIANCEQUEST, "", 2, m_RecvValue.m_tc_kind ); -- 完成点兵术任务
end

-- 点击按钮：下一步
function HeroLevyDlgOnBtnNextTask()
	system_askinfo( ASKINFO_TIANCEQUEST, "", 3, m_RecvValue.m_tc_kind ); -- 激活这个任务的点兵术
	if m_RecvValue.m_tc_kind < 8 then 
		m_bGoNext = true;
	end
end

-- 点兵术列表初始化
function HeroLevyDlgTaskView()
	local scrollPage = m_uiTaskScroll.transform:GetComponent( typeof(UIScrollPage) )
	scrollPage:ClearPage()
	
	for i = 1,8,1 do
		HeroLevyDlgSetTask(i,scrollPage);
	end	
	
	scrollPage:PageCountChanged();
		
	HeroLevyDlgSetTaskPage();
end

function HeroLevyDlgSetTask(index,scrollPage)	
	local uiObj = GameObject.Instantiate( m_uiUIP_Task );
	SetTrue( uiObj )
	scrollPage:AddPage( uiObj )
	
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	m_uiName = objs[0];
	m_uiTitle = objs[1];
	
	local nameIndex = tonumber("320"..index);
	SetText(m_uiName,T(nameIndex));
	local TitleIndex = tonumber("321"..index);
	SetText(m_uiTitle,T(TitleIndex));
	
end

-- 上一个
function HeroLevyDlgTaskPre()
	if m_TaskPage <= 0 then
		return
	end
	local scrollPage = m_uiTaskScroll.transform:GetComponent( typeof(UIScrollPage) )
	scrollPage:Pre()
end

-- 下一个
function HeroLevyDlgTaskNext()
	if m_TaskPage >= 7 then
		return
	end
	local scrollPage = m_uiTaskScroll.transform:GetComponent( typeof(UIScrollPage) )
	scrollPage:Next()
end

function HeroLevyDlgSetTaskPage()
	if m_TaskPage <= 0 then
		SetFalse( m_uiLeftBtn )
	else
		SetTrue( m_uiLeftBtn )
	end
	
	if m_TaskPage >= 7 then
		SetFalse( m_uiRightBtn )
	else
		SetTrue( m_uiRightBtn )
	end
end