using UnityEngine;
using UnityEngine.EventSystems;
using System.Collections;
using LuaInterface;

public class UISystem : UIMod 
{
	public UILayer		layer = UILayer.LAYER1;
    public bool         autoDestroy = true;
    public bool         childUI = false;        // 如果为真，当他打开时，会关闭上一个界面，关闭时会打开上一个界面。*注意，仅限Layer1

    protected LuaFunction close;

    protected override void Awake()
    {
        base.Awake();
        close = eye.luaManager.GetFunction( uiName + "Close" );
    }

	protected override void OnDestroy()
	{
		if( onDestroy != null )
			onDestroy.Call("");

		if( eye.uiManager != null )
			eye.uiManager.Unload( this );
	}

    public void Close()
    {
        if( close != null )
            close.Call();
    }
}
