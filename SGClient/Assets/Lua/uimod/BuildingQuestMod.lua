local m_Mod;
----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function BuildingQuestModOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
		if nControlID == 1 then
			BuildingQuestClick( gameObject )
		end
	elseif nType == UI_EVENT_PRESS then
		if value == 0 then
			print( "Button Pressed Down, nControlID:" .. nControlID );
		elseif value == 1 then
			print( "Button Pressed UP, nControlID:" .. nControlID);
		end
	end
end

-- 载入时调用
function BuildingQuestModOnAwake( gameObject )
	m_Mod = gameObject;
	local objs = gameObject:GetComponent( "UIMod" ).relatedGameObject;
end

-- 界面初始化时调用
function BuildingQuestModOnStart( gameObject )
	
end

-- 界面显示时调用
function BuildingQuestModOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function BuildingQuestModOnDisable( gameObject )
	
end

-- 界面删除时调用
function BuildingQuestModOnDestroy( gameObject )
	
end

-- 每帧调用
function BuildingQuestModOnLogic( gameObject )
	
end

function BuildingQuestModGet()
	return m_Mod;
end
----------------------------------------
-- 自定
----------------------------------------
function BuildingQuestClick(gameObject)
	local ShareData = gameObject.transform:GetComponent("ShareData");
	local questid = ShareData.intValue[0];
	
	if questid == 5 then
		NpcTalkOne( T(10001), T(10002), function() 
			system_askinfo( ASKINFO_QUEST, "", 2, questid );
		end );
	end
	gameObject:SetActive( false );
end
