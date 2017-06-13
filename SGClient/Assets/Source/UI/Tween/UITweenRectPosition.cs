using UnityEngine;
using System.Collections;
using DG.Tweening;

[RequireComponent(typeof(RectTransform))]
public class UITweenRectPosition : UITween 
{
	public Vector2 	from;
	public Vector2 	to;

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
		_RectTransform.anchoredPosition = forward ? from : to;
		_Tweener = _RectTransform.DOAnchorPos( forward ? to : from, duration );

		// 最后调
		base.Play( forward );
	}
}
