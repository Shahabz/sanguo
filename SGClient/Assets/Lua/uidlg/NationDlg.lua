-- 界面
local m_Dlg = nil;
local m_DialogFrameMod = nil;

local m_uiLevel = nil; --UnityEngine.GameObject
local m_uiExp = nil; --UnityEngine.GameObject
local m_uiBuild = nil; --UnityEngine.GameObject
local m_uiPlace = nil; --UnityEngine.GameObject
local m_uiRank = nil; --UnityEngine.GameObject
local m_uiPrestige = nil; --UnityEngine.GameObject
local m_uiFlag = nil; --UnityEngine.GameObject
local m_uiOfficialer = nil; --UnityEngine.GameObject
local m_uiNotice = nil; --UnityEngine.GameObject
local m_uiKingName = nil; --UnityEngine.GameObject
local m_uiNationBuildBtn = nil; --UnityEngine.GameObject
local m_uiPlaceUpBtn = nil; --UnityEngine.GameObject
local m_uiQuestBtn = nil; --UnityEngine.GameObject
local m_uiTownBtn = nil; --UnityEngine.GameObject
local m_uiWarBtn = nil; --UnityEngine.GameObject
local m_uiHeroBtn = nil; --UnityEngine.GameObject
local m_uiHonorBtn = nil; --UnityEngine.GameObject
local m_uiOfficerListBtn = nil; --UnityEngine.GameObject
local m_uiLogBtn = nil; --UnityEngine.GameObject
local m_uiQuestInfo = nil; --UnityEngine.GameObject
local m_uiPlaceIcon = nil; --UnityEngine.GameObject
local m_uiChangeNoticeBtn = nil; --UnityEngine.GameObject
local m_uiNoticeLayer = nil; --UnityEngine.GameObject
local m_uiContentField = nil; --UnityEngine.GameObject

local m_recvValue = nil
local m_curQuestKind = 1;

-- 打开界面
function NationDlgOpen()
	m_Dlg = eye.uiManager:Open( "NationDlg" );
	m_DialogFrameMod = DialogFrameModOpen( m_Dlg, T(133), HELP_NationDlg, NationDlgClose );
end

-- 隐藏界面
function NationDlgClose()
	if m_Dlg == nil then
		return;
	end
	DialogFrameModClose( m_DialogFrameMod );
	m_DialogFrameMod = nil;
	eye.uiManager:Close( "NationDlg" );
	m_recvValue = nil
	m_curQuestKind = 1;
end

-- 删除界面
function NationDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
	m_recvValue = nil
	m_curQuestKind = 1;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function NationDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            NationDlgClose();
			
		elseif nControlID == -2 then
			NationDlgHideNoticeLayer()
			
		--国家建设
		elseif nControlID == 1 then
			if m_recvValue.m_level >= #g_nation_upgrade then
				pop(T(1769))
			else
				NationUpgradeDlgShow( m_recvValue )
			end
			
		--爵位升级
		elseif nControlID == 2 then
			NationPlaceDlgShow()
			
		--修改公告
		elseif nControlID == 3 then
			NationDlgShowNoticeLayer()
			
		--国家任务
		elseif nControlID == 4 then
			NationQuestDlgShow()
			SetFalse(m_uiQuestInfo.transform:Find("Effect"));
		--国家城池
		elseif nControlID == 5 then
			NationTownDlgShow()
			
		--国家战争
		elseif nControlID == 6 then
			NationWarDlgShow()
			
		--国家名将
		elseif nControlID == 7 then
			if m_recvValue.m_level < 3 then
				AlertMsg( F(1895, 3) )
			else
				NationHeroDlgShow()
			end
			
		--国家荣誉
		elseif nControlID == 8 then
			NationHonorDlgShow()
			
		--国家官员
		elseif nControlID == 9 then
			NationOfficialDlgShow()
		
		--国家日志
		elseif nControlID == 10 then
			NationLogDlgShow()
			
		-- 公告重新编辑
		elseif nControlID == 101 then
			NationDlgNoticeLayerReset()
			
		-- 公告确认发布
		elseif nControlID == 102 then
			NationDlgNoticeLayerSend()
		
        end
	elseif nType == UI_EVENT_TIMECOUNTEND then
		if nControlID == 1 then
			NationDlgSetQuest()
		end	
	
	elseif nType == UI_EVENT_INPUTVALUECHANGED then
		local content = m_uiContentField:GetComponent("UIInputField").text;
		if content == "" then
			m_uiContentField.transform:Find( "Hint" ):GetComponent( typeof(UIText) ).text = T(1849)
		else
			m_uiContentField.transform:Find( "Hint" ):GetComponent( typeof(UIText) ).text = ""
		end
		SetRichText( m_uiContentField.transform:Find( "RichText" ), m_uiContentField:GetComponent("UIInputField").text )
		
	end
end

-- 载入时调用
function NationDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiLevel = objs[0];
	m_uiExp = objs[1];
	m_uiBuild = objs[2];
	m_uiPlace = objs[3];
	m_uiRank = objs[4];
	m_uiPrestige = objs[5];
	m_uiFlag = objs[6];
	m_uiOfficialer = objs[7];
	m_uiNotice = objs[8];
	m_uiKingName = objs[9];
	m_uiNationBuildBtn = objs[10];
	m_uiPlaceUpBtn = objs[11];
	m_uiQuestBtn = objs[12];
	m_uiTownBtn = objs[13];
	m_uiWarBtn = objs[14];
	m_uiHeroBtn = objs[15];
	m_uiHonorBtn = objs[16];
	m_uiOfficerListBtn = objs[17];
	m_uiLogBtn = objs[18];
	m_uiQuestInfo = objs[19];
	m_uiPlaceIcon = objs[20];
	m_uiChangeNoticeBtn = objs[21];
	m_uiNoticeLayer = objs[22];
	m_uiContentField = objs[23];
end

-- 界面初始化时调用
function NationDlgOnStart( gameObject )

end

-- 界面显示时调用
function NationDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function NationDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function NationDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function NationDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function NationDlgShow()
	NationDlgOpen()
	m_uiHeroBtn.transform:SetSiblingIndex(1000);
	m_uiHonorBtn.transform:SetSiblingIndex(1001);
	m_uiOfficerListBtn.transform:SetSiblingIndex(1002);
	m_uiLogBtn.transform:SetSiblingIndex(1003);
	
	-- 获取信息
	system_askinfo( ASKINFO_NATION, "", 0 )
	
	-- 国家旗帜
	SetImage( m_uiFlag, NationFlagSprite( GetPlayer().m_nation ) )

	-- 官职
	NationDlgChangeOfficial();
	-- 爵位
	NationDlgChangePlace();
	-- 威望
	NationDlgChangePrestige();
end

-- m_level=0,m_exp=0,m_donate_num=0,m_myrank=0,m_notice_len=0,m_notice="[m_notice_len]",m_kingname_len=0,m_kingname="[m_kingname_len]",
function NationDlgRecv( recvValue )
	m_recvValue = recvValue;
	NationDlgChangeBase()
	NationDlgChangeNotice();
	NationDlgSetQuest()
end

-- m_level=0,m_exp=0,m_donate_num=0,m_myrank=0
function NationDlgRecvBase( recvValue )
	m_recvValue.m_level = recvValue.m_level
	m_recvValue.m_exp = recvValue.m_exp
	m_recvValue.m_donate_num = recvValue.m_donate_num
	m_recvValue.m_myrank = recvValue.m_myrank
	NationDlgChangeBase()
end

-- 基本信息
function NationDlgChangeBase()
	SetText( m_uiLevel, F( 1936, m_recvValue.m_level ) )
	if m_recvValue.m_level >= #g_nation_upgrade then
		SetText( m_uiExp, T(1768) )
		SetText( m_uiBuild, T(1769) )
	else
		SetText( m_uiExp, F( 1937, knum(m_recvValue.m_exp), knum(g_nation_upgrade[m_recvValue.m_level][1].maxexp) ) )
		SetText( m_uiBuild, F( 1940, m_recvValue.m_donate_num, 11 ) )
	end
	SetText( m_uiRank, F( 1939, m_recvValue.m_myrank ) )
end

-- 公告
function NationDlgChangeNotice()
	SetText( m_uiNotice, m_recvValue.m_notice )
	if m_recvValue.m_kingname_len > 0 then
		SetText( m_uiKingName, OfficialName(GetPlayer().m_nation)..":"..m_recvValue.m_kingname )
	else
		SetText( m_uiKingName, OfficialName(GetPlayer().m_nation)..":"..T(240) )
	end
end

-- 国家任务
function NationDlgSetQuest()
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	SetTrue( m_uiQuestInfo.transform:Find("Timer") )
	SetTimer( m_uiQuestInfo.transform:Find("Timer"), 6, 6, 1 );
	
	-- 检查是否有优先显示的
	for i=1, 3, 1 do
		local value = m_recvValue.m_questvalue[i];
		local maxvalue = m_recvValue.m_questvalue_max[i];
		if value >= maxvalue then
			m_curQuestKind = i;
		end
	end
	
	local kind = m_curQuestKind
	local value = m_recvValue.m_questvalue[kind];
	local maxvalue = m_recvValue.m_questvalue_max[kind];
	if value == -1 then
		value = maxvalue
	end
	if kind == 1 then
		SetText( m_uiQuestInfo.transform:Find("Text"), F(1787, maxvalue, value, maxvalue) );
	elseif kind == 2 then
		SetText( m_uiQuestInfo.transform:Find("Text"), F(1788, maxvalue, value, maxvalue) );
	elseif kind == 3 then
		SetText( m_uiQuestInfo.transform:Find("Text"), F(1789, maxvalue, value, maxvalue) );
	end
	m_curQuestKind = m_curQuestKind + 1;
	if m_curQuestKind > 3 then
		m_curQuestKind = 1
	end
	if value == maxvalue then
		SetTrue(m_uiQuestInfo.transform:Find("Effect"));
	else
		SetFalse(m_uiQuestInfo.transform:Find("Effect"));
	end
end

-- 我的爵位
function NationDlgChangePlace()
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	SetText( m_uiPlace, F(1938, PlaceName( GetPlayer().m_place )) )
	SetImage( m_uiPlaceIcon, PlaceSprite( GetPlayer().m_place ) )
end

-- 我的威望
function NationDlgChangePrestige()
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	SetText( m_uiPrestige, F(1941, knum(GetPlayer().m_prestige)) )
end

-- 我的官职
function NationDlgChangeOfficial()
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	SetText( m_uiOfficialer, T(1943)..":"..OfficialName(GetPlayer().m_official) )
	if GetPlayer().m_official == 1 or GetPlayer().m_official == 2 or GetPlayer().m_official == 3 then
		SetTrue( m_uiChangeNoticeBtn )
	else
		SetFalse( m_uiChangeNoticeBtn )
	end
end

-- 显示修改公告
function NationDlgShowNoticeLayer()
	SetTrue( m_uiNoticeLayer )
	m_uiNoticeLayer.transform:SetSiblingIndex(1010);
	if m_recvValue.m_notice_len > 0 then
		m_uiContentField.transform:Find( "RichText" ):GetComponent( typeof(YlyRichText) ).text = m_recvValue.m_notice
		m_uiContentField.transform:Find( "Hint" ):GetComponent( typeof(UIText) ).text = "";
	else
		m_uiContentField.transform:Find( "RichText" ):GetComponent( typeof(YlyRichText) ).text = ""
		m_uiContentField.transform:Find( "Hint" ):GetComponent( typeof(UIText) ).text = T(1849)
	end
end

-- 隐藏修改公告
function NationDlgHideNoticeLayer()
	SetFalse( m_uiNoticeLayer )
end

-- 重新编辑
function NationDlgNoticeLayerReset()
	m_uiContentField.transform:Find( "RichText" ):GetComponent( typeof(YlyRichText) ).text = ""
	m_uiContentField.transform:Find( "Hint" ):GetComponent( typeof(UIText) ).text = T(1849)
end

-- 确认发布
function NationDlgNoticeLayerSend()
	local content = m_uiContentField.transform:Find( "RichText" ):GetComponent( typeof(YlyRichText) ).text;
	local len = string.len( content )
	if len < 4 or len >= 400 then
		pop(T(1850))
		return;
	end
	if string.checksign( content ) == false then
		pop(T(788))
		return
	end
	if Utils.MaskWordCheck( content ) == false then
		pop(T(789))
		return;
	end
	if m_recvValue.m_notice == content then
		pop(T(1882))
		return;
	end
	system_askinfo( ASKINFO_NATION, content, 17 )
	NationDlgHideNoticeLayer()
end
