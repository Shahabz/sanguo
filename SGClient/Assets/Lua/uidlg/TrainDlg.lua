-- 界面
local m_Dlg = nil;
local m_DialogFrameMod = nil;
--------------------------------------------------------------
local m_uiCorps = nil; --UnityEngine.GameObject
local m_uiShape = nil; --UnityEngine.GameObject
local m_uiDesc = nil; --UnityEngine.GameObject
local m_uiSoldiers = nil; --UnityEngine.GameObject
local m_uiSoldiersMax = nil; --UnityEngine.GameObject
local m_uiQueueLevel = nil; --UnityEngine.GameObject
local m_uiUIP_Train = nil; --UnityEngine.GameObject
local m_uiUIP_TrainQueue = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_ObjectPool = nil;
local m_pBuilding = nil;
local m_buildingKind = 0;
local m_corps;
local m_slider = nil;
local m_sliderNum = nil;
local m_sliderSec = nil;
local m_sliderFood = nil;
local m_selectnum = 0;
local m_selectsec = 0;
local m_selectfood = 0;
local SEC = 300;
local m_recvValue = {};

-- 打开界面
function TrainDlgOpen()
	m_Dlg = eye.uiManager:Open( "TrainDlg" );
	m_DialogFrameMod = DialogFrameModOpen( m_Dlg, T(m_buildingKind).." Lv."..m_pBuilding.m_level, 8, TrainDlgClose );
end

-- 隐藏界面
function TrainDlgClose()
	if m_Dlg == nil then
		return;
	end
	local objs = {};
	for i = 0 ,m_uiContent.transform.childCount - 1 do
		table.insert( objs, m_uiContent.transform:GetChild(i).gameObject )
    end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "UIP_Train(Clone)" then
			obj.transform:Find("Content/Timer"):GetComponent( typeof(UITextTimeCountdown) ):Stop();
			m_ObjectPool:Release( "UIP_Train", obj );
		elseif obj.name == "UIP_TrainQueue(Clone)" then
			m_ObjectPool:Release( "UIP_TrainQueue", obj );
		end
    end
	DialogFrameModClose( m_DialogFrameMod );
	eye.uiManager:Close( "TrainDlg" );
end

-- 删除界面
function TrainDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function TrainDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            TrainDlgClose();
		
		-- 招募
		elseif nControlID == 1 then
			TrainDlgTrain()
		
		-- 加速
		elseif nControlID == 2 then
			TrainDlgQuick()
				
		-- 扩建
		elseif nControlID == 3 then	
			TrainDlgQueue()
		
		-- 强征
		elseif nControlID == 11 then	
		
		-- 募兵加时
		elseif nControlID == 12 then
		
		-- 取消
		elseif nControlID > 1000 and nControlID < 1100 then
			TrainDlgCancel( nControlID - 1000 )
        end
	elseif nType == UI_EVENT_TIMECOUNTEND then
		if nControlID == 1 then
			TrainDlgClose();
		end
	elseif nType == UI_EVENT_SLIDERVALUECHANGE then
		if nControlID == 0 then
			TrainDlgSetSlider( m_slider, value );
			local space = m_recvValue.m_soldiers_max - m_recvValue.m_soldiers;
			if m_selectnum > space then
				pop( T(630) )
			end
		end
	end
end

-- 载入时调用
function TrainDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiCorps = objs[0];
	m_uiShape = objs[1];
	m_uiDesc = objs[2];
	m_uiSoldiers = objs[3];
	m_uiSoldiersMax = objs[4];
	m_uiQueueLevel = objs[5];
	m_uiUIP_Train = objs[6];
	m_uiUIP_TrainQueue = objs[7];
	m_uiContent = objs[8];
	
	-- 对象池
	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );
	m_ObjectPool:CreatePool("UIP_Train", 1, 1, m_uiUIP_Train);
	m_ObjectPool:CreatePool("UIP_TrainQueue", 1, 1, m_uiUIP_TrainQueue);
end

-- 界面初始化时调用
function TrainDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function TrainDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function TrainDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function TrainDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function TrainDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function TrainDlgOnShow( buildingKind )
	m_buildingKind = buildingKind;
	m_pBuilding = GetPlayer():GetBuilding( buildingKind );
	TrainDlgOpen();
	
	if m_buildingKind == BUILDING_Infantry or m_buildingKind == BUILDING_Militiaman_Infantry then
		m_corps = 0
	elseif m_buildingKind == BUILDING_Cavalry or m_buildingKind == BUILDING_Militiaman_Cavalry then
		m_corps = 1
	elseif m_buildingKind == BUILDING_Archer or m_buildingKind == BUILDING_Militiaman_Archer then
		m_corps = 2
	end
	SetImage( m_uiCorps, CorpsSprite( m_corps ) );
	SetImage( m_uiShape, CorpsSprite( m_corps ) );
	
	-- 请求信息
	system_askinfo( ASKINFO_TRAIN, "", 0 );
	
	--[[if m_corps == 0 then
		TrainDlgRecv( {m_soldiers=1800,m_soldiers_max=2000,m_trainnum=250,m_trainsec=10,m_trainsec_need=300,m_queuenum={250,0,0,0,0,0,0,0},m_queue=1,m_trainlong=0,m_train_confnum=250,m_train_confsec=600,m_train_conffood=30} )
	elseif m_corps == 1 then
		TrainDlgRecv( {m_soldiers=2800,m_soldiers_max=2000,m_trainnum=0,m_trainsec=0,m_trainsec_need=0,m_queuenum={0,0,0,0,0,0,0,0},m_queue=2,m_trainlong=0,m_train_confnum=250,m_train_confsec=600,m_train_conffood=30} )
	elseif m_corps == 2 then
		TrainDlgRecv( {m_soldiers=4000,m_soldiers_max=6000,m_trainnum=250,m_trainsec=10,m_trainsec_need=300,m_queuenum={0,0,0,0,0,0,0,0},m_queue=6,m_trainlong=0,m_train_confnum=250,m_train_confsec=1200,m_train_conffood=30} )
	end--]]
end

-- 信息返回
-- m_soldiers=0,m_soldiers_max=0,m_trainnum=0,m_trainsec=0,m_trainsec_need=0,m_queuenum={[16]},m_queue=0,m_trainlong=0,m_train_confnum=0,m_train_confsec=0,m_train_conffood=0
function TrainDlgRecv( recvValue )
	m_recvValue = recvValue;
	SetText( m_uiDesc, T(m_corps+619) )
	SetText( m_uiSoldiers, T(622)..":"..recvValue.m_soldiers )
	SetText( m_uiSoldiersMax, T(623)..":"..recvValue.m_soldiers_max )
	SetRichText( m_uiQueueLevel, T(624)..":"..recvValue.m_queue )
	
	-- 正在训练的
	if recvValue.m_trainnum > 0 then
		TrainDlgSetObject( recvValue, 1, 0 );
	else
		TrainDlgSetObject( recvValue, 2, 0 );
	end
	
	-- 队列
	local hasselect = false;
	if recvValue.m_queue > 0 then
		for i=1, recvValue.m_queue, 1 do
			if recvValue.m_queuenum[i] > 0 then -- 等待中
				TrainDlgSetObject( recvValue, 4, i )
			elseif recvValue.m_trainnum > 0 and hasselect == false then
				TrainDlgSetObject( recvValue, 2, i )
				hasselect = true;
			else
				TrainDlgSetObject( recvValue, 3, i )
			end
		end
	end
	
	-- 扩建
	if recvValue.m_queue < 8 then
		local uiObj = m_ObjectPool:Get( "UIP_TrainQueue" );
		uiObj.transform:SetParent( m_uiContent.transform );
		SetText( uiObj.transform:Find("Content/Num"), F(627,3000) );
	end
end

function TrainDlgSetObject( recvValue, type, index )
	local uiObj = m_ObjectPool:Get( "UIP_Train" );
	uiObj.transform:SetParent( m_uiContent.transform );
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiNum = objs[0];
	local uiSec = objs[1];
	local uiFood = objs[2];
	local uiTimerProgress = objs[3];
	local uiTimer = objs[4];
	local uiSlider = objs[5];
	local uiQuickBtn = objs[6];
	local uiTrainBtn = objs[7];
	local uiCancelBtn = objs[8];
	local uiIdle = objs[9];
	SetTimer( uiTimer, 0, 0, 0 )
	
	if type == 1 then -- 正在招募中
		SetTrue( uiNum )
		SetTrue( uiTimerProgress )
		SetTrue( uiTimer )
		SetTrue( uiQuickBtn )
		SetFalse( uiSec )
		SetFalse( uiFood )
		SetFalse( uiSlider )
		SetFalse( uiTrainBtn )
		SetFalse( uiCancelBtn )
		SetFalse( uiIdle )	
		SetText( uiNum, T(625)..":"..recvValue.m_trainnum )
		SetTimer( uiTimer, recvValue.m_trainsec, recvValue.m_trainsec_need, 1 )
	elseif type == 2 then -- 选择招募
		SetTrue( uiNum )
		SetFalse( uiTimerProgress )
		SetFalse( uiTimer )
		SetFalse( uiQuickBtn )
		SetTrue( uiSec )
		SetTrue( uiFood )
		SetTrue( uiSlider )
		SetTrue( uiTrainBtn )
		SetFalse( uiCancelBtn )
		SetFalse( uiIdle )
		local maxv = math.floor(recvValue.m_train_confsec/SEC);
		m_slider = uiSlider:GetComponent( typeof(UISlider) )
		m_slider.minValue = 1;
		m_slider.maxValue = maxv;
		m_sliderNum = uiNum;
		m_sliderSec = uiSec;
		m_sliderFood = uiFood;
		TrainDlgSetSlider( m_slider, maxv )
	elseif type == 3 then -- 空闲中
		SetFalse( uiNum )
		SetFalse( uiTimerProgress )
		SetFalse( uiTimer )
		SetFalse( uiQuickBtn )
		SetFalse( uiSec )
		SetFalse( uiFood )
		SetFalse( uiSlider )
		SetFalse( uiTrainBtn )
		SetFalse( uiCancelBtn )
		SetTrue( uiIdle )
		SetText( uiIdle, T(632) )
	elseif type == 4 then -- 等待中
		SetTrue( uiNum )
		SetFalse( uiTimerProgress )
		SetFalse( uiTimer )
		SetFalse( uiQuickBtn )
		SetFalse( uiSec )
		SetFalse( uiFood )
		SetFalse( uiSlider )
		SetFalse( uiTrainBtn )
		SetTrue( uiCancelBtn )
		SetTrue( uiIdle )
		SetText( uiIdle, T(643) )
		SetText( uiNum, T(625)..":"..recvValue.m_queuenum[index] )
		SetControlID( uiCancelBtn, 1000+index )
	end
	
end

-- 设置滑动条
function TrainDlgSetSlider( slider, maxv )
	if m_slider == nil then
		return;
	end
	
--[[	local space = m_recvValue.m_soldiers_max - m_recvValue.m_soldiers;
	if maxv*m_recvValue.m_train_confnum > space then
		maxv = 1;
	end
	m_slider.value = maxv;--]]
	
	m_selectnum = maxv*m_recvValue.m_train_confnum;
	m_selectsec = maxv*SEC;
	m_selectfood = m_selectnum*m_recvValue.m_train_conffood;
	
	SetText( m_sliderNum, m_selectnum );
	SetText( m_sliderSec, secnum( m_selectsec ) );
	SetText( m_sliderFood, m_selectfood )
end

-- 招募
function TrainDlgTrain()
	if m_selectnum < 0 or m_selectnum >= m_recvValue.m_soldiers_max - m_recvValue.m_soldiers then
		pop(T(631))
		return
	end
	if m_selectfood < GetPlayer().m_food then
		pop(T(644))
		return
	end
	--m_selectsec
	system_askinfo( ASKINFO_TRAIN, "", 1, m_buildingKind, m_selectsec );
end

-- 加速
function TrainDlgQuick()
	system_askinfo( ASKINFO_TRAIN, "", 2, m_buildingKind );
end

-- 扩建
function TrainDlgQueue()
	system_askinfo( ASKINFO_TRAIN, "", 5, m_buildingKind );
end

-- 取消
function TrainDlgCancel( index )
	system_askinfo( ASKINFO_TRAIN, "", 3, m_buildingKind, index );
end
