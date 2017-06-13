using UnityEngine;
using System.Collections;
using DG.Tweening;

public class UITweenScale : UITween 
{
	public Vector3 	from;
	public Vector3 	to;

    protected override void OnEnable()
    {
        base.OnEnable();
    }

    public override void ToInit()
    {
        base.ToInit();
        transform.localScale = from;
    }

	public override void Play( bool forward )
	{
        transform.localScale = forward ? from : to;
        _Tweener = transform.DOScale( forward ? to : from, duration );

        // 最后调
        base.Play( forward );
	}
}
