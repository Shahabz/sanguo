local m_Mod;
----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function Activity12ModOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
		--print( "Button Clicked, nControlID:" .. nControlID );
	elseif nType == UI_EVENT_PRESS then
		if value == 0 then
			--print( "Button Pressed Down, nControlID:" .. nControlID );
		elseif value == 1 then
			--print( "Button Pressed UP, nControlID:" .. nControlID);
		end
	end
end

-- 载入时调用
function Activity12ModOnAwake( gameObject )
	m_Mod = gameObject;
	local objs = gameObject:GetComponent( "UIMod" ).relatedGameObject;
end

-- 界面初始化时调用
function Activity12ModOnStart( gameObject )
	
end

-- 界面显示时调用
function Activity12ModOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function Activity12ModOnDisable( gameObject )
	
end

-- 界面删除时调用
function Activity12ModOnDestroy( gameObject )
	m_Mod = nil
end

-- 每帧调用
function Activity12ModOnLogic( gameObject )
	
end

function Activity12ModGet()
	return m_Mod;
end
----------------------------------------
-- 自定
----------------------------------------
