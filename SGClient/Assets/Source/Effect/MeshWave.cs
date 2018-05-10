using UnityEngine;
using System.Collections;

public class FXWave : MonoBehaviour
{
    public float    scale = 2.0f;
    public float    speed = 2.0f;
    public bool     useOriginal = false;
    public bool     refreshNormals = false;

    public Vector2[] fixedVertex;

    Mesh            _Mesh;
    Vector3[]       _BaseHeight;

	void Start () 
    {
	    _Mesh = GetComponent<MeshFilter>().mesh;
        _BaseHeight = _Mesh.vertices;
	}
	
	void Update () 
    {
	    Vector3[] vertices = new Vector3[_BaseHeight.Length];
        for( int i = 0; i < vertices.Length; i++ )
        {
            Vector3 vertex = _BaseHeight[i];
            if( IsFixedVertex( i ) )
            {
                vertices[i] = vertex;
                continue;
            }

            if( useOriginal )
            {
                vertex.y += Mathf.Sin( Time.time * speed+ _BaseHeight[i].x + _BaseHeight[i].y + _BaseHeight[i].z ) * scale;
            }
            else
            {
                vertex.y += Mathf.Sin( Time.time * speed+ _BaseHeight[i].x + _BaseHeight[i].y ) * ( scale * 0.5f ) + Mathf.Sin(Time.time * speed+ _BaseHeight[i].z + _BaseHeight[i].y ) * ( scale * 0.5f );
            }

            vertices[i] = vertex;
        }
        _Mesh.vertices = vertices;

        if( refreshNormals )
            _Mesh.RecalculateNormals();
	}

    bool IsFixedVertex( int index )
    {
        for( int i = 0; i < fixedVertex.Length; i++ )
        {
            if( index >= fixedVertex[i].x && index <= fixedVertex[i].y )
                return true;
        }
        return false;
    }
}
