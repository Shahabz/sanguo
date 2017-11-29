-- 界面
local m_Dlg = nil;
local m_uiTypeName = nil; --UnityEngine.GameObject
local m_uiLeftNation = nil; --UnityEngine.GameObject
local m_uiRightNation = nil; --UnityEngine.GameObject
local m_uiLeftName = nil; --UnityEngine.GameObject
local m_uiRightName = nil; --UnityEngine.GameObject
local m_uiLeftHeroList = nil; --UnityEngine.GameObject
local m_uiRightHeroList = nil; --UnityEngine.GameObject
local m_uiLeftHp = nil; --UnityEngine.GameObject
local m_uiRightHp = nil; --UnityEngine.GameObject
local m_uiSkipDesc = nil; --UnityEngine.GameObject
local m_uiSkipBtn = nil; --UnityEngine.GameObject
local m_uiLeftPk = nil; --UnityEngine.GameObject
local m_uiRightPk = nil; --UnityEngine.GameObject
local m_uiPkIcon = nil; --UnityEngine.GameObject

local m_recvValue = nil;
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
        end
	end
end

-- 载入时调用
function FightDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiTypeName = objs[0];
	m_uiLeftNation = objs[1];
	m_uiRightNation = objs[2];
	m_uiLeftName = objs[3];
	m_uiRightName = objs[4];
	m_uiLeftHeroList = objs[5];
	m_uiRightHeroList = objs[6];
	m_uiLeftHp = objs[7];
	m_uiRightHp = objs[8];
	m_uiSkipDesc = objs[9];
	m_uiSkipBtn = objs[10];
	m_uiLeftPk = objs[11];
	m_uiRightPk = objs[12];
	m_uiPkIcon = objs[13];
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
function FightDlgShow( recvValue )
	FightDlgOpen()
	m_recvValue = recvValue;
	local info = json.decode( m_recvValue.m_fight_content );
		
--[[	-- 我是攻击方，攻击方显示左面
	local my = m_recvValue.m_content_json["my"];
	if my == nil or my == 1 then	
		-- 攻击方
		SetText( m_uiMailFight.transform:Find("AttackBack/Text"), T(1174) );
		-- 防御方
		SetText( m_uiMailFight.transform:Find("DefenseBack/Text"), T(1175) );
		
		MailInfoDlgSetFightMainUnit( m_uiMailFight.transform:Find("AttackMain"), info["a_type"], info["a_name"], info["a_shape"], info["a_nation"], info["a_maxhp"], info["a_hp"] );
		MailInfoDlgSetFightMainUnit( m_uiMailFight.transform:Find("DefenseMain"), info["d_type"], info["d_name"], info["d_shape"], info["d_nation"], info["d_maxhp"], info["d_hp"] );
	
	-- 我是防御方，防御方显示左面
	else
		-- 防御方
		SetText( m_uiMailFight.transform:Find("AttackBack/Text"), T(1175) );
		-- 攻击方
		SetText( m_uiMailFight.transform:Find("DefenseBack/Text"), T(1174) );
		
		MailInfoDlgSetFightMainUnit( m_uiMailFight.transform:Find("DefenseMain"), info["a_type"], info["a_name"], info["a_shape"], info["a_nation"], info["a_maxhp"], info["a_hp"] );
		MailInfoDlgSetFightMainUnit( m_uiMailFight.transform:Find("AttackMain"), info["d_type"], info["d_name"], info["d_shape"], info["d_nation"], info["d_maxhp"], info["d_hp"] );
	end
		
		
		-- 战斗单元取一个最多的
		local unitcount = #info["a_unit"];
		if #info["d_unit"] > unitcount then
			unitcount = #info["d_unit"];
		end
		-- 创建战斗单元
		for i=1, unitcount, 1 do
			local unit = nil;
			local uiObj = m_ObjectPool:Get( "UIP_UnitRow" );
			uiObj.transform:SetParent( m_uiMailFight.transform );

			local uiAttack = uiObj.transform:Find("Attack");
			-- 我是攻击方，攻击方显示左面
			if my == nil or my == 1 then
				unit = info["a_unit"][i];
			else
				unit = info["d_unit"][i];
			end
			if unit ~= nil then
				SetTrue( uiAttack )
				MailInfoDlgSetFightUnit( uiAttack, unit["t"], unit["n"], unit["na"], unit["kd"], unit["sp"], unit["lv"], unit["cr"], unit["cs"], unit["mhp"], unit["hp"], unit["dmg"], unit["vw"] )
			else
				SetFalse( uiAttack )
			end
			
			local uiDefense = uiObj.transform:Find("Defense");
			-- 我是防御方，防御方显示左面
			if my == nil or my == 1 then
				unit = info["d_unit"][i];
			else
				unit = info["a_unit"][i];
			end
			if unit ~= nil then
				SetTrue( uiDefense )
				MailInfoDlgSetFightUnit( uiDefense, unit["t"], unit["n"], unit["na"], unit["kd"], unit["sp"], unit["lv"], unit["cr"], unit["cs"], unit["mhp"], unit["hp"], unit["dmg"], unit["vw"] )
			else
				SetFalse( uiDefense )
			end	
		end--]]
		
end
