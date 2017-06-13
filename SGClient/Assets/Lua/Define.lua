---------------------- UI事件 ----------------------
UI_EVENT_CLICK 			    = 0;		-- UIButton单击				无参
UI_EVENT_PRESS 			    = 1;		-- UIButton按下				0 按下，1 抬起
UI_EVENT_INPUTSUBMIT	    = 2;		-- UIInputField提交			无参
UI_EVENT_TWEENFINISH	    = 3;		-- UITween播放完毕			0 正向结束，1 逆向
UI_EVENT_SCROLLDRAG		    = 4;		-- UIScrollRect拖动			0 开始拖动，1 拖动中，2 结束拖动
UI_EVENT_SLIDERVALUECHANGE  = 5;		-- UISlider值改变			slider value值
UI_EVENT_CENTERCHILD	    = 6;		-- UIScrollRing子居中		子index
UI_EVENT_TIMECOUNTEND       = 7;        -- UITextTimeCountdown      计时结束
UI_EVENT_DRAGSENSOR         = 8;        -- UIDragSensor拖拽事件
UI_EVENT_RICHTEXTSETEND     = 9;        -- UIRcihText标记点击
UI_EVENT_RICHTEXTMASKCLICK  = 10;       -- UIRcihText标记点击
UI_EVENT_INPUTVALUECHANGED  = 11;       -- UIInputField内容修改		当前inputfield内容
UI_EVENT_TIMECOUNTCHANGED   = 12;       -- 计数修改
UI_EVENT_VOICE              = 13;       -- UIVoice
UI_EVENT_PUSHEVENT		    = 1000;		-- UI通用事件，不想定义新事件可用			    参数

-- 文字表
-- 获取本地化文字
function GetLocalizeText( id )
    if id == nil then
        return "";
    end
	return Localization.text( id );
end
function T( nameid )
    return GetLocalizeText( nameid );
end

-- 写gamelog
function gamelog( msg )
	LogUtil.GetInstance():WriteGame( msg );
end

-- 写netlog
function netlog( msg )
	LogUtil.GetInstance():WriteNet( msg );
end

-- 清空所有子节点
function clearChild( object )
	for i = 0 ,object.transform.childCount - 1 do
        GameObject.Destroy( object.transform:GetChild(i).gameObject );
    end
end

-- 延时执行
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
-- 停止延时执行
function InvokeStop(name)
    eye.gameManager:GameInvoke_Stop(name);
end
