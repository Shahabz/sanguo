using UnityEngine;
using System.Collections;
using DG.Tweening;
using UnityEngine.UI;

public class UITweenJump : UITween
{
    public Vector3 from;
    public Vector3 to;
    public Vector3 end;

    Sequence mySequence;

    Tweener	_tweener1;
    Tweener	_tweener2;

    protected override void Awake()
    {
        base.Awake();
        mySequence = DOTween.Sequence();
    }

    protected override void OnEnable()
    {
        base.OnEnable();
    }
	public override void ToInit()
	{
		base.ToInit();
		transform.localPosition = from;
	}	
    public override void Play( bool forward )
    {
		transform.localPosition = forward ? from : end;
		_tweener1 = transform.DOLocalMove( forward ? to : end, duration/2 );
		_tweener2 = transform.DOLocalMove( forward ? end : from, duration/2 );

        mySequence.Append( _tweener1 );
        mySequence.Append( _tweener2 );
        mySequence.SetEase( ease );
        mySequence.SetLoops( loop, loopType );
		mySequence.SetDelay( delay );
        _Forward = forward;
        // 最后调
        //base.Play( forward );
    }
}
