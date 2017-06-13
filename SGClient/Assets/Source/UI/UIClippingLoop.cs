using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using UnityEngine.UI;

// 自动隐藏超出范围的Child
public class UIClippingLoop : MonoBehaviour
{
    enum Direction
    {
        Horizontal,
        Vertical
    }

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
        if (mScrollRect.vertical)
        {
            if (Mathf.Abs(mValueChanged.y * mScrollRect.content.sizeDelta.y) < refreshThreshold)
                return;
        }
        else
        {
            if (Mathf.Abs(mValueChanged.x * mScrollRect.content.sizeDelta.x) < refreshThreshold)
                return;
        }

        Refresh();
    }

    bool IsInBound(RectTransform child)
    {
        //RectTransform child = mScrollRect.content.GetChild(index).GetComponent<RectTransform>();
        // 下边界
        float downPos = -child.localPosition.y - mScrollRect.content.offsetMax.y;
        // 上边界
        float upPos = downPos ;
        if (mScrollRect.vertical)
        {
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
        }
        else
        {

        }
        return true;
    }

    public void Refresh()
    {
        mValueChanged = Vector2.zero;
        for (int i = 0; i < mScrollRect.content.childCount; i++)
        {
            RectTransform child = mScrollRect.content.GetChild(i).GetComponent<RectTransform>();
            Transform obj = child.Find("Content");
            if (obj != null)
            {
                if (!IsInBound(child))
                {
                    obj.gameObject.SetActive(false);
                }
                else
                {
                    obj.gameObject.SetActive(true);
                }
            }
        }
    }
}