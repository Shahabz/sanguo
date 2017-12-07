-- 界面
local m_Dlg = nil;
local m_DialogFrameMod = nil;
local m_uiDot = nil; --UnityEngine.GameObject
local m_uiGodHero = nil; --UnityEngine.GameObject
local m_uiGodHeroGray = nil; --UnityEngine.GameObject
local m_uiOpenTime = nil; --UnityEngine.GameObject
local m_uiCloseTime = nil; --UnityEngine.GameObject
local m_uiGodOpenTime = nil; --UnityEngine.GameObject
local m_uiGrayText = nil; --UnityEngine.GameObject
local m_uiHandle = nil; --UnityEngine.GameObject
local m_uiInfoLayer = nil; --UnityEngine.GameObject
local m_uiAwardLayer  = nil; --UnityEngine.GameObject
local m_uiUpToken = nil; --UnityEngine.GameObject
local m_uiLeftToken = nil; --UnityEngine.GameObject
local m_uiRightToken = nil; --UnityEngine.GameObject
local m_uiHeroLayout = nil; --UnityEngine.GameObject
local m_uiOnceText = nil; --UnityEngine.GameObject
local m_uiTenthText = nil; --UnityEngine.GameObject
local m_cacheAward = nil;
local m_tokenObjectArray = {};
local m_heroObjectArray = {};
local m_controlID = 0 ;
-- 打开界面
function HeroVisitDlgOpen()
	ResourceManager.LoadAssetBundle( "_ab_ui_static_herovisit_gray" );
	ResourceManager.LoadAssetBundle( "_ab_ui_static_herovisit_up" );
	m_Dlg = eye.uiManager:Open( "HeroVisitDlg" );
	m_DialogFrameMod = DialogFrameModOpen( m_Dlg, T(1923), HELP_HeroDlg, HeroVisitDlgClose );
end

-- 隐藏界面
function HeroVisitDlgClose()
	if m_Dlg == nil then
		return;
	end
	DialogFrameModClose( m_DialogFrameMod );
	m_DialogFrameMod = nil;
	eye.uiManager:Close( "HeroVisitDlg" );
end

-- 删除界面
function HeroVisitDlgDestroy()
	GameObject.Destroy( m_Dlg );
	ResourceManager.UnloadAssetBundle( "_ab_ui_static_herovisit_gray" )
	ResourceManager.UnloadAssetBundle( "_ab_ui_static_herovisit_up" )
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function HeroVisitDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            HeroVisitDlgClose();	
		elseif nControlID == 1 then --聚贤馆
			
		elseif nControlID == 2 then --良将寻访
			HeroVisitDlgOpenAward(nControlID);
		elseif nControlID == 3 then
			HeroVisitDlgOpenAward(nControlID);
		elseif nControlID >= 20000 then
			if m_controlID	==2 then
				HeroConfigDlgShow( g_heroinfo[nControlID-20000][3]);
			else
				HeroConfigDlgShow( g_heroinfo[nControlID-20000][2]);
			end
			
        end		
	elseif nType == UI_EVENT_TIMECOUNTEND  then
		if nControlID == 1 then
			print("Timer()");
			HeroVisitDlgShow();
		end
	end
end

-- 载入时调用
function HeroVisitDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiDot = objs[0];
	m_uiGodHero = objs[1];
	m_uiGodHeroGray = objs[2];
	m_uiOpenTime = objs[3];
	m_uiCloseTime = objs[4];
	m_uiGodOpenTime = objs[5];
	m_uiGrayText = objs[6];
	m_uiHandle = objs[7];
	m_uiInfoLayer = objs[8];
	m_uiAwardLayer  = objs[9];
	m_uiUpToken = objs[10];
	m_uiLeftToken = objs[11];
	m_uiRightToken = objs[12];
	m_uiHeroLayout = objs[13];
	m_uiOnceText = objs[14];
	m_uiTenthText = objs[15];
	HeroVisitDlgGetObject();
end

-- 界面初始化时调用
function HeroVisitDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function HeroVisitDlgOnEnable( gameObject )
	if IsActive(m_uiInfoLayer) then
		return;
	else
		SetTrue(m_uiInfoLayer);
		SetFalse(m_uiAwardLayer);
	end	
end

-- 界面隐藏时调用
function HeroVisitDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function HeroVisitDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function HeroVisitDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function HeroVisitDlgShow()
	HeroVisitDlgOpen()
	m_cacheAward = nil;
	system_askinfo( ASKINFO_HERO_VISIT, "", 0 );
end

-- m_hv_free_cd=0,m_hv_high_sec=0,m_hv_high_free=0,m_hv_low_num=0,m_hv_high_num=0,m_hv_progress=0,
function HeroVisitDlgRecv( recvValue )
	
	-- m_hv_free_cd=0,良将寻访免费CD
	-- m_hv_high_sec=0,神将寻访解锁时长
	-- m_hv_high_free=0,神将寻访剩余免费次数
	-- m_hv_low_num=0,良将寻访次数
	-- m_hv_high_num=0,神将寻访次数
	-- m_hv_progress=0,寻访进度
--[[
global.hero_visit_actorlevel = 88 --武将寻访玩家等级限制
global.hero_visit_mainlevel = 17 --武将寻访官府等级限制
global.hero_visit_low_token10 = 1800 --良将寻访十连钻石
global.hero_visit_high_token = 800 --神将寻访钻石
global.hero_visit_high_token10 = 6400 --神将寻访十连钻石
global.hero_visit_low_itemnum = 1 --良将寻访道具数
global.hero_visit_low_itemnum10 = 10 --良将寻访十连道具数
global.hero_visit_high_itemnum = 1 --神将寻访道具数
global.hero_visit_high_itemnum10 = 10 --神将寻访十连道具数
global.hero_visit_low_max = 10 --良将特殊抽取达到次数
global.hero_visit_high_max = 10 --神将特殊抽取达到次数
global.hero_visit_low_normal_award = 1 --良将普通寻访获得道具和武将
global.hero_visit_low_hero_award = 2 --良将特殊寻访获得武将
global.hero_visit_high_normal_award = 3 --神将普通寻访获得道具和武将
global.hero_visit_high_hero_award = 4 --神将特殊寻访获得武将
global.hero_visit_progress_color1 = 10 --寻访突破获取进度
global.hero_visit_progress_color2 = 5 --寻访突破获取进度
global.hero_visit_progress_color3 = 15 --寻访突破获取进度
global.hero_visit_progress_color4 = 50 --寻访突破获取进度
global.hero_visit_progress_normal = 2 --寻访获取进度
--]]
	recvValue.m_hv_free_cd = 0;
	recvValue.m_hv_high_free = 3000;
	recvValue.m_hv_progress = 100;
	
	if	recvValue.m_hv_free_cd == 0 then
		SetTrue(m_uiDot);
		SetFalse(m_uiOpenTime);
		SetTrue(m_uiCloseTime);
		SetTimer( m_uiOpenTime, 0, 0, 0 );
	else
		SetFalse(m_uiDot);
		SetTrue(m_uiOpenTime);
		SetFalse(m_uiCloseTime);
		SetTimer( m_uiOpenTime, recvValue.m_hv_free_cd, recvValue.m_hv_free_cd, 1,T(1950) );
	end
	if recvValue.m_hv_progress >= global.hero_visit_progress_max then
		SetTrue(m_uiGodHero);
		SetFalse(m_uiGodHeroGray);
		SetTimer( m_uiGodOpenTime, recvValue.m_hv_high_free, recvValue.m_hv_high_free, 2,T(1954) );
	else
		SetTimer( m_uiGodOpenTime, 0, 0, 0 );
		SetTrue(m_uiGodHeroGray);
		SetFalse(m_uiGodHero);
		local floorNum = recvValue.m_hv_progress / global.hero_visit_progress_max ;
		local intNum = math.floor(floorNum*100);
		SetText(m_uiGrayText,F(1957,intNum));
		m_uiHandle.transform:GetComponent( "Image" ).fillAmount = floorNum;
	end
end

-- 良将寻访
function HeroVisitDlgLow()
	-- 关键条件判断
	
	-- 发送信息
	system_askinfo( ASKINFO_HERO_VISIT, "", 1, 0 );
end

-- 良将寻访10连
function HeroVisitDlgLow10()
	-- 关键条件判断
	
	-- 发送信息
	system_askinfo( ASKINFO_HERO_VISIT, "", 1, 1 );
end

-- 神将寻访
function HeroVisitDlgHigh()
	-- 关键条件判断
	
	-- 发送信息
	system_askinfo( ASKINFO_HERO_VISIT, "", 2, 0 );
end

-- 神将寻访10连
function HeroVisitDlgHigh()
	-- 关键条件判断
	
	-- 发送信息
	system_askinfo( ASKINFO_HERO_VISIT, "", 2, 1 );
end

-- 添加获取的奖励到缓存type==1道具 type==2武将
function HeroVisitDlgAwardAdd( type, recvValue )
	table.insert( m_cacheAward, { awardType=type, awardInfo=recvValue } ) 
end

-- 播放奖励动画
function HeroVisitDlgAwardPlayAction()
	if m_cacheAward == nil then
		return
	end
	if #m_cacheAward == 1 then
		-- 播放单个道具或武将的动画
		
	else
		-- 播放多个道具或武将的动画
	end
end

--设置良将
function HeroVisitDlgSetGood()

end 

--设置神将 
function HeroVisitDlgSetGod()
	
end
--关闭聚贤馆 打开寻访界面 并填充每个模块
function HeroVisitDlgOpenAward( controlid )	
	if controlid == 2 then
		if m_controlID == 2 then
			SetTrue(m_uiAwardLayer);
			SetFalse(m_uiInfoLayer);
			return;
		end	
		m_controlID = 2;
		local goodHeroList = {};
		for key, value in pairs(g_hero_visit) do      
			if global.hero_visit_low_normal_award == value.awardgroup then
				table.insert(goodHeroList,value);
			end
		end	
		local tokenCount = 1;
		local heroCount = 1;
		for key,value in pairs(goodHeroList) do	
			if value.kind >= 20000 then
				HeroVisitDlgSetCell(m_heroObjectArray[heroCount],value);
				heroCount=heroCount+1;
			else
				HeroVisitDlgSetCell(m_tokenObjectArray[tokenCount],value);
				tokenCount=tokenCount+1;
			end
		end	
		SetText(m_uiOnceText,global.hero_visit_low_token)	
		SetText(m_uiTenthText,global.hero_visit_low_token10)		
	elseif controlid == 3 then
		if m_controlID == 3 then
			SetTrue(m_uiAwardLayer);
			SetFalse(m_uiInfoLayer);
			return;
		end	
		m_controlID = 3;
		local goodHeroList = {};
		for key, value in pairs(g_hero_visit) do      
			if global.hero_visit_high_normal_award == value.awardgroup then
				table.insert(goodHeroList,value);
			end
		end	
		local tokenCount = 1;
		local heroCount = 1;
		for key,value in pairs(goodHeroList) do	
			if value.kind >= 20000 then
				HeroVisitDlgSetCell(m_heroObjectArray[heroCount],value);
				heroCount=heroCount+1;
			else
				HeroVisitDlgSetCell(m_tokenObjectArray[tokenCount],value);
				tokenCount=tokenCount+1;
			end
		end	
		SetText(m_uiOnceText,global.hero_visit_high_token)
		SetText(m_uiTenthText,global.hero_visit_high_token10)		
	end	
	SetTrue(m_uiAwardLayer);
	SetFalse(m_uiInfoLayer);				
end

--获得道具和英雄的GameObject
function HeroVisitDlgGetObject()

	for i = 0,m_uiLeftToken.transform.childCount-1  do
		table.insert(m_tokenObjectArray,m_uiLeftToken.transform:GetChild(i));
	end
	for i = 0,m_uiRightToken.transform.childCount-1  do
		
		table.insert(m_tokenObjectArray,m_uiRightToken.transform:GetChild(i));
	end	
	for i = 0,m_uiUpToken.transform.childCount-1  do
		table.insert(m_tokenObjectArray,m_uiUpToken.transform:GetChild(i));
	end	
	for i = 0,m_uiHeroLayout.transform.childCount-1  do
		table.insert(m_heroObjectArray,m_uiHeroLayout.transform:GetChild(i));
	end	
end
	--[[
	local goodHeroList = {};
	for key, value in pairs(g_hero_visit) do      
		if global.hero_visit_low_normal_award == value.awardgroup then
			table.insert(goodHeroList,value);
		end
	end
	local tokenCount = 1;
	local heroCount = 1;
	for key,value in pairs(goodHeroList) do	
		--local sprite, color, name = AwardInfo(value.kind  ) ;	
		if value.kind >= 20000 then
			HeroVisitDlgSetCell(m_heroObjectArray[heroCount],value);
			heroCount=heroCount+1;
		else
			HeroVisitDlgSetCell(m_tokenObjectArray[tokenCount],value);
			tokenCount=tokenCount+1;
		
			SetImage(m_tokenObjectArray[tokenCount].transform:Find("Shape"),sprite);
			SetImage( m_tokenObjectArray[tokenCount].transform:Find("Color"), ItemColorSprite(value.color) );
			SetText(m_tokenObjectArray[tokenCount].transform:Find("Num"),"x"..value.num,NameColor(value.color));
			tokenCount=tokenCount+1;
		
		end
	end
	--]]

--设置单个对象
function HeroVisitDlgSetCell(uiHeroObj,value)
	local sprite, color, name = AwardInfo(value.kind  ) ;
	local colorSprite = ItemColorSprite(value.color);	
	local objs = uiHeroObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	if value.kind >= 20000 then
		local uiShape = objs[0];
		local uiColor = objs[1];
		local uiCorps = objs[2];
		local uiName = objs[3];
		local uiSelect = objs[4];
		local uiAlreadlyHave = objs[5]
		SetImage(uiShape,sprite);
		SetImage(uiColor,colorSprite);
		SetImage(uiCorps,CorpsSprite(g_heroinfo[(value.kind-20000)][value.color].corps));
		SetText(uiName,name);
		uiHeroObj:GetComponent("UIButton").controlID = value.kind;
		local haveHero = GetHero():GetPtr(value.kind-20000);
		if haveHero ~= nil  then
			SetTrue(uiAlreadlyHave);
		else
			SetFalse(uiAlreadlyHave);
		end
	else
		local uiShape = objs[0];
		local uiColor = objs[1];
		local uiName = objs[2];
		SetImage(uiShape,sprite);
		SetImage(uiColor,colorSprite);
		SetText(uiName,"x"..value.num,NameColor(value.color));
	end
end
--点击英雄头像
function HeroVisitDlgSelect( colickid )
	
	
end








