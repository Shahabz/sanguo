local m_Mod;
----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function GuardCallModOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
		CityGuardDlgShow();
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
function GuardCallModOnAwake( gameObject )
	m_Mod = gameObject;
	local objs = gameObject:GetComponent( "UIMod" ).relatedGameObject;
end

-- 界面初始化时调用
function GuardCallModOnStart( gameObject )
	
end

-- 界面显示时调用
function GuardCallModOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function GuardCallModOnDisable( gameObject )
	
end

-- 界面删除时调用
function GuardCallModOnDestroy( gameObject )
	
end

-- 每帧调用
function GuardCallModOnLogic( gameObject )
	
end

function GuardCallModGet()
	return m_Mod;
end
----------------------------------------
-- 自定
----------------------------------------
