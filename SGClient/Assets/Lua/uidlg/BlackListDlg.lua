-- 界面
local m_Dlg = nil;
local m_uiScrollView = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiUIP_Black = nil; --UnityEngine.GameObject

local m_ObjectPool = nil;
local m_recvValue = nil;

-- 打开界面
function BlackListDlgOpen()
	m_Dlg = eye.uiManager:Open( "BlackListDlg" );
end

-- 隐藏界面
function BlackListDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "BlackListDlg" );
end

-- 删除界面
function BlackListDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
	m_recvValue = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function BlackListDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            BlackListDlgClose();
		elseif nControlID >= 1000 then
			BlackListDlgDelete( nControlID-1000 );
        end
	end
end

-- 载入时调用
function BlackListDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiScrollView = objs[0];
	m_uiContent = objs[1];
	m_uiUIP_Black = objs[2];
	
	-- 对象池
	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );
	m_ObjectPool:CreatePool("UIP_Black", 2, 2, m_uiUIP_Black);
end

-- 界面初始化时调用
function BlackListDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function BlackListDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function BlackListDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function BlackListDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function BlackListDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function BlackListDlgShow()
	BlackListDlgOpen()
	BlackListDlgClear();
	if m_recvValue == nil then
		system_askinfo( ASKINFO_ACTOR, "", 3 );
	else
		BlackListDlgEndRecv()
	end
end

-- 开始接收
function BlackListDlgBegin()
	m_recvValue={}
end

-- m_count=0,m_list={m_actorid=0,m_namelen=0,m_name="[m_namelen]",m_level=0,m_nation=0,[m_count]},
function BlackListDlgRecv( recvValue )
	for i=1, recvValue.m_count, 1 do
		table.insert( m_recvValue, recvValue.m_list[i] );
	end
end

-- 接收完毕
function BlackListDlgEndRecv()
	for i=1, #m_recvValue, 1 do
		BlackListDlgSetObj( i, m_recvValue[i] );
	end
end

--设置每一行
function BlackListDlgSetObj( index, info )
	local uiObj = m_ObjectPool:Get("UIP_Black");
	uiObj.transform:SetParent( m_uiContent.transform );
	uiObj.transform.localScale = Vector3.one;
	info.m_uiObj = uiObj;
	info.m_index = index;
	
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiShape = objs[0];
	local uiName = objs[1]
	local uiNation = objs[2];
	local uiRemoveBtn = objs[3];
	
	SetImage( uiShape, PlayerHeadSprite( info.m_shape ) );
	SetImage( uiNation, NationSprite( info.m_nation ) );
	SetLevel( uiName, info.m_level.." "..info.m_name );
	SetControlID( uiRemoveBtn, 1000 + index );
end

--清空
function BlackListDlgClear()
	local objs = {};
	for i=0,m_uiContent.transform.childCount-1 do
		table.insert(objs,m_uiContent.transform:GetChild(i).gameObject);
	end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "UIP_Black(Clone)" then
			m_ObjectPool:Release( "UIP_Black", obj );
		end
	end
end

-- 服务器通知添加一个人到黑名单
-- m_actorid=0,m_namelen=0,m_name="[m_namelen]",m_level=0,m_nation=0,
function BlackListDlgAdd( recvValue )
	if m_recvValue == nil then
		m_recvValue = {}
	end
	table.insert( m_recvValue, recvValue );
end

-- 客户端删除一个玩家
function BlackListDlgDel( actorid )
	system_askinfo( ASKINFO_ACTOR, "", 5, actorid );
	GetPlayer():DelBlacklist( actorid )
	if m_recvValue == nil then
		return
	end
	for i=1, #m_recvValue do
		if m_recvValue[i].m_actorid == actorid then
			table.remove( m_recvValue, i );
			break;
		end
	end
	BlackListDlgClear()
	BlackListDlgEndRecv()
end

--删除
function BlackListDlgDelete( index )
	if m_recvValue[index] == nil then
		BlackListDlgClose()
		return
	end
	m_ObjectPool:Release( "UIP_Black", m_recvValue[index].m_uiObj )
	BlackListDlgDel( m_recvValue[index].m_actorid );
end
