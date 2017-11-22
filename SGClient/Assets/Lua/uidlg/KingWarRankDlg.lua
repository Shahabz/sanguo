-- 界面
local m_Dlg = nil;
local m_uiTotalScore = nil; --UnityEngine.GameObject
local m_uiNowEnemyKills = nil; --UnityEngine.GameObject
local m_uiNowAwardScore = nil; --UnityEngine.GameObject
local m_uiNextEnemyKills = nil; --UnityEngine.GameObject
local m_uiNextAwardScore = nil; --UnityEngine.GameObject
local m_uiPossibleAwardScore = nil; --UnityEngine.GameObject
local m_uiUIP_Army	=	nil;
local m_uiContent	=	nil;
--对象池
local m_ObjectPool = nil;

-- 打开界面
function KingWarRankDlgOpen()
	m_Dlg = eye.uiManager:Open( "KingWarRankDlg" );

end

-- 隐藏界面
function KingWarRankDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "KingWarRankDlg" );
end

-- 删除界面
function KingWarRankDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function KingWarRankDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            KingWarRankDlgClose();
        end
	end
end

-- 载入时调用
function KingWarRankDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiTotalScore = objs[0];
	m_uiNowEnemyKills = objs[1];
	m_uiNowAwardScore = objs[2];
	m_uiNextEnemyKills = objs[3];
	m_uiNextAwardScore = objs[4];
	m_uiPossibleAwardScore = objs[5];
	m_uiUIP_Army = objs[6];
	m_uiContent = objs[7];
	-- 对象池
	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );
	m_ObjectPool:CreatePool("UIP_King", 5, 5, m_uiUIP_Army);
	
end

-- 界面初始化时调用
function KingWarRankDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function KingWarRankDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function KingWarRankDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function KingWarRankDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function KingWarRankDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function KingWarRankDlgShow()
	
	KingWarRankDlgOpen()
	KingWarRankDlgClear();
	local recvValue = {}
	-- 获取血战排行榜列表
	system_askinfo( ASKINFO_KINGWAR, "", 10 )
	--[[local recvValue = {}
	recvValue.m_count = 15
	recvValue.m_mypoint = 1000
	recvValue.m_totalkill = 25000
	recvValue.m_list ={ 
		{ m_rank=1,m_kill=456222,m_nation=1,m_name="x皇天后土",m_herokind=1,m_herocolor=1 },
		{ m_rank=2,m_kill=55655,m_nation=2,m_name="身高多少",m_herokind=2,m_herocolor=1 },
		{ m_rank=3,m_kill=656,m_nation=3,m_name="x法国队",m_herokind=3,m_herocolor=1 },
		{ m_rank=4,m_kill=756,m_nation=2,m_name="似的发射点",m_herokind=4,m_herocolor=1 },
		{ m_rank=5,m_kill=856,m_nation=1,m_name="防晒",m_herokind=2,m_herocolor=1 },
		{ m_rank=6,m_kill=900,m_nation=2,m_name="xxxxxx",m_herokind=3,m_herocolor=1 },
		{ m_rank=7,m_kill=1056,m_nation=3,m_name="xxxxxx",m_herokind=1,m_herocolor=1 },
		{ m_rank=8,m_kill=116,m_nation=1,m_name="xxxxxx",m_herokind=2,m_herocolor=1 },
		{ m_rank=9,m_kill=12000,m_nation=1,m_name="xxxxxx",m_herokind=3,m_herocolor=1 },
		{ m_rank=10,m_kill=1356,m_nation=1,m_name="xxxxxx",m_herokind=2,m_herocolor=1 },
		{ m_rank=11,m_kill=1456,m_nation=1,m_name="xxxxxx",m_herokind=1,m_herocolor=1 },
		{ m_rank=12,m_kill=1556,m_nation=1,m_name="xxxxxx",m_herokind=1,m_herocolor=1 },
		{ m_rank=13,m_kill=1656,m_nation=1,m_name="xxxxxx",m_herokind=1,m_herocolor=1 },
		{ m_rank=14,m_kill=1756,m_nation=1,m_name="xxxxxx",m_herokind=1,m_herocolor=1 },
		{ m_rank=15,m_kill=1856,m_nation=1,m_name="xxxxxx",m_herokind=1,m_herocolor=1 },
	}
	 recvValue.m_myrank = 10;
	]]--
end

--填充榜单中的信息
function KingWarRankDlgRecvValue( recvValue )
	for i=1, recvValue.m_count, 1 do
		KingWarRankDlgCreateKing( i, recvValue.m_list[i] )
	end
	SetText(m_uiTotalScore,F(1909,recvValue.m_mypoint));
	local kills=recvValue.m_totalkill;
	SetText(m_uiNowEnemyKills,F(1910,kills));
	local killNum;
	for i=1,#g_kingwar_config do
		if kills < g_kingwar_config[i].totalkill then
			killNum = i-1;
			break;
		end
	end
	local newAwardScore=nil;
	if killNum == 0 then
		newAwardScore=0;
		SetText(m_uiNowAwardScore,F(1911,0));
	else
		newAwardScore=g_kingwar_config[killNum].totalkill_award_point;
		SetText(m_uiNowAwardScore,F(1911,newAwardScore));
	end
	local nextKillNum = killNum+1;
	SetText(m_uiNextEnemyKills,F(1912,g_kingwar_config[nextKillNum].totalkill));
	SetText(m_uiNextAwardScore,F(1911,g_kingwar_config[nextKillNum].totalkill_award_point));
	local myrank = recvValue.m_myrank;
	local myaward = nil;
	local sumrank = #g_kingwar_config;
	if myrank == 0 then
		myaward = 0;
	elseif myrank < sumrank then
		myaward = g_kingwar_config[myrank].rank_award_point;
	else
		myaward = g_kingwar_config[myrank].rank_award_point;
	end
	SetText(m_uiPossibleAwardScore,F(1913,newAwardScore+myaward)) 
end

-- 清空单元
function KingWarRankDlgClear()
	local objs = {};
	for i = 0 ,(m_uiContent.transform.childCount - 1) do
		table.insert( objs, m_uiContent.transform:GetChild(i).gameObject )
    end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "UIP_King(Clone)" then
			m_ObjectPool:Release( "UIP_King", obj );
		end
    end
end
--显示榜单中前30名的击杀信息
function KingWarRankDlgCreateKing( index, info )
	local uiObj = m_ObjectPool:Get( "UIP_King" );
	uiObj.transform:SetParent( m_uiContent.transform );
	local objs = uiObj:GetComponent( typeof(Reference) ).relatedGameObject;
	uiSort = objs[0];
	uiKills = objs[1];
	uiNation = objs[2];
	uiPlayer = objs[3];
	uiArmed = objs[4];
	uiScore = objs[5];
	local textColorSet=nil;
	if index == 1 then
		textColorSet=NameColor(5);
	elseif index == 2 then
		textColorSet=NameColor(4);
	elseif index == 3 then
		textColorSet=NameColor(3);
	else 
		textColorSet=NameColor(0);
	end
	SetText( uiSort, index ,textColorSet);
	SetText( uiKills, knum(info.m_kill) ,textColorSet);
	SetText( uiPlayer, info.m_name,textColorSet );
	SetText( uiArmed, HeroName(info.m_herokind),textColorSet );
	SetText( uiScore,knum(KingWarRankDlgAward(index)) ,textColorSet);
	SetImage( uiNation,NationSprite(info.m_nation));
end
--排名奖励积分 超出排名的以最后一名奖励积分为奖励
function KingWarRankDlgAward(sort)
	local maxNumber	= #g_kingwar_config
	if sort	> maxNumber then
		return g_kingwar_config[maxNumber].rank_award_point;
	else
		return	g_kingwar_config[sort].rank_award_point;
	end
end











