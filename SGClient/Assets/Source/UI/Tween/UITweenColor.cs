using UnityEngine;
using System.Collections;
using DG.Tweening;
using UnityEngine.UI;

public class UITweenColor : UITween 
{
    public Color from = Color.white;
    public Color to = Color.white;

    Graphic _Graphic;
    SpriteRenderer _SpriteRender;

	protected override void Awake()
	{
		base.Awake();
        _Graphic = GetComponent<Graphic>();
        if( _Graphic == null )
            _SpriteRender = GetComponent<SpriteRenderer>();
	}

    protected override void OnEnable()
    {
        base.OnEnable();
	}
	
	public override void ToInit()
	{
		base.ToInit();
		if( _Graphic != null )
		{
			_Graphic.color = from;
		}
        else if( _SpriteRender != null )
        {
            _SpriteRender.color = from;
        }
	}

	public override void Play( bool forward )
	{
		if ( _Graphic == null && _SpriteRender == null )
		{
            Debug.LogError("Can't Find Graphic or SpriteRender!");
            return;
		}

		if( _Graphic != null )
		{
			_Graphic.color = forward ? from : to;
	        _Tweener = _Graphic.DOColor(forward ? to : from, duration);
		}
        else if( _SpriteRender != null )
        {
            _SpriteRender.color = forward ? from : to;
            _Tweener = _SpriteRender.DOColor( forward ? to : from, duration );
        }

		// 最后调
		base.Play( forward );
	}
}
