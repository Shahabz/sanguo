-- 界面
local m_Dlg = nil;
local m_uiTitle = nil; --UnityEngine.GameObject
local m_uiShape = nil; --UnityEngine.GameObject
local m_uiName = nil; --UnityEngine.GameObject
local m_uiAwardGrid = nil; --UnityEngine.GameObject
local m_uiAwardGrid = nil; --UnityEngine.GameObject
local m_uiWarn = nil; --UnityEngine.GameObject
local m_uiAwardButton = nil; --UnityEngine.GameObject
local m_uiGotoButton = nil; --UnityEngine.GameObject

local m_awardCache = {}
local m_questid = 0;

-- 打开界面
function WorldQuestInfoDlgOpen()
	m_Dlg = eye.uiManager:Open( "WorldQuestInfoDlg" );
end

-- 隐藏界面
function WorldQuestInfoDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "WorldQuestInfoDlg" );
end

-- 删除界面
function WorldQuestInfoDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function WorldQuestInfoDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            WorldQuestInfoDlgClose();
			
		-- 领取奖励
		elseif nControlID == 1 then
			WorldQuestInfoDlgGetAward()
			
		-- 前往
		elseif nControlID == 2 then
			WorldQuestInfoDlgClose();
        end
	end
end

-- 载入时调用
function WorldQuestInfoDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiTitle = objs[0];
	m_uiShape = objs[1];
	m_uiName = objs[2];
	m_uiAwardGrid = objs[3];
	m_uiAwardGrid = objs[4];
	m_uiWarn = objs[5];
	m_uiAwardButton = objs[6];
	m_uiGotoButton = objs[7];
end

-- 界面初始化时调用
function WorldQuestInfoDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function WorldQuestInfoDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function WorldQuestInfoDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function WorldQuestInfoDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function WorldQuestInfoDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function WorldQuestInfoDlgShow( recvValue )
	WorldQuestInfoDlgOpen()
	for i = 0 ,m_uiAwardGrid.transform.childCount - 1 do
		local objs = m_uiAwardGrid.transform:GetChild(i).gameObject
		SetFalse( objs );
    end
	local questid = recvValue.m_questid;
	m_questid = questid;
	
	-- 任务标题
	SetText( m_uiTitle, T(1352).." "..WorldQuestName(questid) ); 
	
	-- 显示流寇
	if questid == 1 or questid == 2 or questid == 3 then
		SetImage( m_uiShape, EnemySprite( 1, 1 ), true )
		SetText( m_uiName, T(938) )
		
	-- 显示县城
	elseif questid == 4 then
		SetImage( m_uiShape, LoadSprite(MapUnitTownShapeList[1].."_0"), true )
		SetText( m_uiName, T(181) )
		
	-- 显示郡城
	elseif questid == 5 then
		SetImage( m_uiShape, LoadSprite(MapUnitTownShapeList[2].."_0"), true )
		SetText( m_uiName, T(182) )
		
	-- 显示治所
	elseif questid == 6 then
		SetImage( m_uiShape, LoadSprite(MapUnitTownShapeList[3].."_0"), true )
		SetText( m_uiName, T(183) )
	end
	
	if recvValue.m_complete == 1 then
		SetTrue( m_uiAwardButton )
		SetFalse( m_uiGotoButton )
		str = "("..T(1349)..")";
		
	else
		SetFalse( m_uiAwardButton )
		SetTrue( m_uiGotoButton )
	end 
	
		
	-- 描述
	if questid == 1 or questid == 2 or questid == 3 then
		if recvValue.m_complete == 1 then
			SetText( m_uiWarn, FQUEST( 2050+questid, recvValue.m_maxvalue, "("..T(1349)..")" ) );
		else
			SetText( m_uiWarn, FQUEST( 2050+questid, recvValue.m_maxvalue, "("..recvValue.m_value.."/"..recvValue.m_maxvalue..")" ) );
		end
	else
		if recvValue.m_complete == 1 then
			if recvValue.m_dd == 1 then
				SetText( m_uiWarn, FQUEST( 2050+questid, str ).."\n"..Localization.text_quest( 2050 ) );
			else
				SetText( m_uiWarn, FQUEST( 2050+questid, str ) );
			end
		else
			SetText( m_uiWarn, FQUEST( 2050+questid, "" ) );
		end
	end
	
	-- 获取奖励
	if m_awardCache[questid] == nil then
		system_askinfo( ASKINFO_QUEST, "", 11, questid );
	else
		WorldQuestInfoDlgRecvAward( m_awardCache[questid] )
	end
end

-- 获取奖励
-- m_count=0,m_list={m_kind=0,m_num=0,[m_count]},m_callback_code=0,m_value
function WorldQuestInfoDlgRecvAward( recvValue )
	m_awardCache[recvValue.m_value] = recvValue;
	local count = math.min( recvValue.m_count, m_uiAwardGrid.transform.childCount );
	for i = 0 ,count - 1 do
		local uiObj = m_uiAwardGrid.transform:GetChild(i).gameObject
		SetTrue( uiObj );
		local sprite, color, name = AwardInfo( recvValue.m_list[i+1].m_kind )
		SetImage( uiObj.transform:Find("Shape"), sprite );
		SetImage( uiObj.transform:Find("Color"), color );
		SetText( uiObj.transform:Find("Name"), name );
    end
end

-- 领取奖励
function WorldQuestInfoDlgGetAward()
	system_askinfo( ASKINFO_QUEST, "", 12, m_questid );
	WorldQuestInfoDlgClose()
end
