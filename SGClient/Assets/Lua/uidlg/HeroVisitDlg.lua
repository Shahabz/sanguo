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
local m_uiTimesText = nil; --UnityEngine.GameObject
local m_uiOneUpText = nil; --UnityEngine.GameObject
local m_uiTenUpText = nil; --UnityEngine.GameObject
local m_uiOnePage = nil; --UnityEngine.GameObject
local m_uiTenPage = nil; --UnityEngine.GameObject
local m_uiTenTokenText = nil; --UnityEngine.GameObject
local m_uiAward = nil; --UnityEngine.GameObject
local m_uiAwardGrop = nil; --UnityEngine.GameObject
local m_uiTenAgainVisitBtn = nil; --UnityEngine.GameObject
local m_uiTenthBtn = nil; --UnityEngine.GameObject
local m_uiOnceBtn = nil; --UnityEngine.GameObject
local m_uiFreeItemBtnGroup = nil; --UnityEngine.GameObject
local m_uiFreeItemText = nil; --UnityEngine.GameObject
local m_uiFreeItemBtn = nil; --UnityEngine.GameObject
local m_uiGodDot = nil; --UnityEngine.GameObject

local m_recvValue = nil;
local m_cacheAward = nil;
local m_tokenObjectArray = {};
local m_heroObjectArray = {};
local m_combineObjectArray = {};
local i_goodTimesNum = 0 ; --良将寻访次数
local i_godTimesNum = 0 ; --神将寻访次数
local i_TimesNum = 0 ;  -- 再寻访几次得特殊武将 
local b_isVisit = false ; --控制一次寻访
local b_isTenVisit = false ;--控制十连寻访
local i_controlID = 0 ;
local f_awardPosition = {}; --存储十连抽每个奖励的位置
local i_tweenId = 1; --十连寻访每个奖励物品的nControlID 
local i_freeTimes = 0 ; --免费寻访次数
local i_itemTimes = 0 ;--道具数量
local t_getHero ={} ; --已用于英雄列表
local objCache = {
 [101] = 17 ,[102] = 18,[103] = 19,[104] = 20,[105]= 21,[106] = 22,[107] = 23,[108] = 24,[109] = 25,
 [301] = 217 ,[302] = 218,[303] = 219,[304] = 220,[305]= 222,[306] = 223,[307] = 224,[308] = 225,[309] = 226
	}

-- 打开界面
function HeroVisitDlgOpen()
	ResourceManager.LoadAssetBundle( "_ab_ui_static_cabinet1" );
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
	if b_isVisit == true then
		return;
	end
	if IsActive(m_uiTenPage) then
		clearChild(m_uiAwardGrop);
		i_tweenId = 1;
		SetFalse(m_uiTenPage);
		SetTrue(m_uiOnePage);
		return;
	end
	-- 此处判断当前在哪个页，如果在奖励页就关闭奖励页，显示信息页
	if IsActive(m_uiInfoLayer) then
		DialogFrameModClose( m_DialogFrameMod );
		m_DialogFrameMod = nil;
		eye.uiManager:Close( "HeroVisitDlg" );
	else
		SetTrue(m_uiInfoLayer);
		SetFalse(m_uiAwardLayer);
	end	
end

-- 删除界面
function HeroVisitDlgDestroy()
	GameObject.Destroy( m_Dlg );
	ResourceManager.UnloadAssetBundle( "_ab_ui_static_herovisit_gray" )
	ResourceManager.UnloadAssetBundle( "_ab_ui_static_herovisit_up" )
	ResourceManager.UnloadAssetBundle( "_ab_ui_static_cabinet1" );
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
			
		elseif nControlID == 2 then --良将寻访进入
			HeroVisitDlgOpenAward(nControlID);
		elseif nControlID == 3 then --神将寻访进去
			HeroVisitDlgOpenAward(nControlID);
		elseif nControlID == 4 then	--寻访1次
			if i_controlID == 2 then
				HeroVisitDlgLow();
			elseif i_controlID == 3 then
				HeroVisitDlgHigh();
			else
				print("i_controlID value error ");
			end
		elseif nControlID == 5 then	--寻访10次
			i_tweenId=1;
			if i_controlID == 2 then
				HeroVisitDlgLow10();
			elseif i_controlID == 3 then
				HeroVisitDlgHigh10();
			else
				print("i_controlID value error ");
			end	
		elseif nControlID == 6 then --免费寻访和道具寻访
			if i_controlID == 2 then
				HeroVisitDlgLow();
			elseif i_controlID == 3 then
				HeroVisitDlgHigh();
			else
				print("i_controlID value error ");
			end
		elseif nControlID >= 17 and nControlID < 400 then
			HeroVisitDlgSelect(nControlID);
        end		
	elseif nType == UI_EVENT_TIMECOUNTEND  then
		if nControlID == 1 then
			HeroVisitDlgShow();
		end
	elseif nType == UI_EVENT_TWEENFINISH then
		if nControlID == 1 then
			if b_isTenVisit == true then
				HeroVisitDlgSetCellTen(g_hero_visit[m_cacheAward.m_id[i_tweenId]]);
			end
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
	m_uiTimesText = objs[16];
	m_uiOneUpText = objs[17];
	m_uiTenUpText = objs[18];
	m_uiOnePage = objs[19];
	m_uiTenPage = objs[20];
	m_uiTenTokenText = objs[21];
	m_uiAward = objs[22];
	m_uiAwardGrop = objs[23];
	m_uiTenAgainVisitBtn = objs[24];
	m_uiTenthBtn = objs[25];
	m_uiOnceBtn = objs[26];
	m_uiFreeItemBtnGroup = objs[27];
	m_uiFreeItemText = objs[28];
	m_uiFreeItemBtn = objs[29];
	m_uiGodDot = objs[30];
	--HeroVisitDlgGetObject();
end

-- 界面初始化时调用
function HeroVisitDlgOnStart( gameObject )
	HeroVisitDlgGetObject();
end

-- 界面显示时调用
function HeroVisitDlgOnEnable( gameObject )

end

-- 界面隐藏时调用
function HeroVisitDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function HeroVisitDlgOnDestroy( gameObject )
	m_Dlg = nil;
end
local logic_sumTime = 0; --选中奖励物体转动效果两物体间隔时间
local logic_deltTime = 25 ; --时间插值 调节转动速度
local logic_mindeltTime = 2 ; --最小时间插值 转动速度最小值
local logic_deltTimeMed = 0 ; --每次递减的速度
local logic_objectSum = 25  --显示的奖励英雄道具总数
local transformNum = 1;	--寻访过程中的位置变化
local logic_lastSelectId = 0; --记录上次寻访奖励的位置
local logic_selectid = 0 ; --本次寻访奖励的位置s
local logic_times = 0 ; --寻访圈数
local logic_value = nil ; -- 奖励信息

-- 每帧调用
function HeroVisitDlgOnLogic( gameObject )	
	
	if b_isVisit == true then
		HeroVisitDlgVisitBefore();
		logic_sumTime = logic_sumTime + Time.deltaTime * logic_deltTime;
		if logic_sumTime > 1 then
			logic_sumTime = 0 
			if transformNum == 1 then
				if logic_lastSelectId ~= 0 then
					SetFalse(m_combineObjectArray[logic_lastSelectId].transform:Find("Select"));
					logic_lastSelectId = 0;
				else
					SetFalse(m_combineObjectArray[25].transform:Find("Select"));
				end
			else
				SetFalse(m_combineObjectArray[transformNum-1].transform:Find("Select"));
			end
			SetTrue(m_combineObjectArray[transformNum].transform:Find("Select"));
			if logic_times == 2 then
				if transformNum == logic_selectid then
					HeroVisitDlgSetAnimation(logic_value);
					if transformNum > 16 then
						SetTrue(m_combineObjectArray[transformNum].transform:Find("AlreadlyHave"));
					end
					transformNum = 1;
					logic_times = 0;
					logic_objectSum = 25;
					logic_lastSelectId = logic_selectid;
					logic_deltTime = 25;
					logic_deltTimeMed = 0;
					b_isVisit = false;
					HeroVisitDlgVisitLater();
					return ;
				end
				if logic_deltTime - logic_deltTimeMed < logic_mindeltTime then
					logic_deltTime = logic_mindeltTime ;
				else
					logic_deltTime =logic_deltTime - logic_deltTimeMed;
				end
			end			
			if transformNum == 25 then			
				transformNum = 1;
				logic_times = logic_times +1 ;
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
	m_recvValue = recvValue;
	i_goodTimesNum = recvValue.m_hv_low_num ;
	i_godTimesNum = recvValue.m_hv_high_num ;
	if	recvValue.m_hv_free_cd == 0 then
		i_freeTimes = 1;
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
	if recvValue.m_hv_high_sec > 0  then
		i_freeTimes = recvValue.m_hv_high_free;
		if i_freeTimes > 0 then
			SetTrue(m_uiGodDot);
		else
			SetFalse(m_uiGodDot);
		end
		SetTrue(m_uiGodHero);
		SetFalse(m_uiGodHeroGray);
		SetTimer( m_uiGodOpenTime, recvValue.m_hv_high_sec,recvValue.m_hv_high_sec, 2,T(1954) );
	else
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
	-- 
	-- m_hv_free_cd=0,m_hv_high_sec=0,m_hv_high_free=0,m_hv_low_num=0,m_hv_high_num=0,m_hv_progress=0,
	if m_recvValue.m_hv_free_cd <= 0 then
		i_TimesNum = i_TimesNum -1 ;
		if i_TimesNum == 0 then
			i_TimesNum = 10 ;
		end
		SetText(m_uiTimesText,F(1963,i_TimesNum));
		system_askinfo( ASKINFO_HERO_VISIT, "", 1, 0 );		
		return
	end
	
	local token = global.hero_visit_low_token
	MsgBox(F(1978,token,1),function()
		if GetPlayer().m_token < token then
			JumpToken();
		else
			i_TimesNum = i_TimesNum -1 ;
			if i_TimesNum == 0 then
				i_TimesNum = 10 ;
			end
			SetText(m_uiTimesText,F(1963,i_TimesNum));
			system_askinfo( ASKINFO_HERO_VISIT, "", 1, 0 );		
		end
	end);
end

-- 良将寻访10连
function HeroVisitDlgLow10()
	-- 关键条件判断
	local token = global.hero_visit_low_token10
	MsgBox(F(1978,token,10),function()
		if GetPlayer().m_token < token then
			JumpToken();
		else
			if IsActive(m_uiOnePage) then		
				SetFalse(m_uiOnePage);
				SetTrue(m_uiTenPage);
				SetText(m_uiTenTokenText,global.hero_visit_low_token10);
			end
			if #f_awardPosition == 0 then
				HeroVisitDlgGetPosition();
			else
				clearChild(m_uiAwardGrop);
			end
			-- 发送信息
			system_askinfo( ASKINFO_HERO_VISIT, "", 1, 1 );
		end				
	end);	
end

-- 神将寻访
function HeroVisitDlgHigh()
	-- 关键条件判断
	local token = global.hero_visit_high_token
	MsgBox(F(1979,token,1),function()
		if GetPlayer().m_token < token then
			JumpToken();
		else
			i_TimesNum = i_TimesNum -1 ;
			if i_TimesNum == 0 then
				i_TimesNum = 10 ;
			end
			SetText(m_uiTimesText,F(1964,i_TimesNum));

			-- 发送信息
			system_askinfo( ASKINFO_HERO_VISIT, "", 2, 0 );
		end
	 end);
end

-- 神将寻访10连
function HeroVisitDlgHigh10()
	-- 关键条件判断
	local token = global.hero_visit_high_token10
	MsgBox(F(1979,token,10),function()
		if GetPlayer().m_token < token then
			JumpToken();
		else
			if IsActive(m_uiOnePage) then		
				SetFalse(m_uiOnePage);
				SetTrue(m_uiTenPage);
				SetText(m_uiTenTokenText,global.hero_visit_high_token10);
			end
			if #f_awardPosition == 0 then
				HeroVisitDlgGetPosition();
			else
				clearChild(m_uiAwardGrop);
			end	
			-- 发送信息
			system_askinfo( ASKINFO_HERO_VISIT, "", 2, 1 );
		end					
	 end);
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
				logic_value = v ;
				logic_selectid = HeroVisitDlgTransform(v.id);
				logic_deltTimeMed =logic_deltTime / math.ceil( logic_selectid - 1 ) ;
				b_isVisit = true ;
				break
			end
		end	
	else
		b_isTenVisit = true;
		HeroVisitDlgSetCellTen(g_hero_visit[m_cacheAward.m_id[i_tweenId]]);
		if IsActive(m_uiTenAgainVisitBtn) then
			SetFalse( m_uiTenAgainVisitBtn );
		end
		-- 播放多个道具或武将的动画
		-- 原理同上，只不过这里循环10个动画显示
	end
end
--设置奖励道具和武将动画
function HeroVisitDlgSetAnimation( value )
		-- 奖励武将
	if value.kind >= AWARDKIND_HEROBASE then
		-- 武将配置信息
		local pHeroConfig = g_heroinfo[value.kind-AWARDKIND_HEROBASE][value.color]
		if pHeroConfig == nil then
			return;
		end
		local hero = { m_kind = value.kind-AWARDKIND_HEROBASE, m_color = value.color, m_level = 1, m_corps = pHeroConfig.corps, m_exp=0,m_exp_max=0,m_soldiers=0,m_state=0,
			m_attack_base = pHeroConfig.attack_base, m_attack_wash = pHeroConfig.attack_wash,
			m_defense_base = pHeroConfig.defense_base, m_defense_wash = pHeroConfig.defense_wash,
			m_troops_base = pHeroConfig.troops_base, m_troops_wash = pHeroConfig.troops_wash,
			m_attack = pHeroConfig.attack,m_defense = pHeroConfig.defense,m_troops = pHeroConfig.troops, m_offset = 0 ,m_itemnum = pHeroConfig.itemnum }
		HeroGetDlgShow( hero );		
	-- 奖励道具
	else
		ItemGetDlgShow( value.kind, value.num )
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
	t_getHero = {};
	if controlid == 2 then
		SetText(m_uiOneUpText,T(1962));
		 i_TimesNum = global.hero_visit_low_max - i_goodTimesNum ;
		 SetText(m_uiTimesText,F(1963,i_TimesNum));
		i_itemTimes = GetItem():GetCount(484) ;
		--[[
		if i_controlID == 2 then
			SetTrue(m_uiAwardLayer);
			SetFalse(m_uiInfoLayer);
			HeroVisitDlgDownBtn();
			return;
		end	
		--]]
		i_controlID = 2;
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
	elseif controlid == 3 then
		SetText(m_uiOneUpText,T(1961));
		i_TimesNum = global.hero_visit_high_max - i_godTimesNum ;
		SetText(m_uiTimesText,F(1964,i_TimesNum));
		i_itemTimes = GetItem():GetCount(485) ;
		--[[
		if i_controlID == 3 then
			SetTrue(m_uiAwardLayer);
			SetFalse(m_uiInfoLayer);			
			HeroVisitDlgDownBtn();
			return;
		end	
		--]]
		i_controlID = 3;
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
	end	
	HeroVisitDlgDownBtn();
	SetTrue(m_uiAwardLayer);
	SetFalse(m_uiInfoLayer);	
end
--设置奖励页面下排按钮显示
function HeroVisitDlgDownBtn()
	if i_freeTimes > 0 or i_itemTimes > 0 then
		SetFalse(m_uiOnceBtn);
		SetFalse(m_uiTenthBtn);
		SetTrue( m_uiFreeItemBtnGroup );
		if i_freeTimes > 0 then
			SetText(m_uiFreeItemText,T(1971));
		else
			if i_controlID == 2 then
				SetText(m_uiFreeItemText,F(1969,global.hero_visit_low_itemnum ,i_itemTimes));
			elseif i_controlID == 3 then
				
				SetText(m_uiFreeItemText,F(1970,global.hero_visit_high_itemnum,i_itemTimes));
			end
		end
	else
		SetTrue(m_uiOnceBtn);
		SetTrue(m_uiTenthBtn);
		SetFalse( m_uiFreeItemBtnGroup );
		if i_controlID == 2 then
			SetText(m_uiOnceText,global.hero_visit_low_token);	
			SetText(m_uiTenthText,global.hero_visit_low_token10);		
		elseif i_controlID == 3 then
			SetText(m_uiOnceText,global.hero_visit_high_token);
			SetText(m_uiTenthText,global.hero_visit_high_token10);
		end
	end
end

--排序
function HeroVisitDlgSort(a,b)
	return 	tonumber(a.transform.name) < tonumber(b.transform.name)
end
--获得道具和英雄的GameObject
function HeroVisitDlgGetObject()
	m_combineObjectArray ={};
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

--单次寻访设置单个对象
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
			HeroVisitDlgAddHero(value.kind);
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
--单次寻访开始前判断灰化哪种按钮
function HeroVisitDlgVisitBefore()
	if i_freeTimes > 0 or i_itemTimes > 0 then 
		SetButtonFalse(m_uiFreeItemBtn);
	else
		SetButtonFalse(m_uiOnceBtn);
		SetButtonFalse(m_uiTenthBtn);
	end
end
--单次寻访结束后执行的一系列方法
function HeroVisitDlgVisitLater()
	if i_freeTimes > 0 or i_itemTimes > 0 then 
		SetButtonTrue(m_uiFreeItemBtn);
		if	i_freeTimes > 0 then
			i_freeTimes = 0;
		elseif i_itemTimes > 0 then
			i_itemTimes = i_itemTimes -1;
		end
	else
		SetButtonTrue(m_uiOnceBtn);
		SetButtonTrue(m_uiTenthBtn);
	end
	HeroVisitDlgDownBtn();
end

-- 十连寻访设置单个对象
function HeroVisitDlgSetCellTen(value)
	
	--print("number:"..i_tweenId);
	--print("value"..value.kind);
	local uiItemObj = newobject(m_uiAward);
	SetTrue(uiItemObj.transform);
	uiItemObj.transform:SetParent( m_uiAwardGrop.transform,false );
	local sprite, color, name = AwardInfo(value.kind  ) ;
	local colorSprite = ItemColorSprite(value.color);
	local objs = uiItemObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	if value.kind >= 20000 then
		local uiShape = objs[0];
		local uiColor = objs[1];
		local uiCorps = objs[2];
		local uiName = objs[3];
		SetImage(uiShape,sprite);
		SetImage(uiColor,colorSprite);
		SetImage(uiCorps,CorpsSprite(g_heroinfo[(value.kind-20000)][value.color].corps));
		--local haveHero = GetHero():GetPtr(value.kind-20000);
		uiItemObj.transform.localPosition = f_awardPosition[i_tweenId];
		local b_haveHero = HeroVisitDlgIsHaveHero(value.kind);
		if b_haveHero == true  then
			SetText(uiName,"将令x"..g_heroinfo[(value.kind-20000)][value.color].itemnum);
		else
			SetText(uiName,name);
			local transformId =HeroVisitDlgTransform(value.id)
			SetTrue(m_combineObjectArray[transformId].transform:Find("AlreadlyHave"));
		end
		b_isTenVisit = false;
		HeroVisitDlgSetAnimation(value);
	else
		uiItemObj.transform:GetComponent("UITweenLocalPosition").to = f_awardPosition[i_tweenId];
		uiItemObj.transform:GetComponent("UITweenLocalPosition"):Play(true);
		uiItemObj.transform:GetComponent("UITweenLocalRotation"):Play(true);
		local uiShape = objs[0];
		local uiColor = objs[1];
		local uiCorps = objs[2];
		local uiName = objs[3];	
		SetFalse(uiCorps);
		SetImage(uiShape,sprite);
		SetImage(uiColor,colorSprite);
		SetText(uiName,"将令x"..value.num);	
	end
	i_tweenId =i_tweenId+1;	
	if i_tweenId ==11 then
		b_isTenVisit = false;
		SetTrue( m_uiTenAgainVisitBtn );
	end
end
--点击英雄头像
function HeroVisitDlgSelect( colickid )
	local kindid = g_hero_visit[colickid].kind ;
	if kindid >= 20000 then
		if i_controlID	== 2 then
			HeroConfigDlgShow( g_heroinfo[kindid-20000][3], 0 );
		else
			HeroConfigDlgShow( g_heroinfo[kindid-20000][2], 0 );	
		end				
	end	
end


-- 通过id找出寻访物品位置
function HeroVisitDlgTransform( id )
	local transformid  = 0;
	local selectid = id;
	if (id > 100 and id < 200) or ( id > 300 and id < 400) then
		selectid = objCache[id]
	end
	for i = 1 ,#m_combineObjectArray do
		if selectid == m_combineObjectArray[i].transform:GetComponent("UIButton").controlID then
			transformid = i
		end
	end
	return transformid ;
end
-- 获取十连寻访每个将领物体的位置
function HeroVisitDlgGetPosition()
	for i = m_uiAwardGrop.transform.childCount-1,0,-1 do
		f_awardPosition[i+1]=m_uiAwardGrop.transform:GetChild(i).transform.localPosition ;		
	end
	clearChild(m_uiAwardGrop);
end
-- 获得新武将界面关闭时调用此方法
function HeroVisitDlgCloseHeroGetEvent()
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	if IsActive(m_uiTenPage) then
		if i_tweenId  >=11 then
			return;
		else
			b_isTenVisit = true;
			HeroVisitDlgSetCellTen(g_hero_visit[m_cacheAward.m_id[i_tweenId]]);
		end
	end
end
--判段是否已经拥有此英雄
function HeroVisitDlgIsHaveHero( ikind )
	for key, value in ipairs(t_getHero) do  
		if value == ikind then
			return true;
		end
	end 
	return false ;
end
--存储英雄
function HeroVisitDlgAddHero( kind )
	table.insert(t_getHero,kind);
end







