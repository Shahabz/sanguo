--  角色缓存信息
local Player = class("Player");
function Player:ctor()
	self:Init();
end
function Player:Init()
	self.m_usertype			=	0;	-- 用户类型
	self.m_createtime		=	0;	-- 创建时间
	self.m_config		    =   {};  --配置信息
	
	self.m_actorid			=	0;	-- 角色编号
	self.m_name				=	"";	-- 角色名称
	self.m_nation			=	0;
	self.m_shape			=	0;
	self.m_level			=	0;
	self.m_exp				=	0;
	self.m_exp_max			=	0;
	self.m_token			=	0;
	self.m_viplevel			=	0;
	self.m_vipexp			=	0;
	self.m_vipexp_max		=	0;
	self.m_body				=	0;
	self.m_place			=	0;
	self.m_official			=	0;
	self.m_zone				=	0;
	self.m_battlepower		=	0;
	self.m_silver			=	0;
	self.m_wood				=	0;
	self.m_food				=	0;
	self.m_iron				=	0;
	self.m_infantry_num		=	0;
	self.m_cavalry_num		=	0;
	self.m_archer_num		=	0;
	self.m_mokilllv			=	0;
	self.m_sflag			=	0;
end

-- 属性变化
function Player:Set( recvValue )
	self.m_actorid			=	recvValue.m_actorid;
	self.m_name				=	recvValue.m_name;
	self.m_nation			=	recvValue.m_nation;
	self.m_shape			=	recvValue.m_shape;
	self.m_level			=	recvValue.m_level;
	self.m_exp				=	recvValue.m_exp;
	self.m_exp_max			=	recvValue.m_exp_max;
	self.m_token			=	recvValue.m_token;
	self.m_viplevel			=	recvValue.m_viplevel;
	self.m_vipexp			=	recvValue.m_vipexp;
	self.m_vipexp_max		=	recvValue.m_vipexp_max;
	self.m_body				=	recvValue.m_body;
	self.m_place			=	recvValue.m_place;
	self.m_official			=	recvValue.m_official;
	self.m_zone				=	recvValue.m_zone;
	self.m_battlepower		=	recvValue.m_battlepower;
	self.m_silver			=	recvValue.m_silver;
	self.m_wood				=	recvValue.m_wood;
	self.m_food				=	recvValue.m_food;
	self.m_iron				=	recvValue.m_iron;
	self.m_infantry_num		=	recvValue.m_infantry_num;
	self.m_cavalry_num		=	recvValue.m_cavalry_num;
	self.m_archer_num		=	recvValue.m_archer_num;
	self.m_mokilllv			=	recvValue.m_mokilllv;
	self.m_sflag			=	recvValue.m_sflag;
	
	--self:ChangeToken( recvValue.m_exp, 0 )
	--self:ChangeExp( recvValue.m_exp, recvValue.m_expmax, 0, 0 )
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
