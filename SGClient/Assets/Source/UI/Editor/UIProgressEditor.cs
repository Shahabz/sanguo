using UnityEngine;
using UnityEditor;
using UnityEditor.UI;
using System.Collections;

[CustomEditor( typeof( UIProgress ) )]
public class UIProgressEditor : Editor
{
    UIProgress progress;
    float hSliderValue = 1.0f;
    public override void OnInspectorGUI()
    {
        progress = target as UIProgress;
        hSliderValue = GUILayout.HorizontalSlider( hSliderValue, 0.0f, 1.0f );
        progress.SetValue( hSliderValue );
        base.OnInspectorGUI();
    }

}

