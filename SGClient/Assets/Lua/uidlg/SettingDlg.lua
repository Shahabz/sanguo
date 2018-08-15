-- 界面
local m_Dlg = nil;
--------------------------------------------------------------
local m_uiCloseBtn = nil; --UnityEngine.GameObject
local m_uiOptionBtn = nil; --UnityEngine.GameObject
local m_uiSignatureBtn = nil; --UnityEngine.GameObject
local m_uiChatBubbleBtn = nil; --UnityEngine.GameObject
local m_uiExchangeBtn = nil; --UnityEngine.GameObject
local m_uiTitleName = nil; --UnityEngine.GameObject
local m_uiOption = nil; --UnityEngine.GameObject
local m_uiSignature = nil; --UnityEngine.GameObject
local m_uiChatBubble = nil; --UnityEngine.GameObject
local m_uiExchange = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiContentBubble = nil; --UnityEngine.GameObject
local m_uiUIP_Setting_Title = nil; --UnityEngine.GameObject
local m_uiUIP_Setting_Option = nil; --UnityEngine.GameObject
local m_uiUIP_Setting_Bubble = nil; --UnityEngine.GameObject
local m_uiScroll = nil; --UnityEngine.GameObject
local m_uiSignatureText = nil; --UnityEngine.GameObject
local m_uiExchange = nil; --UnityEngine.GameObject

m_Setting_state = 0 ; --0设置选项界面取消按钮直接关闭界面，1返回设置主界面

--游戏设置选项
local m_SettingItems = {
{name="SETTING_SYSTEMSETTING", 		type = 1, textid = 1695, uiObj = nil }, 
{name="SETTING_BGM",				type = 2, textid = 1696,  default = 1, controlid=101, uiObj = nil }, 
{name="SETTING_AUDIO",				type = 2, textid = 1697,  default = 1, controlid=102, uiObj = nil }, 
{name="SETTING_NIGHTPUSH",			type = 2, textid = 1698,  default = 0, controlid=103, uiObj = nil }, 
{name="SETTING_GAMESETTING",		type = 1, textid = 1699, uiObj = nil }, 
{name="SETTING_AUTOSUPPLY",			type = 2, textid = 1700,  default = 1, controlid=104, uiObj = nil }, 
{name="SETTING_PUSHSETTING",		type = 1, textid = 1701, uiObj = nil }, 
{name="SETTING_PUSH_TRAIN",			type = 2, textid = 1702,  default = 1, controlid=105, uiObj = nil }, 
--{name="SETTING_PUSH_ENEMYATTACK",	type = 2, textid = 1703,  default = 1, controlid=106, uiObj = nil},
--{name="SETTING_PUSH_ENEMYDETECT",	type = 2, textid = 1704,  default = 1, controlid=107, uiObj = nil}, 
--{name="SETTING_PUSH_KINGWARSTART",	type = 2, textid = 1705,  default = 1, controlid=108, uiObj = nil}, 
{name="SETTING_PUSH_BODYFULL",		type = 2, textid = 1706,  default = 1, controlid=109, uiObj = nil}, 
{name="SETTING_PUSH_EQUIPCOMPLETE",	type = 2, textid = 1707,  default = 1, controlid=110, uiObj = nil},
{name="SETTING_PUSH_TECHCOMPLETE",	type = 2, textid = 1708,  default = 1, controlid=111, uiObj = nil}, 
{name="SETTING_PUSH_BUILDCOMPLETE",	type = 2, textid = 1709,  default = 1, controlid=112, uiObj = nil}, 
{name="SETTING_PUSH_MATERIALCOMPLETE",type = 2, textid = 1710,  default = 1, controlid=113, uiObj = nil}, 
{name="SETTING_PUSH_HEROWASHFULL",	type = 2, textid = 1711,  default = 1, controlid=114, uiObj = nil},
{name="SETTING_PUSH_WISHING",	type = 2, textid = 1712,  default = 1, controlid=115, uiObj = nil},
{name="SETTING_PUSH_GATHERCOMPLETE",type = 2, textid = 1713,  default = 1, controlid=116, uiObj = nil},
{name="SETTING_PUSH_LEVYFULL",		type = 2, textid = 1714,  default = 1, controlid=117, uiObj = nil},
{name="SETTING_PUSH_GUARD",	type = 2, textid = 1715,  default = 1, controlid=118, uiObj = nil}
}

--游戏默认聊天气泡



-- 打开界面
function SettingDlgOpen()
	m_Dlg = eye.uiManager:Open( "SettingDlg" );
end

-- 隐藏界面
function SettingDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "SettingDlg" );
end

-- 删除界面
function SettingDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function SettingDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
		if nControlID == -2 then
			SettingDlgSignatureHide()
        elseif nControlID == -1 then
            SettingDlgClose();
		elseif nControlID == 1 then			--选项按钮
			SettingDlgOptionShow()	
		elseif nControlID == 2 then			--个性签名按钮
			SettingDlgSignatureShow()	
		elseif nControlID == 3 then			--聊天气泡按钮
			SettingDlgBubbleShow()	
		elseif nControlID == 4 then			--兑换按钮
			SettingDlgExchangeShow()	
		elseif nControlID == 5 then			--设置选项界面返回按钮
			if m_Setting_state == 0 then
				SettingDlgClose()
			else
				SettingDlgShow()
			end
		elseif nControlID == 6 then			--签名、气泡、兑换子界面关闭按钮
			SettingDlgShow()	
			
		elseif nControlID == 7 then			--签名界面确定按钮
			SettingDlgSendSignature()
				
		elseif nControlID == 8 then			--兑换界面粘贴按钮
		
		elseif nControlID == 9 then			--兑换界面兑换按钮
			SettingDlgSendExchangeCode()
			
		elseif nControlID > 100 and nControlID < 200 then			--设置选项
			SettingDlgOptionSelect( nControlID )							
        end
		
	elseif nType == UI_EVENT_INPUTVALUECHANGED then
		if nControlID == 0 then
			local text = m_uiSignature.transform:Find("Edit/Input"):GetComponent( "UIInputField" ).text;
			if text == "" then
				SetTrue( m_uiSignature.transform:Find("Edit/Input/Hint") )
			else
				SetFalse( m_uiSignature.transform:Find("Edit/Input/Hint") )
			end
		elseif nControlID == 1 then
			local text = m_uiExchange.transform:Find("Edit/Input"):GetComponent( "UIInputField" ).text;
			if text == "" then
				SetTrue( m_uiExchange.transform:Find("Edit/Input/Hint") )
			else
				SetFalse( m_uiExchange.transform:Find("Edit/Input/Hint") )
			end
		end
	end
end

-- 载入时调用
function SettingDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiCloseBtn = objs[0];
	m_uiOptionBtn = objs[1];
	m_uiSignatureBtn = objs[2];
	m_uiChatBubbleBtn = objs[3];
	m_uiExchangeBtn = objs[4];
	m_uiTitleName = objs[5];
	m_uiOption = objs[6];
	m_uiSignature = objs[7];
	m_uiChatBubble = objs[8];
	m_uiExchange = objs[9];
	m_uiContent = objs[10];
	m_uiContentBubble = objs[11];
	m_uiUIP_Setting_Title = objs[12];
	m_uiUIP_Setting_Option = objs[13];
	m_uiUIP_Setting_Bubble = objs[14];
	m_uiScroll = objs[15];
	m_uiSignatureText = objs[16];
	m_uiExchange = objs[17];

	-- 对象池
	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );
	m_ObjectPool:CreatePool("UIP_Setting_Title", 2, 2, m_uiUIP_Setting_Title);
	m_ObjectPool:CreatePool("UIP_Setting_Option",2, 2, m_uiUIP_Setting_Option);
	m_ObjectPool:CreatePool("UIP_Setting_Bubble", 2, 2, m_uiUIP_Setting_Bubble);
	
end

-- 界面初始化时调用
function SettingDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function SettingDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function SettingDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function SettingDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function SettingDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function SettingDlgShow()
	SettingDlgOpen()
	SetFalse( m_uiOption )
	SetFalse( m_uiSignature )	
	SetFalse( m_uiChatBubble )		
	SetFalse( m_uiExchange )
	SettingDlgSetTrueBtns()
	SetText(m_uiTitleName,T(561))
end

function SettingDlgOptionShow()
	SettingDlgOpen()
	SettingDlgOptionClear()
	SettingDlgTitleClear()
	SetTrue( m_uiOption )
		
	for k,v in pairs(m_SettingItems) do
		-- 创建标题
		if v.type == 1 then
			local uiObj = m_ObjectPool:Get( "UIP_Setting_Title" );
			uiObj.transform:SetParent( m_uiContent.transform );
			SetText( uiObj.transform:Find("TitleText"), T(v.textid) )
			v.uiObj = uiObj;
			
		-- 创建内容	
		elseif v.type == 2 then
			local uiObj = m_ObjectPool:Get( "UIP_Setting_Option" );
			uiObj.transform:SetParent( m_uiContent.transform );
			uiObj.transform:GetComponent( typeof(UIButton) ).controlID = v.controlid
					
			local onORoff = GameManager.ini( v.name, v.default )
			if tonumber(onORoff) == 0 then -- 关闭
				SetText( uiObj.transform:Find("OptionText"), T(v.textid), Hex2Color(0x547688FF) )
				SetTrue( uiObj.transform:Find("OptionBackClose") )
				SetTrue( uiObj.transform:Find("OptionCheckClose") )
				SetFalse( uiObj.transform:Find("OptionCheckOpen") )
				SetFalse( uiObj.transform:Find("OptionBackOpen") )
			else
				SetText( uiObj.transform:Find("OptionText"), T(v.textid), Hex2Color(0xBBDDF3FF) )
				SetTrue( uiObj.transform:Find("OptionCheckOpen") )
				SetTrue( uiObj.transform:Find("OptionBackOpen") )
				SetFalse( uiObj.transform:Find("OptionCheckClose") )
				SetFalse( uiObj.transform:Find("OptionBackClose") )
			end
			v.uiObj = uiObj
		end
		
	end
	--m_uiScroll.transform:GetComponent("UIScrollRect"):ScrollToTop();
end


function SettingDlgOptionClear()
	local objs = {};
	for i = 0 ,m_uiContent.transform.childCount - 1 do
			table.insert( objs, m_uiContent.transform:GetChild(i).gameObject )
    end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "UIP_Setting_Option(Clone)" then
			m_ObjectPool:Release( "UIP_Setting_Option", obj );
		end
    end
end

function SettingDlgTitleClear()
	local objs = {};
	for i = 0 ,m_uiContent.transform.childCount - 1 do
			table.insert( objs, m_uiContent.transform:GetChild(i).gameObject )
    end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "UIP_Setting_Title(Clone)" then
			m_ObjectPool:Release( "UIP_Setting_Title", obj );
		end
    end
end

function SettingDlgSignatureShow()
	SettingDlgHideBtns()
	SetTrue(m_uiSignature)
	SetText(m_uiTitleName,T(1716))
end

function SettingDlgSignatureHide()
	SettingDlgSetTrueBtns()
	SetFalse(m_uiSignature)
end

function SettingDlgBubbleShow()
	SettingDlgHideBtns()
	SetTrue(m_uiChatBubble)
	SetText(m_uiTitleName,T(1718))
end

function SettingDlgExchangeShow()
	SettingDlgHideBtns()
	SetTrue(m_uiExchange)
end

function SettingDlgOptionSelect( nControlID )
	for k,v in pairs(m_SettingItems) do
		if v.controlid == nControlID then
			local onORoff = GameManager.ini( v.name, v.default )
			if tonumber(onORoff) == 0 then -- 如果为关闭，则开启
				GameManager.writeini( v.name, 1 )
				SetText( v.uiObj.transform:Find("OptionText"), T(v.textid), Hex2Color(0xBBDDF3FF) )
				SetTrue( v.uiObj.transform:Find("OptionCheckOpen") )
				SetTrue( v.uiObj.transform:Find("OptionBackOpen") )
				SetFalse( v.uiObj.transform:Find("OptionCheckClose") )
				SetFalse( v.uiObj.transform:Find("OptionBackClose") )
				
				-- 背景音乐
				if v.name == "SETTING_BGM" then
					eye.audioManager:SetAudioSwitch( 2, true )
					
				-- 音效
				elseif v.name == "SETTING_AUDIO" then
					eye.audioManager:SetAudioSwitch( 1, true )
					eye.audioManager:SetAudioSwitch( 3, true )
					eye.audioManager:SetAudioSwitch( 4, true )
					
				-- 自动补兵
				elseif v.name == "SETTING_AUTOSUPPLY" then
					system_askinfo( ASKINFO_ACTOR, "", 6, CITY_FUNCTION_BATTLE_ADDHP, 1 );
				end
			else
				GameManager.writeini( v.name, 0 )
				SetText( v.uiObj.transform:Find("OptionText"), T(v.textid), Hex2Color(0x547688FF) )
				SetTrue( v.uiObj.transform:Find("OptionBackClose") )
				SetTrue( v.uiObj.transform:Find("OptionCheckClose") )
				SetFalse( v.uiObj.transform:Find("OptionCheckOpen") )
				SetFalse( v.uiObj.transform:Find("OptionBackOpen") )
				
				-- 背景音乐
				if v.name == "SETTING_BGM" then
					eye.audioManager:SetAudioSwitch( 2, false )
					
				-- 音效
				elseif v.name == "SETTING_AUDIO" then
					eye.audioManager:SetAudioSwitch( 1, false )
					eye.audioManager:SetAudioSwitch( 3, false )
					eye.audioManager:SetAudioSwitch( 4, false )
					
				-- 自动补兵
				elseif v.name == "SETTING_AUTOSUPPLY" then
					system_askinfo( ASKINFO_ACTOR, "", 6, CITY_FUNCTION_BATTLE_ADDHP, 0 );
				end
			end
			break
		end
	end
end

function SettingDlgHideBtns()
	SetFalse(m_uiCloseBtn)
	SetFalse(m_uiOptionBtn)
	SetFalse(m_uiSignatureBtn)
	--SetFalse(m_uiChatBubbleBtn)
	SetFalse(m_uiExchangeBtn)
end

function SettingDlgSetTrueBtns()
	SetTrue(m_uiCloseBtn)
	SetTrue(m_uiOptionBtn)
	SetTrue(m_uiSignatureBtn)
	--SetTrue(m_uiChatBubbleBtn)
	SetTrue(m_uiExchangeBtn)
end

-- 发送签名
function SettingDlgSendSignature()	
	local signature = m_uiSignatureText.transform:GetComponent( typeof(UIText) ).text;
	local len = string.len( signature )
	--非法检查
	if len == 0  then
		pop(T(1730))
		return;
	end
	if string.checksign( signature ) == false then
		pop(T(788))
		return
	end
	if Utils.MaskWordCheck( signature ) == false then
		pop(T(1732))
		return;
	end
	system_askinfo( ASKINFO_CHANGESIGN, signature, 0 );
	SettingDlgClose()
	pop( T(1731) )
end

-- 发送兑换码
function SettingDlgSendExchangeCode()	
	local cdkey =  m_uiExchange.transform:Find("Edit/Input"):GetComponent( "UIInputField" ).text;
	local len = string.len( cdkey )
	--非法检查
	if len < 6 then
		pop(T(1735))
		return
	end
	system_askinfo( ASKINFO_CDKEY, cdkey, 0 );
	SettingDlgShow()
end

-- 初始化
function SettingInit()
	for k,v in pairs(m_SettingItems) do
		if v.type == 2 then
			local onORoff = GameManager.ini( v.name, "" )
			if onORoff == "" then
				GameManager.writeini( v.name, v.default )
				onORoff = v.default;
			end
			
			if tonumber(onORoff) == 0 then -- 关闭
				-- 背景音乐
				if v.name == "SETTING_BGM" then
					eye.audioManager:SetAudioSwitch( 2, false )
					
				-- 音效
				elseif v.name == "SETTING_AUDIO" then
					eye.audioManager:SetAudioSwitch( 1, false )
					eye.audioManager:SetAudioSwitch( 3, false )
					eye.audioManager:SetAudioSwitch( 4, false )
				end
			else
				-- 背景音乐
				if v.name == "SETTING_BGM" then
					eye.audioManager:SetAudioSwitch( 2, true )
					
				-- 音效
				elseif v.name == "SETTING_AUDIO" then
					eye.audioManager:SetAudioSwitch( 1, true )
					eye.audioManager:SetAudioSwitch( 3, true )
					eye.audioManager:SetAudioSwitch( 4, true )
				end
			end
		end
	end
end