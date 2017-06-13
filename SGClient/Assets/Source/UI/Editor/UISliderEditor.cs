using UnityEngine;
using UnityEditor;
using UnityEditor.UI;
using System.Collections;

[CustomEditor(typeof(UISlider))]
public class UISliderEditor : SliderEditor
{
	UISlider slider;
	public override void OnInspectorGUI(){
		slider = target as UISlider;
		int controlID = EditorGUILayout.IntField( "ControlID", slider.controlID );
		if( slider.controlID != controlID )
		{
			slider.controlID = controlID;
		}
		UIMod uis = EditorGUILayout.ObjectField( "UIMod", slider.uiMod, typeof(UIMod), true ) as UIMod;
		if( slider.uiMod != uis )
		{
			slider.uiMod = uis;
		}
		base.OnInspectorGUI();
	}
}
