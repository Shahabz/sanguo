-- 界面
local m_Dlg = nil;
local m_DialogFrameMod = nil;

local m_uiGetLayer = nil; --UnityEngine.GameObject
local m_uiListContent = nil; --UnityEngine.GameObject
local m_uiGetBtn = nil; --UnityEngine.GameObject
local m_uiPlaceLayer = nil; --UnityEngine.GameObject
local m_uiGarthBtn = nil; --UnityEngine.GameObject
local m_uiIncognito1Btn = nil; --UnityEngine.GameObject
local m_uiIncognito2Btn = nil; --UnityEngine.GameObject
local m_uiGarthLayer = nil; --UnityEngine.GameObject
local m_uiCloseBtn = nil; --UnityEngine.GameObject
local m_uiSeeBtn = nil; --UnityEngine.GameObject
local m_uiGarthCount = nil; --UnityEngine.GameObject
local m_uiTopSroll = nil; --UnityEngine.GameObject
local m_uiBottomSroll = nil; --UnityEngine.GameObject
local m_uiSeeLayer = nil; --UnityEngine.GameObject
local m_uiAgainBtn = nil; --UnityEngine.GameObject
local m_uiSeeBtn = nil; --UnityEngine.GameObject
local m_uiGirlBtn = nil; --UnityEngine.GameObject
local m_uiShopBtn = nil; --UnityEngine.GameObject
local m_uiIncognitoCount = nil; --UnityEngine.GameObject
local m_uiPlaceInfo = nil; --UnityEngine.GameObject
local m_uiUIP_GetGrid = nil; --UnityEngine.GameObject
local m_uiUIP_GirlGrid = nil; --UnityEngine.GameObject
local m_uiUIP_InfoGrid = nil; --UnityEngine.GameObject
local m_uiInfoTitle = nil; --UnityEngine.GameObject
local m_uiInfoContent = nil; --UnityEngine.GameObject
local m_uiGetEffect = nil; --UnityEngine.GameObject
local m_uiAwardDescLayer = nil; --UnityEngine.GameObject

local m_ObjectPool = nil;
local m_recvInfo = nil;
local m_recvVisit = nil;
local m_recvGarth = nil;
local m_recvSee = nil;
local m_placeSelect = nil;
local m_grathSelect = nil;
local m_GetIndex = nil;
local m_IsPlayGet = false;
local m_IsShowGetGirl = false;
local m_cacheNodeAward = {};

-- 打开界面
function FangshiDlgOpen()
	m_Dlg = eye.uiManager:Open( "FangshiDlg" );
	m_DialogFrameMod = DialogFrameModOpen( m_Dlg, BuildingName(33), HELP_FangshiDlg, FangshiDlgClose );
end

-- 隐藏界面
function FangshiDlgClose()
	if m_Dlg == nil then
		return;
	end
	DialogFrameModClose( m_DialogFrameMod );
	m_DialogFrameMod = nil;
	m_recvInfo = nil;
	m_recvVisit = nil;
	m_recvGarth = nil;
	m_recvSee = nil;
	m_placeSelect = nil;
	m_grathSelect = nil;
	m_cacheNodeAward = {};
	m_IsPlayGet = nil;
	eye.uiManager:Close( "FangshiDlg" );
end

-- 删除界面
function FangshiDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
	m_recvInfo = nil;
	m_recvVisit = nil;
	m_recvGarth = nil;
	m_recvSee = nil;
	m_placeSelect = nil;
	m_grathSelect = nil;
	m_cacheNodeAward = {};
	m_IsPlayGet = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function FangshiDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
		if m_IsPlayGet == true then 
			return 
		end;
		FangshiDlgClosePlaceInfo();	
		FangshiDlgCloseAwardDesc();
        if nControlID == -1 then
            FangshiDlgClose();
		elseif nControlID == -2 then
           SetFalse( m_uiAwardDescLayer )
			SetFalse( m_uiPlaceInfo )
		elseif nControlID == 1 then 								-- 打开皇宫内院界面
			FangshiDlgOnBtnGarth();	
		elseif nControlID == 2 then									-- 普通巡游
			FangshiDlgOnBtnIncognito(1);		
		elseif nControlID == 3 then									-- 多次巡游
			FangshiDlgOnBtnIncognito(2);				
		elseif nControlID == 4 then									-- 领取巡游奖励
			FangshiDlgOnBtnGet();	
		elseif nControlID == 5 then									-- 关闭皇宫内院界面
			FangshiDlgOnBtnCloseGarth();
		elseif nControlID == 6 then									-- 觐见
			FangshiDlgOnBtnSee();
		elseif nControlID == 7 then									-- 再次觐见
			FangshiDlgOnBtnSee();	
		elseif nControlID == 8 then									-- 关闭觐见获得界面	
			FangshiDlgOnBtnCloseSee();	
		elseif nControlID == 9 then									-- 关闭巡游位置奖励界面	
			FangshiDlgClosePlaceInfo();			
		elseif nControlID >= 20 and nControlID < 40 then			-- 巡游奖励tips
			FangshiDlgShowAwardDesc(nControlID - 20);
		elseif nControlID == 40 then								-- 巡游奖励tips关闭
			FangshiDlgCloseAwardDesc();
		elseif nControlID >= 50 and nControlID <= 60 then			-- 皇宫内院女将选择
			m_grathSelect = nControlID - 50;
			FangshiDlgSetGarthSelect();								
		elseif nControlID == 100 then								-- 打开女将界面
			GirlDlgShow();
		elseif nControlID == 101 then								-- 打开女将商店
			GirlShopDlgShow();
		elseif nControlID >= 200 and nControlID <= 300 then			-- 查看巡游地点信息
			FangshiDlgOnBtnPlace( nControlID - 200 )
        end
	elseif nType == UI_EVENT_TWEENFINISH then
		FangshiDlgPlayGetOff();	
	end
end

-- 载入时调用
function FangshiDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiGetLayer = objs[0];
	m_uiListContent = objs[1];
	m_uiGetBtn = objs[2];
	m_uiPlaceLayer = objs[3];
	m_uiGarthBtn = objs[4];
	m_uiIncognito1Btn = objs[5];
	m_uiIncognito2Btn = objs[6];
	m_uiGarthLayer = objs[7];
	m_uiCloseBtn = objs[8];
	m_uiSeeBtn = objs[9];
	m_uiGarthCount = objs[10];
	m_uiTopSroll = objs[11];
	m_uiBottomSroll = objs[12];
	m_uiSeeLayer = objs[13];
	m_uiAgainBtn = objs[14];
	m_uiSeeBtn = objs[15];
	m_uiGetEffect = objs[16];
	m_uiGirlBtn = objs[17];
	m_uiShopBtn = objs[18];
	m_uiIncognitoCount = objs[19];
	m_uiPlaceInfo = objs[20];
	m_uiUIP_GetGrid = objs[21];
	m_uiUIP_GirlGrid = objs[22];
	m_uiUIP_InfoGrid = objs[23];
	m_uiInfoTitle = objs[24];
	m_uiInfoContent = objs[25];
	m_uiAwardDescLayer = objs[26]
	
	-- 对象池
	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );	
	--巡游奖励Grid
	m_ObjectPool:CreatePool("UIP_GetGrid", 14, 14, m_uiUIP_GetGrid);
	--皇宫内院Grid
	m_ObjectPool:CreatePool("UIP_GirlGrid", 6, 6, m_uiUIP_GirlGrid);
	--巡游位置Grid
	m_ObjectPool:CreatePool("UIP_InfoGrid", 6, 6, m_uiUIP_InfoGrid);
	
end

-- 界面初始化时调用
function FangshiDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function FangshiDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function FangshiDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function FangshiDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function FangshiDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
--system_askinfo( ASKINFO_FANGSHI, "", 0 ); 打开界面调用
--m_freenum=0,m_nodeid=0,m_awardcount=0,m_awardlist={m_kind=0,m_num=0,[m_awardcount]},

function FangshiDlgShow()
	FangshiDlgOpen()
	m_uiGirlBtn.transform:SetSiblingIndex(1000); 
	m_uiShopBtn.transform:SetSiblingIndex(1001);	
	m_uiSeeLayer.transform:SetSiblingIndex(1002);
	system_askinfo( ASKINFO_FANGSHI, "", 0 );
end

-- 接收数据
function FangshiDlgInfoRecv( recvValue )		
	m_recvInfo = recvValue;	
	FangshiDlgSetPlaceInfo();
	FangshiDlgSetIncognitoButtons();
	--绘制未领取的巡游奖励
	if m_recvInfo.m_awardcount ~= 0 then 		
		FangshiDlgInitGetLayer();
	else
		SetFalse(m_uiGetLayer);
	end
end

-- 获得寻游点亮状态
function FangshiDlgGetPlaceOverState(index)
	local NodeIndex = FangshiDlgChildtoNode(index);
	if m_recvInfo.m_nodeid == NodeIndex or index == 0 then
		return true,false;
	end
	local preid = g_fangshi_node[m_recvInfo.m_nodeid].preid;
	if preid == NodeIndex then 
		if m_recvInfo.m_nodeid == 13 or m_recvInfo.m_nodeid == 23 or m_recvInfo.m_nodeid == 33 then
			return true,true;
		else
			return true,false;
		end
	end
	return false,false;
end

-- 设置寻游
function FangshiDlgSetPlaceInfo()
	for i = 1, 7 do 
		local bOver,bReturn = FangshiDlgGetPlaceOverState(i-1)
		FangshiDlgSetPlaceState(i-1,bOver,bReturn);
	end
end

-- 设置寻游位置状态
function FangshiDlgSetPlaceState(index,bOver,bReturn)
	local uiObj = m_uiPlaceLayer.transform:GetChild( index );
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiFoot = objs[0];
	local uiColor = objs[1];
	local uiRFoot = objs[2];
	local uiBack = objs[4];
	local uiName = objs[5];
	SetText( uiName, T(4140+index) )
	if bOver == false then 
		SetTrue(uiFoot);
		SetImage(uiFoot,LoadSprite("ui_fangshi_footoff"))
		SetFalse(uiRFoot);
		SetImage(uiColor,LoadSprite("ui_fangshi_quality0"))			
		SetGray(uiBack,true);
	else
		if bReturn == true then 
			SetFalse(uiFoot);
			SetTrue(uiRFoot);
			SetImage(uiRFoot,LoadSprite("ui_fangshi_footon"))
		else
			SetTrue(uiFoot);
			SetFalse(uiRFoot);
			SetImage(uiFoot,LoadSprite("ui_fangshi_footon"))
		end		
		FangshiDlgSetPlaceColor(uiColor,index);
		SetGray(uiBack,false);
	end
end

-- 设置寻游位置品质颜色
function FangshiDlgSetPlaceColor(uiColor,index)
	if index == 0 then 
		SetImage(uiColor,LoadSprite("ui_fangshi_quality1"))
	elseif index == 1 or index == 3 or index == 5 then 
		SetImage(uiColor,LoadSprite("ui_fangshi_quality2"))
	elseif index == 2 or index == 4 or index == 6 then 
		SetImage(uiColor,LoadSprite("ui_fangshi_quality3"))
	end
	
end

-- 设置寻游按钮状态
function FangshiDlgSetIncognitoButtons()
	SetTrue(m_uiIncognito1Btn);
	if m_recvInfo.m_freenum < global.fangshi_visit_freenum then	
		SetTrue(m_uiIncognitoCount)		
		SetText(m_uiIncognitoCount,F(4001,global.fangshi_visit_freenum - m_recvInfo.m_freenum,global.fangshi_visit_freenum));	
		--按钮1	
		SetFalse(m_uiIncognito1Btn.transform:Find("Yb"));
		--按钮2
		SetFalse(m_uiIncognito2Btn);			
	else
		SetFalse(m_uiIncognitoCount);
		--按钮1				
		SetTrue(m_uiIncognito1Btn.transform:Find("Yb"));
		SetText(m_uiIncognito1Btn.transform:Find("Yb/Num"),global.fangshi_visit_token);
		--按钮2
		SetTrue(m_uiIncognito2Btn);
		local count = 14 - m_recvInfo.m_awardcount;
		SetText(m_uiIncognito2Btn.transform:Find("Yb/Num"),count * global.fangshi_visit_token);
		SetText(m_uiIncognito2Btn.transform:Find("Back/Text"),F(4003,count));
	end
end

-- 巡游位置信息
function FangshiDlgOnBtnPlace(index)
	if m_placeSelect == index then 		
		SetFalse(m_uiPlaceInfo);
		FangshiDlgSetPlaceSelect(-1);
		m_placeSelect = nil;
	else
		m_placeSelect = index;
		if m_cacheNodeAward[m_placeSelect] == nil then 			
			system_askinfo( ASKINFO_FANGSHI, "", 3, index );	
		else
			FangshiDlgPlaceInfoShow();
		end
	end
end

-- 巡游位置信息框数据
function FangshiDlgPlaceInfoRecv(recvValue)
	m_cacheNodeAward[m_placeSelect] = recvValue;
	FangshiDlgPlaceInfoShow()		
end

-- 位置信息框显示
function FangshiDlgPlaceInfoShow()	
	--设置标题	
	local TitleID = 4100 + m_placeSelect;
	SetText(m_uiInfoTitle,T(TitleID))
	
	FangshiDlgClearPlaceInfoGrid()
	
	for i = 1 , m_cacheNodeAward[m_placeSelect].m_count do 
		local data = m_cacheNodeAward[m_placeSelect].m_list[i]
		FangshiDlgCreatPlaceInfoGrid(data);
	end	
	
	--设置出现位置
	local ChildIndex = FangshiDlgNodetoChild(m_placeSelect)
	local uiObj = m_uiPlaceLayer.transform:GetChild( ChildIndex );
	if ChildIndex == 4 or ChildIndex == 6 then
		m_uiPlaceInfo.transform.localPosition = Vector3.New( uiObj.localPosition.x+150, uiObj.localPosition.y+180, 0);
	elseif ChildIndex == 1 then
		m_uiPlaceInfo.transform.localPosition = Vector3.New( uiObj.localPosition.x-110, uiObj.localPosition.y+180, 0);
	elseif ChildIndex == 2 then
		m_uiPlaceInfo.transform.localPosition = Vector3.New( uiObj.localPosition.x-150, uiObj.localPosition.y+180, 0);
	else
		m_uiPlaceInfo.transform.localPosition = Vector3.New( uiObj.localPosition.x, uiObj.localPosition.y+180, 0);
	end
	FangshiDlgSetPlaceSelect(ChildIndex);
	
	SetTrue(m_uiPlaceInfo);	
end

-- 创建位置信息子控件
function FangshiDlgCreatPlaceInfoGrid(data)
	local uiObj = m_ObjectPool:Get( "UIP_InfoGrid" );
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiShape= objs[0];	
	local uiColor = objs[1];
	local uiName = objs[2];
	local sprite, color, name, c, desc = AwardInfo( data.m_kind );	
	SetImage(uiShape,sprite)
	SetImage(uiColor,color)
	SetText(uiName,name);
	SetTextColor(uiName,NameColor(c))
	uiObj.transform:SetParent( m_uiInfoContent.transform );
	uiObj.gameObject:SetActive( true );
end

-- 清空位置信息子控件
function FangshiDlgClearPlaceInfoGrid()
	local objs = {};
	for i=0,m_uiInfoContent.transform.childCount-1 do
		table.insert(objs,m_uiInfoContent.transform:GetChild(i).gameObject);
	end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "UIP_InfoGrid(Clone)" then
			m_ObjectPool:Release( "UIP_InfoGrid", obj );
		end
	end
end

-- Node 转 巡游子控件ID
function FangshiDlgNodetoChild(index)
	local ChildIndex = 0;
	if index == 11 or index == 12 then 
		ChildIndex = index - 10;
	elseif index == 21 or index == 22 then 
		ChildIndex = index - 20 + 2;
	elseif index == 31 or index == 32 then 	
		ChildIndex = index - 30 + 4;
	elseif index == 13 then 
		ChildIndex = 1;
	elseif index == 23 then 
		ChildIndex = 3;
	elseif index == 33 then 
		ChildIndex = 5;
	end
	return ChildIndex;
end

-- 巡游子控件ID 转 Node
function FangshiDlgChildtoNode(index)
	local NodeIndex = 0;
	if index == 1 or index == 2 then 
		NodeIndex = index + 10;
	elseif index == 3 or index == 4 then 
		NodeIndex = index + 20 - 2;
	elseif index == 5 or index == 6 then 	
		NodeIndex = index + 30 - 4;
	end
	return NodeIndex;
end

-- 设置巡游位置选中状态
function FangshiDlgSetPlaceSelect(index)
	for i = 0, 6 do 
		local uiObj = m_uiPlaceLayer.transform:GetChild( i );
		local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
		local uiSelect = objs[3];
		if i == index then 
			SetTrue(uiSelect);
		else
			SetFalse(uiSelect);
		end
	end
end

-- 关闭巡游位置信息
function FangshiDlgClosePlaceInfo()
	SetFalse(m_uiPlaceInfo);
	m_placeSelect = nil;
	FangshiDlgSetPlaceSelect(-1);
end

-- system_askinfo( ASKINFO_FANGSHI, "", 1,0 ); 免费寻访
-- system_askinfo( ASKINFO_FANGSHI, "", 1,1 ); 元宝寻访
-- system_askinfo( ASKINFO_FANGSHI, "", 1,2 ); 元宝多次寻访
-- 点击巡游
function FangshiDlgOnBtnIncognito(Type)
	if m_recvInfo.m_awardcount < 14 then 
		if Type == 1 then 				--普通巡游
			if m_recvInfo.m_freenum < global.fangshi_visit_freenum  then 
				system_askinfo( ASKINFO_FANGSHI, "", 1,0 ); 
			else
				if global.fangshi_visit_token > GetPlayer().m_token then 
					JumpToken();
					return;
				end
				system_askinfo( ASKINFO_FANGSHI, "", 1,1 );
			end
		elseif Type == 2 then 			--多次巡游
			local count = 14 - m_recvInfo.m_awardcount;
			if count * global.fangshi_visit_token > GetPlayer().m_token then 
				JumpToken();
				return;
			end
			system_askinfo( ASKINFO_FANGSHI, "", 1,2 ); 
		end
	else
		pop(T(4004));
	end
end

-- 获得巡游奖励信息
function FangshiDlgGetLayerResult(recvValue)
	m_recvVisit = recvValue;
	m_GetIndex = 1;	
	if m_recvVisit.m_count > 0 then 
		SetTrue(m_uiGetLayer);
		m_IsPlayGet = true;
		FangshiDlgPlayGet();		
	end	
end

-- 初始化巡游奖励界面
function FangshiDlgInitGetLayer()
	SetTrue(m_uiGetLayer)
	FangshiDlgClearGetGrid();
	for i = 1 ,m_recvInfo.m_awardcount do 
		local data = m_recvInfo.m_awardlist[i];
		FangshiDlgCreatGetGrid(data);
	end
end


-- 播放巡游奖励动画
function FangshiDlgPlayGet()	
	SetTrue(m_uiGetEffect);	
	local unitTween = m_uiGetEffect.transform:GetComponent( "UITweenMove" );
	local unitRect = m_uiGetEffect.transform:GetComponent( "RectTransform" );	
	startPos = FangshiDlgGetPlacePos(m_recvVisit.m_list[m_GetIndex].m_nodeid);
	targetPos = Vector2.New( m_uiGetLayer.transform.localPosition.x, m_uiGetLayer.transform.localPosition.y );
	unitTween.from = startPos;
	unitTween.to = targetPos;	
	if m_recvVisit.m_count > 1 then 
		unitTween.duration = 0.2;
	else
		unitTween.duration = 0.4;
	end
	unitTween:Play(true);
	
	
	FangshiDlgSynchroRecv();
	FangshiDlgSetPlaceInfo();
	FangshiDlgSetIncognitoButtons();
	if m_recvVisit.m_list[m_GetIndex].m_girlkind > 0 then
		m_IsShowGetGirl = true;
	else
		m_IsShowGetGirl = false;
	end
end

--	播放动画结束
function FangshiDlgPlayGetOff()
	local award = {};
	award.m_kind = m_recvVisit.m_list[m_GetIndex].m_awardkind;	
	award.m_num = m_recvVisit.m_list[m_GetIndex].m_awardnum;
	FangshiDlgCreatGetGrid(award);
	m_GetIndex = m_GetIndex + 1;
	SetFalse(m_uiGetEffect);
	if m_IsShowGetGirl == true then
		GirlGetDlgShowByFansghi(m_recvVisit.m_list[m_GetIndex].m_girlkind,FangshiDlgPlayGetNext())
	else
		FangshiDlgPlayGetNext()
	end
		
end

--	继续播放动画
function FangshiDlgPlayGetNext()
	if m_GetIndex <= m_recvVisit.m_count then
		FangshiDlgPlayGet();
	else			
		m_IsPlayGet = false;
	end
end
-- 获得动画特效开始坐标
function FangshiDlgGetPlacePos(NodeID)	
	local ChildIndex = FangshiDlgNodetoChild(NodeID);
	local uiObj = m_uiPlaceLayer.transform:GetChild( ChildIndex );
	local Pos = Vector2.New(uiObj.localPosition.x,uiObj.localPosition.y);
	return Pos ;
end

-- 同步巡游数据
function FangshiDlgSynchroRecv()
	m_recvInfo.m_freenum = m_recvVisit.m_freenum;
	m_recvInfo.m_awardcount = m_recvInfo.m_awardcount + 1;
	m_recvInfo.m_nodeid = m_recvVisit.m_list[m_GetIndex].m_nodeid;
	local award = {};
	award.m_kind = m_recvVisit.m_list[m_GetIndex].m_awardkind;	
	award.m_num = m_recvVisit.m_list[m_GetIndex].m_awardnum;
	table.insert(m_recvInfo.m_awardlist,award);
end

-- 巡游奖励子控件
function FangshiDlgCreatGetGrid(data)
	local uiObj = m_ObjectPool:Get( "UIP_GetGrid" );
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiShape= objs[0];	
	local uiColor = objs[1];
	local uiNum = objs[2];
	local uiSui = objs[3];
	local sprite, color, name, c, desc = AwardInfo( data.m_kind );	
	SetImage(uiShape,sprite)
	SetImage(uiColor,color)
	SetText(uiNum,"x"..data.m_num);
	
	if data.m_kind > AWARDKIND_GIRLSOULBASE then 
		SetTrue(uiSui);
		SetTrue(uiNum);
	elseif data.m_kind >= AWARDKIND_GIRLBASE and data.m_kind <AWARDKIND_GIRLSOULBASE then 
		SetFalse(uiSui);
		SetFalse(uiNum);
	else
		SetFalse(uiSui);
		SetTrue(uiNum);
	end		
	
	uiObj.transform:SetParent( m_uiListContent.transform );
	uiObj.gameObject:SetActive( true );
	
	SetControlID(uiObj,m_uiListContent.transform.childCount+20);
end

-- 清空巡游奖励子控件
function FangshiDlgClearGetGrid()
	local objs = {};
	for i=0,m_uiListContent.transform.childCount-1 do
		table.insert(objs,m_uiListContent.transform:GetChild(i).gameObject);
	end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "UIP_GetGrid(Clone)" then
			m_ObjectPool:Release( "UIP_GetGrid", obj );
		end
	end
end

-- 显示巡游奖励Tips
function FangshiDlgShowAwardDesc(awardIndex)	
	-- 位置
	local uiObj = m_uiListContent.transform:GetChild( awardIndex - 1 );
	m_uiAwardDescLayer.transform:SetParent( uiObj.transform )	
	local anchoredX = 0;
	if awardIndex == 1 or awardIndex == 8 then 
		anchoredX = 60;
	elseif awardIndex == 7 or awardIndex == 14 then 
		anchoredX = -60;
	end
	m_uiAwardDescLayer.transform.anchoredPosition = Vector2( anchoredX, -190 )	
	m_uiAwardDescLayer.transform:SetParent( m_Dlg.transform )
	local _, _, name, c, desc = AwardInfo( m_recvInfo.m_awardlist[awardIndex].m_kind )
	SetText( m_uiAwardDescLayer.transform:Find("Name"), name, NameColor(c) )	
	local _desc = string.split( desc, "\n")
	if _desc ~= nil and _desc[1] ~= nil then
		SetText( m_uiAwardDescLayer.transform:Find("Desc"), _desc[1] )
	else
		SetText( m_uiAwardDescLayer.transform:Find("Desc"), desc )
	end
	SetText( m_uiAwardDescLayer.transform:Find("Warn"), "" )
	SetTrue( m_uiAwardDescLayer )
end

-- 关闭巡游奖励Tips
function FangshiDlgCloseAwardDesc()
	SetFalse( m_uiAwardDescLayer );
end

-- 领取巡游奖励
function FangshiDlgOnBtnGet()
	SetFalse(m_uiGetLayer)
	system_askinfo( ASKINFO_FANGSHI, "", 2 );
	if m_recvInfo then
		m_recvInfo.m_awardcount = 0 ;
		m_recvInfo.m_awardlist = {};
	end
	FangshiDlgClearGetGrid();
	FangshiDlgSetIncognitoButtons();
end

-- 打开皇宫内院界面
function FangshiDlgOnBtnGarth()
	system_askinfo( ASKINFO_FANGSHI, "", 4 )
end

-- 初始化皇宫内院界面
function FangshiDlgGarthRecv(recvValue)
	m_recvGarth = recvValue;
	SetTrue(m_uiGarthLayer);
	m_grathSelect = 0;
	FangshiDlgClearGarthTopGrid();
	FangshiDlgClearGarthBottomGrid();
	for i = 1 , m_recvGarth.m_count do 
		local data = m_recvGarth.m_list[i];
		if i <= 2 then 
			FangshiDlgCreatGarthTopGrid(i,data);
		else
			FangshiDlgCreatGarthBottomGrid(data);
		end
	end
	FangshiDlgSetGarthSelect();
end 

-- 设置选择觐见的女将
function FangshiDlgSetGarthSelect()
	for i = 0,m_uiTopSroll.transform.childCount-1 do
		local uiObj = m_uiTopSroll.transform:GetChild(i);
		local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
		local uiSelect = objs[3];
		if i == m_grathSelect then 
			SetTrue(uiSelect);
		else
			SetFalse(uiSelect);
		end
	end
end

-- 创建皇宫内院主要女将
function FangshiDlgCreatGarthTopGrid(index,data)
	local uiObj = m_ObjectPool:Get( "UIP_GirlGrid" );
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiShape= objs[0];	
	local uiColor = objs[1];
	local uiName = objs[2];
	local sprite, color, name, c, desc = AwardInfo( data.m_kind );	
	SetImage(uiShape,sprite);
	SetImage(uiColor,color);
	SetText(uiName,name);	
	SetTextColor(uiName,NameColor(c))
	
	uiObj.transform:SetParent( m_uiTopSroll.transform );
	uiObj.gameObject:SetActive( true );
	SetControlID(uiObj,50 + index - 1);
end

-- 清空皇宫内院主要女将
function FangshiDlgClearGarthTopGrid()
	local objs = {};
	for i=0,m_uiTopSroll.transform.childCount-1 do
		table.insert(objs,m_uiTopSroll.transform:GetChild(i).gameObject);
	end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "UIP_GirlGrid(Clone)" then
			m_ObjectPool:Release( "UIP_GirlGrid", obj );
		end
	end
end

-- 创建皇宫内院次要女将
function FangshiDlgCreatGarthBottomGrid(data)
	local uiObj = m_ObjectPool:Get( "UIP_GirlGrid" );
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiShape= objs[0];	
	local uiColor = objs[1];
	local uiName = objs[2];
	
	local sprite, color, name, c, desc = AwardInfo( data.m_kind );	
	SetImage(uiShape,sprite);
	SetImage(uiColor,color);
	SetText(uiName,name);
	SetTextColor(uiName,NameColor(c))
	
	uiObj.transform:SetParent( m_uiBottomSroll.transform );
	uiObj.gameObject:SetActive( true );
end

-- 清空皇宫内院次要女将
function FangshiDlgClearGarthBottomGrid()
	local objs = {};
	for i=0,m_uiBottomSroll.transform.childCount-1 do
		table.insert(objs,m_uiBottomSroll.transform:GetChild(i).gameObject);
	end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "UIP_GirlGrid(Clone)" then
			m_ObjectPool:Release( "UIP_GirlGrid", obj );
		end
	end
end

-- 点击觐见
function FangshiDlgOnBtnSee()
	system_askinfo( ASKINFO_FANGSHI, "", 5, m_grathSelect )
end

-- 初始化觐见界面
function FangshiDlgSeeRecv(recvValue)
	m_recvSee = recvValue;
	SetTrue(m_uiSeeLayer);	
	SetFalse(m_uiGarthLayer);	
	for i = 1 , m_recvSee.m_count do
		local data = m_recvSee.m_list[i];
		FangshiDlgSetSeeGrid(i,data)
	end
end

-- 觐见奖励子控件
function FangshiDlgSetSeeGrid(index,data)
	local uiObj = m_uiSeeLayer.transform:Find("SeeGrid"..index);
    local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiShape= objs[0];	
	local uiColor = objs[1];
	local uiName = objs[2];
	local uiNum = objs[3]; 
	local uiSui = objs[4]; 
	local sprite, color, name, c, desc = AwardInfo( data.m_awardkind );	
	SetImage(uiShape,sprite);
	SetImage(uiColor,color);
	SetText(uiName,name);
	SetTextColor(uiName,NameColor(c))
	SetText(uiNum,"x"..data.m_awardnum)
	
	if data.m_awardkind > AWARDKIND_GIRLSOULBASE then 
		SetTrue(uiSui);
		SetTrue(uiNum);
	elseif data.m_awardkind >= AWARDKIND_GIRLBASE and data.m_awardkind <AWARDKIND_GIRLSOULBASE then 
		SetFalse(uiSui);
		SetFalse(uiNum);
	else
		SetFalse(uiSui);
		SetTrue(uiNum);
	end	
end

-- 关闭觐见获得界面
function FangshiDlgOnBtnCloseSee()
	SetFalse(m_uiSeeLayer);
end

-- 关闭皇宫内院界面
function FangshiDlgOnBtnCloseGarth()
	SetFalse(m_uiGarthLayer);
end