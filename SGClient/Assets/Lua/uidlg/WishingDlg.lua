-- 界面
local m_Dlg = nil;
local m_DialogFrameMod = nil;
local m_uiWishingBtn = nil; 
local m_uiChangeresBtn = nil;
local m_uiPackresBtn = nil; 

-- 打开界面
function WishingDlgOpen()
	m_Dlg = eye.uiManager:Open( "WishingDlg" );
	m_DialogFrameMod = DialogFrameModOpen( m_Dlg, BuildingName(36), HELP_WishingDlg, WishingDlgClose );
	
end

-- 隐藏界面
function WishingDlgClose()
	if m_Dlg == nil then
		return;
	end
	DialogFrameModClose( m_DialogFrameMod );
	m_DialogFrameMod = nil;
	eye.uiManager:Close( "WishingDlg" );
end

-- 删除界面
function WishingDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function WishingDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            WishingDlgClose();
		elseif nControlID >= 1 and nControlID <= 4 then
			WishingDlgSelectType( nControlID )
        end
	end
end

-- 载入时调用
function WishingDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiWishingBtn = objs[0];
	m_uiChangeresBtn = objs[1];
	m_uiPackresBtn = objs[2];
	m_uiTitle = objs[3];
	m_uiWishingScroll = objs[4];
	m_uiChangeresScroll = objs[5];
end

-- 界面初始化时调用
function WishingDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function WishingDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function WishingDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function WishingDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function WishingDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function WishingDlgShow()
	WishingDlgOpen()
	WishingDlgSelectType( 1 )
end

-- 选择分页
function WishingDlgSelectType( type )
	-- 聚宝盆
	if type == 1 then
		SetTrue(m_uiWishingScroll)
		SetFalse(m_uiChangeresScroll) 		
		SetImage( m_uiWishingBtn.transform:Find("Back"), LoadSprite("ui_button_page1") )
		SetImage( m_uiChangeresBtn.transform:Find("Back"), LoadSprite("ui_button_page2") )
		SetImage( m_uiPackresBtn.transform:Find("Back"), LoadSprite("ui_button_page2") )
		SetText( m_uiTitle, T(3004))
		--system_askinfo( ASKINFO_VIPSHOP, "", 0 );
		
	-- 换宝
	elseif type == 2 then
		SetFalse(m_uiWishingScroll)
		SetTrue(m_uiChangeresScroll) 
		SetImage( m_uiWishingBtn.transform:Find("Back"), LoadSprite("ui_button_page2") )
		SetImage( m_uiChangeresBtn.transform:Find("Back"), LoadSprite("ui_button_page1") )
		SetImage( m_uiPackresBtn.transform:Find("Back"), LoadSprite("ui_button_page2") )
		SetText( m_uiTitle, T(3005))

		--system_askinfo( ASKINFO_VIPBAG, "", 0 );
		
	-- 打包
	elseif type == 3 then
		SetFalse(m_uiWishingScroll)
		SetFalse(m_uiChangeresScroll) 
		SetImage( m_uiWishingBtn.transform:Find("Back"), LoadSprite("ui_button_page2") )
		SetImage( m_uiChangeresBtn.transform:Find("Back"), LoadSprite("ui_button_page2") )
		SetImage( m_uiPackresBtn.transform:Find("Back"), LoadSprite("ui_button_page1") )
		SetText( m_uiTitle, T(3006))
		--system_askinfo( ASKINFO_SHOP, "", 0, 1 );
	end
end