-- 界面
local m_Dlg = nil;
local m_uiLeftBtn = nil; --UnityEngine.GameObject
local m_uiRightBtn = nil; --UnityEngine.GameObject
local m_uiInfoContent = nil; --UnityEngine.GameObject
local m_uiRankTypeText = nil; --UnityEngine.GameObject
local m_uiMyRankText = nil; --UnityEngine.GameObject
local t_uiInfoContentObj = {};

local m_selectType = 0; -- 类型:1全服，2区域， 3国家
local t_selectTypeName = {[1]=1983,[2]=1984,[3]=1985}
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
			RankListDlgPre();
		elseif nControlID == 14 then
			RankListDlgNext();
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
	m_uiRankTypeText = objs[3];
	m_uiMyRankText = objs[4];
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
	t_uiInfoContentObj = GetChild(m_uiInfoContent);
	m_selectType = 1;
	RankListDlgSelectType(m_selectType);
	RankListDlgSetPointShow(1,3,m_selectType);
end
-- m_count=0,m_list={m_rank=0,m_namelen=0,m_name="[m_namelen]",m_nation=0,m_level=0,m_place=0,m_battlepower=0,m_actorid=0,[m_count]},m_myrank=0,m_type=0,m_page=0,
function RankListDlgRecv( recvValue )
	if recvValue.m_count == 0 then
		-- 提示没有更多了
		return
	end
	m_page = recvValue.m_page;
	SetText(m_uiMyRankText,F(1977,recvValue.m_myrank));
	for i = 1,#t_uiInfoContentObj,1 do
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
	if rankInfo == nill then
		SetFalse(t_uiInfoContentObj[index]);
	else
		SetTrue(t_uiInfoContentObj[index]);
		local objs = t_uiInfoContentObj[index]:GetComponent( typeof(Reference) ).relatedGameObject;
		t_uiInfoContentObj[index]:GetComponent( typeof(UIButton) ).controlID = index;
		uiRank = objs[0];
		uiNation = objs[1];
		uiName = objs[2];
		uiLevel = objs[3];
		uiPlace = objs[4];
		uiBattlePower = objs[5];
		local textColorSet=nil;
		if rankInfo.m_rank == 1 then
			textColorSet=NameColor(5);
		elseif rankInfo.m_rank == 2 then
			textColorSet=NameColor(4);
		elseif rankInfo.m_rank == 3 then
			textColorSet=NameColor(3);
		else 
			textColorSet=NameColor(0);
		end
		SetText(uiRank,rankInfo.m_rank,textColorSet);
		SetImage( uiNation,NationSprite(rankInfo.m_nation));
		SetText(uiName,rankInfo.m_name,textColorSet);
		SetText(uiLevel,rankInfo.m_level,textColorSet);
		SetText(uiPlace,PlaceName(rankInfo.m_place),textColorSet);
		SetText(uiBattlePower,rankInfo.m_battlepower,textColorSet);
	end
end

--向左
function RankListDlgLeft()
	m_selectType = m_selectType -1;
	RankListDlgSetPointShow(1,3,m_selectType);
	RankListDlgSelectType( m_selectType );
end
--向右 
function RankListDlgRight()
	m_selectType = m_selectType+1;
	RankListDlgSetPointShow(1,3,m_selectType);
	RankListDlgSelectType(m_selectType);
end

--设置左右按钮的显隐
function RankListDlgSetPointShow( i_min,i_max,index)
	if index == i_min  then
		SetFalse(m_uiLeftBtn);
		SetTrue(m_uiRightBtn);
	elseif index == i_max then
		SetFalse(m_uiRightBtn);
		SetTrue(m_uiLeftBtn);
	else
		SetTrue(m_uiLeftBtn);
		SetTrue(m_uiRightBtn);
	end
	SetText(m_uiRankTypeText,T(t_selectTypeName[index]));
	
end
