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
local m_uiTeacherShop = nil;
local m_uiTeacherList = nil;
local m_uiTeacherTaskLayer = nil;
local m_uiUIP_ShopGrid = nil;
local m_uiUIP_TaskGrid = nil;
local m_uiTaskContent = nil;
local m_uiSelectTeacherLayer = nil;
local m_uiUIP_SelectGrid = nil;
local m_uiSelectListContent = nil;
local m_uiUIP_TeacherGrid = nil;
local m_ShopRecvValue = nil;
	
local m_ObjectPool = nil
local m_recvValue = nil
local m_selectRecvActor = nil
local m_SelectType = nil;
local m_TeacherType = nil;
local m_TeacherRecvValue = nil
local m_TeacherSelectData = nil;


-- m_op=0,m_target_actorid=0,m_target_cityindex=0,m_target_namelen=0,m_target_name="[m_target_namelen]",
local sendValue = {};
sendValue.m_op = 0;  
sendValue.m_target_actorid = 0;
sendValue.m_target_cityindex = 0;
sendValue.m_target_namelen = 0;
sendValue.m_target_name = "";
	
-- 打开界面
function FriendDlgOpen()
	ResourceManager.LoadAssetBundle( "_ab_ui_static_npc3" )
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
	Invoke( function() 
		ResourceManager.UnloadAssetBundleImmediately( "_ab_ui_static_npc3" )
	end, 0.3 );
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
		-- 关闭选择拜师页
		elseif nControlID == -4 then 
			FriendDlgSelectTeacherLayerShow(false);
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
			FriendDlgTeacherShopShow(true);
		-- 选择好友拜师		
		elseif nControlID == 51 then
			FriendDlgSelectTeacherLayerShow(true);
		-- 领取拜师奖励		
		elseif nControlID == 52 then	
			FriendDlgGetApprenticeAward();
		-- 师徒列表
		elseif nControlID == 53 then
			FriendDlgTeacherListShow();
		-- 师徒商店关闭按钮
		elseif nControlID == 54 then
			FriendDlgTeacherShopShow(false);			
		-- 师徒奖励获取按钮
		elseif nControlID > 100 and nControlID < 200 then
			FriendDlgOnBtnTeacherTask( nControlID - 100 );
		elseif nControlID == 200 then
			pop(T(3318));
		-- 拜师操作按钮
		elseif nControlID > 200 and nControlID < 300 then
			FriendDlgOnBtnTeacherSelect(nControlID - 200);
		-- 师徒商店购买按钮
		elseif nControlID > 300 and nControlID < 400 then
			FriendDlgOnBtnTeacherShop(nControlID - 300);
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
	m_uiTeacherShop = objs[15];
	m_uiTeacherList = objs[16];
	m_uiTeacherTaskLayer = objs[17];
	m_uiUIP_ShopGrid = objs[18];
	m_uiUIP_TaskGrid = objs[19];
	m_uiTaskContent = objs[20];
	m_uiSelectTeacherLayer = objs[21];
	m_uiUIP_SelectGrid = objs[22];
	m_uiSelectListContent = objs[23];
	m_uiUIP_TeacherGrid = objs[24];
	
	-- 对象池
	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );
	m_ObjectPool:CreatePool("UIP_Friend", 6, 6, m_uiUIP_Friend);	
	m_ObjectPool:CreatePool("UIP_ShopGrid", 9, 9, m_uiUIP_ShopGrid);	
	m_ObjectPool:CreatePool("UIP_TaskGrid", 8, 8, m_uiUIP_TaskGrid);	
	m_ObjectPool:CreatePool("UIP_SelectGrid", 6, 6, m_uiUIP_SelectGrid);	
	m_ObjectPool:CreatePool("UIP_TeacherGrid", 6, 6, m_uiUIP_TeacherGrid);


	
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
	m_uiTeacherShop.transform:SetSiblingIndex(1002);
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
		SetFalse(m_uiTeacherList);
		SetFalse(m_uiTeacherShopBtn);
	elseif index == 2 then 
		SetImage( m_uiFriendBtn.transform:Find("Back"), LoadSprite("ui_button_page2") );
		SetImage( m_uiTeacherBtn.transform:Find("Back"), LoadSprite("ui_button_page1") );
		SetTrue(m_uiTeacherScroll);
		SetFalse(m_uiFriendScroll);
		SetFalse(m_uiTeacherList);
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
	system_askinfo( ASKINFO_TEACHER, "", 0 );
end

--system_askinfo( ASKINFO_TEACHER, "", 0 ); —获取师徒列表
--system_askinfo( ASKINFO_TEACHER, "", 1,teacher_actorid ); —拜师
--system_askinfo( ASKINFO_TEACHER, "", 2 ); —领取拜师奖励
--system_askinfo( ASKINFO_TEACHER, "", 3, id ); —徒弟等级奖励领取
--system_askinfo( ASKINFO_TEACHER, "", 4 ); —师徒商店列表 
--system_askinfo( ASKINFO_TEACHER, "", 5, id ); —师徒商店购买
--师徒界面消息
function FriendDlgTeacherRecv(recvValue)
	m_TeacherRecvValue = recvValue;
	PrintTable(m_TeacherRecvValue,"m_TeacherRecvValue")
	FriendDlgSetTeacherView();
end

-- 设置师徒界面
function FriendDlgSetTeacherView()
	if GetPlayer().m_level >= 50 then 
		m_TeacherType = 4;
	else
		if m_TeacherRecvValue.m_count > 0 then
			if m_TeacherRecvValue.m_teacheraward_count > 0 then 
				m_TeacherType = 2;
			else
				m_TeacherType = 3;
			end
		else
			m_TeacherType = 1;
		end
	end
	if m_TeacherType == 1 or m_TeacherType == 2 then 			--未拜师或拜师未领取奖励
		SetTrue(m_uiApprenticeLayer);
		SetFalse(m_uiTeacherTaskLayer);
		SetFalse(m_uiTeacherTitle);
		SetFalse(m_uiTeacherListBtn);
		FriendDlgApprenticeView(m_TeacherType);
	elseif m_TeacherType == 3 or m_TeacherType == 4 then 		--已拜师或者可收徒
		SetFalse(m_uiApprenticeLayer);
		SetTrue(m_uiTeacherTaskLayer);
		SetTrue(m_uiTeacherTitle);
		SetTrue(m_uiTeacherListBtn);		
		FriendDlgTeacherTaskView();
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
	local AwardData = m_TeacherRecvValue.m_teacheraward;
	for i = 0 ,4 ,1 do
		local key = "awardkind"..i;
		if AwardData[key] ~= 0 and AwardData[key] ~= nil then
			FriendDlgApprenticeAward(i,AwardList,AwardData);
		end	
	end
	
end

--拜师奖励子控件绘制
function FriendDlgApprenticeAward(index,Content,AwardData)
	local uiObj = Content.transform:GetChild( index );
	SetTrue(uiObj);
	local obj = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiColor = obj[0];
	local uiIcon = obj[1];
	local uiName= obj[2];
	local uiNum = obj[3];	
	
	local KeyKind = "awardkind"..index;	
	local KeyNum = "awardnum"..index;
	local sprite, color, name, c, desc = AwardInfo( AwardData[KeyKind] );			
	SetImage( uiIcon, sprite );
	SetImage( uiColor, color );
	SetText( uiName, name );		
	SetText(uiNum,"x"..AwardData[KeyNum]);
end

--选择拜师界面
function FriendDlgSelectTeacherLayerShow(bShow)
	if bShow == false then SetFalse(m_uiSelectTeacherLayer)  return end	
	SetTrue(m_uiSelectTeacherLayer)
	FriendDlgClearSelectTeacherGrid();
	m_TeacherSelectData = FriendDlgGetCanSelectTeacherData();
	for i = 1 , table.nums(m_TeacherSelectData) do 
		FriendDlgSelectTeacherGridView(i);
	end
	
end

--获取可拜师好友数据
function FriendDlgGetCanSelectTeacherData()
	local Data = {};	
	for k,v in pairs(m_recvValue) do 
		if v.m_level >= 50 then 
			table.insert(Data,v);
		end
	end
	return Data;
end

--清空选择好友拜师子控件
function FriendDlgClearSelectTeacherGrid()
	local objs = {};
	for i=0,m_uiSelectListContent.transform.childCount-1 do
		table.insert(objs,m_uiSelectListContent.transform:GetChild(i).gameObject);
	end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "UIP_SelectGrid(Clone)" then
			m_ObjectPool:Release( "UIP_SelectGrid", obj );
		end
	end
end

--选择好友拜师子控件绘制
function FriendDlgSelectTeacherGridView(index)
	local uiObj = m_ObjectPool:Get( "UIP_SelectGrid" );
	uiObj.transform:SetParent( m_uiSelectListContent.transform );	
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiShape = objs[0];	
	local uiBtnSelect = objs[1];
	local uiName = objs[2];
	local uiPower = objs[3];

	SetText(uiName,m_TeacherSelectData[index].m_name)	
	SetText(uiPower,m_TeacherSelectData[index].m_battlepower)
	SetImage( uiShape, PlayerHeadSprite(m_TeacherSelectData[index].m_shape) );
	SetControlID(uiBtnSelect,200 + index)
end

--发送拜师请求
function FriendDlgOnBtnTeacherSelect(index)
	local Data = m_TeacherSelectData[index];
	system_askinfo( ASKINFO_TEACHER, "", 1,Data.m_actorid ); 
	SetFalse(m_uiSelectTeacherLayer)
end

--领取拜师奖励请求
function FriendDlgGetApprenticeAward()
	system_askinfo( ASKINFO_TEACHER, "", 2 ); 
end

--师徒商店
function FriendDlgTeacherShopShow(bOpen)
	if bOpen == true then 		
		SetTrue(m_uiTeacherShop);
		system_askinfo( ASKINFO_TEACHER, "", 4 );
	else
		SetFalse(m_uiTeacherShop);
	end
end

--接收师徒商店数据
function FriendDlgTeacherShopRecv(recvValue)
	m_ShopRecvValue = recvValue;	
	FriendDlgTeacherShopView();
end
--师徒商店绘制
function FriendDlgTeacherShopView()
	local objs = m_uiTeacherShop.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiPoint = objs[0];
	local uiContent = objs[1];
	
	SetText(uiPoint,m_ShopRecvValue.m_mypoint);
	FriendDlgClearTeacherShopGrid(uiContent)
	
	for i = 1 ,m_ShopRecvValue.m_count ,1 do 
		FriendDlgTeacherShopGridView(i,uiContent,m_ShopRecvValue.m_list[i])
	end	
end


--师徒商店子控件绘制
function FriendDlgTeacherShopGridView(index,Parent,Data)
	local uiObj = m_ObjectPool:Get( "UIP_ShopGrid" );
	uiObj.transform:SetParent( Parent.transform );	
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiColor = objs[0];
	local uiIcon = objs[1];
	local uiName = objs[2];
	local uiBtnBuy = objs[3];
	local uiPoint = objs[4];
	local uiLockTip = objs[5];
	
	local sprite, color, name, c, desc = AwardInfo( Data.m_awardkind );	
	SetImage( uiIcon, sprite );
	SetImage( uiColor, color );
	SetText( uiName, name, NameColor(c) );	

	if GetPlayer().m_level < Data.m_buylevel then
		SetTrue(uiLockTip);
		SetText(uiLockTip,F(3309,Data.m_buylevel))
		SetFalse(uiBtnBuy);
	else
		SetFalse(uiLockTip);
		SetText(uiPoint,F(3310,knum(Data.m_point)))
		if Data.m_point > m_ShopRecvValue.m_mypoint then 
			
			SetTextColor(uiPoint,Hex2Color(0xE80017FF));
		else
			SetTextColor(uiPoint,Hex2Color(0x03DE27FF));
		end
		SetTrue(uiBtnBuy);
		SetControlID(uiBtnBuy,index + 300)
	end
end

-- 清空师徒商店数据
function FriendDlgClearTeacherShopGrid(Content)
	local objs = {};
	for i=0,Content.transform.childCount-1 do
		table.insert(objs,Content.transform:GetChild(i).gameObject);
	end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "UIP_ShopGrid(Clone)" then
			m_ObjectPool:Release( "UIP_ShopGrid", obj );
		end
	end
end

--师徒商店购买按钮
function FriendDlgOnBtnTeacherShop(index)
	if m_ShopRecvValue.m_mypoint < m_ShopRecvValue.m_list[index].m_point then
		pop(T(3319))
	else
		system_askinfo( ASKINFO_TEACHER, "", 5, m_ShopRecvValue.m_list[index].m_id );
	end	
end

--师徒任务奖励列表
function FriendDlgTeacherTaskView()
	FriendDlgClearTeacherTaskGrid();
	local Data = g_teacher_award;
	for i = 1 , table.nums(Data)-1 do 
		FriendDlgTeacherTaskGridView(i,Data)
	end
end

----师徒任务奖励子控件绘制
function FriendDlgTeacherTaskGridView(index,Data)
	local uiObj = m_ObjectPool:Get( "UIP_TaskGrid" );
	uiObj.transform:SetParent( m_uiTaskContent.transform );	
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiTitle = objs[0];
	local uiContent = objs[1];
	local uiCount = objs[2];
	local uiGetBtn = objs[3];
	local uiBtnText = objs[4];
	
	SetText(uiTitle,F(3311,Data[index].student_level))
	SetText(uiCount,F(3312,m_TeacherRecvValue.m_te_awarded[index+1],Data[index].maxnum))
	
	if m_TeacherRecvValue.m_te_award[index+1] == 1 then 
		SetText(uiBtnText,T(3314))
		SetControlID(uiGetBtn,100+ index);
		SetImage(uiGetBtn.transform:Find("Back"),LoadSprite("ui_button_3"))
	else
		SetText(uiBtnText,T(3313))
		SetControlID(uiGetBtn,200);
		SetImage(uiGetBtn.transform:Find("Back"),LoadSprite("ui_button_13"))
	end
--奖励绘制
	for i = 0 ,uiContent.transform.childCount-1 do 
		local uiObj = uiContent.transform:GetChild( i );
		local key = "awardkind"..i;
		if Data[index][key] ~= 0 then 
			SetTrue(uiObj);
			local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
			local uiColor = objs[0];
			local uiIcon = objs[1];
			local uiNum = objs[2];
			local sprite, color, name, c, desc = AwardInfo( Data[index][key] );	
			SetImage( uiIcon, sprite );
			SetImage( uiColor, color );
			local keyNum = "awardnum"..i;
			SetText( uiNum, Data[index][keyNum] );		
		else
			SetFalse(uiObj);
		end
	end
end

-- 清空师徒任务数据
function FriendDlgClearTeacherTaskGrid()
	local objs = {};
	for i=0,m_uiTaskContent.transform.childCount-1 do
		table.insert(objs,m_uiTaskContent.transform:GetChild(i).gameObject);
	end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "UIP_TaskGrid(Clone)" then
			m_ObjectPool:Release( "UIP_TaskGrid", obj );
		end
	end
end

--师徒任务完成领取
function FriendDlgOnBtnTeacherTask( index )
	system_askinfo( ASKINFO_TEACHER, "", 3, index );
end

--师徒列表
function FriendDlgTeacherListShow()	
	if m_SelectType == 3 then return end;	
	SetTrue(m_uiTeacherList);
	SetFalse(m_uiTeacherScroll);
	m_SelectType = 3 ;	
	FriendDlgTeacherListView();
end

--师徒列表绘制
function FriendDlgTeacherListView()
	local objs = m_uiTeacherList.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiContent = objs[0];
	local uiTitle = objs[1];
	
	FriendDlgClearTeacherListGrid(uiContent);
	
	SetText(uiTitle,F(3315,FriendDlgGetApprenticeNum()));
	
	for i = 1, table.nums(m_TeacherRecvValue.m_list) do 
		FriendDlgTeacherListGridView(i, uiContent, m_TeacherRecvValue.m_list[i]);
	end
end

-- 清空师徒列表子控件数据
function FriendDlgClearTeacherListGrid(Content)
	local objs = {};
	for i=0,Content.transform.childCount-1 do
		table.insert(objs,Content.transform:GetChild(i).gameObject);
	end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "UIP_TeacherGrid(Clone)" then
			m_ObjectPool:Release( "UIP_TeacherGrid", obj );
		end
	end
end

--师徒列表子控件绘制
function FriendDlgTeacherListGridView(index,Content,Data)
	local uiObj = m_ObjectPool:Get( "UIP_TeacherGrid" );
	uiObj.transform:SetParent( Content.transform );	
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiShape = objs[0]	
	local uiPlace = objs[1]
	local uiName = objs[2]
	local uiBattlePower = objs[3]	
	local uiLabel = objs[4]
	
	SetImage( uiShape, PlayerHeadSprite(Data.m_shape) );
	SetImage( uiPlace, PlaceSprite(Data.m_place) );
	SetLevel( uiName, Data.m_level.." "..Data.m_name );
	SetText( uiBattlePower, F(1990,Data.m_battlepower) );
	
	if Data.m_isteacher == 1 then 
		SetText(uiLabel,T(3316))
		SetTextColor(uiLabel,Hex2Color(0xD95DF4FF))
	else
		SetText(uiLabel,T(3317))
		SetTextColor(uiLabel,Hex2Color(0x03DE27FF))
	end
end

--获取徒弟数量
function FriendDlgGetApprenticeNum()
	local num = 0;
	for k,v in pairs(m_TeacherRecvValue.m_list) do 
		if v.m_isteacher == 0 then 
			num = num + 1;
		end
	end
	return num;
end