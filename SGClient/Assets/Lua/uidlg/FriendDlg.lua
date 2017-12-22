-- 界面
local m_Dlg = nil;
local m_uiUIP_Friend = nil; --UnityEngine.GameObject
local m_uiContent = nil; --UnityEngine.GameObject
local m_uiFr

local m_ObjectPool = nil;
local m_uiFriendObj = {};
-- m_op=0,m_target_actorid=0,m_target_cityindex=0,m_target_namelen=0,m_target_name="[m_target_namelen]",
local sendValue ={}
sendValue.m_op = 0;
sendValue.m_target_actorid = 0;
sendValue.m_target_cityindex = 0;
sendValue.m_target_namelen = 0;
sendValue.m_target_name = "";
	
-- 打开界面
function FriendDlgOpen()
	m_Dlg = eye.uiManager:Open( "FriendDlg" );
end

-- 隐藏界面
function FriendDlgClose()
	if m_Dlg == nil then
		return;
	end
	
	eye.uiManager:Close( "FriendDlg" );
end

-- 删除界面
function FriendDlgDestroy()
	GameObject.Destroy( m_Dlg );
	m_Dlg = nil;
end

----------------------------------------
-- 事件
----------------------------------------

-- 所属按钮点击时调用
function FriendDlgOnEvent( nType, nControlID, value, gameObject )
	if nType == UI_EVENT_CLICK then
        if nControlID == -1 then
            FriendDlgClose();
        end
	end
end

-- 载入时调用
function FriendDlgOnAwake( gameObject )
	-- 控件赋值	
	local objs = gameObject:GetComponent( typeof(UISystem) ).relatedGameObject;	
	m_uiUIP_Friend = objs[0];
	m_uiContent = objs[1];
	
	-- 对象池
	m_ObjectPool = gameObject:GetComponent( typeof(ObjectPoolManager) );
	m_ObjectPool:CreatePool("UIP_Friend", 6, 6, m_uiUIP_Friend);
end

-- 界面初始化时调用
function FriendDlgOnStart( gameObject )
	
end

-- 界面显示时调用
function FriendDlgOnEnable( gameObject )
	
end

-- 界面隐藏时调用
function FriendDlgOnDisable( gameObject )
	
end

-- 界面删除时调用
function FriendDlgOnDestroy( gameObject )
	m_Dlg = nil;
end

-- 每帧调用
function FriendDlgOnLogic( gameObject )
	
end


----------------------------------------
-- 自定
----------------------------------------
function FriendDlgShow()
	FriendDlgOpen()
	FriendDlgAskList()
	FriendDlgRecv();
end

-- m_count=0,m_list={m_actorid=0,m_city_index=0,m_shape=0,m_namelen=0,m_name="[m_namelen]",m_level=0,m_place=0,m_battlepower=0,m_ask=0,[m_count]},
function FriendDlgRecv( recvValue )
	local recvValue ={};
	recvValue.m_count = 10;
	recvValue.m_list={
						{m_actorid=0,m_city_index =0,m_shape=1,m_namelen=4,m_name="xxxxxxxx",m_level=15,m_place=3,m_battlepower=33323,m_ask=0},	
						{m_actorid=0,m_city_index =0,m_shape=2,m_namelen=4,m_name="cccccccc",m_level=123,m_place=4,m_battlepower=35454,m_ask=1},	
						{m_actorid=0,m_city_index =0,m_shape=3,m_namelen=4,m_name="xxxxxvvs",m_level=152,m_place=6,m_battlepower=356565,m_ask=1},	
						{m_actorid=0,m_city_index =0,m_shape=4,m_namelen=4,m_name="sdasdfds",m_level=124,m_place=7,m_battlepower=37777,m_ask=0},	
						{m_actorid=0,m_city_index =0,m_shape=5,m_namelen=4,m_name="bcxcfgg",m_level=34,m_place=8,m_battlepower=38888,m_ask=1},	
						{m_actorid=0,m_city_index =0,m_shape=6,m_namelen=4,m_name="erwerw",m_level=23,m_place=8,m_battlepower=344444,m_ask=1},	
						{m_actorid=0,m_city_index =0,m_shape=7,m_namelen=4,m_name="hdfrgrg",m_level=6,m_place=8,m_battlepower=333333,m_ask=1},	
						{m_actorid=0,m_city_index =0,m_shape=8,m_namelen=4,m_name="adsdsfds",m_level=123,m_place=4,m_battlepower=322222,m_ask=0},	
						{m_actorid=0,m_city_index =0,m_shape=9,m_namelen=4,m_name="shgsfdsfds",m_level=21,m_place=5,m_battlepower=32222111,m_ask=0},	
						{m_actorid=0,m_city_index =0,m_shape=10,m_namelen=4,m_name="asgasdfsdf",m_level=22,m_place=9,m_battlepower=311111,m_ask=0},	
					}
	table.sort(recvValue.m_list,function(a,b)
				local r
				local al = tonumber(a.m_level);
				local bl = tonumber(b.m_level);
				local at = tonumber(a.m_battlepower);
				local bt = tonumber(b.m_battlepower);
				local as = tonumber(a.m_ask);
				local bs = tonumber(b.m_ask);
				if as == bs then
					if al == bl then
						r = at > bt
					else
						r = al > bl
					end
				else
					r = as > bs
				end
				return r 
		end);
	for i=1,recvValue.m_count,1 do
		local uiObj = m_ObjectPool:Get( "UIP_Friend" );
		uiObj.transform:SetParent( m_uiContent.transform );	
		m_uiFriendObj[i] = 	uiObj;
		local recvList = recvValue.m_list[i];
		local objs = uiObj.transform:GetComponent( typeof(Reference) ).relatedGameObject;
		local uiHeroPic = objs[0]
		local uiPlacePic = objs[1]
		local uiLevelText = objs[2]
		local uiHeroNameText = objs[3]
		local uiBattleText = objs[4]
		local uiAcceptBtn = objs[5]
		local uiRefuseBtn = objs[6]
		SetImage(uiHeroPic,HeroHeadSprite(recvList.m_shape));
		SetImage(uiPlacePic,PlaceSprite(recvList.m_place));
		SetLevel(uiLevelText,recvList.m_level);
		SetText(uiHeroNameText,recvList.m_name);
		SetText(uiBattleText,recvList.m_battlepower);
		if recvList.m_ask == 0 then
			SetFalse(uiAcceptBtn)
			SetFalse(uiRefuseBtn)
		else
			SetTrue(uiAcceptBtn)
			SetTrue(uiRefuseBtn)
		end
		
	end

	
	
end

-- 获取列表
function FriendDlgAskList()
	sendValue.m_op = 0;
	sendValue.m_target_actorid = 0;
	sendValue.m_target_cityindex = -1;
	sendValue.m_target_namelen = 0;
	sendValue.m_target_name = "";
	netsend_friendop_C( sendValue )
end

-- 同意
function FriendDlgAgree( actorid, cityindex )
	sendValue.m_op = 2;
	sendValue.m_target_actorid = actorid;
	sendValue.m_target_cityindex = cityindex;
	sendValue.m_target_namelen = 0;
	sendValue.m_target_name = "";
	netsend_friendop_C( sendValue )
end

-- 拒绝
function FriendDlgReject( actorid, cityindex )
	sendValue.m_op = 3;
	sendValue.m_target_actorid = actorid;
	sendValue.m_target_cityindex = cityindex;
	sendValue.m_target_namelen = 0;
	sendValue.m_target_name = "";
	netsend_friendop_C( sendValue )
end

-- 删除
function FriendDlgDelete( actorid, cityindex )
	sendValue.m_op = 4;
	sendValue.m_target_actorid = actorid;
	sendValue.m_target_cityindex = cityindex;
	sendValue.m_target_namelen = 0;
	sendValue.m_target_name = "";
	netsend_friendop_C( sendValue )
end
