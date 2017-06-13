-- 角色操作

-- 角色已经进入
function IN_OnActorIn( PlayerIdx )
	
end

-- 收取邮件附件内容
function IN_OnMailReadAttach( PlayerIdx, szAttach )
    local splits = string.split(szAttach,'@');
    for k, v in  pairs(splits) do
        local subSplits = string.split( v,',');
        if #subSplits == 2 then
			local kind = tonumber( subSplits[1] );
			local num = tonumber( subSplits[2] );
            c_award_getaward(PlayerIdx,kind,num,-1,PATH_MAIL_ATTACH);
            
        else
            -- 道具内容出现错误
            
        end
    end

    return 0;
end
