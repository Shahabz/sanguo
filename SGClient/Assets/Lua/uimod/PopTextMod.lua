----------------------------------------
-- 事件
----------------------------------------
local PopTextList = {};

-- 所属按钮点击时调用
function PopTextModOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then

	elseif nType == UI_EVENT_PRESS then
		if value == 0 then

		elseif value == 1 then

		end
	elseif nType == UI_EVENT_TWEENFINISH then
		if nControlID == 0 then
			local i = 1;
			while i < #PopTextList+1 do
				if PopTextList[i] == gameObject then
					table.remove( PopTextList, i )
				else
					i = i + 1
				end
			end
			gameObject:SetActive( false );
			eye.objectPoolManager:Release( "UIF_PopText", gameObject );
		end
	end
end

-- 载入时调用
function PopTextModOnAwake( gameObject )
	
end

-- 界面初始化时调用
function PopTextModOnStart( gameObject )
	
end

-- 界面显示时调用
function PopTextModOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function PopTextModOnDisable( gameObject )
	
end

-- 界面删除时调用
function PopTextModOnDestroy( gameObject )
	
end

-- 每帧调用
function PopTextModOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function PopTextModPoolInit()
	eye.objectPoolManager:CreatePool("UIF_PopText", 10, 10, LoadPrefab("UIF_PopText"));
end

function PopTextModPoolShow( text )
	local obj = eye.objectPoolManager:Get( "UIF_PopText" );
	obj.transform:SetParent( eye.uiManager:GetLayer( 3 ).transform );
	obj.transform.localScale = Vector3( 1, 1, 1 );
	obj.transform.localPosition = Vector3( 0, 0, 0 );
	obj.transform:Find("Text"):GetComponent( "UIText" ).text = text
	
	-- 上移
	for i = 1, #PopTextList, 1 do
		local unit = PopTextList[i];
		if unit then
			local unitTween = PopTextList[i].transform:GetComponent( "UITweenMove" );
			local unitRect = PopTextList[i].transform:GetComponent( "RectTransform" );
			unitRect.localPosition = Vector3( unitRect.localPosition.x, unitRect.localPosition.y + 30, 0 );
			unitTween.from = Vector2.New( unitRect.localPosition.x, unitRect.localPosition.y )
			unitTween.to = Vector2.New( unitRect.localPosition.x, unitRect.localPosition.y + unitRect.sizeDelta.y )
			unitTween:Play(true);
		end
	end
	
	local unitTween = obj.transform:GetComponent( "UITweenMove" );
	local unitRect = obj.transform:GetComponent( "RectTransform" );
	unitTween.from = Vector2.New( unitRect.localPosition.x, unitRect.localPosition.y )
	unitTween.to = Vector2.New( unitRect.localPosition.x, unitRect.localPosition.y + unitRect.sizeDelta.y )
	unitTween:Play(true);
	table.insert( PopTextList, obj );
end

function pop( text )
	PopTextModPoolShow( text )
end