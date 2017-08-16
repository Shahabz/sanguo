using System.Collections;
using System.Collections.Generic;
using System.Reflection;
using System.IO;
using System.Text;
using System;

using UnityEngine;
using UnityEngine.UI;

public class UITextAgent : MonoBehaviour {

	public enum TextAgentMode
	{
		PerUp,
		PerDown,
	}
	public UIText _uiText = null;
	public string _formatText = string.Empty;
	public TextAgentMode	_mode = TextAgentMode.PerUp;

	// Use this for initialization
	void Start () {
		_uiText = gameObject.GetComponent<UIText>();
	}
	
	public void setValue( int leftTime, int Duration )
	{
		if (_uiText) 
		{
			if (_mode == TextAgentMode.PerUp) 
			{
				if (_formatText != string.Empty) {
					_uiText.text = string.Format (_formatText, (int)((Duration-leftTime)/(float)Duration * 100));
				} else {
					_uiText.text = string.Format ("{0}%",(int)((Duration-leftTime)/(float)Duration * 100));
				}		
			}
			else
			{
				if (_formatText != string.Empty) {
					_uiText.text = string.Format (_formatText,(int)((leftTime)/(float)Duration * 100));
				} else {
					_uiText.text =  string.Format ("{0}%",(int)((leftTime)/(float)Duration * 100));
				}		
			}

		}
	}
}
