using UnityEngine;
using System.Collections;
using UnityEngine.UI;
using UnityEngine.EventSystems;

public class UIButtonAgent : UIButton
{
    public UIButton button;

	public override void OnPointerClick( PointerEventData eventData )
	{
		base.OnPointerClick( eventData );
        if( button )
            button.OnPointerClick( eventData );
	}
	
	public override void OnPointerDown( PointerEventData eventData )
	{
        base.OnPointerDown( eventData );
        if( button )
            button.OnPointerDown( eventData );
	}	
	
	public override void OnPointerUp( PointerEventData eventData )
	{
		base.OnPointerUp( eventData );
        if( button )
            button.OnPointerUp( eventData );
	}
}
