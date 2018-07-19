-- 界面
local m_Dlg = nil;
local m_DialogFrameMod = nil;
local m_uiInfoLayer = nil; --UnityEngine.GameObject
local m_uiScroll = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiUIP_Quest = {nil,nil}; --UnityEngine.GameObject

local m_recvValue = nil;

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
		elseif nControlID == 1001 or nControlID == 1002 then
			HeroLevyDlgDonate( nControlID-1000 );
        end
	end
end

-- 载入时调用
function HeroLevyDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiInfoLayer = objs[0];
	m_uiScroll = objs[1];
	m_uiContent = objs[2];
	m_uiUIP_Quest[1] = objs[3];
	m_uiUIP_Quest[2] = objs[4];
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

function HeroLevyDlgUpdate()
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	HeroLevyDlgRecv(m_RecvValue)
end

function HeroLevyDlgRecv(recvValue)
	m_RecvValue = recvValue;
	HeroLevyDlgCreateObj( m_uiUIP_Quest[1], 1, recvValue.m_tc_level[1], recvValue.m_tc_progress[1] )
	HeroLevyDlgCreateObj( m_uiUIP_Quest[2], 2, recvValue.m_tc_level[2], recvValue.m_tc_progress[2] )
end

function HeroLevyDlgCreateObj( uiObj, kind, level, progress )
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiName = objs[0];
	local uiDesc = objs[1];
	local uiUnLock = objs[2];
	local uiProgress = objs[3];
	local uiSilver = objs[4];
	local uiWood = objs[5];
	local uiFood = objs[6];
	local uiIron = objs[7];
	local uiDonateButton = objs[8];
	local uiCostBack = objs[9];
	local uiWarning = objs[10];
	SetControlID( uiDonateButton, 1000 + kind )
	
	local nextlevel = level+1
	if nextlevel > 4 then
		SetText( uiName, F(3200+kind,g_tiance_quest[kind][level].level) )
		SetText( uiDesc, F(3210+kind,g_tiance_quest[kind][level].showvalue) )
		SetFalse(uiUnLock)
		SetFalse(uiProgress)
		SetFalse(uiCostBack)
		SetTrue(uiWarning)
		SetFalse(uiDonateButton)
	else	
		SetText( uiName, F(3200+kind,g_tiance_quest[kind][nextlevel].level) )
		SetText( uiDesc, F(3210+kind,g_tiance_quest[kind][nextlevel].showvalue) )
		if GetPlayer().m_level < g_tiance_quest[kind][nextlevel].actor_level then
			SetTrue(uiUnLock)
			SetText( uiUnLock, F(3110,g_tiance_quest[kind][nextlevel].actor_level) )
		else
			SetFalse(uiUnLock)
		end
		SetProgress( uiProgress, progress/g_tiance_quest[kind][nextlevel].progress )
		SetText(uiProgress.transform:Find("Text"),progress.."/"..g_tiance_quest[kind][nextlevel].progress)
		SetTrue(uiProgress)
		if GetPlayer().m_silver < g_tiance_quest[kind][nextlevel].silver then
			SetRichText( uiSilver, "<icon=silver>"..knum(g_tiance_quest[kind][nextlevel].silver).."/<color=e80017ff>"..knum(GetPlayer().m_silver).."</color>" )
		else
			SetRichText( uiSilver, "<icon=silver>"..knum(g_tiance_quest[kind][nextlevel].silver).."/"..knum(GetPlayer().m_silver) )
		end
		if GetPlayer().m_wood < g_tiance_quest[kind][nextlevel].wood then
			SetRichText( uiWood, "<icon=wood>"..knum(g_tiance_quest[kind][nextlevel].wood).."/<color=e80017ff>"..knum(GetPlayer().m_wood).."</color>" )
		else
			SetRichText( uiWood, "<icon=wood>"..knum(g_tiance_quest[kind][nextlevel].wood).."/"..knum(GetPlayer().m_wood) )
		end
		if GetPlayer().m_food < g_tiance_quest[kind][nextlevel].food then
			SetRichText( uiFood, "<icon=food>"..knum(g_tiance_quest[kind][nextlevel].food).."/<color=e80017ff>"..knum(GetPlayer().m_food).."</color>" )
		else
			SetRichText( uiFood, "<icon=food>"..knum(g_tiance_quest[kind][nextlevel].food).."/"..knum(GetPlayer().m_food) )
		end
		if GetPlayer().m_iron < g_tiance_quest[kind][nextlevel].iron then
			SetRichText( uiIron, "<icon=iron>"..knum(g_tiance_quest[kind][nextlevel].iron).."/<color=e80017ff>"..knum(GetPlayer().m_iron).."</color>" )
		else
			SetRichText( uiIron, "<icon=iron>"..knum(g_tiance_quest[kind][nextlevel].iron).."/"..knum(GetPlayer().m_iron) )
		end
		SetTrue(uiCostBack)
		SetFalse(uiWarning)
		SetTrue(uiDonateButton)
	end
end

function HeroLevyDlgDonate( kind )
	local nextlevel = m_RecvValue.m_tc_level[kind]+1
	if GetPlayer().m_level < g_tiance_quest[kind][nextlevel].actor_level then
		pop( F(3110,g_tiance_quest[kind][nextlevel].actor_level) )
		return
	end
	if GetPlayer().m_silver < g_tiance_quest[kind][nextlevel].silver then
		JumpRes( 1 )
		return
	end
	if GetPlayer().m_wood < g_tiance_quest[kind][nextlevel].wood then
		JumpRes( 2 )
		return
	end
	if GetPlayer().m_food < g_tiance_quest[kind][nextlevel].food then
		JumpRes( 3 )
		return
	end
	if GetPlayer().m_iron < g_tiance_quest[kind][nextlevel].iron then
		JumpRes( 4 )
		return
	end
	system_askinfo( ASKINFO_TIANCEQUEST, "", 3, kind );
end
