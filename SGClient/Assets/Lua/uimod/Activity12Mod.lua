local m_Mod;
local m_uiBack = nil; --UnityEngine.GameObject
local m_uiEnemy1 = nil; --UnityEngine.GameObject
local m_uiBack2 = nil; --UnityEngine.GameObject
local m_uiActivityTime = nil; --UnityEngine.GameObject
local m_uiTurn = nil; --UnityEngine.GameObject
local m_uiArrived = nil; --UnityEngine.GameObject
local m_uiOpenBtn = nil; --UnityEngine.GameObject
local m_uiGotoBtn = nil; --UnityEngine.GameObject
local m_uiAwardList = nil; --UnityEngine.GameObject
local m_uiAward = nil; --UnityEngine.GameObject
local m_ActivityStart = 0;
----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function Activity12ModOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
		--print( "Button Clicked, nControlID:" .. nControlID );
		if nControlID == 1 then
			Activity12ModManualOpen()
		elseif nControlID == 2 then
			Activity12ModGoto()
		end
	elseif nType == UI_EVENT_PRESS then
		if value == 0 then
			--print( "Button Pressed Down, nControlID:" .. nControlID );
		elseif value == 1 then
			--print( "Button Pressed UP, nControlID:" .. nControlID);
		end
	elseif nType == UI_EVENT_TIMECOUNTEND then
		if nControlID == 0 then
			Activity12ModSetTime()
		end
	end
end

-- 载入时调用
function Activity12ModOnAwake( gameObject )
	m_Mod = gameObject;
	local objs = gameObject:GetComponent( "UIMod" ).relatedGameObject;
	m_uiBack = objs[0];
	m_uiEnemy1 = objs[1];
	m_uiBack2 = objs[2];
	m_uiActivityTime = objs[3];
	m_uiTurn = objs[4];
	m_uiArrived = objs[5];
	m_uiOpenBtn = objs[6];
	m_uiGotoBtn = objs[7];
	m_uiAwardList = objs[8];
	m_uiAward = objs[9];
end

-- 界面初始化时调用
function Activity12ModOnStart( gameObject )
	system_askinfo( ASKINFO_ACTIVITY, "", ACTIVITY_12, 0 )
	ResourceManager.LoadAssetBundle( "_ab_activity_pic_16" )
	ResourceManager.LoadAssetBundle( "_ab_activity_back_4" )
	
	SetImage( m_uiBack, LoadSprite("activity_pic_16") )
	SetImage( m_uiBack2, LoadSprite("activity_back_4") )
	SetFalse( m_uiTurn )
	SetFalse( m_uiArrived )
	SetFalse( m_uiOpenBtn )
	SetFalse( m_uiGotoBtn )
	SetFalse( m_uiAwardList )
	SetFalse( m_uiAward )
	Activity12ModSetTime()
end

-- 界面显示时调用
function Activity12ModOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function Activity12ModOnDisable( gameObject )
	
end

-- 界面删除时调用
function Activity12ModOnDestroy( gameObject )
	m_Mod = nil
end

-- 每帧调用
function Activity12ModOnLogic( gameObject )
	
end

function Activity12ModGet()
	return m_Mod;
end
----------------------------------------
-- 自定
----------------------------------------
function Activity12ModSetTime()
	if m_Mod == nil then
		return
	end

	local ActivityList = ActivityDlgGetRecvValue()
	if ActivityList == nil then
		return
	end
	local info = nil;
	for i=1, #ActivityList, 1 do
		if ActivityList[i].m_activityid == ACTIVITY_12 then
			info = ActivityList[i];
			break
		end
	end
	if info == nil then
		return
	end
	-- 活动时间
	local servertime = GetServerTime();
	if servertime < info.m_starttime then
		SetTimer( m_uiActivityTime.transform:Find("Time"), info.m_starttime-servertime, info.m_starttime-servertime, 0, T(4267) )
		SetTrue( m_uiActivityTime.transform:Find("Time") )
		SetFalse( m_uiActivityTime.transform:Find("TimeOver") )
		m_ActivityStart = 0;
		
	elseif servertime < info.m_endtime then
		SetTimer( m_uiActivityTime.transform:Find("Time"), info.m_endtime-servertime, info.m_endtime-servertime, 0, T(4268) )
		SetTrue( m_uiActivityTime.transform:Find("Time") )
		SetFalse( m_uiActivityTime.transform:Find("TimeOver") )
		m_ActivityStart = 1;
	else
		SetFalse( m_uiActivityTime.transform:Find("Time") )
		SetTrue( m_uiActivityTime.transform:Find("TimeOver") )
		SetText( m_uiActivityTime.transform:Find("TimeOver"), T(4269) )
		m_ActivityStart = 0;
		
	end
end

function Activity12ModRecv( state, turn, statetime, stateduration )
	if m_Mod == nil then
		return
	end
	SetFalse( m_uiTurn )
	SetFalse( m_uiArrived )
	SetFalse( m_uiOpenBtn )
	SetFalse( m_uiGotoBtn )
	SetFalse( m_uiAwardList )
	SetFalse( m_uiAward )
	
	if state == 0 then
		SetTrue( m_uiOpenBtn )

	elseif state == 1 then
		SetTrue( m_uiTurn )
		SetTrue( m_uiArrived )
		SetTrue( m_uiGotoBtn )
		SetText( m_uiTurn, F(4273,turn) )
		SetTimer( m_uiArrived, stateduration-statetime, stateduration, 0, T(4274) )
		
	else
		SetTrue( m_uiTurn )
		SetText( m_uiTurn, F(4275,turn-1) )
	end
end

-- 获取奖励
-- m_count=0,m_list={m_kind=0,m_num=0,[m_count]},m_callback_code=0,m_value
function Activity12ModRecvAward( recvValue )
	if m_Mod == nil then
		return
	end
	
	SetTrue( m_uiAwardList )
	SetTrue( m_uiAward )
	
	for i = 0 ,m_uiAwardList.transform.childCount - 1 do
		local objs = m_uiAwardList.transform:GetChild(i).gameObject
		SetFalse( objs );
    end
	
	local count = math.min( recvValue.m_count, m_uiAwardList.transform.childCount );
	for i = 0 ,count - 1 do
		local uiObj = m_uiAwardList.transform:GetChild(i).gameObject
		SetTrue( uiObj );
		local sprite, color, name,c = AwardInfo( recvValue.m_list[i+1].m_kind )
		SetImage( uiObj.transform:Find("Shape"), sprite );
		SetImage( uiObj.transform:Find("Color"), color );
		if recvValue.m_list[i+1].m_num > 1 then
			SetText( uiObj.transform:Find("Num"), knum(recvValue.m_list[i+1].m_num) );
			SetTrue( uiObj.transform:Find("NumBack") )
		else
			SetText( uiObj.transform:Find("Num"), "" );
			SetFalse( uiObj.transform:Find("NumBack") )
		end
    end
	
end
	
function Activity12ModManualOpen()
	if m_ActivityStart == 0 then
		pop( T(4277) )
		return
	end
	MsgBox( T(4281), function()
		system_askinfo( ASKINFO_ACTIVITY, "", ACTIVITY_12, 1 )
	end )
end

function Activity12ModGoto()
	ActivityDlgClose()
	if GameManager.currentScence == "city" then
		WorldMap.GotoWorldMap(-1, -1)
	end
end
