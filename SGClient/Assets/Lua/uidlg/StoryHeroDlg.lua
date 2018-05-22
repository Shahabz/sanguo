-- 界面
local m_Dlg = nil;
local m_uiDesc = nil; --UnityEngine.GameObject
local m_uiTalk = nil; --UnityEngine.GameObject
local m_uiHeroGrid = nil; --UnityEngine.GameObject
local m_uiCost = nil; --UnityEngine.GameObject
local m_uiCallBtn = nil; --UnityEngine.GameObject
local m_uiBuyBtn = nil; --UnityEngine.GameObject
local m_storyid = 0;

-- 打开界面
function StoryHeroDlgOpen()
	m_Dlg = eye.uiManager:Open( "StoryHeroDlg" );
end

-- 隐藏界面
function StoryHeroDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "StoryHeroDlg" );
end

-- 删除界面
function StoryHeroDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function StoryHeroDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            StoryHeroDlgClose();
		elseif nControlID == 1 then
			StoryHeroDlgFree()
		elseif nControlID == 2 then
			StoryHeroDlgToken()
        end
	end
end

-- 载入时调用
function StoryHeroDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiDesc = objs[0];
	m_uiTalk = objs[1];
	m_uiHeroGrid = objs[2];
	m_uiCost = objs[3];
	m_uiCallBtn = objs[4];
	m_uiBuyBtn = objs[5];
end

-- 界面初始化时调用
function StoryHeroDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function StoryHeroDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function StoryHeroDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function StoryHeroDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function StoryHeroDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function StoryHeroDlgShow( storyid, info )
	StoryHeroDlgOpen()
	m_storyid = storyid
	SetFalse( m_uiHeroGrid.transform:GetChild( 0 ) )
	SetFalse( m_uiHeroGrid.transform:GetChild( 1 ) )
	local storyConfig = g_story[storyid]
	local save_offset = storyConfig.hero_saveoffset;
	local iscallover = info[save_offset+1];
	
	if storyConfig.hero_kind0 > 0 and storyConfig.hero_kind1 > 0 then
		if iscallover == 11 then
			-- 已经招募完毕
			StoryHeroDlgClose()
			return
		end
		
		if iscallover == 1 then
			StoryHeroDlgSetHero( m_uiHeroGrid.transform:GetChild( 0 ), storyConfig.hero_kind1 )
		elseif iscallover == 10 then
			StoryHeroDlgSetHero( m_uiHeroGrid.transform:GetChild( 0 ), storyConfig.hero_kind0 )
		elseif iscallover == 0 then
			StoryHeroDlgSetHero( m_uiHeroGrid.transform:GetChild( 0 ), storyConfig.hero_kind0 )
			StoryHeroDlgSetHero( m_uiHeroGrid.transform:GetChild( 1 ), storyConfig.hero_kind1 )
		end
		
		if iscallover == 1 or iscallover == 10 then
			SetTrue( m_uiTalk )
			SetTrue( m_uiCost )
			SetFalse( m_uiCallBtn )
			SetTrue( m_uiBuyBtn )
			SetText( m_uiCost.transform:Find("Text"), storyConfig.hero_token )
		elseif iscallover == 0 then
			SetFalse( m_uiTalk )
			SetFalse( m_uiCost )
			SetTrue( m_uiCallBtn )
			SetFalse( m_uiBuyBtn )
		end
		
	elseif storyConfig.hero_kind0 > 0 then
		if iscallover == 1 then
			-- 已经招募完毕
			StoryHeroDlgClose()
			return
		end
		StoryHeroDlgSetHero( m_uiHeroGrid.transform:GetChild( 0 ), storyConfig.hero_kind0 )
		
		if storyConfig.hero_token > 0 then
			SetTrue( m_uiTalk )
			SetTrue( m_uiCost )
			SetFalse( m_uiCallBtn )
			SetTrue( m_uiBuyBtn )
			SetText( m_uiCost.transform:Find("Text"), storyConfig.hero_token )
		else
			SetTrue( m_uiTalk )
			SetFalse( m_uiCost )
			SetTrue( m_uiCallBtn )
			SetFalse( m_uiBuyBtn )
		end
		
		if IsGuiding() then
			if GetCurrentGuideType() == GUIDE_COPY_HERO then
				GuideNext();
			end
			if GetCurrentGuideType() == GUIDE_GET_HERO then 
				FindCmdTpye(m_uiCallBtn.transform);
			end
		end
		
	elseif storyConfig.hero_kind1 > 0 then
		if iscallover == 1 or iscallover == 10 then
			-- 已经招募完毕
			StoryHeroDlgClose()
			return
		end
		StoryHeroDlgSetHero( m_uiHeroGrid.transform:GetChild( 0 ), storyConfig.hero_kind1 )
		
		if storyConfig.hero_token > 0 then
			SetTrue( m_uiTalk )
			SetTrue( m_uiCost )
			SetFalse( m_uiCallBtn )
			SetTrue( m_uiBuyBtn )
			SetText( m_uiCost.transform:Find("Text"), storyConfig.hero_token )
		else
			SetTrue( m_uiTalk )
			SetFalse( m_uiCost )
			SetTrue( m_uiCallBtn )
			SetFalse( m_uiBuyBtn )
		end
	end
		
end

-- 设置武将信息
function StoryHeroDlgSetHero( uiObj, herokind )
	local color = hero_getnormalcolor( herokind )
	local config = g_heroinfo[herokind][color];
	if config == nil then
		SetFalse( uiObj )
		return
	end
	SetTrue( uiObj )
	SetImage( uiObj.transform:Find("Back"), HeroColorSprite( color ) )
	SetImage( uiObj.transform:Find("Shape"), HeroFaceSprite( herokind )  );
	--SetImage( uiObj.transform:Find("Color"),  ItemColorSprite( color )  );
	SetImage( uiObj.transform:Find("Corps"),  CorpsSprite( config.corps )  );
	SetText( uiObj.transform:Find("Name"), HeroName( herokind ), NameColor( color ) )
	local uiGrid = uiObj.transform:Find("Grid");
	
	-- 总资质
	local attrname = T(149)..": <color=#f7f3bbff>{0}</color> +<color=#03DE27FF>{1}</color>"
	local total = config.attack_base + config.defense_base + config.troops_base;
	local total_wash = config.attack_wash + config.defense_wash + config.troops_wash;
	if total_wash == 0 then
		attrname = T(149)..": <color=#f7f3bbff>{0}</color>"
	end
	local uiTotalGrowth = uiGrid.transform:GetChild(0).gameObject;
	SetText( uiTotalGrowth.transform:Find("Text"), Utils.StringFormat( attrname, total, total_wash ) )
	
	-- 攻
	local uiAttackBase = uiGrid.transform:GetChild(1).gameObject;
	SetText( uiAttackBase.transform:Find("Text"), F(138, (config.attack_base+config.attack_wash) ) );

	-- 防
	local uiDefenseBase = uiGrid.transform:GetChild(2).gameObject;
	SetText( uiDefenseBase.transform:Find("Text"), F(139, (config.defense_base+config.defense_wash)) );

	-- 兵
	local uiTroopsBase = uiGrid.transform:GetChild(3).gameObject;
	SetText( uiTroopsBase.transform:Find("Text"), F(140, (config.troops_base+config.troops_wash)) );
end

-- 免费获取
function StoryHeroDlgFree()
	system_askinfo( ASKINFO_STORY, "", 4, m_storyid );
	if IsGuiding() and GetCurrentGuideType() == GUIDE_GET_HERO then 
		GuideNext();
	end
	StoryHeroDlgClose()
end

-- 钻石购买
function StoryHeroDlgToken()
	system_askinfo( ASKINFO_STORY, "", 5, m_storyid );
	StoryHeroDlgClose()
end

