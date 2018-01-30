-- 界面
local m_Dlg = nil;
local m_uiTitle = nil; --UnityEngine.GameObject
local m_uiShape = nil; --UnityEngine.GameObject
local m_uiTalk = nil; --UnityEngine.GameObject

local m_kind = 0;
-- 打开界面
function NationHeroCallDlgOpen()
	m_Dlg = eye.uiManager:Open( "NationHeroCallDlg" );
end

-- 隐藏界面
function NationHeroCallDlgClose()
	if m_Dlg == nil then
		return;
	end
	eye.uiManager:Close( "NationHeroCallDlg" );
end

-- 删除界面
function NationHeroCallDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function NationHeroCallDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            NationHeroCallDlgClose();
		elseif nControlID == 1 then
			 NationHeroCallDlgCall();
        end
	end
end

-- 载入时调用
function NationHeroCallDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiTitle = objs[0];
	m_uiShape = objs[1];
	m_uiTalk = objs[2];
end

-- 界面初始化时调用
function NationHeroCallDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function NationHeroCallDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function NationHeroCallDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function NationHeroCallDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function NationHeroCallDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function NationHeroCallDlgShow( kind )
	NationHeroCallDlgOpen()
	m_kind = kind;
	local name = HeroName(kind)
	SetText( m_uiTitle.transform:Find("Text"), F( 2345, name ) )
	SetImage( m_uiShape, HeroHeadSprite( kind ) )
	
	local cost = ""
	if g_nation_heroinfo[kind].call_silver > 0 then
		cost = cost .. T(121).."x"..g_nation_heroinfo[kind].call_silver;
	end
	if g_nation_heroinfo[kind].call_itemkind0 > 0 then
		cost = cost .. "" ..item_getname(g_nation_heroinfo[kind].call_itemkind0).."x"..g_nation_heroinfo[kind].call_itemnum;
	end
	if g_nation_heroinfo[kind].call_itemkind1 > 0 then
		cost = cost .. "," ..item_getname(g_nation_heroinfo[kind].call_itemkind1).."x"..g_nation_heroinfo[kind].call_itemnum;
	end
	if g_nation_heroinfo[kind].call_itemkind2 > 0 then
		cost = cost .. "," ..item_getname(g_nation_heroinfo[kind].call_itemkind2).."x"..g_nation_heroinfo[kind].call_itemnum;
	end
	if g_nation_heroinfo[kind].call_itemkind3 > 0 then
		cost = cost .. "," ..item_getname(g_nation_heroinfo[kind].call_itemkind3).."x"..g_nation_heroinfo[kind].call_itemnum;
	end
			
	SetText( m_uiTalk.transform:Find("Text"), F( 2353, name, cost ) )
end

function NationHeroCallDlgCall()
	local kind = m_kind;
	if GetPlayer().m_silver < g_nation_heroinfo[kind].call_silver then
		JumpRes(1)
		return
	end
	
	-- 资源
	local costkind = { g_nation_heroinfo[kind].call_itemkind0, g_nation_heroinfo[kind].call_itemkind1, g_nation_heroinfo[kind].call_itemkind2, g_nation_heroinfo[kind].call_itemkind3, }
	for i=1, 4, 1 do
		if costkind[i] > 0 then
			local itemnum = GetItem():GetCount( costkind[i] )
			if itemnum < g_nation_heroinfo[kind].call_itemnum then
				pop( T(1777)..":"..item_getname(costkind[i]).."x"..g_nation_heroinfo[kind].call_itemnum )
				return
			end
		end
	end
	
	system_askinfo( ASKINFO_NATIONHERO, "", 3, kind )
	NationHeroCallDlgClose()
end