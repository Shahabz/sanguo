
local m_Mod = nil;
local PrefabTab = {};

-- 所属按钮点击时调用
function PatrolModOnEvent( nType, nControlID, value )
    if nType == UI_EVENT_CLICK then
		
    elseif nType == UI_EVENT_TWEENFINISH then
		PatrolModWaitPlay(nControlID);		
    elseif nType == UI_EVENT_TIMECOUNTEND then
		PatrolModRePlay(nControlID-10)
    end
end

-- 载入时调用
function PatrolModOnAwake( gameObject )
	m_Mod = gameObject;
	local objs = gameObject:GetComponent( "UIMod" ).relatedGameObject;
	for i = 0,7 do 
		local prefab = objs[i];
		table.insert(PrefabTab,prefab);
		SetFalse(PrefabTab[i+1]);
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
		SetFalse(PrefabTab[i]);
		local unitTween = PrefabTab[i].transform:GetComponent( "UITweenMove" );
		unitTween:Kill(false);
	end
end

function PatrolModPlay()
	for i = 1,5 do 
		if g_patrol_path[i].level <= GetPlayer().m_level then 
			PatrolModTween(g_patrol_path[i].path);
		end
	end	
end
function PatrolModTween(PathData)
	SetTrue(PrefabTab[PathData.id])
	local unitTween = PrefabTab[PathData.id].transform:GetComponent( "UITweenMove" );
	local unitRect = PrefabTab[PathData.id].transform:GetComponent( "RectTransform" );
	unitRect.localScale = Vector3.New( PathData.scale, PathData.scale, PathData.scale );
	startPos = Vector2.New( PathData.bx, PathData.by );
	targetPos = Vector2.New( PathData.ex, PathData.ey );
	unitTween.from = startPos;
	unitTween.to = targetPos;
	unitTween.duration = PathData.sec;
	unitTween:Play(true);
end
function PatrolModRePlay(id)		
	PatrolModTween(g_patrol_path[id].path);
end
function PatrolModWaitPlay(id)
	SetFalse(PrefabTab[id]);
	local NextId = PatrolModGetNextID(id);	
	local Delay = g_patrol_path[NextId].Delay;
	SetTimer(PrefabTab[id],Delay,GetServerTime()+Delay,NextId+10);
end

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