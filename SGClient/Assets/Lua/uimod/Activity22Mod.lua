local m_Mod;
local m_uiBack = nil; --UnityEngine.GameObject
local m_uiMask = nil; --UnityEngine.GameObject
local m_uiBack2 = nil; --UnityEngine.GameObject
local m_uiActivityTime = nil; --UnityEngine.GameObject
local m_uiGotoBtn = nil; --UnityEngine.GameObject
local m_uiRankBtn = nil; --UnityEngine.GameObject
g_Activity22Open = 0;
----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function Activity22ModOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
		--print( "Button Clicked, nControlID:" .. nControlID );
		if nControlID == 1 then
			Activity22ModGoto()
		elseif nControlID == 2 then
			Activity22RankDlgShow()
		end
	elseif nType == UI_EVENT_PRESS then
		if value == 0 then
			--print( "Button Pressed Down, nControlID:" .. nControlID );
		elseif value == 1 then
			--print( "Button Pressed UP, nControlID:" .. nControlID);
		end
	end
end

-- 载入时调用
function Activity22ModOnAwake( gameObject )
	m_Mod = gameObject;
	local objs = gameObject:GetComponent( "UIMod" ).relatedGameObject;
	m_uiBack = objs[0];
	m_uiMask = objs[1];
	m_uiBack2 = objs[2];
	m_uiActivityTime = objs[3];
	m_uiGotoBtn = objs[4];
	m_uiRankBtn = objs[5];
end

-- 界面初始化时调用
function Activity22ModOnStart( gameObject )
	ResourceManager.LoadAssetBundle( "_ab_activity_pic_16" )
	ResourceManager.LoadAssetBundle( "_ab_activity_pic_21" )
	ResourceManager.LoadAssetBundle( "_ab_activity_back_4" )
	
	SetImage( m_uiBack, LoadSprite("activity_pic_16") )
	SetImage( m_uiMask.transform:Find("Shape"), LoadSprite("activity_pic_21") )
	SetImage( m_uiBack2, LoadSprite("activity_back_4") )
end

-- 界面显示时调用
function Activity22ModOnEnable( gameObject )
	Activity22ModSetTime()
end

-- 界面隐藏时调用
function Activity22ModOnDisable( gameObject )
	
end

-- 界面删除时调用
function Activity22ModOnDestroy( gameObject )
	m_Mod = nil
	ResourceManager.UnloadAssetBundle( "_ab_activity_pic_21" )
	ResourceManager.UnloadAssetBundle( "_ab_activity_pic_16" )
	ResourceManager.UnloadAssetBundle( "_ab_activity_back_4" )
end

-- 每帧调用
function Activity22ModOnLogic( gameObject )
	
end

function Activity22ModGet()
	return m_Mod;
end
----------------------------------------
-- 自定
----------------------------------------
function Activity22ModSetTime()
	if m_Mod == nil then
		return
	end

	local ActivityList = ActivityDlgGetRecvValue()
	if ActivityList == nil then
		return
	end
	local info = nil;
	for i=1, #ActivityList, 1 do
		if ActivityList[i].m_activityid == ACTIVITY_22 then
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
		SetFalse( m_uiRankBtn )
		
	elseif servertime < info.m_endtime then
		SetTimer( m_uiActivityTime.transform:Find("Time"), info.m_endtime-servertime, info.m_endtime-servertime, 0, T(4268) )
		SetTrue( m_uiActivityTime.transform:Find("Time") )
		SetFalse( m_uiActivityTime.transform:Find("TimeOver") )
		SetTrue( m_uiRankBtn )
	else
		SetFalse( m_uiActivityTime.transform:Find("Time") )
		SetTrue( m_uiActivityTime.transform:Find("TimeOver") )
		SetText( m_uiActivityTime.transform:Find("TimeOver"), T(4269) )
		SetTrue( m_uiRankBtn )
	end
end

function Activity22ModRecv()
	if m_Mod == nil then
		return
	end	
end

function Activity22ModGoto()
	--if GameManager.currentScence == "city" then
		WorldMap.GotoWorldMap(150, 150)
		ActivityDlgClose()
	--end
end
