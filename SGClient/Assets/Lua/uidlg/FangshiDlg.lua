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
local m_uiInfoContent = nil; --UnityEngine.GameObject
local m_uiGetEffect = nil; --UnityEngine.GameObject

local m_ObjectPool = nil;
local m_recvInfo = nil;
local m_recvVisit = nil;
local m_placeSelect = nil;
local m_awardList = nil;
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
	m_placeSelect = nil;
	m_awardList = nil;
	m_cacheNodeAward = nil;
	eye.uiManager:Close( "FangshiDlg" );
end

-- 删除界面
function FangshiDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
	m_recvInfo = nil;
	m_recvVisit = nil;
	m_placeSelect = nil;
	m_awardList = nil;
	m_cacheNodeAward = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function FangshiDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
		if m_IsPlayGet == true then return end;
        if nControlID == -1 then
            FangshiDlgClose();
		elseif nControlID == 1 then 								-- 打开皇宫内院界面
			FangshiDlgOnBtnGarth();	
			warn("打开皇宫内院界面")
		elseif nControlID == 2 then									-- 普通巡游
			FangshiDlgOnBtnIncognito(1);		
			warn("普通巡游")
		elseif nControlID == 3 then									-- 多次巡游
			FangshiDlgOnBtnIncognito(2);				
			warn("多次巡游")
		elseif nControlID == 4 then									-- 领取巡游奖励
			FangshiDlgOnBtnGet();	
			warn("领取巡游奖励")
		elseif nControlID == 5 then									-- 关闭皇宫内院界面
			FangshiDlgOnBtnCloseGarth();
			warn("关闭皇宫内院界面")	
		elseif nControlID == 6 then									-- 觐见
			FangshiDlgOnBtnSee();
			warn("觐见")
		elseif nControlID == 7 then									-- 再次觐见
			FangshiDlgOnBtnSee();	
			warn("再次觐见")
		elseif nControlID == 8 then									-- 关闭觐见获得界面	
			FangshiDlgOnBtnCloseSee();	
			warn("关闭觐见获得界面")
		elseif nControlID == 9 then									-- 关闭巡游位置信息
			FangshiDlgClosePlaceInfo()
			warn("关闭巡游位置信息")			
		elseif nControlID == 100 then								-- 打开女将界面
			warn("打开女将界面")
		elseif nControlID == 101 then								-- 打开女将商店
			warn("打开女将商店")
		elseif nControlID >= 200 and nControlID <= 300 then				-- 查看巡游地点信息
			FangshiDlgOnBtnPlace( nControlID - 200 )
			warn("查看巡游地点信息")
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
	m_uiInfoContent = objs[24];
	
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
	system_askinfo( ASKINFO_FANGSHI, "", 0 );
end

-- 接收数据
function FangshiDlgInfoRecv( recvValue )		
	m_recvInfo = recvValue;	
	PrintTable(m_recvInfo,"数据")
	FangshiDlgSetPlaceInfo();
	FangshiDlgSetIncognitoButtons();
	--绘制未领取的巡游奖励
	if m_recvInfo.m_awardcount ~= 0 then 		
		FangshiDlgInitGetLayer();
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
	if bOver == false then 
		SetTrue(uiFoot);
		SetImage(uiFoot,LoadSprite("ui_fangshi_footoff"))
		SetFalse(uiRFoot);
		SetImage(uiColor,LoadSprite("ui_fangshi_quality0"))		
		SetButtonFalse(uiObj)
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
		SetButtonTrue(uiObj)
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
		SetText(m_uiIncognitoCount,F(4001,global.fangshi_visit_freenum - m_recvInfo.m_freenum));	
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
	warn("点击巡游位置："..index)	
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
	PrintTable(m_cacheNodeAward[m_placeSelect],"位置信息")
	FangshiDlgPlaceInfoShow()		
end

-- 位置信息框显示
function FangshiDlgPlaceInfoShow()	
	local ChildIndex = FangshiDlgNodetoChild(m_cacheNodeAward[m_placeSelect].m_value)
	--设置出现位置
	local uiObj = m_uiPlaceLayer.transform:GetChild( ChildIndex );
	m_uiPlaceInfo.transform.localPosition = Vector3.New( uiObj.localPosition.x,uiObj.localPosition.y, 0);	
	FangshiDlgSetPlaceSelect(ChildIndex);
	m_placeSelect = m_cacheNodeAward[m_placeSelect].m_value;	
	
	local objs = m_uiPlaceInfo.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiTitle = objs[0];
	local uiContent = objs[1];
	local TitleID = 4100 + m_placeSelect;
	SetText(uiTitle,T(TitleID))
	
	FangshiDlgClearPlaceInfoGrid(uiContent)
	
	for i = 1 , m_cacheNodeAward[m_placeSelect].m_count do 
		local data = m_cacheNodeAward[m_placeSelect].m_list[i]
		FangshiDlgCreatPlaceInfoGrid(uiContent,data);
	end	
	SetTrue(m_uiPlaceInfo);	
end

-- 创建位置信息子控件
function FangshiDlgCreatPlaceInfoGrid(uiContent,data)
	local uiObj = m_ObjectPool:Get( "UIP_InfoGrid" );
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiShape= objs[0];	
	local uiColor = objs[1];
	local uiName = objs[2];
	local sprite, color, name, c, desc = AwardInfo( data.m_kind );	
	SetImage(uiShape,sprite)
	SetImage(uiColor,color)
	SetText(uiName,name);
	
	uiObj.transform:SetParent( uiContent.transform );
	uiObj.gameObject:SetActive( true );
end

-- 清空位置信息子控件
function FangshiDlgClearPlaceInfoGrid(uiContent)
	local objs = {};
	for i=0,uiContent.transform.childCount-1 do
		table.insert(objs,uiContent.transform:GetChild(i).gameObject);
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
	PrintTable(m_recvVisit,"巡游奖励");
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
	if m_recvVisit.m_list[m_GetIndex].m_girlkind and m_recvVisit.m_list[m_GetIndex].m_girlkind > 0 then
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
		SetFalse(m_uiGetEffect);	
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
	PrintTable(m_recvInfo,"同步后数据")
end

-- 巡游奖励子控件
function FangshiDlgCreatGetGrid(data)
	local uiObj = m_ObjectPool:Get( "UIP_GetGrid" );
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiShape= objs[0];	
	local uiColor = objs[1];
	local uiNum = objs[2];
	local sprite, color, name, c, desc = AwardInfo( data.m_kind );	
	SetImage(uiShape,sprite)
	SetImage(uiColor,color)
	SetText(uiNum,data.m_num);
	
	uiObj.transform:SetParent( m_uiListContent.transform );
	uiObj.gameObject:SetActive( true );
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

-- 领取巡游奖励
function FangshiDlgOnBtnGet()
	SetFalse(m_uiGetLayer)
	system_askinfo( ASKINFO_FANGSHI, "", 2 );
	m_recvInfo.m_awardcount = 0 ;
	m_recvInfo.m_awardlist = {};
	FangshiDlgClearGetGrid();
	FangshiDlgSetIncognitoButtons();
end

-- 打开皇宫内院界面
function FangshiDlgOnBtnGarth()
	system_askinfo( ASKINFO_FANGSHI, "", 4 )
	SetTrue(m_uiGarthLayer);
	--FangshiDlgInitGarth();
end

-- 初始化皇宫内院界面
function FangshiDlgInitGarth()
	FangshiDlgClearGarthTopGrid();
	FangshiDlgClearGarthBottomGrid();
	for i = 1 , 2 do 
		FangshiDlgCreatGarthTopGrid();
	end
	for i = 1 , 3 do 
		FangshiDlgCreatGarthBottomGrid();
	end
end 

-- 创建皇宫内院主要女将
function FangshiDlgCreatGarthTopGrid(data)
	local uiObj = m_ObjectPool:Get( "UIP_GirlGrid" );
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiShape= objs[0];	
	local uiColor = objs[1];
	local uiName = objs[2];
	
	SetText(uiName,11);
	
	uiObj.transform:SetParent( m_uiTopSroll.transform );
	uiObj.gameObject:SetActive( true );
end

-- 清空皇宫内院主要女将
function FangshiDlgClearGarthTopGrid()
	local objs = {};
	for i=0,m_uiTopSroll.transform.childCount-1 do
		table.insert(objs,m_uiTopSroll.transform:GetChild(i).gameObject);
	end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "UIP_GetGrid(Clone)" then
			m_ObjectPool:Release( "UIP_GetGrid", obj );
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
	
	SetText(uiName,22);
	
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
		if obj.name == "UIP_GetGrid(Clone)" then
			m_ObjectPool:Release( "UIP_GetGrid", obj );
		end
	end
end

-- 点击觐见
function FangshiDlgOnBtnSee()
	FangshiDlgInitSeeLayer();
end

-- 初始化觐见界面
function FangshiDlgInitSeeLayer()
	SetTrue(m_uiSeeLayer);
	for i = 1 , 5 do
		
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