-- 界面
local m_Dlg = nil;
--local m_DialogFrameMod = nil;
local m_uiScroll = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiUIP_Rank = nil; --UnityEngine.GameObject

local m_ObjectPool = nil
local m_recvValue = nil;
local m_repeatCache = nil;
-- 打开界面
function NationOfficialReplaceDlgOpen()
	m_Dlg = eye.uiManager:Open( "NationOfficialReplaceDlg" );
	--m_DialogFrameMod = DialogFrameModOpen( m_Dlg, T(0), nil, NationOfficialReplaceDlgClose );
end

-- 隐藏界面
function NationOfficialReplaceDlgClose()
	if m_Dlg == nil then
		return;
	end
	--DialogFrameModClose( m_DialogFrameMod );
	--m_DialogFrameMod = nil;
	eye.uiManager:Close( "NationOfficialReplaceDlg" );
	m_recvValue = nil;
	m_repeatCache = nil;
end

-- 删除界面
function NationOfficialReplaceDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
	m_recvValue = nil;
	m_repeatCache = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function NationOfficialReplaceDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            NationOfficialReplaceDlgClose();
			
		-- 任命	
		elseif nControlID >= 1000 and nControlID < 2000 then
			NationOfficialReplaceDlgUp( nControlID - 1000 )
			
		-- 罢免
		elseif nControlID >= 2000 and nControlID < 3000 then
			NationOfficialReplaceDlgDown( nControlID - 2000 )
			
        end
	end
end

-- 载入时调用
function NationOfficialReplaceDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiScroll = objs[0];
	m_uiContent = objs[1];
	m_uiUIP_Rank = objs[2];
	-- 对象池
	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );
	m_ObjectPool:CreatePool("UIP_Rank", 10, 10, m_uiUIP_Rank);
end

-- 界面初始化时调用
function NationOfficialReplaceDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function NationOfficialReplaceDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function NationOfficialReplaceDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function NationOfficialReplaceDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function NationOfficialReplaceDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function NationOfficialReplaceDlgShow()
	NationOfficialReplaceDlgOpen()
	system_askinfo( ASKINFO_NATION, "", 14 )
end

-- 数据接收开始
function NationOfficialReplaceDlgRecvBegin()
	NationOfficialReplaceDlgClear()
	m_recvValue = {}
	m_repeatCache = {}
end

-- 数据接收过程
-- m_count=0,m_list={m_namelen=0,m_name="[m_namelen]",m_level=0,m_battlepower=0,m_place=0,m_official=0,m_actorid=0,[m_count]},m_op=0,
function NationOfficialReplaceDlgRecv( recvValue )
	for i=1, recvValue.m_count, 1 do
		table.insert( m_recvValue, recvValue.m_list[i] )
	end
end

-- 数据接收完毕
function NationOfficialReplaceDlgRecvEnd()
	if #m_recvValue == 0 then
		return
	end
	for i=1, #m_recvValue, 1 do
		NationOfficialReplaceDlgCreate( i, m_recvValue[i] )
	end
end


-- 创建
function NationOfficialReplaceDlgCreate( index, info )
	if m_repeatCache[info.m_actorid] ~= nil then
		return
	end
	if info.m_official >= 1 and info.m_official <= 5 then
		return
	end
	m_repeatCache[info.m_actorid] = info.m_actorid;
	local uiObj = m_ObjectPool:Get( "UIP_Rank" );
	uiObj.transform:SetParent( m_uiContent.transform );
	SetText( uiObj.transform:Find("Place"), PlaceName( info.m_place ) )
	SetText( uiObj.transform:Find("Name"), info.m_name )
	SetText( uiObj.transform:Find("Level"), info.m_level )
	SetText( uiObj.transform:Find("Battlepower"), info.m_battlepower )
	if info.m_official == 0 then
		SetTrue( uiObj.transform:Find("UpBtn") )
		SetFalse( uiObj.transform:Find("DownBtn") )
		SetControlID( uiObj.transform:Find("UpBtn"), 1000+index )
	else
		SetFalse( uiObj.transform:Find("UpBtn") )
		SetTrue( uiObj.transform:Find("DownBtn") )
		SetControlID( uiObj.transform:Find("DownBtn"), 2000+index )
	end
end

-- 清空
function NationOfficialReplaceDlgClear()
	local objs = {};
	for i = 0 ,m_uiContent.transform.childCount - 1 do
		table.insert( objs, m_uiContent.transform:GetChild(i).gameObject )
    end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "UIP_Rank(Clone)" then
			m_ObjectPool:Release( "UIP_Rank", obj );
		end
    end
end

-- 任命
function NationOfficialReplaceDlgUp( index )
	local info = m_recvValue[index]
	if info == nil then
		return
	end
	system_askinfo( ASKINFO_NATION, "", 15, info.m_actorid )
end

-- 罢免
function NationOfficialReplaceDlgDown( index )
	local info = m_recvValue[index]
	if info == nil then
		return
	end
		print( index )
	system_askinfo( ASKINFO_NATION, "", 16, info.m_actorid )
end

