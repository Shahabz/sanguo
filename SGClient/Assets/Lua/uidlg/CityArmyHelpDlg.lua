-- 界面
local m_Dlg = nil;
local m_uiScroll = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiUIP_Army = nil; --UnityEngine.GameObject
local m_uiHeros = nil; --UnityEngine.GameObject
local m_uiSoldiers = nil; --UnityEngine.GameObject

local m_ObjectPool = nil;
local m_recvValue = nil;

-- 打开界面
function CityArmyHelpDlgOpen()
	m_Dlg = eye.uiManager:Open( "CityArmyHelpDlg" );
end

-- 隐藏界面
function CityArmyHelpDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "CityArmyHelpDlg" );
end

-- 删除界面
function CityArmyHelpDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function CityArmyHelpDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            CityArmyHelpDlgClose();
		elseif nControlID >= 1000 then
			CityArmyHelpDlgReturn( nControlID-1000 )
        end
	end
end

-- 载入时调用
function CityArmyHelpDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiScroll = objs[0];
	m_uiContent = objs[1];
	m_uiUIP_Army = objs[2];
	m_uiHeros = objs[3];
	m_uiSoldiers = objs[4];
	-- 对象池
	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );
	m_ObjectPool:CreatePool("UIP_Army", 5, 5, m_uiUIP_Army);
end

-- 界面初始化时调用
function CityArmyHelpDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function CityArmyHelpDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function CityArmyHelpDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function CityArmyHelpDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function CityArmyHelpDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function CityArmyHelpDlgShow()
	CityArmyHelpDlgOpen()
	CityArmyHelpDlgClear()
	-- 获取列表
	system_askinfo( ASKINFO_CITYHELP, "", 0, -1 );
end

-- 清空单元
function CityArmyHelpDlgClear()
	local objs = {};
	for i = 0 ,m_uiContent.transform.childCount - 1 do
		table.insert( objs, m_uiContent.transform:GetChild(i).gameObject )
    end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "UIP_Army(Clone)" then
			m_ObjectPool:Release( "UIP_Army", obj );
		end
    end
end

-- 接收信息
-- m_count=0,m_list={m_level=0,m_actorname_length=0,m_actorname="[m_actorname_length]",m_herokind=0,m_soldiers=0,m_actorid=0,m_army_index=0,[m_count]},
function CityArmyHelpDlgRecv( recvValue )
	local total = 0;
	for i=1, recvValue.m_count, 1 do
		CityArmyHelpDlgCreateArmy( i, recvValue.m_list[i] )
		total = total + recvValue.m_list[i].m_soldiers;
	end
	-- 总部队
	SetText( m_uiHeros, T(1233)..":"..recvValue.m_count.."/"..recvValue.m_walllevel );
	-- 总兵力
	SetText( m_uiSoldiers, T(1245)..":"..total );
end
function CityArmyHelpDlgCreateArmy( index, info )
	local uiObj = m_ObjectPool:Get( "UIP_Army" );
	uiObj.transform:SetParent( m_uiContent.transform );
	local objs = uiObj:GetComponent( typeof(Reference) ).relatedGameObject;
	uiLevel = objs[0];
	uiActorName = objs[1];
	uiHeroName = objs[2];
	uiSoldiers = objs[3];
	uiButton = objs[4];
	uiSort = objs[5];
	SetText( uiSort, index );
	SetText( uiLevel, info.m_level );
	SetText( uiActorName, info.m_actorname );
	SetText( uiHeroName, HeroName(info.m_herokind) );
	SetText( uiSoldiers, info.m_soldiers );

	SetTrue( uiButton );
	SetControlID( uiButton, 1000+info.m_army_index )
end

-- 撤防
function  CityArmyHelpDlgReturn( army_index )
	system_askinfo( ASKINFO_WORLDMAP, "", 9, army_index );
	CityArmyHelpDlgShow()
end
