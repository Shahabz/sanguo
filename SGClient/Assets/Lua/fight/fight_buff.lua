FightBuff = class("FightBuff")
function FightBuff:ctor()
	self:Reset();
end

function FightBuff:Reset()
	self.m_id				=	0;
	
end

-- 回合逻辑
function FightBuff:Logic( unit )
	
end
