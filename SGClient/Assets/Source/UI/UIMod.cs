using UnityEngine;
using UnityEngine.EventSystems;
using System.Collections;
using LuaInterface;


// 需要传递事件到lua的Mod挂载。不需lua的Mod无需挂载。有cs的并需要调用lua的Mod继承此类。
// UISystem用于对话框，继承UIMod的消息处理模式！

public class UIEVENT
{
	public const int CLICK 			        = 0;		// UIButton单击			无参
	public const int PRESS	 		        = 1;		// UIButton按下			0 按下，1 抬起
	public const int INPUTSUBMIT            = 2;		// UIInputField提交		无参
	public const int TWEENFINISH	        = 3;		// UITween播放完毕		0 正向结束，1 逆向
	public const int SCROLLDRAG		        = 4;		// UIScroll系列拖动		0 开始拖动，1 拖动中，2 结束拖动
	public const int SLIDERVALUECHANGE		= 5;		// UISlider值改变		slider value值
	public const int CENTERCHILD	        = 6;		// UIScrollRing子居中	子index
    public const int TIMECOUNTEND           = 7;        // UITextTimeCountdown  计时结束
    public const int DRAGSENSOR             = 8;        // UIDragSensor拖拽事件
    public const int RICHTEXTSETEND         = 9;        // UIRichText设置内容结束
    public const int RICHTEXTMASKCLICK      = 10;       // UIRcihText标记点击
    public const int INPUTVALUECHANGED      = 11;       // UIInputField         内容修改
    public const int TIMECOUNTCHANGED       = 12;       // 计数修改变更
    public const int VOICE                  = 13;       // UIVoice事件
    public const int UIDRAG                 = 14;       // UIDRAG事件            0 开始拖动，1 结束拖动
    public const int UIDROP                 = 15;       // UIDROP事件			dragger ID
    public const int INPUTSUBMITEX            = 16;		// UIInputField提交		无参
	public const int PUSHEVENT		        = 1000;		// UI通用事件，不想定义新事件可用			参数
}

public enum UILayer
{
	LAYER0 = 0,
	LAYER1 = 1,
	LAYER2 = 2,
	LAYER3 = 3
}

public class UIMod : MonoBehaviour 
{
	public bool			enableUpdate = false;
	public GameObject[] relatedGameObject;

	public string uiName = "";
	// 事件函数
	protected LuaFunction onAwake;
	protected LuaFunction onStart;
	protected LuaFunction onEnable;
	protected LuaFunction onDisable;
	protected LuaFunction onDestroy;
	protected LuaFunction onLogic;
	protected LuaFunction onEvent;

	protected virtual void Awake ()
	{
		// 找到对应脚本的函数 事件函数命名为：obj名字加事件名
		if( uiName == "" )
			uiName = gameObject.name.TrimEnd( "(Clone)".ToCharArray() );
		onAwake = eye.luaManager.GetFunction( uiName + "OnAwake" );
		onStart = eye.luaManager.GetFunction( uiName + "OnStart" );
		onEvent = eye.luaManager.GetFunction( uiName + "OnEvent" );
		onEnable = eye.luaManager.GetFunction( uiName + "OnEnable" );
		onDisable = eye.luaManager.GetFunction( uiName + "OnDisable" );
		onDestroy = eye.luaManager.GetFunction( uiName + "OnDestroy" );
		onLogic = eye.luaManager.GetFunction( uiName + "OnLogic" );
		
		if( onAwake != null )
		{
			onAwake.Call( gameObject );
		}
	}

	protected virtual void Start()
	{
		if( onStart != null )
			onStart.Call( gameObject );
	}

	protected virtual void Update ()
	{
		if( enableUpdate && onLogic != null )
			onLogic.Call( gameObject );
	}

	protected virtual void OnEnable()
	{
		if( onEnable != null )
			onEnable.Call( gameObject );
	}

	protected virtual void OnDisable()
	{
		if( onDisable != null ) 
			onDisable.Call( gameObject );
	}

	protected virtual void OnDestroy()
	{
        if (onDestroy != null )
            onDestroy.Call( gameObject );
	}

	public virtual void OnEvent( object[] param )
	{
        //LogUtil.GetInstance().WriteGame( uiName );
		if( onEvent != null )
        {
            object[] newParam = new object[param.Length+1];
            param.CopyTo( newParam, 0 );
            newParam[newParam.Length-1] = gameObject;
            //RecUtil.GetInstance().WriteOpData(uiName, (int)param[0], (int)param[1], param[2]);
            onEvent.Call( newParam );
        }
	}	
	
	public virtual void PushEvent( object param )
	{
		object[] par = { UIEVENT.PUSHEVENT, 0, param, gameObject };
		OnEvent( par );
	}
}
