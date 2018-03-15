-- 界面
local m_Dlg = nil;
local m_DialogFrameMod = nil;

local m_uiScroll = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiUIP_Quest = nil; --UnityEngine.GameObject

local m_ObjectPool = nil;

-- 打开界面
function QuestDlgOpen()
	m_Dlg = eye.uiManager:Open( "QuestDlg" );
	m_DialogFrameMod = DialogFrameModOpen( m_Dlg, T(551), HELP_QuestDlg, QuestDlgClose );
end

-- 隐藏界面
function QuestDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	DialogFrameModClose( m_DialogFrameMod );
	m_DialogFrameMod = nil;
	eye.uiManager:Close( "QuestDlg" );
end

-- 删除界面
function QuestDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function QuestDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            QuestDlgClose();
		elseif nControlID >= 0 and nControlID <= 10 then
			QuestDlgGoto( nControlID )
		elseif nControlID >= 100 and nControlID <= 200 then
			QuestDlgGetAward( nControlID-100 )
        end
	end
end

-- 载入时调用
function QuestDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiScroll = objs[0];
	m_uiContent = objs[1];
	m_uiUIP_Quest = objs[2];
	
	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );
	m_ObjectPool:CreatePool("UIP_Quest", 6, 6, m_uiUIP_Quest);
end

-- 界面初始化时调用
function QuestDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function QuestDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function QuestDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function QuestDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function QuestDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
-- m_count=0,m_list={m_questid=0,m_flag=0,m_datatype=0,m_datakind=0,m_dataoffset=0,m_value=0,m_needvalue=0,m_awardkind={[5]},m_awardnum={[5]},m_nameid=0,[m_count]},
function QuestDlgShow()
	QuestDlgOpen()
	system_askinfo( ASKINFO_QUEST, "", 0 );
--[[	QuestDlgClear()
	for i=1, CacheQuest.m_count, 1 do
		QuestDlgSetObject( i, CacheQuest.m_list[i] )
	end--]]
end

function QuestDlgClear()
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

function QuestDlgSetObject( offset, recvValue )
	local uiObj = m_ObjectPool:Get( "UIP_Quest" );
	uiObj.transform:SetParent( m_uiContent.transform );
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiName = objs[0];
	local uiQuest = objs[1];
	local uiAwardList = objs[2];
	local uiGotoButton = objs[3];
	local uiGetButton = objs[4];
	
	-- 任务名
	if offset == 1 then
		SetText( uiName, QuestName( 0, recvValue ) );
	else
		SetText( uiName, QuestName( 1, recvValue ) );
	end
	
	-- 先隐藏
	for i=0, 3, 1 do
		SetFalse( uiAwardList.transform:GetChild(i) )
	end
	-- 奖励
	local index = 0
	for i=1, 4, 1 do
		if recvValue.m_awardkind[i] > 0 then
			local sprite, color, name = AwardInfo( recvValue.m_awardkind[i] )
			local awardObj = uiAwardList.transform:GetChild(index);
			SetTrue( awardObj )
			SetImage( awardObj.transform:Find("Shape"), sprite );
			SetText( awardObj.transform:Find("Name"), name );
			SetText( awardObj.transform:Find("Num"), recvValue.m_awardnum[i] );
			index = index + 1
		end
	end
	
	if recvValue.m_value >= recvValue.m_needvalue then
		SetTrue( uiGetButton )
		SetFalse( uiGotoButton )
		SetControlID( uiGetButton, 100+offset );
	else
		SetFalse( uiGetButton )
		SetTrue( uiGotoButton )
		SetControlID( uiGotoButton, offset );
	end
end

function QuestDlgUpdate()
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	QuestDlgClear()
	for i=1, CacheQuest.m_count, 1 do
		QuestDlgSetObject( i, CacheQuest.m_list[i] )
	end
end

-- 前往
function QuestDlgGoto( index )
	QuestGoto( index )
	QuestDlgClose()
end

function QuestDlgGetAward( index )
	if CacheQuest.m_list[index] == nil then
		return
	end
	if index == 1 then
		if IsGuiding() then
			ForceGuideNext();
		end
	end
	system_askinfo( ASKINFO_QUEST, "", 1, CacheQuest.m_list[index].m_questid );
end
