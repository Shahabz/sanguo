GIRL_MAX_COLOR	=	5	-- 女将最高突破品质

-- 界面
local m_Dlg = nil;
local m_DialogFrameMod = nil;
local m_uiHeadLayer = nil; --UnityEngine.GameObject
local m_uiGirlPageL = nil; --UnityEngine.GameObject
local m_uiGirlPageR = nil; --UnityEngine.GameObject
local m_uiGirlScroll = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiUIP_Girl = nil; --UnityEngine.GameObject
local m_uiSingleLayer = nil; --UnityEngine.GameObject
local m_uiGirlName = nil; --UnityEngine.GameObject
local m_uiGirlShape = nil; --UnityEngine.GameObject
local m_uiAttrList = nil; --UnityEngine.GameObject
local m_uiPrivateAttrList = nil; --UnityEngine.GameObject
local m_uiAllotBtn = nil; --UnityEngine.GameObject
local m_uiDoubleLayer = nil; --UnityEngine.GameObject
local m_uiHero = nil; --UnityEngine.GameObject
local m_uiGirl = nil; --UnityEngine.GameObject
local m_uiColorUpBtn = nil; --UnityEngine.GameObject
local m_uiLoveLayer = nil; --UnityEngine.GameObject
local m_uiLoveStar = nil; --UnityEngine.GameObject
local m_uiLoveSpeed = nil; --UnityEngine.GameObject
local m_uiLoveProgress = nil; --UnityEngine.GameObject
local m_uiMarryBtn = nil; --UnityEngine.GameObject
local m_uiMakeLoveBtn = nil; --UnityEngine.GameObject
local m_uiNextBtn = nil; --UnityEngine.GameObject
local m_uiSonLayer = nil; --UnityEngine.GameObject
local m_uiSonHead = nil; --UnityEngine.GameObject
local m_uiSonName = nil; --UnityEngine.GameObject
local m_uiSonAttr = nil; --UnityEngine.GameObject
local m_uiGrowUpBtn = nil; --UnityEngine.GameObject
local m_uiAgeProgress = nil; --UnityEngine.GameObject
local m_uiIntimate = nil; --UnityEngine.GameObject
local m_uiDontMarry = nil; --UnityEngine.GameObject
local m_uiMakeLoveAlready = nil; --UnityEngine.GameObject
local m_uiRelieveBtn = nil; --UnityEngine.GameObject
local m_ObjectPool = nil;
local m_GirlCache = nil;
local m_SelectGirl = nil;
local m_star = nil;
local m_cacheValue = nil;

-- 打开界面
function GirlDlgOpen()
	ResourceManager.LoadAssetBundle( "_ab_girlhead" );
	ResourceManager.LoadAssetBundle( "_ab_girlface" );
	m_Dlg = eye.uiManager:Open( "GirlDlg" );
	m_DialogFrameMod = DialogFrameModOpen( m_Dlg, T(565), HELP_GirlDlg, GirlDlgClose );
end

-- 隐藏界面
function GirlDlgClose()
	if m_Dlg == nil then
		return;
	end
	DialogFrameModClose( m_DialogFrameMod );
	m_DialogFrameMod = nil;
	eye.uiManager:Close( "GirlDlg" );
	m_GirlCache = nil
	m_SelectGirl = nil
end

-- 删除界面
function GirlDlgDestroy()
	GameObject.Destroy( m_Dlg );
	ResourceManager.UnloadAssetBundle( "_ab_girlhead" )
	ResourceManager.UnloadAssetBundle( "_ab_girlface" )
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function GirlDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            GirlDlgClose();
		
		-- 委派男将
		elseif nControlID == 1 then
			GirlDlgAllot()
			
		-- 突破
		elseif nControlID == 2 then
			GirlDlgColorUp()
			
		-- 增加亲昵度
		elseif nControlID == 3 then
			GirlDlgLoveItemUse()
		
		-- 喜结连理
		elseif nControlID == 4 then
			GirlDlgMarry()
		
		-- 亲密互动
		elseif nControlID == 5 then
			GirlDlgMakeLove()
		
		-- 下一步
		elseif nControlID == 6 then
			GirlDlgMakeNext()
		
		-- 出师
		elseif nControlID == 7 then
			GirlDlgSonGrowUp()
		
		-- 解除委派
		elseif nControlID == 8 then
			GirlDlgRelieve();
		
		-- 解除委派按钮显示
		elseif nControlID == 9 then
			if IsActive(m_uiRelieveBtn) == true then
				SetFalse(m_uiRelieveBtn);
			else
				SetTrue(m_uiRelieveBtn);
			end
		
		-- 选择女将		
		elseif nControlID > 1000 and nControlID < 2000 then
			GirlDlgSelect( nControlID - 1000 )
        end
	end
end

-- 载入时调用
function GirlDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiHeadLayer = objs[0];
	m_uiGirlPageL = objs[1];
	m_uiGirlPageR = objs[2];
	m_uiGirlScroll = objs[3];
	m_uiContent = objs[4];
	m_uiUIP_Girl = objs[5];
	m_uiSingleLayer = objs[6];
	m_uiGirlName = objs[7];
	m_uiGirlShape = objs[8];
	m_uiAttrList = objs[9];
	m_uiPrivateAttrList = objs[10];
	m_uiAllotBtn = objs[11];
	m_uiDoubleLayer = objs[12];
	m_uiHero = objs[13];
	m_uiGirl = objs[14];
	m_uiColorUpBtn = objs[15];
	m_uiLoveLayer = objs[16];
	m_uiLoveStar = objs[17];
	m_uiLoveSpeed = objs[18];
	m_uiLoveProgress = objs[19];
	m_uiMarryBtn = objs[20];
	m_uiMakeLoveBtn = objs[21];
	m_uiNextBtn = objs[22];
	m_uiSonLayer = objs[23];
	m_uiSonHead = objs[24];
	m_uiSonName = objs[25];
	m_uiSonAttr = objs[26];
	m_uiGrowUpBtn = objs[27];
	m_uiAgeProgress = objs[28];
	m_uiIntimate = objs[29];
	m_uiDontMarry = objs[30];
	m_uiMakeLoveAlready = objs[31];
	m_uiRelieveBtn = objs[32];
	-- 对象池
	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );
	m_ObjectPool:CreatePool("UIP_Girl", 64, 64, m_uiUIP_Girl);
end

-- 界面初始化时调用
function GirlDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function GirlDlgOnEnable( gameObject )
	m_cacheValue = true;
end

-- 界面隐藏时调用
function GirlDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function GirlDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function GirlDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function GirlDlgShow(kind)
	GirlDlgOpen()
	GirlDlgHeadLayerCreate()
	if kind == nil then
		GirlDlgSelect( 0 )
	else
		GirlDlgSelect(kind);
	end
end

function GirlDlgShowByKind( kind )
	GirlDlgOpen()
	GirlDlgHeadLayerCreate()
	GirlDlgSelect( kind )
end

-- 整体刷新
function GirlDlgShowAllot()
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	GirlDlgHeadLayerCreate()
	GirlDlgSelect( m_SelectGirl.m_kind )
end

-- 刷新选择的女将
function GirlDlgUpdate( kind )
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	
	local actorGirl = GetGirl().m_Girl[kind];
	if actorGirl and actorGirl.m_color > 0 then
		-- 有关联武将，检查这个武将是否是上阵状态
		local isup = 0;
		if actorGirl.m_herokind > 0 then
			--if GetHero():GetPtrWithCity( actorGirl.m_herokind ) ~= nil then
				isup = 1
			--end
		end
		m_SelectGirl.m_color = actorGirl.m_color
		m_SelectGirl.m_state = isup
	else
		m_SelectGirl.m_color = g_girlinfo[kind][0].init_color
		m_SelectGirl.m_state = -1
	end
	GirlDlgHeadLayerSetObj( m_SelectGirl.m_uiObj, m_SelectGirl )
	GirlDlgSelect( m_SelectGirl.m_kind )
end

-- 创建女将列表
function GirlDlgHeadLayerCreate()
	GirlDlgHeadLayerClear()
	m_GirlCache = {}
	local m_GirlCantBreak = {}
	local m_GirlNoGet = {}
	-- 遍历所有女将
	for k, v in pairs(g_girlinfo) do
		local kind = k
		local actorGirl = GetGirl().m_Girl[kind];
		if actorGirl and actorGirl.m_color > 0 then
			local isup = 0;
			-- 有关联武将，检查这个武将是否是上阵状态
			if actorGirl.m_herokind > 0 then
				--if GetHero():GetPtrWithCity( actorGirl.m_herokind ) ~= nil then
					isup = 1
				--end
			end
			-- 已经获得这个女将，使用存档
			if actorGirl.m_soul > v[actorGirl.m_color].soul then
				table.insert( m_GirlCache, {m_kind=kind, m_color=actorGirl.m_color, m_state=isup} )
			else
				table.insert( m_GirlCantBreak, {m_kind=kind, m_color=actorGirl.m_color, m_state=isup} )
			end
		else
			-- 未获得这个女将，使用配置
			table.insert( m_GirlNoGet, {m_kind=kind, m_color=v[0].init_color, m_state=-1 } )
		end
	end
	-- 不可突破排序
	table.sort( m_GirlCantBreak, function(a,b) 
		if a.m_state > b.m_state then
			return true;
		elseif a.m_state == b.m_state then
			if a.m_color > b.m_color then 
				return true
			elseif a.m_color == b.m_color then
				if a.m_kind < b.m_kind then 
					return true
				end
			end
		end
		return false
	end );
	
	-- 可突破排序
	table.sort( m_GirlCache, function(a,b) 
		if a.m_state > b.m_state then
			return true;
		elseif a.m_state == b.m_state then
			if a.m_color > b.m_color then 
				return true
			elseif a.m_color == b.m_color then
				if a.m_kind < b.m_kind then 
					return true
				end
			end
		end
		return false
	end );
	
	for i,v in pairs(m_GirlCantBreak) do
		table.insert(m_GirlCache,v)
	end
	
	for i,v in pairs(m_GirlNoGet) do
		table.insert(m_GirlCache,v)
	end
	
	-- 创建
	for i=1, #m_GirlCache, 1 do
		local uiGirlObj = m_ObjectPool:Get( "UIP_Girl" );
		uiGirlObj.transform:SetParent( m_uiContent.transform );	
		m_GirlCache[i].m_uiObj = uiGirlObj;
		GirlDlgHeadLayerSetObj( uiGirlObj, m_GirlCache[i] )
	end
end

-- 设置一个头像对象
function GirlDlgHeadLayerSetObj( uiGirlObj, pGirl )
	local objs = uiGirlObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiShape = objs[0]
	local uiColor = objs[1]
	local uiState1 = objs[2]
	local uiState2 = objs[3]
	local uiState3 = objs[4]
	local uiName = objs[5]
	local uiSelect = objs[6]
	uiGirlObj:GetComponent("UIButton").controlID = 1000 + pGirl.m_kind;
	SetImage( uiShape, GirlHeadSprite(pGirl.m_kind) )
	SetImage( uiColor, ItemColorSprite(pGirl.m_color) )
	SetText( uiName, GirlName(pGirl.m_kind) )
	if pGirl.m_state == -1 then
		SetFalse( uiState1 )
		SetFalse( uiState2 )
		SetTrue( uiState3 )
	elseif pGirl.m_state == 0 then
		SetFalse( uiState1 )
		SetTrue( uiState2 )
		SetFalse( uiState3 )
	elseif pGirl.m_state == 1 then
		SetTrue( uiState1 )
		SetFalse( uiState2 )
		SetFalse( uiState3 )
	end
	SetFalse( uiSelect )
end

-- 清空
function GirlDlgHeadLayerClear()
	local objs = {};
	for i = 0 ,m_uiContent.transform.childCount - 1 do
		table.insert( objs, m_uiContent.transform:GetChild(i).gameObject )
    end
	for k, v in pairs(objs) do
		local obj = v;
		m_ObjectPool:Release( "UIP_Girl", obj );
    end
end

-- 选择女将
function GirlDlgSelect( kind )
	if m_SelectGirl then
		local objs = m_SelectGirl.m_uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
		local uiSelect = objs[6]
		SetFalse( uiSelect )
	end
	
	if kind == 0 then
		local objs = m_GirlCache[1].m_uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
		local uiSelect = objs[6]
		SetTrue( uiSelect )
		m_SelectGirl = m_GirlCache[1];
	else
		for i=1, #m_GirlCache, 1 do
			if kind == m_GirlCache[i].m_kind then
				local objs = m_GirlCache[i].m_uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
				local uiSelect = objs[6]
				SetTrue( uiSelect )
				if m_SelectGirl ~= m_GirlCache[i] then
					m_cacheValue = true;
					SetFalse(m_uiRelieveBtn);
				end
				m_SelectGirl = m_GirlCache[i];
				SetItemCenterInScroll(i - 1,m_uiGirlScroll.transform);
				break
			end
		end
	end
	GirlDlgCreateInfo( m_SelectGirl )
end

-- 创建详细信息
function GirlDlgCreateInfo( pGirl )
	local kind = pGirl.m_kind
	local color = pGirl.m_color
	local config = girlconfig( kind, color )
	if config == nil then
		return
	end
	
	local actorGirl = GetGirl().m_Girl[kind];
	if actorGirl and actorGirl.m_color > 0 and actorGirl.m_herokind > 0 then
		-- 已经获得并且委派了
		SetFalse( m_uiSingleLayer )
		SetTrue( m_uiDoubleLayer )
		GirlDlgDoubleLayerSetHero( m_uiHero, config, actorGirl.m_herokind );
		GirlDlgDoubleLayerSetGirl( m_uiGirl, kind, color );
		
		-- 碎片进度
		local uiProgress = m_uiDoubleLayer.transform:Find("SoulProgress")
		local nowSoul = actorGirl.m_soul;
		local maxSoul = config.soul
		SetProgress( uiProgress, nowSoul/maxSoul )
		SetText( uiProgress.transform:Find("Text"), nowSoul.."/"..maxSoul )
		
		local pHero = GetHero():GetPtr( actorGirl.m_herokind );
		if pHero == nil then
			return
		end
		--有孩子
		if pHero.m_sontime > 0 then
			SetTrue(m_uiSonLayer);
			SetFalse(m_uiLoveLayer);
			GirlDlgSonLayerSetSon(pHero,g_girlson[pHero.m_kind][pHero.m_sonnum]);
			
		--没有孩子
		else
			local m_girlConifg = girlconfig(m_SelectGirl.m_kind,m_SelectGirl.m_color);
			if m_girlConifg ~= nil then
				GirlDlgShowStar(actorGirl.m_herokind,m_girlConifg);
				SetText(m_uiLoveSpeed,T(3357)..(100 * GirlDlgLoveUpSpeed()).."%");
			end
			
			-- 亲密度
			SetProgress( m_uiLoveProgress, actorGirl.m_love_exp/g_girllove[actorGirl.m_love_level].exp ); 
			SetText( m_uiLoveProgress.transform:Find("Text"), actorGirl.m_love_exp.."/"..g_girllove[actorGirl.m_love_level].exp ) 
			SetLevel( m_uiLoveProgress.transform:Find("Title/LoveLevel"),actorGirl.m_love_level);
			SetTrue(m_uiLoveLayer);
			if m_cacheValue == true then
				SetFalse(m_uiSonLayer);
				SetFalse(m_uiDontMarry);
				SetFalse(m_uiIntimate);
				SetFalse(m_uiMarryBtn);
				SetFalse(m_uiMakeLoveBtn);
				SetFalse(m_uiMakeLoveAlready);
				
				-- 还不能喜结连理
				if actorGirl.m_love_level < global.girl_marry_lovelevel then 
					SetTrue(m_uiDontMarry);
					SetText(m_uiDontMarry.transform:Find("Text"),T(3352));
				else
					
					-- 可以喜结连理
					if Utils.get_int_sflag( actorGirl.m_sflag, GIRL_SFLAG_MARRY ) == 0 then
						SetTrue(m_uiMarryBtn);
						
					-- 已经喜结连理
					else
						--未亲密互动
						if Utils.get_int_sflag( actorGirl.m_sflag, GIRL_SFLAG_MAKELOVE) == 0 then
							SetTrue(m_uiMakeLoveBtn);
							
						--已经亲密互动
						else
							SetTrue(m_uiMakeLoveAlready);
							SetText(m_uiMakeLoveAlready.transform:Find("Text"),T(3353));
						end
					end
				end
			end
		end
	else
		-- 未获得 或者 未指派
		SetFalse(m_uiSonLayer);
		SetFalse(m_uiLoveLayer);
		SetTrue( m_uiSingleLayer )
		SetFalse( m_uiDoubleLayer )
		SetText( m_uiGirlName, GirlName( kind ) )
		SetImage( m_uiGirlShape, GirlFaceSprite( kind ) )
		
		-- 属性
		-- 强攻
		local uiAttackIncrease = m_uiAttrList.transform:GetChild(1)
		if config.attack_increase > 0 then
			SetText(uiAttackIncrease.transform:Find("Text"), T(165).."+"..config.attack_increase)
			SetTrue(uiAttackIncrease)
		else
			SetFalse(uiAttackIncrease)
		end
		-- 强防
		local uiDefenseIncrease = m_uiAttrList.transform:GetChild(2)
		if config.defense_increase > 0 then
			SetText(uiDefenseIncrease.transform:Find("Text"), T(166).."+"..config.defense_increase)
			SetTrue(uiDefenseIncrease)
		else
			SetFalse(uiDefenseIncrease)
		end
		-- 攻击资质
		local uiAttackGrowth = m_uiAttrList.transform:GetChild(3)
		if config.attack_growth > 0 then
			SetText(uiAttackGrowth.transform:Find("Text"), F(3336,config.attack_growth))
			SetTrue(uiAttackGrowth)
		else
			SetFalse(uiAttackGrowth)
		end
		-- 防御资质
		local uiDefenseGrowth = m_uiAttrList.transform:GetChild(4)
		if config.defense_growth > 0 then
			SetText(uiDefenseGrowth.transform:Find("Text"), F(3337,config.defense_growth))
			SetTrue(uiDefenseGrowth)
		else
			SetFalse(uiDefenseGrowth)
		end
		
		-- 特殊属性
		if config.private_herokind > 0 then
			SetTrue( m_uiPrivateAttrList )
			local privateAttrHero = m_uiPrivateAttrList.transform:GetChild(0)
			SetText(privateAttrHero.transform:Find("Text"), F(3362, HeroName(config.private_herokind)))
			-- 强攻
			uiAttackIncrease = m_uiPrivateAttrList.transform:GetChild(1)
			if config.private_attack_increase > 0 then
				SetText(uiAttackIncrease.transform:Find("Text"), T(165).."+"..config.private_attack_increase)
				SetTrue(uiAttackIncrease)
			else
				SetFalse(uiAttackIncrease)
			end
			-- 强防
			uiDefenseIncrease = m_uiPrivateAttrList.transform:GetChild(2)
			if config.private_defense_increase > 0 then
				SetText(uiDefenseIncrease.transform:Find("Text"), T(166).."+"..config.private_defense_increase)
				SetTrue(uiDefenseIncrease)
			else
				SetFalse(uiDefenseIncrease)
			end
			-- 攻击资质
			uiAttackGrowth = m_uiPrivateAttrList.transform:GetChild(3)
			if config.private_attack_growth > 0 then
				SetText(uiAttackGrowth.transform:Find("Text"), F(3336,config.private_attack_growth))
				SetTrue(uiAttackGrowth)
			else
				SetFalse(uiAttackGrowth)
			end
			-- 防御资质
			uiDefenseGrowth = m_uiPrivateAttrList.transform:GetChild(4)
			if config.private_defense_growth > 0 then
				SetText(uiDefenseGrowth.transform:Find("Text"), F(3337,config.private_defense_growth))
				SetTrue(uiDefenseGrowth)
			else
				SetFalse(uiDefenseGrowth)
			end
		else
			SetFalse( m_uiPrivateAttrList )
		end
		
		-- 碎片进度
		local uiProgress = m_uiSingleLayer.transform:Find("SoulProgress")
		local nowSoul = actorGirl.m_soul;
		local maxSoul = 0;	
		-- 未指派
		if actorGirl and actorGirl.m_color > 0 then
			SetTrue( m_uiAllotBtn )
			maxSoul = config.soul
			SetText(m_uiSingleLayer.transform:Find("Warning"), T(3346));
		-- 未获得
		else
			SetFalse( m_uiAllotBtn )
			maxSoul = girlconfig( kind, 0 ).soul
			if Utils.get_int_sflag( GetPlayer().m_function, CITY_FUNCTION_FANGSHI ) == 1 then
				SetText(m_uiSingleLayer.transform:Find("Warning"), F(3347,maxSoul,GirlName(kind)));
			else
				SetText(m_uiSingleLayer.transform:Find("Warning"), F(3347,maxSoul,GirlName(kind)).."\n"..T(3365));
			end
		end
		SetProgress( uiProgress, nowSoul/maxSoul )
		SetText( uiProgress.transform:Find("Text"), nowSoul.."/"..maxSoul )
		SetText(m_uiSingleLayer.transform:Find("Desc"), GirlDesc( kind ));
	end
end

-- 拼属性
local function MakeAttrText( uiAttrText, config )
	local msg = "";
	if config  then
		if config.private_herokind > 0 then
			if config.attack_increase > 0 then
				msg = msg.."<color=#FFDE00FF>"..T(165).."+"..(config.attack_increase+config.private_attack_increase).."</color>\n"
			end
			if config.defense_increase > 0 then
				msg = msg.."<color=#25C9FFFF>"..T(166).."+"..(config.defense_increase+config.private_defense_increase).."</color>\n"
			end
			if config.attack_growth > 0 then
				msg = msg.."<color=#FFDE00FF>"..F(3336,(config.attack_growth+config.private_attack_growth)).."</color>\n"
			end
			if config.defense_growth > 0 then
				msg = msg.."<color=#25C9FFFF>"..F(3337,(config.defense_growth+config.defense_growth)).."</color>\n"
			end
		else
			if config.attack_increase > 0 then
				msg = msg.."<color=#FFDE00FF>"..T(165).."+"..config.attack_increase.."</color>\n"
			end
			if config.defense_increase > 0 then
				msg = msg.."<color=#25C9FFFF>"..T(166).."+"..config.defense_increase.."</color>\n"
			end
			if config.attack_growth > 0 then
				msg = msg.."<color=#FFDE00FF>"..F(3336,config.attack_growth).."</color>\n"
			end
			if config.defense_growth > 0 then
				msg = msg.."<color=#25C9FFFF>"..F(3337,config.defense_growth).."</color>\n"
			end
		end
	end
	SetText( uiAttrText, msg )
end

-- 拼儿子属性
local function MakeSonAttrText( uiAttrText, config ,pHero)
	local msg = "";
	if config  then
		if config.attack > 0 then
			msg = msg..F(3406,HeroName(pHero.m_kind),config.attack,T(146)).."\n";
		end
		if config.defense > 0 then
			msg = msg..F(3406,HeroName(pHero.m_kind),config.defense,T(147)).."\n";
		end
		if config.attack_increase > 0 then
			msg = msg..F(3406,HeroName(pHero.m_kind),config.attack_increase,T(165)).."\n";
		end
		if config.defense_increase > 0 then
			msg = msg..F(3406,HeroName(pHero.m_kind),config.defense_increase,T(166)).."\n";
		end
	end
	SetText( uiAttrText, msg..T(3407))
end

-- 设置男将信息
function GirlDlgDoubleLayerSetHero( uiHeroObj, config, herokind )
	local objs = uiHeroObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiColor = objs[0];
	local uiName = objs[1];
	local uiShape = objs[2];
	local uiAttrText = objs[3];
	local pHero = GetHero():GetPtr( herokind )
	if pHero == nil then
		return
	end
	SetImage( uiColor, ItemColorSprite( pHero.m_color ) )
	SetText( uiName, HeroName( herokind ) )
	SetImage( uiShape, HeroFaceSprite( herokind ) )
	MakeAttrText( uiAttrText, config )
	
end

-- 设置女将信息
function GirlDlgDoubleLayerSetGirl( uiGirlObj, kind, color )
	local objs = uiGirlObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiColor = objs[0];
	local uiName = objs[1];
	local uiShape = objs[2];
	local uiAttrName = objs[3];
	local uiAttrText = objs[4];
	SetImage( uiColor, ItemColorSprite( color ) )
	SetText( uiName, GirlName( kind ) )
	SetImage( uiShape, GirlFaceSprite( kind ) )
	if color >= ITEM_COLOR_LEVEL_RED then
		SetText( uiAttrName, T(3350) ) -- 突破已满
		SetText( uiAttrText, "" )
	else
		SetText( uiAttrName, T(3349) ) -- 下次突破加成属性
		MakeAttrText( uiAttrText, girlconfig( kind, color+1 ) )
	end
end

-- 设置儿子信息
function GirlDlgSonLayerSetSon(pHero,sonConifg)
	SetImage(m_uiSonHead,SonHeadSprite(sonConifg.client_shape));
	SetText(m_uiSonName,SonName(sonConifg.client_name));
	MakeSonAttrText(m_uiSonAttr,sonConifg,pHero);
	
	-- 儿子的出师进度
	if pHero.m_sontime - GetServerTime() <= 0 then
		SetTrue(m_uiGrowUpBtn);
		SetProgress(m_uiAgeProgress, 1); 
		SetText(m_uiAgeProgress.transform:Find("Text"), F(3408,18));
	else
		SetFalse(m_uiGrowUpBtn);
		local age,progress = math.modf((GetServerTime() - (pHero.m_sontime - sonConifg.grow_sec))/(sonConifg.grow_sec/18));
		SetProgress(m_uiAgeProgress, progress); 
		SetText(m_uiAgeProgress.transform:Find("Text"), F(3408,age));
	end
end

-- 委派男将
function GirlDlgAllot()
	if m_SelectGirl == nil then
		return
	end
	local kind = m_SelectGirl.m_kind
	local color = m_SelectGirl.m_color
	GirlSelectHeroDlgShow(kind,color);
end

-- 解除委派
function GirlDlgRelieve()
	if m_SelectGirl == nil then
		return
	end
	MsgBox(T(3403),function() 
		system_askinfo( ASKINFO_GIRL, "", 2, m_SelectGirl.m_kind)
	end);
end

-- 突破
function GirlDlgColorUp()
	if m_SelectGirl == nil then
		return
	end
	
	if m_SelectGirl.m_color == GIRL_MAX_COLOR then
		pop(T(3350));
	else
		system_askinfo( ASKINFO_GIRL, "", 3, m_SelectGirl.m_kind )
	end
	
end
			
-- 增加亲昵度
function GirlDlgLoveItemUse()
	if m_SelectGirl == nil then
		return
	end
	local config = girlconfig( m_SelectGirl.m_kind, m_SelectGirl.m_color )
	if config == nil then
		return
	end	
	local itemlist = {}
	local desclist = {}
	if config.type == 0 then
		itemlist = { 224, 225, 226 }
		desclist = { F(3364,g_itemkind[224].base_value1), F(3364,g_itemkind[225].base_value1), F(3364,g_itemkind[226].base_value1) }
	elseif config.type == 1 then
		itemlist = { 227, 228, 229 }
		desclist = { F(3364,g_itemkind[227].base_value1), F(3364,g_itemkind[228].base_value1), F(3364,g_itemkind[229].base_value1) }
	elseif config.type == 2 then
		itemlist = { 221, 222, 223 }
		desclist = { F(3364,g_itemkind[221].base_value1), F(3364,g_itemkind[222].base_value1), F(3364,g_itemkind[223].base_value1) }
	end
	ItemUseDlgShow( itemlist, desclist, false, function( itemkind ) 
			system_askinfo( ASKINFO_GIRL, "", 4, m_SelectGirl.m_kind, itemkind );
	end )
end
		
-- 喜结连理
function GirlDlgMarry()
	if m_SelectGirl == nil then
		return
	end
	system_askinfo( ASKINFO_GIRL, "", 5, m_SelectGirl.m_kind )
end
		
-- 亲密互动
function GirlDlgMakeLove()
	if m_SelectGirl == nil then
		return
	end
	SetFalse(m_uiMakeLoveBtn);
	
	system_askinfo( ASKINFO_GIRL, "", 6, m_SelectGirl.m_kind )
	m_cacheValue = false;
end

-- 亲密互动结果
function GirlDlgMakeLoveResult( kind, born, makelove_exp )
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	if m_SelectGirl == nil then
		return
	end
	if m_SelectGirl.m_kind ~= kind then
		return
	end
	local value = 0;
	if born == 1 then
		-- 已生孩子
		value = math.random(3501,3506);
	else
		-- 未生孩子
		value = math.random(3507,3529);
	end
	
	local actorGirl = GetGirl().m_Girl[kind];
	local pHero = GetHero():GetPtr( actorGirl.m_herokind );
	if pHero == nil then
		return
	end
	SetText(m_uiIntimate.transform:Find("Desc"),F(value,HeroName(pHero.m_kind),GirlName(m_SelectGirl.m_kind),makelove_exp));
	
	SetTrue(m_uiIntimate);
end
		
-- 下一步
function GirlDlgMakeNext()
	if m_SelectGirl == nil then
		return
	end
	
	local actorGirl = GetGirl().m_Girl[m_SelectGirl.m_kind];
	local pHero = GetHero():GetPtr( actorGirl.m_herokind );
	if pHero == nil then
		return
	end
	--有孩子
	if pHero.m_sontime > 0 then
		SetFalse(m_uiLoveLayer);
		SetTrue(m_uiSonLayer);
		GirlDlgSonLayerSetSon(pHero,g_girlson[pHero.m_kind][pHero.m_sonnum]);
	else
		SetFalse(m_uiIntimate);
		SetTrue(m_uiMakeLoveAlready);
		SetText(m_uiMakeLoveAlready.transform:Find("Text"),T(3353));
	end
	m_cacheValue = true;
end
		
-- 出师
function GirlDlgSonGrowUp()
	if m_SelectGirl == nil then
		return
	end
	system_askinfo( ASKINFO_GIRL, "", 7, m_SelectGirl.m_kind )
end

-- 配对指数
function GirlDlgShowStar(herokind,girlConifg)
	if girlConifg.private_herokind == herokind then
		GirlDlgInitStar(girlConifg.love_star);
		local objs = m_uiLoveStar.transform:GetChild(m_uiLoveStar.transform.childCount - 1).gameObject
		SetTrue( objs );
		m_star = girlConifg.love_star + girlConifg.private_love_star;
	else
		GirlDlgInitStar(girlConifg.love_star);
		m_star = girlConifg.love_star;
	end
end

function GirlDlgInitStar(count)
	GirlDlgHideStar();
	for i = 0 ,count - 1 do
		local objs = m_uiLoveStar.transform:GetChild(i).gameObject
		SetTrue( objs );
    end 
end

function GirlDlgHideStar()
	for i = 0 ,m_uiLoveStar.transform.childCount - 1 do
		local objs = m_uiLoveStar.transform:GetChild(i).gameObject
		SetFalse( objs );
    end
end

-- 亲密度增长速率
function GirlDlgLoveUpSpeed()
	return 1 + (m_star - 1) * 0.5;
end
