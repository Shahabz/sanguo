-- 界面
local m_Dlg = nil;
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiUIP_Black = nil; --UnityEngine.GameObject

local BlackList_CONTROLID = 100 ;
local BlackList_REMOVECONTROLID = 1000;
local m_ObjectPool = nil;
local m_recvValue = nil;

local t_BlackObjList = {};
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
		elseif nControlID > BlackList_REMOVECONTROLID then
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
	--[[
	local recvValue ={};
	recvValue.m_count=10;
	recvValue.m_list ={
		{m_actorid=1,m_name="sdfdfsdf",m_level=17,m_nation=1,m_shape=1},
		{m_actorid=2,m_name="sdfsdfsdf",m_level=162,m_nation=2,m_shape=2},
		{m_actorid=3,m_name="dsccc",m_level=1772,m_nation=3,m_shape=3},
		{m_actorid=4,m_name="xxxxxxx",m_level=232,m_nation=1,m_shape=4},
		{m_actorid=5,m_name="zzzzzzz",m_level=343,m_nation=2,m_shape=5},
		{m_actorid=6,m_name="mmmmmmmm",m_level=11,m_nation=3,m_shape=6},
		{m_actorid=7,m_name="gggggggg",m_level=22,m_nation=1,m_shape=7},
		{m_actorid=8,m_name="hhhhhhhh",m_level=33,m_nation=2,m_shape=8},
		{m_actorid=9,m_name="jjjjjjjj",m_level=44,m_nation=3,m_shape=9},
		{m_actorid=10,m_name="ttttttt",m_level=55,m_nation=1,m_shape=10},
		{m_actorid=11,m_name="rrrrrrr",m_level=66,m_nation=2,m_shape=11},
	}
	BlackListDlgRecv(recvValue);
	--]]
	
	if m_recvValue == nil then
		system_askinfo( ASKINFO_ACTOR, "", 3 );
	else
		BlackListDlgRecv( m_recvValue )
	end
	
end

-- m_count=0,m_list={m_actorid=0,m_namelen=0,m_name="[m_namelen]",m_level=0,m_nation=0,[m_count]},
function BlackListDlgRecv( recvValue )
	m_recvValue = recvValue;
	t_BlackObjList = {};
	for i=1, m_recvValue.m_count, 1 do
		local uiObj = m_ObjectPool:Get("UIP_Black");
		uiObj.transform:SetParent(m_uiContent.transform);
		SetControlID(uiObj,BlackList_CONTROLID + i);
		t_BlackObjList[BlackList_CONTROLID + i] = uiObj;
		local blackRecvList = recvValue.m_list[i];
		local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
		local levelText = objs[0];
		local heroText = objs[1];
		local HeroPicture = objs[2]
		local nationPicture = objs[3];
		local removeBtn = objs[4];
		SetText(levelText,blackRecvList.m_name);
		SetLevel(heroText,blackRecvList.m_level);
		SetImage(HeroPicture,PlayerHeadSprite(blackRecvList.m_shape));
		SetImage(nationPicture,NationSprite(blackRecvList.m_nation));
		SetControlID(removeBtn,BlackList_REMOVECONTROLID + i);
	end
end


-- 服务器通知添加一个人到黑名单
-- m_actorid=0,m_namelen=0,m_name="[m_namelen]",m_level=0,m_nation=0,
function BlackListDlgAdd( recvValue )
	if m_recvValue == nil then
		return
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
	for i=1, m_recvValue.m_count do
		if m_recvValue.m_list[i].m_actorid == actorid then
			table.remove( m_recvValue, i );
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
	local objsId = nControlID - BlackList_REMOVECONTROLID;
	if	t_BlackObjList[objsId+BlackList_CONTROLID] == nil then
		return;
	end
	destroy(t_BlackObjList[objsId+BlackList_CONTROLID]);
	BlackListDlgDel(m_recvValue.m_list[objsId].m_actorid);
end





