local m_Mod;
----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function EquipWashModOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
		--print( "Button Clicked, nControlID:" .. nControlID );
		EquipWashDlgShow();
	elseif nType == UI_EVENT_PRESS then
		if value == 0 then
			--print( "Button Pressed Down, nControlID:" .. nControlID );
		elseif value == 1 then
			--print( "Button Pressed UP, nControlID:" .. nControlID);
		end
	end
end

-- 载入时调用
function EquipWashModOnAwake( gameObject )
	m_Mod = gameObject;
	local objs = gameObject:GetComponent( "UIMod" ).relatedGameObject;
end

-- 界面初始化时调用
function EquipWashModOnStart( gameObject )
	
end

-- 界面显示时调用
function EquipWashModOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function EquipWashModOnDisable( gameObject )
	
end

-- 界面删除时调用
function EquipWashModOnDestroy( gameObject )
	m_Mod = nil
end

-- 每帧调用
function EquipWashModOnLogic( gameObject )
	
end

function EquipWashModGet()
	return m_Mod;
end
----------------------------------------
-- 自定
----------------------------------------
