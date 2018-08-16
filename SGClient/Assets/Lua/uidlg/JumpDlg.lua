-- 界面
local m_Dlg = nil;
local m_uiAlertText = nil; --UnityEngine.GameObject
local m_uiLevyButton = nil; --UnityEngine.GameObject
local m_uiBuyButton = nil; --UnityEngine.GameObject
local m_uiPayButton = nil; --UnityEngine.GameObject
local m_uiFreeButton = nil; --UnityEngine.GameObject
local m_uiCloseButton = nil; --UnityEngine.GameObject
local m_uiLeft = nil; --UnityEngine.GameObject
local m_uiFastButton = nil; --UnityEngine.GameObject

local m_res = 0;
local m_res_diff = 0;
-- 打开界面
function JumpDlgOpen()
	m_Dlg = eye.uiManager:Open( "JumpDlg" );
end

-- 隐藏界面
function JumpDlgClose()
	if m_Dlg == nil then
		return;
	end
	m_res = 0;
	eye.uiManager:Close( "JumpDlg" );
end

-- 删除界面
function JumpDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function JumpDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            JumpDlgClose();
			
		-- 征收
		elseif nControlID == 1 then
			JumpDlgClose();
			LevyDlgShow();
		
		-- 购买
		elseif nControlID == 2 then
			if m_res > 0 then
				if m_res == 1 then
					ShopDlgShowByKind( 1, AWARDKIND_LEVY_SILVER )
				elseif m_res == 2 then
					ShopDlgShowByKind( 1, AWARDKIND_LEVY_WOOD )
				elseif m_res == 3 then
					ShopDlgShowByKind( 1, AWARDKIND_LEVY_FOOD )
				elseif m_res == 4 then
					ShopDlgShowByKind( 1, AWARDKIND_LEVY_IRON )
				end
			else
				JumpDlgBodyBuy()
			end
			JumpDlgClose();
			
		-- 充值
		elseif nControlID == 3 then
			JumpDlgClose();
			PayDlgShow()
			EquipForgingDlgClose();
			
		-- 购买体力
		elseif nControlID == 4 then
			JumpDlgBodyBuy()
			JumpDlgClose();
		
		-- 一键补齐	
		elseif nControlID == 5 then
			JumpDlgFast();
        end
	end
end

-- 载入时调用
function JumpDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiAlertText = objs[0];
	m_uiLevyButton = objs[1];
	m_uiBuyButton = objs[2];
	m_uiPayButton = objs[3];
	m_uiFreeButton = objs[4];
	m_uiCloseButton = objs[5];
	m_uiLeft = objs[6];
	m_uiFastButton = objs[7];
end

-- 界面初始化时调用
function JumpDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function JumpDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function JumpDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function JumpDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function JumpDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function JumpDlgShow()
	JumpDlgOpen()
	local sprite, name, left = NpcTalkShapeInfo( 0 );
	SetTrue( m_uiLeft )
	SetImage( m_uiLeft.transform:Find("Shape"), sprite );
	SetText( m_uiLeft.transform:Find("Name"), name );
end

function JumpToken()
	JumpDlgShow()
	SetFalse( m_uiFastButton );
	SetFalse( m_uiCloseButton )
	SetFalse( m_uiLevyButton );
	SetFalse( m_uiBuyButton );
	SetTrue( m_uiPayButton );
	SetFalse( m_uiFreeButton )
	SetText( m_uiAlertText, F(4230, T(125)) )
end

function JumpVip( viplevel )
	JumpDlgShow()
	SetFalse( m_uiFastButton );
	SetFalse( m_uiCloseButton )
	SetFalse( m_uiLevyButton );
	SetFalse( m_uiBuyButton );
	SetTrue( m_uiPayButton );
	SetFalse( m_uiFreeButton )
	SetText( m_uiAlertText, F(2461, viplevel) )
end

function JumpRes( res, diff )
	JumpDlgShow()
	SetFalse( m_uiCloseButton )
	SetTrue( m_uiFastButton );
	SetTrue( m_uiBuyButton );
	SetFalse( m_uiPayButton );
	SetFalse( m_uiFreeButton )
	if GetPlayer().m_levynum <= 0 then
		SetFalse( m_uiLevyButton );
	else
		SetTrue( m_uiLevyButton );
	end
	SetText( m_uiAlertText, F(764, T(120+res), knum(diff)) )
	m_res = res
	m_res_diff = diff;
end

function JumpBody()
	JumpDlgShow()
	SetFalse( m_uiFastButton );
	SetFalse( m_uiCloseButton )
	SetFalse( m_uiLevyButton );
	SetFalse( m_uiPayButton );
	if Utils.get_int_sflag( GetPlayer().m_actor_sflag, ACTOR_SFLAG_BODY_FREEBUY ) == 1 then --首次购买
		SetFalse( m_uiFreeButton );
		SetTrue( m_uiBuyButton );
	else
		SetTrue( m_uiFreeButton );
		SetFalse( m_uiBuyButton );
	end
	SetText( m_uiAlertText, F(781, 100) )
end

function JumpDlgBodyBuy()
	if GetPlayer().m_body >= global.body_max then
		AlertMsg( T(782) )
		return
	end
	if Utils.get_int_sflag( GetPlayer().m_actor_sflag, ACTOR_SFLAG_BODY_FREEBUY ) == 1 then --首次购买
		-- 请求购买
		system_askinfo( ASKINFO_ACTOR, "", 0, 1 );
	else
		-- 直接购买
		system_askinfo( ASKINFO_ACTOR, "", 0, 0 );
	end
end

function JumpFightSkip()
	JumpDlgShow()
	SetFalse( m_uiFastButton );
	SetTrue( m_uiCloseButton )
	SetFalse( m_uiLevyButton );
	SetFalse( m_uiBuyButton );
	SetTrue( m_uiPayButton );
	SetFalse( m_uiFreeButton )
	SetText( m_uiAlertText, T(2337) )
end

function JumpDlgFast()
	system_askinfo( ASKINFO_BUYRES, "", m_res, m_res_diff, 1 )
end

function JumpDlgMsgBox( restype, resnum, token )
	MsgBox( F(4229,token), function()
		system_askinfo( ASKINFO_BUYRES, "", restype, resnum, 0 )
		JumpDlgClose()
	end )
end