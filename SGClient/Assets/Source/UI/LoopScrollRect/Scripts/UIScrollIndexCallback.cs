using UnityEngine;
using UnityEngine.UI;
using System.Collections;

public class UIScrollIndexCallback : MonoBehaviour 
{

	public int controlID = 0;
	public UIMod uiMod;

	void Start ()
	{
		// 挂载uiMod
		if( uiMod == null )
		{
			uiMod = gameObject.GetComponentInParent<UIMod>();
		}
	}
		
    void ScrollCellIndex (int idx) 
    {
		//string name = "Cell " + idx.ToString ();
		//gameObject.name = name;

		if( uiMod == null )
			return;

		object[] param = { UIEVENT.SCROLLINDEXCALLBACK, controlID, idx };        
		uiMod.OnEvent(param);
	}
		
}
