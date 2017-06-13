using UnityEngine;
using UnityEditor;
using UnityEditor.UI;
using System.Collections;

[CustomEditor(typeof(UIText))]
public class UITextEditor : UnityEditor.UI.TextEditor
{
	public override void OnInspectorGUI()
    {
		UIText txt = target as UIText;
		int localizeID = EditorGUILayout.IntField( "LocalizeID", txt.localizeID );
		if( txt.localizeID != localizeID )
		{
            if( localizeID != null )
            {
			    txt.localizeID = localizeID;

                if ( Localization.bInited == false )
                    Localization.init();
                txt.text = Localization.text( localizeID );
                txt.enabled = false;
                txt.enabled = true;
            }
		}

        bool rtlAlignment = EditorGUILayout.Toggle( "RtlAlignment", txt.rtlAlignment );
        if( rtlAlignment != txt.rtlAlignment )
        {
            txt.rtlAlignment = rtlAlignment;
        }

        string rtlReverseSign = EditorGUILayout.TextField( "RtlReverseSign", txt.rtlReverseSign );
        if( rtlReverseSign != txt.rtlReverseSign )
        {
            txt.rtlReverseSign = rtlReverseSign;
        }

		base.OnInspectorGUI();
	}
}
