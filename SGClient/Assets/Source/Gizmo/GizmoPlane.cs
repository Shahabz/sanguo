using UnityEngine;
using System.Collections;

//[ExecuteInEditMode]
[RequireComponent(typeof(MeshFilter))]
[RequireComponent(typeof(MeshRenderer))]
public class GizmoPlane : MonoBehaviour 
{
	public Vector3[] 			vertices;
	public Vector2[] 			uv;
	public int[] 				triangles;

	MeshFilter					_MeshFilter;
	MeshRenderer				_MeshRender;

	void Awake()
	{
		_MeshFilter = GetComponent<MeshFilter>();
		if( _MeshFilter == null )
			_MeshFilter = gameObject.AddComponent<MeshFilter>();

		_MeshRender = GetComponent<MeshRenderer>();
		if( _MeshRender == null )
			_MeshRender = gameObject.AddComponent<MeshRenderer>();
	}

    void Start()
    {
        Set();
    }

    public void Set()
    {
        Set(vertices, uv, triangles);
    }

	public void Set( Vector3[] newVertices, Vector2[] newUV, int[] newTriangles )
	{
		if( _MeshFilter.sharedMesh == null )
			_MeshFilter.sharedMesh = new Mesh();

		_MeshFilter.sharedMesh.Clear();
		vertices = newVertices;
		uv = newUV;
		triangles = newTriangles;

		_MeshFilter.sharedMesh.vertices = newVertices;
		_MeshFilter.sharedMesh.uv = newUV;
		_MeshFilter.sharedMesh.triangles = newTriangles;
	}

	public void SetColor( Color color )
	{
		_MeshRender.material.color = color;
	}

    //void OnDestroy()
    //{
    //    DestroyImmediate( _MeshRender.material );
    //}

//#if UNITY_EDITOR
//    void Update()
//    {
//        Set( vertices, uv, triangles );
//    }
//#endif
}
