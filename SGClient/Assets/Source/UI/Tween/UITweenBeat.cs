using UnityEngine;
using System.Collections;
using DG.Tweening;
using UnityEngine.UI;

public class UITweenBeat : UITween
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
        transform.localScale = from;
    }

    public override void Play( bool forward )
    {
        transform.localScale = forward ? from : end;

        _tweener1 = transform.DOScale( forward ? to : end, duration );
        _tweener2 = transform.DOScale( forward ? end : from, duration );

        mySequence.Append( _tweener1 );
        mySequence.Append( _tweener2 );
        mySequence.SetEase( ease );
        mySequence.SetDelay( delay );
        mySequence.SetLoops( loop, loopType );

        _Forward = forward;
        // 最后调
        //base.Play( forward );
    }
}
