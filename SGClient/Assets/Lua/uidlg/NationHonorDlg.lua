-- 界面
local m_Dlg = nil;
local m_DialogFrameMod = nil;

local m_uiMissionPre = nil; --UnityEngine.GameObject
local m_uiMissionNext = nil; --UnityEngine.GameObject
local m_uiMissionScroll = nil; --UnityEngine.GameObject
local m_uiMissionContent = nil; --UnityEngine.GameObject
local m_uiMissionScrollDot = nil; --UnityEngine.GameObject
local m_uiMissionScrollDotToggle = nil; --UnityEngine.GameObject
local m_uiRankScroll = nil; --UnityEngine.GameObject
local m_uiRankContent = nil; --UnityEngine.GameObject
local m_uiUIP_Mission = nil; --UnityEngine.GameObject
local m_uiAwardInfo = nil; --UnityEngine.GameObject

local m_MissionRecvValue = nil
local m_MissionPage = 0;

-- 打开界面
function NationHonorDlgOpen()
	m_Dlg = eye.uiManager:Open( "NationHonorDlg" );
	m_DialogFrameMod = DialogFrameModOpen( m_Dlg, T(1948), HELP_NationDlg, NationHonorDlgClose );
end

-- 隐藏界面
function NationHonorDlgClose()
	if m_Dlg == nil then
		return;
	end
	DialogFrameModClose( m_DialogFrameMod );
	m_DialogFrameMod = nil;
	eye.uiManager:Close( "NationHonorDlg" );
	m_MissionRecvValue = nil
end

-- 删除界面
function NationHonorDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
	m_MissionRecvValue = nil
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function NationHonorDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            NationHonorDlgClose();
		
		-- 荣誉任务上一个
		elseif nControlID == 1 then
			NationHonorDlgMissionPre();
			
		-- 荣誉任务下一个
		elseif nControlID == 2 then
			NationHonorDlgMissionNext();
		
		-- 奖励	
		elseif nControlID >= 100 and nControlID < 200 then
			NationHonorDlgMissionAward( nControlID-100 )
		
		-- 获取奖励	
		elseif nControlID >= 200 and nControlID < 300 then
			NationHonorDlgMissionAwardGet( nControlID-200 )
        end
	
	elseif nType == UI_EVENT_SCROLLPAGE then
		if nControlID == 0 then
			m_MissionPage = value+1
			NationHonorDlgMissionSetPage( value )
			NationHonorDlgMissionAward( -1 )
		end
	end
end

-- 载入时调用
function NationHonorDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiMissionPre = objs[0];
	m_uiMissionNext = objs[1];
	m_uiMissionScroll = objs[2];
	m_uiMissionContent = objs[3];
	m_uiMissionScrollDot = objs[4];
	m_uiMissionScrollDotToggle = objs[5];
	m_uiRankScroll = objs[6];
	m_uiRankContent = objs[7];
	m_uiUIP_Mission = objs[8];
	m_uiAwardInfo = objs[9];
end

-- 界面初始化时调用
function NationHonorDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function NationHonorDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function NationHonorDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function NationHonorDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function NationHonorDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function NationHonorDlgShow()
	NationHonorDlgOpen()
	system_askinfo( ASKINFO_NATION, "", 10 )
end

-- m_count=0,m_list={m_value={[3]},m_needvalue={[3]},m_awardkind={[2]},m_awardnum={[2]},m_isget=0,[m_count]},
function NationHonorDlgMissionRecv( recvValue )
	local scrollPage = m_uiMissionScroll.transform:GetComponent( typeof(UIScrollPage) )
	scrollPage:ClearPage()
	m_MissionRecvValue = recvValue;
	for i=1, recvValue.m_count, 1 do
		NationHonorDlgMissionCreate( i, scrollPage, recvValue.m_list[i] )
	end
	if recvValue.m_count > 0 then
		scrollPage:PageCountChanged()
	end
	m_MissionPage = 1
	NationHonorDlgMissionSetPage()
end

-- 创建荣誉任务
function NationHonorDlgMissionCreate( index, scrollPage, info )
	local uiObj = GameObject.Instantiate( m_uiUIP_Mission );
	SetTrue( uiObj )
	scrollPage:AddPage( uiObj )
	
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiAwardEffect = objs[0];
	local uiAwardButton = objs[1];
	local uiAwardName = objs[2];
	local uiInfo = objs[3];
	local uiWarn = objs[4];
	
	-- 已经领取
	if info.m_isget == 1 then
		SetTrue( uiWarn )
		SetText( uiWarn.transform:Find("Text"), T(1109) )
		SetControlID( uiAwardButton, 100+index )
		
	-- 目标达成
	elseif info.m_value[1] >= info.m_needvalue[1] and info.m_value[2] >= info.m_needvalue[2] and info.m_value[3] >= info.m_needvalue[3] then
		SetTrue( uiWarn )
		SetText( uiWarn.transform:Find("Text"), T(1801) )
		SetControlID( uiAwardButton, 200+index )
	else
		SetFalse( uiWarn )
		SetControlID( uiAwardButton, 100+index )
	end
	SetText( uiAwardName, T(1791+index) )
	SetText( uiInfo.transform:GetChild(0), T(1794+index) )
	
	if info.m_value[1] >= info.m_needvalue[1] then
		SetText( uiInfo.transform:GetChild(1), F(1798, "<color=#03DE27FF>"..info.m_value[1].."</color>", info.m_needvalue[1]) )
	else
		SetText( uiInfo.transform:GetChild(1), F(1798, "<color=#E80017FF>"..info.m_value[1].."</color>", info.m_needvalue[1]) )
	end
	
	if info.m_value[2] >= info.m_needvalue[2] then
		SetText( uiInfo.transform:GetChild(2), F(1799, "<color=#03DE27FF>"..info.m_value[2].."</color>", info.m_needvalue[2]) )
	else
		SetText( uiInfo.transform:GetChild(2), F(1799, "<color=#E80017FF>"..info.m_value[2].."</color>", info.m_needvalue[2]) )
	end
	
	if info.m_value[3] >= info.m_needvalue[3] then
		SetText( uiInfo.transform:GetChild(3), F(1800, "<color=#03DE27FF>"..info.m_value[3].."</color>", info.m_needvalue[3]) )
	else
		SetText( uiInfo.transform:GetChild(3), F(1800, "<color=#E80017FF>"..info.m_value[3].."</color>", info.m_needvalue[3]) )
	end
end

-- 上一个
function NationHonorDlgMissionPre()
	if m_MissionPage <= 1 then
		return
	end
	local scrollPage = m_uiMissionScroll.transform:GetComponent( typeof(UIScrollPage) )
	scrollPage:Pre()
end

-- 下一个
function NationHonorDlgMissionNext()
	if m_MissionPage >= m_MissionRecvValue.m_count then
		return
	end
	local scrollPage = m_uiMissionScroll.transform:GetComponent( typeof(UIScrollPage) )
	scrollPage:Next()
end

-- 设置翻页
function NationHonorDlgMissionSetPage()
	if m_MissionPage <= 1 then
		SetFalse( m_uiMissionPre )
	else
		SetTrue( m_uiMissionPre )
	end
	
	if m_MissionPage >= m_MissionRecvValue.m_count then
		SetFalse( m_uiMissionNext )
	else
		SetTrue( m_uiMissionNext )
	end
end

-- 奖励
function NationHonorDlgMissionAward( index )
	if IsActive(m_uiAwardInfo) or index == -1 then
		SetFalse( m_uiAwardInfo )
		return
	end
	local info = m_MissionRecvValue.m_list[index]
	if info == nil then
		return
	end
	SetTrue( m_uiAwardInfo )
	local str = ""
	for i = 1, 2, 1 do
		if info.m_awardkind[i] > 0 then
			local sprite, color, name = AwardInfo( info.m_awardkind[i] )
			str = str..name.."x"..info.m_awardnum[i].."\n"
		end
	end
	SetText( m_uiAwardInfo.transform:Find("Text"), str );
	m_uiAwardInfo.transform:SetSiblingIndex(1000);
end

-- 获取奖励
function NationHonorDlgMissionAwardGet( index )
	system_askinfo( ASKINFO_NATION, "", 11, index )
end