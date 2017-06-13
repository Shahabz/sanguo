using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEditor;

[CustomEditor(typeof(UITextUnderlineEx))]
public class UITextUnderlineExEditor : UITextEditor
{
    public override void OnInspectorGUI()
    {
        base.OnInspectorGUI();
        UITextUnderlineEx text = target as UITextUnderlineEx;
        bool showLine = EditorGUILayout.Toggle("ShowLine", text.ShowLine);
        if (text.ShowLine != showLine)
        {
            text.ShowLine = showLine;
        }
        
    }
}

