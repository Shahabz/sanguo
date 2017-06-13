using UnityEngine;
using System.Collections;
using DG.Tweening;

public class UITweenLocalRotation : UITween 
{
	public Vector3 	from;
	public Vector3 	to;

    public override void ToInit()
    {
        base.ToInit();
        transform.localEulerAngles = from;
    }

	public override void Play( bool forward )
	{
        transform.localEulerAngles = forward ? from : to;
        _Tweener = transform.DOLocalRotate( forward ? to : from, duration, RotateMode.FastBeyond360 );

        // 最后调
        base.Play( forward );
	}
}
