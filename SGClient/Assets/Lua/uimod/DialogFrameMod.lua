local m_Mod = {};
local m_closefun = {};
----------------------------------------
-- 事件
----------------------------------------
-- 所属按钮点击时调用
function DialogFrameModOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
		local id = gameObject:GetComponent( "UIMod" ).id;
		if nControlID == -1 then
			if m_closefun[id] then
				m_closefun[id]();
			end
		elseif nControlID == 1 then
		elseif nControlID == 2 then
		end
	elseif nType == UI_EVENT_PRESS then
		if value == 0 then
			--print( "Button Pressed Down, nControlID:" .. nControlID );
		elseif value == 1 then
			--print( "Button Pressed UP, nControlID:" .. nControlID);
		end
	end
end

-- 载入时调用
function DialogFrameModOnAwake( gameObject )
	local id = gameObject:GetComponent( "UIMod" ).id;
	m_Mod[id] = gameObject;
end

-- 界面初始化时调用
function DialogFrameModOnStart( gameObject )
	
end

-- 界面显示时调用
function DialogFrameModOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function DialogFrameModOnDisable( gameObject )
	
end

-- 界面删除时调用
function DialogFrameModOnDestroy( gameObject )
	
end

-- 每帧调用
function DialogFrameModOnLogic( gameObject )
	
end

function DialogFrameModGet()
	return m_Mod;
end
----------------------------------------
-- 自定
----------------------------------------
function DialogFrameModPoolInit()
	eye.objectPoolManager:CreatePool("DialogFrameMod", 5, 5, LoadPrefab("DialogFrameMod"));
end

function DialogFrameModOpen( Dlg, title, help, closefun )
	local obj = nil;
	obj = Dlg.transform:Find("DialogFrameMod");
	if obj == nil then
		obj = eye.objectPoolManager:Get( "DialogFrameMod" );
		obj.transform:SetParent( Dlg.transform );
	end
	obj.transform.localScale = Vector3( 1, 1, 1 );
	obj.transform.localPosition = Vector3( 0, 0, 0 );
	obj.transform.sizeDelta = Vector2( 0, 0 );
	obj.gameObject:SetActive( true );
	
	local uimod = obj:GetComponent( "UIMod" );
	local id = uimod.id;
	local objs = uimod.relatedGameObject;
	local uiTitleTex = objs[0];
	local uiTokenText = objs[1];
	uiTitleTex.transform:GetComponent( "UIText" ).text = title;
	uiTokenText.transform:GetComponent( "UIText" ).text = GetPlayer().m_token;
	m_closefun[id] = closefun;	
	return obj;
end

function DialogFrameModClose( gameObject )
	gameObject:SetActive( false );
	eye.objectPoolManager:Release( "DialogFrameMod", gameObject );
end
