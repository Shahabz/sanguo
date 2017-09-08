-- 界面
local m_Dlg = nil;
local m_uiDialogFrameMod = nil; --UnityEngine.GameObject
local m_uiRecvTime = nil; --UnityEngine.GameObject
local m_uiDelTime = nil; --UnityEngine.GameObject
local m_uiScroll = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiMailContent = nil; --UnityEngine.GameObject
local m_uiMailAttach = nil; --UnityEngine.GameObject
local m_uiUIP_Item = nil; --UnityEngine.GameObject

local m_ObjectPool = nil;
local m_recvValue = nil

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
			MailInfoDlgAttachGet();
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
	
	-- 对象池
	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );
	m_ObjectPool:CreatePool("UIP_Item", 6, 6, m_uiUIP_Item);
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

function MailInfoDlgByRecvValue( recvValue )
	MailInfoDlgShow()
	m_recvValue = recvValue;
	
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
	SetText( m_uiDialogFrameMod.transform:Find("Top/TitleText"), title )
	
	-- 接收时间
	SetText( m_uiRecvTime, os.date( "%m-%d %H:%M", recvValue.m_recvtime ) )
	
	-- 删除时间
	local delday = 7-math.floor( (GetServerTime()-recvValue.m_recvtime)/86400 );
	if delday <= 0 then
		delday = 1;
	end
	SetText( m_uiDelTime, F( 1106, delday ) )
	
	-- 邮件类型决定icon和content解析内容
	-- 解析内容
	local contentid = 0;
	local text = recvValue.m_content_json["text"];
	if GetMail():IsTag( text, TAG_TEXTID ) then
		contentid = tonumber(string.sub(text, string.len(TAG_TEXTID) + 1));
	end
	
	
	-- 系统信息邮件
	if recvValue.m_type == MAIL_TYPE_SYSTEM then
		SetRichText( m_uiMailContent.transform:Find("Text"), T(contentid) )
		
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
			SetRichText( m_uiMailContent.transform:Find("Text"), F(contentid, name, pos, enemyname, tpos, "" ), MailOnLinkClick )
		else
			SetRichText( m_uiMailContent.transform:Find("Text"), F(contentid, name, pos, enemyname, tpos ), MailOnLinkClick )
		end
		
	-- 城战
	elseif recvValue.m_type == MAIL_TYPE_FIGHT_CITY then
	
	-- 国战
	elseif recvValue.m_type == MAIL_TYPE_FIGHT_NATION then
	
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
	if recvValue.m_lock == 0 then
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
	-- 分享邮件，战报
	--recvValue.m_fightid
end

-- 清空一页
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

-- 坐标点击
function MailOnLinkClick( str )
	local pos = string.split(str, ',')
	local gameCoorX = tonumber( pos[1] );
	local gameCoorY = tonumber( pos[2] );
	if gameCoorX >= 0 and gameCoorY >= 0 then 
		MailInfoDlgClose()
		MailDlgClose()
		WorldMap.GotoCoor( gameCoorX, gameCoorY )
	end
end

-- 设置已读状态
function MailInfoDlgRead()
	if m_recvValue == nil then
		return
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
