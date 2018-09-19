-- 界面
local m_Dlg = nil;
local m_uiAwardScroll = nil; --UnityEngine.GameObject
local m_uiAwardContent = nil; --UnityEngine.GameObject
local m_uiUIP_Award = nil; --UnityEngine.GameObject
local m_uiAwardGet = nil; --UnityEngine.GameObject
local m_uiProgressAward = nil; --UnityEngine.GameObject
local m_uiProgressNum = nil; --UnityEngine.GameObject
local m_uiAwardDescLayer = nil; --UnityEngine.GameObject
local m_uiWindow = nil; --UnityEngine.GameObject
local m_uiAwardProgressLayer = nil; --UnityEngine.GameObject
local m_uiMorrowLayer = nil; --UnityEngine.GameObject
local m_recvValue = nil
local m_totalNum = 0;
local m_ObjectPool = nil;
local m_AwardDescLayerShow = false
local m_AwardProgressLayerShow = false

-- 打开界面
function EveryDaySigninDlgOpen()
	ResourceManager.LoadAssetBundle( "_ab_ui_static_pic_5" )
	m_Dlg = eye.uiManager:Open( "EveryDaySigninDlg" );
end

-- 隐藏界面
function EveryDaySigninDlgClose()
	if m_Dlg == nil then
		return;
	end
	eye.uiManager:Close( "EveryDaySigninDlg" );
	m_recvValue = nil
end

-- 删除界面
function EveryDaySigninDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function EveryDaySigninDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            EveryDaySigninDlgClose();
		elseif nControlID == -2 then
			EveryDaySigninDlgAwardDescLayer()
		elseif nControlID == -3 then
			EveryDaySigninDlgProgressLayer()
		elseif nControlID == 1 then	
			EveryDaySigninDlgGetAward()
		elseif nControlID == 2 then	
			EveryDaySigninDlgCallHero()
		elseif nControlID >= 100 and nControlID < 200 then
			EveryDaySigninDlgClickItem( nControlID-100, value )
		elseif nControlID >= 200 and nControlID < 300 then
			EveryDaySigninDlgReset( nControlID-200, value )
		
		elseif nControlID >= 1000 and nControlID < 2000 then
			EveryDaySigninDlgClickProgress( nControlID-1000, value )
			
		elseif nControlID >= 2000 and nControlID < 3000 then
			EveryDaySigninDlgProgressGetAward( nControlID-2000 )
        end
	elseif nType == UI_EVENT_TIMECOUNTEND then
		if nControlID == 0 then
			system_askinfo( ASKINFO_EVERYDAY_SIGNIN, "", 0 )
		end
	end
end

-- 载入时调用
function EveryDaySigninDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiAwardScroll = objs[0];
	m_uiAwardContent = objs[1];
	m_uiUIP_Award = objs[2];
	m_uiAwardGet = objs[3];
	m_uiProgressAward = objs[4];
	m_uiProgressNum = objs[5];
	m_uiAwardDescLayer = objs[6];
	m_uiWindow = objs[7];
	m_uiAwardProgressLayer = objs[8];
	m_uiMorrowLayer = objs[9];
	
	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );
	m_ObjectPool:CreatePool("UIP_Award", 30, 30, m_uiUIP_Award);
end

-- 界面初始化时调用
function EveryDaySigninDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function EveryDaySigninDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function EveryDaySigninDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function EveryDaySigninDlgOnDestroy( gameObject )
	m_Dlg = nil;
	ResourceManager.UnloadAssetBundle( "_ab_ui_static_pic_5" )
end

-- 每帧调用
function EveryDaySigninDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function EveryDaySigninDlgShow()
	EveryDaySigninDlgOpen()
	system_askinfo( ASKINFO_EVERYDAY_SIGNIN, "", 0 )
end

-- m_count=0,m_list={m_id=0,m_awardkind=0,m_awardnum=0,m_token=0,m_isget=0,[m_count]},m_progress_isget=0,m_awardkind={[5]},m_awardnum={[5]},m_today=0,
function EveryDaySigninDlgRecv( recvValue )
	m_recvValue = recvValue
	EveryDaySigninDlgClear()
	
	m_totalNum = 0;
	for i=1, recvValue.m_count, 1 do
		local info = recvValue.m_list[i];
		EveryDaySigninDlgCreate( info, recvValue.m_today )
	end
	SetText( m_uiProgressNum, F( 4320, m_totalNum ) )
	EveryDaySigninDlgProgressCreate()
	
	-- 次日
	if recvValue.m_morrow_isget == 1 then
		SetFalse( m_uiMorrowLayer )
	else
		SetTrue( m_uiMorrowLayer )
		local herokind = recvValue.m_morrow_awardkind - AWARDKIND_HEROBASE;
		local color = hero_getnormalcolor( herokind )
		SetImage( m_uiMorrowLayer.transform:Find("Shape"), HeroFaceSprite(herokind) );
		SetText( m_uiMorrowLayer.transform:Find("Talk/Name"), HeroName(herokind), NameColor(color) )	
		SetText( m_uiMorrowLayer.transform:Find("Talk/Text"), T(4326) )
		
		if recvValue.m_morrow_time > 0 then
			SetTrue( m_uiMorrowLayer.transform:Find("Timer") )
			SetTimer( m_uiMorrowLayer.transform:Find("Timer"), recvValue.m_morrow_time, recvValue.m_morrow_time, 0 )
			SetButtonFalse( m_uiMorrowLayer.transform:Find("CallHeroBtn") )
		else
			SetFalse( m_uiMorrowLayer.transform:Find("Timer") )
			SetButtonTrue( m_uiMorrowLayer.transform:Find("CallHeroBtn") )
		end
	end
end

function EveryDaySigninDlgCreate( info, today )
	local uiObj = m_ObjectPool:Get( "UIP_Award" );
	uiObj.transform:SetParent( m_uiAwardContent.transform );
	uiObj.transform.localScale = Vector3.one;
	uiObj.gameObject:SetActive( true );
	
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiItem = objs[0]
	local uiShape = objs[1]
	local uiColor = objs[2]
	local uiNumBack = objs[3]
	local uiNum = objs[4]
	local uiName = objs[5]
	local uiFlag = objs[6]
	local uiEffect = objs[7]
	
	local sprite, color, name, c, desc = AwardInfo( info.m_awardkind )
	SetImage( uiShape, sprite );
	SetImage( uiColor, color );
	SetText( uiName, info.m_id )
	SetControlID( uiItem, 100+info.m_id )
	info.m_uiItem = uiObj
	
	if info.m_awardnum > 1 then
		SetTrue( uiNumBack )
		SetText( uiNum, knum(info.m_awardnum) )
	else
		SetFalse( uiNumBack )
		SetText( uiNum, "" )
	end
	
	if info.m_isget == 1 then
		SetTrue( uiFlag )
		m_totalNum = m_totalNum + 1
	else
		SetFalse( uiFlag )
	end
	
	if today == info.m_id then
		if info.m_isget == 1 then
			SetFalse( uiEffect )
			SetFalse( m_uiAwardGet )
		else
			SetTrue( uiEffect )
			SetTrue( m_uiAwardGet )
		end
	else
		SetFalse( uiEffect )
	end
end

function EveryDaySigninDlgClear()
	local objs = {};
	for i=0,m_uiAwardContent.transform.childCount-1 do
		table.insert(objs,m_uiAwardContent.transform:GetChild(i).gameObject);
	end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "UIP_Award(Clone)" then
			m_ObjectPool:Release( "UIP_Award", obj );
		end
	end
end

-- 进度
local numtable = { 3, 7, 14, 21, 28 }
function EveryDaySigninDlgProgressCreate()
	for i=1, 5, 1 do
		local uiObj = m_uiProgressAward.transform:GetChild( i-1 );
		SetText( uiObj.transform:Find("Name"), F(4325,numtable[i]) )
		SetControlID( uiObj, 1000+i )
		if m_totalNum >= numtable[i] then
			SetGray( uiObj.transform:Find("Back"), false )
		else
			SetGray( uiObj.transform:Find("Back"), true )
		end
		local sflag = Utils.get_int_sflag( m_recvValue.m_progress_isget, i )
		if sflag == 1 then
			SetTrue( uiObj.transform:Find("Flag") )
			SetFalse( uiObj.transform:Find("Effect") )
		else
			SetFalse( uiObj.transform:Find("Flag") )
			if m_totalNum >= numtable[i] then
				SetTrue( uiObj.transform:Find("Effect") )
			end
		end
	end
end

function EveryDaySigninDlgGetAward()
	system_askinfo( ASKINFO_EVERYDAY_SIGNIN, "", 1 )
end

function EveryDaySigninDlgClickItem( day, uiObj )
	local info = m_recvValue.m_list[day]
	if info == nil then
		return
	end

	-- 显示奖励描述
	if day > m_recvValue.m_today then
		EveryDaySigninDlgAwardShow( info, uiObj, 0 )
		
	-- 直接领取
	elseif day == m_recvValue.m_today then
		if info.m_isget == 0 then
			EveryDaySigninDlgGetAward()
		else
			EveryDaySigninDlgAwardShow( info, uiObj, 1 )
		end
		
	elseif day < m_recvValue.m_today then
		EveryDaySigninDlgAwardShow( info, uiObj, 1 )
	end
end

function EveryDaySigninDlgReset( day, uiObj )
	local info = m_recvValue.m_list[day]
	if info == nil then
		return
	end
	if info.m_isget == 0 then
		-- 补签
		MsgBox( F( 4322, info.m_token ), function()
			system_askinfo( ASKINFO_EVERYDAY_SIGNIN, "", 2, day )
		end )
	end
	EveryDaySigninDlgAwardDescLayer()
end

-- 奖励描述
function EveryDaySigninDlgAwardDescLayer()
	SetFalse( m_uiAwardDescLayer )
	m_AwardDescLayerShow = false
end
function EveryDaySigninDlgAwardShow( info, uiObj, step )
	EveryDaySigninDlgAwardDescLayer()
	EveryDaySigninDlgProgressLayer()
	if info == nil then
		return
	end
	
	if step == 0 then
		SetFalse( m_uiAwardDescLayer.transform:Find("ResetBtn") )
		SetFalse( m_uiAwardDescLayer.transform:Find("Has") )
	else
		if info.m_isget == 0 then
			SetTrue( m_uiAwardDescLayer.transform:Find("ResetBtn") )
			SetFalse( m_uiAwardDescLayer.transform:Find("Has") )
		else
			SetFalse( m_uiAwardDescLayer.transform:Find("ResetBtn") )
			SetTrue( m_uiAwardDescLayer.transform:Find("Has") )
		end
	end
	
	if info.m_awardkind > AWARDKIND_HEROBASE and info.m_awardkind < AWARDKIND_BUILDINGBASE then
		local herokind = info.m_awardkind - AWARDKIND_HEROBASE
		local color = hero_getnormalcolor( herokind )
		SetText( m_uiAwardDescLayer.transform:Find("Name"), HeroName(herokind), NameColor(color) )	
		SetText( m_uiAwardDescLayer.transform:Find("Desc"), "" )
	else
		m_uiAwardDescLayer.transform:SetParent( uiObj.transform )
		m_uiAwardDescLayer.transform.anchoredPosition = Vector2( 0, 80 )
		m_uiAwardDescLayer.transform:SetParent( m_uiWindow.transform )
		--print( m_uiAwardDescLayer.transform.anchoredPosition.x )
		--print( m_uiAwardDescLayer.transform.localPosition.x )
		if m_uiAwardDescLayer.transform.anchoredPosition.x < -300 then
			m_uiAwardDescLayer.transform.anchoredPosition = Vector2( -220, m_uiAwardDescLayer.transform.anchoredPosition.y )
		elseif m_uiAwardDescLayer.transform.anchoredPosition.x > 300 then
			m_uiAwardDescLayer.transform.anchoredPosition = Vector2( 220, m_uiAwardDescLayer.transform.anchoredPosition.y )
		end
		local _, _, name, c, desc = AwardInfo( info.m_awardkind )
		SetText( m_uiAwardDescLayer.transform:Find("Name"), name, NameColor(c) )	
		SetText( m_uiAwardDescLayer.transform:Find("Desc"), desc )
	end
	SetControlID( m_uiAwardDescLayer.transform:Find("ResetBtn"), 200+info.m_id )
	SetTrue( m_uiAwardDescLayer )
	m_AwardDescLayerShow = true
end

-- 点击进度
function EveryDaySigninDlgProgressLayer()
	SetFalse( m_uiAwardProgressLayer )
	m_AwardProgressLayerShow = false
end
function EveryDaySigninDlgClickProgress( index, uiObj )
	EveryDaySigninDlgAwardDescLayer()
	EveryDaySigninDlgProgressLayer()
	if m_recvValue == nil then
		return
	end
	
	local awardkind = m_recvValue.m_awardkind[index];
	if awardkind > AWARDKIND_HEROBASE and awardkind < AWARDKIND_BUILDINGBASE then
		local herokind = awardkind - AWARDKIND_HEROBASE
		local color = hero_getnormalcolor( herokind )
		SetImage( m_uiAwardProgressLayer.transform:Find("Shape"), HeroHeadSprite(herokind) )
		SetImage( m_uiAwardProgressLayer.transform:Find("Color"), ItemColorSprite(herokind) )
		SetText( m_uiAwardProgressLayer.transform:Find("Name"), HeroName(herokind), NameColor(color) )	
		SetText( m_uiAwardProgressLayer.transform:Find("Desc"), "" )
	else
		m_uiAwardProgressLayer.transform:SetParent( uiObj.transform )
		m_uiAwardProgressLayer.transform.anchoredPosition = Vector2( 0, 80 )
		m_uiAwardProgressLayer.transform:SetParent( m_uiWindow.transform )
		if m_uiAwardProgressLayer.transform.anchoredPosition.x < -280 then
			m_uiAwardProgressLayer.transform.anchoredPosition = Vector2( -200, m_uiAwardProgressLayer.transform.anchoredPosition.y )
		elseif m_uiAwardProgressLayer.transform.anchoredPosition.x > 280 then
			m_uiAwardProgressLayer.transform.anchoredPosition = Vector2( 200, m_uiAwardProgressLayer.transform.anchoredPosition.y )
		end
		local sprite, color, name, c, desc = AwardInfo( awardkind )
		SetImage( m_uiAwardProgressLayer.transform:Find("Shape"), sprite )
		SetImage( m_uiAwardProgressLayer.transform:Find("Color"), color )
		SetText( m_uiAwardProgressLayer.transform:Find("Name"), name, NameColor(c) )	
		SetText( m_uiAwardProgressLayer.transform:Find("Desc"), desc )
	end
	if m_totalNum >= numtable[index] then
		local sflag = Utils.get_int_sflag( m_recvValue.m_progress_isget, index )
		if sflag == 0 then
			SetTrue( m_uiAwardProgressLayer.transform:Find("GetBtn") )
			SetControlID( m_uiAwardProgressLayer.transform:Find("GetBtn"), 2000+index )
		else
			SetFalse( m_uiAwardProgressLayer.transform:Find("GetBtn") )
		end
	else
		SetFalse( m_uiAwardProgressLayer.transform:Find("GetBtn") )
	end
	SetTrue( m_uiAwardProgressLayer )
	m_AwardProgressLayerShow = true
end
function EveryDaySigninDlgProgressGetAward( index )
	system_askinfo( ASKINFO_EVERYDAY_SIGNIN, "", 3, index )
	EveryDaySigninDlgProgressLayer()
end

function EveryDaySigninDlgCallHero()
	if m_recvValue == nil then
		return
	end
	if m_recvValue.m_morrow_time > 0 then
		return
	end
	system_askinfo( ASKINFO_EVERYDAY_SIGNIN, "", 4 )
end
