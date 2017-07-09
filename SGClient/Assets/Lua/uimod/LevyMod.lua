----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function LevyModOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
		LevyDlgShow();
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
function LevyModOnAwake( gameObject )
	
end

-- 界面初始化时调用
function LevyModOnStart( gameObject )
	
end

-- 界面显示时调用
function LevyModOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function LevyModOnDisable( gameObject )
	
end

-- 界面删除时调用
function LevyModOnDestroy( gameObject )
	
end

-- 每帧调用
function LevyModOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
