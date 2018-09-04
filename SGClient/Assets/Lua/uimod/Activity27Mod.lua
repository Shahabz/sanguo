local m_Mod;
local m_uiBack = nil; --UnityEngine.GameObject
local m_uiEnemy1 = nil; --UnityEngine.GameObject
local m_uiEnemy2 = nil; --UnityEngine.GameObject
local m_uiBack2 = nil; --UnityEngine.GameObject
local m_uiActivityTime = nil; --UnityEngine.GameObject
----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function Activity27ModOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
		--print( "Button Clicked, nControlID:" .. nControlID );
		if nControlID == 1 then
			ActivityDlgClose()
			if GameManager.currentScence == "city" then
				WorldMap.GotoWorldMap(-1, -1)
			end
		end
	elseif nType == UI_EVENT_PRESS then
		if value == 0 then
			--print( "Button Pressed Down, nControlID:" .. nControlID );
		elseif value == 1 then
			--print( "Button Pressed UP, nControlID:" .. nControlID);
		end
	
	elseif nType == UI_EVENT_TIMECOUNTEND then
		if nControlID == 0 then
			Activity27ModSetTime()
		end
	end
end

-- 载入时调用
function Activity27ModOnAwake( gameObject )
	m_Mod = gameObject;
	local objs = gameObject:GetComponent( "UIMod" ).relatedGameObject;
	m_uiBack = objs[0];
	m_uiEnemy1 = objs[1];
	m_uiEnemy2 = objs[2];
	m_uiBack2 = objs[3];
	m_uiActivityTime = objs[4];
end

-- 界面初始化时调用
function Activity27ModOnStart( gameObject )
	system_askinfo( ASKINFO_ACTIVITY, "", ACTIVITY_27, 0 )
	ResourceManager.LoadAssetBundle( "_ab_activity_pic_12" )
	ResourceManager.LoadAssetBundle( "_ab_activity_back_3" )
	
	SetImage( m_uiBack, LoadSprite("activity_pic_12") )
	SetImage( m_uiBack2, LoadSprite("activity_back_3") )
	Activity27ModSetTime()
end

-- 界面显示时调用
function Activity27ModOnEnable( gameObject )

end

-- 界面隐藏时调用
function Activity27ModOnDisable( gameObject )
	
end

-- 界面删除时调用
function Activity27ModOnDestroy( gameObject )
	m_Mod = nil
end

-- 每帧调用
function Activity27ModOnLogic( gameObject )
	
end

function Activity27ModGet()
	return m_Mod;
end
----------------------------------------
-- 自定
----------------------------------------
function Activity27ModSetTime()
	if m_Mod == nil then
		return
	end

	local ActivityList = ActivityDlgGetRecvValue()
	if ActivityList == nil then
		return
	end
	local info = nil;
	for i=1, #ActivityList, 1 do
		if ActivityList[i].m_activityid == ACTIVITY_27 then
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
		SetFalse( m_uiActivityTime.transform:Find("Text") )
		SetFalse( m_uiActivityTime.transform:Find("TimeOver") )
		
	elseif servertime < info.m_endtime then
		SetTimer( m_uiActivityTime.transform:Find("Time"), info.m_endtime-servertime, info.m_endtime-servertime, 0, T(4268) )
		SetTrue( m_uiActivityTime.transform:Find("Time") )
		SetTrue( m_uiActivityTime.transform:Find("Text") )
		SetFalse( m_uiActivityTime.transform:Find("TimeOver") )
		
	else
		SetFalse( m_uiActivityTime.transform:Find("Time") )
		SetFalse( m_uiActivityTime.transform:Find("Text") )
		SetTrue( m_uiActivityTime.transform:Find("TimeOver") )
		SetText( m_uiActivityTime.transform:Find("TimeOver"), T(4269) )
	end
end

function Activity27ModRecv()
	if m_Mod == nil then
		return
	end

	
end

