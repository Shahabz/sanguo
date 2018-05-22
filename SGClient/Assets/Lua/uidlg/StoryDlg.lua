-- 界面
local m_Dlg = nil;
local m_DialogFrameMod = nil;
local m_uiTitleText = nil; --UnityEngine.GameObject
local m_uiPreBtn = nil; --UnityEngine.GameObject
local m_uiNextBtn = nil; --UnityEngine.GameObject
local m_uiGrid = nil; --UnityEngine.GameObject
local m_uiWarning = nil; --UnityEngine.GameObject
local m_uiResResetLayer = nil; --UnityEngine.GameObject

local m_chapter =0;
local m_storyidAsyn = nil;
local m_recvValue = nil
local m_id = 0;
local m_uiObjCache = {}
local m_resResetLayerStoryid = 0;

STORY_TYPE_NORMAL	=	1	-- 普通，不能反复打
STORY_TYPE_BOSS		=	2	-- 精英，可以反复打，3星后可以扫荡
STORY_TYPE_RES		=	3	-- 资源，点击即可领取资源，有次数和时间限制，之后消失
STORY_TYPE_ITEM		=	4	-- 道具(国器碎片，建筑图纸)，可反复打
STORY_TYPE_HERO		=	5	-- 武将招募，2个武将随机一个，另一个需要元宝购买
STORY_TYPE_DRAWING	=	6	-- 装备图纸，直接元宝购买，购买完消失

-- 打开界面
function StoryDlgOpen()
	m_Dlg = eye.uiManager:Open( "StoryDlg" );
	m_DialogFrameMod = DialogFrameModOpen( m_Dlg, T(553), HELP_StoryDlg, StoryDlgClose );
end

-- 隐藏界面
function StoryDlgClose()
	if m_Dlg == nil then
		return;
	end
	DialogFrameModClose( m_DialogFrameMod );
	m_DialogFrameMod = nil;
	eye.uiManager:Close( "StoryDlg" );
	m_uiObjCache = {}
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
		elseif nControlID == -2 then
			StoryDlgHideResResetLayer()
		elseif nControlID == 1 then
			StoryDlgPre();
		elseif nControlID == 2 then
			StoryDlgNext();
		elseif nControlID == 3 then
			StoryDlgResReset()
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
	m_uiWarning = objs[4];
	m_uiResResetLayer = objs[5];
	SetFalse( m_uiGrid );
	SetFalse( m_uiPreBtn )
	SetFalse( m_uiNextBtn )
	SetFalse( m_uiWarning )
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
function StoryDlgShow( storyid )
	StoryDlgOpen()
	SetFalse( m_uiResResetLayer )
	m_storyidAsyn = storyid;
	if m_recvValue == nil then
		system_askinfo( ASKINFO_STORY, "", 0 );
	else
		StoryDlgRecv( m_recvValue )
	end
	-- 任务检查
	QuestClickCheck( 5 )
	if IsGuiding() and GetCurrentGuideType() == GUIDE_CPOY then
		GuideNext();
	end
end

-- m_story_star={[128]},m_story_hero={[32]},m_story_restime={[32]},m_story_resnum={[32]},m_story_resreset={[32]},m_story_itemnum={[32]},m_story_drawing={[16]},m_storyid=0,m_sweep_herokind[4] = 0
function StoryDlgRecv( recvValue )
	m_recvValue = recvValue;
	--m_recvValue.m_storyid = 646
	if GetPlayer().m_storyid < recvValue.m_storyid then
		GetPlayer().m_storyid = recvValue.m_storyid;
	end
	
	if m_storyidAsyn == nil or m_storyidAsyn == 0 or m_storyidAsyn > recvValue.m_storyid then
		-- 首次打开默认最新章节
		if m_chapter == 0 then
			m_chapter = g_story[m_recvValue.m_storyid].chapter;
			if m_chapter == 0 then
				m_chapter = 1;
			end
		end
		
	else
		-- 有选择的打开
		m_chapter = g_story[m_storyidAsyn].chapter;
		if m_chapter == 0 then
			m_chapter = 1;
		end
	end
	StoryDlgSetInfo()
end

-- 状态接收
-- m_storyid=0,m_state=0,m_savetype=0,m_saveoffset=0,m_actor_storyid=0
function StoryDlgStateRecv( recvValue )
	if recvValue.m_savetype == 1 then
		-- 更新星级
		m_recvValue.m_story_star[recvValue.m_saveoffset+1] = recvValue.m_state
		
	elseif recvValue.m_savetype == 2 then
		-- 更新招募信息
		m_recvValue.m_story_hero[recvValue.m_saveoffset+1] = recvValue.m_state

	elseif recvValue.m_savetype == 3 then
		-- 更新资源副本时间
		m_recvValue.m_story_restime[recvValue.m_saveoffset+1] = recvValue.m_state

	elseif recvValue.m_savetype == 4 then
		-- 更新资源副本可打次数
		m_recvValue.m_story_resnum[recvValue.m_saveoffset+1] = recvValue.m_state

	elseif recvValue.m_savetype == 5 then
		-- 更新资源副本重置次数
		m_recvValue.m_story_resreset[recvValue.m_saveoffset+1] = recvValue.m_state

	elseif recvValue.m_savetype == 6 then
		-- 更新道具副本掉落次数
		m_recvValue.m_story_itemnum[recvValue.m_saveoffset+1] = recvValue.m_state

	elseif recvValue.m_savetype == 7 then
		-- 装备图纸副本是否购买
		m_recvValue.m_story_drawing[recvValue.m_saveoffset+1] = recvValue.m_state
		
	end
	
	-- 更新	
	local storyid = recvValue.m_storyid;
	if m_uiObjCache[storyid] ~= nil then
		StoryDlgSetRank( m_uiObjCache[storyid], g_story[storyid] )
	end
	
	if m_recvValue.m_storyid < recvValue.m_actor_storyid then
		m_recvValue.m_storyid = recvValue.m_actor_storyid;
		StoryDlgRecv( m_recvValue )
		
		-- 检查是否有最新章节，下一章闪光
		local chapter = g_story[m_recvValue.m_storyid].chapter;
		if chapter > m_chapter then
			SetTrue( m_uiNextBtn )
		else
			
		end
	end
		
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
	local maxchapter = g_story[m_recvValue.m_storyid].chapter;
	if m_chapter < maxchapter then
		m_chapter = m_chapter + 1;
		StoryDlgSetInfo()
	end
end

function StoryDlgSetInfo()
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	-- 上一章按钮
	if m_chapter == 1 then
		SetFalse( m_uiPreBtn )
	elseif m_chapter > 1 then
		SetTrue( m_uiPreBtn )
	end
	
	-- 下一章按钮
	local maxchapter = g_story[m_recvValue.m_storyid].chapter;
	if m_chapter == maxchapter then
		SetFalse( m_uiNextBtn )
	elseif m_chapter < maxchapter then
		SetTrue( m_uiNextBtn )
	end
	-- 警告
	SetFalse( m_uiWarning )
	-- 标题
	SetText( m_uiTitleText, StoryChapterName( m_chapter ) )
	
	-- 关卡
	SetTrue( m_uiGrid );
	for i=1, 9, 1 do
		SetFalse( m_uiGrid.transform:GetChild(i-1).gameObject )
	end
	
	-- 找到章节的所有关卡
	local ranklist = {}
	for i=m_chapter*10, m_chapter*10+9, 1 do
		if g_story[i] ~= nil then
			table.insert( ranklist, g_story[i] );
		end
	end
	table.sort( ranklist, 
	function( a, b ) 
		if a.id < b.id then
			return true
		end
	 end )
	
	m_uiObjCache = {}
	for i=1, #ranklist, 1 do
		local uiObj = m_uiGrid.transform:GetChild(i-1).gameObject
		StoryDlgSetRank( uiObj, ranklist[i] )
		-- 缓存起来
		local id = ranklist[i].id;
		m_uiObjCache[id] = uiObj;
	end
	
end

-- 设置一个副本
function StoryDlgSetRank( uiObj, storyConfig )
	if uiObj == nil then
		return
	end
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiShape = objs[0];
	local uiFrame = objs[1];
	local uiName = objs[2];
	local uiFlag = objs[3];
	local uiNew = objs[4];
	local uiStar = objs[5];
	local uiItem = objs[6];
	local uiTimer = objs[7];
	local uiCount = objs[8];
	local uiToken = objs[9];
	local uiShapeBack = objs[10];
	local uiEffect = objs[11];
	local uiBack = objs[12];
	SetTrue( uiObj )
	SetFalse( uiFlag );
	SetFalse( uiNew );
	SetFalse( uiStar );
	SetFalse( uiItem );
	SetFalse( uiTimer );
	SetFalse( uiCount );
	SetFalse( uiToken );
	SetFalse( uiShapeBack )
	SetFalse( uiEffect );
	SetFalse( uiBack );
	
	local color = NameColor( storyConfig.color )
	local type = storyConfig.type;
	if type == STORY_TYPE_NORMAL then -- 普通副本
		-- 已经通关
		if m_recvValue.m_storyid > storyConfig.id then
			SetControlID( uiObj, 0 )
			SetTrue( uiShape )
			SetGray( uiShape, true )
			--SetTrue( uiShapeBack )
			--SetGray( uiShapeBack, true )
			SetTrue( uiFlag );
			color = Hex2Color(0x969696FF)
		
		-- 当前正在打的关卡
		elseif m_recvValue.m_storyid == storyConfig.id then
			SetControlID( uiObj, 10000+storyConfig.id )
			SetTrue( uiShape )
			SetGray( uiShape, false )
			--SetTrue( uiShapeBack )
			--SetGray( uiShapeBack, false )
			SetTrue( uiNew )
			SetTrue( uiEffect );
			StoryDlgShowIcon( uiItem, storyConfig )	
			
		-- 尚未通关
		else
			-- 显示锁定状态的
			if m_recvValue.m_storyid > storyConfig.unlock_preid then
				SetControlID( uiObj, 0 )
				SetFalse( uiShape )
				--SetFalse( uiShapeBack )
				SetTrue( uiBack );
				StoryDlgShowIcon( uiItem, storyConfig )
			else
				SetFalse( uiObj )
				return
			end
		end
		-- 名称
		SetText( uiName, StoryRankName( storyConfig.id ), color )
		-- 形象
		StoryRankShapeBack( uiShapeBack, storyConfig.shapeback )
		StoryRankShape( uiShape, storyConfig.shape )

		
	elseif type == STORY_TYPE_BOSS then-- 精英
		-- 已经通关
		local showWarning = 0;
		if m_recvValue.m_storyid > storyConfig.id then
			SetControlID( uiObj, 10000+storyConfig.id )
			SetTrue( uiShape )
			SetGray( uiShape, false )
			--SetTrue( uiShapeBack )
			--SetGray( uiShapeBack, false )
			SetTrue( uiFlag );

			-- 星级
			local starlv = m_recvValue.m_story_star[storyConfig.star_saveoffset+1]
			StoryDlgStar( uiStar, starlv )
			if starlv ~= nil and starlv < 3 then
				showWarning = 1;
			end
			
		-- 当前正在打的关卡
		elseif m_recvValue.m_storyid == storyConfig.id then
			SetControlID( uiObj, 10000+storyConfig.id )
			SetTrue( uiShape )
			SetGray( uiShape, false )
			--SetTrue( uiShapeBack )
			--SetGray( uiShapeBack, false )
			SetTrue( uiNew )
			SetTrue( uiEffect );
			StoryDlgShowIcon( uiItem, storyConfig )	
			showWarning = 1;
				
		-- 尚未通关
		else
			-- 显示锁定状态的
			if m_recvValue.m_storyid > storyConfig.unlock_preid then
				SetControlID( uiObj, 0 )
				SetFalse( uiShape )
				--SetFalse( uiShapeBack )
				SetTrue( uiBack );
				StoryDlgShowIcon( uiItem, storyConfig )	
				showWarning = 1;
			else
				SetFalse( uiObj )
				return
			end
		end
		-- 名称
		SetText( uiName, StoryRankName( storyConfig.id ), color )
		-- 形象
		--StoryRankShapeBack( uiShapeBack, storyConfig.shapeback )
		StoryRankShape( uiShape, storyConfig.shape )
		
		-- 未3星通关，显示警告
		if showWarning == 1 then
			SetTrue( m_uiWarning )
			if GetPlayer().m_viplevel < global.story_sweep_vip then
				SetText( m_uiWarning, F( 2013,  "<color=#"..NameColorStr(storyConfig.color)..">"..StoryRankName( storyConfig.id ).."</color>" ) )
			else
				SetText( m_uiWarning, F( 2014, "<color=#"..NameColorStr(storyConfig.color)..">"..StoryRankName( storyConfig.id ).."</color>" ) )
			end
		end
		
	elseif type == STORY_TYPE_RES then-- 资源
		-- 攻打次数
		local resnum_saveoffset = storyConfig.resnum_saveoffset;
		local resnum = m_recvValue.m_story_resnum[resnum_saveoffset+1];
		-- 结束时间
		local ressec_saveoffset = storyConfig.ressec_saveoffset;
		local restime = m_recvValue.m_story_restime[ressec_saveoffset+1];
		-- 重置次数
		local res_reset_saveoffset = storyConfig.res_reset_saveoffset;
		local res_reset = m_recvValue.m_story_resreset[res_reset_saveoffset+1];
		-- 没可攻打次数了，并且时间到了，消失
		if resnum >= storyConfig.resnum and GetServerTime() >= restime then
			SetFalse( uiObj )
			return
		end
		
		-- 如果到了重置次数，并且没有次数了消失
		if resnum >= storyConfig.resnum and res_reset >= storyConfig.res_reset_num then
			SetFalse( uiObj )
			return
		end
		SetTrue( uiEffect );
		-- 已经通关
		if m_recvValue.m_storyid > storyConfig.id or m_recvValue.m_storyid > storyConfig.preid then
			SetControlID( uiObj, 10000+storyConfig.id )
			SetTrue( uiShape )
			SetGray( uiShape, false )
			--SetTrue( uiShapeBack )
			--SetGray( uiShapeBack, false )
			StoryDlgShowIcon( uiItem, storyConfig )
			
			if resnum < storyConfig.resnum then
				SetTrue( uiCount )
				SetText( uiCount, F(2015, storyConfig.resnum-resnum) )
				SetFalse( uiToken )
			else
				local token = StoryDlgResResetGet( storyConfig, res_reset )
				SetTrue( uiToken )
				SetText( uiToken.transform:Find("Text"), token )
			end
			if restime > GetServerTime() then
				SetTrue( uiTimer )
				SetTimer( uiTimer, restime-GetServerTime(), storyConfig.ressec )
			else
				SetFalse( uiTimer )
			end
			
		-- 当前正在打的关卡
		elseif m_recvValue.m_storyid == storyConfig.id then
			SetControlID( uiObj, 10000+storyConfig.id )
			SetTrue( uiShape )
			SetGray( uiShape, false )
			--SetTrue( uiShapeBack )
			--SetGray( uiShapeBack, false )
			SetTrue( uiNew )
			StoryDlgShowIcon( uiItem, storyConfig )	
			
			if resnum < storyConfig.resnum then
				SetTrue( uiCount )
				SetText( uiCount, F(2015, storyConfig.resnum-resnum) )
			else
				local token = StoryDlgResResetGet( storyConfig, res_reset )
				SetTrue( uiToken )
				SetText( uiToken.transform:Find("Text"), token )
			end
			if restime > GetServerTime() then
				SetTrue( uiTimer )
				SetTimer( uiTimer, restime-GetServerTime(), storyConfig.ressec )
			else
				SetFalse( uiTimer )
			end
				
		-- 尚未通关
		else
			-- 显示锁定状态的
			if m_recvValue.m_storyid > storyConfig.unlock_preid then
				SetControlID( uiObj, 0 )
				SetFalse( uiShape )
				--SetFalse( uiShapeBack )
				SetTrue( uiBack );
				StoryDlgShowIcon( uiItem, storyConfig )	
			else
				SetFalse( uiObj )
				return
			end
		end
		-- 名称
		SetText( uiName, StoryRankName( storyConfig.id ), color )
		-- 形象
		--StoryRankShapeBack( uiShapeBack, storyConfig.shapeback )
		StoryRankShape( uiShape, storyConfig.shape )
		
	elseif type == STORY_TYPE_ITEM then-- 道具
		local save_offset = storyConfig.itemnum_saveoffset;
		local num = m_recvValue.m_story_itemnum[save_offset+1];
		if num >= storyConfig.itemnum then
			-- 已经掉落完毕
			SetFalse( uiObj )
			return
		end
		SetTrue( uiEffect );
		-- 已经通关
		if m_recvValue.m_storyid > storyConfig.id or m_recvValue.m_storyid > storyConfig.preid then
			SetControlID( uiObj, 10000+storyConfig.id )
			SetTrue( uiShape )
			SetGray( uiShape, false )
			--SetTrue( uiShapeBack )
			--SetGray( uiShapeBack, false )
			StoryDlgShowIcon( uiItem, storyConfig )	
			SetTrue( uiCount )
			SetText( uiCount, F( 2029, num, storyConfig.itemnum )  )
			
		-- 当前正在打的关卡
		elseif m_recvValue.m_storyid == storyConfig.id then
			SetControlID( uiObj, 10000+storyConfig.id )
			SetTrue( uiShape )
			SetGray( uiShape, false )
			--SetTrue( uiShapeBack )
			--SetGray( uiShapeBack, false )
			SetTrue( uiNew )
			StoryDlgShowIcon( uiItem, storyConfig )	
			SetTrue( uiCount )
			SetText( uiCount, F( 2029, num, storyConfig.itemnum ) )
				
		-- 尚未通关
		else
			-- 显示锁定状态的
			if m_recvValue.m_storyid > storyConfig.unlock_preid then
				SetControlID( uiObj, 0 )
				SetFalse( uiShape )
				--SetFalse( uiShapeBack )
				SetTrue( uiBack );
				StoryDlgShowIcon( uiItem, storyConfig )	
			else
				SetFalse( uiObj )
				return
			end
		end
		-- 名称
		SetText( uiName, StoryRankName( storyConfig.id ), color )
		-- 形象
		--StoryRankShapeBack( uiShapeBack, storyConfig.shapeback )
		StoryRankShape( uiShape, storyConfig.shape )
		-- 显示道具形象
		if storyConfig.item_awardkind > 0 then
			StoryDlgItemShape( uiItem, storyConfig.item_awardkind )
		end
		
	elseif type == STORY_TYPE_HERO then-- 武将招募
		local save_offset = storyConfig.hero_saveoffset;
		local iscallover = m_recvValue.m_story_hero[save_offset+1];
		if storyConfig.hero_kind0 > 0 and storyConfig.hero_kind1 > 0 then
			if iscallover == 11 then
				-- 已经招募完毕
				SetFalse( uiObj )
				return
			end
		elseif storyConfig.hero_kind0 > 0 then
			if iscallover == 1 then
				-- 已经招募完毕
				SetFalse( uiObj )
				return
			end
		elseif storyConfig.hero_kind1 > 0 then
			if iscallover == 1 or iscallover == 10 then
				-- 已经招募完毕
				SetFalse( uiObj )
				return
			end
		end
		SetTrue( uiEffect );
		-- 已经通关
		if m_recvValue.m_storyid > storyConfig.id or m_recvValue.m_storyid > storyConfig.preid then
			SetControlID( uiObj, 10000+storyConfig.id )
			SetTrue( uiShape )
			SetGray( uiShape, false )
			--SetTrue( uiShapeBack )
			--SetGray( uiShapeBack, false )
			StoryDlgShowIcon( uiItem, storyConfig )	
			if IsGuiding() then
				if GetCurrentGuideType() == GUIDE_COPY_HERO then
					FindCmdTpye(uiItem.transform);
				end
			end
			
		-- 当前正在打的关卡
		elseif m_recvValue.m_storyid == storyConfig.id then
			SetControlID( uiObj, 10000+storyConfig.id )
			SetTrue( uiShape )
			SetGray( uiShape, false )
			--SetTrue( uiShapeBack )
			--SetGray( uiShapeBack, false )
			SetTrue( uiNew )
			StoryDlgShowIcon( uiItem, storyConfig )	
				
		-- 尚未通关
		else
			-- 显示锁定状态的
			if m_recvValue.m_storyid > storyConfig.unlock_preid then
				SetControlID( uiObj, 0 )
				--SetFalse( uiShape )
				--SetFalse( uiShapeBack )
				SetTrue( uiBack );
				StoryDlgShowIcon( uiItem, storyConfig )	
			else
				SetFalse( uiObj )
				return
			end
		end
		-- 名称
		SetText( uiName, StoryRankName( storyConfig.id ), color )
		-- 形象
		--StoryRankShapeBack( uiShapeBack, storyConfig.shapeback )
		StoryRankShape( uiShape, storyConfig.shape )
	
	elseif type == STORY_TYPE_DRAWING then-- 装备图纸
		local save_offset = storyConfig.drawing_saveoffset;
		local isbuy = m_recvValue. m_story_drawing[save_offset+1];
		if isbuy == 2 then
			-- 已经购买完毕
			SetFalse( uiObj )
			return		
		end
		SetTrue( uiEffect );
		-- 已经通关
		if m_recvValue.m_storyid > storyConfig.id or m_recvValue.m_storyid > storyConfig.preid then
			SetControlID( uiObj, 10000+storyConfig.id )
			SetTrue( uiShape )
			SetGray( uiShape, false )
			--SetTrue( uiShapeBack )
			--SetGray( uiShapeBack, false )
			StoryDlgShowIcon( uiItem, storyConfig )	
			if isbuy == 1 then
				SetTrue( uiCount )
				SetText( uiCount, F( 2015, 1 )  )
			else
				SetTrue( uiToken )
				SetText( uiToken.transform:Find("Text"), storyConfig.drawing_token )
			end
		
		-- 当前正在打的关卡
		elseif m_recvValue.m_storyid == storyConfig.id then
			SetControlID( uiObj, 10000+storyConfig.id )
			SetTrue( uiShape )
			SetGray( uiShape, false )
			--SetTrue( uiShapeBack )
			--SetGray( uiShapeBack, false )
			SetTrue( uiNew )
			StoryDlgShowIcon( uiItem, storyConfig )	
			if isbuy == 1 then
				SetTrue( uiCount )
				SetText( uiCount, F( 2015, 1 )  )
			else
				SetTrue( uiToken )
				SetText( uiToken.transform:Find("Text"), storyConfig.drawing_token )
			end
				
		-- 尚未通关
		else
			-- 显示锁定状态的
			if m_recvValue.m_storyid > storyConfig.unlock_preid then
				SetControlID( uiObj, 0 )
				SetFalse( uiShape )
				--SetFalse( uiShapeBack )
				SetTrue( uiBack );
				StoryDlgShowIcon( uiItem, storyConfig )	
			else
				SetFalse( uiObj )
				return
			end
		end
		-- 名称
		SetText( uiName, StoryRankName( storyConfig.id ), color )
		-- 形象
		--StoryRankShapeBack( uiShapeBack, storyConfig.shapeback )
		StoryRankShape( uiShape, storyConfig.shape )
		-- 显示装备图纸
		if storyConfig.drawing_kind > 0 then
			StoryDlgItemShape( uiItem, storyConfig.drawing_kind )
		end
	end
end

-- 选择副本
function StoryDlgSelect( id )
	m_id = id;
	local storyConfig = g_story[id]
	local type = g_story[id].type;
	if type == STORY_TYPE_NORMAL then -- 普通副本
		BattleDlgShowByStory( id )
		
	elseif type == STORY_TYPE_BOSS then-- 精英
		-- 星级
		local starlv = m_recvValue.m_story_star[storyConfig.star_saveoffset+1]
		if starlv == 3 or (starlv > 0 and Utils.get_int_sflag( GetPlayer().m_actor_sflag, ACTOR_SFLAG_STORYSWEEP ) == 1) then
			-- 扫荡
			StorySweepDlgShow( id, m_recvValue.m_sweep_herokind )
		else
			BattleDlgShowByStory( id )
		end
		
	elseif type == STORY_TYPE_RES then-- 资源
		-- 攻打次数
		local resnum_saveoffset = storyConfig.resnum_saveoffset;
		local resnum = m_recvValue.m_story_resnum[resnum_saveoffset+1];
		-- 结束时间
		local ressec_saveoffset = storyConfig.ressec_saveoffset;
		local restime = m_recvValue.m_story_restime[ressec_saveoffset+1];
		-- 重置次数
		local res_reset_saveoffset = storyConfig.res_reset_saveoffset;
		local res_reset = m_recvValue.m_story_resreset[res_reset_saveoffset+1];
		-- 没有可攻打次数，重置
		if resnum == storyConfig.resnum then
			if res_reset < storyConfig.res_reset_num then
				StoryDlgShowResResetLayer( storyConfig, res_reset )
			end
		else
			system_askinfo( ASKINFO_STORY, "", 2, id );
		end
		
	elseif type == STORY_TYPE_ITEM then-- 道具
		BattleDlgShowByStory( id )
		
	elseif type == STORY_TYPE_HERO then-- 武将招募
		StoryHeroDlgShow( id, m_recvValue.m_story_hero )
		
	elseif type == STORY_TYPE_DRAWING then-- 装备图纸
		local save_offset = storyConfig.drawing_saveoffset;
		local isbuy = m_recvValue. m_story_drawing[save_offset+1];
		if isbuy == 0 then
			MsgBox( F(2030, storyConfig.drawing_token), function() 
				system_askinfo( ASKINFO_STORY, "", 6, id );
			end )
		elseif isbuy == 1 then
			system_askinfo( ASKINFO_STORY, "", 7, id );
		end
	end
end 

-- 设置精英副本星级
function StoryDlgStar( uiRoot, level )
	if level == nil then
		return
	end
	for i=1, 3, 1 do
		local uiStar = uiRoot.transform:GetChild(i-1).gameObject;
		if i <= level then
			SetImage( uiStar, LoadSprite("ui_icon_star_1") )
		else	
			SetImage( uiStar, LoadSprite("ui_icon_star_2") )
		end
	end
	if level > 0 then
		SetTrue( uiRoot )
	end
end

-- 显示图标
function StoryDlgShowIcon( uiItem, storyConfig )
	-- 显示武将
	if storyConfig.hero_kind0 > 0 then
		StoryDlgHeroShape( uiItem, storyConfig.hero_kind0 )
	end
	-- 显示资源
	if storyConfig.restype > 0 then
		StoryDlgResShape( uiItem, storyConfig.restype )
	end
	-- 显示道具
	if storyConfig.item_awardkind > 0 then
		StoryDlgItemShape( uiItem, storyConfig.item_awardkind )
	end
	-- 显示图纸
	if storyConfig.drawing_kind > 0 then
		StoryDlgItemShape( uiItem, storyConfig.drawing_kind )
	end
end

-- 设置副本道具图标
function StoryDlgItemShape( uiItem, itemkind )
	if itemkind > AWARDKIND_EQUIPBASE then
		local sprite = AwardInfo( itemkind )
		SetImage( uiItem.transform:Find("Shape"), sprite )
	else
		SetImage( uiItem.transform:Find("Shape"), ItemSprite(itemkind) )	
	end
	SetTrue( uiItem )
end

-- 设置副本武将图标
function StoryDlgHeroShape( uiItem, herokind )
	SetImage( uiItem.transform:Find("Shape"), HeroHeadSprite(herokind) )
	SetTrue( uiItem )
end

-- 设置副本资源图标
function StoryDlgResShape( uiItem, restype )
	SetImage( uiItem.transform:Find("Shape"), ResItemIcon( restype ) )
	SetTrue( uiItem )
end

-- 资源副本重置次数
function StoryDlgResResetGet( storyConfig, res_reset )
	local token = 0
	if res_reset == 0 then
		token = storyConfig.res_reset_token0
	elseif res_reset == 1 then
		token = storyConfig.res_reset_token1
	elseif res_reset == 2 then
		token = storyConfig.res_reset_token2
	elseif res_reset == 3 then
		token = storyConfig.res_reset_token3
	elseif res_reset == 4 then
		token = storyConfig.res_reset_token4
	elseif res_reset == 5 then
		token = storyConfig.res_reset_token5
	elseif res_reset > 5 then
		token = storyConfig.res_reset_token5
	end
	return token
end

-- 资源副本重置页
function StoryDlgShowResResetLayer( storyConfig, res_reset )
	m_resResetLayerStoryid = storyConfig.id
	SetTrue( m_uiResResetLayer )
	SetImage( m_uiResResetLayer.transform:Find("Windows/Shape"), ResItemIcon( storyConfig.restype ) )
	SetText( m_uiResResetLayer.transform:Find("Windows/Name"), StoryRankName( storyConfig.id ) )
	SetText( m_uiResResetLayer.transform:Find("Windows/Desc"), F( 2032, ResName( storyConfig.restype ).."x"..(storyConfig.rescount*storyConfig.resnum) ) )
	local token = StoryDlgResResetGet( storyConfig, res_reset )
	SetRichText( m_uiResResetLayer.transform:Find("Windows/Cost"), F(2033,token) )
end

-- 资源副本重置
function StoryDlgResReset()
	system_askinfo( ASKINFO_STORY, "", 3, m_resResetLayerStoryid );
	StoryDlgHideResResetLayer()
end

-- 隐藏资源副本页
function StoryDlgHideResResetLayer()
	SetFalse( m_uiResResetLayer )
end

-- 获取章节名称
function StoryChapterName( chapter )
	local id = 7000 + chapter*10;
	return Localization.text_item(id)
end

-- 获取关卡名称
function StoryRankName( id )
	local id = 7000 + id;
	return Localization.text_item(id)
end

-- 获取关卡形象
function StoryRankShapeBack( uiShapeBack, shapeback )
	if shapeback == 0 then
		shapeback = 1
	end
	SetImage( uiShapeBack, LoadSprite("ui_story_shapeback_"..shapeback) )
end

-- 获取关卡形象
function StoryRankShape( uiShape, shape )
	if shape == 0 then
		shape = 1
	end
	SetImage( uiShape, LoadSprite("ui_story_shape_"..shape) )
end
