using UnityEngine;
using System;
using System.Collections;
using UnityEngine.EventSystems;
using UnityEngine.Events;
using UnityEngine.UI;

// 相当于自定义的 Scroll Rect，有孩子居中功能的使用
public class UIScrollView : UIScrollBase
{
	public float			valuePerChild = 0f;

    protected override void Awake()
    {
        _ValuePerChild = valuePerChild;
    }

	protected override void OnScroll()
	{
        if( moveType == MovementType.Horizon )
		    content.anchoredPosition = new Vector3( _OriginalValue + _CurrValue, 0f, 0f );
        else
		    content.anchoredPosition = new Vector3( 0f, _OriginalValue - _CurrValue, 0f );

        base.OnScroll();
	}

    protected override float GetChildOffset( Transform child )
    {
        if( moveType == MovementType.Horizon )
            return Math.Abs( child.position.x - viewPort.position.x );
        else
            return Math.Abs( child.position.y - viewPort.position.y );
    }

    IEnumerator DealyResetChildren()
    {
        yield return new WaitForEndOfFrame();

        ResetChildren_Real();
    }
	
	public override void ResetChildren()
    {
        StartCoroutine( DealyResetChildren() );
	}

    void ResetChildren_Real()
    {       
        _OriginalValue = viewPort.rect.width / 2 - _ValuePerChild / 2;

        base.ResetChildren();

		if( _ChildCount == 0 )
			return;

        // 限制
        _ValueLimit.x = -_ValuePerChild * ( (float)_ChildCount - 1f );
        _ValueLimit.y = 0;
    
    }

    public override void TurnTo( int childIndex )
    {
        StartCoroutine( DealyTurnTo( childIndex ) );
    }

    IEnumerator DealyTurnTo( int childIndex )
    {
        yield return new WaitForEndOfFrame();

        base.TurnTo( childIndex );
    }

    public void TurnToNow(  int childIndex  )
    {
        base.TurnTo( childIndex );
    }
}
