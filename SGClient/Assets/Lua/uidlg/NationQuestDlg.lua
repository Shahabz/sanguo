-- 界面
local m_Dlg = nil;
local m_DialogFrameMod = nil;

local m_uiScroll = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiUIP_Quest = nil; --UnityEngine.GameObject
local m_uiTimer = nil; --UnityEngine.GameObject

local m_ObjectPool = nil
local m_recvValue = nil;
-- 打开界面
function NationQuestDlgOpen()
	m_Dlg = eye.uiManager:Open( "NationQuestDlg" );
	m_DialogFrameMod = DialogFrameModOpen( m_Dlg, T(1945), HELP_NationDlg, NationQuestDlgClose );
end

-- 隐藏界面
function NationQuestDlgClose()
	if m_Dlg == nil then
		return;
	end
	DialogFrameModClose( m_DialogFrameMod );
	m_DialogFrameMod = nil;
	eye.uiManager:Close( "NationQuestDlg" );
	m_recvValue = nil;
end

-- 删除界面
function NationQuestDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
	m_recvValue = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function NationQuestDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            NationQuestDlgClose();
		
		-- 领取奖励
		elseif nControlID >= 100 and nControlID < 200 then
			NationQuestDlgAward( nControlID - 100 )
			
		-- 前往
		elseif nControlID >= 200 and nControlID < 300 then
			NationQuestDlgGoto( nControlID - 200 )
        end
	elseif nType == UI_EVENT_TIMECOUNTEND then
		if nControlID == 0 then
			system_askinfo( ASKINFO_NATION, "", 8 )
		end
	end
end

-- 载入时调用
function NationQuestDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiScroll = objs[0];
	m_uiContent = objs[1];
	m_uiUIP_Quest = objs[2];
	m_uiTimer = objs[3];

	-- 对象池
	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );
	m_ObjectPool:CreatePool("UIP_Quest", 3, 3, m_uiUIP_Quest);
end

-- 界面初始化时调用
function NationQuestDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function NationQuestDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function NationQuestDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function NationQuestDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function NationQuestDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function NationQuestDlgShow()
	NationQuestDlgOpen()
	system_askinfo( ASKINFO_NATION, "", 8 )
end

-- m_count=0,m_list={m_level=0,m_kind=0,m_value=0,m_needvalue=0,m_awardkind={[5]},m_awardnum={[5]},m_other_awardkind=0,m_other_awardnum=0,[m_count]},
function NationQuestDlgRecv( recvValue )
	NationQuestDlgClear()
	m_recvValue = recvValue;
	for i=1, recvValue.m_count, 1 do
		NationQuestDlgCreate( i, recvValue.m_list[i] )
	end
	
	-- 倒计时T(1790)	
	local stamp
	local lefttime
	local tab = os.date("*t", GetServerTime());
	if tab.hour >= global.nation_quest_timer1 and tab.hour < global.nation_quest_timer2 then
		-- 12点刷新
		stamp =  os.time( {year=tab.year, month=tab.month, day=tab.day, hour=global.nation_quest_timer2, min=0,sec=0} )
	elseif tab.hour >= global.nation_quest_timer2 and tab.hour < global.nation_quest_timer3 then
		-- 18点刷新
		stamp =  os.time( {year=tab.year, month=tab.month, day=tab.day, hour=global.nation_quest_timer3, min=0,sec=0} )
	else
		-- 0点刷新
		stamp =  os.time( {year=tab.year, month=tab.month, day=tab.day, hour=23, min=59,sec=59} )
	end
	
	local lefttime = stamp - GetServerTime();
	SetTimer( m_uiTimer, lefttime-1, lefttime, 0, T(1790) )
end

-- 创建
function NationQuestDlgCreate( index, info )
	local uiObj = m_ObjectPool:Get( "UIP_Quest" );
	uiObj.transform:SetParent( m_uiContent.transform );
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiName = objs[0];
	local uiAwardList = objs[1];
	local uiGotoButton = objs[2];
	local uiAwardButton = objs[3];
	local uiAwardWarn = objs[4];
	
	-- 任务名
	local value = info.m_value;
	if value == -1 then
		value = info.m_needvalue
	end
	if info.m_kind == 1 then
		SetText( uiName, F(1787, info.m_needvalue, value, info.m_needvalue) );
	elseif info.m_kind == 2 then
		SetText( uiName, F(1788, info.m_needvalue, value, info.m_needvalue) );
	elseif info.m_kind == 3 then
		SetText( uiName, F(1789, info.m_needvalue, value, info.m_needvalue) );
	end
	
	-- 先隐藏
	for i=0, 4, 1 do
		SetFalse( uiAwardList.transform:GetChild(i) )
	end
	
	-- 奖励
	local step = 0
	for i=1, 5, 1 do
		if info.m_awardkind[i] > 0 then
			local sprite, color, name = AwardInfo( info.m_awardkind[i] )
			local awardObj = uiAwardList.transform:GetChild(step);
			SetTrue( awardObj )
			SetImage( awardObj.transform:Find("Shape"), sprite );
			SetText( awardObj.transform:Find("Name"), name );
			SetText( awardObj.transform:Find("Num"), knum(info.m_awardnum[i]) );
			step = step + 1
		end
	end
	
	-- 已经领取
	if info.m_value == -1 then
		SetFalse( uiGotoButton )
		SetFalse( uiAwardButton )
		SetTrue( uiAwardWarn )
		
	-- 已经完成
	elseif info.m_value >= info.m_needvalue then
		SetFalse( uiGotoButton )
		SetTrue( uiAwardButton )
		SetFalse( uiAwardWarn )
		SetControlID( uiAwardButton, 100+index );
		
	-- 前往按钮
	else
		SetTrue( uiGotoButton )
		SetFalse( uiAwardButton )
		SetFalse( uiAwardWarn )
		SetControlID( uiGotoButton, 200+index );
	end
	
end

-- 清空
function NationQuestDlgClear()
	local objs = {};
	for i = 0 ,m_uiContent.transform.childCount - 1 do
		table.insert( objs, m_uiContent.transform:GetChild(i).gameObject )
    end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "UIP_Quest(Clone)" then
			m_ObjectPool:Release( "UIP_Quest", obj );
		end
    end
end

-- 领取奖励
function NationQuestDlgAward( index )
	system_askinfo( ASKINFO_NATION, "", 9, index )
end

-- 前往
function NationQuestDlgGoto( index )
	if GameManager.currentScence == "city" then
		WorldMap.GotoWorldMap(-1, -1)
	end
	NationQuestDlgClose()
	NationDlgClose()
end
