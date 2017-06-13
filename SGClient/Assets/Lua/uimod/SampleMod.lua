----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function SampleModOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
		print( "Button Clicked, nControlID:" .. nControlID );
	elseif nType == UI_EVENT_PRESS then
		if value == 0 then
			print( "Button Pressed Down, nControlID:" .. nControlID );
		elseif value == 1 then
			print( "Button Pressed UP, nControlID:" .. nControlID);
		end
	end
end

-- 载入时调用
function SampleModOnAwake( gameObject )	
	local objs = gameObject:GetComponent( "UIMod" ).relatedGameObject;
end

-- 界面初始化时调用
function SampleModOnStart( gameObject )
	
end

-- 界面显示时调用
function SampleModOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function SampleModOnDisable( gameObject )
	
end

-- 界面删除时调用
function SampleModOnDestroy( gameObject )
	
end

-- 每帧调用
function SampleModOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
