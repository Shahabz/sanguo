using UnityEngine;
using System.Collections;
using DG.Tweening;

public class UITweenSizeDelta : UITween
{
    public Vector2 from;
    public Vector2 to;

    public override void ToInit()
    {
        base.ToInit();
        GetComponent<RectTransform>().sizeDelta = from;
    }

    public override void Play( bool forward )
    {
        GetComponent<RectTransform>().sizeDelta = forward ? from : to;
        _Tweener = GetComponent<RectTransform>().DOSizeDelta( forward ? to : from, duration );

        // 最后调
        base.Play( forward );
    }
}
