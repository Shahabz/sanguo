----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function MapZoneTipsModOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then

	elseif nType == UI_EVENT_PRESS then
		if value == 0 then

		elseif value == 1 then

		end
	elseif nType == UI_EVENT_TWEENFINISH then
		if nControlID == 0 then
			local uiTweens = gameObject:GetComponents( typeof(UITween) );
			uiTweens[1]:Play(true)
		elseif nControlID == 1 then
			gameObject:SetActive( false );
			eye.objectPoolManager:Release( "UIF_MapZoneTips", gameObject );
		end
	end
end

-- 载入时调用
function MapZoneTipsModOnAwake( gameObject )

end

-- 界面初始化时调用
function MapZoneTipsModOnStart( gameObject )
	
end

-- 界面显示时调用
function MapZoneTipsModOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function MapZoneTipsModOnDisable( gameObject )
	
end

-- 界面删除时调用
function MapZoneTipsModOnDestroy( gameObject )
	
end

-- 每帧调用
function MapZoneTipsModOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function MapZoneTipsModPoolInit()
	eye.objectPoolManager:CreatePool("UIF_MapZoneTips", 1, 1, LoadPrefab("UIF_MapZoneTips"));
end

function MapZoneTipsModShow( name, nation )
	local obj = eye.objectPoolManager:Get( "UIF_MapZoneTips" );
	obj.transform:SetParent( eye.uiManager:GetLayer( 3 ).transform );
	obj.transform.localScale = Vector3( 1, 1, 1 );
	obj.transform.localPosition = Vector3( 0, 260, 0 );
	
	local color = Hex2Color(MapUnitRangeColor[nation]);
	SetText( obj.transform:Find("Name"), name )
	SetText( obj.transform:Find("Nation"), NationEx( nation )..T(115), color )
	
	local uiTweens = obj:GetComponents( typeof(UITween) );
	uiTweens[0]:ToInit()
	uiTweens[0]:Play(true)
end
