-- 界面
local m_Dlg = nil;
local m_uiLevelText = nil; --UnityEngine.GameObject
local m_uiNobilityText = nil; --UnityEngine.GameObject
local m_uiNobilityIcon = nil; --UnityEngine.GameObject
local m_uiExpText = nil; --UnityEngine.GameObject
local m_uiRankText = nil; --UnityEngine.GameObject
local m_uiBuildingText = nil; --UnityEngine.GameObject
local m_uiPrestigeText = nil; --UnityEngine.GameObject
local m_uiOfficialerText = nil; --UnityEngine.GameObject
local m_uiFlag = nil; --UnityEngine.GameObject
local NationInfo = { }
-- 打开界面
function NationDlgOpen()
	m_Dlg = eye.uiManager:Open( "NationDlg" );
end

-- 隐藏界面
function NationDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "NationDlg" );
end

-- 删除界面
function NationDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function NationDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            NationDlgClose();
		--国家建设
		elseif nControlID == 1 then
			
		--爵位升级
		elseif nControlID == 2 then
		
		--修改公告
		elseif nControlID == 3 then
		
		--国家任务
		elseif nControlID == 4 then
		
		--国家城池
		elseif nControlID == 5 then
		
		--国家战争
		elseif nControlID == 6 then
		
		--国家名将
		elseif nControlID == 7 then
		
		--国家荣誉
		elseif nControlID == 8 then
		
		--国家官员
		elseif nControlID == 9 then
		
		--国家战争
		elseif nControlID == 10 then
		
        end
	end
end

-- 载入时调用
function NationDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiLevelText = objs[0];
	m_uiNobilityText = objs[1];
	m_uiNobilityIcon = objs[2];
	m_uiExpText = objs[3];
	m_uiRankText = objs[4];
	m_uiBuildingText = objs[5];
	m_uiPrestigeText = objs[6];
	m_uiOfficialerText = objs[7];
	m_uiFlag = objs[8];
end

-- 界面初始化时调用
function NationDlgOnStart( gameObject )
	NationDlgFillInfo(NationInfo);
end

-- 界面显示时调用
function NationDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function NationDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function NationDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function NationDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function NationDlgShow()
	NationDlgOpen()
	NationInfo.nation = 2
	NationInfo.level = 78 
	NationInfo.newexp = 7520000
	NationInfo.sumexp = 8552155555
	NationInfo.rank = 5
	NationInfo.newbuillding = 5
	NationInfo.sumbuillding = 11
	NationInfo.prestige  = 100000000
	NationInfo.officer = "丞相"
end

function NationDlgFillInfo(nationInfo)
	SetText(m_uiLevelText,nationInfo.level);
	local nationExp = string.format("%s/%s",knum(nationInfo.newexp),knum(nationInfo.sumexp));
	SetText(m_uiExpText,nationExp);
	SetText(m_uiRankText,nationInfo.rank);
	local nationBuild = string.format("已建设（%d/%d）",nationInfo.newbuillding,nationInfo.sumbuillding);
	SetText(m_uiBuildingText,nationBuild);
	SetText(m_uiPrestigeText,knum(nationInfo.prestige));
	SetText(m_uiOfficialerText,nationInfo.officer);
	print("nationInfo.nation:"..nationInfo.nation);
	SetImage(m_uiFlag,NationSpriteFlag(nationInfo.nation));
end








