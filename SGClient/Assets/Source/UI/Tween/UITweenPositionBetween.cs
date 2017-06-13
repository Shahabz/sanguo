using UnityEngine;
using System.Collections;
using DG.Tweening;

public class UITweenPositionBetween : UITween
{
    public float x = 0.0f;
    public float y = 0.0f;
    public float z = 0.0f;

    private Vector3 from;
    private Vector3 to;

    public override void ToInit()
    {
        base.ToInit();
        transform.position = from;
    }

    public override void Play( bool forward )
    {
        from = transform.position;
        to = new Vector3( from.x + x, from.y + y, from.z + z );

        transform.position = forward ? from : to;
        _Tweener = transform.DOMove( forward ? to : from, duration );

        // 最后调
        base.Play( forward );
    }
}
