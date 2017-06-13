using UnityEngine;
using System.Collections;

// 曲线移动，目前仅限localPosition
public class BezierMove : MonoBehaviour
{
    public Vector3[]    point = new Vector3[4];
    public float        duration;
    public float        delay;
    public bool         playOnEnable;

    Bezier              _Bezier;
    Coroutine           _MoveCoroutine;
    bool                _Started = false;

    void Start()
    {
        if( playOnEnable )
            Play();

        _Started = true;
    }

    void OnEnable()
    {
        if( playOnEnable && _Started )
            Play();
    }

    public void Play()
    {
        if( duration <= 0 )
            return;

        if( _Bezier == null )
            _Bezier = new Bezier( point[0], point[1], point[2], point[3] );
        else
            _Bezier.Set( point[0], point[1], point[2], point[3] );

        if( _MoveCoroutine != null )
            StopCoroutine( _MoveCoroutine );
        _MoveCoroutine = StartCoroutine( Move() );
    }

    IEnumerator Move()
    {
        float timer = 0;
        transform.localPosition = point[0];

        yield return new WaitForSeconds( delay );

        while( timer < duration )
        {
            timer += Time.deltaTime;
            transform.localPosition = _Bezier.Lerp( Mathf.Min( timer / duration, 1 ) );
            yield return null;
        }
    }
}
