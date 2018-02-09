-- 界面
local m_Dlg = nil;
local m_DialogFrameMod = nil;

local m_uiFlag1 = nil; --UnityEngine.GameObject
local m_uiFlag2 = nil; --UnityEngine.GameObject
local m_uiFlag3 = nil; --UnityEngine.GameObject
local m_uiReplaceBtn = nil; --UnityEngine.GameObject
local m_uiReplaceMask = nil; --UnityEngine.GameObject
local m_uiOfficialName = nil; --UnityEngine.GameObject
local m_uiTimer = nil; --UnityEngine.GameObject
local m_uiCandidate = nil; --UnityEngine.GameObject
local m_uiCandidateScroll = nil; --UnityEngine.GameObject
local m_uiCandidateContent = nil; --UnityEngine.GameObject
local m_uiVote = nil; --UnityEngine.GameObject
local m_uiOfficial = nil; --UnityEngine.GameObject
local m_uiOfficialScroll = nil; --UnityEngine.GameObject
local m_uiOfficialContent = nil; --UnityEngine.GameObject
local m_uiNextTimer = nil; --UnityEngine.GameObject
local m_uiRightLayer = nil; --UnityEngine.GameObject
local m_uiRow6 = nil; --UnityEngine.GameObject
local m_uiRow7 = nil; --UnityEngine.GameObject
local m_uiRow8 = nil; --UnityEngine.GameObject
local m_uiRow9 = nil; --UnityEngine.GameObject

local m_CandidateRecvValue = nil
local m_OfficialRecvValue = nil
-- 打开界面
function NationOfficialDlgOpen()
	m_Dlg = eye.uiManager:Open( "NationOfficialDlg" );
	m_DialogFrameMod = DialogFrameModOpen( m_Dlg, T(1949), HELP_NationDlg, NationOfficialDlgClose );
end

-- 隐藏界面
function NationOfficialDlgClose()
	if m_Dlg == nil then
		return;
	end
	DialogFrameModClose( m_DialogFrameMod );
	m_DialogFrameMod = nil;
	eye.uiManager:Close( "NationOfficialDlg" );
	m_CandidateRecvValue = nil
	m_OfficialRecvValue = nil
end

-- 删除界面
function NationOfficialDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
	m_CandidateRecvValue = nil
	m_OfficialRecvValue = nil
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function NationOfficialDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            NationOfficialDlgClose();
			
		elseif nControlID == -2 then
			NationOfficialDlgHideRight()
			
		-- 将军替换
		elseif nControlID == 1 then
			NationOfficialReplaceDlgShow()
			
		-- 官职权力
		elseif nControlID == 2 then
			NationOfficialDlgShowRight()
			
		-- 投票	
		elseif nControlID >= 1000 and nControlID < 2000 then
			NationOfficialDlgBallot( nControlID - 1000 )
			
		-- 拉票
		elseif nControlID >= 2000 and nControlID < 3000 then
			NationOfficialDlgBuyBallot( nControlID - 2000 )
			
        end
	elseif nType == UI_EVENT_TIMECOUNTEND then
		if nControlID == 1 then

		end	
	end
end

-- 载入时调用
function NationOfficialDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiFlag1 = objs[0];
	m_uiFlag2 = objs[1];
	m_uiFlag3 = objs[2];
	m_uiReplaceBtn = objs[3];
	m_uiReplaceMask = objs[4];
	m_uiOfficialName = objs[5];
	m_uiTimer = objs[6];
	m_uiCandidate = objs[7];
	m_uiCandidateScroll = objs[8];
	m_uiCandidateContent = objs[9];
	m_uiVote = objs[10];
	m_uiOfficial = objs[11];
	m_uiOfficialScroll = objs[12];
	m_uiOfficialContent = objs[13];
	m_uiNextTimer = objs[14];
	m_uiRightLayer = objs[15];
	m_uiRow6 = objs[16];
	m_uiRow7 = objs[17];
	m_uiRow8 = objs[18];
	m_uiRow9 = objs[19];
end

-- 界面初始化时调用
function NationOfficialDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function NationOfficialDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function NationOfficialDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function NationOfficialDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function NationOfficialDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function NationOfficialDlgShow()
	if GetPlayer().m_open_town6 == 0 then
		pop( T(1791) )
		return
	end
	NationOfficialDlgOpen()
	NationOfficialDlgUpdate()
end

function NationOfficialDlgUpdate()
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	SetFalse( m_uiRightLayer )
	SetFalse( m_uiCandidate )
	SetFalse( m_uiOfficial )
	SetFalse( m_uiOfficialName )
	SetFalse( m_uiTimer )
	SetFalse( m_uiNextTimer )
	SetFalse( m_uiReplaceBtn )
	SetTrue( m_uiReplaceMask )
	SetImage( m_uiFlag1.transform:Find("Back"), LoadSprite( "nationofficial_r1_"..GetPlayer().m_nation ) )
	SetText( m_uiFlag1.transform:Find("Name"), "" )
 	SetText( m_uiFlag2.transform:Find("Name"), "" )
	SetText( m_uiFlag3.transform:Find("Name"), "" )
	system_askinfo( ASKINFO_NATION, "", 12 )
end

-- 接收候选人
-- m_count=0,m_list={m_namelen=0,m_name="[m_namelen]",m_level=0,m_battlepower=0,m_ballot=0,m_actorid=0,[m_count]},m_endtime=0,m_myvote=0,m_isballot=0,m_tokenballot=0,
function NationOfficialDlgCandidateRecv( recvValue )
	if recvValue.m_count == 0 then
		local lefttime = recvValue.m_endtime - GetServerTime()
		SetTrue( m_uiTimer )
		SetTimer( m_uiTimer, lefttime-1, lefttime, 1, T(1824) )
		return
	end
	SetTrue( m_uiCandidate )
	m_CandidateRecvValue = recvValue;
	-- 排序
	table.sort( m_CandidateRecvValue.m_list, function( a, b ) 
		if a.m_ballot > b.m_ballot then
			return true
		else
			if a.m_ballot == b.m_ballot then
				if a.m_battlepower > b.m_battlepower then
					return true
				end
			end
		end
		return false
	end )
	for i=1, recvValue.m_count, 1 do
		local uiObj = m_uiCandidateContent.transform:GetChild( i - 1 )
		if NationOfficialDlgCandidateCreate( i, uiObj, recvValue.m_list[i] ) == false then
			SetFalse( uiObj )
		end
	end
	-- 倒计时
	local lefttime = recvValue.m_endtime - GetServerTime()
	SetTrue( m_uiTimer )
	SetTimer( m_uiTimer, lefttime-1, lefttime, 1, T(1813) )
	-- 拥有票数	
	if m_CandidateRecvValue.m_isballot > 0 then
		SetText( m_uiVote, T(1818), Hex2Color( 0xE80017FF ) )
	else
		SetText( m_uiVote, T(1819)..":"..m_CandidateRecvValue.m_myvote, Hex2Color( 0x03DE27FF ) )
	end
end

-- 创建
function NationOfficialDlgCandidateCreate( index, uiObj, info )
	SetTrue( uiObj )
	SetText( uiObj.transform:Find("Battlepower"), info.m_battlepower )
	SetText( uiObj.transform:Find("Name"), info.m_name )
	SetText( uiObj.transform:Find("Level"), info.m_level )
	SetText( uiObj.transform:Find("Ballot"), info.m_ballot )
	
	if m_CandidateRecvValue.m_myvote > 0 then
		SetTrue( uiObj.transform:Find("BallotBtn") )
		SetFalse( uiObj.transform:Find("BuyBtn") )
		SetControlID( uiObj.transform:Find("BallotBtn"), 1000+index )
	else
		SetFalse( uiObj.transform:Find("BallotBtn") )
		SetTrue( uiObj.transform:Find("BuyBtn") )
		SetControlID( uiObj.transform:Find("BuyBtn"), 2000+index )
	end
end

-- 投票
function NationOfficialDlgBallot( index )
	local info = m_CandidateRecvValue.m_list[index]
	if info == nil then
		return
	end
	MsgBox( F(1821, info.m_name ), function() 
		system_askinfo( ASKINFO_NATION, "", 13, info.m_actorid, 0 )
	end )
end

-- 拉票
function NationOfficialDlgBuyBallot( index )
	local info = m_CandidateRecvValue.m_list[index]
	if info == nil then
		return
	end
	local token = math.floor( math.pow( (m_CandidateRecvValue.m_tokenballot+1), 1.3 ) * 5 ) 
	MsgBox( F(1822, token, info.m_name ), function() 
		system_askinfo( ASKINFO_NATION, "", 13, info.m_actorid, 1 )
	end )
end

-- 接收官员
-- m_count=0,m_list={m_official=0,m_namelen=0,m_name="[m_namelen]",m_level=0,m_battlepower=0,m_zone=0,[m_count]},m_endtime=0,
function NationOfficialDlgOfficialRecv( recvValue )
	for i=0, 9, 1 do
		local uiObj = m_uiOfficialContent.transform:GetChild( i )
		SetFalse( uiObj )
	end
	
	if recvValue.m_count == 0 then
		return
	end
	SetText( m_uiFlag1.transform:Find("Name"), recvValue.m_list[1].m_name )
 	SetText( m_uiFlag2.transform:Find("Name"), recvValue.m_list[2].m_name )
	SetText( m_uiFlag3.transform:Find("Name"), recvValue.m_list[3].m_name )
	SetText( m_uiOfficialName, T(1943)..":"..OfficialName(GetPlayer().m_official) )
	SetTrue( m_uiOfficialName )
	SetTrue( m_uiOfficial )
	m_OfficialRecvValue = recvValue;
	for i=1, recvValue.m_count, 1 do
		local uiObj = m_uiOfficialContent.transform:GetChild( i - 1 )
		if NationOfficialDlgOfficialCreate( i, uiObj, recvValue.m_list[i] ) == false then
			SetFalse( uiObj )
		end
	end
	-- 倒计时
	local lefttime = recvValue.m_endtime - GetServerTime()
	SetTrue( m_uiNextTimer )
	SetTimer( m_uiNextTimer, lefttime-1, lefttime, 1, T(1825) )
	
	-- 如果我是王
	if GetPlayer().m_official == 1 or GetPlayer().m_official == 2 or GetPlayer().m_official == 3 then
		SetTrue( m_uiReplaceBtn )
		SetFalse( m_uiReplaceMask )
	else
		SetFalse( m_uiReplaceBtn )
		SetTrue( m_uiReplaceMask )
	end
end

-- 创建
function NationOfficialDlgOfficialCreate( index, uiObj, info )
	SetTrue( uiObj )
	SetText( uiObj.transform:Find("OfficialName"), OfficialName( info.m_official ) )
	SetText( uiObj.transform:Find("Name"), info.m_name )
	SetText( uiObj.transform:Find("Level"), info.m_level )
	SetText( uiObj.transform:Find("Battlepower"), info.m_battlepower )
	SetText( uiObj.transform:Find("Zone"), MapZoneName( info.m_zone ) )
end

-- 显示特权
function NationOfficialDlgShowRight()
	SetTrue( m_uiRightLayer )
	
	SetText( m_uiRow6.transform:Find("R1"), g_nation_official[1].cityfight_body )
	SetText( m_uiRow6.transform:Find("R4"), g_nation_official[4].cityfight_body )
	SetText( m_uiRow6.transform:Find("R5"), g_nation_official[5].cityfight_body )
	SetText( m_uiRow6.transform:Find("R6"), g_nation_official[6].cityfight_body )
	
	SetText( m_uiRow7.transform:Find("R1"), g_nation_official[1].callnum )
	SetText( m_uiRow7.transform:Find("R4"), g_nation_official[4].callnum )
	SetText( m_uiRow7.transform:Find("R5"), g_nation_official[5].callnum )
	SetText( m_uiRow7.transform:Find("R6"), g_nation_official[6].callnum )
	
	if g_nation_official[1].yield > 0 then
		SetTrue( m_uiRow8 )
		SetText( m_uiRow8.transform:Find("R1"), g_nation_official[1].yield.."%" )
		SetText( m_uiRow8.transform:Find("R4"), g_nation_official[4].yield.."%" )
		SetText( m_uiRow8.transform:Find("R5"), g_nation_official[5].yield.."%" )
		SetText( m_uiRow8.transform:Find("R6"), g_nation_official[6].yield.."%" )
	else
		SetFalse( m_uiRow8 )
	end
	
	if g_nation_official[1].buildquick > 0 then
		SetTrue( m_uiRow9 )
		SetText( m_uiRow9.transform:Find("R1"), g_nation_official[1].buildquick.."%" )
		SetText( m_uiRow9.transform:Find("R4"), g_nation_official[4].buildquick.."%" )
		SetText( m_uiRow9.transform:Find("R5"), g_nation_official[5].buildquick.."%" )
		SetText( m_uiRow9.transform:Find("R6"), g_nation_official[6].buildquick.."%" )
	else
		SetFalse( m_uiRow9 )
	end
	
end

-- 隐藏特权
function NationOfficialDlgHideRight()
	SetFalse( m_uiRightLayer )
end
