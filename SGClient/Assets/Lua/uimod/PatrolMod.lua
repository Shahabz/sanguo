
local m_Mod = nil;
local m_PrefabTab = {};
local m_IsTalkShow = false;
local m_IsStop = false;

-- level 需要等级 ,bx起始点x ,by起始点y ,ex结束点x ,ey结束点y ,sec路程时长, Delay 等待时长
g_patrol_path = {
[1] = { level = 1, Delay = 2,  path = { scale = 1, id=1, bx=19.13,by=-3.57,ex=7.30, ey=-9.55,sec=50} },
[2] = { level = 25, Delay = 2,  path = { scale = 1, id=2, bx=11.64,by=-5.96,ex=15.69,ey=-8.00,sec=25} },
[3] = { level = 30, Delay = 2,  path = { scale = 1, id=3, bx=23.10,by=-4.81,ex=17.79,ey=-7.45,sec=25}  },
[4] = { level = 30, Delay = 2,  path = { scale = 1, id=4, bx=1.83, by=-7.80,ex=5.08, ey=-9.45,sec=15} },
[5] = { level = 40, Delay = 2,  path = { scale = 1, id=5, bx=15.83,by=-0.73,ex=19.06,ey=-2.35,sec=15} },
[6] = { level = 40, Delay = 2,  path = { scale = 1, id=6, bx=16.2, by=-8.24, ex=10.04,ey=-11.31,sec=25} },
[7] = { level = 50, Delay = 2,  path = { scale = 1, id=7, bx=6.69, by=-10.20,ex=8.96, ey=-11.34,sec=15} },
[8] = { level = 50, Delay = 2,  path = { scale = 1, id=8, bx=20.63,by=-3.15, ex=23.02,ey=-4.36, sec=15} },
}

g_patrol_talk = {
[1] = { id = 1 ,textid = 4901 , sec = 10, minlevel = 1, maxlevel = 150, rights = 100 },
[2] = { id = 2 ,textid = 4902 , sec = 10, minlevel = 1, maxlevel = 150, rights = 100 },
[3] = { id = 3 ,textid = 4903 , sec = 10, minlevel = 1, maxlevel = 150, rights = 100 },
[4] = { id = 4 ,textid = 4904 , sec = 10, minlevel = 1, maxlevel = 150, rights = 100 },
[5] = { id = 5 ,textid = 4905 , sec = 10, minlevel = 1, maxlevel = 150, rights = 100 },
[6] = { id = 6 ,textid = 4906 , sec = 10, minlevel = 1, maxlevel = 150, rights = 100},
[7] = { id = 7 ,textid = 4907 , sec = 10, minlevel = 1, maxlevel = 150, rights = 100 },
[8] = { id = 8 ,textid = 4908 , sec = 10, minlevel = 1, maxlevel = 150, rights = 100 },
[9] = { id = 9 ,textid = 4909 , sec = 10, minlevel = 1, maxlevel = 30, rights = 500 },
[10] = { id = 10 ,textid = 4910 , sec = 10, minlevel = 1, maxlevel = 50, rights = 500 },
[11] = { id = 11 ,textid = 4911 , sec = 10, minlevel = 25, maxlevel = 80, rights = 200 },
[12] = { id = 12 ,textid = 4912 , sec = 10, minlevel = 20, maxlevel = 80, rights = 200 },
[13] = { id = 13 ,textid = 4913 , sec = 10, minlevel = 30, maxlevel = 100, rights = 500 },
[14] = { id = 14 ,textid = 4914 , sec = 10, minlevel = 20, maxlevel = 50, rights = 200 },
[15] = { id = 15 ,textid = 4915 , sec = 10, minlevel = 1, maxlevel = 50, rights = 200 },
[16] = { id = 16 ,textid = 4916 , sec = 10, minlevel = 40, maxlevel = 100, rights = 100 },
[17] = { id = 17 ,textid = 4917 , sec = 10, minlevel = 50, maxlevel = 100, rights = 200 },
[18] = { id = 18 ,textid = 4918 , sec = 10, minlevel = 60, maxlevel = 100, rights = 100 },
[19] = { id = 19 ,textid = 4919 , sec = 10, minlevel = 80, maxlevel = 150, rights = 500 },
}

-- 所属按钮点击时调用
function PatrolModOnEvent( nType, nControlID, value )
    if nType == UI_EVENT_CLICK then
		
    elseif nType == UI_EVENT_TWEENFINISH then
		PatrolModWaitPlay(nControlID);		
    elseif nType == UI_EVENT_TIMECOUNTEND then
		if nControlID >= 10 and nControlID <= 20 then 
			PatrolModRePlay(nControlID-10)
		elseif nControlID >= 100 and nControlID <= 110 then 
			m_IsTalkShow = false;
			PatrolModShowTalk(nControlID -100,m_IsTalkShow)
		end
    end
end

-- 载入时调用
function PatrolModOnAwake( gameObject )
	m_Mod = gameObject;
	local objs = gameObject:GetComponent( "UIMod" ).relatedGameObject;
	for i = 0,7 do 
		local prefab = objs[i];
		table.insert(m_PrefabTab,prefab);
		SetFalse(m_PrefabTab[i+1]);
	end
end

-- 界面初始化时调用
function PatrolModOnStart( gameObject )
	
end

-- 界面显示时调用
function PatrolModOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function PatrolModOnDisable( gameObject )
	
end

-- 界面删除时调用
function PatrolModOnDestroy( gameObject )
	
end

-- 每帧调用
function PatrolModOnLogic( gameObject )
end

-- 初始化
function PatrolModInit()
	PatrolModPlay();
end

-- 切换地图场景暂停
function PatrolModStop()
	for i = 1,#g_patrol_path do 
		SetFalse(m_PrefabTab[i]);
		local unitTween = m_PrefabTab[i].transform:GetComponent( "UITweenMove" );
		unitTween:Kill(false);
	end
	m_IsStop = true;
end
-- 设置层级
function PatrolModSetSiblingIndex()
	local Index = City.BuildingRoot().transform.childCount; 
	m_Mod.transform:SetSiblingIndex(Index + 1)
end
function PatrolModPlay()
	if GameManager.currentScence ~= "city" then return end;
	for i = 1,5 do 
		if g_patrol_path[i].level <= GetPlayer().m_level then 
			PatrolModTween(g_patrol_path[i].path);
		end
	end	
	m_IsStop = false;
end
function PatrolModTween(PathData)
	SetTrue(m_PrefabTab[PathData.id])
	local unitTween = m_PrefabTab[PathData.id].transform:GetComponent( "UITweenMove" );
	local unitRect = m_PrefabTab[PathData.id].transform:GetComponent( "RectTransform" );
	unitRect.localScale = Vector3.New( PathData.scale, PathData.scale, PathData.scale );
	startPos = Vector2.New( PathData.bx, PathData.by );
	targetPos = Vector2.New( PathData.ex, PathData.ey );
	unitTween.from = startPos;
	unitTween.to = targetPos;
	unitTween.duration = PathData.sec;
	unitTween:Play(true);
	if m_IsTalkShow == false then 
		PatrolModShowTalk(PathData.id);
	end
end
function PatrolModRePlay(id)		
	if m_IsStop == true then return end;
	PatrolModTween(g_patrol_path[id].path);
end
function PatrolModWaitPlay(id)
	SetFalse(m_PrefabTab[id]);
	local unitTween = m_PrefabTab[id].transform:GetComponent( "UITweenMove" );
	unitTween:Kill(false);
	local NextId = PatrolModGetNextID(id);	
	local Delay = g_patrol_path[NextId].Delay;
	SetTimer(m_PrefabTab[id],Delay,GetServerTime()+Delay,NextId+10);
end

-- 获取下一路线ID
function PatrolModGetNextID(id )
	local NextID = nil;
	if id <= 2 then 
		NextId = id;
	elseif id == 3 then  
		NextId = 6;
	elseif id == 4 then  
		NextId = 7;
	elseif id == 5 then  
		NextId = 8;
	elseif id == 6 then  
		NextId = 3;
	elseif id == 7 then  
		NextId = 4;
	elseif id == 8 then  
		NextId = 5;
	end
	return NextId;
end

-- 巡逻兵展示对话
function PatrolModShowTalk(PathID,bShow)
	if m_IsStop == true then return end;
	local uiObj = m_PrefabTab[PathID];
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiTipWnd = objs[0];
	local uiText = objs[1];
	if bShow ==  false then 
		SetFalse(uiTipWnd);
	else
		local TalkID = PatrolModGetTalkID();	
		SetTrue(uiTipWnd)
		SetText(uiText,T(g_patrol_talk[TalkID].textid));
		local Delay = g_patrol_talk[TalkID].sec;
		SetTimer(uiTipWnd,Delay,GetServerTime()+Delay,PathID+100);
		m_IsTalkShow = true;
	end
	
end

-- 获取对话ID
function PatrolModGetTalkID()
	local TempTab = {};
	local Level = GetPlayer().m_level;
	for i = 1, #g_patrol_talk do 
		local minlevel = g_patrol_talk[i].minlevel;
		local maxlevel = g_patrol_talk[i].maxlevel;
		if Level >= minlevel and Level <= maxlevel then 
			table.insert(TempTab,g_patrol_talk[i]);
		end
	end
	if #TempTab == 0 then 
		return 1;
	else
		return PatrolModRandTalkID(TempTab)
	end;
	return 1;
end

-- 在适应等级中根据权重值随机选取对话id
function PatrolModRandTalkID(tab)
	local count = 0; 
	for i = 1 ,#tab do 
		count = tab[i].rights + count;
	end
	if count == 0 then 
		return 1;
	else
		local num = custom.rand(1,count);
		local temp = 1;
		for i = 1 ,#tab do 
			local lasttemp = temp;
			temp = tab[i].rights + temp;
			if num >= lasttemp and num < temp then 
				return tab[i].id;
			end
		end
	end;
	return 1;	
end

-- 是否有对话
function PatrolModIsTalkShow()
	return m_IsTalkShow;
end
