using UnityEngine;
using UnityEditor;
using UnityEditor.UI;
using System.Collections;

[CustomEditor( typeof( UISystem ) )]
public class UISystemEditor : Editor
{
    UISystem uisys;
    public override void OnInspectorGUI()
    {
        base.DrawDefaultInspector();
        uisys = target as UISystem;
        if ( GUILayout.Button( "自动输出Lua变量" ) )
        {
            string str = string.Empty;
            str = "--------------------------------------------------------------\n";
            foreach ( GameObject obj in uisys.relatedGameObject )
            {
                string localstr = string.Empty;
                localstr = string.Format( "local m_ui{0} = nil; --{1}\n", obj.name, obj.GetType() );
                str += localstr;
            }
            Debug.LogError( str );

            int index = 0;
            str = string.Empty;
            foreach ( GameObject obj in uisys.relatedGameObject )
            {
                string localstr = string.Empty;
                localstr = string.Format( "m_ui{0} = objs[{1}];\n", obj.name, index );
                str += localstr;
                index += 1;
            }
            Debug.LogError( str );
        }
    }
}
