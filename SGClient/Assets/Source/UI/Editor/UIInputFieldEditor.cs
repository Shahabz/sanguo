using UnityEngine;
using UnityEditor;
using UnityEditor.UI;
using System.Collections;

[CustomEditor(typeof(UIInputField))]
public class UIInputFieldEditor : UnityEditor.UI.InputFieldEditor
{
    public override void OnInspectorGUI()
    {
        UIInputField txt = target as UIInputField;
        txt.IsAutoClose = EditorGUILayout.Toggle("IsAutoClose", txt.IsAutoClose);
        int controlID = EditorGUILayout.IntField("ControlID", txt.controlID);
        if (txt.controlID != controlID)
        {
            txt.controlID = controlID;
        }
        base.OnInspectorGUI();
    }
}
