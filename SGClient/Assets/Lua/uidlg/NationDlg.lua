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

local m_recvValue = nil

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
end

-- 删除界面
function NationDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
	m_recvValue = nil
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function NationDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            NationDlgClose();
		--国家建设
		elseif nControlID == 1 then
			if m_recvValue.m_level >= #g_nation_upgrade then
				pop(T(1769))
			elseif m_recvValue.m_donate_num >= 11 then
				pop(T(1770))
			else
				NationUpgradeDlgShow( m_recvValue )
			end
			
		--爵位升级
		elseif nControlID == 2 then
			NationPlaceDlgShow()
			
		--修改公告
		elseif nControlID == 3 then
		
		--国家任务
		elseif nControlID == 4 then
		
		--国家城池
		elseif nControlID == 5 then
			NationTownDlgShow()
			
		--国家战争
		elseif nControlID == 6 then
		
		--国家名将
		elseif nControlID == 7 then
		
		--国家荣誉
		elseif nControlID == 8 then
		
		--国家官员
		elseif nControlID == 9 then
		
		--国家战争
		elseif nControlID == 10 then
		
        end
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

