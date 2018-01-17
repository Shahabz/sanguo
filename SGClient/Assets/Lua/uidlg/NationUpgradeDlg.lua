-- 界面
local m_Dlg = nil;

local m_uiNationFlag = nil; --UnityEngine.GameObject
local m_uiLevelDesc = nil; --UnityEngine.GameObject
local m_uiLevel = nil; --UnityEngine.GameObject
local m_uiExpProgress = nil; --UnityEngine.GameObject
local m_uiCost = nil; --UnityEngine.GameObject
local m_uiAward = nil; --UnityEngine.GameObject
local m_uiUpgradeBtn = nil; --UnityEngine.GameObject
local m_uiWarn = nil; --UnityEngine.GameObject

local m_recvValue = nil

-- 打开界面
function NationUpgradeDlgOpen()
	m_Dlg = eye.uiManager:Open( "NationUpgradeDlg" );
end

-- 隐藏界面
function NationUpgradeDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "NationUpgradeDlg" );
	m_recvValue = nil
end

-- 删除界面
function NationUpgradeDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
	m_recvValue = nil
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function NationUpgradeDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            NationUpgradeDlgClose();
		elseif nControlID == 1 then
			NationUpgradeDlgBuild()
        end
	end
end

-- 载入时调用
function NationUpgradeDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiNationFlag = objs[0];
	m_uiLevelDesc = objs[1];
	m_uiLevel = objs[2];
	m_uiExpProgress = objs[3];
	m_uiCost = objs[4];
	m_uiAward = objs[5];
	m_uiUpgradeBtn = objs[6];
	m_uiWarn = objs[7];
end

-- 界面初始化时调用
function NationUpgradeDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function NationUpgradeDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function NationUpgradeDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function NationUpgradeDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function NationUpgradeDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function NationUpgradeDlgShow( recvValue )
	NationUpgradeDlgOpen()
	NationUpgradeDlgShowRecv( recvValue )
end

-- m_level=0,m_exp=0,m_donate_num=0,m_myrank=0
function NationUpgradeDlgShowRecv( recvValue )
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	m_recvValue = recvValue
	
	-- 满级了就关界面
	if m_recvValue.m_level >= #g_nation_upgrade then
		NationUpgradeDlgClose()
		return
	end
	
	SetImage( m_uiNationFlag, NationFlagSprite( GetPlayer().m_nation ) )

	SetText( m_uiLevelDesc.transform:Find("Text"), F(1761,m_recvValue.m_level+1,m_recvValue.m_level+1) );
	SetText( m_uiLevel, F(1762, m_recvValue.m_level, m_recvValue.m_exp, g_nation_upgrade[m_recvValue.m_level][1].maxexp) );
	SetProgress( m_uiExpProgress, m_recvValue.m_exp/g_nation_upgrade[m_recvValue.m_level][1].maxexp )
	-- 今日次数已经用尽
	if m_recvValue.m_donate_num >= 11 then
		SetFalse( m_uiUpgradeBtn )
		SetFalse( m_uiCost )
		SetFalse( m_uiAward )
		SetTrue( m_uiWarn )
		return
	end
	
	SetTrue( m_uiUpgradeBtn )
	SetTrue( m_uiCost )
	SetTrue( m_uiAward )
	SetFalse( m_uiWarn )
	
	-- 消耗
	NationUpgradeDlgSetCost()
	
	-- 获得
	local donate_num = m_recvValue.m_donate_num;
	SetText( m_uiAward, T(1765)..":"
						..T(1537).."x"..g_nation_upgrade[m_recvValue.m_level][donate_num].exp.." "
						..T(1766).."x"..g_nation_upgrade[m_recvValue.m_level][donate_num].prestige )
	
end

-- 消耗
function NationUpgradeDlgSetCost()
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	if m_recvValue.m_donate_num == 0 then
		SetRichText( m_uiCost, T(1764)..":<color=03DE27FF>"..T(780).."</color>" )
		return
	end
	-- 消耗
	local silver = g_nation_upgrade[m_recvValue.m_level][m_recvValue.m_donate_num].silver
	local wood = g_nation_upgrade[m_recvValue.m_level][m_recvValue.m_donate_num].wood
	local food = g_nation_upgrade[m_recvValue.m_level][m_recvValue.m_donate_num].food
	local str = "";
	if silver > 0 then
		if GetPlayer().m_silver >= silver  then
			str = str .. "<icon=silver><color=03DE27FF>"..knum(silver).."</color><color=03DE27FF>/"..knum(GetPlayer().m_silver).."</color>"
		else
			str = str .. "<icon=silver><color=03DE27FF>"..knum(silver).."</color><color=E80017FF>/"..knum(GetPlayer().m_silver).."</color>"
		end
	end
	if wood > 0 then
		if GetPlayer().m_wood >= wood  then
			str = str .. "<icon=wood><color=03DE27FF>"..knum(wood).."</color><color=03DE27FF>/"..knum(GetPlayer().m_wood).."</color>"
		else
			str = str .. "<icon=wood><color=03DE27FF>"..knum(wood).."</color><color=E80017FF>/"..knum(GetPlayer().m_wood).."</color>"
		end
	end
	if food > 0 then
		if GetPlayer().m_food >= food  then
			str = str .. "<icon=food><color=03DE27FF>"..knum(food).."</color><color=03DE27FF>/"..knum(GetPlayer().m_food).."</color>"
		else
			str = str .. "<icon=food><color=03DE27FF>"..knum(food).."</color><color=E80017FF>/"..knum(GetPlayer().m_food).."</color>"
		end
	end
	SetRichText( m_uiCost, T(1764)..":"..str )
end

-- 建设
function NationUpgradeDlgBuild()
	local silver = g_nation_upgrade[m_recvValue.m_level][m_recvValue.m_donate_num].silver
	local wood = g_nation_upgrade[m_recvValue.m_level][m_recvValue.m_donate_num].wood
	local food = g_nation_upgrade[m_recvValue.m_level][m_recvValue.m_donate_num].food
	if GetPlayer().m_silver < silver then
		JumpRes(1)
		return
	end
	if GetPlayer().m_wood < wood then
		JumpRes(2)
		return
	end
	if GetPlayer().m_food < food then
		JumpRes(3)
		return
	end
	system_askinfo( ASKINFO_NATION, "", 1 )
end