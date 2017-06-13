using UnityEngine;
using System.Collections;
using DG.Tweening;

public class UITweenPosition : UITween 
{
	public Vector3 	from;
	public Vector3 	to;

    public override void ToInit()
    {
        base.ToInit();
        transform.position = from;
    }

	public override void Play( bool forward )
	{
        transform.position = forward ? from : to;
        _Tweener = transform.DOMove( forward ? to : from, duration );

        // 最后调
        base.Play( forward );
	}
}
