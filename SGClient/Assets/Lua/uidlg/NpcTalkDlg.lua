-- 界面
local m_Dlg = nil;

local m_uiLeft = nil; --UnityEngine.GameObject
local m_uiRight = nil; --UnityEngine.GameObject
local m_uiTalk = nil; --UnityEngine.GameObject
local m_uiButton = nil; --UnityEngine.GameObject
local m_uiContinue = nil; --UnityEngine.GameObject

local m_WaitCallback = nil;
local m_WaitValue = nil;
local m_clickstep = 0
local m_ButtonShow = false;

-- 打开界面
function NpcTalkDlgOpen()
	m_Dlg = eye.uiManager:Open( "NpcTalkDlg" );
end

-- 隐藏界面
function NpcTalkDlgClose()
	if m_Dlg == nil then
		return;
	end
	if m_WaitCallback then
		m_WaitCallback( m_WaitValue );
	end
	m_WaitCallback = nil;
	m_WaitValue = nil;
	SetFalse( m_uiLeft )
	SetFalse( m_uiRight )
	eye.uiManager:Close( "NpcTalkDlg" );
end

-- 删除界面
function NpcTalkDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function NpcTalkDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
			if m_clickstep == 0 then
				-- 文字快速出现
				m_uiTalk:GetComponent( typeof(TypeWriter) ):OnFinish()
				m_clickstep = 1;
				if m_ButtonShow == true then
					SetTrue( m_uiButton )
				end
			else
				NpcTalkDlgClose();
			end
		
		elseif nControlID == 1 then
			NpcTalkDlgClose();
			
        end
	end
end

-- 载入时调用
function NpcTalkDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiLeft = objs[0];
	m_uiRight = objs[1];
	m_uiTalk = objs[2];
	m_uiButton = objs[3];
	m_uiContinue = objs[4];
end

-- 界面初始化时调用
function NpcTalkDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function NpcTalkDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function NpcTalkDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function NpcTalkDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function NpcTalkDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function NpcTalkShapeInfo( shape )
	local sprite = nil;
	local name = ""
	local left = 1;
	
	if shape == 999 then -- 玩家自己
		sprite = PlayerFaceSprite( GetPlayer().m_shape )
		name = GetPlayer().m_name
		left = 0;
	
	elseif shape == 1000 then -- 自己国家君主
		local kind = 0;
		if GetPlayer().m_nation == 1 then
			kind = 123
		elseif GetPlayer().m_nation == 2 then
			kind = 124
		elseif GetPlayer().m_nation == 3 then
			kind = 125
		end
		sprite = HeroFaceSprite( kind )
		name = HeroName( kind )
			
	elseif shape > 0 and shape < 999 then -- 武将
		sprite = HeroFaceSprite( shape )
		name = HeroName( shape )
		
	elseif shape > 1000 and shape <= 2000 then -- npc
		local kind = shape-1000;
		sprite = LoadSprite( "peopleface_"..kind );
		name = Localization.text_item(kind+3180);
			
	elseif shape > 2000 and shape <= 3000 then -- 谋士
	elseif shape > 3000 and shape <= 4000 then -- 女将
		local kind = shape-3000
		sprite = GirlFaceSprite( kind )
		name = GirlName( kind )
		
	else -- 侍女
		sprite = LoadSprite( "peopleface_0" );
		name = GetPlayer().m_maidname
		
	end
	
	return sprite, name, left
end
	
function NpcTalk( info )
	NpcTalkDlgOpen();
	SetFalse( m_uiLeft )
	SetFalse( m_uiRight )
	
	m_clickstep = 0;
	
	local shape = info[1]
	local talktext = info[2]
	local btntext = info[3]
	local callback = info[4]
	
	local sprite, name, left = NpcTalkShapeInfo( shape );
	local obj = nil
	if left == 1 then
		obj = m_uiLeft
	else
		obj = m_uiRight
	end
	
	SetTrue( obj )
	SetImage( obj.transform:Find("Shape"), sprite );
	SetText( obj.transform:Find("Name"), name );
	SetFalse( m_uiButton )
	--local uiTween = obj.transform:GetComponent("UITweenRectPosition")
	--uiTween:Play(true)
	
	if btntext ~= nil then
		SetText( m_uiButton.transform:Find("Back/Text"), btntext );
		m_ButtonShow = true;
	else
		m_ButtonShow = false;
	end
	
	SetTextWriter( m_uiTalk, talktext, function() 
		m_clickstep=1
		if m_ButtonShow == true then
			SetTrue( m_uiButton )
		end
	end )
	m_WaitCallback = callback;
end

function NpcTalkIsShow()
	if m_Dlg ~= nil then
		return IsActive( m_Dlg )
	end
	return false;
end

-- 设置等待数据
function NpcTalkWait( callback, value )
	m_WaitCallback = callback;
	m_WaitValue = value;
end
