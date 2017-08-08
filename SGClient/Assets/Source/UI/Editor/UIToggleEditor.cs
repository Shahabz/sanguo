using UnityEngine;
using UnityEditor;
using UnityEditor.UI;
using System.Collections;

[CustomEditor(typeof(UIToggle))]
public class UIToggleEditor : ToggleEditor
{
	UIToggle toggle;
	public override void OnInspectorGUI(){
		toggle = target as UIToggle;
		int controlID = EditorGUILayout.IntField( "ControlID", toggle.controlID );
		if( toggle.controlID != controlID )
		{
			toggle.controlID = controlID;
		}
		UIMod uis = EditorGUILayout.ObjectField( "UIMod", toggle.uiMod, typeof(UIMod), true ) as UIMod;
		if( toggle.uiMod != uis )
		{
			toggle.uiMod = uis;
		}
		base.OnInspectorGUI();
	}
}