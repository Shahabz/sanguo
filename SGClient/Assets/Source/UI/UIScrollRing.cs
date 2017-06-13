using UnityEngine;
using System.Collections;
using System;
using UnityEngine.UI;

// 旋转控件拖动控件，默认只对z轴进行旋转。如需其它轴，请把content放到axis下，并对axis进行旋转。具体参考BuildingDlg
public class UIScrollRing : UIScrollBase
{
	// 分布
	public Transform		axis;
	public float			angleStart = 0f;				// 以此为起点逆时针排列：x轴正向为0
	public float			anglePerChild = 15f;			// 角度
	public float			childRadius = 200f;				// 半径
	public bool				childFixed = true;				// 孩子固定角度

    protected override void Awake()
    {
        _ValuePerChild = anglePerChild;
    }

	protected override void OnScroll()
	{
		Vector3 angles = content.localEulerAngles;
		angles.z = _CurrValue;
		content.localEulerAngles = new Vector3( 0f, 0f, _CurrValue );
		if( childFixed )
		{
			for( int i = 0; i < content.childCount; i++ )
			{
				Transform child = content.GetChild( i );
				angles = child.localEulerAngles;
				angles.z = -_CurrValue;
				child.localEulerAngles = angles;
			}
		}

        base.OnScroll();
	}

    protected override float GetChildOffset( Transform child )
    {
		return Vector3.Angle( child.position - content.position,
            Quaternion.AngleAxis( angleStart, axis.localRotation * new Vector3( 0f, 0f, 1f ) ) * ( axis.localRotation * new Vector3( 1f, 0f, 0f ) ) );
    }
	
	public override void ResetChildren()
    {
        base.ResetChildren();

		if( _ChildCount == 0 )
			return;

        // 设置旋转限制， 目前的排列方式只能逆时针旋转。
		_ValueLimit.x = -(float)( _ChildCount - 1 ) * _ValuePerChild - limitExpand;
		_ValueLimit.y = 0 + limitExpand;
		// 对孩子按照设置进行排列
		for( int i = 0; i < _ChildCount; i++ )
		{
			Transform child = content.GetChild( i );
			float angle = angleStart + _ValuePerChild * i;
			float radian = angle / 180 * Mathf.PI;
			child.localPosition = new Vector3( Mathf.Cos( radian ) * childRadius, Mathf.Sin( radian ) * childRadius, 0f );
		}
	}
}

