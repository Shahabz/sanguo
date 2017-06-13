using UnityEngine;
using System.Collections;
using DG.Tweening;

public class UITweenLocalPositionBetween : UITween 
{
    public float x = 0.0f;
    public float y = 0.0f;
    public float z = 0.0f;

    private Vector3 from;
    private Vector3 to;

    public override void ToInit()
    {
        base.ToInit();
        transform.localPosition = from;
    }

	public override void Play( bool forward )
	{
        from = transform.localPosition;
        to = new Vector3( from.x + x, from.y + y, from.z + z );

        transform.localPosition = forward ? from : to;
        _Tweener = transform.DOLocalMove( forward ? to : from, duration );

        // 最后调
        base.Play( forward );
	}
}
