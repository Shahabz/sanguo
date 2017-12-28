-- 界面
local m_Dlg = nil;
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiUIP_Black = nil; --UnityEngine.GameObject

local BLACKLIST_CONTROLID = 100 ;
local BLACKLIST_REMOVECONTROLID = 1000;
local m_ObjectPool = nil; --缓存显示的每个黑名单物体
local m_recvValue = nil; --缓存黑名单信息

local t_BlackObjList = {};
local t_actorid = {};
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
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function BlackListDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            BlackListDlgClose();
		elseif nControlID > BLACKLIST_REMOVECONTROLID then
			BlackListDlgDelete(nControlID);
        end
	end
end

-- 载入时调用
function BlackListDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiContent = objs[0];
	m_uiUIP_Black = objs[1];
	
	-- 对象池
	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );
	m_ObjectPool:CreatePool("UIP_Black", 6, 6, m_uiUIP_Black);
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
		BlackListDlgRecv( m_recvValue )
	end
	
end

-- m_count=0,m_list={m_actorid=0,m_namelen=0,m_name="[m_namelen]",m_level=0,m_nation=0,[m_count]},
function BlackListDlgRecv( recvValue )
	if recvValue.m_count ==0 then
		return;
	end
	m_recvValue = recvValue;
	t_BlackObjList = {};
	t_actorid = {};
	for i=1, m_recvValue.m_count, 1 do
		BlackListDlgSetOneCell(i);
	end
end


-- 服务器通知添加一个人到黑名单
-- m_actorid=0,m_namelen=0,m_name="[m_namelen]",m_level=0,m_nation=0,
function BlackListDlgAdd( recvValue )
	if m_recvValue == nil then
		return
	end
	table.insert( m_recvValue.m_list, recvValue );
	m_recvValue.m_count = m_recvValue.m_count +1 ;
end

-- 客户端删除一个玩家
function BlackListDlgDel( actorid )
	system_askinfo( ASKINFO_ACTOR, "", 5, actorid );
	GetPlayer():DelBlacklist( actorid )
	if m_recvValue == nil then
		return
	end
	for i=1, m_recvValue.m_count do
		if m_recvValue.m_list[i].m_actorid == actorid then
			table.remove( m_recvValue.m_list, i );
			m_recvValue.m_count = m_recvValue.m_count -1 ;
			break;
		end
	end
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
--删除
function BlackListDlgDelete( nControlID )
	local objsId = nControlID - BLACKLIST_REMOVECONTROLID;
	if	t_BlackObjList[objsId+BLACKLIST_CONTROLID] == nil then
		return;
	end
	m_ObjectPool:Release("UIP_Black",t_BlackObjList[objsId+BLACKLIST_CONTROLID])
	BlackListDlgDel(t_actorid[objsId]);
end
--设置每一行
function BlackListDlgSetOneCell( index )
	local uiObj = m_ObjectPool:Get("UIP_Black");
	uiObj.transform:SetParent(m_uiContent.transform);
	uiObj.transform.localScale = Vector3.one;
	SetControlID(uiObj,BLACKLIST_CONTROLID + index);
	t_BlackObjList[BLACKLIST_CONTROLID + index] = uiObj;
	local blackRecvList = m_recvValue.m_list[index];
	t_actorid[index] = blackRecvList.m_actorid;
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local levelText = objs[0];
	local heroText = objs[1];
	local HeroPicture = objs[2]
	local nationPicture = objs[3];
	local removeBtn = objs[4];
	SetText(heroText,blackRecvList.m_name);
	SetLevel(levelText,blackRecvList.m_level);
	SetImage(HeroPicture,PlayerHeadSprite(blackRecvList.m_shape));
	SetImage(nationPicture,NationSprite(blackRecvList.m_nation));
	SetControlID(removeBtn,BLACKLIST_REMOVECONTROLID + index);
end





