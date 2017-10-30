-- 界面
local json = require "cjson"
local m_Dlg = nil;
local m_uiDialogFrameMod = nil; --UnityEngine.GameObject
local m_uiRecvTime = nil; --UnityEngine.GameObject
local m_uiDelTime = nil; --UnityEngine.GameObject
local m_uiScroll = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiMailContent = nil; --UnityEngine.GameObject
local m_uiMailAttach = nil; --UnityEngine.GameObject
local m_uiUIP_Item = nil; --UnityEngine.GameObject
local m_uiMailFight = nil; --UnityEngine.GameObject
local m_uiUIP_UnitRow = nil; --UnityEngine.GameObject
local m_uiShareBtn = nil; --UnityEngine.GameObject
local m_uiRecBtn = nil; --UnityEngine.GameObject
local m_uiDefriendBtn = nil; --UnityEngine.GameObject
local m_uiReplyBtn = nil; --UnityEngine.GameObject
local m_uiActorContent = nil; --UnityEngine.GameObject
local m_uiGatherContent = nil; --UnityEngine.GameObject
local m_uiSpyWallContent = nil; --UnityEngine.GameObject
local m_uiSpyHeroContent = nil; --UnityEngine.GameObject
local m_uiUIP_SpyUnit = nil; --UnityEngine.GameObject
local m_uiSpyWarnContent = nil; --UnityEngine.GameObject

local m_ObjectPool = nil;
local m_recvValue = nil

local m_share_a_name = "";
local m_share_d_name = "";

local m_from_actorid = 0;
local m_from_actorname = "";
local m_from_actornation = 0;
local m_from_msg = "";
local m_from_recvtime = 0;
local m_spy_hashero = 0;

FIGHT_UNITTYPE_HERO		=	1 -- 玩家英雄
FIGHT_UNITTYPE_GUARD	=	2 -- 玩家城墙守卫
FIGHT_UNITTYPE_MONSTER	=	3 -- 怪物表

-- 打开界面
function MailInfoDlgOpen()
	m_Dlg = eye.uiManager:Open( "MailInfoDlg" );
end

-- 隐藏界面
function MailInfoDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "MailInfoDlg" );
end

-- 删除界面
function MailInfoDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function MailInfoDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            MailInfoDlgClose();
		
		-- 点击解锁
		elseif nControlID == 1 then
			MailInfoDlgUnLock()
			
		-- 点击锁定
		elseif nControlID == 2 then
			MailInfoDlgLock()
		
		-- 点击删除
		elseif nControlID == 3 then
			MailInfoDlgDelete()
		
		-- 领取附件
		elseif nControlID == 10 then
			MailInfoDlgAttachGet()
		
		-- 分享邮件	
		elseif nControlID == 11 then
			MailInfoDlgShare()
			
		-- 战斗回放
		elseif nControlID == 12 then
		
		-- 拉黑
		elseif nControlID == 13 then
		
		-- 回复
		elseif nControlID == 14 then
			MailSendDlgReply( m_from_actorid, m_from_actorname, m_from_actornation, m_from_msg, m_from_recvtime )
        end
	end
end

-- 载入时调用
function MailInfoDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiDialogFrameMod = objs[0];
	m_uiRecvTime = objs[1];
	m_uiDelTime = objs[2];
	m_uiScroll = objs[3];
	m_uiContent = objs[4];
	m_uiMailContent = objs[5];
	m_uiMailAttach = objs[6];
	m_uiUIP_Item = objs[7];
	m_uiMailFight = objs[8];
	m_uiUIP_UnitRow = objs[9];
	m_uiShareBtn = objs[10];
	m_uiRecBtn = objs[11];
	m_uiDefriendBtn = objs[12];
	m_uiReplyBtn = objs[13];
	m_uiActorContent = objs[14];
	m_uiGatherContent = objs[15];
	m_uiSpyWallContent = objs[16];
	m_uiSpyHeroContent = objs[17];
	m_uiUIP_SpyUnit = objs[18];
	m_uiSpyWarnContent = objs[19];
	
	-- 对象池
	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );
	m_ObjectPool:CreatePool("UIP_Item", 6, 6, m_uiUIP_Item);
	m_ObjectPool:CreatePool("UIP_UnitRow", 5, 5, m_uiUIP_UnitRow);
	m_ObjectPool:CreatePool("UIP_SpyUnit", 5, 5, m_uiUIP_SpyUnit);
end

-- 界面初始化时调用
function MailInfoDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function MailInfoDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function MailInfoDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function MailInfoDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function MailInfoDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function MailInfoDlgShow()
	MailInfoDlgOpen()
	MailInfoDlgAttachClear()
	MailInfoDlgFightUnitClear()
	MailInfoDlgSpyUnitClear()
	SetFalse( m_uiMailContent );
	SetFalse( m_uiMailFight );
	SetFalse( m_uiActorContent );
	SetFalse( m_uiGatherContent );
	SetFalse( m_uiSpyWallContent );
	SetFalse( m_uiSpyHeroContent );
	SetFalse( m_uiSpyWarnContent );
	m_uiScroll:GetComponent("UIScrollRect"):ResetScroll();
end

function MailInfoDlgByIncrementID( incrementid )
	local recvValue = nil;
	local endIndex = #GetMail().m_Mails;
	for i = 1, endIndex, 1 do
		if GetMail().m_Mails[i].m_incrementid == incrementid then
			recvValue = GetMail().m_Mails[i];
			break;
		end
	end
	MailInfoDlgByRecvValue( recvValue )
end

function MailInfoDlgByMailID( mailid )
	local recvValue = nil;
	local endIndex = #GetMail().m_Mails;
	for i = 1, endIndex, 1 do
		if int64.equals( GetMail().m_Mails[i].m_mailid, mailid ) == true then
			recvValue = GetMail().m_Mails[i];
			break;
		end
	end
	if recvValue == nil then
		return false
	end
	MailInfoDlgByRecvValue( recvValue )
	return true;
end

function MailInfoDlgByRecvValue( recvValue )
	if recvValue == nil then
		return
	end
	MailInfoDlgShow()
	m_recvValue = recvValue;
	m_share_a_name = "";
	m_share_d_name = "";
	m_from_actorid = 0;
	m_from_actorname = "";
	m_from_actornation = 0;
	m_from_msg = "";
	m_from_recvtime = 0;
	m_spy_hashero = 0;
	
	-- 解析标题
	local title = "";
	if recvValue.m_type == MAIL_TYPE_ACTOR_SEND or recvValue.m_type == MAIL_TYPE_ACTOR_REPLY then
		title = T(1192)	
	else
		if GetMail():IsTag( recvValue.m_title, TAG_TEXTID ) then
			local textid = tonumber(string.sub(recvValue.m_title, string.len(TAG_TEXTID) + 1));
			if textid ~= nil then
				title = T(textid)
			end
		else
			title = recvValue.m_title;
		end
	end
	SetText( m_uiDialogFrameMod.transform:Find("Top/TitleText"), title )
	
	-- 接收时间
	SetText( m_uiRecvTime, os.date( "%m-%d %H:%M", recvValue.m_recvtime ) )
	
	-- 删除时间
	local delday = 7-math.floor( (GetServerTime()-recvValue.m_recvtime)/86400 );
	if delday <= 0 then
		delday = 1;
	end
	SetText( m_uiDelTime, F( 1106, delday ) )
	
	-- 解析内容
	-- 玩家间邮件
	if recvValue.m_type == MAIL_TYPE_ACTOR_SEND or recvValue.m_type == MAIL_TYPE_ACTOR_REPLY then
		SetTrue( m_uiActorContent );
		
		-- 来信人
		local title_json = json.decode( recvValue.m_title );
		local fromname = F( 5504, Nation( title_json["n"] ), title_json["lv"], title_json["na"] );
		SetRichText( m_uiActorContent.transform:Find("Target/Name"), fromname )
		
		-- 来信内容
		m_from_actorname = title_json["na"];
		m_from_actornation = title_json["n"];
		m_from_actorid = recvValue.m_content_json["fromid"];
		local msg = recvValue.m_content_json["msg"];
		SetRichText( m_uiActorContent.transform:Find("Target/Text"), msg )
		m_from_msg = msg;
		m_from_recvtime = recvValue.m_recvtime;
		
		if recvValue.m_type == MAIL_TYPE_ACTOR_SEND then
			SetFalse( m_uiActorContent.transform:Find("My") );
		else
			SetTrue( m_uiActorContent.transform:Find("My") );
			-- 我的上一条信息
			local _nation = recvValue.m_content_json["n"];
			local _name = recvValue.m_content_json["na"];
			local _recvtime = recvValue.m_content_json["t"];
			SetRichText( m_uiActorContent.transform:Find("My/Name"), "["..Nation( _nation ).."]".._name );
			SetText( m_uiActorContent.transform:Find("My/RecvTime"), os.date( "%m-%d %H:%M", _recvtime ) );
			SetRichText( m_uiActorContent.transform:Find("My/Text"), recvValue.m_content_json["reply"] )
		end
	
	-- 侦察
	elseif recvValue.m_type == MAIL_TYPE_CITY_SPY then
		SetTrue( m_uiMailContent );
		
		local flag = recvValue.m_content_json["flag"];
		local nation = recvValue.m_content_json["n"];
		local level = recvValue.m_content_json["lv"];
		local name = recvValue.m_content_json["na"];
		local pos = recvValue.m_content_json["pos"];
		
		-- 侦察失败
		if flag == 0 then
			SetRichText( m_uiMailContent.transform:Find("Text"), F( 5512, Nation(nation), level, name, pos ), MailOnLinkClick );
			
		-- 侦察成功
		else
			-- 资源信息
			local people = recvValue.m_content_json["pp"];
			local silver = recvValue.m_content_json["silver"];
			local wood = recvValue.m_content_json["wood"];
			local food = recvValue.m_content_json["food"];
			local iron = recvValue.m_content_json["iron"];
			SetRichText( m_uiMailContent.transform:Find("Text"), F( 5515, Nation(nation), level, name, pos, people, knum(silver), knum(wood), knum(food), knum(iron) ), MailOnLinkClick );
			
			-- 城墙信息
			SetTrue( m_uiSpyWallContent );
			local walllv = recvValue.m_content_json["walllv"];
			if walllv ~= nil then
				local bp = recvValue.m_content_json["bp"];
				local cp1 = recvValue.m_content_json["cp1"];
				local cp2 = recvValue.m_content_json["cp2"];
				local cp3 = recvValue.m_content_json["cp3"];
				local hsu = recvValue.m_content_json["hsu"];
				SetTrue( m_uiSpyWallContent.transform:Find("WallLevel") );
				SetTrue( m_uiSpyWallContent.transform:Find("BattlePower") );
				SetTrue( m_uiSpyWallContent.transform:Find("CorpsGrid") );
				SetFalse( m_uiSpyWallContent.transform:Find("Warn") );
				SetText( m_uiSpyWallContent.transform:Find("WallLevel"), F(1186, walllv) );
				SetText( m_uiSpyWallContent.transform:Find("BattlePower"), F(1185, bp) );
				SetText( m_uiSpyWallContent.transform:Find("CorpsGrid/Corps1/Text"), T(134).."x"..cp1 );
				SetText( m_uiSpyWallContent.transform:Find("CorpsGrid/Corps2/Text"), T(135).."x"..cp2 );
				SetText( m_uiSpyWallContent.transform:Find("CorpsGrid/Corps3/Text"), T(136).."x"..cp3 );
				m_spy_hashero = hsu;
				
				-- 没武将信息
				if m_spy_hashero == 0 then
					SetTrue( m_uiSpyWarnContent );
					SetText( m_uiSpyWarnContent.transform:Find("Text"), T(5517) );
				end
			
			else
				SetFalse( m_uiSpyWallContent.transform:Find("WallLevel") );
				SetFalse( m_uiSpyWallContent.transform:Find("BattlePower") );
				SetFalse( m_uiSpyWallContent.transform:Find("CorpsGrid") );
				SetTrue( m_uiSpyWallContent.transform:Find("Warn") );
				SetText( m_uiSpyWallContent.transform:Find("Warn"), T(5516) );
			end
			
			m_share_a_name = "["..Nation(GetPlayer().m_nation).."]"..GetPlayer().m_name;
			m_share_d_name = "["..Nation(nation).."]"..name;
		end
	
	-- 被侦察
	elseif recvValue.m_type == MAIL_TYPE_CITY_BESPY then
		SetTrue( m_uiMailContent );
		
		local flag = recvValue.m_content_json["flag"];
		local nation = recvValue.m_content_json["n"];
		local level = recvValue.m_content_json["lv"];
		local name = recvValue.m_content_json["na"];
		local pos = recvValue.m_content_json["pos"];
		SetRichText( m_uiMailContent.transform:Find("Text"), F( 5514, Nation(nation), level, name, pos ), MailOnLinkClick );
			
	-- 采集
	elseif recvValue.m_type == MAIL_TYPE_GATHER then
		SetTrue( m_uiMailContent );
		SetTrue( m_uiGatherContent );
		
		local reback = recvValue.m_content_json["reback"];
		local pos = recvValue.m_content_json["pos"];
		local time = recvValue.m_content_json["time"];
		local restype = recvValue.m_content_json["res"];
		local resnum = recvValue.m_content_json["num"];
		local buff = recvValue.m_content_json["buff"];
		local herokind = recvValue.m_content_json["hkind"];
		local herolevel = recvValue.m_content_json["hlv"];
		local heroexp = recvValue.m_content_json["hexp"];
		
		-- 内容
		if reback == ARMY_REBACK_NORMAL	then
			SetRichText( m_uiMailContent.transform:Find("Text"), F( 5507, pos ), MailOnLinkClick );
			
		elseif reback == ARMY_REBACK_RETURN then
			SetRichText( m_uiMailContent.transform:Find("Text"), F( 5508, pos ), MailOnLinkClick );
			
		elseif reback == ARMY_REBACK_FIGHTLOSE then
			SetRichText( m_uiMailContent.transform:Find("Text"), F( 5509, pos ), MailOnLinkClick );
			
		end

		-- 采集详细内容 采集时间:{0}\n采集获得:{1}\nLv.{2} {3} 经验+{4}
		local gatherContent = "";
		if buff > 0 then
			gatherContent = F( 5510, secnum( time ), ResName( restype ).."x"..resnum.."("..T(1215)..")", herolevel, HeroName( herokind ), heroexp );
		else
			gatherContent = F( 5510, secnum( time ), ResName( restype ).."x"..resnum, herolevel, HeroName( herokind ), heroexp );
		end
		SetText( m_uiGatherContent.transform:Find("GatherText"), gatherContent );
		
		-- 如果是被打回来的
		if reback == ARMY_REBACK_FIGHTLOSE then
			--SetTrue( m_uiGatherContent.transform:Find("FightLose") );
			SetFalse( m_uiGatherContent.transform:Find("FightLose") );
		else
			SetFalse( m_uiGatherContent.transform:Find("FightLose") );
		end
	
	-- 采集战斗
	elseif recvValue.m_type == MAIL_TYPE_GATHER_FIGHT then
		SetTrue( m_uiMailContent );
		local my = recvValue.m_content_json["my"];
		local win = recvValue.m_content_json["win"];
		
		local name = recvValue.m_content_json["na"];		
		local nation = recvValue.m_content_json["n"];
		local level = recvValue.m_content_json["lv"];
		local pos = recvValue.m_content_json["pos"];
		
		local tname = recvValue.m_content_json["tna"];
		local tnation = recvValue.m_content_json["tn"];
		local tlevel = recvValue.m_content_json["tlv"];
		local tpos = recvValue.m_content_json["tpos"];
		
		if my == 1 then -- 我是攻击方
			SetRichText( m_uiMailContent.transform:Find("Text"), F(5518, Nation(nation), name, pos, Nation(tnation), tname, tpos ), MailOnLinkClick )
			m_share_a_name = "["..Nation(nation).."]"..name;
			m_share_d_name = "["..Nation(tnation).."]"..tname;
		else -- 我是防御方
			SetRichText( m_uiMailContent.transform:Find("Text"), F(5518, Nation(tnation), tname, tpos, Nation(nation), name, pos ), MailOnLinkClick )
			m_share_a_name = "["..Nation(tnation).."]"..tname;
			m_share_d_name = "["..Nation(nation).."]"..name;
		end
	
	-- 城战
	elseif recvValue.m_type == MAIL_TYPE_FIGHT_CITY then
		SetTrue( m_uiMailContent );
		local my = recvValue.m_content_json["my"];
		local win = recvValue.m_content_json["win"];
		
		local name = recvValue.m_content_json["na"];		
		local nation = recvValue.m_content_json["n"];
		local level = recvValue.m_content_json["lv"];
		local pos = recvValue.m_content_json["pos"];
		
		local tname = recvValue.m_content_json["tna"];
		local tnation = recvValue.m_content_json["tn"];
		local tlevel = recvValue.m_content_json["tlv"];
		local tpos = recvValue.m_content_json["tpos"];
		
		if my == 1 then -- 我是攻击方
			local award = "";
			if win == 1 then
				award = F( 5520, name, 
									T(121).."x"..knum(recvValue.m_content_json["silver"]),
									T(122).."x"..knum(recvValue.m_content_json["wood"]),
									T(123).."x"..knum(recvValue.m_content_json["food"]),
									T(127).."x"..recvValue.m_content_json["people"] )
			end
			SetRichText( m_uiMailContent.transform:Find("Text"), F(5518, Nation(nation), name, pos, Nation(tnation), tname, tpos ).."\n"..award, MailOnLinkClick )
			m_share_a_name = "["..Nation(nation).."]"..name;
			m_share_d_name = "["..Nation(tnation).."]"..tname;
		else -- 我是防御方
			local award = "";
			if win == 0 then
				award = F( 5519, name, 
									T(121).."x"..knum(recvValue.m_content_json["silver"]),
									T(122).."x"..knum(recvValue.m_content_json["wood"]),
									T(123).."x"..knum(recvValue.m_content_json["food"]),
									T(127).."x"..recvValue.m_content_json["people"] )
			end
			SetRichText( m_uiMailContent.transform:Find("Text"), F(5518, Nation(tnation), tname, tpos, Nation(nation), name, pos ).."\n"..award, MailOnLinkClick )
			m_share_a_name = "["..Nation(tnation).."]"..tname;
			m_share_d_name = "["..Nation(nation).."]"..name;
		end
	
	-- 国战
	elseif recvValue.m_type == MAIL_TYPE_FIGHT_NATION then	
		SetTrue( m_uiMailContent );
		--{"my":1,"win":0,"na":"令狐荧","n":2,"townid":81,"tn":0}
		local my = recvValue.m_content_json["my"];
		local win = recvValue.m_content_json["win"];
		local name = recvValue.m_content_json["na"];
		local nation = recvValue.m_content_json["n"];
		local pos = recvValue.m_content_json["pos"];
		local townid = recvValue.m_content_json["townid"];
		local tnation = recvValue.m_content_json["tn"];
		-- 解析一下名称
		if GetMail():IsTag( name, TAG_TOWNID ) then
			local textid = tonumber(string.sub(name, string.len(TAG_TOWNID) + 1));
			if textid ~= nil then
				name = MapTownName( textid )
			end
		end
		
		local tname = MapTownName( townid )
		local tpos = g_towninfo[townid].posx..","..g_towninfo[townid].posy

		if my == 1 then -- 我是攻击方
			local awardstr = "";
			if recvValue.m_content_json["silver"] ~= nil and recvValue.m_content_json["silver"] > 0 then
				awardstr = awardstr..T(121).."x"..knum(recvValue.m_content_json["silver"]).."\n"
			end
			if recvValue.m_content_json["wood"] ~= nil and recvValue.m_content_json["wood"] > 0 then
				awardstr = awardstr..T(122).."x"..knum(recvValue.m_content_json["wood"]).."\n"
			end
			if recvValue.m_content_json["food"] ~= nil and recvValue.m_content_json["food"] > 0 then
				awardstr = awardstr..T(123).."x"..knum(recvValue.m_content_json["food"]).."\n"
			end
			local award = "";
			if awardstr ~= "" then
				award = F( 5526, awardstr);
			end
			SetRichText( m_uiMailContent.transform:Find("Text"), F(5518, Nation(nation), name, pos, Nation(tnation), tname, tpos ).."\n"..award, MailOnLinkClick )
			m_share_a_name = "["..Nation(nation).."]"..name;
			m_share_d_name = "["..Nation(tnation).."]"..tname;
			
		else -- 我是防御方
			local awardstr = "";
			if recvValue.m_content_json["silver"] ~= nil and recvValue.m_content_json["silver"] > 0 then
				awardstr = awardstr..T(121).."x"..knum(recvValue.m_content_json["silver"]).."\n"
			end
			if recvValue.m_content_json["wood"] ~= nil and recvValue.m_content_json["wood"] > 0 then
				awardstr = awardstr..T(122).."x"..knum(recvValue.m_content_json["wood"]).."\n"
			end
			if recvValue.m_content_json["food"] ~= nil and recvValue.m_content_json["food"] > 0 then
				awardstr = awardstr..T(123).."x"..knum(recvValue.m_content_json["food"]).."\n"
			end
			local award = "";
			if awardstr ~= "" then
				award = F( 5526, awardstr);
			end
			SetRichText( m_uiMailContent.transform:Find("Text"), F(5518, Nation(nation), name, pos, Nation(tnation), tname, tpos ).."\n"..award, MailOnLinkClick )
			m_share_a_name = "["..Nation(nation).."]"..name;
			m_share_d_name = "["..Nation(tnation).."]"..tname;
		end
		
	else
		SetTrue( m_uiMailContent );
		
		local contentid = 0;
		local text = recvValue.m_content_json["text"];
		if GetMail():IsTag( text, TAG_TEXTID ) then
			contentid = tonumber(string.sub(text, string.len(TAG_TEXTID) + 1));
		end
		
		-- 系统信息邮件
		if recvValue.m_type == MAIL_TYPE_SYSTEM then
			if recvValue.m_content_json["v1"] ~= nil and recvValue.m_content_json["v2"] ~= nil and recvValue.m_content_json["v3"] ~= nil then
				local v1_str = GetMail():GetString( recvValue.m_content_json["v1"] );
				local v2_str = GetMail():GetString( recvValue.m_content_json["v2"] );
				local v3_str = GetMail():GetString( recvValue.m_content_json["v3"] );
				SetRichText( m_uiMailContent.transform:Find("Text"), F(contentid, v1_str, v2_str, v3_str), MailOnLinkClick )
				
			elseif recvValue.m_content_json["v1"] ~= nil and recvValue.m_content_json["v2"] ~= nil then
				local v1_str = GetMail():GetString( recvValue.m_content_json["v1"] );
				local v2_str = GetMail():GetString( recvValue.m_content_json["v2"] );
				SetRichText( m_uiMailContent.transform:Find("Text"), F(contentid, v1_str, v2_str), MailOnLinkClick )
				
			elseif recvValue.m_content_json["v1"] ~= nil then
				local v1_str = GetMail():GetString( recvValue.m_content_json["v1"] );
				SetRichText( m_uiMailContent.transform:Find("Text"), F(contentid, v1_str), MailOnLinkClick )
			else
				SetRichText( m_uiMailContent.transform:Find("Text"), T(contentid) )
			end

		-- 公告邮件，内容外部http服务器获取
		elseif recvValue.m_type == MAIL_TYPE_NOTIFY then
			SetRichText( m_uiMailContent.transform:Find("Text"), T(contentid) )
			
		-- 每日登录
		elseif recvValue.m_type == MAIL_TYPE_EVERYDAY then
			SetRichText( m_uiMailContent.transform:Find("Text"), T(contentid) )
			
		-- 流寇
		elseif recvValue.m_type == MAIL_TYPE_FIGHT_ENEMY then
			local win = recvValue.m_content_json["win"];
			local name = recvValue.m_content_json["name"];
			local level = recvValue.m_content_json["lv"];
			local pos = recvValue.m_content_json["pos"];
			local tpos = recvValue.m_content_json["tpos"];
			local enemyname = "Lv."..level.." "..T(938);
			if win == 1 then
				local awardstr = ""
				local award = recvValue.m_content_json["award"];
				local awardList = string.split( award, '@' )
				for i= 1, #awardList, 1 do
					local awardinfo = string.split( awardList[i], ',' )
					if #awardinfo == 2 then
						local kind = tonumber(awardinfo[1]);
						local num = tonumber(awardinfo[2]);
						local sprite, color, name = AwardInfo( kind )
						awardstr = awardstr .. name.."x"..num.." "
					end
				end
				SetRichText( m_uiMailContent.transform:Find("Text"), F(contentid, name, pos, enemyname, tpos, awardstr ), MailOnLinkClick )
			else
				SetRichText( m_uiMailContent.transform:Find("Text"), F(contentid, name, pos, enemyname, tpos ), MailOnLinkClick )
			end
			m_share_a_name = name;
			m_share_d_name = enemyname;
		end
	end

	
	-- 附件状态
	if recvValue.m_attach == "" then
		SetFalse( m_uiMailAttach );
	else
		SetTrue( m_uiMailAttach );
		local AttachList = string.split( recvValue.m_attach, '@' )
		for i= 1, #AttachList, 1 do
			local Attach = string.split( AttachList[i], ',' )
			if #Attach == 2 then
				local kind = tonumber(Attach[1]);
				local num = tonumber(Attach[2]);
				local sprite, color, name = AwardInfo( kind )
				local uiObj = m_ObjectPool:Get( "UIP_Item" );
				uiObj.transform:SetParent( m_uiMailAttach.transform );
				SetImage( uiObj.transform:Find("Shape"), sprite )
				SetImage( uiObj.transform:Find("Color"), color )
				SetText( uiObj.transform:Find("Name"), name.."x"..num )
			end
		end
		if recvValue.m_attachget == 1 then
			SetFalse( m_uiMailAttach.transform:Find("GetAttachBtn") );
		else
			SetTrue( m_uiMailAttach.transform:Find("GetAttachBtn") );
		end
	end
	
	-- 未读设已读
	if recvValue.m_read == 0 then
		MailInfoDlgRead();
	end
	
	-- 锁定状态
	if recvValue.m_lock == 0 and recvValue.m_viewpath == 0 then
		SetFalse( m_uiDialogFrameMod.transform:Find("Top/LockBtn") )
		SetTrue( m_uiDialogFrameMod.transform:Find("Top/UnLockBtn") )
		SetTrue( m_uiDialogFrameMod.transform:Find("Top/DeleteBtn") )
		SetFalse( m_uiDialogFrameMod.transform:Find("Top/NoDeleteBtn") )
	else
		SetTrue( m_uiDialogFrameMod.transform:Find("Top/LockBtn") )
		SetFalse( m_uiDialogFrameMod.transform:Find("Top/UnLockBtn") )
		SetFalse( m_uiDialogFrameMod.transform:Find("Top/DeleteBtn") )
		SetTrue( m_uiDialogFrameMod.transform:Find("Top/NoDeleteBtn") )
	end
	
	-- 获取战斗信息
	if recvValue.m_type == MAIL_TYPE_FIGHT_ENEMY or recvValue.m_type == MAIL_TYPE_FIGHT_CITY or recvValue.m_type == MAIL_TYPE_FIGHT_NATION or recvValue.m_type == MAIL_TYPE_GATHER_FIGHT or ( recvValue.m_type == MAIL_TYPE_CITY_SPY and m_spy_hashero == 1) then
		-- 没缓存，去服务器拿
		if recvValue.m_fight_content == nil or recvValue.m_fight_content == "" then
			-- 自己看自己的邮件
			if recvValue.m_viewpath == 0 then			
				local sendValue = {};
				sendValue.m_op = 7;
				sendValue.m_mailid = recvValue.m_mailid;
				netsend_mailop_C( sendValue )
			
			-- 查看别人邮件
			else		
				local sendValue = {};
				sendValue.m_op = 9;
				sendValue.m_mailid = recvValue.m_mailid;
				netsend_mailop_C( sendValue )
			end
		else
			-- 有缓存，直接用
			MailInfoDlgRecvFight( recvValue.m_mailid, recvValue.m_fight_content )
		end
		-- 分享邮件 战斗回放
		if recvValue.m_viewpath == 0 then
			SetTrue( m_uiShareBtn )
			if recvValue.m_type == MAIL_TYPE_CITY_SPY then
				SetFalse( m_uiRecBtn )
			else
				SetTrue( m_uiRecBtn )
			end
		else
			SetFalse( m_uiShareBtn )
			if recvValue.m_type == MAIL_TYPE_CITY_SPY then
				SetFalse( m_uiRecBtn )
			else
				SetTrue( m_uiRecBtn )
			end
		end
		SetFalse( m_uiDefriendBtn )
		SetFalse( m_uiReplyBtn )
		
	elseif recvValue.m_type == MAIL_TYPE_ACTOR_SEND or recvValue.m_type == MAIL_TYPE_ACTOR_REPLY then
		SetFalse( m_uiShareBtn )
		SetFalse( m_uiRecBtn )
		SetTrue( m_uiDefriendBtn )
		SetTrue( m_uiReplyBtn )
	else
		SetFalse( m_uiShareBtn )
		SetFalse( m_uiRecBtn )
		SetFalse( m_uiDefriendBtn )
		SetFalse( m_uiReplyBtn )
	end
end

-- 接收战斗信息
function MailInfoDlgRecvFight( mailid, content )
	if int64.equals( m_recvValue.m_mailid, mailid ) == false then
		return;
	end
	m_recvValue.m_fight_content = content;
	
	if m_recvValue.m_type == MAIL_TYPE_CITY_SPY then
		local info = json.decode( content );
		SetTrue( m_uiSpyHeroContent );
		
		-- 武将
		if info["heros"] ~= nil then
			for tmpi = 1, #info["heros"], 1 do
				local pHero = info["heros"][tmpi];
				local uiObj = m_ObjectPool:Get( "UIP_SpyUnit" );
				uiObj.transform:SetParent( m_uiSpyHeroContent.transform );
				SetImage( uiObj.transform:Find("Shape"), HeroHeadSprite( pHero["kd"] ) );
				SetImage( uiObj.transform:Find("Color"), ItemColorSprite( pHero["cr"] ) );
				SetImage( uiObj.transform:Find("Corps"), CorpsSprite( pHero["cs"] ) );
				SetText( uiObj.transform:Find("Name"), HeroName( pHero["kd"] )..MailInfoDlgSpyArmyState(pHero) );
				SetText( uiObj.transform:Find("Level"), "Lv."..pHero["lv"] );
				SetText( uiObj.transform:Find("Desc"), T(1117).." "..pHero["hp"] );
			end
		end
		-- 守卫
		for tmpi = 1, #info["guards"], 1 do
			local pHero = info["guards"][tmpi];
			local uiObj = m_ObjectPool:Get( "UIP_SpyUnit" );
			uiObj.transform:SetParent( m_uiSpyHeroContent.transform );
			SetImage( uiObj.transform:Find("Shape"), GuardSprite( pHero["sp"] ) );
			SetImage( uiObj.transform:Find("Color"), ItemColorSprite( pHero["cr"] ) );
			SetImage( uiObj.transform:Find("Corps"), CorpsSprite( pHero["cs"] ) );
			SetText( uiObj.transform:Find("Name"), T(1119) );
			SetText( uiObj.transform:Find("Level"), "Lv."..pHero["lv"] );
			SetText( uiObj.transform:Find("Desc"), T(1117).." "..pHero["hp"] );
		end
		
	else
		SetTrue( m_uiMailFight );
	
		local info = json.decode( content );
		
		-- 我是攻击方，攻击方显示左面
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
		end
		
	end			
end

-- 设置双方主角信息
function MailInfoDlgSetFightMainUnit( uiObj, type, name, shape, nation, maxhp, hp )
	-- 玩家 部队 资源点
	if type == MAPUNIT_TYPE_CITY or type == MAPUNIT_TYPE_ARMY or type == MAPUNIT_TYPE_RES then
		SetImage( uiObj.transform:Find("Shape"), PlayerHeadSprite( shape ) );
		SetText( uiObj.transform:Find("Name"), "["..Nation(nation).."]"..name );
			
	-- 城镇
	elseif type == MAPUNIT_TYPE_TOWN then
		SetImage( uiObj.transform:Find("Shape"), EnemyHeadSprite( shape ) );
		SetText( uiObj.transform:Find("Name"), EnemyName( tonumber(name) ) );
		
	-- 流寇
	elseif type == MAPUNIT_TYPE_ENEMY then
		SetImage( uiObj.transform:Find("Shape"), EnemyHeadSprite( shape ) );
		SetText( uiObj.transform:Find("Name"), EnemyName( tonumber(name) ) );
	end
	
	-- 血量
	local lost = maxhp-hp;
	if lost > maxhp then
		lost = maxhp
	end
	SetText( uiObj.transform:Find("Hp"), T(1117).." "..maxhp );
	SetText( uiObj.transform:Find("Lost"), T(1118).." "..lost );
end

-- 设置双方战斗单元信息
function MailInfoDlgSetFightUnit( uiObj, type, nation, name, kind, shape, level, color, corps, maxhp, hp, damage, vw )
	
	-- 玩家英雄	
	if type == FIGHT_UNITTYPE_HERO then
		SetImage( uiObj.transform:Find("Shape"), HeroHeadSprite( kind ) );
		SetImage( uiObj.transform:Find("Color"), ItemColorSprite( color ) );
		SetImage( uiObj.transform:Find("Corps"), CorpsSprite( corps ) );
		if name and name ~= "" then
			SetText( uiObj.transform:Find("Name"), "["..name.."]"..HeroName( kind ) );
		else
			SetText( uiObj.transform:Find("Name"), HeroName( kind ) );
		end
		SetText( uiObj.transform:Find("Level"), "Lv."..level );
		SetText( uiObj.transform:Find("Desc"), T(1117).." "..maxhp.." "..T(152).."+"..vw );
		
	-- 玩家城墙守卫
	elseif type == FIGHT_UNITTYPE_GUARD then
		SetImage( uiObj.transform:Find("Shape"), GuardSprite( shape ) );
		SetImage( uiObj.transform:Find("Color"), ItemColorSprite( color ) );
		SetImage( uiObj.transform:Find("Corps"), CorpsSprite( corps ) );
		SetText( uiObj.transform:Find("Name"), T( 1119 ) );
		SetText( uiObj.transform:Find("Level"), "Lv."..level );
		SetText( uiObj.transform:Find("Desc"), T(1117).." "..maxhp );
		
	-- 怪物表
	elseif type == FIGHT_UNITTYPE_MONSTER then
		SetImage( uiObj.transform:Find("Shape"), EnemyHeadSprite( shape ) );
		SetImage( uiObj.transform:Find("Color"), ItemColorSprite( color ) );
		SetImage( uiObj.transform:Find("Corps"), CorpsSprite( corps ) );
		SetText( uiObj.transform:Find("Name"), EnemyName( kind ) );
		SetText( uiObj.transform:Find("Level"), "Lv."..level );
		SetText( uiObj.transform:Find("Desc"), T(1117).." "..maxhp );
	end
end

-- 清空附件
function MailInfoDlgAttachClear()
	local objs = {};
	for i = 0 ,m_uiMailAttach.transform.childCount - 1 do
		table.insert( objs, m_uiMailAttach.transform:GetChild(i).gameObject )
    end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "UIP_Item(Clone)" then
			m_ObjectPool:Release( "UIP_Item", obj );
		end
    end
end

-- 清空战斗单元
function MailInfoDlgFightUnitClear()
	local objs = {};
	for i = 0 ,m_uiMailFight.transform.childCount - 1 do
		table.insert( objs, m_uiMailFight.transform:GetChild(i).gameObject )
    end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "UIP_UnitRow(Clone)" then
			m_ObjectPool:Release( "UIP_UnitRow", obj );
		end
    end
end

-- 清空侦察单元
function MailInfoDlgSpyUnitClear()
	local objs = {};
	for i = 0 ,m_uiSpyHeroContent.transform.childCount - 1 do
		table.insert( objs, m_uiSpyHeroContent.transform:GetChild(i).gameObject )
    end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "UIP_SpyUnit(Clone)" then
			m_ObjectPool:Release( "UIP_SpyUnit", obj );
		end
    end
end

-- 坐标点击
function MailOnLinkClick( str )
	local pos = string.split(str, ',')
	local gameCoorX = tonumber( pos[1] );
	local gameCoorY = tonumber( pos[2] );
	if gameCoorX >= 0 and gameCoorY >= 0 then 
		MailInfoDlgClose()
		MailDlgClose()
		ChatDlgClose()
		MapArmyGroupDlgClose()
		WorldMap.GotoCoor( gameCoorX, gameCoorY )
	end
end

-- 设置已读状态
function MailInfoDlgRead()
	if m_recvValue == nil then
		return
	end
	if m_recvValue.m_actorid ~= GetPlayer().m_actorid then
		return;
	end
	m_recvValue.m_read = 1;
	-- m_op=0,m_mailid=0,
	local sendValue = {};
	sendValue.m_op = 1;
	sendValue.m_mailid = m_recvValue.m_mailid;
	netsend_mailop_C( sendValue )
end

-- 领取附件
function MailInfoDlgAttachGet()
	if m_recvValue == nil then
		return
	end
	if m_recvValue.m_attachget == 1 then
		return	
	end
	if m_recvValue.m_actorid ~= GetPlayer().m_actorid then
		return;
	end	
	m_recvValue.m_attachget = 1;
	SetFalse( m_uiMailAttach.transform:Find("GetAttachBtn") );
	
	-- m_op=0,m_mailid=0,
	local sendValue = {};
	sendValue.m_op = 2;
	sendValue.m_mailid = m_recvValue.m_mailid;
	netsend_mailop_C( sendValue )
end

-- 解锁
function MailInfoDlgUnLock()
	if m_recvValue == nil then
		return
	end
	if m_recvValue.m_lock == 0 then
		return	
	end
	if m_recvValue.m_actorid ~= GetPlayer().m_actorid then
		return;
	end
	m_recvValue.m_lock = 0;
	
	SetFalse( m_uiDialogFrameMod.transform:Find("Top/LockBtn") )
	SetTrue( m_uiDialogFrameMod.transform:Find("Top/UnLockBtn") )
	SetTrue( m_uiDialogFrameMod.transform:Find("Top/DeleteBtn") )
	SetFalse( m_uiDialogFrameMod.transform:Find("Top/NoDeleteBtn") )
	
	-- m_op=0,m_mailid=0,
	local sendValue = {};
	sendValue.m_op = 6;
	sendValue.m_mailid = m_recvValue.m_mailid;
	netsend_mailop_C( sendValue )
end

-- 锁定
function MailInfoDlgLock()
	if m_recvValue == nil then
		return
	end
	if m_recvValue.m_lock == 1 then
		return	
	end
	if m_recvValue.m_actorid ~= GetPlayer().m_actorid then
		return;
	end
	
	m_recvValue.m_lock = 1;
	
	SetTrue( m_uiDialogFrameMod.transform:Find("Top/LockBtn") )
	SetFalse( m_uiDialogFrameMod.transform:Find("Top/UnLockBtn") )
	SetFalse( m_uiDialogFrameMod.transform:Find("Top/DeleteBtn") )
	SetTrue( m_uiDialogFrameMod.transform:Find("Top/NoDeleteBtn") )
	
	-- m_op=0,m_mailid=0,
	local sendValue = {};
	sendValue.m_op = 5;
	sendValue.m_mailid = m_recvValue.m_mailid;
	netsend_mailop_C( sendValue )
end

-- 删除邮件
function MailInfoDlgDelete()
	if m_recvValue == nil then
		return
	end
	if m_recvValue.m_attach ~= "" then
		if m_recvValue.m_attachget == 0 then
			AlertMsg( T(1113) )
			return
		end
	end
	if m_recvValue.m_actorid ~= GetPlayer().m_actorid then
		return;
	end
	
	-- 本地删除
	local endIndex = #GetMail().m_Mails;
	for i = 1, endIndex, 1 do
		if int64.equals( GetMail().m_Mails[i].m_mailid, m_recvValue.m_mailid ) then
			table.remove( GetMail().m_Mails, i );
			break;
		end
	end
	
	-- m_op=0,m_mailid=0,
	local sendValue = {};
	sendValue.m_op = 3;
	sendValue.m_mailid = m_recvValue.m_mailid;
	netsend_mailop_C( sendValue )
	
	MailInfoDlgClose()
end

-- 分享邮件
function MailInfoDlgShare()
	if m_recvValue.m_actorid ~= GetPlayer().m_actorid then
		return;
	end
	-- m_mailid=0,m_a_name_len=0,m_a_name="[m_a_name_len]",m_d_name_len=0,m_d_name="[m_d_name_len]",m_type=0,
	local sendValue = {}
	sendValue.m_mailid = m_recvValue.m_mailid;
	sendValue.m_type = m_recvValue.m_type;
	sendValue.m_a_name = m_share_a_name;
	sendValue.m_a_name_len = string.len( m_share_a_name );
	sendValue.m_d_name = m_share_d_name;
	sendValue.m_d_name_len = string.len( m_share_d_name );
	netsend_mailshare_C( sendValue )
end

-- 侦察武将当前状态
function MailInfoDlgSpyArmyState( pHero )
	local name = "";
	if pHero["state"] == 0 then
		name = T(1182); -- 驻防
	else
		if pHero["armystate"] == ARMY_STATE_MARCH then --行军
			local toname = "";
			if pHero["totype"] == MAPUNIT_TYPE_CITY then
				toname = pHero["toname"];
				
			elseif pHero["totype"] == MAPUNIT_TYPE_TOWN then
				local tokind = pHero["tokind"]; 
				toname = T(1183)
				
			elseif pHero["totype"] == MAPUNIT_TYPE_ENEMY then
				local tokind = pHero["tokind"];
				local info = g_enemyinfo[tokind];
				if info == nil then
					toname = T(1183)
				else
					toname = "Lv."..info.level..T(938)
				end
			
			elseif pHero["totype"] == MAPUNIT_TYPE_RES then
				local tokind = pHero["tokind"];
				local info = g_resinfo[tokind];
				if info == nil then
					toname = T(1183)
				else
					toname = "Lv."..info.level..T(MapUnitResNameList[info.type])
				end
				
			end
			
			name = F( 1180, math.ceil( pHero["armytime"]/60 ), toname );
			
		elseif pHero["armystate"] == ARMY_STATE_REBACK then -- 返程
			name = F( 1181, math.ceil( pHero["armytime"]/60 ) );
			
		elseif pHero["armystate"] == ARMY_STATE_GATHER then -- 采集
			local tokind = pHero["tokind"];
			local info = g_resinfo[tokind];
			if info == nil then
				name = F( 1179, 0, T(1183) )
			else
				name = F( 1179, info.level, T(MapUnitResNameList[info.type]) )
			end
			
		else
			name = T(1183); -- 未知
		end
	end
	return "("..name..")";
end

