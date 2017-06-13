--  角色缓存信息
local Player = class("Player");
function Player:ctor()
	self:Init();
end
function Player:Init()
	self.m_actorid			=	0;	-- 角色编号
	self.m_usertype			=	0;	-- 用户类型
	self.m_aclass			=	0;	-- 角色种类
	self.m_shape			=	0;	-- 形象
	self.m_level			=	0;	-- 级别
	self.m_exp				=	0;	-- 经验
	self.m_expmax			=	0;	-- 最大经验
	self.m_viplevel 		=	0;  -- VIP等级
	self.m_vipexp 			=	0;  -- VIP经验
	self.m_money			=	0;	-- 金钱
	self.m_token			=	0;	-- 钻石
	self.m_createtime		=	0;	-- 创建时间
	self.m_name				=	"";	-- 角色名称
	self.m_config		    =   {};  --配置信息
end

-- 属性变化
function Player:Set( recvValue )
	self.m_actorid 			= 	recvValue.m_actorid;
	self.m_viplevel			=	recvValue.m_viplevel;
	self.m_name				=	recvValue.m_name;
	self.m_aclass			=	tonumber(recvValue.m_aclass);
	self.m_shape			=	recvValue.m_shape;
	self.m_level			=	recvValue.m_level;
	self.m_vipexp			=	recvValue.m_vipexp;
	self:ChangeToken( recvValue.m_exp, 0 )
	self:ChangeExp( recvValue.m_exp, recvValue.m_expmax, 0, 0 )
end

-- 钻石改变
-- EventProtocol.addEventListener( "ChangeToken", function( recvValue ) end )
function Player:ChangeToken( nValue, nPath )
	local oldValue = self.m_token;
	self.m_token = nValue;
	EventProtocol.dispatchEvent( "ChangeToken", { value=nValue, change=nValue-oldValue, path=nPath } );
end

-- 经验改变
-- EventProtocol.addEventListener( "ChangeExp", function( recvValue ) end )
function Player:ChangeExp( nValue, nMaxValue, bIsUp, nPath )
	local oldValue = self.m_exp;
	self.m_exp = nValue;
	if nMaxValue ~= 0 then
		self.m_expmax = nMaxValue;
	end
	EventProtocol.dispatchEvent( "ChangeExp", { value=nValue, maxvalue=self.m_expmax, change=nValue-oldValue, isup=bIsUp, path=nPath } );
end

-- 全局
G_Player = nil;
function GetPlayer()
	if G_Player == nil then
		G_Player = Player.new();
	end
	return G_Player;
end
