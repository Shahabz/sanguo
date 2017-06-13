using UnityEngine;
using System.Collections;

[ExecuteInEditMode]
[RequireComponent(typeof(LineRenderer))]

public class GizmoBezierLine : MonoBehaviour
{
    public Vector3[]    points;
    public int          smooth;

    Bezier              _Bezier;
	LineRenderer	    _Renderer;

    void Awake()
    {
		_Renderer = GetComponent<LineRenderer>();
        _Bezier = new Bezier( points[0], points[1], points[2], points[3] );
    }

    public void Refresh()
    {        
        if( smooth <= 0 )
            smooth = 8;

		_Renderer.SetVertexCount( smooth );
		for( int i = 0; i < smooth; i++ )
		{
            _Bezier.Set( points[0], points[1], points[2], points[3] );
			_Renderer.SetPosition( i, _Bezier.Lerp( (float)i / (float)( smooth - 1 ) ) );
		}
    }

    void Update()
    {
        Refresh();
    }
}
