-- 界面
local m_Dlg = nil;
local m_DialogFrameMod = nil;

local m_uiFriendBtn = nil; --UnityEngine.GameObject
local m_uiTeacherBtn = nil; --UnityEngine.GameObject
local m_uiAddFriendBtn = nil; --UnityEngine.GameObject
local m_uiFriendScroll = nil; --UnityEngine.GameObject
local m_uiFriendContent = nil; --UnityEngine.GameObject
local m_uiUIP_Friend = nil; --UnityEngine.GameObject
local m_uiAddFriendLayer = nil; --UnityEngine.GameObject
local m_uiNameInput = nil; --UnityEngine.GameObject
local m_uiOpLayer = nil; --UnityEngine.GameObject
local m_uiTeacherBtnName = nil;
local m_uiTeacherScroll = nil;
local m_uiApprenticeLayer = nil;
local m_uiTeacherTitle = nil;
local m_uiTeacherListBtn = nil;
local m_uiTeacherShopBtn = nil;

local m_ObjectPool = nil
local m_recvValue = nil
local m_selectRecvActor = nil
local m_SelectType = nil;
local m_TeacherType = nil;

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
	m_recvValue = nil;
end

-- 删除界面
function FriendDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
	m_recvValue = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function FriendDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            FriendDlgClose();
		
		-- 关闭添加好友页	
		elseif nControlID == -2 then
            FriendDlgAddFriendLayerHide();
		
		-- 关闭好友操作页	
		elseif nControlID == -3 then 
			FriendDlgOpLayerHide();
		
		-- 查看好友信息	
		elseif nControlID == 1 then
			FriendDlgOpView();
		-- 发送邮件 	
		elseif nControlID == 2 then
			FriendDlgOpSendMail();
		
		-- 删除好友	
		elseif nControlID == 3 then
			FriendDlgOpDelete();
		
		-- 好友列表	
		elseif nControlID == 4 then
			FriendDlgAskList();
		
		-- 师徒列表	
		elseif nControlID == 5 then
			FriendDlgAskTeacherList();
				
		-- 打开添加好友页
		elseif nControlID == 6 then
			FriendDlgAddFriendLayerShow();
		
		-- 添加好友		
		elseif nControlID == 7 then
			FriendDlgAddFriend();
		
		-- 师徒商店
		elseif nControlID == 50 then
			
		-- 选择好友拜师		
		elseif nControlID == 51 then

		-- 领取拜师奖励		
		elseif nControlID == 52 then	
		
		-- 师徒列表
		elseif nControlID == 53 then
			
		-- 显示操作页	
		elseif nControlID >= 1000 and nControlID < 2000 then
			FriendDlgOpLayerShow( nControlID-1000, value );
		
		-- 接受
		elseif nControlID >= 2000 and nControlID < 3000 then
			FriendDlgAccept( nControlID - 2000 );
		
		-- 拒绝	
		elseif nControlID >= 3000 and nControlID < 4000 then
			FriendDlgRefuse( nControlID - 3000 );
        end
	elseif nType == UI_EVENT_SCROLLDRAG then
		if nControlID == 1 then
			if IsActive( m_uiOpLayer ) == true then
				FriendDlgOpLayerHide();
			end
		end
	end
end

-- 载入时调用
function FriendDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiFriendBtn = objs[0];
	m_uiTeacherBtn = objs[1];
	m_uiAddFriendBtn = objs[2];
	m_uiFriendScroll = objs[3];
	m_uiFriendContent = objs[4];
	m_uiUIP_Friend = objs[5];
	m_uiAddFriendLayer = objs[6];
	m_uiNameInput = objs[7];
	m_uiOpLayer = objs[8];
	--师徒
	m_uiTeacherBtnName = objs[9];
	m_uiTeacherScroll = objs[10];
	m_uiApprenticeLayer = objs[11];
	m_uiTeacherTitle = objs[12];
	m_uiTeacherListBtn = objs[13];
	m_uiTeacherShopBtn = objs[14];
	
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
	m_uiTeacherListBtn.transform:SetSiblingIndex(1000);
	m_uiTeacherShopBtn.transform:SetSiblingIndex(1001);
	FriendDlgAskList()
	FriendDlgSetTeacherBtnName()
	m_SelectType = 1;
end

-- 设置师徒按钮名称
function FriendDlgSetTeacherBtnName()
	if GetPlayer().m_level < 50 then 
		SetText(m_uiTeacherBtnName,T(3301));
	else
		SetText(m_uiTeacherBtnName,T(3302));
	end
end

-- 设置选择标签页
function FriendDlgSetSelectType(index)
	if index == m_SelectType then 
		return 
	end
	
	if index == 1 then 
		SetImage( m_uiFriendBtn.transform:Find("Back"), LoadSprite("ui_button_page1") );
		SetImage( m_uiTeacherBtn.transform:Find("Back"), LoadSprite("ui_button_page2") );
		SetTrue(m_uiFriendScroll);
		SetFalse(m_uiTeacherScroll);
		SetFalse(m_uiTeacherShopBtn);
	elseif index == 2 then 
		SetImage( m_uiFriendBtn.transform:Find("Back"), LoadSprite("ui_button_page2") );
		SetImage( m_uiTeacherBtn.transform:Find("Back"), LoadSprite("ui_button_page1") );
		SetTrue(m_uiTeacherScroll);
		SetFalse(m_uiFriendScroll);
		SetTrue(m_uiTeacherShopBtn);
	end
	m_SelectType = index;
end

-- 开始接收好友信息
function FriendDlgFriendBeginRecv()
	m_recvValue = {};
	FriendDlgFriendClear()
end

-- 接收好友列表
-- m_count=0,m_list={m_actorid=0,m_city_index=0,m_shape=0,m_namelen=0,m_name="[m_namelen]",m_level=0,m_place=0,m_battlepower=0,m_ask=0,[m_count]},
function FriendDlgFriendRecv( recvValue )
	for i=1, recvValue.m_count do
		table.insert( m_recvValue, recvValue.m_list[i] )
	end
end

-- 接收好友信息完毕
function FriendDlgFriendEndRecv()
	-- 排序
	table.sort( m_recvValue, function( a, b )
				if a.m_ask > b.m_ask then
					return true
				else
					if a.m_ask == b.m_ask then
						if a.m_level > b.m_level then
							return true
						else
							if a.m_level == b.m_level then
								if a.m_battlepower > b.m_battlepower then
									return true
								else
									return false
								end
							else
								return false;
							end
						end
					else
						return false
					end
				end
				return false
		end );
	
	-- 创建
	for i=1, #m_recvValue, 1 do
		FriendDlgSetFriendObj( i, m_recvValue[i] );
	end
end

--设置好友信息
function FriendDlgSetFriendObj( index, info )
	local uiObj = m_ObjectPool:Get( "UIP_Friend" );
	uiObj.transform:SetParent( m_uiFriendContent.transform );	
	SetControlID( uiObj, 1000 + index );
	info.m_FriendObj = uiObj
	info.m_index = index
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiShape = objs[0]
	local uiPlace = objs[1]
	local uiName = objs[2]
	local uiBattlePower = objs[3]
	local uiAcceptBtn = objs[4]
	local uiRefuseBtn = objs[5]
	
	SetImage( uiShape, PlayerHeadSprite(info.m_shape) );
	SetImage( uiPlace, PlaceSprite(info.m_place) );
	SetLevel( uiName, info.m_level.." "..info.m_name );
	SetText( uiBattlePower, F(1990,info.m_battlepower) );
	
	if info.m_ask == 0 then
		SetFalse( uiAcceptBtn )
		SetFalse( uiRefuseBtn )
	else
		SetTrue( uiAcceptBtn )
		SetTrue( uiRefuseBtn )
		SetControlID( uiAcceptBtn, 2000 + index);
		SetControlID( uiRefuseBtn, 3000 + index);
	end
end

-- 清空
function FriendDlgFriendClear()
	local objs = {};
	for i = 0 ,m_uiFriendContent.transform.childCount - 1 do
		table.insert( objs, m_uiFriendContent.transform:GetChild(i).gameObject )
    end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "UIP_Friend(Clone)" then
			m_ObjectPool:Release( "UIP_Friend", obj );
		end
    end
	m_uiFriendScroll:GetComponent("UIScrollRect"):ResetScroll();
	m_uiFriendContent.transform.localPosition = Vector2.zero
end

-- 获取列表
function FriendDlgAskList()
	sendValue.m_op = 0;
	sendValue.m_target_actorid = 0;
	sendValue.m_target_cityindex = -1;
	sendValue.m_target_namelen = 0;
	sendValue.m_target_name = "";
	netsend_friendop_C( sendValue )
	FriendDlgSetSelectType(1);
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

-- 添加好友
function FriendDlgOpFriend( name, namelen )
	sendValue.m_op = 1;
	sendValue.m_target_actorid = 0;
	sendValue.m_target_cityindex = -1;
	sendValue.m_target_namelen = namelen;
	sendValue.m_target_name = name;
	netsend_friendop_C( sendValue )
end

-- 好友操作页
function FriendDlgOpLayerShow( index, uiObj )
	if m_recvValue[index].m_ask == 1 then
		return; 
	end
	m_selectRecvActor = m_recvValue[index];
	m_uiOpLayer.transform:SetParent( uiObj.transform );
	m_uiOpLayer.transform.anchoredPosition = Vector2(150,-70);
	m_uiOpLayer.transform:SetParent( m_Dlg.transform );
	SetTrue( m_uiOpLayer );
end

-- 关闭好友操作页
function FriendDlgOpLayerHide()
	SetFalse( m_uiOpLayer );
	m_selectRecvActor = nil
end

-- 选中玩家查看详细信息
function FriendDlgOpView()
	if m_selectRecvActor == nil then
		return;
	end
	if m_selectRecvActor.m_city_index == nil then
		ActorSearchDlgShow( m_selectRecvActor.m_actorid );
	else
		ActorSearchDlgShow( m_selectRecvActor.m_actorid, m_selectRecvActor.m_city_index )
	end
	FriendDlgOpLayerHide();
end

-- 发邮件
function FriendDlgOpSendMail()
	if m_selectRecvActor == nil then
		return;
	end
	MailSendDlgShow( m_selectRecvActor.m_actorid, m_selectRecvActor.m_name, GetPlayer().m_nation );
	FriendDlgOpLayerHide();
end

-- 删除好友
function FriendDlgOpDelete()
	if m_selectRecvActor == nil then
		return;
	end
	MsgBox( T(1991), function()
		m_ObjectPool:Release( "UIP_Friend", m_selectRecvActor.m_FriendObj )
		FriendDlgDelete( m_selectRecvActor.m_actorid, m_selectRecvActor.m_city_index );
		table.remove( m_recvValue, m_selectRecvActor.m_index );
		FriendDlgFriendClear()
		FriendDlgFriendEndRecv()
		FriendDlgOpLayerHide();
	end);
end

-- 同意
function FriendDlgAccept( index )
	if m_recvValue[index].m_ask == 0 then
		return; 
	end
	m_recvValue[index].m_ask = 0;
	FriendDlgAgree( m_recvValue[index].m_actorid, m_recvValue[index].m_city_index );
	FriendDlgFriendClear()
	FriendDlgFriendEndRecv()
end

-- 拒绝
function FriendDlgRefuse( index )
	if m_recvValue[index].m_ask == 0 then
		return; 
	end
	FriendDlgReject( m_recvValue[index].m_actorid, m_recvValue[index].m_city_index );
	table.remove( m_recvValue, index );
	FriendDlgFriendClear()
	FriendDlgFriendEndRecv()
end

-- 显示添加好友界面
function FriendDlgAddFriendLayerShow()
	SetTrue( m_uiAddFriendLayer );
	FriendDlgOpLayerHide();
end

-- 关闭添加好友界面
function FriendDlgAddFriendLayerHide()
	SetFalse( m_uiAddFriendLayer );
	m_uiNameInput.transform:GetComponent("UIInputField" ).text = ""
end

--确定添加好友
function FriendDlgAddFriend()
	local playerName = m_uiNameInput.transform:GetComponent("UIInputField" ).text;
	-- 非法检查
	local namelen = string.len( playerName );
	if namelen < 4 or namelen > 18 then
		pop(T(790))
		return
	end
	if string.checksign( playerName ) == false then
		pop(T(788))
		return
	end
	if Utils.MaskWordCheck( playerName ) == false then
		pop(T(789))
		return;
	end
	FriendDlgOpFriend( playerName, namelen );
	FriendDlgAddFriendLayerHide()
end

-- 师徒界面
function FriendDlgAskTeacherList()
	FriendDlgSetSelectType(2);
	m_TeacherType = 1;
	FriendDlgSetTeacherView();
end

-- 设置师徒界面
function FriendDlgSetTeacherView()
	if m_TeacherType == 1 or m_TeacherType == 2 then 		--未拜师
		SetTrue(m_uiApprenticeLayer);
		SetFalse(m_uiTeacherTitle);
		SetFalse(m_uiTeacherListBtn);
		FriendDlgApprenticeView(m_TeacherType);
	elseif m_TeacherType == 3 then 							--拜师
		SetFalse(m_uiApprenticeLayer);
		SetTrue(m_uiTeacherTitle);
		SetTrue(m_uiTeacherListBtn);		
	end
end

-- 拜师界面
function FriendDlgApprenticeView(Type)
	local objs = m_uiApprenticeLayer.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local Title = objs[0];
	local Tip = objs[1];
	local SelectBtn= objs[2];
	local GetBtn = objs[3];	
	local AwardList = objs[4];
	
	if Type == 1 then
		SetText(Title,T(3305));
		SetText(Tip,T(3307));
		SetTrue(SelectBtn);
		SetFalse(GetBtn);
	elseif Type == 2 then 
		SetText(Title,T(3306));
		SetText(Tip,T(3308));
		SetFalse(SelectBtn);
		SetTrue(GetBtn);
	end
	
-- 奖励
	for i = 1 ,4 ,1 do
		FriendDlgApprenticeAward(i,AwardList);
	end
	
end
function FriendDlgApprenticeAward(index,Content)
	local uiObj = Content.transform:GetChild( index - 1 );
	local obj = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiColor = obj[0];
	local uiIcon = obj[1];
	local uiName= obj[2];
	local uiNum = obj[3];	
end