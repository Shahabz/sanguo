using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEditor;


[CustomEditor(typeof(UITextFirstUpper))]
public class UITextFirstUpperEditor : UITextEditor
{
    UITextFirstUpper txt;
    public override void OnInspectorGUI()
    {
        base.OnInspectorGUI();

        txt = target as UITextFirstUpper;
        txt.UpperSize = EditorGUILayout.IntField("UpperSize", txt.UpperSize);
    }
}

