-- 界面
local m_Dlg = nil;
local m_uiWeiTalkPanel = nil; --UnityEngine.GameObject
local m_uiShuTalkPanel = nil; --UnityEngine.GameObject
local m_uiWuTalkPanel = nil; --UnityEngine.GameObject
local m_uiAward = nil; --UnityEngine.GameObject

local m_nation_award = 0;
local m_SelectNation = 0;
-- 打开界面
function CreateDlgOpen()
	m_Dlg = eye.uiManager:Open( "CreateDlg" );
end

-- 隐藏界面
function CreateDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "CreateDlg" );
end

-- 删除界面
function CreateDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function CreateDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
		elseif nControlID == 1 or nControlID == 2 or nControlID == 3 then
			CreateDlgSelect( nControlID )
		elseif nControlID == 10 then
			CreateDlgCreate()
        end
	end
end

-- 载入时调用
function CreateDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiWeiTalkPanel = objs[0];
	m_uiShuTalkPanel = objs[1];
	m_uiWuTalkPanel = objs[2];
	m_uiAward = objs[3];
end

-- 界面初始化时调用
function CreateDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function CreateDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function CreateDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function CreateDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function CreateDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function CreateDlgShow( nation_award )
	CreateDlgOpen();
	m_nation_award = nation_award;
	CreateDlgSelect(nation_award);
end

function CreateDlgSelect( nation )	
	m_SelectNation = nation;
	m_uiWeiTalkPanel:SetActive( false );
	m_uiShuTalkPanel:SetActive( false );
	m_uiWuTalkPanel:SetActive( false );
	if nation == 1 then
		m_uiWeiTalkPanel:SetActive( true );
		CreateDlgSetAward(m_uiWeiTalkPanel,nation);
	elseif nation == 2 then
		m_uiShuTalkPanel:SetActive( true );
		CreateDlgSetAward(m_uiShuTalkPanel,nation);
	elseif nation == 3 then
		m_uiWuTalkPanel:SetActive( true );
		CreateDlgSetAward(m_uiWuTalkPanel,nation);
	end
end
function CreateDlgSetAward( gameObj,nation )
	local uiAward=gameObj.transform:Find("SelectNation");
	if nation == m_nation_award then
		SetTrue( uiAward );
		SetText( uiAward.transform:Find("Num"),F(548,global.nation_award_token));
	else
		SetFalse( uiAward );
	end
	
end	
function CreateDlgCreate()
	if m_SelectNation <= 0 then
		return;
	end
	
	-- 创建角色
	local sendValue = {};
	sendValue.m_nation = m_SelectNation;
	sendValue.m_name = ""
	sendValue.m_name_length = string.len( sendValue.m_name );
	netsend_create_C( sendValue );
	CreateDlgClose();
end