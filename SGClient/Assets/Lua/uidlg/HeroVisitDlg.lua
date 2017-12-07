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
local m_combineObjectArray = {};
local b_isVisit = false ;

local m_controlID = 0 ;


local sumtime = 0;
local delttime = 18 ;
local transformNum = 1;
local trsndfsum = 25
local lastSelectId = 0;
local selectid = 0 ;
local nextid = 0 ;
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
	
	-- 此处判断当前在哪个页，如果在奖励页就关闭奖励页，显示信息页
	
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
		elseif nControlID == 4 then	--寻访1次
			 selectid = HeroVisitDlgTransform(20)
			print("selectid:"..selectid);
			 b_isVisit = true ;
		elseif nControlID == 5 then	--寻访10次
		
		elseif nControlID >= 17 and 400 > nControlID then
			local kindid = g_hero_visit[nControlID].kind ;
			if kindid >= 20000 then
				if m_controlID	==2 then
					HeroConfigDlgShow( g_heroinfo[kindid-20000][3]);
				else
					HeroConfigDlgShow( g_heroinfo[kindid-20000][2]);
				end				
			end
        end		
	elseif nType == UI_EVENT_TIMECOUNTEND  then
		if nControlID == 1 then
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
	
	if b_isVisit == true then
		sumtime = sumtime + Time.deltaTime * delttime;
		if sumtime > 1 then
			sumtime = 0 
			if transformNum == 1 then
				if lastSelectId ~= 0 then
					SetFalse(m_combineObjectArray[lastSelectId].transform:Find("Select"));
					lastSelectId = 0;
				else
					SetFalse(m_combineObjectArray[25].transform:Find("Select"));
				end
			else
				SetFalse(m_combineObjectArray[transformNum-1].transform:Find("Select"));
			end
			SetTrue(m_combineObjectArray[transformNum].transform:Find("Select"));
			if nextid == 2 then
				if transformNum == selectid then
					b_isVisit = false;
					transformNum = 1;
					nextid = 0;
					trsndfsum=25;
					lastSelectId=selectid;
					return ;
				end
			end			
			if transformNum == 25 then			
				transformNum = 1;
				nextid = nextid +1 ;
			else 
				transformNum = transformNum + 1 ;
			end
		end
	end
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

-- 播放奖励动画
-- m_id={[10]},m_count=0,
function HeroVisitDlgRecvAward( recvValue )
	m_cacheAward = recvValue
	if m_cacheAward == nil then
		return
	end
	if m_cacheAward.m_count <= 0 then
		return
	end
	if m_cacheAward.m_count == 1 then
		-- 播放单个道具或武将的动画
		for k, v in pairs( g_hero_visit ) do      
			if v.id == m_cacheAward.m_id[1] then
				-- 奖励武将
				if v.kind >= AWARDKIND_HEROBASE then
					-- 武将配置信息
					local pHeroConfig = g_heroinfo[v.kind][v.color]
					if pHeroConfig == nil then
						break
					end
					local hero = { m_kind = v.kind, m_color = v.color, m_level = 1, m_corps = pHeroConfig.corps, m_exp=0,m_exp_max=0,m_soldiers=0,m_state=0,
						m_attack_base = pHeroConfig.attack_base, m_attack_wash = pHeroConfig.attack_wash,
						m_defense_base = pHeroConfig.defense_base, m_defense_wash = pHeroConfig.defense_wash,
						m_troops_base = pHeroConfig.troops_base, m_troops_wash = pHeroConfig.troops_wash,
						m_attack = pHeroConfig.attack,m_defense = pHeroConfig.defense,m_troops = pHeroConfig.troops, m_offset = 0 }
					HeroGetDlgShow( hero );
					
				-- 奖励道具
				else
					ItemGetDlgShow( v.kind, v.num )
				end
				break
			end
		end	
	
	else
		-- 播放多个道具或武将的动画
		-- 原理同上，只不过这里循环10个动画显示
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
--
function HeroVisitDlgSort(a,b)
	return 	tonumber(a.transform.name) < tonumber(b.transform.name)
end
--获得道具和英雄的GameObject
function HeroVisitDlgGetObject()

	for i = 0,m_uiLeftToken.transform.childCount-1  do
		table.insert(m_tokenObjectArray,m_uiLeftToken.transform:GetChild(i));
		table.insert(m_combineObjectArray,m_uiLeftToken.transform:GetChild(i));
	end
	for i = 0,m_uiRightToken.transform.childCount-1  do		
		table.insert(m_tokenObjectArray,m_uiRightToken.transform:GetChild(i));
		table.insert(m_combineObjectArray,m_uiRightToken.transform:GetChild(i));
	end	
	for i = 0,m_uiUpToken.transform.childCount-1  do
		table.insert(m_tokenObjectArray,m_uiUpToken.transform:GetChild(i));
		table.insert(m_combineObjectArray,m_uiUpToken.transform:GetChild(i));
	end	
	for i = 0,m_uiHeroLayout.transform.childCount-1  do
		table.insert(m_heroObjectArray,m_uiHeroLayout.transform:GetChild(i));
		table.insert(m_combineObjectArray,m_uiHeroLayout.transform:GetChild(i));
	end	
	table.sort(m_combineObjectArray,HeroVisitDlgSort)

	
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
	uiHeroObj:GetComponent("UIButton").controlID = value.id;
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
-- 通过id找出寻访物品位置
function HeroVisitDlgTransform( id )
	local transformid  = 0;
	for i = 1 ,#m_combineObjectArray do
		if id == m_combineObjectArray[i].transform:GetComponent("UIButton").controlID then
			transformid = i
			
		end
	end
	return transformid ;
end








