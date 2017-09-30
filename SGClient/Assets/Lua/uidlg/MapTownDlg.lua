-- 界面
local m_Dlg = nil;
local m_uiTitle = nil; --UnityEngine.GameObject
local m_uiShape = nil; --UnityEngine.GameObject
local m_uiName = nil; --UnityEngine.GameObject
local m_uiPos = nil; --UnityEngine.GameObject
local m_uiOwn = nil; --UnityEngine.GameObject
local m_uiNation = nil; --UnityEngine.GameObject
local m_uiAwardWarn = nil; --UnityEngine.GameObject
local m_uiScroll = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiUIP_Award = nil; --UnityEngine.GameObject
local m_uiWarn = nil; --UnityEngine.GameObject
local m_uiOwnSec = nil; --UnityEngine.GameObject
local m_uiSoldiers = nil; --UnityEngine.GameObject
local m_uiLevyCost = nil; --UnityEngine.GameObject
local m_uiCloseButton = nil; --UnityEngine.GameObject
local m_uiLevyButton = nil; --UnityEngine.GameObject
local m_uiRebuildButton = nil; --UnityEngine.GameObject
local m_uiFightButton = nil; --UnityEngine.GameObject

local m_ObjectPool = nil;
local m_path = 0;
local m_LastRecvValue = nil;
local m_awardCache = {};

local m_nation = 0;
local m_townid = 0;
local m_produce_maxnum = 0;
local m_produce_maxsec = 0;


-- 打开界面
function MapTownDlgOpen()
	m_Dlg = eye.uiManager:Open( "MapTownDlg" );
end

-- 隐藏界面
function MapTownDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "MapTownDlg" );
end

-- 删除界面
function MapTownDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function MapTownDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            MapTownDlgClose();
        end
	end
end

-- 载入时调用
function MapTownDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiTitle = objs[0];
	m_uiShape = objs[1];
	m_uiName = objs[2];
	m_uiPos = objs[3];
	m_uiOwn = objs[4];
	m_uiNation = objs[5];
	m_uiAwardWarn = objs[6];
	m_uiScroll = objs[7];
	m_uiContent = objs[8];
	m_uiUIP_Award = objs[9];
	m_uiWarn = objs[10];
	m_uiOwnSec = objs[11];
	m_uiSoldiers = objs[12];
	m_uiLevyCost = objs[13];
	m_uiCloseButton = objs[14];
	m_uiLevyButton = objs[15];
	m_uiRebuildButton = objs[16];
	m_uiFightButton = objs[17];
	
	-- 对象池
	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );
	m_ObjectPool:CreatePool("UIP_Award", 5, 5, m_uiUIP_Award);
end

-- 界面初始化时调用
function MapTownDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function MapTownDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function MapTownDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function MapTownDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function MapTownDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function MapTownDlgShow( path, recvValue )
	MapTownDlgOpen()
	MapTownDlgAwardClear()
	SetFalse( m_uiOwnSec )
	SetFalse( m_uiSoldiers )
	SetFalse( m_uiLevyCost )
	SetFalse( m_uiCloseButton )
	SetFalse( m_uiLevyButton )
	SetFalse( m_uiRebuildButton )
	SetFalse( m_uiFightButton )
	
	m_path = path;
	m_LastRecvValue = recvValue;
	local posx 			= recvValue.m_posx;
	local posy 			= recvValue.m_posy;
	local custom_name	= recvValue.m_name;
	local custom_namelen= recvValue.m_namelen;
	m_nation 			= recvValue.m_char_value[1];
	m_townid 			= recvValue.m_short_value[1];
	local produce_num	= recvValue.m_short_value[2];
	local protect_sec	= recvValue.m_int_value[1];
	local produce_sec	= recvValue.m_int_value[2];
	local type 			= g_towninfo[m_townid].type
	local level 		= g_towninfo[m_townid].level
	m_produce_maxnum= g_towninfo[m_townid].produce_maxnum
	m_produce_maxsec= g_towninfo[m_townid].produce_maxsec
	
	-- 获取奖励
	if m_awardCache[m_townid] == nil then
		system_askinfo( ASKINFO_WORLDMAP, "", 10, m_townid );
	else
		MapTownDlgRecvAward( m_awardCache[kind] )
	end
	
	-- 城镇信息
	system_askinfo( ASKINFO_WORLDMAP, "", 11, m_townid );
	
	-- 点击城池
	if m_path == 0 then
		SetText( m_uiTitle, T(1274) )
		SetText( m_uiAwardWarn, T(1277) )
		SetText( m_uiWarn, F(1278, 21, math.floor(g_towninfo[m_townid].own_maxsec/86400) ) )
	-- 点击图纸
	elseif m_path == 1 then	
		SetText( m_uiTitle, T(1275) )
		SetText( m_uiAwardWarn, T(1276) )
		SetText( m_uiWarn, T(1283) )
	end
	
	-- 形象
	SetImage( m_uiShape, LoadSprite( MapUnitTownShapeList[type].."_"..m_nation ) )
	-- 名字
	if custom_namelen > 0 then
		SetText( m_uiName, custom_name )
	else
		SetText( m_uiName, MapTownName(m_townid) )
	end
	-- 位置
	SetText( m_uiPos, F(1272, posx, posy) )
	-- 所属
	if m_nation == 0 then
		SetText( m_uiOwn, T(951) );
	else
		SetText( m_uiOwn, T(1211)..NationEx(m_nation) );
	end
	-- 国旗
	SetImage( m_uiNation, NationSprite(m_nation) )
	
end

-- 清空
function MapTownDlgAwardClear()
	local objs = {};
	for i = 0 ,m_uiContent.transform.childCount - 1 do
		table.insert( objs, m_uiContent.transform:GetChild(i).gameObject )
    end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "UIP_Award(Clone)" then
			m_ObjectPool:Release( "UIP_Award", obj );
		end
    end
end

-- 获取奖励
-- m_count=0,m_list={m_kind=0,m_num=0,[m_count]},m_callback_code=0,m_value
function MapTownDlgRecvAward( recvValue )
	m_awardCache[recvValue.m_value] = recvValue;
	for i = 1 , recvValue.m_count do
		local uiObj = m_ObjectPool:Get( "UIP_Award" );
		uiObj.transform:SetParent( m_uiContent.transform );
		local sprite, color, name, c = AwardInfo( recvValue.m_list[i].m_kind );
		SetImage( uiObj.transform:Find("Shape"), sprite );
		SetImage( uiObj.transform:Find("Color"), color );
		SetText( uiObj.transform:Find("Name"), name, NameColor(c) );
    end
end

-- 获取详细信息
-- m_protect_sec=0,m_produce_sec=0,m_own_actorid=0,m_own_namelen=0,m_own_name="[m_own_namelen]",m_own_sec=0,m_hp=0,m_maxhp=0
function MapTownDlgRecvValue( recvValue )
	
	-- 城池详情
	if m_path == 0 then
		
		if m_nation == GetPlayer().m_nation then
			-- 如果在保护状态
			if recvValue.m_protect_sec > 0 then
				SetTrue( m_uiRebuildButton );
			else
				if recvValue.m_own_actorid <= 0 then
					SetTrue( m_uiRebuildButton );
				else
					SetTrue( m_uiOwn );
					SetText( m_uiOwn, T(1211)..recvValue.m_own_name );
					
					SetTrue( m_uiOwnSec );
					SetTimer( m_uiOwnSec, (m_produce_maxsec-recvValue.m_own_sec), m_produce_maxsec, 1, T(1280) );
					
					SetTrue( m_uiSoldiers );
					SetText( m_uiSoldiers, F(1281, recvValue.m_hp, recvValue.m_maxhp) );
				end
			end
			
		else
			-- 如果在保护状态
			if recvValue.m_protect_sec > 0 then
				SetTrue( m_uiCloseButton );
			else
				SetTrue( m_uiCloseButton );
				SetTrue( m_uiFightButton );
			end
		end
	
	-- 征收	
	elseif m_path == 1 then
		
		
		
	end	
end
