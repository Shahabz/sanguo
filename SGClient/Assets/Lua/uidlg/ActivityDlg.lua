-- 界面
local m_Dlg = nil;
local m_uiActivityScroll = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiUIP_ActivityName = nil; --UnityEngine.GameObject
local m_uiUpBtn = nil; --UnityEngine.GameObject
local m_uiDownBtn = nil; --UnityEngine.GameObject
local m_uiTokenText = nil; --UnityEngine.GameObject
local m_uiActivityLayer = nil; --UnityEngine.GameObject

local m_ObjectPool = nil
local m_WaitSelectActivityID = 0;
local m_SelectActivityID = 0
local m_recvValue;
local m_ActivityMod = nil

ACTIVITY_1		=	1	--	首充礼包
ACTIVITY_2		=	2	--	主城等级
ACTIVITY_3		=	3	--	攻城略地
ACTIVITY_4		=	4	--	七日狂欢
ACTIVITY_5		=	5	--	七星拜将
ACTIVITY_6		=	6	--	全服返利
ACTIVITY_7		=	7	--	大咖带队
ACTIVITY_8		=	8	--	成长计划
ACTIVITY_9		=	9	--	幸运罗盘
ACTIVITY_PAYBAG	=	10	--	特价礼包
ACTIVITY_11		=	11	--	出师大宴
ACTIVITY_12		=	12	--	副本掉落
ACTIVITY_13		=	13	--	流寇图纸
ACTIVITY_14		=	14	--	流寇资源
ACTIVITY_15		=	15	--	经验翻倍
ACTIVITY_16		=	16	--	作坊加速
ACTIVITY_17		=	17	--	充值豪礼
ACTIVITY_18	=	18	--	镔铁转盘
ACTIVITY_19	=	19	--	采集翻倍
ACTIVITY_20	=	20	--	屯田活动
ACTIVITY_21	=	21	--	国器暴击
ACTIVITY_22	=	22	--	皇城采集
ACTIVITY_23	=	23	--	官员召唤
ACTIVITY_24	=	24	--	募兵翻倍
ACTIVITY_25	=	25	--	首日免费
ACTIVITY_26	=	26	--	强国策
ACTIVITY_27	=	27	--	西凉暴乱
ACTIVITY_28	=	28	--	黄巾军暴乱
ACTIVITY_29	=	29	--	体力大卖
ACTIVITY_30	=	30	--	图纸活动
ACTIVITY_31	=	31	--	全民寻宝
ACTIVITY_32	=	32	--	抢夺名城
ACTIVITY_33	=	33	--	充值排行
ACTIVITY_34	=	34	--	屯银排行
ACTIVITY_35	=	35	--	屯木排行
ACTIVITY_36	=	36	--	屯粮排行
ACTIVITY_37	=	37	--	屯铁排行
ACTIVITY_38	=	38	--	国战排行
ACTIVITY_39	=	39	--	城战排行
ACTIVITY_40	=	40	--	洗练排行
ACTIVITY_41	=	41	--	建设排行
ACTIVITY_42	=	42	--	锻造排行
ACTIVITY_43	=	43	--	充值排行
ACTIVITY_44	=	44	--	兵力排行

-- 打开界面
function ActivityDlgOpen()
	m_Dlg = eye.uiManager:Open( "ActivityDlg" );
end

-- 隐藏界面
function ActivityDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "ActivityDlg" );
	m_recvValue = nil
	ActivityDlgActivityModHide()
	m_WaitSelectActivityID = 0;
	m_SelectActivityID = 0
end

-- 删除界面
function ActivityDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
	m_recvValue = nil
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function ActivityDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            ActivityDlgClose();
		elseif nControlID >= 1000 and nControlID < 2000 then
			ActivityDlgSelect( nControlID-1000 )
        end
	end
end

-- 载入时调用
function ActivityDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiActivityScroll = objs[0];
	m_uiContent = objs[1];
	m_uiUIP_ActivityName = objs[2];
	m_uiUpBtn = objs[3];
	m_uiDownBtn = objs[4];
	m_uiTokenText = objs[5];
	m_uiActivityLayer = objs[6];
	
	-- 对象池
	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );
	m_ObjectPool:CreatePool("UIP_ActivityName", 5, 5, m_uiUIP_ActivityName);
end

-- 界面初始化时调用
function ActivityDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function ActivityDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function ActivityDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function ActivityDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function ActivityDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function ActivityDlgShow()
	ActivityDlgOpen()
	ActivityDlgChangeToken()
	system_askinfo( ASKINFO_ACTIVITY, "", 0 )
	m_SelectActivityID = 0
	m_WaitSelectActivityID = 0;
end

function ActivityDlgShowByID( id ) 
	ActivityDlgOpen()
	ActivityDlgChangeToken()
	system_askinfo( ASKINFO_ACTIVITY, "", 0 )
	m_WaitSelectActivityID = id;
end

-- m_count=0,m_list={m_activityid=0,m_starttime=0,m_endtime=0,m_closetime=0[m_count]},
function ActivityDlgRecv( recvValue )
	ActivityDlgClear()
	m_recvValue = {}
	for i=1, recvValue.m_count, 1 do
		table.insert( m_recvValue, recvValue.m_list[i] );
	end
	for i=1, #m_recvValue do
		ActivityDlgSetObj( i, m_recvValue[i] )
	end	
end

-- 设置活动
function ActivityDlgSetObj( index, info )
	local uiObj = m_ObjectPool:Get("UIP_ActivityName");
	uiObj.transform:SetParent( m_uiContent.transform );
	uiObj.transform.localScale = Vector3.one;
	info.m_uiObj = uiObj;
	info.m_index = index;
	
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiName = objs[0]
	local uiLock = objs[1]
	local uiWarn = objs[2]
	local uiSelect = objs[3]
	info.m_uiSelect = uiSelect;
	
	SetControlID( uiObj, 1000 + index )
	SetText( uiName, ActivityName( info.m_activityid ) )
	if GetServerTime() < info.m_starttime then
		SetTrue( uiLock )
	else
		SetFalse( uiLock )
	end
	if info.m_red == 1 then
		SetTrue( uiWarn )
	else
		SetFalse( uiWarn )
	end
	SetFalse( uiSelect )
	if m_WaitSelectActivityID > 0 then
		if m_WaitSelectActivityID == info.m_activityid then
			SetTrue( uiSelect )
			ActivityDlgActivityModShow( info.m_activityid )
		end
	elseif index == 1 then
		SetTrue( uiSelect )
		ActivityDlgActivityModShow( info.m_activityid )
	end
end

--清空
function ActivityDlgClear()
	local objs = {};
	for i=0,m_uiContent.transform.childCount-1 do
		table.insert(objs,m_uiContent.transform:GetChild(i).gameObject);
	end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "UIP_ActivityName(Clone)" then
			m_ObjectPool:Release( "UIP_ActivityName", obj );
		end
	end
end

-- 选择活动
function ActivityDlgSelect( index )
	if m_recvValue == nil or m_recvValue[index] == nil then
		return
	end
	for i=1,#m_recvValue do
		SetFalse( m_recvValue[i].m_uiSelect )
	end
	local uiObj = m_recvValue[index].m_uiObj;
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiSelect = objs[3]
	SetTrue( uiSelect )
	ActivityDlgActivityModShow( m_recvValue[index].m_activityid )
end

-- 显示活动详细信息页面
function ActivityDlgActivityModShow( activityid )
	if activityid == m_SelectActivityID then
		return
	end
	ActivityDlgActivityModHide()
	local prefab = LoadPrefab( "Activity"..activityid.."Mod" );
	m_ActivityMod = GameObject.Instantiate( prefab );
	if m_ActivityMod == nil then
		return
	end
	m_ActivityMod.transform:SetParent( m_uiActivityLayer.transform );
	m_ActivityMod.transform.localPosition = Vector3.zero;
	m_ActivityMod.transform.localScale = Vector3.one;
	m_ActivityMod.transform.sizeDelta = Vector2( 0, 0 );
	m_SelectActivityID = activityid
end

-- 隐藏活动详细信息页面
function ActivityDlgActivityModHide()
	if m_ActivityMod then
		GameObject.Destroy( m_ActivityMod.gameObject );
		m_ActivityMod = nil
	end
end

-- 更新钻石
function ActivityDlgChangeToken()
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	SetText( m_uiTokenText, GetPlayer().m_token );
end

-- 更新活动红点
function ActivityDlgChangeRed( activityid, show )
	local ActivityList = ActivityDlgGetRecvValue()
	local info = nil;
	for i=1, #ActivityList, 1 do
		if ActivityList[i].m_activityid == activityid then
			info = ActivityList[i];
			break
		end
	end
	if info == nil then
		return
	end	
	local objs = info.m_uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiWarn = objs[2]
	if show == 1 then
		SetTrue( uiWarn )
	else
		SetFalse( uiWarn )
	end
end

--
function ActivityDlgGetRecvValue()
	return m_recvValue
end

-- 活动名
function ActivityName( activityid )
	return T( 2500 + activityid )
end
