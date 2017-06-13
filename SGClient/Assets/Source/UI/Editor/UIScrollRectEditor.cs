using UnityEngine;
using UnityEditor;
using UnityEditor.UI;
using System.Collections;

[CustomEditor(typeof(UIScrollRect))]
public class UIScrollRectEditor : ScrollRectEditor
{
	UIScrollRect scr;
	public override void OnInspectorGUI(){
		scr = target as UIScrollRect;
		int controlID = EditorGUILayout.IntField( "ControlID", scr.controlID );
		if( scr.controlID != controlID )
		{
			scr.controlID = controlID;
		}
		UIMod uis = EditorGUILayout.ObjectField( "UIMod", scr.uiMod, typeof(UIMod), true ) as UIMod;
		if( scr.uiMod != uis )
		{
			scr.uiMod = uis;
		}
        scr.enableOnDragEvent = EditorGUILayout.Toggle("enableOnDragEvent", scr.enableOnDragEvent);
		base.OnInspectorGUI();
	}
}
