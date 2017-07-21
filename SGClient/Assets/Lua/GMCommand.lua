---------------------- GM指令 ----------------------

-- 发到服务端指令
local GMCMD_SERVER = { 
[1]="test",
[2]="sc",
[3]="lua",
[4]="reload",
[5]="reset",
[6]="exp",
[7]="token",
[8]="item",
[9]="equip",
[10]="hero",
[11]="silver",
[12]="wood",
[13]="food",
[14]="iron",
[15]="body",
[16]="levy",
[17]="award",
[18]="quest",
[19]="rollmsg",
[20]="mail",
[21]="systalk";
[22]="vipexp",
}
 
-- 发到客户端本地指令
local GMCMD_CLIENT = { 
[1]="pop",
[2]="alert",
[3]="msgbox",
[4]="notify",
}

-- 检查GM指令
function GMCommandCheck( strarray )
	if strarray == nil then
		return false;
	end
	
	-- 指令
	local cmdstr = strarray[1];
	
	-- 检查是否是发送到服务端的指令
	for k, v in pairs(GMCMD_SERVER) do
		if v == cmdstr then
	        -- 4个数值参数 + 1个字符串参数
	        local cmdvalue = {0,0,0,0,""};
	        for i=2, #strarray, 1 do
		        cmdvalue[i-1] = strarray[i];
	        end

			local sendValue = {}
			sendValue.m_cmd = k;
			sendValue.m_value = {0,0,0,0};
			sendValue.m_value[1] = cmdvalue[1];
			sendValue.m_value[2] = cmdvalue[2];
			sendValue.m_value[3] = cmdvalue[3];
			sendValue.m_value[4] = cmdvalue[4];
			sendValue.m_msg = cmdvalue[5];
			sendValue.m_msg_length = string.len( sendValue.m_msg );
			netsend_gmcmd_C( sendValue )
			return true;
		end
	end
	
	-- 检查是否是发送到客户端本地指令
	for k, v in pairs(GMCMD_CLIENT) do
		if v == cmdstr then
            -- 9个数值参数 + 1个字符串参数
	        local cmdvalue = {0,0,0,0,0,0,0,0,0,""};
	        for i=2, #strarray, 1 do
		        cmdvalue[i-1] = strarray[i];
	        end

			GMCommandClientProc( cmdstr, cmdvalue[1],cmdvalue[2],cmdvalue[3],cmdvalue[4],cmdvalue[5],cmdvalue[6],cmdvalue[7],cmdvalue[8],cmdvalue[9],cmdvalue[10] );
			return true;
		end
	end   
	
	return false;
end

-- 客户端本地指令处理
function GMCommandClientProc( cmdstr, value1, value2, value3, value4, value5, value6, value7, value8, value9, msg )
	if cmdstr == "cc" then
		pop( msg )
	elseif cmdstr == "alert" then
		Alert( msg, "确定", function() end )
	
	elseif cmdstr == "msgbox" then
		MsgBox( msg, function() end )
		
	elseif cmdstr == "notify" then
		Notify( msg )
		
	end
end