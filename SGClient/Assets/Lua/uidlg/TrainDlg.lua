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
local m_uiForceBtn = nil; --UnityEngine.GameObject
local m_uiTrainlongBtn = nil; --UnityEngine.GameObject
local m_uiTrainlongInfo = nil; --UnityEngine.GameObject
local m_uiTrainQueueInfo = nil; --UnityEngine.GameObject

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
local m_uiTimerCache = nil;
local m_popcd = 0
-- 打开界面
function TrainDlgOpen()
	m_Dlg = eye.uiManager:Open( "TrainDlg" );
	m_DialogFrameMod = DialogFrameModOpen( m_Dlg, T(m_buildingKind).." Lv."..m_pBuilding.m_level, HELP_TrainDlg, TrainDlgClose );
end

-- 隐藏界面
function TrainDlgClose()
	if m_Dlg == nil then
		return;
	end
	TrainDlgReset()
	DialogFrameModClose( m_DialogFrameMod );
	m_DialogFrameMod = nil;
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
			
		elseif nControlID == -2 then
			SetFalse( m_uiTrainlongInfo );
		
		elseif nControlID == -3 then
			SetFalse( m_uiTrainQueueInfo );
		
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
			ShopDlgShowByType( 1 )
			TrainDlgClose();
		
		-- 募兵加时
		elseif nControlID == 12 then
			TrainDlgTrainLong()
			
		-- 募兵加时购买
		elseif nControlID == 13 then
			TrainDlgTrainLongBuy()
		
		-- 扩建兵营购买
		elseif nControlID == 14 then
			TrainDlgQueueBuy()
			
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
			if space <= 0 and os.time()-m_popcd > 1 then
				pop( T(630) )
				m_popcd = os.time();
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
	m_uiForceBtn = objs[9];
	m_uiTrainlongBtn = objs[10];
	m_uiTrainlongInfo = objs[11];
	m_uiTrainQueueInfo = objs[12];
	
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
	m_uiForceBtn.transform:SetSiblingIndex(1000);
	m_uiTrainlongBtn.transform:SetSiblingIndex(1000);
	
	if m_buildingKind == BUILDING_Infantry or m_buildingKind == BUILDING_Militiaman_Infantry then
		m_corps = 0
	elseif m_buildingKind == BUILDING_Cavalry or m_buildingKind == BUILDING_Militiaman_Cavalry then
		m_corps = 1
	elseif m_buildingKind == BUILDING_Archer or m_buildingKind == BUILDING_Militiaman_Archer then
		m_corps = 2
	end
	SetImage( m_uiCorps, CorpsSprite( m_corps ) );
	SetImage( m_uiShape, CorpsFaceSprite( m_corps ) );
	
	-- 请求信息
	system_askinfo( ASKINFO_TRAIN, "", 0, buildingKind );
	
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
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
		return;
	end
	TrainDlgReset()
	m_recvValue = recvValue;
	m_uiTimerCache = nil
	SetText( m_uiDesc, T(m_corps+619) )
	SetText( m_uiSoldiers, "<color=#927F64FF>"..T(622)..": </color><color=#ABC7D9FF>"..recvValue.m_soldiers.."</color>" )
	SetText( m_uiSoldiersMax, "<color=#927F64FF>"..T(623)..": </color><color=#ABC7D9FF>"..recvValue.m_soldiers_max.."</color>" )
	SetRichText( m_uiQueueLevel, "<color=927F64FF>"..T(624)..": </color><emote=002><color=ABC7D9FF>x"..recvValue.m_queue.."</color>" )
	
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
	if recvValue.m_queue < 11 then
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
	local uiSecIcon = objs[10];
	local uiFoodIcon = objs[11];
	local uiWait = objs[12];
	local uiBack1 = objs[13];
	local uiBack2 = objs[14];
	local uiBack3 = objs[15];
	local uiBack4 = objs[16];
	SetTimer( uiTimer, 0, 0, 0 )
	
	if type == 1 then -- 正在招募中
		SetTrue( uiNum )
		SetTrue( uiTimerProgress )
		SetTrue( uiTimer )
		SetTrue( uiQuickBtn )
		SetTrue( uiBack2 )
		SetTrue( uiBack3 )
		SetTrue( uiBack4 )
		SetFalse( uiSecIcon )
		SetFalse( uiFoodIcon )
		SetFalse( uiSec )
		SetFalse( uiFood )
		SetFalse( uiSlider )
		SetFalse( uiTrainBtn )
		SetFalse( uiCancelBtn )
		SetFalse( uiIdle )	
		SetFalse( uiWait )	
		SetText( uiNum, T(625)..":"..recvValue.m_trainnum )
		SetTimer( uiTimer, recvValue.m_trainsec, recvValue.m_trainsec_need, 1, T(756) )
		m_uiTimerCache = uiTimer
	elseif type == 2 then -- 选择招募
		SetTrue( uiNum )
		SetTrue( uiBack2 )
		SetTrue( uiBack3 )
		SetTrue( uiBack4 )
		SetFalse( uiTimerProgress )
		SetFalse( uiTimer )
		SetFalse( uiQuickBtn )
		SetTrue( uiSecIcon )
		SetTrue( uiFoodIcon )
		SetTrue( uiSec )
		SetTrue( uiFood )
		SetTrue( uiSlider )
		SetTrue( uiTrainBtn )
		SetFalse( uiCancelBtn )
		SetFalse( uiIdle )
		SetFalse( uiWait )
		local maxv = math.floor(recvValue.m_train_confsec/SEC);
		m_slider = uiSlider:GetComponent( typeof(UISlider) )
		m_slider.minValue = 1;
		m_slider.maxValue = maxv;
		m_sliderNum = uiNum;
		m_sliderSec = uiSec;
		m_sliderFood = uiFood;
		TrainDlgSetSlider( m_slider, maxv )
	elseif type == 3 then -- 空闲中
		SetFalse( uiBack2 )
		SetFalse( uiBack3 )
		SetFalse( uiBack4 )
		SetFalse( uiNum )
		SetFalse( uiTimerProgress )
		SetFalse( uiTimer )
		SetFalse( uiQuickBtn )
		SetFalse( uiSecIcon )
		SetFalse( uiFoodIcon )
		SetFalse( uiSec )
		SetFalse( uiFood )
		SetFalse( uiSlider )
		SetFalse( uiTrainBtn )
		SetFalse( uiCancelBtn )
		SetTrue( uiBack1 )
		SetTrue( uiIdle )
		SetText( uiIdle, T(632) )
		SetFalse( uiWait )
	elseif type == 4 then -- 等待中
		SetTrue( uiNum )
		SetTrue( uiTimerProgress )
		SetProgress( uiTimerProgress, 0 );
		SetFalse( uiTimer )
		SetFalse( uiQuickBtn )
		SetFalse( uiSecIcon )
		SetFalse( uiFoodIcon )
		SetFalse( uiSec )
		SetFalse( uiFood )
		SetFalse( uiSlider )
		SetFalse( uiTrainBtn )
		SetTrue( uiCancelBtn )
		SetFalse( uiIdle )
		SetTrue( uiWait )
		SetText( uiWait, T(641) );
		SetText( uiNum, T(625)..":"..recvValue.m_queuenum[index] )
		SetControlID( uiCancelBtn, 1000+index )
	end
	
end

-- 设置滑动条
function TrainDlgSetSlider( slider, maxv )
	if m_slider == nil then
		return;
	end
	local space = m_recvValue.m_soldiers_max - m_recvValue.m_soldiers;
	m_slider.value = maxv;
	m_selectnum = maxv*m_recvValue.m_train_confnum;
	m_selectsec = maxv*SEC;
	m_selectfood = m_selectnum*m_recvValue.m_train_conffood;
	
	-- 选择数量
	if space <= 0 then
		SetText( m_sliderNum, "<color=#e80017>"..T(625)..":"..m_selectnum.."</color>" );
	else
		SetText( m_sliderNum, T(625)..":"..m_selectnum );
	end
	-- 需要时间
	SetText( m_sliderSec, secnum( m_selectsec ) );
	
	-- 需要粮草
	if m_selectfood < GetPlayer().m_food then
		SetText( m_sliderFood, "<color=#03de27>"..knum(m_selectfood).."/"..knum(GetPlayer().m_food).."</color>" )
	else
		SetText( m_sliderFood, "<color=#03de27>"..knum(m_selectfood).."</color><color=#e80017>/"..knum(GetPlayer().m_food).."</color>" )
	end
end

-- 招募
function TrainDlgTrain()
	local space = m_recvValue.m_soldiers_max - m_recvValue.m_soldiers;
	if m_selectnum < 0 or space <= 0 then
		pop(T(631))
		return
	end
	if GetPlayer().m_food < m_selectfood then
		pop(T(644))
		return
	end
	--m_selectsec
	system_askinfo( ASKINFO_TRAIN, "", 1, m_buildingKind, m_selectsec );
end

-- 加速
function TrainDlgQuick()
	--system_askinfo( ASKINFO_TRAIN, "", 2, m_buildingKind );
	if m_uiTimerCache ~= nil then
		QuickItemDlgShow( 2, m_buildingKind, -1, m_uiTimerCache.transform:GetComponent( typeof(UITextTimeCountdown) ).LeftTime )
	end
end

-- 取消
function TrainDlgCancel( index )
	system_askinfo( ASKINFO_TRAIN, "", 3, m_buildingKind, index );
end

-- 拼星级
local function TrainDlgQueueStar( queue )
	local star = ""
	for i=1, queue, 1 do
		star = star.."<emote=002>"
	end
	return star
end

-- 扩建
function TrainDlgQueue()
	SetTrue( m_uiTrainQueueInfo );
	local objs = m_uiTrainQueueInfo.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiBefore = objs[0];
	local uiAfter = objs[1];
	local uiArrow = objs[2];
	local uiBuyBtn = objs[3];
	local uiBuyBtnText = objs[4];
	local uiDesc = objs[5];
	
	local maxlevel = 0;
	for k, v in pairs(g_trainqueue) do
		if k > maxlevel then
			maxlevel = k
		end
	end
	if m_recvValue.m_queue >= maxlevel then
		SetFalse( uiAfter )
		SetFalse( uiArrow )
		SetFalse( uiBuyBtn )
		SetFalse( uiDesc )
		
		SetRichText( uiBefore.transform:Find("Text1"), T(637)..":"..TrainDlgQueueStar(m_recvValue.m_queue) )
		SetText( uiBefore.transform:Find("Text2"), T(639)..":<color=#25c9ff>"..(m_recvValue.m_queue+1).."</color>" )
		SetText( uiBefore.transform:Find("Text3"), T(639)..":<color=#25c9ff>"..m_recvValue.m_soldiers_max.."</color>" )
	else
		SetTrue( uiAfter )
		SetTrue( uiArrow )
		SetTrue( uiBuyBtn )
		SetTrue( uiDesc )
		
		if m_pBuilding.m_kind == BUILDING_Infantry or m_pBuilding.m_kind == BUILDING_Militiaman_Infantry then
			SetText( uiDesc, F( 640, item_getname(445), 1 ) )
		elseif m_pBuilding.m_kind == BUILDING_Cavalry or m_pBuilding.m_kind == BUILDING_Militiaman_Cavalry then
			SetText( uiDesc, F( 640, item_getname(446), 1 ) )
		elseif m_pBuilding.m_kind == BUILDING_Archer or m_pBuilding.m_kind == BUILDING_Militiaman_Archer then
			SetText( uiDesc, F( 640, item_getname(447), 1 ) )
		end
		
		SetRichText( uiBefore.transform:Find("Text1"), T(637)..":"..TrainDlgQueueStar(m_recvValue.m_queue) )
		SetText( uiBefore.transform:Find("Text2"), T(639)..":<color=#25c9ff>"..(m_recvValue.m_queue+1).."</color>" )
		SetText( uiBefore.transform:Find("Text3"), T(639)..":<color=#25c9ff>"..m_recvValue.m_soldiers_max.."</color>" )
		
		SetRichText( uiAfter.transform:Find("Text1"), T(638)..":"..TrainDlgQueueStar(m_recvValue.m_queue+1) )
		SetText( uiAfter.transform:Find("Text2"), T(639)..":<color=#25c9ff>"..(m_recvValue.m_queue+1).."</color><color=#ecc244>".."+1</color>" )
		SetText( uiAfter.transform:Find("Text3"), T(639)..":<color=#25c9ff>"..m_recvValue.m_soldiers_max.."</color><color=#ecc244>".."+3000</color>" )
		
		SetText( uiBuyBtnText, knum(g_trainqueue[m_recvValue.m_queue+1].token) );
	end
end

-- 扩建购买
function TrainDlgQueueBuy()
	SetFalse( m_uiTrainQueueInfo );
	local token = g_trainqueue[m_recvValue.m_queue+1].token
	MsgBox( F(760, knum(token) ), function() 
		if GetPlayer().m_token < token then
			JumpToken()
		else
			system_askinfo( ASKINFO_TRAIN, "", 5, m_buildingKind );
		end
	end )
	
end

-- 募兵时长
function TrainDlgTrainLong()
	SetTrue( m_uiTrainlongInfo );
	local objs = m_uiTrainlongInfo.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiBefore = objs[0];
	local uiAfter = objs[1];
	local uiArrow = objs[2];
	local uiBuyBtn = objs[3];
	local uiBuyBtnText = objs[4];
	
	local maxlevel = 0;
	for k, v in pairs(g_trainlong) do
		if k > maxlevel then
			maxlevel = k
		end
	end
	if m_recvValue.m_trainlong >= maxlevel then
		SetFalse( uiAfter )
		SetFalse( uiArrow )
		SetFalse( uiBuyBtn )
		SetText( uiBefore.transform:Find("Text"), F( 634, zhtime( g_trainlong[m_recvValue.m_trainlong].timelong*60 ) ) )
	else
		SetTrue( uiAfter )
		SetTrue( uiArrow )
		SetTrue( uiBuyBtn )
		SetText( uiBefore.transform:Find("Text"), F( 634, zhtime( g_trainlong[m_recvValue.m_trainlong].timelong*60 ) ) )
		SetText( uiAfter.transform:Find("Text"), F( 635, zhtime( g_trainlong[m_recvValue.m_trainlong+1].timelong*60 ) ) )
		SetText( uiBuyBtnText, knum(g_trainlong[m_recvValue.m_trainlong+1].silver) );
	end
end

-- 募兵加时购买
function TrainDlgTrainLongBuy()
	SetFalse( m_uiTrainlongInfo );
	local silver = g_trainlong[m_recvValue.m_trainlong+1].silver;
	MsgBox( F(757, knum(silver) ), function() 
		if GetPlayer().m_silver < silver then
			JumpRes( 1 )
		else
			system_askinfo( ASKINFO_TRAIN, "", 6, m_buildingKind );
		end
	end )
	
end

function TrainDlgReset()
	if m_Dlg == nil or IsActive( m_Dlg ) == false then
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
end
