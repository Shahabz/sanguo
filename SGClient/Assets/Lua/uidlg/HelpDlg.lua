-- 界面
local m_Dlg = nil;
local m_uiUIP_System = nil; --UnityEngine.GameObject
local m_uiTitleText = nil; --UnityEngine.GameObject
local m_uiInfoText = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiLayerDetails = nil; --UnityEngine.GameObject
local m_ObjectPool = nil;	--对象池
local m_uiNoticeText = nil; --UnityEngine.GameObject
local m_State=0;--界面状态（一级菜单页，二级菜单页，详情页）
local m_Index=nil;--界面显示的详情页HELP_UIDLG ID
local m_uiButtonFront = nil; --UnityEngine.GameObject
local m_uiButtonNext = nil; --UnityEngine.GameObject

--各个界面帮助ID

HELP_StoryDlg = 1501--副本
HELP_NationEquipDlg = 1502--国器
HELP_QuestDlg = 1503--任务
HELP_VipDlg = 1506--VIP
HELP_LevyDlg = 1520--官府
HELP_HeroDlg = 1521--武将
HELP_HeroListDlg=1521--聚贤馆
HELP_WishingDlg = 1522--聚宝盆
HELP_StoreDlg = 1523--仓库
HELP_EquipForgingDlg = 1524--铁匠
--HELP_BattleDlg = 1525--资源田
HELP_CityTechDlg = 1526--太学院
HELP_CityGuardDlg = 1527--城墙
HELP_EquipWashDlg = 1528--装备
HELP_MaterialMakeDlg = 1529--作坊
HELP_TrainDlg = 1530--兵营
HELP_CabinetDlg = 1531--内阁
HELP_BagDlg = 1532--背包
HELP_MailDlg = 1533--邮件
--HELP_BattleDlg = 1534--国家官职、职位
HELP_ChatDlg = 1535--聊天
--HELP_BattleDlg = 1536--公告
HELP_NationDlg = 1537--国家主界面
--HELP_BattleDlg = 1538--国家日志
--HELP_BattleDlg = 1539--国家名将
--HELP_BattleDlg = 1540--采集
--HELP_BattleDlg = 1541--皇城血战
--HELP_BattleDlg = 1542--皇城
--HELP_BattleDlg = 1543--流寇
--HELP_BattleDlg = 1544--NPC城池

--一级菜单
local m_HelpMainItems = { 1500,1501,1502,1503, 1504,1505,1506,1507 }

--二级菜单
local m_HelpMainItemsEx = {
[1500] = { 1520, 1521, 1522, 1523, 1524, 1525, 1526, 1527, 1528, 1529, 1530, 1531, 1532, 1533 },
[1504] = { 1534, 1535, 1536, 1537, 1538, 1539 },
[1505] = { 1540, 1541, 1542, 1543, 1544 },
} 


--详情
local m_HelpItemsDetails = {
[1520] = { textid = 1570,  pre=0, next=1521 }, 
[1521] = { textid = 1571,  pre=1520, next=1522 }, 
[1522] = { textid = 1572,  pre=1521, next=1523 }, 
[1523] = { textid = 1573,  pre=1522, next=1524 }, 
[1524] = { textid = 1574,  pre=1523, next=1525 }, 
[1525] = { textid = 1575,  pre=1524, next=1526 }, 
[1526] = { textid = 1576,  pre=1525, next=1527 }, 
[1527] = { textid = 1577,  pre=1526, next=1528 }, 
[1528] = { textid = 1578,  pre=1527, next=1529 }, 
[1529] = { textid = 1579,  pre=1528, next=1530 }, 
[1530] = { textid = 1580,  pre=1529, next=1531 }, 
[1531] = { textid = 1581,  pre=1530, next=1532 }, 
[1532] = { textid = 1582,  pre=1531, next=1533 }, 
[1533] = { textid = 1583,  pre=1532, next=1534 }, 

[1501] = { textid = 1584,  pre=1533, next=1502 }, 
[1502] = { textid = 1585,  pre=1501, next=1503 }, 
[1503] = { textid = 1586,  pre=1502, next=1534 }, 

[1534] = { textid = 1587,  pre=1503, next=1535 }, 
[1535] = { textid = 1588,  pre=1534, next=1536 }, 
[1536] = { textid = 1589,  pre=1535, next=1537 }, 
[1537] = { textid = 1590,  pre=1536, next=1538 }, 
[1538] = { textid = 1591,  pre=1537, next=1539 }, 
[1539] = { textid = 1592,  pre=1538, next=1540 }, 

[1540] = { textid = 1593,  pre=1539, next=1541 }, 
[1541] = { textid = 1594,  pre=1540, next=1542 }, 
[1542] = { textid = 1595,  pre=1541, next=1543 }, 
[1543] = { textid = 1596,  pre=1542, next=1544 }, 
[1544] = { textid = 1597,  pre=1543, next=1506 }, 

[1506] = { textid = 1598,  pre=1503, next=0 }, 
} 

--附加信息
local m_HelpItemsDetailsEx = {
[1502] = 1634,
[1503] = 1635,
[1521] = 1630,
[1522] = 1631,
[1523] = 1632,
[1526] = 1633,
[1543] = 1636,
[1544] = 1637,
} 


-- 打开界面
function HelpDlgOpen()
	m_Dlg = eye.uiManager:Open( "HelpDlg" );
end

-- 隐藏界面
function HelpDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "HelpDlg" );
	SetFalse(m_uiLayerDetails)
end

-- 删除界面
function HelpDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function HelpDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            HelpDlgClose();
			--返回按钮
		elseif nControlID == 1 then
            if m_State == 1 then
				HelpDlgShow()
			elseif m_State == 2 then
				if m_HelpItemsDetailsEx[m_uiIndex] ~= nil then
				HelpDlgShowEx(m_uiIndex)
				else
				SetFalse(m_uiLayerDetails)
				HelpDlgShow()
				end
			end
			
			--点击菜单
		elseif nControlID > 1499 then
		
			--点击反馈和意见
			if nControlID == m_HelpMainItems[#m_HelpMainItems] then
				print("需要接web")
			else
		
				--判断如果没有详情则显示二级菜单
				if	m_HelpItemsDetails[nControlID] == nil then
					HelpDlgClear()
					HelpDlgShowEx(nControlID)
				else
				
					--如果有详情则直接显示详情
					HelpDlgClear()
					HelpDlgShowDetails(nControlID)
				end
			end
			--点击上一条按钮
		elseif nControlID ==2 then	
	
		    HelpDlgShowDetails(m_HelpItemsDetails[m_Index].pre)
			--点击下一条按钮
		elseif nControlID ==3 then	
		    HelpDlgShowDetails(m_HelpItemsDetails[m_Index].next)

        end
	end
end

-- 载入时调用
function HelpDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiUIP_System = objs[0];
	m_uiTitleText = objs[1];
	m_uiInfoText = objs[2];
	m_uiContent = objs[3];
	m_uiLayerDetails = objs[4];
	m_uiNoticeText = objs[5];
	m_uiButtonFront = objs[6];
	m_uiButtonNext = objs[7];
	
	-- 对象池
	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );
	m_ObjectPool:CreatePool("UIP_System", 10, 10, m_uiUIP_System);
end

-- 界面初始化时调用
function HelpDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function HelpDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function HelpDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function HelpDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function HelpDlgOnLogic( gameObject )
	
end

function HelpDlgOnStart( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------


function HelpDlgShow()
	HelpDlgOpen()
	HelpDlgClear()
	for i=1, #m_HelpMainItems, 1 do
		local uiObj = m_ObjectPool:Get( "UIP_System" );
		uiObj.transform:SetParent( m_uiContent.transform );
		SetText( uiObj.transform:Find("Text"), T(m_HelpMainItems[i]) )
		uiObj.transform:GetComponent( typeof(UIButton) ).controlID=m_HelpMainItems[i]
	end
end

-- 清空
function HelpDlgClear()
	local objs = {};
	for i = 0 ,m_uiContent.transform.childCount - 1 do
		table.insert( objs, m_uiContent.transform:GetChild(i).gameObject )
    end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "UIP_System(Clone)" then
			m_ObjectPool:Release( "UIP_System", obj );
		end
    end
end

--显示二级菜单
function HelpDlgShowEx(nControlID)
	HelpDlgOpen()
	HelpDlgClear()
	
	for i=1, #m_HelpMainItemsEx[nControlID], 1 do
		local uiObj = m_ObjectPool:Get( "UIP_System" );
		uiObj.transform:SetParent( m_uiContent.transform );
		SetText( uiObj.transform:Find("Text"), T(m_HelpMainItemsEx[nControlID][i]) )
		uiObj.transform:GetComponent( typeof(UIButton) ).controlID=m_HelpMainItemsEx[nControlID][i]
	end
	
	--将界面状态置为二级菜单状态
	m_State=1
end

function HelpDlgShowDetails(nControlID)	
	--显示详情
	SetTrue(m_uiLayerDetails)
	SetText(m_uiTitleText,T(nControlID))
	SetText(m_uiInfoText,T(m_HelpItemsDetails[nControlID]["textid"]))
	--显示附加信息
	if m_HelpItemsDetailsEx[nControlID] ~= nil then
		SetText(m_uiNoticeText,T(m_HelpItemsDetailsEx[nControlID]))
		else
		SetFalse(m_uiNoticeText)
	end
	--将界面状态置为详情状态
	m_State=2
	
	--记录当前详情ID
	m_Index=nControlID
	
	--隐藏上一条按钮或下一条按钮
	if m_HelpItemsDetails[m_Index].pre == 0 then
		SetButtonFalse(m_uiButtonFront)
		else
		SetButtonTrue(m_uiButtonFront)
	end
	if m_HelpItemsDetails[m_Index].next == 0 then
		SetButtonFalse(m_uiButtonNext)
		else
		SetButtonTrue(m_uiButtonNext)
	end
end

function HelpDlgShowBySystem( id )
	HelpDlgOpen()
	HelpDlgClear()
	HelpDlgShowDetails( id )
end
