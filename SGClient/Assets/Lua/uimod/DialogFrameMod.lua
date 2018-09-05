local m_Mod = {};
local m_uiTokenText = {};
local m_closefun = {};
local m_helptable = 0
local g_id = 0;
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
			HelpDlgShowBySystem( m_helptable )
			
		elseif nControlID == 2 then
			PayDlgShow()
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
	gameObject:GetComponent( "UIMod" ).id = g_id;
	local id = gameObject:GetComponent( "UIMod" ).id;
	m_Mod[id] = gameObject;
	g_id = g_id + 1;
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
	local id = gameObject:GetComponent( "UIMod" ).id;
	m_Mod[id] = nil;
	m_uiTokenText[id] = nil;
	m_closefun[id] = nil;	
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
	SetText( uiTitleTex, title );
	SetText( uiTokenText, GetPlayer().m_token );
	m_uiTokenText[id] = uiTokenText;
	m_closefun[id] = closefun;	
	m_helptable = help;
	return obj;
end

function DialogFrameModClose( gameObject )
	if gameObject == nil then
		return;
	end
	gameObject:SetActive( false );
	eye.objectPoolManager:Release( "DialogFrameMod", gameObject );
end

function DialogFrameModChangeToken()
	for k, v in pairs( m_uiTokenText ) do
		SetText( v, GetPlayer().m_token );
	end
end


