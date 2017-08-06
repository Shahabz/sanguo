
using UnityEngine;
using UnityEditor;
using UnityEditor.UI;
using System.Collections;

[CustomEditor(typeof(UITextTimeCountdown))]
public class UITextTimeCountdownEditor : UITextEditor
{
    public override void OnInspectorGUI()
    {
        UITextTimeCountdown txt = target as UITextTimeCountdown;
        int Interval = EditorGUILayout.IntField("Interval", txt.Interval);
        if (txt.Interval != Interval)
        {
            txt.Interval = Interval;
        }
        UITextTimeCountdown.DirectType direct = (UITextTimeCountdown.DirectType)EditorGUILayout.EnumPopup("Direct", txt.Direct);
        txt.Direct = direct;

        int controlID = EditorGUILayout.IntField("ControlID", txt.controlID);
        txt.controlID = controlID;

        //int duration = EditorGUILayout.IntField("Duration", txt.Duration);
        //if (duration != txt.Duration)
            //txt.Duration = duration;

        bool playOnEable = EditorGUILayout.Toggle("PlayOnEable", txt.playOnEable);
        txt.playOnEable = playOnEable;

		bool iso = EditorGUILayout.Toggle("iso", txt.iso);
		txt.iso = iso;

		bool showall = EditorGUILayout.Toggle("showall", txt.showall);
		txt.showall = showall;

		UIProgress uiProgress = EditorGUILayout.ObjectField( "UIProgress", txt.uiProgress, typeof(UIProgress), true ) as UIProgress;
		txt.uiProgress = uiProgress;

		UISlider uiSlider = EditorGUILayout.ObjectField( "UISlider", txt.uiSlider, typeof(UISlider), true ) as UISlider;
		txt.uiSlider = uiSlider;


        base.OnInspectorGUI();
    }
}