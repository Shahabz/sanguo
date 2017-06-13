using UnityEngine;
using UnityEditor;
using UnityEditor.UI;
using System.Collections;

[CustomEditor(typeof(UIButton))]
public class UIButtonEditor : ButtonEditor
{
	UIButton btn;
	public override void OnInspectorGUI(){
		btn = target as UIButton;
		int controlID = EditorGUILayout.IntField( "ControlID", btn.controlID );
		if( btn.controlID != controlID )
		{
			btn.controlID = controlID;
		}

        int clickInterval = EditorGUILayout.IntField( "ClickInterval", btn.clickInterval );
        if ( btn.clickInterval != clickInterval )
        {
            btn.clickInterval = clickInterval;
        }

        int audioID = EditorGUILayout.IntField("AudioID", btn.audioID);
        if(audioID != btn.audioID)
        {
            btn.audioID = audioID;
        }

		UIMod uis = EditorGUILayout.ObjectField( "UIMod", btn.uiMod, typeof(UIMod), true ) as UIMod;
		if( btn.uiMod != uis )
		{
			btn.uiMod = uis;
		}
		base.OnInspectorGUI();
	}
}
