local m_uiUIP_SonInfo = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiWarn = nil; --UnityEngine.GameObject

-- 界面
local m_Dlg = nil;
--local m_DialogFrameMod = nil;

-- 打开界面
function HeroOffspringDlgOpen()
	m_Dlg = eye.uiManager:Open( "HeroOffspringDlg" );
	--m_DialogFrameMod = DialogFrameModOpen( m_Dlg, T(0), nil, HeroOffspringDlgClose );
end

-- 隐藏界面
function HeroOffspringDlgClose()
	if m_Dlg == nil then
		return;
	end
	--DialogFrameModClose( m_DialogFrameMod );
	--m_DialogFrameMod = nil;
	eye.uiManager:Close( "HeroOffspringDlg" );
end

-- 删除界面
function HeroOffspringDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function HeroOffspringDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            HeroOffspringDlgClose();
        end
	end
end

-- 载入时调用
function HeroOffspringDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiUIP_SonInfo = objs[0];
	m_uiContent = objs[1];
	m_uiWarn = objs[2];
	
	-- 对象池
	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );
	m_ObjectPool:CreatePool("UIP_SonInfo", 4, 4, m_uiUIP_SonInfo);
end

-- 界面初始化时调用
function HeroOffspringDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function HeroOffspringDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function HeroOffspringDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function HeroOffspringDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function HeroOffspringDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function HeroOffspringDlgShow(herokind)
	HeroOffspringDlgOpen()
	HeroOffspringDlgInit(herokind);
end

function HeroOffspringDlgInit(herokind)
	HeroOffspringDlgClear();
	local pHero = GetHero():GetPtr(herokind);
	local sonConfig = g_girlson[pHero.m_kind];
	if pHero.m_sonnum == 0 and pHero.m_sontime == 0 then
		SetTrue( m_uiWarn )
	else
		SetFalse( m_uiWarn )
		if sonConfig == nil then
			return;
		end
		
		local num = pHero.m_sonnum;
		if pHero.m_sontime > 0 then
			num = num + 1
		end
		if num > 4 then
			num = 4;
		end
		for i=0,num,1 do
			HeroOffspringDlgInfo(sonConfig[i],pHero,i);
		end
	end	
end

function HeroOffspringDlgInfo(config,pHero,i)
	local uiHeroObj = m_ObjectPool:Get( "UIP_SonInfo" );
	uiHeroObj.transform:SetParent( m_uiContent.transform );
	SetTrue( uiHeroObj )
	local objs = uiHeroObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiShape = objs[0];
	local uiColor = objs[1];
	local uiName = objs[2];
	local uiAttrText = objs[3];
	local uiNoFinish = objs[4];
	local uiFinish = objs[5];
	SetFalse(uiFinish);
	SetFalse(uiNoFinish);
	SetImage(uiShape,SonHeadSprite(config.client_shape));
	SetText(uiName,SonName(config.client_name));
	
	if pHero.m_sonnum > i then
		--出师了
		SetTrue(uiFinish);
	else
		--未出师
		SetTrue(uiNoFinish);
	end
	
	HeroOffspringDlgMakeSonAttrText(uiAttrText,config,pHero)
end

-- 拼儿子属性
function HeroOffspringDlgMakeSonAttrText( uiAttrText, config ,pHero)
	local msg = "";
	if config  then
		if config.attack > 0 then
			msg = msg..F(3406,HeroName(pHero.m_kind),config.attack,T(146));
		end
		if config.defense > 0 then
			msg = msg..F(3406,HeroName(pHero.m_kind),config.defense,T(147));
		end
		if config.attack_increase > 0 then
			msg = msg..F(3406,HeroName(pHero.m_kind),config.attack_increase,T(165));
		end
		if config.defense_increase > 0 then
			msg = msg..F(3406,HeroName(pHero.m_kind),config.defense_increase,T(166));
		end
	end
	SetText( uiAttrText, msg)
end

function HeroOffspringDlgClear()
	local objs = {};
	for i = 0 ,m_uiContent.transform.childCount - 1 do
		table.insert( objs, m_uiContent.transform:GetChild(i).gameObject )
    end
	for k, v in pairs(objs) do
		local obj = v;
		m_ObjectPool:Release( "UIP_SonInfo", obj );
    end
end