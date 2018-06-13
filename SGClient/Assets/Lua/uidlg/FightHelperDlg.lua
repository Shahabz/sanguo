-- 界面
local m_Dlg = nil;
local m_uiScrollView = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiUIP_Hero = nil; --UnityEngine.GameObject
local m_ObjectPool = nil;
local m_recvValue = nil;
local m_group_index = -1;


-- 打开界面
function FightHelperDlgOpen()
	m_Dlg = eye.uiManager:Open( "FightHelperDlg" );
end

-- 隐藏界面
function FightHelperDlgClose()
	if m_Dlg == nil then
		return;
	end
	eye.uiManager:Close( "FightHelperDlg" );
	m_recvValue = nil;
	m_group_index = -1;
	FightHelperDlgClear()
end

-- 删除界面
function FightHelperDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
	m_recvValue = nil;
	m_group_index = -1;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function FightHelperDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            FightHelperDlgClose();
		elseif nControlID >= 1000 and nControlID <= 2000 then
			FightHelperDlgBuy( nControlID-1000 )
        end
	end
end

-- 载入时调用
function FightHelperDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiScrollView = objs[0];
	m_uiContent = objs[1];
	m_uiUIP_Hero = objs[2];
	
	-- 对象池
	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );
	m_ObjectPool:CreatePool("UIP_Hero", 12, 12, m_uiUIP_Hero);
end

-- 界面初始化时调用
function FightHelperDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function FightHelperDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function FightHelperDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function FightHelperDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function FightHelperDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function FightHelperDlgShow( group_index )
	FightHelperDlgOpen()
	system_askinfo( ASKINFO_FIGHTHELPER, "", 0, group_index );
end

function FightHelperDlgRecv( recvValue )
end

-- {m_kind=0,m_shape=0,m_color=0,m_corps=0,m_attack=0,m_defense=0,m_troops=0,m_token=0,[m_count]},m_group_index=0,
function FightHelperDlgRecvList( recvValue )
	FightHelperDlgClear()
	m_group_index = recvValue.m_group_index
	m_recvValue = {}
	for i=1, recvValue.m_count, 1 do
		table.insert( m_recvValue, recvValue.m_list[i] )
	end
	table.sort( m_recvValue, function(a,b) 
		if a.m_sort < b.m_sort then
			return true
		end
	end )
	
	for i= 1, #m_recvValue, 1 do
		FightHelperDlgSet( m_recvValue[i] )
	end
end

function FightHelperDlgSet( info )
	local kind = info.m_kind;
	local uiObj = m_ObjectPool:Get("UIP_Hero");
	uiObj.transform:SetParent( m_uiContent.transform );
	uiObj.transform.localScale = Vector3.one;
	
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiShape = objs[0];
	local uiColor = objs[1]
	local uiCorps = objs[2];
	local uiName = objs[3];
	local uiAttack = objs[4];
	local uiDefense = objs[5];
	local uiSoldier = objs[6];
	local uiHasBuy = objs[7];
	local uiBuyBtn = objs[8];
	local uiToken = objs[9];
	
	SetControlID( uiBuyBtn, 1000+info.m_kind )
	SetImage( uiShape, EnemyHeadSprite( info.m_shape ) );
	SetImage( uiColor, ItemColorSprite( info.m_color ) );
	SetImage( uiCorps, CorpsSprite( info.m_corps ) );
	SetText( uiName,  Localization.text_item(10000+info.m_kind), NameColor(info.m_color) );
	SetText( uiToken.transform:Find("Text"), info.m_token );
	SetText( uiAttack,  T(146).."："..info.m_attack );
	SetText( uiDefense,  T(147).."："..info.m_defense )
	SetText( uiSoldier,  T(148).."："..info.m_troops )
	if info.m_buynum > 0 then
		SetFalse( uiBuyBtn )
		SetTrue( uiHasBuy )
		SetFalse( uiToken )
	else
		SetTrue( uiBuyBtn )
		SetFalse( uiHasBuy )
		SetTrue( uiToken )
	end
end

--清空
function FightHelperDlgClear()
	local objs = {};
	for i=0,m_uiContent.transform.childCount-1 do
		table.insert(objs,m_uiContent.transform:GetChild(i).gameObject);
	end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "UIP_Hero(Clone)" then
			m_ObjectPool:Release( "UIP_Hero", obj );
		end
	end
end

function FightHelperDlgBuy( kind )
	local token = 0;
	local color = 0;
	for i= 1, #m_recvValue, 1 do
		if m_recvValue[i].m_kind == kind then
			token = m_recvValue[i].m_token;
			color = m_recvValue[i].m_color;
			break;
		end
	end
	if token == 0 then
		return
	end
	local name = NameColorText( Localization.text_item(10000+kind), color )
	MsgBox( F(3237,token,name), function() 
		system_askinfo( ASKINFO_FIGHTHELPER, "", 1, m_group_index, kind );
	end )
end