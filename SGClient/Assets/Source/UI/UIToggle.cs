using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.EventSystems;

public class UIToggle : Toggle {

	public int controlID = 0;
	public UIMod uiMod;

	// Use this for initialization
	protected override void  Start () 
	{
		base.Start ();

		// 挂载uiMod
		if( uiMod == null )
		{
			uiMod = gameObject.GetComponentInParent<UIMod>();
		}

		this.onValueChanged.AddListener( ValueChanged );
	}
	
	protected void ValueChanged( bool isOn )
	{
		if( uiMod == null )
			return;

		object[] param = { UIEVENT.TOGGLEVALUECHANGE, controlID, isOn };
		uiMod.OnEvent(param);
	}
}
