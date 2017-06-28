-- 一个结构
--[[SLK_Building = class("SLK_Building")
function SLK_Building:ctor()
	self:empty();
end

function SLK_Building:empty()
	
	-- 固定配置信息
	self.m_kind  			= 0;-- j种类 决定了物品的名字
	self.m_level 			= 0;-- 等级
	
	return self;
end--]]
