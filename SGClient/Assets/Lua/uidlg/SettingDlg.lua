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



--游戏设置选项
local m_SettingItems = {
["SETTING_SYSTEMSETTING"]	= {  sort=1, type = 1, textid = 1695, uiObj = nil }, 
["SETTING_BGM"]	= {  sort=2, type = 2, textid = 1696,  default = 1, controlid=101, uiObj = nil }, 
["SETTING_AUDIO"] = { sort=3, type = 2, textid = 1697,  default = 1, controlid=102, uiObj = nil }, 
["SETTING_NIGHTPUSH"] = { sort=4, type = 2, textid = 1698,  default = 0, controlid=103, uiObj = nil }, 
["SETTING_GAMESETTING"] = { sort=5, type = 1, textid = 1699, uiObj = nil }, 
["SETTING_AUTOSUPPLY"] = { sort=6, type = 2, textid = 1700,  default = 0, controlid=104, uiObj = nil }, 
["SETTING_PUSHSETTING"] = { sort=7, type = 1, textid = 1701, uiObj = nil }, 
["SETTING_PUSH_CONSCRIP"] = { sort=8, type = 2, textid = 1702,  default = 1, controlid=105, uiObj = nil }, 
["SETTING_PUSH_ENEMYATTACK"] = { sort=9, type = 2, textid = 1703,  default = 1, controlid=106}, uiObj = nil,
["SETTING_PUSH_ENEMYDETECT"] = { sort=10, type = 2, textid = 1704,  default = 1, controlid=107, uiObj = nil}, 
["SETTING_PUSH_KINGWARSTART"] = { sort=11, type = 2, textid = 1705,  default = 1, controlid=108, uiObj = nil}, 
["SETTING_PUSH_POWERFULL"] = { sort=12, type = 2, textid = 1706,  default = 1, controlid=109, uiObj = nil}, 
["SETTING_PUSH_EQUIPCOMPLETE"] = { sort=13, type = 2, textid = 1707,  default = 1, controlid=110, uiObj = nil},
["SETTING_PUSH_TECHCOMPLETE"] = { sort=14, type = 2, textid = 1708,  default = 1, controlid=111, uiObj = nil}, 
["SETTING_PUSH_BUILDCOMPLETE"] = { sort=15, type = 2, textid = 1709,  default = 1, controlid=112, uiObj = nil}, 
["SETTING_PUSH_MATERIALCOMPLETE"] = { sort=16, type = 2, textid = 1710,  default = 1, controlid=113, uiObj = nil}, 
["SETTING_PUSH_HEROWASHFULL"] = { sort=17, type = 2, textid = 1711,  default = 1, controlid=114, uiObj = nil},
["SETTING_PUSH_JUBAOCOOLDOWN"] = { sort=18, type = 2, textid = 1712,  default = 1, controlid=115, uiObj = nil},
["SETTING_PUSH_COLLECTCOMPLETE"] = { sort=19, type = 2, textid = 1713,  default = 1, controlid=116, uiObj = nil},
["SETTING_PUSH_LEVYFULL"] = { sort=20, type = 2, textid = 1714,  default = 1, controlid=117, uiObj = nil},
["SETTING_PUSH_GUARDCOOLDOWN"] = { sort=21, type = 2, textid = 1715,  default = 1, controlid=118, uiObj = nil}
} 


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
        if nControlID == -1 then
            SettingDlgClose();
		elseif nControlID == 1 then			--选项按钮
			SettingDlgOptionShow()	
		elseif nControlID == 2 then			--个性签名按钮
			SettingDlgSignatureShow()	
		elseif nControlID == 3 then			--聊天气泡按钮
			SettingDlgBubbleShow()	
		elseif nControlID == 4 then			--兑换按钮
			SettingDlgExchangeShow()	
		elseif nControlID > 100 and nControlID < 200 then			--设置选项
			SettingDlgOptionSelect( nControlID )							
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
	
	-- 对象池
	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );
	m_ObjectPool:CreatePool("UIP_Setting_Title", 2, 2, m_uiUIP_Setting_Title);
	m_ObjectPool:CreatePool("UIP_Setting_Option", 2, 2, m_uiUIP_Setting_Option);
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
end

function SettingDlgOptionShow()
	SettingDlgOptionClear()
	SettingDlgHideBtns()
	SetTrue( m_uiOption )
	SetText(m_uiTitleName,T(1694))
	for k,v in pairs(m_SettingItems) do
		-- 创建标题
		if v.type == 1 then
			local uiObj = m_ObjectPool:Get( "UIP_Setting_Title" );
			uiObj.transform:SetParent( m_uiContent.transform );
			SetText( uiObj.transform:Find("TitleText"), T(v.textid) )
			uiObj.transform:SetSiblingIndex(v.sort);
			v.uiObj = uiObj;
		-- 创建内容	
		elseif v.type == 2 then
			local uiObj = m_ObjectPool:Get( "UIP_Setting_Option" );
			uiObj.transform:SetParent( m_uiContent.transform );
			uiObj.transform:GetComponent( typeof(UIButton) ).controlID = v.controlid
			local onORoff = GameManager.ini( k, v.default )
			if onORoff == 0 then 
				SetText( uiObj.transform:Find("OptionText"), T(v.textid), Hex2Color(0x547688FF) )
				SetTrue( uiObj.transform:Find("OptionCheck2") )
				SetTrue( uiObj.transform:Find("OptionBack2") )
			else
				SetText( uiObj.transform:Find("OptionText"), T(v.textid) )
				SetTrue( uiObj.transform:Find("OptionCheck1") )
				SetTrue( uiObj.transform:Find("OptionBack1") )
			end
			uiObj.transform:SetSiblingIndex(v.sort);
			v.uiObj = uiObj
		end
	end
end

function SettingDlgOptionClear()
	local objs = {};
	for i = 0 ,m_uiContent.transform.childCount - 1 do
		table.insert( objs, m_uiContent.transform:GetChild(i).gameObject )
    end
	for k, v in pairs(objs) do
		local obj = v;
		m_ObjectPool:Release( "UIP_Guard", obj );
    end
end

function SettingDlgSignatureShow()
	
end

function SettingDlgBubbleShow()
	
end

function SettingDlgExchangeShow()
	
end

function SettingDlgOptionSelect( nControlID )
	for k,v in pairs(m_SettingItems) do
		if v.controlid == nControlID then
			local onORoff = GameManager.ini( k, v.default )
			print( k.."="..onORoff )
			if onORoff == 0 then
				GameManager.writeini( k, 1 )
				SetText( v.uiObj.transform:Find("OptionText"), T(v.textid) )
				SetTrue( v.uiObj.transform:Find("OptionCheck1") )
				SetTrue( v.uiObj.transform:Find("OptionBack1") )
			else
				GameManager.writeini( k, 0 )
				SetText( v.uiObj.transform:Find("OptionText"), T(v.textid), Hex2Color(0x547688FF) )
				SetTrue( v.uiObj.transform:Find("OptionCheck2") )
				SetTrue( v.uiObj.transform:Find("OptionBack2") )
			end
			break
		end
	end
end

function SettingDlgHideBtns()
	SetFalse(m_uiCloseBtn)
	SetFalse(m_uiOptionBtn)
	SetFalse(m_uiSignatureBtn)
	SetFalse(m_uiChatBubbleBtn)
	SetFalse(m_uiExchangeBtn)
end
