-- 界面
local m_Dlg = nil;
local m_DialogFrameMod = nil;

local m_uiBaseSilver = nil; --UnityEngine.GameObject
local m_uiBaseWood = nil; --UnityEngine.GameObject
local m_uiBaseFood = nil; --UnityEngine.GameObject
local m_uiBaseIron = nil; --UnityEngine.GameObject
local m_uiTechSilver = nil; --UnityEngine.GameObject
local m_uiTechWood = nil; --UnityEngine.GameObject
local m_uiTechFood = nil; --UnityEngine.GameObject
local m_uiTechIron = nil; --UnityEngine.GameObject
local m_uiWeatherSilver = nil; --UnityEngine.GameObject
local m_uiWeatherWood = nil; --UnityEngine.GameObject
local m_uiWeatherFood = nil; --UnityEngine.GameObject
local m_uiWeatherIron = nil; --UnityEngine.GameObject
local m_uiActivitySilver = nil; --UnityEngine.GameObject
local m_uiActivityWood = nil; --UnityEngine.GameObject
local m_uiActivityFood = nil; --UnityEngine.GameObject
local m_uiActivityIron = nil; --UnityEngine.GameObject
local m_uiOfficialSilver = nil; --UnityEngine.GameObject
local m_uiOfficialWood = nil; --UnityEngine.GameObject
local m_uiOfficialFood = nil; --UnityEngine.GameObject
local m_uiOfficialIron = nil; --UnityEngine.GameObject
local m_uiTotalSilver = nil; --UnityEngine.GameObject
local m_uiTotalWood = nil; --UnityEngine.GameObject
local m_uiTotalFood = nil; --UnityEngine.GameObject
local m_uiTotalIron = nil; --UnityEngine.GameObject
local m_uiTimerText = nil; --UnityEngine.GameObject
local m_uiLevyNumText = nil; --UnityEngine.GameObject
local m_uiResBuyBtn = nil; --UnityEngine.GameObject
local m_uiInteriorBtn = nil; --UnityEngine.GameObject
local m_uiOfficialShape = nil; --UnityEngine.GameObject
local m_uiOfficialTimeBack = nil; --UnityEngine.GameObject
local m_uiOfficialTime = nil; --UnityEngine.GameObject
local m_uiOfficialName = nil; --UnityEngine.GameObject
local m_uiOfficialDesc = nil; --UnityEngine.GameObject
local m_uiOfficialEffect = nil; --UnityEngine.GameObject
local m_uiOfficialBtn = nil; --UnityEngine.GameObject

-- 打开界面
function LevyDlgOpen()
	m_Dlg = eye.uiManager:Open( "LevyDlg" );
	m_DialogFrameMod = DialogFrameModOpen( m_Dlg, BuildingName(1), HELP_LevyDlg, LevyDlgClose );
end

-- 隐藏界面
function LevyDlgClose()
	if m_Dlg == nil then
		return;
	end
	DialogFrameModClose( m_DialogFrameMod );
	m_DialogFrameMod = nil;
	eye.uiManager:Close( "LevyDlg" );
end

-- 删除界面
function LevyDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function LevyDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            LevyDlgClose();
		
		-- 收取
		elseif nControlID == 1 then
			LevyDlgGet();
		
		-- 购买资源
		elseif nControlID == 2 then
		
		-- 内政总览
		elseif nControlID == 3 then
			GovInfoDlgShow()
		
		-- 雇佣
		elseif nControlID == 5 then
			OfficialHireDlgShow( 1 );
			
        end
	elseif nType == UI_EVENT_TWEENFINISH then
		if nControlID == 1 then
			LevyDlgShow();
		end
	end
end

-- 载入时调用
function LevyDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;
	m_uiBaseSilver = objs[0];
	m_uiBaseWood = objs[1];
	m_uiBaseFood = objs[2];
	m_uiBaseIron = objs[3];
	m_uiTechSilver = objs[4];
	m_uiTechWood = objs[5];
	m_uiTechFood = objs[6];
	m_uiTechIron = objs[7];
	m_uiWeatherSilver = objs[8];
	m_uiWeatherWood = objs[9];
	m_uiWeatherFood = objs[10];
	m_uiWeatherIron = objs[11];
	m_uiActivitySilver = objs[12];
	m_uiActivityWood = objs[13];
	m_uiActivityFood = objs[14];
	m_uiActivityIron = objs[15];
	m_uiOfficialSilver = objs[16];
	m_uiOfficialWood = objs[17];
	m_uiOfficialFood = objs[18];
	m_uiOfficialIron = objs[19];
	m_uiTotalSilver = objs[20];
	m_uiTotalWood = objs[21];
	m_uiTotalFood = objs[22];
	m_uiTotalIron = objs[23];
	m_uiTimerText = objs[24];	
	m_uiLevyNumText = objs[25];
	m_uiResBuyBtn = objs[26];
	m_uiInteriorBtn = objs[27];
	m_uiOfficialShape = objs[28];
	m_uiOfficialTimeBack = objs[29];
	m_uiOfficialTime = objs[30];
	m_uiOfficialName = objs[31];
	m_uiOfficialDesc = objs[32];
	m_uiOfficialEffect = objs[33];
	m_uiOfficialBtn = objs[34];
end

-- 界面初始化时调用
function LevyDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function LevyDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function LevyDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function LevyDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function LevyDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function LevyDlgShow()
	LevyDlgOpen();
	m_uiResBuyBtn.transform:SetSiblingIndex(1000);
	m_uiInteriorBtn.transform:SetSiblingIndex(1000);
	LevyDlgSetOfficial()
end

local function LevyAdd( value, pre )
	if value > 0 then
		if pre ~= nil then
			return knum(value)
		else
			return "+"..knum(value)
		end
	else
		return "-"
	end
end

-- m_base={[4]},m_tech={[4]},m_weather={[4]},m_activity={[4]},m_offical={[4]},
function LevyDlgRecv( recvValue )
	-- 基础
	SetText( m_uiBaseSilver, LevyAdd( recvValue.m_base[1], 0 ) );
	SetText( m_uiBaseWood, LevyAdd( recvValue.m_base[2], 0 ) );
	SetText( m_uiBaseFood, LevyAdd( recvValue.m_base[3], 0 ) );
	SetText( m_uiBaseIron, LevyAdd( recvValue.m_base[4], 0 ) );
	
	-- 科技
	SetText( m_uiTechSilver,LevyAdd( recvValue.m_tech[1] ) );
	SetText( m_uiTechWood, LevyAdd( recvValue.m_tech[2] ) );
	SetText( m_uiTechFood, LevyAdd( recvValue.m_tech[3] ) );
	SetText( m_uiTechIron, LevyAdd( recvValue.m_tech[4] ) );
	
	-- 季节
	SetText( m_uiWeatherSilver, LevyAdd( recvValue.m_weather[1] ) );
	SetText( m_uiWeatherWood, LevyAdd( recvValue.m_weather[2] ) );
	SetText( m_uiWeatherFood, LevyAdd( recvValue.m_weather[3] ) );
	SetText( m_uiWeatherIron, LevyAdd( recvValue.m_weather[4] ) );
	
	-- 活动
	SetText( m_uiActivitySilver, LevyAdd( recvValue.m_activity[1] ) );
	SetText( m_uiActivityWood, LevyAdd( recvValue.m_activity[2] ) );
	SetText( m_uiActivityFood, LevyAdd( recvValue.m_activity[3] ) );
	SetText( m_uiActivityIron, LevyAdd( recvValue.m_activity[4] ) );
	
	-- 官职
	SetText( m_uiOfficialSilver, LevyAdd( recvValue.m_offical[1] ) );
	SetText( m_uiOfficialWood, LevyAdd( recvValue.m_offical[2] ) );
	SetText( m_uiOfficialFood, LevyAdd( recvValue.m_offical[3] ) );
	SetText( m_uiOfficialIron, LevyAdd( recvValue.m_offical[4] ) );
	
	-- 总计
	local total = {0,0,0,0}
	for i=1, 4, 1 do
		total[i] = knum( recvValue.m_base[i] + recvValue.m_tech[i] + recvValue.m_weather[i] + recvValue.m_activity[i] + recvValue.m_offical[i] );
	end
	SetText( m_uiTotalSilver, total[1] );
	SetText( m_uiTotalWood, total[2] );
	SetText( m_uiTotalFood, total[3] );
	SetText( m_uiTotalIron, total[4] );

	LevyDlgChangeSec( recvValue.m_sec )
end

function LevyDlgChangeSec( sec )
	if m_Dlg == nil then
		return;
	end
	if GetPlayer().m_levynum > 0 then
		SetFalse( m_uiTimerText );
		SetTrue( m_uiLevyNumText );
		SetTimer( m_uiTimerText, 0, 0, 0 );
		SetText( m_uiLevyNumText, F(656,GetPlayer().m_levynum,global.levy_max) );
	else
		SetFalse( m_uiLevyNumText );
		SetTrue( m_uiTimerText );
		SetTimer( m_uiTimerText, sec, sec, 1, T(658) );
	end
end


-- 设置内政官
function LevyDlgSetOfficial()
	if m_Dlg == nil then
		return;
	end
	system_askinfo( ASKINFO_LEVY, "", 0 )
	local info = GetPlayer().m_officialhire[1];
	-- 已雇佣
	if info.m_ofkind > 0 then
		SetTrue( m_uiOfficialShape )
		SetFalse( m_uiOfficialEffect )
		SetTrue( m_uiOfficialTimeBack )
		SetTrue( m_uiOfficialTime )
		SetTimer( m_uiOfficialTime, info.m_ofsec, info.m_ofsec, 2 )
		if g_official_gov[info.m_ofkind].token > 0 then
			SetText( m_uiOfficialName, T(719).."Lv."..g_official_gov[info.m_ofkind].level..T(722), Hex2Color(0xD95DF4FF) )
		else
			SetText( m_uiOfficialName, T(719).."Lv."..g_official_gov[info.m_ofkind].level..T(722), Hex2Color(0xF7F3BBFF) )
		end
		SetText( m_uiOfficialDesc, F(732, g_official_gov[info.m_ofkind].produce ), Hex2Color(0x25C9FFFF) )
		SetImage( m_uiOfficialShape, OfSprite( g_official_gov[info.m_ofkind].shape ) )
	else
		-- 未雇佣
		SetFalse( m_uiOfficialShape )
		SetFalse( m_uiOfficialTimeBack )
		SetFalse( m_uiOfficialTime )
		local pBuilding = GetPlayer():GetBuilding( BUILDING_Main, -1 );
		if pBuilding == nil then
			return;
		end
		if pBuilding.m_level < g_official_gov[1].buildinglevel then
			-- 官府N级解锁Lv.1内政官
			SetText( m_uiOfficialName, F(726, BuildingName(BUILDING_Main), g_official_gov[1].buildinglevel, g_official_gov[1].level, T(722) ), Hex2Color(0x8A8A8AFF) )
			SetText( m_uiOfficialDesc, F(732, g_official_gov[1].produce ), Hex2Color(0x8A8A8AFF) )
			SetFalse( m_uiOfficialEffect )
			SetImage( m_uiOfficialBtn.transform:Find("Back"), LoadSprite("ui_icon_back_2") )
		else
			SetTrue( m_uiOfficialEffect )
			SetImage( m_uiOfficialBtn.transform:Find("Back"), LoadSprite("ui_icon_back_4") )
			SetText( m_uiOfficialName, T(722).."("..T(2338)..")", Hex2Color(0x8A8A8AFF) )
			SetText( m_uiOfficialDesc, T(2339), Hex2Color(0x8A8A8AFF) )
			-- 可雇佣
			--[[for kind=#g_official_gov, 1, -1 do
				if pBuilding.m_level >= g_official_gov[kind].buildinglevel then
					SetText( m_uiOfficialName, T(718).."Lv."..g_official_gov[kind].level..T(722), Hex2Color(0x8A8A8AFF) )
					SetText( m_uiOfficialDesc, F(732, g_official_gov[kind].produce ), Hex2Color(0x8A8A8AFF) )
					break;
				end
			end--]]
		end
	end

end

-- 收取
function LevyDlgGet()
	if GetPlayer().m_levynum <= 0 then
		AlertMsg(T(657))
		return
	end
	system_askinfo( ASKINFO_LEVY, "", 1 )
end