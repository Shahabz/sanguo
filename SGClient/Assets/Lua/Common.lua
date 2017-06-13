--输出日志--
function log(str)
    Utils.Log(str);
end

--打印字符串--
function print(str) 
	Utils.Log(str);
end

--错误日志--
function error(str) 
	Utils.LogError(str);
end

--警告日志--
function warn(str) 
	Utils.LogWarning(str);
end

-- 判断文件夹
function io.exists(path)
    local file = io.open(path, "r")
    if file then
        io.close(file)
        return true
    end
    return false
end

--local pathinfo  = io.pathinfo("/var/app/test/abc.png")
-- 结果:
-- pathinfo.dirname  = "/var/app/test/"
-- pathinfo.filename = "abc.png"
-- pathinfo.basename = "abc"
-- pathinfo.extname  = ".png"
function io.pathinfo(path)
    local pos = string.len(path)
    local extpos = pos + 1
    while pos > 0 do
        local b = string.byte(path, pos)
        if b == 46 then -- 46 = char "."
            extpos = pos
        elseif b == 47 then -- 47 = char "/"
            break
        end
        pos = pos - 1
    end

    local dirname = string.sub(path, 1, pos)
    local filename = string.sub(path, pos + 1)
    extpos = extpos - pos
    local basename = string.sub(filename, 1, extpos - 1)
    local extname = string.sub(filename, extpos)
    return {
        dirname = dirname,
        filename = filename,
        basename = basename,
        extname = extname
    }
end

--------------------------------
-- 返回指定文件的大小，如果失败返回 false
-- @function [parent=#io] filesize
-- @param string path 文件完全路径
function io.filesize(path)
    local size = false
    local file = io.open(path, "r")
    if file then
        local current = file:seek()
        size = file:seek("end")
        file:seek("set", current)
        io.close(file)
    end
    return size
end

-------------------------------
-- 计算表格包含的字段数量
-- @function [parent=#table] nums
-- @param table t 要检查的表格
-- @return integer#integer 
function table.nums(t)
    local count = 0
    for k, v in pairs(t) do
        count = count + 1
    end
    return count
end

--------------------------------
-- 返回指定表格中的所有键
-- @function [parent=#table] keys
-- @param table hashtable 要检查的表格
-- @return table#table 
--local hashtable = {a = 1, b = 2, c = 3}
--local keys = table.keys(hashtable)
-- keys = {"a", "b", "c"}
function table.keys(hashtable)
    local keys = {}
    for k, v in pairs(hashtable) do
        keys[#keys + 1] = k
    end
    return keys
end

--------------------------------
-- 返回指定表格中的所有值
-- @function [parent=#table] values
-- @param table hashtable 要检查的表格
-- @return table#table 
--local hashtable = {a = 1, b = 2, c = 3}
--local values = table.values(hashtable)
-- values = {1, 2, 3}
function table.values(hashtable)
    local values = {}
    for k, v in pairs(hashtable) do
        values[#values + 1] = v
    end
    return values
end

-- table迭代器 按key顺序
function pairsByKeys(t)      
    local a = {}      
    for n in pairs(t) do          
        a[#a+1] = n      
    end      
    table.sort(a)      
    local i = 0      
    return function()          
		i = i + 1          
		return a[i], t[a[i]]      
    end  
end

--查找对象--
function find(str)
	return GameObject.Find(str);
end

function destroy(obj)
	GameObject.Destroy(obj);
end

function newobject(prefab)
	return GameObject.Instantiate(prefab);
end

--创建面板--
function createPanel(name)
	ioo.panelManager:CreatePanel(name);
end

function child(str)
	return transform:FindChild(str);
end

function subGet(childNode, typeName)		
	return child(childNode):GetComponent(typeName);
end

function findPanel(str) 
	local obj = find(str);
	if obj == nil then
		error(str.." is null");
		return nil;
	end
	return obj:GetComponent("BaseLua");
end

function handler(obj, method)
    return function(...)
        return method(obj, ...)
    end
end

function clone(object)
    local lookup_table = {}
    local function _copy(object)
        if type(object) ~= "table" then
            return object
        elseif lookup_table[object] then
            return lookup_table[object]
        end
        local new_table = {}
        lookup_table[object] = new_table
        for key, value in pairs(object) do
            new_table[_copy(key)] = _copy(value)
        end
        return setmetatable(new_table, getmetatable(object))
    end
    return _copy(object)
end

function class(classname, super)
    local superType = type(super)
    local cls

    if superType ~= "function" and superType ~= "table" then
        superType = nil
        super = nil
    end

    if superType == "function" or (super and super.__ctype == 1) then
        -- inherited from native C++ Object
        cls = {}

        if superType == "table" then
            -- copy fields from super
            for k,v in pairs(super) do cls[k] = v end
            cls.__create = super.__create
            cls.super    = super
        else
            cls.__create = super
            cls.ctor = function() end
        end

        cls.__cname = classname
        cls.__ctype = 1

        function cls.new(...)
            local instance = cls.__create(...)
            -- copy fields from class to native object
            for k,v in pairs(cls) do instance[k] = v end
            instance.class = cls
            instance:ctor(...)
            return instance
        end

    else
        -- inherited from Lua Object
        if super then
            cls = {}
            setmetatable(cls, {__index = super})
            cls.super = super
        else
            cls = {ctor = function() end}
        end

        cls.__cname = classname
        cls.__ctype = 2 -- lua
        cls.__index = cls

        function cls.new(...)
            local instance = setmetatable({}, cls)
            instance.class = cls
            instance:ctor(...)
            return instance
        end
    end

    return cls
end

-- 分割字符串
function string.split(input, delimiter)
    input = tostring(input)
    delimiter = tostring(delimiter)
    if (delimiter=='') then return false end
    local pos,arr = 0, {}
    -- for each divider found
    for st,sp in function() return string.find(input, delimiter, pos, true) end do
        table.insert(arr, string.sub(input, pos, st - 1))
        pos = sp + 1
    end
    table.insert(arr, string.sub(input, pos))
    return arr
end

--[[ 计算 UTF8 字符串的长度，每一个中文算一个字符 local input = "你好World" print(string.utf8len(input)) 输出 7 --]]
function string.utf8len(input)
    local len  = string.len(input)
    local left = len
    local cnt  = 0
    local arr  = {0, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc}
    while left ~= 0 do
        local tmp = string.byte(input, -left)
        local i   = #arr
        while arr[i] do
            if tmp >= arr[i] then
                left = left - i
                break
            end
            i = i - 1
        end
        cnt = cnt + 1
    end
    return cnt
end

--[[ 检查并尝试转换为数值，如果无法转换则返回 0-- ]]
function checknumber(value, base)
    return tonumber(value, base) or 0
end

--[[ 将数值格式化为包含千分位分隔符的字符串 print(string.formatnumberthousands(1924235)) 输出 1,924,235 --]]
function string.formatnumberthousands(num,unformat)
    if type(num) ~= "number" then
        return num;
    end

    if unformat ~= true then
        if num >= 1000000 then
            num = num / 1000000;
            return string.format( "%.1f", num ) .. "M";
        elseif num >= 1000 then
            num = num / 1000;
            return string.format( "%.1f", num ) .. "K";
        end
    end

    local formatted = tostring(checknumber(num))
    local k
    while true do
        formatted, k = string.gsub(formatted, "^(-?%d+)(%d%d%d)", '%1,%2')
        if k == 0 then break end
    end
    return formatted
end

-- 检查是不是英文or数字
function string.checkenglish( str )
    local lenInByte = #str
    local width = 0

    for i = 1, lenInByte, 1 do
        local curByte = string.byte(str, i);
        if curByte >= 48 and curByte <= 57 then
        elseif curByte >= 65 and curByte <= 90 then
        elseif curByte >= 97 and curByte <= 122 then
        else
            return false;
        end
    end

    return true;
end


-- 检查符号
function string.checksign( str )
    local lenInByte = #str
    local width = 0

    for i = 1, lenInByte, 1 do
        local curByte = string.byte(str, i);
        -- 控制符
        if curByte <= 31 then
            return false;
        -- 符号1
        elseif curByte >= 33 and curByte <= 47 then
            return false;
        -- 符号2
        elseif curByte >= 58 and curByte <= 64 then
            return false;
        -- 符号3
        elseif curByte >= 91 and curByte <= 96 then
            return false;
        -- 符号4
        elseif curByte >= 123 and curByte <= 127 then
            return false;
        end
    end

    return true;
end

--------------------------- 队列 ---------------------------------
Queue = {}
function Queue.new()
    return {first = 0, last = -1}
end

function Queue.pushFront(queue, value)
    local first = queue.first - 1;
    queue.first = first
    queue[first] = value
end

function Queue.pushBack(queue, value)
    local last = queue.last + 1     queue.last = last
    queue[last] = value
end

function Queue.popFirst(queue)
    local first = queue.first
    if first > queue.last then 
		--error("queue is empty")
		return nil;
	end
    local value = queue[first]
    queue[first] = nil
    queue.first = first + 1    
	return value
end

function Queue.popLast(queue)
    local last = queue.last
    if queue.first > last then 
		--error("queue is empty") 
		return nil;
	end
    local value = queue[last]
    queue[last] = nil
    queue.last = last - 1     
	return value
end

function Queue.has( queue )
    if queue.first > queue.last then 
		--error("queue is empty") 
		return false;
	end
	return true
end

function Queue.clear(queue)
    queue.first = 0;
	queue.last = -1;
end

--[[queueOne = Queue.new()
Queue.pushFront(queueOne,"Hello")
Queue.pushBack(queueOne,"World")
print(Queue.popFirst(queueOne))--]]

-- 指定一个日期时间字符串，返回与之对应的东八区（服务器时区）时间戳
-- sTime: 格式 "2013-07-02 20:00:00"
function getIntervalByTimeString( sTime )

	local t = string.split( sTime, " " );
	local tDate = string.split( t[1], "-" );
	local tTime = string.split( t[2], ":" );

	local tt = os.time({year = tDate[1], month = tDate[2], day = tDate[3], hour = tTime[1], min = tTime[2], sec = tTime[3]});
	local ut = os.date("!*t", tt)
	local east8 = os.time(ut) + 8*60*60 -- UTC时间+8小时转为东八区北京时间
	return east8;
end
function getTimeStringByInterval(time)
    local tab = os.date("*t", time);
    local sec = tostring(tab.sec);
    local min= tostring(tab.min);
    local hour = tostring(tab.hour);
    local day = tostring(tab.day);
    local month = tostring(tab.month);
    local year = tostring(tab.year);
    if tab.sec < 10 then
        sec = '0'..sec;
    end
    if tab.min < 10 then
       min = '0'..min;
    end
    if tab.hour < 10 then
       hour = '0'..hour;
    end
    if tab.day < 10 then
       day = '0'..day;
    end
    if tab.month < 10 then
       month = '0'..month;
    end
    if tab.year < 10 then
       year = '0'..year;
    end
    return year .. "-" .. month .. "-" .. day .. " " .. hour .. ":" .. min .. ':' .. sec;
end;
function getTimeStringByNormal(time)
    local tab = os.date("!*t", time);
    local sec = tostring(tab.sec);
    local min= tostring(tab.min);
    local hour = tostring(tab.hour);
    local day = tostring(tab.day);
    local month = tostring(tab.month);
    local year = tostring(tab.year);
    if tab.sec < 10 then
        sec = '0'..sec;
    end
    if tab.min < 10 then
       min = '0'..min;
    end
    if tab.hour < 10 then
       hour = '0'..hour;
    end
    if tab.day < 10 then
       day = '0'..day;
    end
    if tab.month < 10 then
       month = '0'..month;
    end
    if tab.year < 10 then
       year = '0'..year;
    end
    return year .. "-" .. month .. "-" .. day .. " " .. hour .. ":" .. min .. ':' .. sec;
end
function getDateStringByInterval(time)
    local tab = os.date("*t", time);
    local sec = tostring(tab.sec);
    local min= tostring(tab.min);
    local hour = tostring(tab.hour);
    local day = tostring(tab.day);
    local month = tostring(tab.month);
    local year = tostring(tab.year);
    if tab.day < 10 then
       day = '0'..day;
    end
    if tab.month < 10 then
       month = '0'..month;
    end
    if tab.year < 10 then
       year = '0'..year;
    end
    return year .. "-" .. month .. "-" .. day;
end

function getHourStringByInterval(time)
    local tab = os.date("*t", time);
    local sec = tostring(tab.sec);
    local min= tostring(tab.min);
    local hour = tostring(tab.hour);
    local day = tostring(tab.day);
    local month = tostring(tab.month);
    local year = tostring(tab.year);
    if tab.day < 10 then
       day = '0'..day;
    end
    if tab.month < 10 then
       month = '0'..month;
    end
    if tab.year < 10 then
       year = '0'..year;
    end
    if tab.sec < 10 then
        sec = '0'..sec;
    end
    if tab.min < 10 then
       min = '0'..min;
    end
    if tab.hour < 10 then
       hour = '0'..hour;
    end
    return hour .. ":" .. min .. ':'..sec;
end

-- 把秒 转换成标准的 HH:MM:SS 格式
function getTimeStringBySecond(second)
    local hour = math.floor( second / 3600 );
    local min = ( math.floor( second /60) ) % 60;
    local sec = ( second % 60);
    local strHour,strMin,strSec;
    if hour < 10 then
        strHour="0"..tostring(hour);
    else
        strHour = tostring(hour);
    end
    if min < 10 then
        strMin = "0"..tostring(min);
    else
        strMin = tostring(min);
    end
    if sec < 10 then
        strSec = "0"..tostring(sec);
    else
        strSec = tostring(sec);
    end
    return  strHour..":"..strMin..":"..strSec;
end

function getIntervalByTime()
    local ut = os.date("!*t", os.time())
	local east8 = os.time(ut) + 8*60*60 -- UTC时间+8小时转为东八区北京时间
    return east8;
end

--点的函数直接照抄cocos2d的算法
cc = cc or {}
function cc.p(_x,_y)
    if nil == _y then
         return { x = _x.x, y = _x.y }
    else
         return { x = _x, y = _y }
    end
end

function cc.pAdd(pt1,pt2)
    return {x = pt1.x + pt2.x , y = pt1.y + pt2.y }
end

function cc.pSub(pt1,pt2)
    return {x = pt1.x - pt2.x , y = pt1.y - pt2.y }
end

function cc.pMul(pt1,factor)
    return { x = pt1.x * factor , y = pt1.y * factor }
end

function cc.pMidpoint(pt1,pt2)
    return { x = (pt1.x + pt2.x) / 2.0 , y = ( pt1.y + pt2.y) / 2.0 }
end

function cc.pForAngle(a)
    return { x = math.cos(a), y = math.sin(a) }
end

function cc.pGetLength(pt)
    return math.sqrt( pt.x * pt.x + pt.y * pt.y )
end

function cc.pNormalize(pt)
    local length = cc.pGetLength(pt)
    if 0 == length then
        return { x = 1.0,y = 0.0 }
    end

    return { x = pt.x / length, y = pt.y / length }
end

function cc.pCross(self,other)
    return self.x * other.y - self.y * other.x
end

function cc.pDot(self,other)
    return self.x * other.x + self.y * other.y
end

function cc.pToAngleSelf(self)
    return math.atan2(self.y, self.x)
end

function cc.pGetAngle(self,other)
    local a2 = cc.pNormalize(self)
    local b2 = cc.pNormalize(other)
    local angle = math.atan2(cc.pCross(a2, b2), cc.pDot(a2, b2) )
    if math.abs(angle) < 1.192092896e-7 then
        return 0.0
    end

    return angle
end

function cc.pGetDistance(startP,endP)
    return cc.pGetLength(cc.pSub(startP,endP))
end

function cc.pIsLineIntersect(A, B, C, D, s, t)
    if ((A.x == B.x) and (A.y == B.y)) or ((C.x == D.x) and (C.y == D.y))then
        return false, s, t
    end

    local BAx = B.x - A.x
    local BAy = B.y - A.y
    local DCx = D.x - C.x
    local DCy = D.y - C.y
    local ACx = A.x - C.x
    local ACy = A.y - C.y

    local denom = DCy * BAx - DCx * BAy
    s = DCx * ACy - DCy * ACx
    t = BAx * ACy - BAy * ACx

    if (denom == 0) then
        if (s == 0 or t == 0) then
            return true, s , t
        end

        return false, s, t
    end

    s = s / denom
    t = t / denom

    return true,s,t
end

function cc.pPerp(pt)
    return { x = -pt.y, y = pt.x }
end

function cc.RPerp(pt)
    return { x = pt.y,  y = -pt.x }
end

function cc.pProject(pt1, pt2)
    return { x = pt2.x * (cc.pDot(pt1,pt2) / cc.pDot(pt2,pt2)) , y = pt2.y * (cc.pDot(pt1,pt2) / cc.pDot(pt2,pt2)) }
end

function cc.pRotate(pt1, pt2)
    return { x = pt1.x * pt2.x - pt1.y * pt2.y, y = pt1.x * pt2.y + pt1.y * pt2.x }
end

function cc.pUnrotate(pt1, pt2)
    return { x = pt1.x * pt2.x + pt1.y * pt2.y, pt1.y * pt2.x - pt1.x * pt2.y }
end
--Calculates the square length of pt
function cc.pLengthSQ(pt)
    return cc.pDot(pt,pt)
end
--Calculates the square distance between pt1 and pt2
function cc.pDistanceSQ(pt1,pt2)
    return cc.pLengthSQ(cc.pSub(pt1,pt2))
end

function cc.pGetClampPoint(pt1,pt2,pt3)
    return { x = cc.clampf(pt1.x, pt2.x, pt3.x), y = cc.clampf(pt1.y, pt2.y, pt3.y) }
end

function cc.pFromSize(sz)
    return { x = sz.width, y = sz.height }
end

function cc.pLerp(pt1,pt2,alpha)
    return cc.pAdd(cc.pMul(pt1, 1.0 - alpha), cc.pMul(pt2,alpha) )
end

function cc.pFuzzyEqual(pt1,pt2,variance)
    if (pt1.x - variance <= pt2.x) and (pt2.x <= pt1.x + variance) and (pt1.y - variance <= pt2.y) and (pt2.y <= pt1.y + variance) then
        return true
    else
        return false
    end
end

function cc.pRotateByAngle(pt1, pt2, angle)
    return cc.pAdd(pt2, cc.pRotate( cc.pSub(pt1, pt2),cc.pForAngle(angle)))
end

function cc.pIsSegmentIntersect(pt1,pt2,pt3,pt4)
    local s,t,ret = 0,0,false
    ret,s,t =cc.pIsLineIntersect(pt1, pt2, pt3, pt4,s,t)

    if ret and  s >= 0.0 and s <= 1.0 and t >= 0.0 and t <= 0.0 then
        return true;
    end

    return false
end

function cc.pGetIntersectPoint(pt1,pt2,pt3,pt4)
    local s,t, ret = 0,0,false
    ret,s,t = cc.pIsLineIntersect(pt1,pt2,pt3,pt4,s,t)
    if ret then
        return cc.p(pt1.x + s * (pt2.x - pt1.x), pt1.y + s * (pt2.y - pt1.y))
    else
        return cc.p(0,0)
    end
end

function cc.ptInLine( point, lineStartPoint, lineEndPoint, fTolerance )
	local L, R, S;
	local bResult = false;

	L = math.sqrt( ((lineEndPoint.x - lineStartPoint.x)*(lineEndPoint.x - lineStartPoint.x) + (lineEndPoint.y - lineStartPoint.y)*(lineEndPoint.y - lineStartPoint.y)) );
	if L ~= 0 then
		R = ((lineStartPoint.y - point.y)*(lineStartPoint.y - lineEndPoint.y) - (lineStartPoint.x - point.x)*(lineEndPoint.x - lineStartPoint.x)) / (L*L);
		S = ((lineStartPoint.y - point.y)*(lineEndPoint.x - lineStartPoint.x) - (lineStartPoint.x - point.x)*(lineEndPoint.y - lineStartPoint.y)) / (L*L);
		if  R > 0 and R < 1 then
			if math.abs( math.floor(S*L) ) <= fTolerance then
				bResult = true;
			end
		end
	end

	return bResult;
end

-- 自定义随机数发生器
custom = custom or {}
custom._randseed = os.time();
function custom.randseed( seed )
	custom._randseed = seed;
end
function custom.rand( min, max )
	custom._randseed = 7 * custom._randseed % 2147483647
	return custom._randseed % (max - min) + min
end


