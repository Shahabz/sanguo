-- 界面
local m_Dlg = nil;
local m_uiRankLayer = nil; --UnityEngine.GameObject
local m_uiRankScroll = nil; --UnityEngine.GameObject
local m_uiRankContent = nil; --UnityEngine.GameObject
local m_uiUIP_Rank = nil; --UnityEngine.GameObject
local m_uiWaiting = nil; --UnityEngine.GameObject
local m_ObjectPool = nil;
local m_RankRecvValue = nil
-- 打开界面
function Activity22RankDlgOpen()
	ResourceManager.LoadAssetBundle( "_ab_ui_static_pic_5" )
	m_Dlg = eye.uiManager:Open( "Activity22RankDlg" );
end

-- 隐藏界面
function Activity22RankDlgClose()
	if m_Dlg == nil then
		return;
	end
	eye.uiManager:Close( "Activity22RankDlg" );
	m_RankRecvValue = nil
end

-- 删除界面
function Activity22RankDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
	ResourceManager.UnloadAssetBundle( "_ab_ui_static_pic_5" )
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function Activity22RankDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            Activity22RankDlgClose();
        end
	end
end

-- 载入时调用
function Activity22RankDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiRankLayer = objs[0];
	m_uiRankScroll = objs[1];
	m_uiRankContent = objs[2];
	m_uiUIP_Rank = objs[3];
	m_uiWaiting = objs[4];
	
	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );
	m_ObjectPool:CreatePool("UIP_Rank", 30, 30, m_uiUIP_Rank);
end

-- 界面初始化时调用
function Activity22RankDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function Activity22RankDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function Activity22RankDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function Activity22RankDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function Activity22RankDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function Activity22RankDlgShow()
	Activity22RankDlgOpen()
	Activity22RankDlgShowLayer()
end

-- 排行榜
function Activity22RankDlgShowLayer()
	SetTrue(m_uiRankLayer)
	m_RankRecvValue = nil
	if m_RankRecvValue == nil then
		SetTrue( m_uiWaiting )
		system_askinfo( ASKINFO_ACTIVITY22, "", 3 )
	else
		Activity22RankDlgRecv( m_RankRecvValue )
	end
end
-- 收到排行列表
function Activity22RankDlgRecv( recvValue )
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	Activity22RankDlgClear()
	SetFalse( m_uiWaiting )
	m_RankRecvValue = recvValue
	for i=1, recvValue.m_count, 1 do
		local info = recvValue.m_list[i]
		local uiObj = m_ObjectPool:Get( "UIP_Rank" );
		uiObj.transform:SetParent( m_uiRankContent.transform );
		uiObj.transform.localScale = Vector3.one;
		uiObj.gameObject:SetActive( true );
		Activity22RankDlgCreate( uiObj, info )
	end
end
-- 重置排名列表
function Activity22RankDlgClear()
	local objs = {};
	for i=0,m_uiRankContent.transform.childCount-1 do
		table.insert(objs,m_uiRankContent.transform:GetChild(i).gameObject);
	end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "UIP_Rank(Clone)" then
			m_ObjectPool:Release( "UIP_Rank", obj );
		end
	end
end
-- 创建排名
function Activity22RankDlgCreate( uiObj, info )
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiRank = objs[0]
	local uiRankImage = objs[1]
	local uiNation = objs[2]
	local uiName = objs[3]
	local uikill = objs[4]
	
	if info.m_rank == 1 then
		SetTrue( uiRankImage )
		SetImage( uiRankImage, LoadSprite( "ui_icon_rank1" ) )
	elseif info.m_rank == 2 then
		SetTrue( uiRankImage )
		SetImage( uiRankImage, LoadSprite( "ui_icon_rank2" ) )
	elseif info.m_rank == 3 then
		SetTrue( uiRankImage )
		SetImage( uiRankImage, LoadSprite( "ui_icon_rank3" ) )
	else
		SetFalse( uiRankImage )
	end
	SetText( uiRank, info.m_rank )
	SetImage( uiNation, NationSprite(info.m_nation) );
	SetText( uiName, info.m_name );
	SetText( uikill, info.m_kill )
end