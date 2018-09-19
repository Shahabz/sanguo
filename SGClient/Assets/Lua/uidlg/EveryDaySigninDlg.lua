-- 界面
local m_Dlg = nil;
local m_uiAwardScroll = nil; --UnityEngine.GameObject
local m_uiAwardContent = nil; --UnityEngine.GameObject
local m_uiUIP_Award = nil; --UnityEngine.GameObject
local m_uiAwardGet = nil; --UnityEngine.GameObject
local m_uiProgressAward = nil; --UnityEngine.GameObject
local m_uiProgressNum = nil; --UnityEngine.GameObject
local m_uiAwardDescLayer = nil; --UnityEngine.GameObject
local m_recvValue = nil
local m_totalNum = 0;
local m_ObjectPool = nil;

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
		elseif nControlID == 1 then	
			EveryDaySigninDlgGetAward()
		elseif nControlID >= 100 and nControlID < 200 then
			EveryDaySigninDlgReset( nControlID-100 )
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

function EveryDaySigninDlgProgressCreate( info )
	
end

function EveryDaySigninDlgGetAward()
	system_askinfo( ASKINFO_EVERYDAY_SIGNIN, "", 1 )
end

function EveryDaySigninDlgReset( day )
	local info = m_recvValue.m_list[day]
	if info == nil then
		return
	end
	
	-- 显示奖励描述
	if day > m_recvValue.m_today then
		
	-- 直接领取
	elseif day == m_recvValue.m_today then
		EveryDaySigninDlgGetAward()
		
	elseif day < m_recvValue.m_today then
		
		if info.m_isget == 1 then
			-- 显示奖励描述
			
		else
			-- 补签
			MsgBox( F( 4322, info.m_token ), function()
				system_askinfo( ASKINFO_EVERYDAY_SIGNIN, "", 2, day )
			end )
		end
	end
end

