using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class UIClippingChat : MonoBehaviour {


	// 重新计算的阈值，手动填入
	public float refreshThreshold = 160;
	Vector2 mLastValue = Vector3.zero;
	Vector2 mValueChanged = Vector3.zero;

	private UIScrollRect mScrollRect = null;
	private float mBufferAlterPix = 300;
	private RectTransform mScrollRectTrans = null;

	public void Awake()
	{
		mScrollRect = GetComponent<UIScrollRect>();
		mScrollRectTrans = mScrollRect.GetComponent<RectTransform>();
		mScrollRect.onValueChanged.AddListener(ValueChanged);
	}

	void ValueChanged(Vector2 value)
	{
		if (!enabled)
			return;

		// 未达重新计算的阈值，返回
		// 只计算单方向
		mValueChanged += value - mLastValue;
		mLastValue = value;
		if (Mathf.Abs(mValueChanged.y * mScrollRect.content.sizeDelta.y) < refreshThreshold)
			return;
		Refresh();
	}

	bool IsInBound(RectTransform child)
	{
		
		// 下边界
		float downPos = -child.localPosition.y - mScrollRect.content.offsetMax.y;
		// 上边界
		float upPos = downPos ;
		if (upPos > mScrollRectTrans.rect.height + mBufferAlterPix)
		{
			// 超出列表的下边框
			return false;
		}
		else if (downPos < -mBufferAlterPix)
		{
			// 超出列表的上边框
			return false;
		}
		else
		{
			return true;
		}

		return true;
	}

	public void Refresh()
	{
		mValueChanged = Vector2.zero;
		for (int i = 0; i < mScrollRect.content.childCount; i++)
		{
			RectTransform child = mScrollRect.content.GetChild(i).GetComponent<RectTransform>();
			Transform obj = child.GetChild(0);
			if (obj != null)
			{
				if (!IsInBound(child))
				{
					obj.gameObject.SetActive(false);
					if ( child.childCount == 2 )
						child.GetChild(1).gameObject.SetActive(false);
				}
				else
				{
					obj.gameObject.SetActive(true);
					if ( child.childCount == 2 )
						child.GetChild(1).gameObject.SetActive(true);
				}
			}
		}
	}
}
