using UnityEngine;
using System.Collections;

using UnityEngine;
using System.Collections.Generic;
using System.Collections;

[ExecuteInEditMode]
[RequireComponent(typeof(LineRenderer))]
public class GizmoRing : MonoBehaviour {
	
	public int pointCount	= 10;
	public float radius		= 10f;

	float angle;
	List<Vector3>	_Points  	= new List<Vector3>();
	LineRenderer	_Renderer;

	void Start ()
	{
		_Renderer = GetComponent<LineRenderer>();
        _Renderer.useWorldSpace = false;

        Calculation_Points();
	}
	
	void Calculation_Points()
	{
		_Renderer.SetVertexCount( pointCount + 1 );
		angle = 360f / pointCount;
        _Points.Clear();
		for(int i = 1;i < pointCount + 1; i++ )
		{
			Quaternion q = Quaternion.Euler( 0, ( i - 1 ) * angle, 0 );
			Vector3 v = q * Vector3.forward * radius;
			_Points.Add( v );
		}
	}

	void Drow_Points()
	{
		for( int i = 0; i < _Points.Count; i++ )
		{
			_Renderer.SetPosition( i, _Points[i] );
		}
		if ( _Points.Count >  0)
			_Renderer.SetPosition( pointCount, _Points[0]);
	}

	void Update () 
	{
        // 计算，正式包注释
#if UNITY_EDITOR
		Calculation_Points();
#endif
        // 刷新
		Drow_Points();
	}
}
