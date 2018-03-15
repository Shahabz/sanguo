-- 邮件
local json = require "cjson"
MAIL_TYPE_SYSTEM		=	0	-- 系统信息邮件
MAIL_TYPE_NOTIFY		=	1	-- 公告邮件，内容外部http服务器获取
MAIL_TYPE_ACTOR_SEND	=	2	-- 玩家发送邮件
MAIL_TYPE_ACTOR_REPLY	=	3	-- 玩家回复邮件
MAIL_TYPE_FIGHT_ENEMY	=	10	-- 流寇
MAIL_TYPE_FIGHT_CITY	=	11	-- 城战
MAIL_TYPE_FIGHT_NATION	=	12	-- 国战
MAIL_TYPE_CITY_SPY		=	13	-- 侦察
MAIL_TYPE_CITY_BESPY	=	14	-- 被侦察
MAIL_TYPE_FIGHT_NATIONHERO	=	15	-- 国家名将
MAIL_TYPE_GATHER		=	20	-- 采集
MAIL_TYPE_GATHER_FIGHT	=	21	-- 采集战斗

TAG_TEXT		= "##"	-- 标记--标记为文字
TAG_TEXTID 		= "#$"	-- 标记 为文字表id
TAG_ITEMID 		= "$$"	-- 标记 标记为道具装备ID
TAG_EQUIPKIND 	= "$%"	-- 标记 标记为装备专用
TAG_ZONEID		= "$*"	-- 标记 标记为地区专用
TAG_TOWNID 		= "$&"	-- 标记 标记为城镇专用
TAG_TIMEDAY		= "$D"	-- 标记 标记为时间天
TAG_TIMEHOUR	= "$H"	-- 标记 标记为时间小时
TAG_TIMEMIN		= "$M"	-- 标记 标记为时间分钟
TAG_TIMESEC		= "$S"	-- 标记 标记为时间秒
TAG_DATE		= "$T"	-- 标记--标记为时间年月日：时分秒
TAG_POS    		= "$#"	-- 标记 标记为坐标
TAG_NATION		= "$N"  -- 标记 标记为国家
TAG_AWARD		= "$A"  -- 标记 标记为奖励
TAG_KINGWAR		= "$K"	-- 标记--标记为皇城战据点
TAG_HERO		= "$R"  -- 标记--标记为武将
TAG_COLOR		= "$C"	-- 标记--标记为颜色
TAG_OFFICIAL	= "$O"	-- 标记--标记为官职

local Mail = class("Mail");
function Mail:ctor()
    self:Init();
end
function Mail:Init()
	self.m_FileName = ""
    self.m_Mails = {};
	self.m_IncrementID = 0;
    self.m_bIsLoad = false;
	self.m_nNoReadCount = 0;
    self.m_nRecvCount = 0;
	self.m_nMinMailID = int64.new(0);
	self.m_nMaxMailID = int64.new(0);
	self.m_nLastRecvMinMailID = int64.new(0);
	self.m_bLoadNew = false;
	self.m_bLoadAll = false;
end

function Mail:NewIndex()
    self.m_MailIndex = self.m_MailIndex + 1;
    return self.m_MailIndex;
end

-- 读取本地缓存
function Mail:LoadCache()
	-- 缓存文件名
--[[    self.m_FileName = PathUtil.GameCachePath() .. "mail_"..Const.serverid .. '_' .. Const.actorid .. ".cache";
	-- 读取缓存
    local _tableUtil = TableUtil.New();
    local currTime = GetServerTime();
    if _tableUtil:OpenFromTXT( self.m_FileName ) == true then
        local fieldsNum = _tableUtil:GetFieldsNum();
        for records = 0, _tableUtil:GetRecordsNum() -1, 1 do
            local mailid = tonumber(_tableUtil:GetValue(records, 0));
            local type = tonumber(_tableUtil:GetValue(records, 1));
			local title = tostring(_tableUtil:GetValue(records, 2));
            local content = tostring(_tableUtil:GetValue(records, 3));
			local attach = tostring(_tableUtil:GetValue(records, 4));
			local attachget = tostring(_tableUtil:GetValue(records, 5));
			local recvtime = tonumber(_tableUtil:GetValue(records, 6));
			local read = tonumber(_tableUtil:GetValue(records, 7));
			local fightid = tonumber(_tableUtil:GetValue(records, 8));
			local lock = tonumber(_tableUtil:GetValue(records, 9));
			
            if currTime < recvtime + 7*86400 then
                content = string.gsub(content, '\\n', '\n');
                content = string.gsub(content, '\\t', '\t');
				
				table.insert( self.m_Mails, {
                    m_mailid = mailid,
                    m_type = type,
                    m_title = title,
                    m_content = content,
                    m_attach = attach,
                    m_attachget = attachget,
                    m_recvtime = recvtime,
                    m_read = read,
					m_lock = lock,
                    m_fightid = fightid,
                } )
				
				-- 接收个数
				if m_read == 0 then
					self.m_nRecvCount = self.m_nRecvCount + 1;
				end
				
				-- 最大id
				if mailid > self.m_nMaxMailID then
					self.m_nMaxMailID = mailid;
				end
				
				-- 最小id
				if mailid < self.m_nMinMailID or self.m_nMinMailID <= 0 then
					self.m_nMinMailID = mailid;
				end
            end
			
        end
    end
	
	-- 排序
	self:Sort();
	self.m_bIsLoad = true;--]]
end

-- 存档本地缓存
function Mail:SaveCache()
	--print( PathUtil.GameCachePath() )
	--[[if not Utils.Exists( PathUtil.GameCachePath() ) then
        Utils.CreateDirectory( PathUtil.GameCachePath() )
    end
	
	-- 清空文件
    local fp = io.open( self.m_FileName, "w+" )
    if fp == nil then
        return;
    end
    fp:close();
	
    -- 重新打开文件
    fp = io.open( self.m_FileName, "a+b" )
    if fp == nil then
        return;
    end
	
    fp:write( "mailid" .. "\t" 
	.. "type" .. "\t" 
	.. "title" .. "\t" 
	.. "content" .. "\t" 
	.. "attach" .. "\t" 
	.. "attachget" .. "\t" 
	.. "recvtime" .. "\t" 
	.. "read" .. "\t"
	.. "lock" .. "\t"
	.. "fightid" .. "\r\n")
	
	local content = "";
    local serverTime = GetServerTime();
    for k, v in pairs(self.m_Mails) do
        if serverTime < v.m_recvtime + 7*86400 then
            content = string.gsub(v.m_content, '\n', '\\n');
            content = string.gsub(content, '\t', '\\t');
			
            local text = v.m_mailid .. "\t" 
			.. v.m_type .. "\t" 
			.. v.m_title .. "\t" 
			.. v.m_content .. "\t" 
			.. v.m_attach .. "\t" 
			.. v.m_attachget .. "\t" 
			.. v.m_recvtime .. "\t" 
			.. v.m_read .. "\t" 
			.. v.m_lock .. "\t" 
			.. v.m_fightid .. "\r\n";
            fp:write(text)
        end
    end
    fp:close();--]]
end

-- 插入最新数据
function Mail:Insert( recvValue )
	local m_content_json = json.decode( recvValue.m_content );
	self.m_IncrementID = self.m_IncrementID + 1
	table.insert( self.m_Mails, {
		m_incrementid = self.m_IncrementID,
		m_mailid = recvValue.m_mailid,
		m_actorid = recvValue.m_actorid,
		m_type = recvValue.m_type,
		m_title = recvValue.m_title,
		m_content = recvValue.m_content,
		m_attach = recvValue.m_attach,
		m_attachget = recvValue.m_attachget,
		m_recvtime = recvValue.m_recvtime,
		m_read = recvValue.m_read,
		m_lock = recvValue.m_lock,
		m_fightid = recvValue.m_fightid,
		m_viewpath = recvValue.m_viewpath,
		m_content_json = m_content_json,
		m_fight_content = "",
		m_delete_toggle = 0,
	} )
		
	-- 最大id
	if recvValue.m_mailid > self.m_nMaxMailID then
		self.m_nMaxMailID = recvValue.m_mailid;
	end
	
	-- 最小id
	if recvValue.m_mailid < self.m_nMinMailID or int64.equals(self.m_nMinMailID, 0) then
		self.m_nMinMailID = recvValue.m_mailid;
	end
	
	-- 上次接收的最小id，用这个值去拿未获取到的邮件
	if recvValue.m_mailid < self.m_nLastRecvMinMailID or int64.equals( self.m_nLastRecvMinMailID, 0 ) then
		self.m_nLastRecvMinMailID = recvValue.m_mailid;
	end
	
	-- 排序
	self:Sort();
	
	--print( recvValue.m_content )
end

-- 排序
function Mail:Sort()
	table.sort( self.m_Mails, function( a, b )
		if a.m_mailid < b.m_mailid then
			return true;
		else
			return false;
		end
	end )
end

-- 判断内容是否包含特殊格式
function Mail:IsTag(con, tag)
    local mask = string.sub(con, 1, string.len(tag));
    if mask == tag then
        return true;
    else
        return false;
    end
end

function Mail:GetString( v )
	local str = ""
	
	-- 文字
	if self:IsTag( v, TAG_TEXT ) then
		local text = string.sub(v, string.len(TAG_TEXT) + 1);
		if text ~= nil then
			str = text
		else
			str = v
		end
		
	-- 文字表
    elseif self:IsTag( v, TAG_TEXTID ) then
		local textid = tonumber(string.sub(v, string.len(TAG_TEXTID) + 1));
		if textid ~= nil then
			str = T(textid)
		else
			str = v
		end
	
	-- 道具	
	elseif self:IsTag( v, TAG_ITEMID ) then
		local textid = tonumber(string.sub(v, string.len(TAG_ITEMID) + 1));
		if textid ~= nil then
			str = item_getname( textid )
		else
			str = v
		end
	
	-- 装备
	elseif self:IsTag( v, TAG_EQUIPKIND ) then
		local textid = tonumber(string.sub(v, string.len(TAG_EQUIPKIND) + 1));
		if textid ~= nil then
			str = equip_getname( textid )
		else
			str = v
		end
		
	-- 地区	
	elseif self:IsTag( v, TAG_ZONEID ) then
		local textid = tonumber(string.sub(v, string.len(TAG_ZONEID) + 1));
		if textid ~= nil then
			str = MapZoneName( textid )
		else
			str = v
		end
		
	-- 城镇		
	elseif self:IsTag( v, TAG_TOWNID ) then
		local textid = tonumber(string.sub(v, string.len(TAG_TOWNID) + 1));
		if textid ~= nil then
			str = MapTownName( textid )
		else
			str = v
		end
	
	-- 时间天		
	elseif self:IsTag( v, TAG_TIMEDAY ) then
		local textid = tonumber(string.sub(v, string.len(TAG_TIMEDAY) + 1));
		if textid ~= nil then
			str = math.floor( textid/86400 ) .. T(156)
		else
			str = v
		end
	
	-- 时间小时		
	elseif self:IsTag( v, TAG_TIMEHOUR ) then
		local textid = tonumber(string.sub(v, string.len(TAG_TIMEHOUR) + 1));
		if textid ~= nil then
			str = math.floor( textid/3600 ) .. T(155)
		else
			str = v
		end
		
	-- 时间分钟		
	elseif self:IsTag( v, TAG_TIMEMIN ) then
		local textid = tonumber(string.sub(v, string.len(TAG_TIMEMIN) + 1));
		if textid ~= nil then
			str = math.floor( textid/60 ) .. T(154)
		else
			str = v
		end
		
	-- 时间秒		
	elseif self:IsTag( v, TAG_TIMESEC ) then
		local textid = tonumber(string.sub(v, string.len(TAG_TIMESEC) + 1));
		if textid ~= nil then
			str = textid .. T(153)
		else
			str = v
		end	
		
	-- 国家
	elseif self:IsTag( v, TAG_NATION ) then
		local textid = tonumber(string.sub(v, string.len(TAG_NATION) + 1));
		if textid ~= nil then
			str = NationEx( textid )
		else
			str = v
		end
	
	-- 奖励
	elseif self:IsTag( v, TAG_AWARD ) then
		local award = string.sub(v, string.len(TAG_AWARD) + 1);
		if award ~= nil then
			local awardList = string.split( award, '@' )
			for i= 1, #awardList, 1 do
				local awardinfo = string.split( awardList[i], ',' )
				if #awardinfo == 2 then
					local kind = tonumber(awardinfo[1]);
					local num = tonumber(awardinfo[2]);
					local sprite, color, name = AwardInfo( kind )
					str = str .. name.."x"..num.." "
				end
			end
		else
			str = v
		end
		
	-- 血战据点
	elseif self:IsTag( v, TAG_KINGWAR ) then
		local textid = tonumber(string.sub(v, string.len(TAG_KINGWAR) + 1));
		if textid ~= nil then
			str = KingWarTownName( textid )
		else
			str = v
		end
	
	-- 武将	
	elseif self:IsTag( v, TAG_HERO ) then
		local textid = tonumber(string.sub(v, string.len(TAG_HERO) + 1));
		if textid ~= nil then
			str = HeroName( textid )
		else
			str = v
		end
	
	-- 颜色	
	elseif self:IsTag( v, TAG_COLOR ) then
		local textid = tonumber(string.sub(v, string.len(TAG_COLOR) + 1));
		if textid ~= nil then
			str = NameColorStr( textid )
		else
			str = v
		end
		
	-- 官职	
	elseif self:IsTag( v, TAG_OFFICIAL ) then
		local textid = tonumber(string.sub(v, string.len(TAG_OFFICIAL) + 1));
		if textid ~= nil then
			str = OfficialName( textid )
		else
			str = v
		end
			
	-- 日期	
	elseif self:IsTag( v, TAG_DATE ) then
		local textid = tonumber(string.sub(v, string.len(TAG_DATE) + 1));
		if textid ~= nil then
			str =  getTimeStringByInterval( textid )
		else
			str = v
		end		
	
	else
		str = v;
	end
	return str
end


-- 全局
G_Mail = nil;
function GetMail()
    if G_Mail == nil then
        G_Mail = Mail.new();
    end
    return G_Mail;
end
