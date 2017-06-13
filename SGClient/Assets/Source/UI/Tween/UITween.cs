using UnityEngine;
using System.Collections;
using DG.Tweening;

public abstract class UITween : MonoBehaviour
{
    public delegate void OnFinish( GameObject obj, int value );
    public OnFinish onFinish;

    public bool     isPlaying { get { if( _Tweener == null ) return false; else return _Tweener.IsPlaying(); } }

	public int 		controlID = 0;
	public UIMod	uiMod;
	public Ease		ease;
	public float	duration;
	public int		loop;
	public LoopType loopType;
	public float	delay;
	public bool		playOnEnable = false;
    public bool		playOnStart = false;

	protected bool			_Forward = true;
	protected Tweener		_Tweener;

    protected virtual void Start()
    {
        if ( playOnStart )
            Play( true );
    }

    protected virtual void OnEnable()
	{
		if( playOnEnable )
			Play( true );
	}

	protected virtual void OnDisable()
	{
        if( _Tweener != null )
		    _Tweener.Pause();
	}

	protected virtual void Awake()
	{
	}
	
	public virtual void ToInit()
	{
        if( _Tweener != null )
		    _Tweener.Kill( false );
	}
	
	public virtual void Play( bool forward )
	{
        if ( _Tweener != null )
        {
            _Tweener.SetEase( ease );
            _Tweener.OnComplete( OnFinished );
            _Tweener.SetLoops( loop, loopType );
            _Tweener.SetDelay( delay );

            _Forward = forward;
        }
	}

    public bool IsPlaying()
    {
        if( _Tweener == null )
            return false;

        return _Tweener.IsPlaying();
    }

	// 正向结束为0，逆向为1
	protected void OnFinished()
	{
        if( onFinish != null )
        {
            onFinish( gameObject, _Forward ? 0 : 1 );
        }

		if( uiMod != null )
        {
		    object[] param = { UIEVENT.TWEENFINISH, controlID, _Forward ? 0 : 1 };
		    uiMod.OnEvent( param );
        }
	}
}
