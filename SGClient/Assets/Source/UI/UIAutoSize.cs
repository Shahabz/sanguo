using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class UIAutoSize : MonoBehaviour {

	public UIText uitext = null;
	public YlyRichText uylytext = null;
	public float offsetx = 0;
	public float offsety = 0;
	bool bDirty = false;

	public enum AutoMode
	{
		Width,
		Height,
		All
	}
	public AutoMode	mode = AutoMode.All;

	// Use this for initialization
	void Start () 
	{
		
	}
	
	// Update is called once per frame
	void Update () 
	{
		if (bDirty == true) 
		{
			bDirty = false;
			var rt = gameObject.GetComponent<RectTransform>();

			if (uitext != null) 
			{
				if (mode == AutoMode.Width) {
					rt.sizeDelta = new Vector2 (uitext.preferredWidth + offsetx, rt.sizeDelta.y);
				} else if (mode == AutoMode.Height) {
					rt.sizeDelta = new Vector2 (rt.sizeDelta.x, uitext.preferredHeight + offsety);
				} else if (mode == AutoMode.All) {
					rt.sizeDelta = new Vector2 (uitext.preferredWidth + offsetx, uitext.preferredHeight + offsety);
				}
			}
			else if ( uylytext != null )
			{
				if (mode == AutoMode.Width) {
					rt.sizeDelta = new Vector2 (uylytext.preferredWidth + offsetx, rt.sizeDelta.y);
				} else if (mode == AutoMode.Height) {
					rt.sizeDelta = new Vector2 (rt.sizeDelta.x, uylytext.preferredHeight + offsety);
				} else if (mode == AutoMode.All) {
					rt.sizeDelta = new Vector2 (uylytext.preferredWidth + offsetx, uylytext.preferredHeight + offsety);
				}
			}
		}
	}

	public void Dirty()
	{
		bDirty = true;
	}
}
