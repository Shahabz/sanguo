using UnityEngine;
using UnityEditor;
using UnityEditor.UI;
using System.Collections;

[CustomEditor(typeof(UITextUnderline))]
public class UITextUnderlineEditor : UITextEditor
{
	public override void OnInspectorGUI()
    {
		UITextUnderline text = target as UITextUnderline;
		bool showLine = EditorGUILayout.Toggle( "ShowLine", text.showLine );
		if( text.showLine != showLine )
		{
			text.showLine = showLine;
		}
		base.OnInspectorGUI();
	}
}
