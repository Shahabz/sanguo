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
        if( _SpriteRender == null )
        { // 找到子组件
            for ( int i = 0; i < transform.childCount; i++ )
            {
                GameObject obj = transform.GetChild( i ).gameObject;
                if ( obj.activeSelf == false )
                    continue;
                _SpriteRender = obj.GetComponent<SpriteRenderer>();
                if ( _SpriteRender )
                    break;
            }
        }
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

    public override void Kill( bool complete )
    {
        if ( _Graphic == null && _SpriteRender == null )
        {
            Debug.LogError( "Can't Find Graphic or SpriteRender!" );
            return;
        }

        if ( _Graphic != null )
        {
            _Graphic.color = from;
        }
        else if ( _SpriteRender != null )
        {
            _SpriteRender.color = from;
        }

        // 最后调
        base.Kill( complete );
    }
}
