-- 道具操作
ITEM_ABILITY_SCRIPTID				=	25	-- 脚本调用ID

------------------------------------------------------------------ 道具的脚本调用总入口 ------------------------------------------------------------------------
function IN_UseItem( PlayerIdx, nMember, nItemIndex, nItemKind, nUseNum )
	local nID = c_item_get_ability_value( nItemKind, ITEM_ABILITY_SCRIPTID );
	if nID == 1 then
		
		
	elseif nID == 2 then
		
	elseif nID == 3 then
	
	end
	return nUseNum;
end
