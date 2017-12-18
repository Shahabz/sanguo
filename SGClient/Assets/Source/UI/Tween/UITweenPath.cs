using UnityEngine;
using System.Collections;
using DG.Tweening;
using UnityEngine.UI;

[RequireComponent(typeof(RectTransform))]
public class UITweenPath : UITween {

	public Vector2 	from;
	public Vector2[] path;
	public float[] path_duration;
	RectTransform 	_RectTransform;
	Sequence mySequence;

	protected override void Awake()
	{
		base.Awake();
		mySequence = DOTween.Sequence();
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
		transform.localPosition = from;

		for (int tmpi = 0; tmpi < path.Length; tmpi++) 
		{
			mySequence.Append( _RectTransform.DOAnchorPos ( path[tmpi], path_duration[tmpi] ) );
		}

		mySequence.SetEase( ease );
		mySequence.SetDelay( delay );
		mySequence.SetLoops( loop, loopType );

		_Forward = forward;
		// 最后调
		//base.Play( forward );
	}
}
