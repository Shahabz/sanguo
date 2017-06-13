using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using UnityEngine.UI;

[AddComponentMenu( "UI/Effects/UIGradient" )]
[RequireComponent(typeof(UIText))]
public class UIGradient : BaseMeshEffect
{
    public Color32 topColor = Color.white;
    public Color32 bottomColor = Color.black;

    public override void ModifyMesh( VertexHelper mesh )
    {
        if ( !IsActive() )
        {
            return;
        }

        for( int i = 0; i < mesh.currentVertCount; i++ )
        {
            UIVertex uv = new UIVertex();
            mesh.PopulateUIVertex( ref uv, i );
            uv.color = i % 4 < 2 ? topColor : bottomColor;
            mesh.SetUIVertex( uv, i );
        }
    }
}