-- 一个战场房间
-- 实时PVP模式
-- 客户端每秒20次(50ms)运行着战斗逻辑
-- 客户端每运行10次战斗逻辑，检查操作队列是否有数据，无数据，等待。有数据，执行数据内容，然后下轮游戏逻辑
FightRoom = class("FightRoom")
function FightRoom:ctor()
	self:Reset();
end

function FightRoom:Reset()
	self.m_fightid			=	0;				-- 战场ID
	self.m_maxtime			=	0;				-- 战场时间
	self.m_randseed			=	0;				-- 随机种子
	self.m_turns			=	0;				-- 当前第几个游戏回合
	self.m_serverturns		=	0;				-- 当前第几个游戏回合
	self.m_auto_increment	=	0;				-- 自动增量，每增加一个单元自增
	self.m_side				=	0;				-- 我是攻击方1还是防御方-1
	
	-- 我方
	self.m_our_units		=	{};				-- 单元
	self.m_our_unitnum 		=	0;				-- 单元总数量
	self.m_our_god			=	nil;			-- 神邸
	
	-- 敌方
	self.m_enemy_units		=	{};
	self.m_enemy_unitnum 	=	0;	
	self.m_enemy_god		=	nil;
	
	-- 帧同步相关
	self.m_GameFrame		=	0;
	self.m_LockStepTurnID	=	0;	
	self.m_CommandQueue		=	Queue.new();	-- 帧同步回合队列		
end

-- 回合逻辑
function FightRoom:Logic()
	
	-- 战斗单元逻辑
	for k, v in pairs( self.m_our_units ) do
		v:Logic();
	end
	for k, v in pairs( self.m_enemy_units ) do
		v:Logic();
	end
	
	-- 神邸逻辑
	self.m_our_god:Logic( self );
	self.m_enemy_god:Logic( self );
	
	-- 游戏总回合数
	self.m_turns = self.m_turns + 1;
end

-- 创建一个战场
function FightRoom:Create( recvValue )
	MainDlgClose();
	FightRoomSetHeroDlgClose();
	if GameManager.MainCityScence ~= nil then
		GameManager.MainCityScence.gameObject:SetActive( false );
	end
	self:Reset();
	self.m_fightid = recvValue.m_fightid;
	self.m_side = recvValue.m_side;
	self.m_maxtime = recvValue.m_maxtime;
	self.m_randseed = recvValue.m_randseed;
	custom.randseed( self.m_randseed );
	
	if self.m_side == 1 then
		-- 我是攻击方
	else
		-- 我是防御方
	end
	
	-- 战场
	GameManager.FightScence = GameObject.Instantiate( LoadPrefab( "FightScence" ) );
	GameManager.FightScence.transform:Find("Camera").localPosition = Vector3( 930, 0, -10000 );
	
	Invoke( function()
		GameManager.FightScence.transform:Find("Camera"):GetComponent( "FightCamera" ):TweenPosToInBound( Vector3( -930, 0, -10000 ), 1 );
	end, 1, param, "11111")
	
	Invoke( function()
		FightDlgShow();
		GameManager.FightScence.transform:GetComponent( "LockStepManager" ).enabled = true;
	end, 1, param, "22222")
	
	-- 我方神邸
	self.m_our_god = FightUnit.new();
	self.m_our_god:CreateGod( 1, 1, { m_life = 10000 } );
	self.m_our_god:Play( "wind_left_loop", 0 );
	self.m_our_god:SetPos( -1400, 0 );
	
	
	-- 敌方神邸
	self.m_enemy_god = FightUnit.new();
	self.m_enemy_god:CreateGod( -1, 1, { m_life = 10000 } );
	self.m_enemy_god:Play( "wind_left_loop", 0 );
	self.m_enemy_god:SetPos( 1400, 0 );
	
	-- 倒计时
	FightDlgChangeCountdown( self.m_maxtime );
	
	-- 血条
	FightDlgChangeOurLife( 10000, 10000 );
	FightDlgChangeEnemyLife( 10000, 10000 );
	
	if self.m_fightid < 0 then
		GetFightRoom():AddUnit( {m_side = -1, m_kind = 1, m_offsetx = -200} )
	end
	--GetFightRoom():AddUnit( {m_side = -1, m_kind = 2, m_offsetx = -400} )
	--GetFightRoom():AddUnit( {m_side = -1, m_kind = 3, m_offsetx = -600} )
end

-- 加入一个单元
function FightRoom:AddUnit( recvValue )
	local unitObj = FightUnit.new();
	local side = 0;
	if recvValue.m_side == self.m_side then	
		side = 1; -- 我方
	else
		side = -1;-- 敌方
	end	
	unitObj:Create( side, recvValue.m_kind, {}, function() 
		unitObj:SetPos( -1400*side + recvValue.m_offsetx, 0 );
		unitObj:ChangeState( 1 );
		if side == 1 then
			-- 加入我方
			table.insert( self.m_our_units, unitObj );
		else
			-- 加入敌方
			table.insert( self.m_enemy_units, unitObj );
		end
	
	end );
	
	-- 此处记录录像
	-- 记录内容：帧号self.m_turns，干了什么事
end

-- 获取自动增量
function FightRoom:GetIncrement()
	self.m_auto_increment = self.m_auto_increment + 1;
	return self.m_auto_increment;
end

-- 全局
G_FightRoom = nil;
function GetFightRoom()
	if G_FightRoom == nil then
		G_FightRoom = FightRoom.new();
	end
	return G_FightRoom;
end

-- 添加指令
function FightRoom:AddTurnCommand( recvValue )
	self.m_serverturns = recvValue.m_turns
	Queue.pushBack( self.m_CommandQueue, recvValue );
end

-- 帧同步回合
function FightRoom:NextTurnCommand()
	while Queue.has( self.m_CommandQueue ) do
		local recvValue = Queue.popFirst( self.m_CommandQueue );
		if recvValue and recvValue.m_turns == self.m_LockStepTurnID then
			return recvValue;
		end
	end 
	return nil;
end

-- 帧同步回合
function FightRoom:LockStepTurn()
	print( "LockStepTurnID: " .. self.m_LockStepTurnID .. "recvValue: "..self.m_serverturns );
	-- 检查下一回合能否继续
	local recvValue = GetFightRoom():NextTurnCommand();
	if recvValue then
		-- process actions
		for i=1, recvValue.m_count, 1 do
			GetFightRoom():AddUnit( {m_side = recvValue.m_list[i].m_side, m_kind = recvValue.m_list[i].m_kind, m_offsetx = -200} )
		end
		
		self.m_LockStepTurnID = self.m_LockStepTurnID + 1;
		return true;
	end
	return false;
end

local GMONSTER = 0
-- 游戏回合
function FightFrameTurn()
	-- 帧同步回合
	if GetFightRoom().m_fightid > 0 then
		if GetFightRoom().m_GameFrame == 0 then
			if GetFightRoom():LockStepTurn() == false then
				return;
			end
		end	
	end
	
	-- 战斗逻辑
	GetFightRoom():Logic();
	--print( ""..GetFightRoom().m_LockStepTurnID .. " GetFightRoom():Logic()" )
	
	-- 回合
	GetFightRoom().m_GameFrame = GetFightRoom().m_GameFrame + 1;
	if GetFightRoom().m_GameFrame == 10 then
		GetFightRoom().m_GameFrame = 0;
	end
	
	
	--
	GMONSTER = GMONSTER + 1;
	if GetFightRoom().m_fightid < 0 and GMONSTER == 500 then
		GetFightRoom():AddUnit( {m_side = -1, m_kind = custom.rand( 1, 7 ), m_offsetx = -200} )
		GMONSTER = 0;
	end
end

