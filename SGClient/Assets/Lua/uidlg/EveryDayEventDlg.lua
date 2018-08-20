-- 界面
local m_Dlg = nil;
local m_uiLeft = nil; --UnityEngine.GameObject
local m_uiRight = nil; --UnityEngine.GameObject
local m_uiTalk = nil; --UnityEngine.GameObject
local m_uiEventCount = nil; --UnityEngine.GameObject
local m_uiEventTimer = nil; --UnityEngine.GameObject
local m_uiLayer = nil; --UnityEngine.GameObject
local m_uiGirl = nil; --UnityEngine.GameObject
--local m_DialogFrameMod = nil;

-- 打开界面
function EveryDayEventDlgOpen()
	ResourceManager.LoadAssetBundle( "_ab_ui_static_pic_5" )
	m_Dlg = eye.uiManager:Open( "EveryDayEventDlg" );
	--m_DialogFrameMod = DialogFrameModOpen( m_Dlg, T(0), nil, EveryDayEventDlgClose );
end

-- 隐藏界面
function EveryDayEventDlgClose()
	if m_Dlg == nil then
		return;
	end
	--DialogFrameModClose( m_DialogFrameMod );
	--m_DialogFrameMod = nil;
	eye.uiManager:Close( "EveryDayEventDlg" );
end

-- 删除界面
function EveryDayEventDlgDestroy()
	GameObject.Destroy( m_Dlg );
	ResourceManager.UnloadAssetBundle( "_ab_ui_static_pic_5" )
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function EveryDayEventDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            EveryDayEventDlgClose();
		elseif nControlID == 1 then
			EveryDayEventDlgSelect( 0 )
		elseif nControlID == 2 then
			EveryDayEventDlgSelect( 1 )
        end
	end
end

-- 载入时调用
function EveryDayEventDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiLeft = objs[0];
	m_uiRight = objs[1];
	m_uiTalk = objs[2];
	m_uiEventCount = objs[3];
	m_uiEventTimer = objs[4];
	m_uiLayer = objs[5];
	m_uiGirl = objs[6];
end

-- 界面初始化时调用
function EveryDayEventDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function EveryDayEventDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function EveryDayEventDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function EveryDayEventDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function EveryDayEventDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function EveryDayEventDlgShow()
	EveryDayEventDlgOpen()
	system_askinfo( ASKINFO_EVERYDAY_EVENT, "", 0 )
	SetFalse( m_uiLayer )
	SetFalse( m_uiGirl )
end

-- m_index=0,m_textid=0,m_talkid0=0,m_talkid1=0,m_awardkind0=0,m_awardnum0=0,m_awardkind1=0,m_awardnum1=0,m_eventnum=0,m_eventsec=0,
function EveryDayEventDlgRecv( recvValue )
	SetText( m_uiEventCount, F(4235,recvValue.m_eventnum,global.everyday_event_max) )
	SetTimer( m_uiEventTimer, recvValue.m_eventsec, recvValue.m_eventsec, 1, T(4236) );
	if recvValue.m_index == -1 then
		SetTrue( m_uiGirl )
		SetImage( m_uiGirl.transform:Find("Shape"), LoadSprite( "peopleface_0" ) )
		SetText( m_uiGirl.transform:Find("Name"), GetPlayer().m_maidname )
		return
	end
	SetTrue( m_uiLayer )
	SetText( m_uiTalk, Localization.text_quest(recvValue.m_textid) )
	SetText( m_uiLeft.transform:Find("Talk/Text"), Localization.text_quest(recvValue.m_talkid0) )
	SetText( m_uiRight.transform:Find("Talk/Text"), Localization.text_quest(recvValue.m_talkid1) )
	
	local herolist = {}
	for i=0,MAX_CITYHERONUM-1,1 do
		local pHero = GetHero().m_CityHero[i]
		if pHero.m_kind > 0 then
			table.insert( herolist, pHero.m_kind );
		end
	end
	
	for i=0,MAX_HERONUM-1,1 do
		local pHero = GetHero().m_Hero[i]
		if pHero.m_kind > 0 then
			table.insert( herolist, pHero.m_kind );
		end
	end
	
	table.ruffle(herolist)
	
	local herokind1 = herolist[1];
	local herokind2 = 0;
	if #herolist == 1 then
		herokind2 = herolist[1];
	else
		herokind2 = herolist[2];
	end

	SetImage( m_uiLeft.transform:Find("Shape"), HeroFaceSprite( herokind1 ) )
	SetImage( m_uiRight.transform:Find("Shape"), HeroFaceSprite( herokind2 ) )
	SetText( m_uiLeft.transform:Find("Name"), HeroName(herokind1) )
	SetText( m_uiRight.transform:Find("Name"), HeroName(herokind2) )
	
	local sprite0, color0, name0, c0 = AwardInfo(recvValue.m_awardkind0)
	SetImage( m_uiLeft.transform:Find("Award/Shape"), sprite0 );
	SetText( m_uiLeft.transform:Find("Award/Name"), name0.."x"..recvValue.m_awardnum0, NameColor(c0) );
	
	local sprite1, color1, name1, c1 = AwardInfo(recvValue.m_awardkind1)
	SetImage( m_uiRight.transform:Find("Award/Shape"), sprite1 );
	SetText( m_uiRight.transform:Find("Award/Name"), name1.."x"..recvValue.m_awardnum1, NameColor(c1) );
end

function EveryDayEventDlgSelect( select )
	system_askinfo( ASKINFO_EVERYDAY_EVENT, "", 1, select )
	SetFalse( m_uiLayer )
end
