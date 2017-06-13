using UnityEngine;
using System.Collections;
using DG.Tweening;
using UnityEngine.UI;

[RequireComponent(typeof(CanvasGroup))]
public class UITweenFade : UITween
{
    public float from = 1.0f;
    public float to = 0.0f;

    CanvasGroup _canvasGroup;

	protected override void Awake()
	{
		base.Awake();
        _canvasGroup = GetComponent<CanvasGroup>();
	}

    protected override void OnEnable()
    {
        base.OnEnable();
    }

	public override void ToInit()
	{
		base.ToInit();

		_canvasGroup.alpha = from;
	}

	public override void Play( bool forward )
	{
		_canvasGroup.alpha = forward ? from : to;
        _Tweener = _canvasGroup.DOFade(forward ? to : from, duration);

		// 最后调
		base.Play( forward );
	}
}
