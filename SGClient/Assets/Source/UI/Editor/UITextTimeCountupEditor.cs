using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEditor;

[CustomEditor(typeof(UITextTimeCountup))]
public class UITextTimeCountupEditor : UITextEditor
{
    public override void OnInspectorGUI()
    {
        UITextTimeCountup txt = target as UITextTimeCountup;
        int controlID = EditorGUILayout.IntField("ControlID", txt.controlID);
        if (txt.controlID != controlID)
        {
            txt.controlID = controlID;
        }
        UIMod uis = EditorGUILayout.ObjectField("UIMod", txt.uiMod, typeof(UIMod), true) as UIMod;
        if (txt.uiMod != uis)
        {
            txt.uiMod = uis;
        }
        base.OnInspectorGUI();
    }
}

