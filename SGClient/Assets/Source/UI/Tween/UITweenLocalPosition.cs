using UnityEngine;
using System.Collections;
using DG.Tweening;

public class UITweenLocalPosition : UITween 
{
	public Vector3 	from;
	public Vector3 	to;

    public override void ToInit()
    {
        base.ToInit();
        transform.localPosition = from;
    }

	public override void Play( bool forward )
	{
        transform.localPosition = forward ? from : to;
        _Tweener = transform.DOLocalMove( forward ? to : from, duration );

        // 最后调
        base.Play( forward );
	}
}
