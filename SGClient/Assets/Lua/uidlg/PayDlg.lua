-- 界面
local m_Dlg = nil;
local m_uiVipLevel = nil; --UnityEngine.GameObject
local m_uiVipNext = nil; --UnityEngine.GameObject
local m_uiVipProgress = nil; --UnityEngine.GameObject
local m_uiMCard = nil; --UnityEngine.GameObject
local m_uiGoodsScroll = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiUIP_Goods = nil; --UnityEngine.GameObject

local m_ObjectPool = nil;
-- 打开界面
function PayDlgOpen()
	m_Dlg = eye.uiManager:Open( "PayDlg" );
end

-- 隐藏界面
function PayDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "PayDlg" );
end

-- 删除界面
function PayDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function PayDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            PayDlgClose();
        end
	end
end

-- 载入时调用
function PayDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;
	m_uiVipLevel = objs[0];
	m_uiVipNext = objs[1];
	m_uiVipProgress = objs[2];
	m_uiMCard = objs[3];
	m_uiGoodsScroll = objs[4];
	m_uiContent = objs[5];
	m_uiUIP_Goods = objs[6];
	-- 对象池
	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );
	m_ObjectPool:CreatePool("UIP_Goods", 6, 6, m_uiUIP_Goods);
end

-- 界面初始化时调用
function PayDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function PayDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function PayDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function PayDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function PayDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function PayDlgShow()
	PayDlgOpen()
	system_askinfo( ASKINFO_PAY, "", 0 )
end

-- m_count=0,m_list={m_goodsid=0,m_price=0,m_token=0,m_day=0,m_nameid=0,m_icon=0,[m_count]},
function PayDlgRecv( recvValue )
	
end
