-- 界面
local m_Dlg = nil;
local m_uiAwardName = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiUIP_Award = nil; --UnityEngine.GameObject
local m_uiLeft = nil; --UnityEngine.GameObject
local m_ObjectPool = nil;

-- 打开界面
function LostRebuildDlgOpen()
	m_Dlg = eye.uiManager:Open( "LostRebuildDlg" );
end

-- 隐藏界面
function LostRebuildDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "LostRebuildDlg" );
end

-- 删除界面
function LostRebuildDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function LostRebuildDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            --LostRebuildDlgClose();
		elseif nControlID == 1 then
			LostRebuildDlgGet();
        end
	end
end

-- 载入时调用
function LostRebuildDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;
	m_uiAwardName = objs[0];
	m_uiContent = objs[1];
	m_uiUIP_Award = objs[2];
	m_uiLeft = objs[3];
	
	-- 对象池
	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );
	m_ObjectPool:CreatePool("m_uiUIP_Award", 3, 3, m_uiUIP_Award);
end

-- 界面初始化时调用
function LostRebuildDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function LostRebuildDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function LostRebuildDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function LostRebuildDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function LostRebuildDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
-- m_rb_silver=0,m_rb_wood=0,m_rb_food=0,m_rb_df=0,
function LostRebuildDlgShow( recvValue )
	LostRebuildDlgOpen()
	LostRebuildDlgClear()
	
	local sprite, name, left = NpcTalkShapeInfo( 0 );
	SetTrue( m_uiLeft )
	SetImage( m_uiLeft.transform:Find("Shape"), sprite );
	SetText( m_uiLeft.transform:Find("Name"), name );
	
	-- 奖励
	if recvValue.m_rb_silver > 0 then
		local sprite, color, name = AwardInfo( AWARDKIND_SILVER )
		local uiObj = m_ObjectPool:Get( "m_uiUIP_Award" );
		uiObj.transform:SetParent( m_uiContent.transform );
		SetImage( uiObj.transform:Find("Shape"), sprite );
		SetText( uiObj.transform:Find("Name"), name );
		SetText( uiObj.transform:Find("Num"), "x"..knum(recvValue.m_rb_silver) );
	end
	if recvValue.m_rb_wood > 0 then
		local sprite, color, name = AwardInfo( AWARDKIND_WOOD )
		local uiObj = m_ObjectPool:Get( "m_uiUIP_Award" );
		uiObj.transform:SetParent( m_uiContent.transform );
		SetImage( uiObj.transform:Find("Shape"), sprite );
		SetText( uiObj.transform:Find("Name"), name );
		SetText( uiObj.transform:Find("Num"), "x"..knum(recvValue.m_rb_wood) );
	end
	if recvValue.m_rb_food > 0 then
		local sprite, color, name = AwardInfo( AWARDKIND_FOOD )
		local uiObj = m_ObjectPool:Get( "m_uiUIP_Award" );
		uiObj.transform:SetParent( m_uiContent.transform );
		SetImage( uiObj.transform:Find("Shape"), sprite );
		SetText( uiObj.transform:Find("Name"), name );
		SetText( uiObj.transform:Find("Num"), "x"..knum(recvValue.m_rb_food) );
	end
	if recvValue.m_rb_df > 0 then
		local sprite, color, name = AwardInfo( AWARDKIND_CITYGUARDNUM )
		local uiObj = m_ObjectPool:Get( "m_uiUIP_Award" );
		uiObj.transform:SetParent( m_uiContent.transform );
		SetImage( uiObj.transform:Find("Shape"), sprite );
		SetText( uiObj.transform:Find("Name"), name );
		SetText( uiObj.transform:Find("Num"), recvValue.m_rb_df );
	end
end

-- 清空
function LostRebuildDlgClear()
	local objs = {};
	for i = 0 ,m_uiContent.transform.childCount - 1 do
		table.insert( objs, m_uiContent.transform:GetChild(i).gameObject )
    end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "m_uiUIP_Award(Clone)" then
			m_ObjectPool:Release( "m_uiUIP_Award", obj );
		end
    end
end

-- 领取奖励
function LostRebuildDlgGet()
	system_askinfo( ASKINFO_LOSTREBUILD, "", 1 );
	LostRebuildDlgClose();
end
