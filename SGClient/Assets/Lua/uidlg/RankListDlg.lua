-- 界面
local m_Dlg = nil;
local m_uiLeftBtn = nil; --UnityEngine.GameObject
local m_uiRightBtn = nil; --UnityEngine.GameObject
local m_uiInfoContent = nil; --UnityEngine.GameObject
local t_uiInfoContentObj = {};

local m_selectType = 0; -- 类型:1全服，2区域， 3国家
local m_page = 0; -- 当前页

-- 发送结构
local sendValue = {}
sendValue.m_type = 0;
sendValue.m_page = 0;
sendValue.m_myrange = 0;

-- 打开界面
function RankListDlgOpen()
	m_Dlg = eye.uiManager:Open( "RankListDlg" );
end

-- 隐藏界面
function RankListDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "RankListDlg" );
end

-- 删除界面
function RankListDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function RankListDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            RankListDlgClose();
		elseif nControlID == 11 then
			RankListDlgLeft();
		elseif	nControlID == 12 then
			RankListDlgRight();
		elseif nControlID == 13 then
			RankListDlgLastPage();
		elseif nControlID == 14 then
			RankListDlgNextPage();
        end
	end
end

-- 载入时调用
function RankListDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiLeftBtn = objs[0];
	m_uiRightBtn = objs[1];
	m_uiInfoContent = objs[2];
end

-- 界面初始化时调用
function RankListDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function RankListDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function RankListDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function RankListDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function RankListDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function RankListDlgShow()
	RankListDlgOpen()
	RankListDlgGetInfoContent();
	t_uiInfoContentObj = GetChild(m_uiInfoContent);
end
-- m_count=0,m_list={m_rank=0,m_namelen=0,m_name="[m_namelen]",m_nation=0,m_level=0,m_place=0,m_battlepower=0,[m_count]},m_myrank=0,m_type=0,m_page=0,
function RankListDlgRecv( recvValue )
	if recvValue.m_count == 0 then
		-- 提示没有更多了
		return
	end
	
	m_page = recvValue.m_page;
	for i=1, recvValue.m_count, 1 do
		local rankInfo = recvValue.m_list[i]
		-- 创建一行排名信息
		RankListDlgCreateRow( i, rankInfo )
	end
end

-- 选择排行榜类型
function RankListDlgSelectType( type )
	m_selectType = type
	sendValue.m_type = m_selectType;
	sendValue.m_page = 0;
	sendValue.m_myrange = 0;
	netsend_ranklistask_C( sendValue )
end

-- 上一页
function RankListDlgPre()
	sendValue.m_type = m_selectType;
	sendValue.m_page = m_page - 1;
	sendValue.m_myrange = 0;
	netsend_ranklistask_C( sendValue )
end

-- 下一页
function RankListDlgNext()
	sendValue.m_type = m_selectType;
	sendValue.m_page = m_page + 1;
	sendValue.m_myrange = 0;
	netsend_ranklistask_C( sendValue )
end

-- 我的
function RankListDlgMyRank()
	sendValue.m_type = m_selectType;
	sendValue.m_page = 0;
	sendValue.m_myrange = 1;
	netsend_ranklistask_C( sendValue )
end

-- 创建一行
function RankListDlgCreateRow( index, rankInfo )
	
end

--向左
function RankListDlgLeft()
	
end
--向右 
function RankListDlgRight()
	
end
--上一页
function RankListDlgLastPage()
	
end
--下一页
function RankListDlgNextPage()
	
end
--填充每行信息
function RankListDlgFillInfo( recvValue )
	
end




