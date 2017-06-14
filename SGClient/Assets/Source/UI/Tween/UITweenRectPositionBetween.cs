using UnityEngine;
using System.Collections;
using DG.Tweening;

[RequireComponent(typeof(RectTransform))]
public class UITweenRectPositionBetween : UITween 
{
    public float x = 0.0f;
    public float y = 0.0f;

    Vector2 	from;
	Vector2 	to;

	RectTransform 	_RectTransform;

	protected override void Awake()
	{
		base.Awake();
		_RectTransform = GetComponent<RectTransform>();
	}

	protected override void OnEnable()
	{
		base.OnEnable();
	}    
	public override void ToInit()
	{
		base.ToInit();

		_RectTransform.anchoredPosition = from;
	}

	public override void Play( bool forward )
	{		
        from = _RectTransform.anchoredPosition;
        to = new Vector2( from.x + x, from.y + y );

		_RectTransform.anchoredPosition = forward ? from : to;
		_Tweener = _RectTransform.DOAnchorPos( forward ? to : from, duration );

		// 最后调
		base.Play( forward );
	}
}
