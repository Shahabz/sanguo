-- 界面
local m_Dlg = nil;
local m_uiTitle = nil; --UnityEngine.GameObject
local m_uiTimeNum = nil; --UnityEngine.GameObject
local m_uiAward1 = nil; --UnityEngine.GameObject
local m_uiAward2 = nil; --UnityEngine.GameObject
local m_uiAward3 = nil; --UnityEngine.GameObject
local m_uiAward4 = nil; --UnityEngine.GameObject
local m_uiAward5 = nil; --UnityEngine.GameObject
local m_uiAward6 = nil; --UnityEngine.GameObject
local m_uiExitButton = nil; --UnityEngine.GameObject
local m_uiCongratulateButton = nil; --UnityEngine.GameObject

-- 打开界面
function TreasureAwardDlgOpen()
	ResourceManager.LoadAssetBundle( "_ab_ui_static_pic_3" )
	ResourceManager.LoadAssetBundle( "_ab_ui_static_pic_4" )
	m_Dlg = eye.uiManager:Open( "TreasureAwardDlg" );
end

-- 隐藏界面
function TreasureAwardDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "TreasureAwardDlg" );
end

-- 删除界面
function TreasureAwardDlgDestroy()
	GameObject.Destroy( m_Dlg );
	ResourceManager.UnloadAssetBundle( "_ab_ui_static_pic_3" )
	ResourceManager.UnloadAssetBundle( "_ab_ui_static_pic_4" )
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function TreasureAwardDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            TreasureAwardDlgClose();
		elseif nControlID == 1 then
			print"点击恭贺"
		
        end
	end
end

-- 载入时调用
function TreasureAwardDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiTitle = objs[0];
	m_uiTimeNum = objs[1];
	m_uiAward1 = objs[2];
	m_uiAward2 = objs[3];
	m_uiAward3 = objs[4];
	m_uiAward4 = objs[5];
	m_uiAward5 = objs[6];
	m_uiAward6 = objs[7];
	m_uiExitButton = objs[8];
	m_uiCongratulateButton = objs[9];
end

-- 界面初始化时调用
function TreasureAwardDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function TreasureAwardDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function TreasureAwardDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function TreasureAwardDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function TreasureAwardDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function TreasureAwardDlgShow()
	TreasureAwardDlgOpen()
	local playernation = GetPlayer().m_nation
	print("m_nation"..playernation)
	SetText(m_uiTitle,F(1682,T(110+playernation)))
	--倒计时
	local lasttime = "12:12:12"
	SetText(m_uiTimeNum,lasttime)

	--获得图纸的玩家名称
	local name={"吴晖","廖一凡","郑佳","张自若","许文浴","司徒星空","神","","妹子五个字","","六个名字玩家",""}
	SetText(m_uiAward1.transform:Find("Name1"),name[1])
	SetText(m_uiAward1.transform:Find("Name2"),name[2])
	SetText(m_uiAward2.transform:Find("Name1"),name[3])
	SetText(m_uiAward2.transform:Find("Name2"),name[4])
	SetText(m_uiAward3.transform:Find("Name1"),name[5])
	SetText(m_uiAward3.transform:Find("Name2"),name[6])
	SetText(m_uiAward4.transform:Find("Name1"),name[7])
	SetText(m_uiAward4.transform:Find("Name2"),name[8])
	SetText(m_uiAward5.transform:Find("Name1"),name[9])
	SetText(m_uiAward5.transform:Find("Name2"),name[10])
	SetText(m_uiAward6.transform:Find("Name1"),name[11])
	SetText(m_uiAward6.transform:Find("Name2"),name[12])
	--	图纸数量
	local equipnum={2,2,2,1,1,1}
	SetText(m_uiAward1.transform:Find("Num"),"x"..equipnum[1])
	SetText(m_uiAward2.transform:Find("Num"),"x"..equipnum[2])
	SetText(m_uiAward3.transform:Find("Num"),"x"..equipnum[3])
	SetText(m_uiAward4.transform:Find("Num"),"x"..equipnum[4])
	SetText(m_uiAward5.transform:Find("Num"),"x"..equipnum[5])
	SetText(m_uiAward6.transform:Find("Num"),"x"..equipnum[6])

end
