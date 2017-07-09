local m_Mod;
local NotifyList = {};

----------------------------------------
-- 事件
----------------------------------------
-- 所属按钮点击时调用
function NotifyModOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
		--print( "Button Clicked, nControlID:" .. nControlID );
	elseif nType == UI_EVENT_PRESS then
		if value == 0 then
			--print( "Button Pressed Down, nControlID:" .. nControlID );
		elseif value == 1 then
			--print( "Button Pressed UP, nControlID:" .. nControlID);
		end
	elseif nType == UI_EVENT_TWEENFINISH then
		if nControlID == 0 then
			local i = 1;
			while i < #NotifyList+1 do
				if NotifyList[i] == gameObject then
					table.remove( NotifyList, i )
				else
					i = i + 1
				end
			end
			gameObject:SetActive( false );
			eye.objectPoolManager:Release( "UIF_Notify", gameObject );
		end
	end
end

-- 载入时调用
function NotifyModOnAwake( gameObject )
	m_Mod = gameObject;
	local objs = gameObject:GetComponent( "UIMod" ).relatedGameObject;
end

-- 界面初始化时调用
function NotifyModOnStart( gameObject )
	
end

-- 界面显示时调用
function NotifyModOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function NotifyModOnDisable( gameObject )
	
end

-- 界面删除时调用
function NotifyModOnDestroy( gameObject )
	
end

-- 每帧调用
function NotifyModOnLogic( gameObject )
	
end

function NotifyModGet()
	return m_Mod;
end
----------------------------------------
-- 自定
----------------------------------------
function NotifyModPoolInit()
	eye.objectPoolManager:CreatePool("UIF_Notify", 5, 5, LoadPrefab("UIF_Notify"));
end

function NotifyModPoolShow( text )
	local obj = eye.objectPoolManager:Get( "UIF_Notify" );
	obj.transform:SetParent( eye.uiManager:GetLayer( 3 ).transform );
	obj.transform.localScale = Vector3( 1, 1, 1 );
	obj.transform.localPosition = Vector3( 0, 0, 0 );
	obj.transform:Find("Text"):GetComponent( "UIText" ).text = text
	
	-- 上移
	for i = 1, #NotifyList, 1 do
		local unit = NotifyList[i];
		if unit then
			local unitTween = NotifyList[i].transform:GetComponent( "UITweenMove" );
			local unitRect = NotifyList[i].transform:GetComponent( "RectTransform" );
			unitTween.from = Vector2.New( unitRect.localPosition.x, unitRect.localPosition.y )
			unitTween.to = Vector2.New( unitRect.localPosition.x, unitRect.localPosition.y + unitRect.sizeDelta.y )
			unitTween:Play(true);
		end
	end
	table.insert( NotifyList, obj );
end

function Notify( text )
	NotifyModPoolShow( text )
end
function notify( text )
	NotifyModPoolShow( text )
end
