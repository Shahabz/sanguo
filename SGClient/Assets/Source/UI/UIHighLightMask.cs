using UnityEngine;
using System.Collections;
using UnityEngine.UI;
using System.Collections.Generic;
using UnityEngine.Serialization;

public class UIHighLightMask : MaskableGraphic
{
    public Vector2 center = Vector2.zero;
    public Vector2 size = new Vector2( 100, 100 );

    protected override void OnPopulateMesh( VertexHelper vbo )
    {    
        if ( !IsActive() )
        {
            return;
        }

        Vector4 outer = new Vector4( -rectTransform.pivot.x * rectTransform.rect.width, 
                                    -rectTransform.pivot.y * rectTransform.rect.height,
                                    ( 1 - rectTransform.pivot.x ) * rectTransform.rect.width,
                                    ( 1 - rectTransform.pivot.y ) * rectTransform.rect.height );

        Vector4 inner = new Vector4( center.x - size.x / 2,
                                    center.y - size.y / 2,
                                    center.x + size.x * 0.5f,
                                    center.y + size.y * 0.5f );

        vbo.Clear();

        UIVertex[] vert = new UIVertex[4];

        // left
        vert[0].position = new Vector2(outer.x, outer.y);
        vert[0].color = color;

        vert[1].position = new Vector2(outer.x, outer.w);
        vert[1].color = color;

        vert[2].position = new Vector2(inner.x, outer.w);
        vert[2].color = color;

        vert[3].position = new Vector2(inner.x, outer.y);
        vert[3].color = color;
        vbo.AddUIVertexQuad( vert );

        // top
        vert[0].position = new Vector2(inner.x, inner.w);
        vert[0].color = color;

        vert[1].position = new Vector2(inner.x, outer.w);
        vert[1].color = color;

        vert[2].position = new Vector2(inner.z, outer.w);
        vert[2].color = color;

        vert[3].position = new Vector2(inner.z, inner.w);
        vert[3].color = color;
        vbo.AddUIVertexQuad( vert );

        // right
        vert[0].position = new Vector2(inner.z, outer.y);
        vert[0].color = color;

        vert[1].position = new Vector2(inner.z, outer.w);
        vert[1].color = color;

        vert[2].position = new Vector2(outer.z, outer.w);
        vert[2].color = color;

        vert[3].position = new Vector2(outer.z, outer.y);
        vert[3].color = color;
        vbo.AddUIVertexQuad( vert );

        // bottom
        vert[0].position = new Vector2(inner.x, outer.y);
        vert[0].color = color;

        vert[1].position = new Vector2(inner.x, inner.y);
        vert[1].color = color;

        vert[2].position = new Vector2(inner.z, inner.y);
        vert[2].color = color;

        vert[3].position = new Vector2(inner.z, outer.y);
        vert[3].color = color;
        vbo.AddUIVertexQuad( vert );
    }
}