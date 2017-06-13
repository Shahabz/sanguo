using UnityEngine;
using System.Collections;
using UnityEngine.UI;
using UnityEngine.EventSystems;

public class UISlider : Slider 
{
	public int controlID = 0;
	public UIMod uiMod;

	protected override void Start ()
	{
		base.Start ();
        
		// 挂载uiMod
		if( uiMod == null )
		{
			uiMod = gameObject.GetComponentInParent<UIMod>();
		}

        this.onValueChanged.AddListener( ValueChanged );
    }

    protected void ValueChanged(float value)
    {
        if( uiMod == null )
            return;

        object[] param = { UIEVENT.SLIDERVALUECHANGE, controlID, this.value };        
        uiMod.OnEvent(param);
    }
}
