-- 界面
local m_Dlg = nil;
local m_DialogFrameMod = nil;
local m_uiHeadLayer = nil; --UnityEngine.GameObject
local m_uiGirlPageL = nil; --UnityEngine.GameObject
local m_uiGirlPageR = nil; --UnityEngine.GameObject
local m_uiGirlScroll = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiUIP_Girl = nil; --UnityEngine.GameObject
local m_uiSingleLayer = nil; --UnityEngine.GameObject
local m_uiGirlName = nil; --UnityEngine.GameObject
local m_uiGirlShape = nil; --UnityEngine.GameObject
local m_uiAttrList = nil; --UnityEngine.GameObject
local m_uiPrivateAttrList = nil; --UnityEngine.GameObject
local m_uiAllotBtn = nil; --UnityEngine.GameObject
local m_uiDoubleLayer = nil; --UnityEngine.GameObject
local m_uiHero = nil; --UnityEngine.GameObject
local m_uiGirl = nil; --UnityEngine.GameObject
local m_uiColorUpBtn = nil; --UnityEngine.GameObject
local m_uiLoveLayer = nil; --UnityEngine.GameObject
local m_uiLoveStar = nil; --UnityEngine.GameObject
local m_uiLoveSpeed = nil; --UnityEngine.GameObject
local m_uiLoveProgress = nil; --UnityEngine.GameObject
local m_uiMarryBtn = nil; --UnityEngine.GameObject
local m_uiMakeLoveBtn = nil; --UnityEngine.GameObject
local m_uiNextBtn = nil; --UnityEngine.GameObject
local m_uiSonLayer = nil; --UnityEngine.GameObject
local m_uiSonHead = nil; --UnityEngine.GameObject
local m_uiSonName = nil; --UnityEngine.GameObject
local m_uiSonAttr = nil; --UnityEngine.GameObject
local m_uiGrowUpBtn = nil; --UnityEngine.GameObject
local m_uiAgeProgress = nil; --UnityEngine.GameObject
local m_ObjectPool = nil;

-- 打开界面
function GirlDlgOpen()
	ResourceManager.LoadAssetBundle( "_ab_girlhead" );
	ResourceManager.LoadAssetBundle( "_ab_girlface" );
	m_Dlg = eye.uiManager:Open( "GirlDlg" );
	m_DialogFrameMod = DialogFrameModOpen( m_Dlg, T(565), HELP_GirlDlg, GirlDlgClose );
end

-- 隐藏界面
function GirlDlgClose()
	if m_Dlg == nil then
		return;
	end
	DialogFrameModClose( m_DialogFrameMod );
	m_DialogFrameMod = nil;
	eye.uiManager:Close( "GirlDlg" );
end

-- 删除界面
function GirlDlgDestroy()
	GameObject.Destroy( m_Dlg );
	ResourceManager.UnloadAssetBundle( "_ab_girlhead" )
	ResourceManager.UnloadAssetBundle( "_ab_girlface" )
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function GirlDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            GirlDlgClose();
        end
	end
end

-- 载入时调用
function GirlDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiHeadLayer = objs[0];
	m_uiGirlPageL = objs[1];
	m_uiGirlPageR = objs[2];
	m_uiGirlScroll = objs[3];
	m_uiContent = objs[4];
	m_uiUIP_Girl = objs[5];
	m_uiSingleLayer = objs[6];
	m_uiGirlName = objs[7];
	m_uiGirlShape = objs[8];
	m_uiAttrList = objs[9];
	m_uiPrivateAttrList = objs[10];
	m_uiAllotBtn = objs[11];
	m_uiDoubleLayer = objs[12];
	m_uiHero = objs[13];
	m_uiGirl = objs[14];
	m_uiColorUpBtn = objs[15];
	m_uiLoveLayer = objs[16];
	m_uiLoveStar = objs[17];
	m_uiLoveSpeed = objs[18];
	m_uiLoveProgress = objs[19];
	m_uiMarryBtn = objs[20];
	m_uiMakeLoveBtn = objs[21];
	m_uiNextBtn = objs[22];
	m_uiSonLayer = objs[23];
	m_uiSonHead = objs[24];
	m_uiSonName = objs[25];
	m_uiSonAttr = objs[26];
	m_uiGrowUpBtn = objs[27];
	m_uiAgeProgress = objs[28];
	-- 对象池
	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );
	m_ObjectPool:CreatePool("UIP_Girl", 64, 64, m_uiUIP_Girl);
end

-- 界面初始化时调用
function GirlDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function GirlDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function GirlDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function GirlDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function GirlDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function GirlDlgShow()
	GirlDlgOpen()
	
end
