using UnityEngine;
using System;
using System.Collections;
using UnityEngine.EventSystems;
using UnityEngine.Events;
using UnityEngine.UI;

public abstract class UIScrollBase : UIBehaviour, IEventSystemHandler, IBeginDragHandler, IInitializePotentialDragHandler, IDragHandler, IEndDragHandler
{
	public enum MovementType
	{
		Horizon,
		Vertical
	}
    
    // 基础
	public int				controlID;
	public UIMod			uiMod;
	public RectTransform	content;
    public RectTransform    viewPort;
    public bool             enableDragginEvent = false;

    public bool             initOnStart = false;

    // 拖动
	public MovementType		moveType;
    public float            limitExpand = 0f;
    public float            stopLimit = 0.5f;
	public bool				monentumEnable = true;
	public float			momentumDecay = 0.1f;
	public float			momentumLimit = 15f;
    public float            momentBoost = 1f;
	public bool				dragInverse = false;
	public float			dragSpeed = 0.5f;
    public float            dragThreshold = 0.15f;

	// 孩子
	public bool				centerOnChild = true;
    public float            centerOnChildDecay = 0.33f;
	public float			centerOnChildFlap = 10f;
    public bool				fadeFromCenter = false;
    public float			fadeRatio = 0.01f;
    public bool				scaleFromCenter = false;
    public float			scaleRatio = 0.5f;
    public bool				darkFromCenter = false;
    public float			darkRatio = 0.01f;
    
    // 拖动
	protected Vector2 		_PointerStartLocalCursor;
    protected float         _DragAmount;
    protected bool          _DragBegin;
    protected bool          _Dragging;
    protected float         _Momentum;

    // 滚动
    protected float			_OriginalValue;
    protected float			_StartValue;
	protected float			_CurrValue;
	protected float			_LastValue;
	protected float			_ChildCenterValue;
	protected float			_FlapValue;
    protected Vector2       _ValueLimit;
    protected float         _ValuePerChild;
    protected int           _ChildCount;
	protected int		    _ChildCenterIndex;

	protected override void Start ()
    {
		if( content == null )
		{
			Debug.LogError( "content is null! Set it!" );
            enabled = false;
			return;
		}

        if( initOnStart )
            ResetChildren();
	}
	
	protected virtual void LateUpdate()
	{
		// 记录动量
		if( _DragBegin && monentumEnable )
		{
			if( _LastValue != _CurrValue )
			{
				_Momentum = _CurrValue - _LastValue;
				_LastValue = _CurrValue;

				if( _Momentum > momentumLimit )
					_Momentum = momentumLimit;
				else if( _Momentum < -momentumLimit )
					_Momentum = -momentumLimit;

                //print( _Momentum );
			}
			else
				_Momentum = 0f;
		}
		else if( content != null )
		{
			// 动量移动
			if( monentumEnable && _Momentum != 0 )
			{
				_Momentum -= _Momentum * momentumDecay;
				if( Mathf.Abs( _Momentum ) < stopLimit )
					_Momentum = 0;

				if( _Momentum != 0 )
				{
                    if( _CurrValue < _ValueLimit.x || _CurrValue > _ValueLimit.y )
                        _CurrValue += _Momentum * 0.2f;
                    else
                        _CurrValue += _Momentum;

					OnScroll();
				}
				else if( centerOnChild )
					// 动量模式这里处理孩子居中
					ProcessCenterOnChild();
			}
			// 孩子位置居中
			else if( centerOnChild && _ChildCenterValue != _CurrValue )
			{
				if( Mathf.Abs( _ChildCenterValue - _CurrValue ) < stopLimit )
				{
					_CurrValue = _ChildCenterValue;

					if( _FlapValue != 0f )
					{
						_ChildCenterValue -= _FlapValue;
						_FlapValue = 0f;
					}
					else
					{
                        OnCenterOnChild();
					}
				}
				
				_CurrValue += ( _ChildCenterValue - _CurrValue ) / 3f;
				
				OnScroll();
			}
			// 超出范围
			else
			{
				if( _CurrValue > _ValueLimit.y )
				{					
					if( _CurrValue - _ValueLimit.y < stopLimit )
						_CurrValue = _ValueLimit.y;

					_CurrValue -= ( _CurrValue - _ValueLimit.y ) / 10f;

					OnScroll();
				}
				else if( _CurrValue < _ValueLimit.x )
				{					
					if( _ValueLimit.x - _CurrValue < stopLimit )
						_CurrValue = _ValueLimit.x;

					_CurrValue += ( _ValueLimit.x - _CurrValue ) / 10f;
					
					OnScroll();
				}
			}
		}
	} 
		
	public virtual void OnBeginDrag (PointerEventData eventData)
	{
        if ( eventData.button != PointerEventData.InputButton.Left )
        {
            return;
        }
        if ( !this.IsActive() )
        {
            return;
        }

        Vector2 vector;
        if ( !RectTransformUtility.ScreenPointToLocalPointInRectangle( viewPort, eventData.position, eventData.pressEventCamera, out vector ) )
        {
            return;
        }

        _DragBegin = true;
        _PointerStartLocalCursor = vector;
	}    
	
	public virtual void OnDrag (PointerEventData eventData)
	{
		if (eventData.button != PointerEventData.InputButton.Left)
		{
			return;
		}

		Vector2 vector;
		if ( !RectTransformUtility.ScreenPointToLocalPointInRectangle ( viewPort, eventData.position, eventData.pressEventCamera, out vector ) )
		{
			return;
		}

		Vector2 vector2 = vector - _PointerStartLocalCursor;
		switch( moveType )
		{
		case MovementType.Horizon:
            if( !_Dragging )
            {
                _DragAmount += vector2.x;
                if( Math.Abs( _DragAmount ) >= dragThreshold )
                {
                    _Dragging = true;

                    _PointerStartLocalCursor = vector;
                    BeginDrag();
                    //_ContentStartAngle = _CurrAxisAngle;

                    if ( uiMod )
                    {
                        object[] param = { UIEVENT.SCROLLDRAG, controlID, 0 };
                        uiMod.OnEvent( param );
                    }
                }
                else
                    return;
            }
			Drag( vector2.x * dragSpeed * ( dragInverse ? -1 : 1 ) );
			break;

        case MovementType.Vertical:
            if ( !_Dragging )
            {
                _DragAmount += vector2.y;
                if ( Math.Abs( _DragAmount ) >= dragThreshold )
                {
                    _Dragging = true;

                    _PointerStartLocalCursor = vector;
                    BeginDrag();
                    //_ContentStartAngle = _CurrAxisAngle;

                    if ( uiMod )
                    {
                        object[] param = { UIEVENT.SCROLLDRAG, controlID, 0 };
                        uiMod.OnEvent( param );
                    }
                }
                else
                    return;
            }
			Drag( vector2.y * dragSpeed * ( dragInverse ? -1 : 1 ) );
			break;
		}

		if( enableDragginEvent && uiMod )
		{
			object[] param = { UIEVENT.SCROLLDRAG, controlID, 1 };
			uiMod.OnEvent( param );
		}
	}
	
	public virtual void OnEndDrag ( PointerEventData eventData )
	{
        _Dragging = false;
        _DragBegin = false;

        _DragAmount = 0f;

		// 非动量模式这里处理
		if( ( !monentumEnable || ( monentumEnable && _Momentum == 0 ) ) && centerOnChild )
			ProcessCenterOnChild();

		if( uiMod )
		{
			object[] param = { UIEVENT.SCROLLDRAG, controlID, 2 };
			uiMod.OnEvent( param );
		}

        _Momentum *= momentBoost;
	}
    
    public virtual void OnInitializePotentialDrag ( PointerEventData eventData )
	{
	}

    // 拖动
	protected virtual void Drag( float volume )
	{
		if( volume == 0 )
			return;

		_CurrValue = _StartValue + volume; 

		if( _CurrValue > _ValueLimit.y )
		{
			_CurrValue = _ValueLimit.y + ( _CurrValue - _ValueLimit.y ) / 5f;
		}
		else if( _CurrValue < _ValueLimit.x )
		{
			_CurrValue = _ValueLimit.x + ( _CurrValue - _ValueLimit.x ) / 5f;
		}

		OnScroll();
	}

    // 发生滚动时
    protected virtual void OnScroll()
    {
        // 刷新孩子们的状态
        for( int i = 0; i < content.childCount; i++ )
		{
			Transform child = content.GetChild( i );

            float childOffset = GetChildOffset( child );

		    if( fadeFromCenter )
		    {
			    child.GetComponent<CanvasGroup>().alpha = 1 - childOffset * fadeRatio;
		    }

		    if( scaleFromCenter )
		    {
			    float scale = Mathf.Max( 1 - childOffset * scaleRatio, 0 );
			    child.localScale = new Vector3( scale, scale, scale );
		    }

		    if( darkFromCenter )
		    {
			    Image[] images;
			    UIText[] texts;

			    images = child.GetComponentsInChildren<Image>();
			    texts = child.GetComponentsInChildren<UIText>();

			    float color = Mathf.Max( 1 - childOffset * darkRatio, 0 );
			    for( int j = 0; j < images.Length; j++ )
			    {
				    images[j].color = new Color( color, color, color );
			    }
			    for( int j = 0; j < texts.Length; j++ )
			    {
				    texts[j].color = new Color( color, color, color );
			    }
		    }
		}
    }

	// 算孩子居中矫正
	protected virtual void ProcessCenterOnChild()
	{
		// 算出该矫正至哪个角度--找最近的perChild的倍数
		if( _CurrValue > _ValueLimit.y )
		{
			_ChildCenterIndex = 0;
			_ChildCenterValue = _ValueLimit.y;
		}
		else if( _CurrValue < _ValueLimit.x )
		{
			_ChildCenterIndex = _ChildCount - 1;
			_ChildCenterValue = _ValueLimit.x;
		}
		else if( _CurrValue < _ValueLimit.y && _CurrValue > _ValueLimit.x )
		{
			_ChildCenterIndex = Mathf.RoundToInt( _CurrValue / _ValuePerChild );
			_ChildCenterValue = (float)_ChildCenterIndex * _ValuePerChild;
		}
			
		_FlapValue = ( _ChildCenterValue - _CurrValue ) * centerOnChildFlap * 0.025f;
		if( _FlapValue != 0 )
		{
			if( Mathf.Abs( _FlapValue ) > _ValuePerChild * centerOnChildDecay )
				_FlapValue = _ValuePerChild * centerOnChildDecay * ( _FlapValue > 0 ? 1 : -1 );
			_ChildCenterValue += _FlapValue;
		}
	}

    protected virtual void OnCenterOnChild()
    {
		if( uiMod != null )
		{						
			object[] param = { UIEVENT.CENTERCHILD, controlID, Math.Abs( _ChildCenterIndex ) };
			uiMod.OnEvent( param );	
		}
    }

    // 真正开始拖动操作
    protected virtual void BeginDrag()
    {        
        _StartValue = _CurrValue;
    }    

    public virtual void ResetChildren()
    {        
        _ChildCount = 0;
        for ( int i = 0; i < content.childCount; i++ )
        {
            if ( content.GetChild( i ).gameObject.activeSelf )
                _ChildCount++;
        }        

		// 全部还原
		_Dragging = false;
		_LastValue = 0f;
		_CurrValue = 0f;
		_ChildCenterValue = 0f;
		_StartValue = limitExpand;
		_CurrValue = limitExpand;
		_LastValue = limitExpand;
		_ChildCenterValue = 0f;
		_ChildCenterIndex = 0;
		_FlapValue = 0f;
		_Dragging = false;
		_Momentum = 0f;

		OnScroll();
		OnCenterOnChild();
    }

    // 滚动至某个孩子
	public virtual void ScrollTo( int childIndex )
	{
		if( childIndex < 0 || childIndex >= _ChildCount )
			return;
		
		if( centerOnChild )
		{	
			_ChildCenterValue = -(float)childIndex * _ValuePerChild;
						
			_FlapValue = ( _ChildCenterValue + (float)_ChildCenterIndex * _ValuePerChild ) * centerOnChildFlap * 0.025f;
			if( _FlapValue != 0 )
			{
				if( Mathf.Abs( _FlapValue ) > _ValuePerChild * centerOnChildDecay )
					_FlapValue = _ValuePerChild * centerOnChildDecay * ( _FlapValue > 0 ? 1 : -1 );
				_ChildCenterValue += _FlapValue;
			}

			_ChildCenterIndex = childIndex;
		}
	}

    // 跳转至某个孩子
    public virtual void TurnTo( int childIndex )
    {
        if ( childIndex < 0 || childIndex >= _ChildCount )
            return;

        if ( centerOnChild )
        {
            _ChildCenterValue = -(float)childIndex * _ValuePerChild;
            _CurrValue = _ChildCenterValue;
            _ChildCenterIndex = childIndex;
            OnScroll();
            OnCenterOnChild();
        }
    }

    // 相对现在位置跳转
    public virtual void TurnBy( float value )
    {        
        _CurrValue -= value * _ValuePerChild;
        OnScroll();
    }

    // 获得孩子的偏移值
    protected abstract float GetChildOffset( Transform child );
}
