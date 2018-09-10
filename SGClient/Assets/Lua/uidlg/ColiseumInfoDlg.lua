-- 界面
local m_Dlg = nil;
local m_uiShape = nil; --UnityEngine.GameObject
local m_uiRankLayer = nil; --UnityEngine.GameObject
local m_uiRankScroll = nil; --UnityEngine.GameObject
local m_uiRankContent = nil; --UnityEngine.GameObject
local m_uiUIP_Rank = nil; --UnityEngine.GameObject
local m_uiRecordLayer = nil; --UnityEngine.GameObject
local m_uiRecordScroll = nil; --UnityEngine.GameObject
local m_uiRecordContent = nil; --UnityEngine.GameObject
local m_uiUIP_Record = nil; --UnityEngine.GameObject
local m_uiRuleLayer = nil; --UnityEngine.GameObject
local m_uiRuleScroll = nil; --UnityEngine.GameObject
local m_uiRuleContent = nil; --UnityEngine.GameObject
local m_uiUIP_Rule = nil; --UnityEngine.GameObject
local m_uiWaiting = nil; --UnityEngine.GameObject
local m_uiUIP_RuleAward = nil; --UnityEngine.GameObject
local m_uiAwardDescLayer = nil; --UnityEngine.GameObject
local m_uiWindow = nil; --UnityEngine.GameObject

local m_ObjectPool = nil
local m_RankRecvValue = nil;
local m_RecordRecvValue = nil;
local m_RuleAwardRecvValue = nil;
-- 打开界面
function ColiseumInfoDlgOpen()
	m_Dlg = eye.uiManager:Open( "ColiseumInfoDlg" );
end

-- 隐藏界面
function ColiseumInfoDlgClose()
	if m_Dlg == nil then
		return;
	end

	eye.uiManager:Close( "ColiseumInfoDlg" );
	m_RankRecvValue = nil;
	m_RecordRecvValue = nil;
	m_RuleAwardRecvValue = nil;
end

-- 删除界面
function ColiseumInfoDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
	m_RankRecvValue = nil;
	m_RecordRecvValue = nil;
	m_RuleAwardRecvValue = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function ColiseumInfoDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            ColiseumInfoDlgClose();
		
		-- 排行榜	
		elseif nControlID == 1 then
			ColiseumInfoDlgShowRankLayer()
		
		-- 精彩对决	
		elseif nControlID == 2 then
			ColiseumInfoDlgShowRecordLayer()
		
		-- 规则	
		elseif nControlID == 3 then
			ColiseumInfoDlgShowRuleLayer()
		
		-- 观战
		elseif nControlID >= 10000 then
			ColiseumInfoDlgRecordView( nControlID-10000 )
        end
	elseif nType == UI_EVENT_PRESS then
		if nControlID > -2000000 and nControlID < -1000000 then
			ColiseumInfoDlgSelectAward( -(nControlID+1000000), value );
		end
	end
end

-- 载入时调用
function ColiseumInfoDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiShape = objs[0];
	m_uiRankLayer = objs[1];
	m_uiRankScroll = objs[2];
	m_uiRankContent = objs[3];
	m_uiUIP_Rank = objs[4];
	m_uiRecordLayer = objs[5];
	m_uiRecordScroll = objs[6];
	m_uiRecordContent = objs[7];
	m_uiUIP_Record = objs[8];
	m_uiRuleLayer = objs[9];
	m_uiRuleScroll = objs[10];
	m_uiRuleContent = objs[11];
	m_uiUIP_Rule = objs[12];
	m_uiWaiting = objs[13];
	m_uiUIP_RuleAward = objs[14];
	m_uiAwardDescLayer = objs[15];
	m_uiWindow = objs[16];
	
	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );
	m_ObjectPool:CreatePool("UIP_Rank", 30, 30, m_uiUIP_Rank);
	m_ObjectPool:CreatePool("UIP_Record", 5, 5, m_uiUIP_Record);
	m_ObjectPool:CreatePool("UIP_RuleAward", 10, 10, m_uiUIP_RuleAward);
end

-- 界面初始化时调用
function ColiseumInfoDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function ColiseumInfoDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function ColiseumInfoDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function ColiseumInfoDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function ColiseumInfoDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function ColiseumInfoDlgShow()
	ColiseumInfoDlgOpen()
	ColiseumInfoDlgShowRankLayer()
end

function ColiseumInfoDlgScale( scale )
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	m_Dlg.transform.localScale = Vector3.New(scale,scale,scale)
	if scale == 1 then
		SetFalse( m_uiWaiting )
	end
end

-- 排行榜	
function ColiseumInfoDlgShowRankLayer()
	SetTrue(m_uiRankLayer)
	SetFalse(m_uiRecordLayer)
	SetFalse(m_uiRuleLayer)
	if m_RankRecvValue == nil then
		SetTrue( m_uiWaiting )
		system_askinfo( ASKINFO_COLISEUM, "", 1 )
	else
		ColiseumInfoDlgRankRecv( m_RankRecvValue )
	end
end
-- 收到排行列表
function ColiseumInfoDlgRankRecv( recvValue )
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	ColiseumInfoDlgRankClear()
	SetFalse( m_uiWaiting )
	m_RankRecvValue = recvValue
	for i=1, recvValue.m_count, 1 do
		local info = recvValue.m_list[i]
		local uiObj = m_ObjectPool:Get( "UIP_Rank" );
		uiObj.transform:SetParent( m_uiRankContent.transform );
		uiObj.transform.localScale = Vector3.one;
		uiObj.gameObject:SetActive( true );
		ColiseumInfoDlgRankCreate( uiObj, info )
	end
end

-- 重置排名列表
function ColiseumInfoDlgRankClear()
	local objs = {};
	for i=0,m_uiRankContent.transform.childCount-1 do
		table.insert(objs,m_uiRankContent.transform:GetChild(i).gameObject);
	end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "UIP_Rank(Clone)" then
			m_ObjectPool:Release( "UIP_Rank", obj );
		end
	end
end

-- 创建排名
function ColiseumInfoDlgRankCreate( uiObj, info )
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiRank = objs[0]
	local uiRankImage = objs[1]
	local uiNation = objs[2]
	local uiName = objs[3]
	local uiLevel = objs[4]
	local uiPower = objs[5]
	
	if info.m_rank == 1 then
		SetTrue( uiRankImage )
		SetImage( uiRankImage, LoadSprite( "ui_icon_rank1" ) )
	elseif info.m_rank == 2 then
		SetTrue( uiRankImage )
		SetImage( uiRankImage, LoadSprite( "ui_icon_rank2" ) )
	elseif info.m_rank == 3 then
		SetTrue( uiRankImage )
		SetImage( uiRankImage, LoadSprite( "ui_icon_rank3" ) )
	else
		SetFalse( uiRankImage )
	end
	SetText( uiRank, info.m_rank )
	SetImage( uiNation, NationSprite(info.m_nation) );
	SetText( uiName, info.m_name );
	SetText( uiLevel, info.m_level )
	SetText( uiPower, info.m_bpower )
end

	
-- 精彩对决	
function ColiseumInfoDlgShowRecordLayer()
	SetFalse(m_uiRankLayer)
	SetTrue(m_uiRecordLayer)
	SetFalse(m_uiRuleLayer)
	if m_RecordRecvValue == nil then
		SetTrue( m_uiWaiting )
		system_askinfo( ASKINFO_COLISEUM, "", 7 )
	else
		ColiseumInfoDlgRecordRecv( m_RecordRecvValue )
	end
end

-- 收到战报列表
function ColiseumInfoDlgRecordRecv( recvValue )
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	ColiseumInfoDlgRecordClear()
	SetFalse( m_uiWaiting )
	m_RecordRecvValue = recvValue
	for i=1, recvValue.m_count, 1 do
		local info = recvValue.m_list[i]
		local uiObj = m_ObjectPool:Get( "UIP_Record" );
		uiObj.transform:SetParent( m_uiRecordContent.transform );
		uiObj.transform.localScale = Vector3.one;
		uiObj.gameObject:SetActive( true );
		ColiseumInfoDlgRecordCreate( uiObj, info )
	end
end

-- 重置战报列表
function ColiseumInfoDlgRecordClear()
	local objs = {};
	for i=0,m_uiRecordContent.transform.childCount-1 do
		table.insert(objs,m_uiRecordContent.transform:GetChild(i).gameObject);
	end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "UIP_Record(Clone)" then
			m_ObjectPool:Release( "UIP_Record", obj );
		end
	end
end

-- 创建战报
function ColiseumInfoDlgRecordCreate( uiObj, info )
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiANation = objs[0]
	local uiAName = objs[1]
	local uiAPower = objs[2]
	local uiARank = objs[3]
	local uiAHeroList = objs[4]
	local uiDNation = objs[5]
	local uiDName = objs[6]
	local uiDPower = objs[7]
	local uiDRank = objs[8]
	local uiDHeroList = objs[9]
	local uiViewBtn = objs[10]
	local uiOptime = objs[11]
	local uiALose = objs[12]
	local uiDLose = objs[13]
	
	SetControlID( uiViewBtn, 10000+info.m_fightid )
	SetText( uiOptime, os.date( "%m-%d %H:%M", info.m_optime ) )
	
	SetImage( uiANation, NationSprite(info.m_attack.m_nation) );
	SetImage( uiDNation, NationSprite(info.m_defense.m_nation) );
	
	SetText( uiAName, info.m_attack.m_name );
	SetText( uiDName, info.m_defense.m_name );
	
	SetText( uiAPower, F(4290,info.m_attack.m_bpower) );
	SetText( uiDPower, F(4290,info.m_defense.m_bpower) );
	
	SetText( uiARank, F(4291,info.m_attack.m_rank) );
	SetText( uiDRank, F(4291,info.m_defense.m_rank) );
	
	if info.m_win == 1 then
		SetFalse( uiALose )
		SetTrue( uiDLose )
	else
		SetTrue( uiALose )
		SetFalse( uiDLose )
	end
	
	for i=1, 3, 1 do
		local uiHeroObj = uiAHeroList.transform:GetChild(i-1);
		local heroinfo = info.m_attack.m_hero[i]
		ColiseumInfoDlgCreateRecordHero( uiHeroObj, heroinfo )
	end
	for i=1, 3, 1 do
		local uiHeroObj = uiDHeroList.transform:GetChild(i-1);
		local heroinfo = info.m_defense.m_hero[i]
		ColiseumInfoDlgCreateRecordHero( uiHeroObj, heroinfo )
	end	
end

-- 创建我的武将
function ColiseumInfoDlgCreateRecordHero( uiHeroObj, pHero )
	if pHero then
		SetTrue( uiHeroObj )
		SetImage( uiHeroObj.transform:Find("Color"), ItemColorSprite(pHero.m_color) );
		SetImage( uiHeroObj.transform:Find("Shape"), HeroHeadSprite(pHero.m_kind) );
		--SetText( uiHeroObj.transform:Find("Name"), HeroName(pHero.m_kind) );
	else
		SetFalse( uiHeroObj )
	end
end

-- 观战
function ColiseumInfoDlgRecordView( fightid )
	SetTrue( m_uiWaiting )
	system_askinfo( ASKINFO_COLISEUM, "", 9, fightid )
end


-- 规则	
function ColiseumInfoDlgShowRuleLayer()
	SetFalse(m_uiRankLayer)
	SetFalse(m_uiRecordLayer)
	SetTrue(m_uiRuleLayer)
	if m_RuleAwardRecvValue == nil then
		SetTrue( m_uiWaiting )
		system_askinfo( ASKINFO_COLISEUM, "", 10 )
	else
		ColiseumInfoDlgRuleAwardRecv( m_RuleAwardRecvValue )
	end
end

-- 收到奖励
function ColiseumInfoDlgRuleAwardRecv( recvValue )
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	ColiseumInfoDlgRuleAwardClear()
	SetFalse( m_uiWaiting )
	m_RuleAwardRecvValue = recvValue
	for i=1, recvValue.m_count, 1 do
		local info = recvValue.m_list[i]
		local uiObj = m_ObjectPool:Get( "UIP_RuleAward" );
		uiObj.transform:SetParent( m_uiRuleContent.transform );
		uiObj.transform.localScale = Vector3.one;
		uiObj.gameObject:SetActive( true );
		ColiseumInfoDlgRuleAwardCreate( uiObj, info )
	end
end

-- 重置奖励列表
function ColiseumInfoDlgRuleAwardClear()
	local objs = {};
	for i=0,m_uiRuleContent.transform.childCount-1 do
		table.insert(objs,m_uiRuleContent.transform:GetChild(i).gameObject);
	end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "UIP_RuleAward(Clone)" then
			m_ObjectPool:Release( "UIP_RuleAward", obj );
		end
	end
end

-- 创建奖励
function ColiseumInfoDlgRuleAwardCreate( uiObj, info )
	if info.m_minrank == info.m_maxrank then
		SetText( uiObj.transform:Find("Rank"), info.m_minrank )
	else
		SetText( uiObj.transform:Find("Rank"), info.m_minrank.."~"..info.m_maxrank )
	end
	
	for i=1, 5, 1 do
		local awardObj = uiObj.transform:Find("AwardList").transform:GetChild(i-1);
		if info.m_award[i] and info.m_award[i].m_kind > 0 then
			local sprite, color, name = AwardInfo( info.m_award[i].m_kind )
			SetTrue( awardObj )
			SetControlID( awardObj, -1000000-info.m_award[i].m_kind )
			SetImage( awardObj.transform:Find("Shape"), sprite );
			if info.m_award[i].m_num > 1 then
				SetTrue( awardObj.transform:Find("NumBack") )
				SetText( awardObj.transform:Find("Num"), knum(info.m_award[i].m_num) );
			else
				SetFalse( awardObj.transform:Find("NumBack") )
				SetText( awardObj.transform:Find("Num"), "" );
			end
		else
			SetFalse( awardObj )
		end
	end
end

function ColiseumInfoDlgSelectAward( awardkind, value )
	if value == 1 then
		SetFalse( m_uiAwardDescLayer )
		return
	end

	local sprite, color, name, c, desc = AwardInfo( awardkind )
	SetTrue( m_uiAwardDescLayer )
	SetText( m_uiAwardDescLayer.transform:Find("Name"), name )
	SetText( m_uiAwardDescLayer.transform:Find("Desc"), desc )
end