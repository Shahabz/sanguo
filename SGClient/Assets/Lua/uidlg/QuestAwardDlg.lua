-- 界面
local m_Dlg = nil;
local m_uiQuestName = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiUIP_Award = nil; --UnityEngine.GameObject
local m_ObjectPool = nil;
local m_questid = 0;

-- 打开界面
function QuestAwardDlgOpen()
	ResourceManager.LoadAssetBundle( "_ab_ui_static_npc1" );
	m_Dlg = eye.uiManager:Open( "QuestAwardDlg" );
end

-- 隐藏界面
function QuestAwardDlgClose()
	if m_Dlg == nil then
		return;
	end
	local objs = {};
	for i = 0 ,m_uiContent.transform.childCount - 1 do
		table.insert( objs, m_uiContent.transform:GetChild(i).gameObject )
    end
	for k, v in pairs(objs) do
		m_ObjectPool:Release( "m_uiUIP_Award", v );
    end
	eye.uiManager:Close( "QuestAwardDlg" );
end

-- 删除界面
function QuestAwardDlgDestroy()
	GameObject.Destroy( m_Dlg );
	ResourceManager.UnloadAssetBundle( "_ab_ui_static_npc1" )
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function QuestAwardDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == 1 then
			QuestAwardDlgGet();
        end
	end
end

-- 载入时调用
function QuestAwardDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;
	m_uiQuestName = objs[0];
	m_uiContent = objs[1];
	m_uiUIP_Award = objs[2];
	m_uiGetBtn = objs[3];
	
	-- 对象池
	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );
	m_ObjectPool:CreatePool("m_uiUIP_Award", 3, 3, m_uiUIP_Award);
end

-- 界面初始化时调用
function QuestAwardDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function QuestAwardDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function QuestAwardDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function QuestAwardDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function QuestAwardDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
-- m_questid=0,m_count=0,m_list={m_kind=0,m_num=0,[m_count]},m_datatype=0,m_datakind=0,m_dataoffset=0,m_value=0,m_needvalue=0,m_nameid=0,m_type=0;
function QuestAwardDlgShow( recvValue )
	if m_Dlg ~= nil and IsActive( m_Dlg ) == true then
		return;
	end
	QuestAwardDlgOpen()
	QuestAwardDlgClear()
	m_questid = recvValue.m_questid;
	
	-- 任务名
	SetText( m_uiQuestName, QuestName( recvValue.m_type, recvValue ) );
	-- 奖励
	for i=1, recvValue.m_count, 1 do
		local sprite, color, name = AwardInfo( recvValue.m_list[i].m_kind )
		local uiObj = m_ObjectPool:Get( "m_uiUIP_Award" );
		uiObj.transform:SetParent( m_uiContent.transform );
		SetImage( uiObj.transform:Find("Shape"), sprite );
		SetText( uiObj.transform:Find("Name"), name );
		SetText( uiObj.transform:Find("Num"), recvValue.m_list[i].m_num );
	end
end

-- 清空
function QuestAwardDlgClear()
	local objs = {};
	for i = 0 ,m_uiContent.transform.childCount - 1 do
		table.insert( objs, m_uiContent.transform:GetChild(i).gameObject )
    end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "m_uiUIP_Award(Clone)" then
			m_ObjectPool:Release( "m_uiUIP_Award", obj );
		end
    end
end

-- 领取奖励
function QuestAwardDlgGet()
	if IsGuiding() then
		GuideEnd();
	end
	system_askinfo( ASKINFO_QUEST, "", 1, m_questid );
	QuestAwardDlgClose();
end

function GetQuestAwardBtn()
	return m_uiGetBtn..transform.position;
end