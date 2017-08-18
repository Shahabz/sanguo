-- 界面
local m_Dlg = nil;
local m_DialogFrameMod = nil;
local m_uiTitleText = nil; --UnityEngine.GameObject
local m_uiPreBtn = nil; --UnityEngine.GameObject
local m_uiNextBtn = nil; --UnityEngine.GameObject
local m_uiGrid = nil; --UnityEngine.GameObject
local m_chapter =0;

local m_recvValue = nil
local m_id = 0;

STORY_TYPE_NORMAL	=	1	-- 普通
STORY_TYPE_BOSS		=	2	-- 精英
STORY_TYPE_RES		=	3	-- 资源
STORY_TYPE_ITEM		=	4	-- 道具
STORY_TYPE_HERO		=	5	-- 武将招募
STORY_TYPE_DRAWING	=	6	-- 装备图纸

-- 打开界面
function StoryDlgOpen()
	m_Dlg = eye.uiManager:Open( "StoryDlg" );
	m_DialogFrameMod = DialogFrameModOpen( m_Dlg, T(553), 8, StoryDlgClose );
end

-- 隐藏界面
function StoryDlgClose()
	if m_Dlg == nil then
		return;
	end
	DialogFrameModClose( m_DialogFrameMod );
	m_DialogFrameMod = nil;
	eye.uiManager:Close( "StoryDlg" );
end

-- 删除界面
function StoryDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function StoryDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            StoryDlgClose();
		elseif nControlID == 1 then
			StoryDlgPre();
		elseif nControlID == 2 then
			StoryDlgNext();
		elseif nControlID > 10000 and nControlID < 20000 then
			StoryDlgSelect( nControlID - 10000 )
        end
	end
end

-- 载入时调用
function StoryDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiTitleText = objs[0];
	m_uiPreBtn = objs[1];
	m_uiNextBtn = objs[2];
	m_uiGrid = objs[3];
	SetFalse( m_uiGrid );
	SetFalse( m_uiPreBtn )
	SetFalse( m_uiNextBtn )
end

-- 界面初始化时调用
function StoryDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function StoryDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function StoryDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function StoryDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function StoryDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function StoryDlgShow()
	StoryDlgOpen()
	if m_recvValue == nil then
		system_askinfo( ASKINFO_STORY, "", 0 );
	else
		StoryDlgRecv( m_recvValue )
	end
end

function StoryDlgRecv( recvValue )
	m_recvValue = recvValue;
	m_chapter = m_recvValue.m_story_chapter;
	if m_chapter == 0 then
		m_chapter = 1;
	end
	StoryDlgSetInfo()
end

-- 上一章
function StoryDlgPre()
	if m_chapter > 1 then
		m_chapter = m_chapter - 1;
		StoryDlgSetInfo()
	end
end

-- 下一章
function StoryDlgNext()
	if m_chapter < m_recvValue.m_story_chapter then
		m_chapter = m_chapter + 1;
		StoryDlgSetInfo()
	end
end

function StoryDlgSetInfo()
	-- 上一页
	if m_chapter == 1 then
		SetFalse( m_uiPreBtn )
	elseif m_chapter > 1 then
		SetTrue( m_uiPreBtn )
	end
	
	-- 下一页
	if m_chapter == m_recvValue.m_story_chapter then
		SetFalse( m_uiNextBtn )
	elseif m_chapter < m_recvValue.m_story_chapter then
		SetTrue( m_uiNextBtn )
	end
	
	-- 标题
	SetText( m_uiTitleText, StoryChapterName( m_chapter ) )
	
	-- 关卡
	SetTrue( m_uiGrid );
	for i=1, 9, 1 do
		local uiStory = m_uiGrid.transform:GetChild(i-1).gameObject
		local objs = uiStory.transform:GetComponent( typeof(Reference) ).relatedGameObject;
		local uiShape = objs[0];
		local uiFrame = objs[1];
		local uiName = objs[2];
		local uiFlag = objs[3];
		local uiNew = objs[4];
		local uiStar = objs[5];
		local uiItem = objs[6];
		local uiTimer = objs[7];
		SetFalse( uiStory )
		if i <= #g_story[m_chapter] then
			SetGray( uiShape, true );
			SetFalse( uiFlag );
			SetFalse( uiNew );
			SetFalse( uiStar );
			SetFalse( uiItem );
			SetFalse( uiTimer );
			
			local id = g_story[m_chapter][i].id;
			local type = g_story[m_chapter][i].type;		 		-- 副本类型
			local color = g_story[m_chapter][i].color;		 		-- 副本颜色
			local saveoffset = g_story[m_chapter][i].saveoffset+1;	-- 副本状态存档索引
			local state = m_recvValue.m_rankstate[saveoffset];		-- 副本状态
			local pre_id = g_story[m_chapter][i].preid;		 		-- 前置副本id
			local pre_state = 0; 									-- 前置副本状态
			if pre_id > 0 then
				local pre_chapter = math.floor(pre_id/10)
				local pre_rank = pre_id%10
				local pre_saveoffset = g_story[pre_chapter][pre_rank].saveoffset+1
				pre_state = m_recvValue.m_rankstate[pre_saveoffset]; 
			end
			
			-- 名称
			SetText( uiName, StoryRankName( m_chapter, i ), NameColor(color) ) 
			
			-- 前置关卡已经打通
			if pre_id == 0 or pre_state > 0 then
				SetTrue( uiStory )
				SetTrue( uiShape )
				SetGray( uiShape, false )
			else
				
				-- 如果是第3个关卡,只要第1个关卡通关，就显示锁定
				if i == 3 then
					if m_recvValue.m_rankstate[m_chapter*10+1] > 0 then
						SetTrue( uiStory )
						SetFalse( uiShape )
					end
				
				-- 如果是第5和6个关卡,只要第3个关卡通关，就显示锁定
				elseif i == 5 or i == 6 then
					if m_recvValue.m_rankstate[m_chapter*10+3] > 0 then
						SetTrue( uiStory )
						SetFalse( uiShape )
					end
				end	
			end
			
			-- 已经通关
			if state > 0 then
				SetTrue( uiShape )
				SetGray( uiShape, true )
				SetTrue( uiFlag );
				SetTextColor( uiName, Hex2Color(0x9b8869ff) )
			else
				SetControlID( uiStory, 10000+id )
				SetTrue( uiNew )
				SetGray( uiShape, false )
				SetTextColor( uiName, NameColor(color) )
			end
			
			if type == STORY_TYPE_NORMAL then-- 普通
				if state == 0 then
					-- 未通关，检查是否解锁
					if pre_state > 0 then
						-- 已经解锁
						SetGray( uiShape, false )
					else
					
					end
				else
					SetTrue( uiFlag );
				end
				
			elseif type == STORY_TYPE_BOSS then-- 精英
			elseif type == STORY_TYPE_RES then-- 资源
			elseif type == STORY_TYPE_ITEM then-- 道具
			elseif type == STORY_TYPE_HERO then-- 武将招募
			elseif type == STORY_TYPE_DRAWING then-- 装备图纸
			end
		end
	end
end

-- 获取章节名称
function StoryChapterName( chapter )
	local id = 7000 + chapter*10;
	return Localization.text_item(id)
end

-- 获取关卡名称
function StoryRankName( chapter, rank )
	local id = 7000 + chapter*10 + rank;
	return Localization.text_item(id)
end

-- 选择副本
function StoryDlgSelect( id )
	m_id = id;
	BattleDlgShowByStory( id )
end 
