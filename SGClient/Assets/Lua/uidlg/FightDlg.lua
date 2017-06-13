-- 界面
local m_Dlg = nil;
local m_uiCountdown = nil; --UnityEngine.GameObject
local m_uiOur = nil; --UnityEngine.GameObject
local m_uiEnemy = nil; --UnityEngine.GameObject
local m_uiHeroList = nil; --UnityEngine.GameObject
local m_uiUIP_FightHero = nil; --UnityEngine.GameObject
local m_uiTop = nil; --UnityEngine.GameObject
local m_uiBottom = nil; --UnityEngine.GameObject

local m_heroObjectCache = {};

local herosprite = {
[1] = "icon_head_baird",
[2] = "icon_head_mcghan",
[3] = "icon_head_catgirl",
[4] = "icon_head_dana",
[5] = "icon_head_doris",
[6] = "icon_head_fries",
[7] = "icon_head_gnoll",
[8] = "icon_head_line",
}

-- 打开界面
function FightDlgOpen()
	m_Dlg = eye.uiManager:Open( "FightDlg" );
end

-- 隐藏界面
function FightDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "FightDlg" );
	
	m_heroObjectCache = {};
end

-- 删除界面
function FightDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function FightDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            FightDlgClose();
			
		-- 选择英雄
		elseif nControlID >= 1 and nControlID <= 10 then
			FightDlgSelectHero( nControlID );
        end
	end
end

-- 载入时调用
function FightDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiCountdown = objs[0];
	m_uiOur = objs[1];
	m_uiEnemy = objs[2];
	m_uiHeroList = objs[3];
	m_uiUIP_FightHero = objs[4];
	m_uiTop = objs[5];
	m_uiBottom = objs[6];
end

-- 界面初始化时调用
function FightDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function FightDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function FightDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function FightDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function FightDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
-- 显示战斗界面
function FightDlgShow()
	-- 英雄按钮
	clearChild( m_uiHeroList );
	m_heroObjectCache = {};
	for id = 1, 7, 1 do
		local uiObj = GameObject.Instantiate( m_uiUIP_FightHero );
		uiObj.transform:SetParent( m_uiHeroList.transform );
		uiObj.transform.localScale = Vector3.one;
		uiObj:SetActive( true );
		uiObj.transform:GetComponent( "UIButton" ).controlID = id;
		uiObj.transform:Find("Head"):GetComponent( "Image" ).sprite = LoadSprite( herosprite[id] );
		table.insert( m_heroObjectCache, uiObj );
	end
	
	-- 控件动画
	m_uiTop:GetComponent( typeof(UITweenRectPosition) ):Play( true );
	m_uiBottom:GetComponent( typeof(UITweenRectPosition) ):Play( true );
end

-- 我方血条
function FightDlgChangeOurLife( life, maxlife )
	if m_Dlg == nil then
		return;
	end
	m_uiOur.transform:Find("HP"):GetComponent( "UIProgress" ):SetValue( life/maxlife );
end

-- 敌方血条
function FightDlgChangeEnemyLife( life, maxlife )
	if m_Dlg == nil then
		return;
	end
	m_uiEnemy.transform:Find("HP"):GetComponent( "UIProgress" ):SetValue( life/maxlife );
end

-- 倒计时
function FightDlgChangeCountdown( countdown )
	if m_Dlg == nil then
		return;
	end
	m_uiCountdown:GetComponent( "UIText" ).text = getTimeStringBySecond( countdown );
end

-- 选择英雄
function FightDlgSelectHero( offset )
	local sendValue = {};
	sendValue.m_fightid = GetFightRoom().m_fightid;
	sendValue.m_cmd = 0;
	sendValue.m_value = offset;
	if GetFightRoom().m_fightid < 0 then
		-- 本地
		GetFightRoom():AddUnit( {m_side = 1, m_kind = offset, m_offsetx = 0} )
	else
		-- 网络
		netsend_fightcommand_C( sendValue );
	end
end
