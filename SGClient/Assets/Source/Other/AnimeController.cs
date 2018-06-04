using UnityEngine;
using System.Collections;

public class AnimeController : MonoBehaviour
{
    Animator    _Animator;
    void Awake()
    {
        _Animator = GetComponent<Animator>();
        if( _Animator == null )
            Destroy( this );
    }

    public void CountUp()
    {
        _Animator.SetInteger( "Count", _Animator.GetInteger( "Count" ) + 1 );
    }

    public void CountDown()
    {        
        _Animator.SetInteger( "Count", _Animator.GetInteger( "Count" ) - 1 );
    }

    public void CountSet( int count )
    {
        _Animator.SetInteger( "Count", count );
    }
}
