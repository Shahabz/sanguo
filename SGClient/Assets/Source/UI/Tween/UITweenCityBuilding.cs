using UnityEngine;
using System.Collections;
using DG.Tweening;

public class UITweenCityBuilding : UITween 
{
    public float y = 0.1f;
    private Vector3 from;
    private Vector3 to;

    //private new Ease ease = Ease.Linear;
    //private new float duration = 2;
    //private new int loop = -1;
    //private new LoopType loopType = LoopType.Yoyo;
    //private new bool playOnEnable = true;

    public override void ToInit()
    {
        base.ToInit();
        //transform.localPosition = from;
    }

	public override void Play( bool forward )
	{
        from = this.transform.localPosition;
        to = new Vector3( from.x, from.y + y, from.z );

        transform.localPosition = forward ? from : to;
        _Tweener = transform.DOLocalMove( forward ? to : from, duration );

        // 最后调
        base.Play( forward );
	}
}
