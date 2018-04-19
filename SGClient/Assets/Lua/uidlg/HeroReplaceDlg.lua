-- 界面
local m_Dlg = nil;
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiUIP_HeroHead = nil; --UnityEngine.GameObject
local m_uiDesc = nil; --UnityEngine.GameObject

local m_upkind = 0;
local m_selectkind = 0;
local m_path = 0;
-- 打开界面
function HeroReplaceDlgOpen()
	m_Dlg = eye.uiManager:Open( "HeroReplaceDlg" );
end

-- 隐藏界面
function HeroReplaceDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "HeroReplaceDlg" );
end

-- 删除界面
function HeroReplaceDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function HeroReplaceDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            HeroReplaceDlgClose();
		
		elseif nControlID == 1 then
			HeroReplaceDlgReplace()
			
		elseif nControlID > 1000 and nControlID < 2000 then
			HeroReplaceDlgSelectHero( nControlID - 1000 )
        end
	end
end

-- 载入时调用
function HeroReplaceDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiContent = objs[0];
	m_uiUIP_HeroHead = objs[1];
	m_uiDesc = objs[2];
end

-- 界面初始化时调用
function HeroReplaceDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function HeroReplaceDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function HeroReplaceDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function HeroReplaceDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function HeroReplaceDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function HeroReplaceDlgShow( path, upkind )
	m_path = path
	m_upkind = upkind
	HeroReplaceDlgOpen()
	HeroReplaceDlgSetHero()
	HeroReplaceDlgSelectHero(-1)
end

-- 英雄
function HeroReplaceDlgSetHero()
	local baseoffset = 0; 
	if m_path == HEROLIST_PATH_HERO or m_path == HEROLIST_PATH_HERO_LIST then -- 上阵武将
		baseoffset = 0;
		
	elseif m_path == HEROLIST_PATH_HERO_GATHER then -- 财赋署武将
		baseoffset = 4
		
	elseif m_path == HEROLIST_PATH_HERO_GUARD then -- 御林卫武将
		baseoffset = 8	
	end
	
	local isfull = 1;
	for i=0,3,1 do
		local uiHero = m_uiContent.transform:GetChild(i).gameObject;
		local pHero = GetHero().m_CityHero[i+baseoffset]
		SetTrue( uiHero )
		SetControlID( uiHero, 1000+pHero.m_kind )
		local objs = uiHero.transform:GetComponent( typeof(Reference) ).relatedGameObject;
		local uiShape = objs[0];
		local uiColor = objs[1];
		local uiCorps = objs[2];
		local uiName = objs[3];
		local uiSelect = objs[4];
		local uiType = objs[5];
			
		if pHero.m_kind > 0 then		
			SetTrue( uiShape )
			SetTrue( uiColor )
			SetTrue( uiCorps )
			SetTrue( uiName )
			SetFalse( uiSelect )
			
			SetImage( uiShape, HeroHeadSprite( pHero.m_kind )  );
			SetImage( uiColor,  ItemColorSprite( pHero.m_color )  );
			SetImage( uiCorps,  CorpsSprite( pHero.m_corps )  );
			SetText( uiName, HeroNameLv( pHero.m_kind, pHero.m_level ) );
			
			local only = GetHero():IsNationHeroOnly( pHero.m_kind )
			if only == true and pHero.m_god == 1 then
				SetTrue( uiType )
				SetText( uiType, T(2359) )
			elseif only == true then
				SetTrue( uiType )
				SetText( uiType, T(2357) )
			elseif pHero.m_god == 1 then
				SetTrue( uiType )
				SetText( uiType, T(2358) )
			else
				SetFalse( uiType )
			end
	
		else
			SetTrue( uiShape )
			SetFalse( uiColor )
			SetFalse( uiCorps )
			SetFalse( uiName )
			SetFalse( uiSelect )
			SetImage( uiShape, LoadSprite( "ui_icon_back_2" )  );
			isfull = 0;
		end
	end
	
	if isfull == 1 then
		SetTrue( m_uiDesc )
		SetText( m_uiDesc.transform:Find("Text"), T(1499) )
	else
		SetFalse( m_uiDesc )
	end
end

-- 选择英雄
function HeroReplaceDlgSelectHero( kind )
	m_selectkind = kind
	
	local baseoffset = 0; 
	if m_path == HEROLIST_PATH_HERO or m_path == HEROLIST_PATH_HERO_LIST then -- 上阵武将
		baseoffset = 0;
		
	elseif m_path == HEROLIST_PATH_HERO_GATHER then -- 财赋署武将
		baseoffset = 4
		
	elseif m_path == HEROLIST_PATH_HERO_GUARD then -- 御林卫武将
		baseoffset = 8	
	end
	
	for i=0,3,1 do
		local uiHero = m_uiContent.transform:GetChild(i).gameObject;
		local objs = uiHero.transform:GetComponent( typeof(Reference) ).relatedGameObject;
		local uiSelect = objs[4];
		
		local pHero = GetHero().m_CityHero[i+baseoffset]
		if pHero.m_kind > 0 and pHero.m_kind == kind then
			m_pHero = pHero;
			SetTrue( uiSelect )
		else
			SetFalse( uiSelect )
		end
	end
end

function HeroReplaceDlgReplace()
	if m_selectkind <= 0 or m_upkind <= 0 then
		return;
	end
	MsgBoxExLabel( F(836, HeroName( m_selectkind )), T(837), function( sure, toggle )
		if sure == 1 then
			local replace_equip = 0;
			if toggle == true then
				replace_equip = 1;
			end
			system_askinfo( ASKINFO_HERO, "", 3, m_selectkind, m_upkind, replace_equip );
			HeroReplaceDlgClose();
			HeroInfoDlgClose();
		end
	end )
end
