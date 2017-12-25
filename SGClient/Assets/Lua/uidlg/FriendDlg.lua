-- 界面
local m_Dlg = nil;
local m_DialogFrameMod = nil;

local m_uiUIP_Friend = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiFrendsLayer = nil; --UnityEngine.GameObject
local m_uiClickShowIcon = nil; --UnityEngine.GameObject
local m_uiScroll = nil; --UnityEngine.GameObject

local m_ObjectPool = nil;
local m_uiFriendObj = {};
local FRIENDLIST_BUTTONID = 1000;
local FRIENDLIST_REFUSE = 2000; --拒绝
local FRIENDLIST_ACCEPT = 3000; -- 同意
local t_actorid = {} --存储m_actorid和m_city_index值 点击详细信息时传入，如果m_city_index为nil就只传m_actorid
local t_selectActorid = nil; --存储选中好友的m_actorid和m_city_index值，
local t_recvValue = {} --缓存好友列表

-- m_op=0,m_target_actorid=0,m_target_cityindex=0,m_target_namelen=0,m_target_name="[m_target_namelen]",
local sendValue = {};
sendValue.m_op = 0;
sendValue.m_target_actorid = 0;
sendValue.m_target_cityindex = 0;
sendValue.m_target_namelen = 0;
sendValue.m_target_name = "";
	
-- 打开界面
function FriendDlgOpen()
	m_Dlg = eye.uiManager:Open( "FriendDlg" );
	m_DialogFrameMod = DialogFrameModOpen( m_Dlg, T(557), HELP_ChatDlg, FriendDlgClose );
end

-- 隐藏界面
function FriendDlgClose()
	if m_Dlg == nil then
		return;
	end
	DialogFrameModClose( m_DialogFrameMod );
	m_DialogFrameMod = nil;
	eye.uiManager:Close( "FriendDlg" );
end

-- 删除界面
function FriendDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function FriendDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            FriendDlgClose();
		elseif	nControlID	== 1 then --关闭好友列表进阶操作面
			FriendDlgCloseClickShow();
		elseif nControlID == 2 then --查看好友信息
			if IsActive(m_uiClickShowIcon) then
				FriendDlgCheckInfo();
			end
		elseif nControlID == 3 then --给好友发邮件
			
		elseif nControlID == 4 then --删除好友
		
		elseif nControlID == 5 then --获取好友列表
			FriendDlgClear();
			FriendDlgAskList();
		elseif nControlID == 6 then --添加好友
			FriendDlgAddFriends();
		elseif nControlID < FRIENDLIST_REFUSE then
			FriendDlgClickShow(nControlID);
		elseif nControlID < FRIENDLIST_ACCEPT then
			FriendDlgRefuse(nControlID)
		elseif	nControlID > FRIENDLIST_ACCEPT then
			FriendDlgAccept(nControlID)	
        end
	elseif	nType ==UI_EVENT_SCROLLDRAG then
		if value == 0 then
			if	nControlID == 1 then
				FriendDlgCloseClickShow();
			end
		end
	end
end

-- 载入时调用
function FriendDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiUIP_Friend = objs[0];
	m_uiContent = objs[1];
	m_uiFrendsLayer = objs[2];
	m_uiClickShowIcon = objs[3];
	m_uiScroll = objs[4];
	-- 对象池
	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );
	m_ObjectPool:CreatePool("UIP_Friend", 6, 6, m_uiUIP_Friend);
end

-- 界面初始化时调用
function FriendDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function FriendDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function FriendDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function FriendDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function FriendDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function FriendDlgShow()
	FriendDlgOpen()
	FriendDlgClear()
	FriendDlgAskList()
end

-- m_count=0,m_list={m_actorid=0,m_city_index=0,m_shape=0,m_namelen=0,m_name="[m_namelen]",m_level=0,m_place=0,m_battlepower=0,m_ask=0,[m_count]},
function FriendDlgRecv( recvValue )

	table.sort(recvValue.m_list,function(a,b)
				local r
				local al = tonumber(a.m_level);
				local bl = tonumber(b.m_level);
				local at = tonumber(a.m_battlepower);
				local bt = tonumber(b.m_battlepower);
				local as = tonumber(a.m_ask);
				local bs = tonumber(b.m_ask);
				if as == bs then
					if al == bl then
						r = at > bt
					else
						r = al > bl
					end
				else
					r = as > bs
				end
				return r 
		end);
	t_recvValue = recvValue;
	for i=1,#t_recvValue.m_list,1 do
		local uiObj = m_ObjectPool:Get( "UIP_Friend" );
		uiObj.transform:SetParent( m_uiContent.transform );	
		SetControlID(uiObj,FRIENDLIST_BUTTONID + i);
		m_uiFriendObj[FRIENDLIST_BUTTONID + i] = uiObj;
		local recvList = t_recvValue.m_list[i];
		local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
		local uiHeroPic = objs[0]
		local uiPlacePic = objs[1]
		local uiLevelText = objs[2]
		local uiHeroNameText = objs[3]
		local uiBattleText = objs[4]
		local uiAcceptBtn = objs[5]
		local uiRefuseBtn = objs[6]
		t_actorid[FRIENDLIST_BUTTONID + i] ={m_actorid=recvList.m_actorid,m_city_index=recvList.m_city_index}
		SetImage(uiHeroPic,PlayerHeadSprite(recvList.m_shape));
		SetImage(uiPlacePic,PlaceSprite(recvList.m_place));
		SetLevel(uiLevelText,recvList.m_level);
		SetText(uiHeroNameText,recvList.m_name);
		SetText(uiBattleText,F(1990,recvList.m_battlepower));
		if recvList.m_ask == 0 then
			SetFalse(uiAcceptBtn)
			SetFalse(uiRefuseBtn)
		else
			SetTrue(uiAcceptBtn)
			SetTrue(uiRefuseBtn)
			SetControlID(uiAcceptBtn,FRIENDLIST_ACCEPT + i);
			SetControlID(uiRefuseBtn,FRIENDLIST_ACCEPT + i);
		end
	end
end

-- 获取列表
function FriendDlgAskList()
	sendValue.m_op = 0;
	sendValue.m_target_actorid = 0;
	sendValue.m_target_cityindex = -1;
	sendValue.m_target_namelen = 0;
	sendValue.m_target_name = "";
	netsend_friendop_C( sendValue )
end

-- 同意
function FriendDlgAgree( actorid, cityindex )
	sendValue.m_op = 2;
	sendValue.m_target_actorid = actorid;
	sendValue.m_target_cityindex = cityindex;
	sendValue.m_target_namelen = 0;
	sendValue.m_target_name = "";
	netsend_friendop_C( sendValue )
end

-- 拒绝
function FriendDlgReject( actorid, cityindex )
	sendValue.m_op = 3;
	sendValue.m_target_actorid = actorid;
	sendValue.m_target_cityindex = cityindex;
	sendValue.m_target_namelen = 0;
	sendValue.m_target_name = "";
	netsend_friendop_C( sendValue )
end

-- 删除
function FriendDlgDelete( actorid, cityindex )
	sendValue.m_op = 4;
	sendValue.m_target_actorid = actorid;
	sendValue.m_target_cityindex = cityindex;
	sendValue.m_target_namelen = 0;
	sendValue.m_target_name = "";
	netsend_friendop_C( sendValue )
end
--点击好友列表弹出进阶操作
function FriendDlgClickShow( ncontrolid )
	t_selectActorid = t_actorid[ncontrolid];
	local parentObj = m_uiFriendObj[ncontrolid];
	m_uiClickShowIcon.transform:SetParent(parentObj.transform);
	m_uiClickShowIcon.transform.anchoredPosition = Vector2(150,-70);
	m_uiClickShowIcon.transform:SetParent(m_uiFrendsLayer.transform);
	SetTrue(m_uiClickShowIcon);
end
--关闭好友列表进阶操作
function FriendDlgCloseClickShow()
	if	IsActive(m_uiClickShowIcon) then
		SetFalse(m_uiClickShowIcon);
		t_selectActorid = nil ;
	end
end
--点击每个排行玩家查看详细信息
function FriendDlgCheckInfo()
	if t_selectActorid == nil then
		print("t_selectActorid==nil");
		return;
	end
	if t_selectActorid.m_city_index == nil then
		ActorSearchDlgShow(t_selectActorid.m_actorid);
	else
		ActorSearchDlgShow(t_selectActorid.m_actorid ,t_selectActorid.m_city_index )
	end
end
--从好友列表中删除
function FriendDlgDeleteFriend()
	if t_selectActorid == nil then
		print("t_selectActorid==nil");
		return;
	end
	local actorid = t_selectActorid.m_actorid;
	local city_index = t_selectActorid.m_city_index;
	FriendDlgDelete(actorid,city_index);
end
--向好友发邮件
function FriendDlgSendMail()
	if t_selectActorid == nil then
		return;
	end
	local actorid = t_selectActorid.m_actorid;
	local city_index = t_selectActorid.m_city_index;
end
--同意
function FriendDlgAccept( ncontrolid )
	local listid = ncontrolid - FRIENDLIST_ACCEPT;
	t_recvValue.m_list[listid].m_ask = 0;
	FriendDlgAgree(t_actorid[FRIENDLIST_BUTTONID + listid].m_actorid,t_actorid[FRIENDLIST_BUTTONID + listid].m_city_index );
	FriendDlgClear();
	FriendDlgRecv(t_recvValue);
end
--拒绝
function FriendDlgRefuse( ncontrolid )
	local listid = ncontrolid - FRIENDLIST_REFUSE;
	table.remove(t_recvValue.m_list,listid);
	FriendDlgReject(t_actorid[FRIENDLIST_BUTTONID + listid].m_actorid,t_actorid[FRIENDLIST_BUTTONID + listid].m_city_index );
	FriendDlgClear();
	FriendDlgRecv(t_recvValue);	
end
-- 清空
function FriendDlgClear()
	local objs = {};
	for i = 0 ,m_uiContent.transform.childCount - 1 do
		table.insert( objs, m_uiContent.transform:GetChild(i).gameObject )
    end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "UIP_Friend(Clone)" then
			m_ObjectPool:Release( "UIP_Friend", obj );
		end
    end
	m_uiScroll:GetComponent("UIScrollRect"):ResetScroll();
	m_uiContent.transform.localPosition = Vector2.zero
	m_uiFriendObj ={}; --清空好友列表Obj
	t_actorid = {};	--清空好友列表m_actorid和m_city_index值 
end
--添加好友
function FriendDlgAddFriends()
	print("AddFriends");
end










