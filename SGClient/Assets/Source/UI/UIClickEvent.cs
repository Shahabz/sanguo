using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class UIClickEvent : MonoBehaviour {
	public int 			controlID = 0;
	public UIMod 		uiMod;
	Canvas canvas;
	RectTransform rectTransform;
	// Use this for initialization
	void Start () {
		// 挂载uiMod
		if( uiMod == null )
		{
			uiMod = gameObject.GetComponentInParent<UIMod>();
		}
		rectTransform = transform as RectTransform;
		canvas = GameObject.Find("UIRoot").GetComponent<Canvas>();
	}
	
	// Update is called once per frame
	void Update () {
		Vector2 touchpos = Vector2.zero;

#if UNITY_STANDALONE || UNITY_EDITOR
		if( Input.GetMouseButtonUp(0) )
		{
			touchpos = Input.mousePosition;

			if(RectTransformUtility.ScreenPointToLocalPointInRectangle(canvas.transform as RectTransform, Input.mousePosition, canvas.worldCamera, out touchpos)){
				//rectTransform.anchoredPosition = pos;
			}

			object[] param = { UIEVENT.CLICKPOS, controlID, touchpos };
			uiMod.OnEvent( param );
		}
#else
		if( Input.touchCount == 1 )
		{
			if( Input.GetTouch(0).phase == TouchPhase.Ended )
			{
				touchpos =  Input.GetTouch(0).position;
				object[] param = { UIEVENT.CLICKPOS, controlID, touchpos };
				uiMod.OnEvent( param );
			}
		}
#endif

		//object[] param = { obj, touchpos, unit_index };
		//LuaFun.worldMapOnClick.Call( param );

	}
		
}
