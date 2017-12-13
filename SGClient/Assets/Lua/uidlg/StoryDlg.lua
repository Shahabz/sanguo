-- 界面
local m_Dlg = nil;
local m_DialogFrameMod = nil;
local m_uiTitleText = nil; --UnityEngine.GameObject
local m_uiPreBtn = nil; --UnityEngine.GameObject
local m_uiNextBtn = nil; --UnityEngine.GameObject
local m_uiGrid = nil; --UnityEngine.GameObject
local m_uiWarning = nil; --UnityEngine.GameObject
local m_chapter =0;

local m_recvValue = nil
local m_id = 0;

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
	m_uiWarning = objs[4];
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
function StoryDlgShow()
	StoryDlgOpen()
	if m_recvValue == nil then
		system_askinfo( ASKINFO_STORY, "", 0 );
	else
		StoryDlgRecv( m_recvValue )
	end
end

-- m_story_star={[128]},m_story_hero={[32]},m_story_restime={[32]},m_story_resnum={[32]},m_story_resreset={[32]},m_story_itemnum={[32]},m_story_drawing={[16]},m_storyid=0,
function StoryDlgRecv( recvValue )
	m_recvValue = recvValue;
	m_recvValue.m_storyid = 646
	m_chapter = g_story[m_recvValue.m_storyid].chapter;
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
	
	for i=1, #ranklist, 1 do
		local uiObj = m_uiGrid.transform:GetChild(i-1).gameObject
		StoryDlgSetRank( uiObj, ranklist[i] )
	end
	
end

-- 设置一个副本
function StoryDlgSetRank( uiObj, storyConfig )
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
	SetTrue( uiObj )
	SetFalse( uiFlag );
	SetFalse( uiNew );
	SetFalse( uiStar );
	SetFalse( uiItem );
	SetFalse( uiTimer );
	SetFalse( uiCount );
	SetFalse( uiToken );
	
	local color = NameColor( storyConfig.color )
	local type = storyConfig.type;
	if type == STORY_TYPE_NORMAL then -- 普通副本
		-- 已经通关
		if m_recvValue.m_storyid > storyConfig.id then
			SetControlID( uiObj, 0 )
			SetTrue( uiShape )
			SetGray( uiShape, true )
			SetTrue( uiShapeBack )
			SetGray( uiShapeBack, true )
			SetTrue( uiFlag );
			color = Hex2Color(0x9b8869ff)
		
		-- 当前正在打的关卡
		elseif m_recvValue.m_storyid == storyConfig.id then
			SetControlID( uiObj, 10000+storyConfig.id )
			SetTrue( uiShape )
			SetGray( uiShape, false )
			SetTrue( uiShapeBack )
			SetGray( uiShapeBack, false )
			SetTrue( uiNew )
			StoryDlgShowIcon( uiItem, storyConfig )	
			
		-- 尚未通关
		else
			-- 显示锁定状态的
			if m_recvValue.m_storyid > storyConfig.unlock_preid then
				SetControlID( uiObj, 0 )
				SetFalse( uiShape )
				SetFalse( uiShapeBack )
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
			SetTrue( uiShapeBack )
			SetGray( uiShapeBack, true )
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
			SetTrue( uiShapeBack )
			SetGray( uiShapeBack, false )
			SetTrue( uiNew )
			StoryDlgShowIcon( uiItem, storyConfig )	
			showWarning = 1;
				
		-- 尚未通关
		else
			-- 显示锁定状态的
			if m_recvValue.m_storyid > storyConfig.unlock_preid then
				SetControlID( uiObj, 0 )
				SetFalse( uiShape )
				SetFalse( uiShapeBack )
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
		StoryRankShapeBack( uiShapeBack, storyConfig.shapeback )
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
		-- 已经通关
		if m_recvValue.m_storyid > storyConfig.id then
			SetControlID( uiObj, 10000+storyConfig.id )
			SetTrue( uiShape )
			SetGray( uiShape, true )
			SetTrue( uiShapeBack )
			SetGray( uiShapeBack, true )
			StoryDlgShowIcon( uiItem, storyConfig )	
			
		-- 当前正在打的关卡
		elseif m_recvValue.m_storyid == storyConfig.id then
			SetControlID( uiObj, 10000+storyConfig.id )
			SetTrue( uiShape )
			SetGray( uiShape, false )
			SetTrue( uiShapeBack )
			SetGray( uiShapeBack, false )
			SetTrue( uiNew )
			StoryDlgShowIcon( uiItem, storyConfig )	
				
		-- 尚未通关
		else
			-- 显示锁定状态的
			if m_recvValue.m_storyid > storyConfig.unlock_preid then
				SetControlID( uiObj, 0 )
				SetFalse( uiShape )
				SetFalse( uiShapeBack )
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
		-- 显示资源形象
		if storyConfig.restype > 0 then
			StoryDlgItemShape( uiItem, storyConfig.restype )
		end
		
	elseif type == STORY_TYPE_ITEM then-- 道具
		-- 已经通关
		if m_recvValue.m_storyid > storyConfig.id then
			SetControlID( uiObj, 10000+storyConfig.id )
			SetTrue( uiShape )
			SetGray( uiShape, true )
			SetTrue( uiShapeBack )
			SetGray( uiShapeBack, true )
			StoryDlgShowIcon( uiItem, storyConfig )	
			
		-- 当前正在打的关卡
		elseif m_recvValue.m_storyid == storyConfig.id then
			SetControlID( uiObj, 10000+storyConfig.id )
			SetTrue( uiShape )
			SetGray( uiShape, false )
			SetTrue( uiShapeBack )
			SetGray( uiShapeBack, false )
			SetTrue( uiNew )
			StoryDlgShowIcon( uiItem, storyConfig )	
				
		-- 尚未通关
		else
			-- 显示锁定状态的
			if m_recvValue.m_storyid > storyConfig.unlock_preid then
				SetControlID( uiObj, 0 )
				SetFalse( uiShape )
				SetFalse( uiShapeBack )
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
		-- 显示道具形象
		if storyConfig.item_awardkind > 0 then
			StoryDlgItemShape( uiItem, storyConfig.item_awardkind )
		end
		
	elseif type == STORY_TYPE_HERO then-- 武将招募
		-- 已经通关
		if m_recvValue.m_storyid > storyConfig.id then
			SetControlID( uiObj, 10000+storyConfig.id )
			SetTrue( uiShape )
			SetGray( uiShape, true )
			SetTrue( uiShapeBack )
			SetGray( uiShapeBack, true )
			StoryDlgShowIcon( uiItem, storyConfig )	
			
		-- 当前正在打的关卡
		elseif m_recvValue.m_storyid == storyConfig.id then
			SetControlID( uiObj, 10000+storyConfig.id )
			SetTrue( uiShape )
			SetGray( uiShape, false )
			SetTrue( uiShapeBack )
			SetGray( uiShapeBack, false )
			SetTrue( uiNew )
			StoryDlgShowIcon( uiItem, storyConfig )	
				
		-- 尚未通关
		else
			-- 显示锁定状态的
			if m_recvValue.m_storyid > storyConfig.unlock_preid then
				SetControlID( uiObj, 0 )
				SetFalse( uiShape )
				SetFalse( uiShapeBack )
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
	
	elseif type == STORY_TYPE_DRAWING then-- 装备图纸
		-- 已经通关
		if m_recvValue.m_storyid > storyConfig.id then
			SetControlID( uiObj, 10000+storyConfig.id )
			SetTrue( uiShape )
			SetGray( uiShape, true )
			SetTrue( uiShapeBack )
			SetGray( uiShapeBack, true )
			StoryDlgShowIcon( uiItem, storyConfig )	
			
		-- 当前正在打的关卡
		elseif m_recvValue.m_storyid == storyConfig.id then
			SetControlID( uiObj, 10000+storyConfig.id )
			SetTrue( uiShape )
			SetGray( uiShape, false )
			SetTrue( uiShapeBack )
			SetGray( uiShapeBack, false )
			SetTrue( uiNew )
			StoryDlgShowIcon( uiItem, storyConfig )	
				
		-- 尚未通关
		else
			-- 显示锁定状态的
			if m_recvValue.m_storyid > storyConfig.unlock_preid then
				SetControlID( uiObj, 0 )
				SetFalse( uiShape )
				SetFalse( uiShapeBack )
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
		-- 显示装备图纸
		if storyConfig.drawing_kind > 0 then
			StoryDlgItemShape( uiItem, storyConfig.drawing_kind )
		end
	end
end

-- 选择副本
function StoryDlgSelect( id )
	m_id = id;
	
	local type = g_story[id].type;
	if type == STORY_TYPE_NORMAL then -- 普通副本
		BattleDlgShowByStory( id )
	elseif type == STORY_TYPE_BOSS then-- 精英
		BattleDlgShowByStory( id )
	elseif type == STORY_TYPE_RES then-- 资源
	elseif type == STORY_TYPE_ITEM then-- 道具
		BattleDlgShowByStory( id )
	elseif type == STORY_TYPE_HERO then-- 武将招募
	elseif type == STORY_TYPE_DRAWING then-- 装备图纸
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
	SetImage( uiItem.transform:Find("Shape"), ItemSprite(itemkind) )
	SetTrue( uiItem )
end

-- 设置副本武将图标
function StoryDlgHeroShape( uiItem, herokind )
	SetImage( uiItem.transform:Find("Shape"), HeroHeadSprite(herokind) )
	SetTrue( uiItem )
end

-- 设置副本资源图标
function StoryDlgResShape( uiRoot, restype )
	SetImage( uiItem.transform:Find("Shape"), ResItemIcon( restype ) )
	SetTrue( uiItem )
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
