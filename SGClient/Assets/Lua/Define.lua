---------------------- UI浜嬩欢 ----------------------
UI_EVENT_CLICK 			    = 0;		-- UIButton鍗曞嚮				鏃犲弬
UI_EVENT_PRESS 			    = 1;		-- UIButton鎸変笅				0 鎸変笅锛�1 鎶捣
UI_EVENT_INPUTSUBMIT	    = 2;		-- UIInputField鎻愪氦			鏃犲弬
UI_EVENT_TWEENFINISH	    = 3;		-- UITween鎾斁瀹屾瘯			0 姝ｅ悜缁撴潫锛�1 閫嗗悜
UI_EVENT_SCROLLDRAG		    = 4;		-- UIScrollRect鎷栧姩			0 寮�濮嬫嫋鍔紝1 鎷栧姩涓紝2 缁撴潫鎷栧姩
UI_EVENT_SLIDERVALUECHANGE  = 5;		-- UISlider鍊兼敼鍙�			slider value鍊�
UI_EVENT_CENTERCHILD	    = 6;		-- UIScrollRing瀛愬眳涓�		瀛恑ndex
UI_EVENT_TIMECOUNTEND       = 7;        -- UITextTimeCountdown      璁℃椂缁撴潫
UI_EVENT_DRAGSENSOR         = 8;        -- UIDragSensor鎷栨嫿浜嬩欢
UI_EVENT_RICHTEXTSETEND     = 9;        -- UIRcihText鏍囪鐐瑰嚮
UI_EVENT_RICHTEXTMASKCLICK  = 10;       -- UIRcihText鏍囪鐐瑰嚮
UI_EVENT_INPUTVALUECHANGED  = 11;       -- UIInputField鍐呭淇敼		褰撳墠inputfield鍐呭
UI_EVENT_TIMECOUNTCHANGED   = 12;       -- 璁℃暟淇敼
UI_EVENT_VOICE              = 13;       -- UIVoice
UI_EVENT_UIDRAG             = 14;       -- UIDRAG浜嬩欢            0 寮�濮嬫嫋鍔紝1 缁撴潫鎷栧姩
UI_EVENT_UIDROP             = 15;       -- UIDROP浜嬩欢			dragger ID
UI_EVENT_INPUTSUBMITEX      = 16;		-- UIInputField鎻愪氦		鏃犲弬
UI_EVENT_TOGGLEVALUECHANGE	= 17;
UI_EVENT_CLICKPOS			= 18;		-- 鐐瑰嚮浣嶇疆浜嬩欢
UI_EVENT_SCROLLPAGE			= 20;		-- UIScrollRectHelper 缈婚〉婊氬姩
UI_EVENT_PUSHEVENT		    = 1000;		-- UI閫氱敤浜嬩欢锛屼笉鎯冲畾涔夋柊浜嬩欢鍙敤			    鍙傛暟

-- 鍙崌绾х殑鏅�氬缓绛�
BUILDING_Main           = 1  -- 涓诲煄
BUILDING_Wall           = 2  -- 鍩庡
BUILDING_StoreHouse     = 3  -- 浠撳簱
BUILDING_Tech           = 4  -- 澶闄�
BUILDING_Craftsman      = 5  -- 鏉愭枡浣滃潑
BUILDING_Cabinet        = 6  -- 鍐呴榿

-- 鍏佃惀
BUILDING_Infantry       		= 11  -- 姝ュ叺钀�
BUILDING_Cavalry        		= 12  -- 楠戝叺钀�
BUILDING_Archer         		= 13  -- 寮撳叺钀�
BUILDING_Militiaman_Infantry	= 14  -- 姘戝叺钀�-姝ュ叺
BUILDING_Militiaman_Cavalry		= 15  -- 姘戝叺钀�-楠戝叺
BUILDING_Militiaman_Archer		= 16  -- 姘戝叺钀�-寮撳叺

-- 璧勬簮寤虹瓚
BUILDING_Silver		    =  21	-- 閾跺竵
BUILDING_Wood		    =  22	-- 鏈ㄦ潗
BUILDING_Food		    =  23	-- 椋熺墿
BUILDING_Iron		    =  24   -- 闀旈搧

-- 绾叆鍙ｅ缓绛�
BUILDING_Smithy		    =  31	-- 閾佸尃閾�
BUILDING_Wash		    =  32	-- 娲楃偧閾�
BUILDING_Fangshi	    =  33	-- 鍧婂競
BUILDING_Shop		    =  34	-- 鍟嗗簵
BUILDING_Hero		    =  35	-- 鑱氳搐棣�
BUILDING_Wishing	    =  36	-- 鑱氬疂鐩�
BUILDING_Help		    =  37	-- 甯姪

-- 鍔熻兘鍏ュ彛
CITY_FUNCTION_SMITHY		=0	-- 閾佸尃閾�
CITY_FUNCTION_WASH			=1	-- 娲楃偧閾�
CITY_FUNCTION_FANGSHI		=2	-- 鍧婂競
CITY_FUNCTION_SHOP			=3	-- 鍟嗗簵
CITY_FUNCTION_HERO			=4	-- 鑱氳搐棣�
CITY_FUNCTION_WISHING		=5	-- 鑱氬疂鐩�
CITY_FUNCTION_NATION		=10	-- 鍥藉
CITY_FUNCTION_WORLD			=11	-- 涓栫晫
CITY_FUNCTION_STORY			=12	-- 鍓湰
CITY_FUNCTION_MAIL			=13	-- 閭欢
CITY_FUNCTION_FRIEND		=14	-- 濂藉弸
CITY_FUNCTION_NATIONEQUIP	=15	-- 鍥藉櫒
CITY_FUNCTION_RANK			=16	-- 鎺掕姒�
CITY_FUNCTION_HEROBUTTON	=17	-- 姝﹀皢
CITY_FUNCTION_CHAT			=18	-- 鑱婂ぉ
CITY_FUNCTION_WARKEREX		=19	-- 鍟嗕笟寤洪�犻槦
CITY_FUNCTION_Militiaman	=20	-- 姘戝叺钀�
CITY_FUNCTION_AUTO_ADDHP	=25	-- 鑷姩琛ュ叺寮�鍚叧闂�
CITY_FUNCTION_BATTLE_ADDHP	=26	-- 鎴樺墠琛ュ叺寮�鍚叧闂�

BuildingPrefab={
[BUILDING_Main] = { prefab = "BUILDING_Main", land="BuildingLand0" },
[BUILDING_Wall] = { prefab = "BUILDING_Wall", land="BuildingLand1" },
[BUILDING_StoreHouse] = { prefab = "BUILDING_StoreHouse", land="BuildingLand2" },
[BUILDING_Tech] = { prefab = "BUILDING_Tech", land="BuildingLand3" },
[BUILDING_Craftsman] = { prefab = "BUILDING_Craftsman", land="BuildingLand4" },
[BUILDING_Cabinet] = { prefab = "BUILDING_Cabinet", land="BuildingLand5" },
[BUILDING_Infantry] = { prefab = "BUILDING_Infantry", land="BuildingLand6" },
[BUILDING_Cavalry] = { prefab = "BUILDING_Cavalry", land="BuildingLand7" },
[BUILDING_Archer] = { prefab = "BUILDING_Archer", land="BuildingLand8" },
[BUILDING_Militiaman_Infantry] = { prefab = "BUILDING_Militiaman_Infantry", land="BuildingLand9" },
[BUILDING_Militiaman_Cavalry] = { prefab = "BUILDING_Militiaman_Cavalry", land="BuildingLand9" },
[BUILDING_Militiaman_Archer] = { prefab = "BUILDING_Militiaman_Archer", land="BuildingLand9" },
[BUILDING_Silver] = { prefab = "BUILDING_Silver", land="ResLand" },
[BUILDING_Wood] = { prefab = "BUILDING_Wood", land="ResLand" },
[BUILDING_Food] = { prefab = "BUILDING_Food", land="ResLand" },
[BUILDING_Iron] = { prefab = "BUILDING_Iron", land="ResLand" },
[BUILDING_Smithy] = { prefab = "BUILDING_Smithy", land="BuildingLand12" },
[BUILDING_Wash] = { prefab = "BUILDING_Wash", land="BuildingLand13" },
[BUILDING_Fangshi] = { prefab = "BUILDING_Fangshi", land="BuildingLand14" },
[BUILDING_Shop] = { prefab = "BUILDING_Shop", land="BuildingLand15" },
[BUILDING_Hero] = { prefab = "BUILDING_Hero", land="BuildingLand16" },
[BUILDING_Wishing] = { prefab = "BUILDING_Wishing", land="BuildingLand17" },
[BUILDING_Help] = { prefab = "BUILDING_Help", land="BuildingLand18" },
} 
--[[
	if kind == BUILDING_Main then
	elseif kind == BUILDING_Wall then
	elseif kind == BUILDING_StoreHouse then
	elseif kind == BUILDING_Tech then
	elseif kind == BUILDING_Craftsman then
	elseif kind == BUILDING_Cabinet then
	elseif kind == BUILDING_Infantry then
	elseif kind == BUILDING_Cavalry then
	elseif kind == BUILDING_Archer then
	elseif kind == BUILDING_Militiaman_Infantry then
	elseif kind == BUILDING_Militiaman_Cavalry then
	elseif kind == BUILDING_Militiaman_Archer then
	elseif kind == BUILDING_Silver then
	elseif kind == BUILDING_Wood then
	elseif kind == BUILDING_Food then
	elseif kind == BUILDING_Iron then
	elseif kind == BUILDING_Smithy then
	elseif kind == BUILDING_Wash then
	elseif kind == BUILDING_Fangshi then
	elseif kind == BUILDING_Shop then
	elseif kind == BUILDING_Hero then
	elseif kind == BUILDING_Wishing then
	elseif kind == BUILDING_Help then
	end
--]]
-- 閫斿緞
PATH_SYSTEM						=	1	-- 绯荤粺
PATH_GM							=	2	-- GM
PATH_PAY						=	3	-- 鍏呭��
PATH_ITEMUSE					=	4	-- 閬撳叿浣跨敤
PATH_TOKENITEMUSE				=	5	-- 閽荤煶鐩存帴浣跨敤
PATH_SELL						=	6	-- 鍞崠涓㈠純
PATH_BUILDING_UPGRADE			=	7	-- 寤虹瓚
PATH_QUEST						=	8	-- 浠诲姟
PATH_GUARD_UPGRADE				=	9	-- 瀹堝崼鍗囩骇
PATH_LEVY						=	10	-- 寰佹敹
PATH_TRAIN						=	11	-- 璁粌
PATH_RESOLVE					=	12	-- 鍒嗚В
PATH_EQUIPEXT					=	13	-- 璐拱瑁呭瀹归噺
PATH_TECH_UPGRADE				=	14	-- 绉戞妧鍗囩骇
PATH_TECH_FINISH				=	15	-- 绉戞妧瀹屾垚
PATH_TECH_GET					=	16	-- 绉戞妧棰嗗彇
PATH_TECH_QUICK					=	17	-- 绉戞妧鍔犻��
PATH_TECH_FREEQUICK				=	18	-- 绉戞妧鍏嶈垂鍔犻��
PATH_HIRE_FORGING				=	19	-- 闆囦剑
PATH_HIRE_GOV					=	20	-- 闆囦剑
PATH_HIRE_TECH					=	21	-- 闆囦剑
PATH_BUILDING_WORKER			=	22	-- 鍟嗙敤寤洪�犻槦鍒�
PATH_TRAIN_QUEUE				=	23	-- 鎷涘嫙闃熷垪
PATH_TRAIN_LONG					=	24	-- 璁粌鏃堕暱
PATH_TRAIN_GET					=	25	-- 璁粌棰嗗彇
PATH_TRAIN_CANCEL				=	26	-- 鍙栨秷鎷涘嫙
PATH_EQUIP_FORGING				=	27	-- 瑁呭鎵撻��
PATH_FORGING_QUICK				=	28	-- 鎵撻�犲姞閫�
PATH_FORGING_FREEQUICK			=	29	-- 閾佸尃鍔犻��
PATH_BODYBUY					=	30	-- 璐拱浣撳姏
PATH_CHANGENAME					=	31	-- 鏀瑰悕
PATH_GUARD_LIFE					=	32	-- 瀹堝崼琛ヨ
PATH_EQUIP_UP					=	33	-- 瑁呭涓�
PATH_EQUIP_DOWN					=	34	-- 瑁呭涓�
PATH_HERO_ADDEXP				=	35	-- 鍔犺嫳闆勭粡楠�
PATH_HERO_ADDSOLDIERS			=	36	-- 琛ュ叺
PATH_HERO_SOLDIERS_EQUIP		=	37	-- 鑴卞嵏瑁呭甯︽潵鐨勫叺鍔涙崯澶�
PATH_HERO_WASH					=	38	-- 鑻遍泟娲楅珦
PATH_EQUIP_WASH					=	39	-- 瑁呭娲楃粌
PATH_MATERIALMAKE_QUEUE			=	40	-- 鏉愭枡鍒堕�犻槦鍒�
PATH_MATERIALMAKE				=	41	-- 鏉愭枡鍒堕��
PATH_STORY						=	42	-- 鍓湰
PATH_GATHER						=	43	-- 閲囬泦
PATH_FIGHT						=	44	-- 鎴樻枟
PATH_MATERIALMAKE				=	41	-- 鏉愭枡鍒堕��
PATH_HEROVISIT					=	63  -- 姝﹀皢瀵昏
PATH_HEROVISIT_CHANGE			=	64	-- 姝﹀皢瀵昏-鑻遍泟杞亾鍏�
PATH_HERO_UP					=	65	-- 涓婇樀
PATH_HERO_DOWN					=	66	-- 涓嬮樀
PATH_HERO_GUARD					=	67	-- 寰℃灄鍗�
PATH_HERO_GUARD_AUTO			=	68	-- 寰℃灄鍗嚜鍔�
PATH_STORY_SWEEP				=   72	-- 鍓湰鎵崱閫斿緞
PATH_WOUNDED_SOLDIERS			=	76	-- 浼ゅ叺鎭㈠
PATH_NATIONHERO					=	85	-- 鍥藉鍚嶅皢
PATH_WISHINGSHOP				=	87	-- 鑱氬疂鐩�
PATH_WISHINGCHANGE				=	88	-- 鑱氬疂鐩嗗厬鎹�
PATH_WISHINGPACK				=	89	-- 鑱氬疂鐩嗘墦鍖�
PATH_SELECTNATION				=	99	-- 閫夋嫨鍥藉

-- 鏂囧瓧琛�
-- 鑾峰彇鏈湴鍖栨枃瀛�
function GetLocalizeText( id )
    if id == nil then
        return "";
    end
	return Localization.text( id );
end
function T( nameid )
    return GetLocalizeText( nameid );
end
function zh( text )
    return text;
end

function F( nameid, ... )
	return Utils.StringFormat( Localization.text( nameid ), ... );
end

function FF( text, ... )
	return Utils.StringFormat( text, ... );
end

function FQUEST( nameid, ... )
	return Utils.StringFormat( Localization.text_quest( nameid ), ... );
end

function Hex2Color( hex )
	return Utils.HexColor( hex )
end

function Hex2Color32( hex )
	return Utils.HexColor32( hex )
end

function zhtime( second )
	
	local day = math.floor( second / 86400 );
	local hour = math.floor(second / 3600) % 24;
	local min = math.floor(second / 60) % 60;
	local sec = math.floor(second % 60);
	local szmsg = "";
	if day > 0 then
        szmsg = day..T(156)
        if hour > 0 then
            szmsg = szmsg..hour..T(155)
            if min > 0 then
                szmsg = szmsg..min..T(154)
            end
        end
		return szmsg
    end
	
	if hour > 0 then
        szmsg = szmsg..hour..T(155)
        if min > 0 then
            szmsg = szmsg..min..T(154)
        end
		if sec > 0 then
            szmsg = szmsg..sec..T(153)
        end
		return szmsg
    end
	
	if min > 0 then
        szmsg = szmsg..min..T(154)
		if sec > 0 then
            szmsg = szmsg..sec..T(153)
        end
		return szmsg
    end
       
    return sec..T(153)
end

-- 鍐檊amelog
function gamelog( msg )
	LogUtil.GetInstance():WriteGame( msg );
end

-- 鍐檔etlog
function netlog( msg )
	LogUtil.GetInstance():WriteNet( msg );
end

-- 寤舵椂鎵ц
function Invoke(func, delay, param, name)
    if param == nil then
        param = 0;
    end
    if name == nil then
        name = "";
    end
    eye.gameManager.delayExecute = func;
    eye.gameManager:GameInvoke(delay, param, name);
end

-- 鍋滄寤舵椂鎵ц
function InvokeStop(name)
    eye.gameManager:GameInvoke_Stop(name);
end

-- 鑾峰彇鍥藉鍚嶇О
function Nation( nation )
	if nation == 0 then
		return T(110)
	end
	return T(nation+100);
end

function NationEx( nation )
	return T(nation+110);
end

-- 鑾峰彇寤虹瓚鍚嶇О
function BuildingName( kind, offset )
	if kind >= BUILDING_Silver and kind <= BUILDING_Iron and offset ~= nil then
		return F( 99, offset+1, T( kind ) )
	else
		return T(kind);
	end
end

-- 鑾峰彇寤虹瓚鍚嶇О甯︾瓑绾�
function BuildingNameLv( kind, offset, level )
	if kind >= BUILDING_Silver and kind <= BUILDING_Iron and offset ~= nil then
		return "Lv."..level.." "..F( 99, offset+1, T( kind ) )
	else
		return "Lv."..level.." "..T(kind);
	end
end

-- 鑾峰彇鑻遍泟鍚嶇О
function HeroName( kind )
	return Localization.text_item(kind+3000);
end

-- 鑾峰彇鑻遍泟鍚嶇О锛屽甫瀛楃殑
function HeroNameEx( kind )
	return Localization.text_item(kind+13000);
end


-- 鑾峰彇鑻遍泟鍚嶇О甯︾瓑绾�
function HeroNameLv( kind, level )
	return "Lv."..level.." "..HeroName( kind );
end

-- 鑻遍泟鐘舵�佹枃瀛�
function HeroState( state )
	if state == 0 then
		return T(141);
	elseif state == 1 or state == 3 then
		return T(142);
	elseif state == 2 then
		return T(299);
	end
end

-- 鑻遍泟鐘舵�佹枃瀛�
function HeroStateFull( state )
	if state == 0 then
		return T(296);
	elseif state == 1 or state == 3 then
		return T(297);
	elseif state == 2 then
		return T(298);
	end
end

-- 鑾峰彇濂冲皢鍚嶇О
function GirlName( kind )
	return Localization.text_item(kind+3200);
end

-- 鑾峰彇濂冲皢绫诲瀷
function GirlType( type )
	return T(type+3330);
end

-- 濂冲皢鎻忚堪
function GirlDesc(kind)
	return Localization.text_item(kind+3400);
end

-- 鑾峰彇瀛╁瓙鍚嶇О
function SonName( kind )
	return Localization.text_item(kind);
end

-- 鍏电鍚嶇О
function CorpsName( corps )
	return T(134+corps);
end

-- 鍏电鍚嶇О
function CorpsNameEx( buildingkind )
	local corps = 0;
	if buildingkind == BUILDING_Infantry or buildingkind == BUILDING_Militiaman_Infantry then
		corps = 0
	elseif buildingkind == BUILDING_Cavalry or buildingkind == BUILDING_Militiaman_Cavalry then
		corps = 1
	elseif buildingkind == BUILDING_Archer or buildingkind == BUILDING_Militiaman_Archer then
		corps = 2
	end
	return CorpsName( corps );
end

-- 鑾峰彇瑁呭鍚嶇О
function EquipName( kind )
	return Localization.text_item(kind+2000);
end

-- 鑾峰彇鍥藉櫒鍚嶇О
function NationEquipName( kind )
	return Localization.text_item(kind+2200);
end

-- 鑾峰彇绉戞妧鍚嶇О
function TechName( kind )
	return Localization.text_item(kind+4000);
end

-- 鑾峰彇绉戞妧鎻忚堪
function TechDesc( kind )
	return Localization.text_item(kind+4100);
end

-- 鑾峰彇绉戞妧鎻忚堪
function TechDescUp( kind )
	return Localization.text_item(kind+4200);
end

-- 鑾峰彇璧勬簮鍚嶇О
function ResName( restype )
	return T(120+restype)
end

-- 鑾峰彇鍦板浘鍖哄煙鍚嶇О
function MapZoneName( id )
	return Localization.text_item(14000+id);
end

-- 鑾峰彇鍦板浘鍩庢睜鍚嶇О
function MapTownName( id )
	return Localization.text_item(14100+id);
end

-- 鑾峰彇鍦板浘鍩庢睜瑙勬ā鍚�
function MapTownType( type )
	return Localization.text(180+type);
end

-- 鑾峰彇琛�鎴樻嵁鐐�
KingWarTownNameList = {1384,1385,1386,1380,1381,1382,1383 }
function KingWarTownName( id )
	return T( KingWarTownNameList[id] )
end

-- 鑾峰彇鏁屽啗鍚嶇О
function EnemyName( id )
	return Localization.text_item(8000+id);
end

-- 瀹樿亴鍚嶇О
function OfficialName( place )
	return Localization.text( 240+place );
end

-- 鑱屼綅鍚嶇О
function PlaceName( place )
	return Localization.text( 200+place );
end

-- 鑱屼綅鎻忚堪
function PlaceDesc( place )
	return Localization.text( 260+place );
end

-- 涓栫晫浠诲姟鍚嶇О
function WorldQuestName( questid )
	return Localization.text_quest( 2000+questid );
end

-- 鍚嶇О棰滆壊
function NameColor( color )
	local c = { 0xf7f3bbff, 0x03de27ff, 0x25c9ffff, 0xd95df4ff, 0xffde00ff, 0xe80017ff, 0x9b8869ff, 0x9b8869ff, 0x9b8869ff, 0x9b8869ff, 0x9b8869ff  }
	return Hex2Color( c[color+1] )
end

-- 鍚嶇О棰滆壊
function NameColorStr( color )
	local c = { "f7f3bbff", "03de27ff", "25c9ffff", "d95df4ff", "ffde00ff", "e80017ff", "9b8869ff", "9b8869ff", "9b8869ff", "9b8869ff", "9b8869ff" }
	return c[color+1]
end

-- 甯﹂鑹茬殑鏂囨湰
function NameColorText( text, color )
	local name = "<color=#"..NameColorStr(color)..">"..text.."</color>"
	return name
end

-- 鍥藉棰滆壊
function NationColor( nation )
	local c = { 0x25C9FFFF, 0xE80017FF, 0x03DE27FF }
	return Hex2Color( c[nation] )
end

-- 鍥藉棰滆壊
function NationColorStr( nation )
	local c = { "25C9FFFF", "E80017FF", "03DE27FF" }
	return c[nation]
end

-- 棰滆壊鍚嶇О
function ColorName( color )
	return T( 157+color )
end

-- 娓呯┖鎵�鏈夊瓙鑺傜偣
function clearChild( object )
	--for i = 0 ,object.transform.childCount - 1 do
       -- GameObject.Destroy( object.transform:GetChild(i).gameObject );
    --end
	Utils.ClearChild( object.transform );
end
-- 鑾峰彇鎵�鏈夊瓙鑺傜偣
function GetChild( object )
	local arrayObj = {};
	for i = 0 ,object.transform.childCount - 1 do
		table.insert(arrayObj,object.transform:GetChild(i).gameObject);
	end
	return arrayObj;
end

-- 娣诲姞鑺傜偣
function addChild( object, prefab )
	local obj = GameObject.Instantiate( prefab );
	obj.transform:SetParent( object.transform );
	obj.transform.localPosition = Vector3.zero;
	obj.transform.localScale = Vector3.one;
	obj.gameObject:SetActive( true );
	return obj;
end

-- 娣诲姞鑺傜偣
function addObj( object, prefab )
	local obj = GameObject.Instantiate( prefab );
	obj.transform:SetParent( object.transform );
	obj.transform.position = object.transform.position;
	obj.transform.localScale = Vector3.one;
	return obj;
end

function SetParent( obj, parent )
	obj.transform:SetParent( parent.transform );
	obj.transform.localPosition = Vector3.zero;
	obj.transform.localScale = Vector3.one;
	obj.gameObject:SetActive( true )
end

function SetImage( transform, sprite, nativeSize )
	local uiComponent = transform:GetComponent( typeof(Image) )
	uiComponent.sprite = sprite;
	if nativeSize then
		uiComponent:SetNativeSize();
	end
end

function SetText( transform, text, color )
	local uiComponent = transform:GetComponent( typeof(UIText) );
	uiComponent.text = text;
	if color ~= nil then
		uiComponent.color = color;
	end
end

function SetLevel( transform, level, color )
	SetText( transform, "Lv."..level, color )
end

function SetTextColor( transform, color )
	transform:GetComponent( typeof(UIText) ).color = color;
end

function SetRichText( transform, text, callback )
	local uiComponent = transform:GetComponent( typeof(YlyRichText) )
	uiComponent.text = text;
	if callback ~= nil then
		uiComponent.onLinkClick = callback
	end
end

function SetRichTextColor( transform, color )
	local uiComponent = transform:GetComponent( typeof(YlyRichText) )
	uiComponent.color = color;
end

function SetTextWriter( transform, text, fun )
	SetText( transform, text );
	local uiTypeWriter = transform:GetComponent( typeof(TypeWriter) );
	uiTypeWriter:Play();
	uiTypeWriter.onFinish = fun
end

function SetControlID( transform, controlID )
	transform:GetComponent( typeof(UIButton) ).controlID = controlID;
end

function SetButtonTrue( transform )
	transform:GetComponent( typeof(UIButton) ).interactable = true;
end

function SetButtonFalse( transform )
	transform:GetComponent( typeof(UIButton) ).interactable = false;
end

function SetTimer( transform, sec, needsec, controlID, formatText )
	local timer = transform:GetComponent( typeof(UITextTimeCountdown) )
    timer:SetTime( needsec, needsec-sec );
	if sec == 0 and needsec == 0 then
		timer:Stop()
	end
	if controlID ~= nil then
		timer.controlID = controlID;
	end
	if formatText ~= nil then
		timer.formatText = formatText;
	end
end

function SetTimerOver( transform, text )
	local timer = transform:GetComponent( typeof(UITextTimeCountdown) )
	timer.overText = text;
end

function SetTimerText( transform, text )
	local timer = transform:GetComponent( typeof(UITextTimeCountdown) )
	timer.text = text;
end

function SetProgress( transform, value )
	transform:GetComponent( typeof(UIProgress) ):SetValue(value);
end

function SetProgressPlay( transform, beginvalue, endvalue, duration, playcount, callback )
	local transform = transform:GetComponent( typeof(UIProgress) )
	transform:Play( beginvalue, endvalue, duration, playcount );
	transform.callback = callback
end
function SetProgressStop( transform)
	local transform = transform:GetComponent( typeof(UIProgress) )
	transform:Stop();
end

function SetSlider( transform, value )
	transform:GetComponent( typeof(UISlider) ).value = value;
end

function SetTrue( transform )
	transform.gameObject:SetActive( true )
end

function SetFalse( transform )
	transform.gameObject:SetActive( false )
end

function SetShow( transform, active )
	transform.gameObject:SetActive( active )
end

function IsActive( transform )
	return transform.gameObject.activeSelf
end

function SetGray( transform, gray )
	if gray == true then
		transform:GetComponent( typeof(Image) ).material = ResourceManager.LoadMaterial( "UI_Mat_SpriteGray" );
	else
		transform:GetComponent( typeof(Image) ).material = nil;
	end
end

--
function SetSpriteGray( transform, gray )
	if gray == true then
		transform:GetComponent( "SpriteRenderer" ).material = ResourceManager.LoadMaterial( "UI_Mat_SpriteGray" );
	else
		transform:GetComponent( "SpriteRenderer" ).material = ResourceManager.LoadMaterial( "UI_Mat_SpriteDefault" );
	end
end

function SetSprite( transform, sprite )
	transform:GetComponent( "SpriteRenderer" ).sprite = sprite;
end

function SetItemCenterInScroll(index,scroll)
	scroll:GetComponent( "UIScrollRect" ):CenterOnItem(index,scroll.gameObject);
end