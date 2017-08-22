using UnityEngine;
using System.Collections;

[RequireComponent(typeof(GizmoPlane))]
public class MapBorder : MonoBehaviour
{
    public Vector2          cellSize = new Vector2( 2.56f, 1.28f );
    public float            speed = 6f;
    public float            waitOnMax = 1;
    public float            space = 0.6f;

    GizmoPlane              _GizimoPlane;
    Coroutine               _PlaneCoroutine;
    float                   _Size;

    void Awake()
    {
        _GizimoPlane = GetComponent<GizmoPlane>();
    }

    void OnEnable()
    {
        if( _PlaneCoroutine != null )
                _PlaneCoroutine = StartCoroutine( UpdatePlane( _Size ) );
    }

    public void PlaySize( float size )
    {
        _Size = size;

        if( _PlaneCoroutine != null )
            return;

        _PlaneCoroutine = StartCoroutine( UpdatePlane( size ) );
    }

    public void SetSize( float size )
    {
        float ratio = cellSize.y / cellSize.x;
        // 控制8个点移动方向
        _GizimoPlane.vertices[0].y = cellSize.y * size / 2;
        _GizimoPlane.vertices[1].y = cellSize.y * size / 2 - space * ratio;
        _GizimoPlane.vertices[2].x = cellSize.x * size / 2;
        _GizimoPlane.vertices[3].x = cellSize.x * size / 2 - space;
        _GizimoPlane.vertices[4].y = -cellSize.y * size / 2;
        _GizimoPlane.vertices[5].y = -cellSize.y * size / 2 + space * ratio;
        _GizimoPlane.vertices[6].x = -cellSize.x * size / 2;
        _GizimoPlane.vertices[7].x = -cellSize.x * size / 2 + space;

        _GizimoPlane.Set();
    }

    public void SetColor( Color color )
    {
        _GizimoPlane.SetColor( color );
    }

    public void Stop()
    {
        if( _PlaneCoroutine == null )
            return;

        StopCoroutine( _PlaneCoroutine );
        _PlaneCoroutine = null;
    }

    IEnumerator UpdatePlane( float size )
    {
        float currSize = 0;
        // 算出单位长度

        while( true )
        {
            SetSize( currSize );
            // 如果扩散到最大，停waitOnMax
            if( currSize == size )
            {
                yield return new WaitForSeconds( waitOnMax );
                currSize = 0;
                continue;
            }

            currSize += Time.deltaTime * speed;            
            if( currSize > size )
                currSize = size;

            yield return null;
        }
        
    }
}
