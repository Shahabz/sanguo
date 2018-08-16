-- 界面
local m_Dlg = nil;
local m_uiScrollView = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiUIP_Town = nil; --UnityEngine.GameObject

local m_ObjectPool = nil
local m_recvValue = nil

-- 打开界面
function NationTownDlgOpen()
	m_Dlg = eye.uiManager:Open( "NationTownDlg" );
end

-- 隐藏界面
function NationTownDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "NationTownDlg" );
	m_recvValue = nil
end

-- 删除界面
function NationTownDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
	m_recvValue = nil
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function NationTownDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            NationTownDlgClose();
		
		-- 修复
		elseif nControlID >= 1000 and nControlID < 2000 then
			 NationTownDlgRepair( nControlID-1000 )
			
		-- 重建
		elseif nControlID >= 2000 and nControlID < 3000 then
			 NationTownDlgRebuild( nControlID-2000 )
			
		-- 前往
		elseif nControlID >= 3000 and nControlID < 4000 then
			 NationTownDlgGoto( nControlID-3000 )
        end
	end
end

-- 载入时调用
function NationTownDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiScrollView = objs[0];
	m_uiContent = objs[1];
	m_uiUIP_Town = objs[2];
	-- 对象池
	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );
	m_ObjectPool:CreatePool("UIP_Town", 5, 5, m_uiUIP_Town);
end

-- 界面初始化时调用
function NationTownDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function NationTownDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function NationTownDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function NationTownDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function NationTownDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function NationTownDlgShow()
	NationTownDlgOpen()
	NationTownDlgClear()
	system_askinfo( ASKINFO_NATION, "", 3 )
end

-- 开始接收
function NationTownDlgBegin()
	m_recvValue={}
end

-- m_count=0,m_list={m_info={m_protect_sec=0,m_produce_sec=0,m_own_actorid=0,m_own_namelen=0,m_own_name="[m_own_namelen]",m_own_sec=0,m_hp=0,m_maxhp=0,m_myask=0,m_produce_num=0,m_townid=0,},[m_count]},m_op=0,
function NationTownDlgRecv( recvValue )
	for i=1, recvValue.m_count, 1 do
		table.insert( m_recvValue, recvValue.m_list[i].m_info );
	end
end

-- 接收完毕
function NationTownDlgEndRecv()
	for i=1, #m_recvValue, 1 do
		local uiObj = m_ObjectPool:Get("UIP_Town");
		uiObj.transform:SetParent( m_uiContent.transform );
		uiObj.transform.localScale = Vector3.one;
		m_recvValue[i].m_uiObj = uiObj;
		m_recvValue[i].m_index = i;
		NationTownDlgSetObj( i, uiObj, m_recvValue[i] );
	end
end

-- 更新
function NationTownDlgUpdate( recvValue )
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	for i=1, #m_recvValue, 1 do
		if m_recvValue[i].m_townid == recvValue.m_townid then
			m_recvValue[i].m_own_actorid = recvValue.m_own_actorid;
			m_recvValue[i].m_own_namelen = recvValue.m_own_namelen;
			m_recvValue[i].m_own_name = recvValue.m_own_name;
			m_recvValue[i].m_hp = recvValue.m_hp;
			m_recvValue[i].m_maxhp = recvValue.m_maxhp;
			m_recvValue[i].m_myask = recvValue.m_myask;
			NationTownDlgSetObj( i, m_recvValue[i].m_uiObj, m_recvValue[i] );
			break;
		end
	end
end

--清空
function NationTownDlgClear()
	local objs = {};
	for i=0,m_uiContent.transform.childCount-1 do
		table.insert(objs,m_uiContent.transform:GetChild(i).gameObject);
	end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "UIP_Town(Clone)" then
			m_ObjectPool:Release( "UIP_Town", obj );
		end
	end
end

--设置每一行
function NationTownDlgSetObj( index, uiObj, info )
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiShape = objs[0];
	local uiName = objs[1]
	local uiOwn = objs[2];
	local uiHp = objs[3];
	local uiGoto = objs[4];
	local uiRebuildBtn = objs[5];
	local uiRepairBtn = objs[6];
	SetControlID( uiGoto, 3000 + index );
	
	local type 			= g_towninfo[info.m_townid].type
	local level 		= g_towninfo[info.m_townid].level
	local posx 			= g_towninfo[info.m_townid].posx
	local posy 			= g_towninfo[info.m_townid].posy
	
	-- 形象
	SetImage( uiShape, LoadSprite( MapUnitTownShapeList[type].."_"..GetPlayer().m_nation ) )
	-- 名称
	SetText( uiName, F(1780, level, MapTownName(info.m_townid), posx, posy) )
	-- 所属
	if info.m_own_namelen > 0 then
		SetText( uiOwn, T(1211)..":"..info.m_own_name )
	else
		SetText( uiOwn, T(1211)..":"..NationEx( GetPlayer().m_nation ) )
	end
	-- 兵力
	SetText( uiHp, F(1281, info.m_hp, info.m_maxhp) )
	
	if info.m_own_actorid == GetPlayer().m_actorid then
		SetFalse( uiRepairBtn )
		SetFalse( uiRebuildBtn )
		if info.m_hp < info.m_maxhp then
			SetTrue( uiRepairBtn )
			SetControlID( uiRepairBtn, 1000 + index );
		end
	else
		if info.m_own_actorid == 0 then
			SetTrue( uiRebuildBtn )
			SetFalse( uiRepairBtn )
			SetControlID( uiRebuildBtn, 2000 + index );
		else
			SetFalse( uiRebuildBtn )
			SetFalse( uiRepairBtn )
		end
	end
end

-- 修复
function NationTownDlgRepair( index )
	local info = m_recvValue[index]
	if info == nil then
		return
	end
	
	local repairhp = math.min( info.m_maxhp-info.m_hp, math.floor(info.m_maxhp/20) )
	local cost_silver = math.ceil( repairhp/info.m_maxhp * g_towninfo[info.m_townid].ask_silver );
	local cost_wood = math.ceil( repairhp/info.m_maxhp * g_towninfo[info.m_townid].ask_wood );
	MsgBox( F(1316, MapTownName(info.m_townid), T(121)..knum(cost_silver).." "..T(122)..knum(cost_wood) ), function() 
		
		if GetPlayer().m_silver < cost_silver then
			JumpRes(1,cost_silver-GetPlayer().m_silver)
			return
		end

		if GetPlayer().m_wood < cost_wood then
			JumpRes(2,cost_wood-GetPlayer().m_wood)
			return
		end
		
		system_askinfo( ASKINFO_NATION, "", 5, info.m_townid )
	end )
end

-- 重建
function NationTownDlgRebuild( index )
	local info = m_recvValue[index]
	if info == nil then
		return
	end
	system_askinfo( ASKINFO_NATION, "", 4, info.m_townid )
end

-- 前往
function NationTownDlgGoto( index )
	local info = m_recvValue[index]
	if info == nil then
		return
	end
	local posx 			= g_towninfo[info.m_townid].posx
	local posy 			= g_towninfo[info.m_townid].posy
	NationTownDlgClose()
	NationDlgClose()
	WorldMap.GotoCoor( posx, posy )
end
