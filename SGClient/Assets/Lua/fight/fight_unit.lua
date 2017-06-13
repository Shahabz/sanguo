-- 一个战场单元
-- 英雄
 heroprefab = {
[1] = "hero_brute_dragon",
[2] = "hero_centaur",
[3] = "hero_faith_healer",
[4] = "hero_orc_monk",
[5] = "hero_pirate",
[6] = "hero_polemaster",
[7] = "hero_shadow_assassin",
[8] = "hero_shadow_assassin",
}

-- 神邸
local godprefab = {
[1] = "bosspit_flag",
}

FightUnit = class("FightUnit")
function FightUnit:ctor()
	self:Reset();
end

function FightUnit:Reset()
	self.m_id				=	0;	-- 战场单元唯一标识ID
	self.m_kind				=	0;	-- 种类
	self.m_job				=	0;	-- 职业
	self.m_side				=	0;	-- 1我方 -1敌方
	self.m_state			=	0;	-- 状态
	self.m_target			=	0;	-- 目标
	self.m_obj				=	nil;-- 对象
	self.m_posx				=	0;  -- 位置
	self.m_posy				=	0;  -- 位置
	self.m_toposx			=	0;  -- 位置
	self.m_toposy			=	0;  -- 位置
	self.m_attackcd		=	0;	-- 攻击冷却
	
	-- 基础属性
	self.m_life						=	1000;	-- 当前生命值(固定数值)
	self.m_life_max					=	1000;	-- 生命值(固定数值)
	self.m_attack					=	0;	-- 攻击(固定数值)
	self.m_defence					=	0;	-- 防御(固定数值)
	
	self.m_precision				=	0;	-- 命中率(百分比数值)
	self.m_dodge					=	0;	-- 闪避率(百分比数值)
	
	self.m_crit						=	0;	-- 暴击率(百分比数值)
	self.m_crit_resist				=	0;	-- 抗暴率(百分比数值)
	
	self.m_crit_damage				=	0;	-- 暴伤(固定数值)
	self.m_crit_damage_resist		=	0;	-- 暴伤减免(固定数值)
	
	self.m_speed_attack				=	0;	-- 攻速(固定数值)
	self.m_speed_move				=	0;	-- 移速(固定数值)
	
	self.m_ignore_defence			=	0;	-- 破击率(百分比数值)
	
	self.m_damage_increase			=	0;	-- 伤害加成
	self.m_damage_reduce			=	0;	-- 伤害减免
	
	self.m_record_damage	=	0;  -- 数据统计 - 造成伤害
	self.m_record_bear		=	0;	-- 数据统计 - 承受伤害
	self.m_record_addhp		=	0;	-- 数据统计 - 治疗
	
	-- 拥有技能
	self.m_skills			=	{};
	-- 正在使用的技能
	self.m_useskill			=	0;
	
	-- BUFF
	self.m_buffs			=	{};
	-- 事件
	self.m_events			=	{};
	-- 目标
	self.m_target			=	nil;
	self.m_levent			=	false
end

-- 回合逻辑
function FightUnit:Logic()
	
	for k, v in pairs( self.m_buffs ) do
		v:Logic( self );
	end
	for k, v in pairs( self.m_events ) do
		v:Logic( self );
	end
	if self.m_job < 100 then
		self:FindTarget();
		if self.m_state == 1 then
			self:Move();
		elseif self.m_state == 2 then
			self:Attack();
		elseif self.m_state == 3 then
			
		end
	end
end

-- 创建
function FightUnit:Create( side, kind, attr, callback )
	self.m_id = GetFightRoom():GetIncrement();
	self.m_side = side;
	self.m_kind = kind;
	self.m_speed_attack = custom.rand( 30, 60 )
	self.m_obj = eye.objectPoolManager:Get( heroprefab[kind] );
	self.m_obj.transform:SetParent( GameManager.FightScence.transform:Find("Land").transform );
	self.m_obj.transform.localScale = Vector3( side, 1, 1 );
	
--[[	local UIU_HPProgress = self.m_target.m_obj.transform:Find("UIU_HPProgress");
	if UIU_HPProgress == nil then
		UIU_HPProgress = GameObject.Instantiate( LoadPrefab( "UIU_HPProgress" ) );
		UIU_HPProgress.transform:SetParent( self.m_target.m_obj.transform );
		UIU_HPProgress.transform.localScale = Vector3( 1, 1, 1 );
		UIU_HPProgress.transform.localPosition = Vector3( 0, 170, 0 );
	end--]]
	self.m_obj.transform:Find("UIU_HPProgress").gameObject:SetActive( false );
						
	if callback then
		callback();
	end
end

-- 创建神邸
function FightUnit:CreateGod( side, kind, attr )
	self.m_id = GetFightRoom():GetIncrement();
	self.m_side = side;
	self.m_kind = kind;
	self.m_job = 100;
	local prefab = LoadPrefab( godprefab[kind] ) 
	self.m_obj = GameObject.Instantiate( prefab );
	self.m_obj.transform:SetParent( GameManager.FightScence.transform:Find("Land").transform );
	self.m_obj.transform.localScale = Vector3( side, 1, 1 );
	
--[[	local UIU_HPProgress = self.m_target.m_obj.transform:Find("UIU_HPProgress");
	if UIU_HPProgress == nil then
		UIU_HPProgress = GameObject.Instantiate( LoadPrefab( "UIU_HPProgress" ) );
		UIU_HPProgress.transform:SetParent( self.m_target.m_obj.transform );
		UIU_HPProgress.transform.localScale = Vector3( 1, 1, 1 );
		UIU_HPProgress.transform.localPosition = Vector3( 0, 170, 0 );
	end--]]
	self.m_obj.transform:Find("UIU_HPProgress").gameObject:SetActive( false );
	
end

-- 删除
function FightUnit:Delete()
	GameObject.Destroy( self.m_obj );
end

-- 添加Buff
function FightUnit:AddBuff( buff )
	table.insert( self.m_buffs, buff );
end

-- 添加事件
function FightUnit:AddEvent( event )
	table.insert( self.m_events, event );
end

-- 播放
function FightUnit:Play( actionName, loop )
	self.m_obj.transform:GetChild(0):GetComponent( "UnityArmatureComponent" ).animation:Play( actionName, loop );
end

-- 设置位置
function FightUnit:SetPos( posx, posy )
	self.m_posx				=	posx;
	self.m_posy				=	posy;
	self.m_obj.transform.localPosition = Vector3( posx, posy, 0 );
	
	-- 目标点
	self.m_toposx	= self.m_posx * -self.m_side;
	self.m_toposy 	= self.m_posy;
end

-- 设置状态
function FightUnit:ChangeState( state )
	if self.m_state == state then
		return
	end
	self.m_state = state;
	actionName = "move";
	if state == 1 then
		actionName = "move";
		--self.m_obj:GetComponent( "UnitMove" ).toPosition = Vector3( self.m_toposx, self.m_toposy, 0 );
		--self.m_obj:GetComponent( "UnitMove" ).speed = 30 
		self.m_obj:GetComponent( "UnitMove" ).stat = 0;
		self.m_obj.transform:GetChild(0):GetComponent( "UnityArmatureComponent" ).animation:Play( actionName, 0 );
	elseif state == 2 then
		actionName = "attack";
		self.m_obj:GetComponent( "UnitMove" ).stat = 1;
	elseif state == 3 then
		actionName = "death";
		self.m_obj:GetComponent( "UnitMove" ).stat = 0;
		self.m_obj.transform:GetChild(0):GetComponent( "UnityArmatureComponent" ).animation:Play( actionName, 1 );
		Invoke(function( obj ) 
			eye.objectPoolManager:Release( heroprefab[self.m_kind], self.m_obj );				
		end, 1, self.m_obj, "Destroy_"..self.m_id )
	end
end

-- 行走
function FightUnit:Move()	
	self.m_posx	= self.m_posx + self.m_side*5; -- 比如每秒30回合，一回合走10像素，那么每秒走30*30个像素
	--print( self.m_obj.transform.localPosition.x .. "~" .. self.m_posx )
	--print( os.time() )
	--self.m_timer = self.m_timer + Time.deltaTime;
	--print self.m_toposx )
	--print( Time.deltaTime )
	--self.m_obj.transform.localPosition = Vector3.MoveTowards( self.m_obj.transform.localPosition, Vector3( self.m_posx, self.m_posy, 0 ), 10 );
	--self.m_obj.transform.localPosition = Vector3.Lerp( self.m_obj.transform.localPosition, Vector3( self.m_posx, self.m_posy, 0 ), Time.deltaTime );
	--self.m_obj.transform.localPosition = Vector3( self.m_posx, self.m_posy, 0 );
	
		self.m_obj:GetComponent( "UnitMove" ).toPosition = Vector3( self.m_posx, self.m_posy, 0 );
		self.m_obj:GetComponent( "UnitMove" ).speed = 150;
		
		--print( self.m_obj.transform.localPosition.x .. "~" .. self.m_posx )
end

-- 攻击
function FightUnit:Attack()
	self.m_attackcd = self.m_attackcd + 1
	if self.m_attackcd == self.m_speed_attack then
		if self.m_target == nil or self.m_target.m_state == 3 then
			self.m_attackcd = 0;
			return
		end
		self.m_obj.transform:GetChild(0):GetComponent( "UnityArmatureComponent" ).animation:Play( actionName, 1 );
		
		-- 帧事件
		self.m_levent = function( type, eventObject ) 
				if eventObject.name == "vfx_attack" then
					if self.m_target then
						local damage = custom.rand(10, 500);
						-- 血字
						local prefab = LoadPrefab( "UIU_Damage" ) 
						local obj = GameObject.Instantiate( prefab );
						obj.transform:SetParent( GameManager.FightScence.transform:Find("Land").transform );
						obj.transform.localScale = Vector3( 1, 1, 1 );
						obj.transform.localPosition = Vector3( self.m_target.m_obj.transform.localPosition.x, self.m_target.m_obj.transform.localPosition.y+160, 0 );
						obj.transform:Find("Text"):GetComponent("UIText").text = damage;
						Invoke(function( obj ) 
							GameObject.Destroy( obj );
						end, 1, obj )
						
						-- 血条
						self.m_target.m_life = self.m_target.m_life - damage
						
						local UIU_HPProgress = self.m_target.m_obj.transform:Find("UIU_HPProgress");
						UIU_HPProgress:GetComponent("UIProgress"):SetValue( self.m_target.m_life/self.m_target.m_life_max )
						UIU_HPProgress.gameObject:SetActive( true );
						Invoke(function( hpobj ) 
							hpobj.transform:Find("UIU_HPProgress").gameObject:SetActive( false );
						end, 3, self.m_target.m_obj, "Destroy_UIU_HPProgress_"..self.m_target.m_id )
						
						-- 死亡
						if self.m_target.m_life <= 0 then
							InvokeStop( "Destroy_UIU_HPProgress_"..self.m_target.m_id )
							
							if self.m_target.m_job < 100 then
								self.m_target:Dead();
								self.m_target = nil;
							else
								-- 游戏胜利
								
								self.m_target = nil;
							end
						end
					end
					self.m_obj.transform:GetChild(0):GetComponent( "UnityArmatureComponent" ).armature.eventDispatcher:RemoveEventListener("frameEvent", self.m_levent )
				end		
		end	
		self.m_obj.transform:GetChild(0):GetComponent( "UnityArmatureComponent" ).armature.eventDispatcher:AddEventListener("frameEvent", self.m_levent )		
		self.m_attackcd = 0;
	end
end

-- 死亡
function FightUnit:Dead()
	self:ChangeState( 3 );
end

-- 发现目标
function FightUnit:FindTarget()
	if self.m_state == 3 then
		return
	end
	local god = nil;
	local targetList = {};
	if self.m_side == -1 then
		targetList = GetFightRoom().m_our_units;
		god = GetFightRoom().m_our_god;
	else
		targetList = GetFightRoom().m_enemy_units;
		god = GetFightRoom().m_enemy_god;
	end
	
	local myPos = cc.p( self.m_posx, self.m_posy );
	local targetUnit = nil;
	local minDistance = 100000;
	for k, v in pairs( targetList ) do
		local d = cc.pGetDistance(myPos,{x=v.m_posx,y=v.m_posy});
		if d < minDistance and d < 120 and v.m_state ~= 3 then
			minDistance = d;
			targetUnit = v
		end
	end
	
	-- 没找到敌人，看看神邸
	if targetUnit == nil then
		local d = cc.pGetDistance(myPos,{x=god.m_posx,y=god.m_posy});
		if d < 120 then
			targetUnit = god;
		end
	end
	
	self.m_target = targetUnit;
	if targetUnit ~= nil then
		self:ChangeState( 2 );
	else
		self:ChangeState( 1 );
	end
end