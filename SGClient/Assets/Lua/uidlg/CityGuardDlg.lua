-- 界面
local m_Dlg = nil;
local m_DialogFrameMod = nil;
local m_uiGuardNum = nil; --UnityEngine.GameObject
local m_uiUIP_Guard = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiTimerText = nil; --UnityEngine.GameObject
local m_uiDescText = nil; --UnityEngine.GameObject
local m_uiCDButton = nil; --UnityEngine.GameObject
local m_uiCallButton = nil; --UnityEngine.GameObject
local m_level = 1;
local m_ObjectPool = nil;
local m_dataCache = {};
local m_uiCache = {};
local m_num = 0;

-- 打开界面
function CityGuardDlgOpen()
	m_Dlg = eye.uiManager:Open( "CityGuardDlg" );
	m_DialogFrameMod = DialogFrameModOpen( m_Dlg, "城墙"..m_level.."级", 4, CityGuardDlgClose );
end

-- 隐藏界面
function CityGuardDlgClose()
	if m_Dlg == nil then
		return;
	end
	local objs = {};
	for i = 0 ,m_uiContent.transform.childCount - 1 do
		table.insert( objs, m_uiContent.transform:GetChild(i).gameObject )
    end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "UIP_Guard(Clone)" then
			m_ObjectPool:Release( "UIP_Guard", obj );
		end
    end
	m_dataCache = {};
	m_uiCache = {};
	DialogFrameModClose( m_DialogFrameMod );
	m_DialogFrameMod = nil;
	eye.uiManager:Close( "CityGuardDlg" );
end

-- 删除界面
function CityGuardDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function CityGuardDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            CityGuardDlgClose();
        end
	end
end

-- 载入时调用
function CityGuardDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;
	m_uiGuardNum = objs[0];
	m_uiUIP_Guard = objs[1];
	m_uiContent = objs[2];
	m_uiTimerText = objs[3];
	m_uiDescText = objs[4];
	m_uiCDButton = objs[5];
	m_uiCallButton = objs[6];
	-- 对象池
	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );
	m_ObjectPool:CreatePool("UIP_Guard", m_level, m_level, m_uiUIP_Guard);
end

-- 界面初始化时调用
function CityGuardDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function CityGuardDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function CityGuardDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function CityGuardDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function CityGuardDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function CityGuardDlgShow()
	m_level = GetPlayer():BuildingLevel( BUILDING_Wall, -1 );
	m_level = 10
	CityGuardDlgOpen();
	CityGuardDlgCreateEmpty();
	system_askinfo( ASKINFO_CITYGUARD, "", 0 );
end

-- m_count=0,m_list={m_corps=0,m_color=0,m_shape=0,m_level=0,m_soldiers=0,m_troops=0,m_offset=0,[m_count]},m_guardsec=0,
function CityGuardDlgRecv( recvValue )
	m_dataCache = {};
	for i=1, recvValue.m_count, 1 do
		CityGuardDlgSet( recvValue.m_list[i] );
	end
	
	-- 城防军数量
	m_num = recvValue.m_count;
	SetText( m_uiGuardNum, m_num.."/"..m_level );
	
	-- 冷却时间
	CityGuardDlgChangeSec( recvValue )
end

function CityGuardDlgCreateEmpty()
	for i=1, m_level, 1 do
		local uiObj = m_ObjectPool:Get( "UIP_Guard" );
		uiObj.transform:SetParent( m_uiContent.transform );
		local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
		local uiShape = objs[0];
		local uiColor = objs[1];
		local uiCorps = objs[2];
		local uiLevel = objs[3];
		local uiArrow = objs[4];
		SetFalse( uiShape );
		SetFalse( uiColor );
		SetFalse( uiCorps );
		SetFalse( uiLevel );
		SetFalse( uiArrow );
		m_uiCache[i] = uiObj;
	end
end

function CityGuardDlgSet( recvValue )
	local offset = recvValue.m_offset;
	if offset < 0 then
		return;
	end
	if m_dataCache[offset] == nil then
		-- 新加城防军数量
		m_num = m_num + 1;
		SetText( m_uiGuardNum, m_num.."/"..m_level );
	end
	m_dataCache[offset] = recvValue;
	
	local uiObj = m_uiCache[offset];
	if uiObj == nil then
		uiObj = m_ObjectPool:Get( "UIP_Guard" );
		uiObj.transform:SetParent( m_uiContent.transform );
		m_uiCache[offset] = uiObj;	
	end
	
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiShape = objs[0];
	local uiColor = objs[1];
	local uiCorps = objs[2];
	local uiLevel = objs[3];
	local uiArrow = objs[4];
	SetTrue( uiShape );
	SetTrue( uiColor );
	SetTrue( uiCorps );
	SetTrue( uiLevel );
	SetTrue( uiArrow );
	SetImage( uiShape, GuardSprite( m_dataCache[offset].m_shape ) )
	SetImage( uiColor, HeroColorSprite( m_dataCache[offset].m_color ) )
	SetLevel( uiLevel, m_dataCache[offset].m_level )
end

function CityGuardDlgChangeSec( recvValue )
	-- 冷却时间
	if recvValue.m_guardsec > 0 then
		SetTrue( m_uiTimerText );
		SetTrue( m_uiDescText );
		SetTrue( m_uiCDButton );
		SetFalse( m_uiCallButton );
		SetTimer( m_uiTimerText, 1, recvValue.m_guardsec )
	else
		SetFalse( m_uiTimerText );
		SetFalse( m_uiDescText );
		SetFalse( m_uiCDButton );
		SetTrue( m_uiCallButton );
	end
end
