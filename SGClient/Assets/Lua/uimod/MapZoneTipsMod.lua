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
			local fade = gameObject.transform:GetComponent( typeof(UITweenFade) );
			fade.controlID = 1;
			fade:Play( false )
			
		elseif nControlID == 1 then
			local fade = gameObject.transform:GetComponent( typeof(UITweenFade) );
			fade.controlID = 0;
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

function MapZoneTipsModShow( text )
	local obj = eye.objectPoolManager:Get( "UIF_MapZoneTips" );
	obj.transform:SetParent( eye.uiManager:GetLayer( 3 ).transform );
	obj.transform.localScale = Vector3( 1, 1, 1 );
	obj.transform.localPosition = Vector3( 0, 0, 0 );
	obj.transform:Find("Text"):GetComponent( "UIText" ).text = text
end
