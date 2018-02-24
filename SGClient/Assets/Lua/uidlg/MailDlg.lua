-- 界面
local json = require "cjson"
local m_Dlg = nil;
local m_DialogFrameMod = nil;
local m_uiListLayer = nil; --UnityEngine.GameObject
local m_uiListScroll = nil; --UnityEngine.GameObject
local m_uiListContent = nil; --UnityEngine.GameObject
local m_uiUIP_Mail = nil; --UnityEngine.GameObject
local m_uiAllDelBtn = nil; --UnityEngine.GameObject
local m_uiAllReadBtn = nil; --UnityEngine.GameObject
local m_uiCancelBtn = nil; --UnityEngine.GameObject
local m_uiDelBtn = nil; --UnityEngine.GameObject

local m_ObjectPool = nil;
local m_CurrPage = 0;
local m_TotalPage = 0;
local m_PageDirection = 0;
local m_IsShowLoadingItem = false;
local m_uiCache = {};
local m_AllDelState = 0;

-- 打开界面
function MailDlgOpen()
	m_Dlg = eye.uiManager:Open( "MailDlg" );
	m_DialogFrameMod = DialogFrameModOpen( m_Dlg, T(554), HELP_MailDlg, MailDlgClose );
end

-- 隐藏界面
function MailDlgClose()
	if m_Dlg == nil then
		return;
	end
	m_CurrPage = 0;
	DialogFrameModClose( m_DialogFrameMod );
	m_DialogFrameMod = nil;
	eye.uiManager:Close( "MailDlg" );
end

-- 删除界面
function MailDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function MailDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            MailDlgClose();
			
		-- 选择邮件
		elseif nControlID >= 1 and nControlID <= 100000 then
			MailDlgSelect( nControlID )
		
		-- 选择批量删除的项
		elseif nControlID > 100000 and nControlID <= 200000 then
			MailDlgAllDeleteToggle( nControlID-100000 )
			
		-- 批量删除
		elseif nControlID == 300001 then
			MailDlgAllDeleteState();
			
		-- 全部已读
		elseif nControlID == 300002 then
			MailDlgAllRead();
			
		-- 取消操作
		elseif nControlID == 300003 then
			MailDlgAllDeleteCancel();
			
		-- 删除
		elseif nControlID == 300004 then
			MailDlgAllDeleteOK();
        end
		
	elseif nType == UI_EVENT_SCROLLDRAG then
	
		if value == 0 then
            -- 判断列表是否允许滑动
            m_IsShowLoadingItem = m_uiListScroll:GetComponent("UIScrollRect"):IsShowLoading();
		
		-- 滚动条超过一定的范围,这个时候访问数据,读取更多数据	
        elseif value == 1 and m_IsShowLoadingItem == false then
            local vn = m_uiListScroll:GetComponent("UIScrollRect").verticalNormalizedPosition;
            local rectTrans = m_uiListContent:GetComponent("RectTransform");
            local scrollHeight = m_uiListScroll:GetComponent("RectTransform").rect.height;
            local maxHeight =(math.max(scrollHeight, rectTrans.rect.height) - rectTrans.offsetMax.y);
            if maxHeight < 0.9 * scrollHeight then
                -- 增加一个提示用户读取数据的 一行
                m_uiListScroll:GetComponent("UIScrollRect"):ShowLoading(false, 2.5);
                m_IsShowLoadingItem = true;
				m_PageDirection = 1;
				
			elseif vn >= 1 and math.abs(rectTrans.offsetMax.y) > 0.1 * scrollHeight then
                -- 增加一个提示用户读取数据的 一行
                --m_uiListScroll:GetComponent("UIScrollRect"):ShowLoading(true, 2.5);
                m_IsShowLoadingItem = true;
				m_PageDirection = 2;
            end
         
        -- 拖动放开
        elseif value == 2 then
            m_uiListScroll:GetComponent("UIScrollRect").enableDrag = true;
            --if m_IsShowLoadingItem then                    
				m_uiListScroll:GetComponent("UIScrollRect").enabled = false;
                m_uiListScroll:GetComponent("UIScrollRect").enabled = true;
                -- 加载更多数据
				
				-- 下一页
				if m_PageDirection == 1 then
					-- 所有历史邮件都获取全了，就不取历史的了
					if GetMail().m_bLoadAll == true then
						local isLoading = true;
						if m_IsShowLoadingItem == true then
							m_uiListScroll:GetComponent("UIScrollRect"):HideLoading();
							m_IsShowLoadingItem = false;
							isLoading = false;
						end
						MailDlgMailPageNext();
						if isLoading then
							m_uiListScroll:GetComponent("UIScrollRect"):ResetScroll();
						end
					
					else
						-- 获取过首页数据，并且历史邮件没获取全，就获取
						if GetMail().m_bLoadNew == true then
							-- m_minid=0,m_maxid=0,,m_op=0,
							local sendValue ={};
							sendValue.m_op = 1
							sendValue.m_minid = GetMail().m_nLastRecvMinMailID;
							sendValue.m_maxid = GetMail().m_nMaxMailID;
							netsend_mailask_C( sendValue )
						end
					end
					
				-- 上一页
				elseif m_PageDirection == 2 then 
					MailDlgMailPagePre();
				end
            --end
        end
        
	end
end

-- 载入时调用
function MailDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiListLayer = objs[0];
	m_uiListScroll = objs[1];
	m_uiListContent = objs[2];
	m_uiUIP_Mail = objs[3];
	m_uiAllDelBtn = objs[4];
	m_uiAllReadBtn = objs[5];
	m_uiCancelBtn = objs[6];
	m_uiDelBtn = objs[7];
	
	-- 对象池
	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );
	m_ObjectPool:CreatePool("UIP_Mail", 6, 6, m_uiUIP_Mail);
end

-- 界面初始化时调用
function MailDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function MailDlgOnEnable( gameObject )
	if m_CurrPage == 0 then
		return
	end
	-- 刷新
	MailDlgMailPageUpdate();
end

-- 界面隐藏时调用
function MailDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function MailDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function MailDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function MailDlgShow()
	MailDlgOpen()
	SetTrue( m_uiAllDelBtn );
	SetTrue( m_uiAllReadBtn );
	SetFalse( m_uiCancelBtn );
	SetFalse( m_uiDelBtn );


	m_uiListScroll:GetComponent("UIScrollRect"):HideLoading();
    m_IsShowLoadingItem = false;
	m_AllDelState = 0;
	m_uiCache = {};
	
	-- 当前页
	m_CurrPage = 0;
	
	-- 首次获取一次最新数据
	if GetMail().m_bLoadNew == false then
		-- m_minid=0,m_maxid=0,,m_op=0,
		local sendValue ={};
		sendValue.m_op = 0
		sendValue.m_minid = GetMail().m_nLastRecvMinMailID;
		sendValue.m_maxid = GetMail().m_nMaxMailID;
		netsend_mailask_C( sendValue )
		return
	end
	
	-- 总页数 1页6条数据
	m_TotalPage = math.ceil( #GetMail().m_Mails/6 )
	-- 非首次打开
	MailDlgSetMailPage( 1 )
end

-- 设置列表
function MailDlgSetMailPage( nPage )	
	-- 对象池还原
	MailDlgMailPageClear();
	m_CurrPage = nPage;
	-- 该页起始索引
	local beginIndex = #GetMail().m_Mails - 6*(m_CurrPage-1);
	-- 该页结束索引
	local endIndex = beginIndex-5;
	if endIndex < 1 then
		endIndex = 1;
		--beginIndex = 6;
	end
	--print ( beginIndex..","..endIndex )
	
	-- 创建条目
	for i = beginIndex, endIndex, -1 do
		if GetMail().m_Mails[i] == nil then
			break;
		end
		MailDlgSetMail( GetMail().m_Mails[i] )
	end
end

-- 下一页
function MailDlgMailPageNext()
	local nPage = m_CurrPage;
	if nPage >= m_TotalPage then
		pop(T(1102))
		return
	end
	nPage = nPage + 1;
	MailDlgSetMailPage( nPage )
end

-- 上一页
function MailDlgMailPagePre()
	local nPage = m_CurrPage;
	if nPage <= 1 then
		pop(T(1103))
		return
	end
	nPage = nPage - 1;
	MailDlgSetMailPage( nPage )
end

-- 刷新 
function MailDlgMailPageUpdate()
	-- 总页数 1页6条数据
	m_TotalPage = math.ceil( #GetMail().m_Mails/6 )
	if m_CurrPage > m_TotalPage then
		m_CurrPage = m_TotalPage;
	end
	MailDlgSetMailPage( m_CurrPage )
end

-- 设置一条邮件
function MailDlgSetMail( recvValue )
	local uiObj = m_ObjectPool:Get( "UIP_Mail" );
	uiObj.transform:SetParent( m_uiListContent.transform );
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiShape = objs[0];
	local uiTitle = objs[1];
	local uiRecvTime = objs[2];
	local uiContent = objs[3];
	local uiRead = objs[4];
	local uiAttach = objs[5];
	local uiLock = objs[6];
	local uiToggle = objs[7];
	SetControlID( uiObj, recvValue.m_incrementid )
	m_uiCache[recvValue.m_incrementid] = uiObj
	
	-- 解析内容
	-- 玩家间
	if recvValue.m_type == MAIL_TYPE_ACTOR_SEND or recvValue.m_type == MAIL_TYPE_ACTOR_REPLY then
		local from_actorid = recvValue.m_content_json["fromid"];
		local msg = recvValue.m_content_json["msg"];
		SetRichText( uiContent, msg )
		SetImage( uiShape, LoadSprite("ui_mail_icon_1") )
		
	-- 采集
	elseif recvValue.m_type == MAIL_TYPE_GATHER then
		local restype = recvValue.m_content_json["res"];
		local resnum = recvValue.m_content_json["num"];
		SetRichText( uiContent, F( 5506, ResName( restype ).."x"..resnum ) )
		SetImage( uiShape, LoadSprite("ui_mail_icon_2") )
	
	-- 采集战斗
	elseif recvValue.m_type == MAIL_TYPE_GATHER_FIGHT then
		local win = recvValue.m_content_json["win"];
		local name = recvValue.m_content_json["na"];
		local tname = recvValue.m_content_json["tna"];
		if win == 1 then
			SetRichText( uiContent, F(1110, name, tname ) )
		else
			SetRichText( uiContent, F(1111, name, tname ) )
		end
		SetImage( uiShape, LoadSprite("ui_mail_icon_8") )
		
	-- 侦察
	elseif recvValue.m_type == MAIL_TYPE_CITY_SPY then
		local flag = recvValue.m_content_json["flag"];
		local nation = recvValue.m_content_json["n"];
		local level = recvValue.m_content_json["lv"];
		local name = recvValue.m_content_json["na"];
		local pos = recvValue.m_content_json["pos"];
		SetRichText( uiContent, F( 5511, Nation(nation), level, name, pos ) );
		SetImage( uiShape, LoadSprite("ui_mail_icon_3") )
		
	-- 被侦察
	elseif recvValue.m_type == MAIL_TYPE_CITY_BESPY then
		local flag = recvValue.m_content_json["flag"];
		local nation = recvValue.m_content_json["n"];
		local level = recvValue.m_content_json["lv"];
		local name = recvValue.m_content_json["na"];
		local pos = recvValue.m_content_json["pos"];
		SetRichText( uiContent, F( 5513, Nation(nation), level, name, pos ) );
		SetImage( uiShape, LoadSprite("ui_mail_icon_3") )
	
	-- 城战
	elseif recvValue.m_type == MAIL_TYPE_FIGHT_CITY then
		local win = recvValue.m_content_json["win"];
		local name = recvValue.m_content_json["na"];
		local tname = recvValue.m_content_json["tna"];
		if win == 1 then
			SetRichText( uiContent, F(1110, name, tname ) )
			SetImage( uiShape, LoadSprite("ui_mail_icon_6") )
		else
			SetRichText( uiContent, F(1111, name, tname ) )
			SetImage( uiShape, LoadSprite("ui_mail_icon_3") )
		end
		
	-- 国战
	elseif recvValue.m_type == MAIL_TYPE_FIGHT_NATION then
		local win = recvValue.m_content_json["win"];
		local name = recvValue.m_content_json["na"];
		local townid = recvValue.m_content_json["townid"];
		local tn = recvValue.m_content_json["tn"];
		if win == 1 then
			SetRichText( uiContent, F(1110, name, "["..Nation(tn).."]"..MapTownName(townid) ) )
			SetImage( uiShape, LoadSprite("ui_mail_icon_4") )
		else
			SetRichText( uiContent, F(1111, name, "["..Nation(tn).."]"..MapTownName(townid) ) )
			SetImage( uiShape, LoadSprite("ui_mail_icon_7") )
		end
	else
		-- 解析内容
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
				SetRichText( uiContent, F(contentid, v1_str, v2_str, v3_str) )
				
			elseif recvValue.m_content_json["v1"] ~= nil and recvValue.m_content_json["v2"] ~= nil then
				local v1_str = GetMail():GetString( recvValue.m_content_json["v1"] );
				local v2_str = GetMail():GetString( recvValue.m_content_json["v2"] );
				SetRichText( uiContent, F(contentid, v1_str, v2_str) )
				
			elseif recvValue.m_content_json["v1"] ~= nil then
				local v1_str = GetMail():GetString( recvValue.m_content_json["v1"] );
				SetRichText( uiContent, F(contentid, v1_str) )
			else
				SetRichText( uiContent, T(contentid) )
			end
			
		-- 公告邮件，内容外部http服务器获取
		elseif recvValue.m_type == MAIL_TYPE_NOTIFY then
			SetRichText( uiContent, T(contentid) )
			SetImage( uiShape, LoadSprite("ui_mail_icon_2") )
			
		-- 每日登录
		elseif recvValue.m_type == MAIL_TYPE_EVERYDAY then
			SetRichText( uiContent, T(contentid) )
			SetImage( uiShape, LoadSprite("ui_mail_icon_1") )
			
		-- 流寇
		elseif recvValue.m_type == MAIL_TYPE_FIGHT_ENEMY then
			local win = recvValue.m_content_json["win"];
			local name = recvValue.m_content_json["name"];
			local kind = recvValue.m_content_json["kind"];
			local level = recvValue.m_content_json["lv"];
			local pos = recvValue.m_content_json["pos"];
			local tpos = recvValue.m_content_json["tpos"];
			local enemyname = ""
			if kind ~= nil then
				if g_enemyinfo[kind].nameid > 0 then
					enemyname = "Lv."..level.." "..T(g_enemyinfo[kind].nameid);
				else
					enemyname = "Lv."..level.." "..T(938);
				end
			else
				enemyname = "Lv."..level.." "..T(938);
			end
			if win == 1 then
				SetRichText( uiContent, F(1110, name, enemyname ) )
			else
				SetRichText( uiContent, F(1111, name, enemyname ) )
			end
			SetImage( uiShape, LoadSprite("ui_mail_icon_5") )
		
		-- 国家名将
		elseif recvValue.m_type == MAIL_TYPE_FIGHT_NATIONHERO then
			local win = recvValue.m_content_json["win"];
			local name = recvValue.m_content_json["name"];
			local kind = recvValue.m_content_json["kind"];
			local level = recvValue.m_content_json["lv"];
			local pos = recvValue.m_content_json["pos"];
			local tpos = recvValue.m_content_json["tpos"];
			local heroname = "Lv."..level.." "..Nation( g_nation_heroinfo[kind].nation ).."·"..HeroName(kind)
			if win == 1 then
				SetRichText( uiContent, F(1110, name, heroname ) )
			else
				SetRichText( uiContent, F(1111, name, heroname ) )
			end
			SetImage( uiShape, LoadSprite("ui_mail_icon_2") )
		end
	end

	
	-- 解析标题
	local title = "";
	if recvValue.m_type == MAIL_TYPE_ACTOR_SEND or recvValue.m_type == MAIL_TYPE_ACTOR_REPLY then
		local title_json = json.decode( recvValue.m_title );
		title = F( 5004, Nation( title_json["n"] ), title_json["lv"], title_json["na"] );
		
	else
		-- 其它类型解析
		if GetMail():IsTag( recvValue.m_title, TAG_TEXTID ) then
			local textid = tonumber(string.sub(recvValue.m_title, string.len(TAG_TEXTID) + 1));
			if textid ~= nil then
				title = T(textid)
			end
		else
			title = recvValue.m_title;
		end
	end
	SetText( uiTitle, title )

	-- 接收时间
	SetText( uiRecvTime, os.date( "%m-%d %H:%M", recvValue.m_recvtime ) )
	
	-- 读取状态
	if recvValue.m_read == 1 then
		SetFalse( uiRead );
	else
		SetTrue( uiRead );
	end
	
	-- 锁定状态
	if recvValue.m_lock == 1 then
		SetTrue( uiLock );
	else
		SetFalse( uiLock );
	end
	
	-- 批量删除
	recvValue.m_delete_toggle = 0
	if m_AllDelState == 1 then
		SetFalse( uiAttach );
		SetTrue( uiToggle );
		SetControlID( uiToggle, 100000 + recvValue.m_incrementid )

		-- 附件状态
		if recvValue.m_attach == "" or recvValue.m_attachget == 1 then
			if recvValue.m_lock == 1 then
				SetImage( uiToggle.transform:Find("Back"), LoadSprite("ui_button_select3") )
				recvValue.m_delete_toggle = 0
			else
				SetImage( uiToggle.transform:Find("Back"), LoadSprite("ui_button_select4") )
				recvValue.m_delete_toggle = 1
			end
	
		else
			SetImage( uiToggle.transform:Find("Back"), LoadSprite("ui_button_select3") )
			recvValue.m_delete_toggle = 0
		end
		
	else
		SetFalse( uiToggle );
		-- 附件状态
		if recvValue.m_attach == "" or recvValue.m_attachget == 1 then
			SetFalse( uiAttach );
		else
			SetTrue( uiAttach );
		end
	end
	
end

-- 清空一页
function MailDlgMailPageClear()
	local objs = {};
	for i = 0 ,m_uiListContent.transform.childCount - 1 do
		table.insert( objs, m_uiListContent.transform:GetChild(i).gameObject )
    end
	for k, v in pairs(objs) do
		local obj = v;
		if obj.name == "UIP_Mail(Clone)" then
			m_ObjectPool:Release( "UIP_Mail", obj );
		end
    end
end


-- 接收到数据
function MailDlgMailRecvOver( op, count )
    if m_Dlg == nil or m_Dlg.activeInHierarchy == false then
       return
    end
	
    local isLoading = true;
    if m_IsShowLoadingItem == true then
        m_uiListScroll:GetComponent("UIScrollRect"):HideLoading();
        m_IsShowLoadingItem = false;
        isLoading = false;
    end
	
	-- 首次打开
	if op == 0 then
		GetMail().m_bLoadNew = true;
		MailDlgSetMailPage( 1 )
	elseif op == 1 then
		-- 没有了，就证明历史邮件获取全了
		if count == 0 then
			GetMail().m_bLoadAll  = true;
		end
		-- 总页数 1页6条数据
		m_TotalPage = math.ceil( #GetMail().m_Mails/6 )
		MailDlgMailPageNext()
	end
	
    if isLoading then
        m_uiListScroll:GetComponent("UIScrollRect"):ResetScroll();
    end
end

-- 选择邮件
function MailDlgSelect( incrementid )
	if m_AllDelState == 1 then
		return
	end
	MailInfoDlgByIncrementID( incrementid )
end

-- 设置全部已读状态
function MailDlgAllRead()
	if GetMail().m_nNoReadCount <= 0 then
		pop( T(1112) )
		return
	end
	for i = 1, #GetMail().m_Mails, 1 do
		GetMail().m_Mails[i].m_read = 1;
	end
	-- 刷新 
	MailDlgMailPageUpdate();
	
	-- m_op=0,m_mailid=0,
	local sendValue = {};
	sendValue.m_op = 4;
	sendValue.m_mailid = 0;
	netsend_mailop_C( sendValue )
end

-- 选择批量删除的项
function MailDlgAllDeleteToggle( incrementid )
	local uiObj = m_uiCache[incrementid]	
	if uiObj == nil then
		return
	end
	uiObj.transform:SetParent( m_uiListContent.transform );
	local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
	local uiToggle = objs[7];
	
	
	local recvValue = nil;
	local endIndex = #GetMail().m_Mails;
	for i = 1, endIndex, 1 do
		if GetMail().m_Mails[i].m_incrementid == incrementid then
			recvValue = GetMail().m_Mails[i];
			break;
		end
	end
	
	-- 附件状态
	if recvValue.m_attach == "" or recvValue.m_attachget == 1 then
		if recvValue.m_lock == 1 then
			pop( T(1116) )
		else
			if recvValue.m_delete_toggle == 0 then
				SetImage( uiToggle.transform:Find("Back"), LoadSprite("ui_button_select4") )
				recvValue.m_delete_toggle = 1
			else
				SetImage( uiToggle.transform:Find("Back"), LoadSprite("ui_button_select3") )
				recvValue.m_delete_toggle = 0
			end
		end

	else
		pop( T(1113) )
	end
end

-- 批量删除
function MailDlgAllDeleteState()
	m_AllDelState = 1;
	SetFalse( m_uiAllDelBtn );
	SetFalse( m_uiAllReadBtn );
	SetTrue( m_uiCancelBtn );
	SetTrue( m_uiDelBtn );
	MailDlgMailPageUpdate()
end

-- 批量删除取消
function MailDlgAllDeleteCancel()
	m_AllDelState = 0;
	SetTrue( m_uiAllDelBtn );
	SetTrue( m_uiAllReadBtn );
	SetFalse( m_uiCancelBtn );
	SetFalse( m_uiDelBtn );
	MailDlgMailPageUpdate()
end

-- 批量删除确定
function MailDlgAllDeleteOK()
	if m_AllDelState == 0 then
		return
	end
	local MailList = {}
	local i = 1;
	while i <= #GetMail().m_Mails do
		if GetMail().m_Mails[i].m_delete_toggle == 1 then
			table.insert( MailList, GetMail().m_Mails[i].m_mailid );
			table.remove( GetMail().m_Mails, i )
		else
			i = i + 1
		end
	end
	MailDlgShow()
	
	--print( #MailList )
	-- m_count=0,m_mailid={}[m_count],
	local sendValue = {};
	sendValue.m_count = 0
	sendValue.m_mailid = {}
	for i=1, #MailList, 1 do
		table.insert( sendValue.m_mailid, MailList[i] );
		sendValue.m_count = sendValue.m_count + 1;
		
		if sendValue.m_count >= 127 then
			netsend_mailalldel_C( sendValue )
			sendValue.m_count = 0
			sendValue.m_mailid = {}
		end
	end
	if sendValue.m_count > 0 then
		netsend_mailalldel_C( sendValue )
	end
end
