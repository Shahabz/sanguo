-- 界面
local m_Dlg = nil;
local m_DialogFrameMod = nil;
local m_uiListLayer = nil; --UnityEngine.GameObject
local m_uiListScroll = nil; --UnityEngine.GameObject
local m_uiListContent = nil; --UnityEngine.GameObject
local m_uiUIP_Mail = nil; --UnityEngine.GameObject

local m_ObjectPool = nil;
local m_CurrPage = 0;
local m_TotalPage = 0;
local m_PageDirection = 0;
local m_IsShowLoadingItem = false;

-- 打开界面
function MailDlgOpen()
	m_Dlg = eye.uiManager:Open( "MailDlg" );
	m_DialogFrameMod = DialogFrameModOpen( m_Dlg, T(554), 5, MailDlgClose );
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
		elseif nControlID >= 1 and nControlID <= 100000 then
			MailDlgSelect( nControlID )
			
		-- 全部删除
		elseif nControlID == 100001 then
		
		-- 全部已读
		elseif nControlID == 100002 then
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
	-- 总页数 1页6条数据
	m_TotalPage = math.ceil( #GetMail().m_Mails/6 )
	if m_CurrPage > m_TotalPage then
		m_CurrPage = m_TotalPage;
	end
	MailDlgSetMailPage( m_CurrPage )
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
	m_uiListScroll:GetComponent("UIScrollRect"):HideLoading();
    m_IsShowLoadingItem = false;
	
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
	SetControlID( uiObj, recvValue.m_incrementid )
	
	-- 邮件类型决定icon和content解析内容
	-- 解析内容
	local contentid = 0;
	local text = recvValue.m_content_json["text"];
	if GetMail():IsTag( text, TAG_TEXTID ) then
		contentid = tonumber(string.sub(text, string.len(TAG_TEXTID) + 1));
	end
	
	-- 系统信息邮件
	if recvValue.m_type == MAIL_TYPE_SYSTEM then
		SetText( uiContent, T(contentid) )
		
	-- 公告邮件，内容外部http服务器获取
	elseif recvValue.m_type == MAIL_TYPE_NOTIFY then
		SetText( uiContent, T(contentid) )
		
	-- 每日登录
	elseif recvValue.m_type == MAIL_TYPE_EVERYDAY then
		SetText( uiContent, T(contentid) )
		
	-- 流寇
	elseif recvValue.m_type == MAIL_TYPE_FIGHT_ENEMY then
		local win = recvValue.m_content_json["win"];
		local name = recvValue.m_content_json["name"];
		local level = recvValue.m_content_json["lv"];
		local pos = recvValue.m_content_json["pos"];
		local tpos = recvValue.m_content_json["tpos"];
		local enemyname = "Lv."..level.." "..T(938);
		if win == 1 then
			SetText( uiContent, F(1110, name, enemyname ) )
		else
			SetText( uiContent, F(1111, name, enemyname ) )
		end
		
	-- 城战
	elseif recvValue.m_type == MAIL_TYPE_FIGHT_CITY then
	
	-- 国战
	elseif recvValue.m_type == MAIL_TYPE_FIGHT_NATION then
	
	end
	
	-- 解析标题
	local title = "";
	if GetMail():IsTag( recvValue.m_title, TAG_TEXTID ) then
		local textid = tonumber(string.sub(recvValue.m_title, string.len(TAG_TEXTID) + 1));
		if textid ~= nil then
			title = T(textid)
		end
	else
		title = recvValue.m_title;
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
	
	-- 附件状态
	if recvValue.m_attach == "" or recvValue.m_attachget == 1 then
		SetFalse( uiAttach );
	else
		SetTrue( uiAttach );
	end
	
	--recvValue.m_attach
	--recvValue.m_attachget
	--recvValue.m_fightid
	
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
	MailInfoDlgByIncrementID( incrementid )
end

