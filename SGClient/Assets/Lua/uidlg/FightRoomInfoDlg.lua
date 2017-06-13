-- 界面
local m_Dlg = nil;
local m_uiMatching = nil; --UnityEngine.GameObject
local m_uiAttack = nil; --UnityEngine.GameObject
local m_uiDefense = nil; --UnityEngine.GameObject
local m_uiUIP_ActorInfo = nil; --UnityEngine.GameObject

local m_fightid = 0;
local m_uiObjCache = {};
local m_recvValue = {};

-- 打开界面
function FightRoomInfoDlgOpen()
	m_Dlg = eye.uiManager:Open( "FightRoomInfoDlg" );
end

-- 隐藏界面
function FightRoomInfoDlgClose()
	if m_Dlg == nil then
		return;
	end
	m_fightid = 0;
	m_recvValue = 0;
	m_uiObjCache = {};
	clearChild( m_uiAttack );
	clearChild( m_uiDefense );
	
	eye.uiManager:Close( "FightRoomInfoDlg" );
end

-- 删除界面
function FightRoomInfoDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function FightRoomInfoDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            FightRoomInfoDlgClose();
			system_askinfo( ASKINFO_FIGHTROOMQUIT, "", 0 );
        end
	end
end

-- 载入时调用
function FightRoomInfoDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiMatching = objs[0];
	m_uiAttack = objs[1];
	m_uiDefense = objs[2];
	m_uiUIP_ActorInfo = objs[3];
end

-- 界面初始化时调用
function FightRoomInfoDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function FightRoomInfoDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function FightRoomInfoDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function FightRoomInfoDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
local m_speed = 0;
function FightRoomInfoDlgOnLogic( gameObject )
	if m_speed == 1 then
		m_uiMatching:GetComponent( typeof(UIText) ).text = "匹配对手中";
	elseif m_speed == 10 then
		m_uiMatching:GetComponent( typeof(UIText) ).text = "匹配对手中.";
	elseif m_speed == 30 then
		m_uiMatching:GetComponent( typeof(UIText) ).text = "匹配对手中..";
	elseif m_speed == 40 then
		m_uiMatching:GetComponent( typeof(UIText) ).text = "匹配对手中...";
	elseif m_speed == 50 then
		m_speed = 0;
	end
	m_speed = m_speed + 1;
end


----------------------------------------
-- 自定
----------------------------------------
-- 匹配
function FightRoomInfoDlgMatch()
	FightRoomInfoDlgOpen();
	system_askinfo( ASKINFO_FIGHTMATCH, "", 0 );
end

-- 刷新匹配结果
function FightRoomInfoDlgRefurbish( recvValue )
	m_uiObjCache = {};
	clearChild( m_uiAttack );
	clearChild( m_uiDefense );
	-- 缓存
	m_recvValue = recvValue;
	-- 战斗ID
	m_fightid = recvValue.m_fightid;
	
	-- 攻击方
	for i=1,3,1 do
		if recvValue.m_attack_actorinfo[i].m_actorid > 0 then
			local uiObj = GameObject.Instantiate( m_uiUIP_ActorInfo );
			uiObj.transform:SetParent( m_uiAttack.transform );
			uiObj.transform.localScale = Vector3.one;
			uiObj:SetActive( true );
			table.insert( m_uiObjCache, recvValue.m_attack_actorinfo[i].m_actorid, uiObj );
			
			uiObj.transform:Find("Name"):GetComponent( "UIText" ).text = recvValue.m_attack_actorinfo[i].m_name;
		end
	end
	
	-- 防御方
	for i=1,3,1 do
		if recvValue.m_defense_actorinfo[i].m_actorid > 0 then
			local uiObj = GameObject.Instantiate( m_uiUIP_ActorInfo );
			uiObj.transform:SetParent( m_uiDefense.transform );
			uiObj.transform.localScale = Vector3.one;
			uiObj:SetActive( true );
			table.insert( m_uiObjCache, recvValue.m_defense_actorinfo[i].m_actorid, uiObj );
			
			uiObj.transform:Find("Name"):GetComponent( "UIText" ).text = recvValue.m_defense_actorinfo[i].m_name;
		end
	end
		
end

-- 退出匹配
-- m_fightid=0,m_actorid=0,
function FightRoomInfoDlgQuit( recvValue )
	if m_fightid ~= recvValue.m_fightid then
		return;
	end
	
	-- 清空对象缓存
	local uiobj = m_uiObjCache[recvValue.m_actorid];
	if uiobj == nil then
		return;
	end
	GameObject.Destroy( uiobj );
	m_uiObjCache[recvValue.m_actorid] = nil;
	
	-- 清空数据缓存
	for i=1,3,1 do
		if m_recvValue.m_attack_actorinfo[i].m_actorid == recvValue.m_actorid then
			m_recvValue.m_attack_actorinfo[i].m_actorid = 0;
			break;
		end
		if m_recvValue.m_defense_actorinfo[i].m_actorid == recvValue.m_actorid then
			m_recvValue.m_defense_actorinfo[i].m_actorid = 0;
			break;
		end
	end
end

-- 进入英雄布阵
function FightRoomInfoDlgSetHero( recvValue )
	if m_fightid ~= recvValue.m_fightid then
		return;
	end
	FightRoomSetHeroDlgShow( m_recvValue, recvValue );
	FightRoomInfoDlgClose();
end