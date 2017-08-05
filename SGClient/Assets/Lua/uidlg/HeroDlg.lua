-- 界面
local m_Dlg = nil;
local m_DialogFrameMod = nil;
local m_uiUIP_Hero = {nil,nil,nil,nil}; --UnityEngine.GameObject

-- 打开界面
function HeroDlgOpen()
	m_Dlg = eye.uiManager:Open( "HeroDlg" );
	m_DialogFrameMod = DialogFrameModOpen( m_Dlg, T(550), 2, HeroDlgClose );
end

-- 隐藏界面
function HeroDlgClose()
	if m_Dlg == nil then
		return;
	end
	DialogFrameModClose( m_DialogFrameMod );
	m_DialogFrameMod = nil;
	eye.uiManager:Close( "HeroDlg" );
end

-- 删除界面
function HeroDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function HeroDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            HeroDlgClose();
		elseif nControlID >= 1 and nControlID <= 4 then
			HeroDlgSelect( nControlID )
        end
	end
end

-- 载入时调用
function HeroDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;
	m_uiUIP_Hero[1] = objs[0];
	m_uiUIP_Hero[2] = objs[1];
	m_uiUIP_Hero[3] = objs[2];
	m_uiUIP_Hero[4] = objs[3];	
end

-- 界面初始化时调用
function HeroDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function HeroDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function HeroDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function HeroDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function HeroDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function HeroDlgShow()
	HeroDlgOpen();
	HeroDlgUpdate();
end

function HeroDlgUpdate()
	if m_Dlg == nil then
		return
	end
	local index = 1;
	for i=0,3,1 do
		HeroDlgSetHero( index, GetHero().m_CityHero[i] );
		index = index + 1;
	end
end

function HeroDlgSetHero( index, pHero )
	local objs = m_uiUIP_Hero[index].transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiShape = objs[0];
	local uiColor = objs[1];
	local uiCorps = objs[2];
	local uiName = objs[3];
	local uiSoldiersProgress = objs[4];
	local uiSoldiersText = objs[5];
	local uiSoldiersBtn = objs[6];
	local uiState = objs[7];
	local uiShapeBack = objs[8];
	local uiUnLockText = objs[9];
	
	if pHero == nil or pHero.m_kind <= 0 then
		SetTrue( uiShapeBack )
		SetTrue( uiUnLockText )
		SetFalse( uiShape )
		SetFalse( uiColor )
		SetFalse( uiCorps )
		SetFalse( uiName )
		SetFalse( uiSoldiersProgress )
		SetFalse( uiSoldiersText )
		SetFalse( uiState )
		return;
	end
	
	SetFalse( uiShapeBack )
	SetFalse( uiUnLockText )
	SetTrue( uiShape )
	SetTrue( uiColor )
	SetTrue( uiCorps )
	SetTrue( uiName )
	SetTrue( uiSoldiersProgress )
	SetTrue( uiSoldiersText )
	SetTrue( uiState )
	
	SetImage( uiShape, HeroHeadSprite( pHero.m_kind )  );
	SetImage( uiColor,  HeroColorSprite( pHero.m_color )  );
	SetImage( uiCorps,  CorpsSprite( pHero.m_corps )  );
	SetText( uiName, HeroNameLv( pHero.m_kind, pHero.m_level ) );
	SetProgress( uiSoldiersProgress, pHero.m_soldiers/pHero.m_troops );
	SetText( uiSoldiersText, pHero.m_soldiers.."/"..pHero.m_troops );
	if pHero.m_soldiers < pHero.m_troops then
		SetTrue( uiSoldiersBtn );
	else
		SetFalse( uiSoldiersBtn );
	end
	SetText( uiState, HeroState( pHero.m_state ) );
end

function HeroDlgSelect( index )
	if GetHero().m_CityHero[index] == nil then
		return;
	end
	HeroInfoDlgShow( GetHero().m_CityHero[index], true );
end
