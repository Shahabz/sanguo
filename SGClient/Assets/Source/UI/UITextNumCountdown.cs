using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine.UI;

/*
 * UI 数字计数
 */
public class UITextNumCountdown : UIText
{
    /// <summary>
    /// 本次计数的最大个数
    /// </summary>
    public float StartCount
    {
        set
        {
            mStartCount = value;
            mCurrCount = value;
            mPreTime = DateTime.Now;
            RefreshText();
        }
        get
        {
            return mStartCount;
        }
    }
    private float mStartCount = 0;
    /// <summary>
    /// 结束的个数
    /// </summary>
    public float EndCount
    {
        set
        {
            mEndCount = value;
        }
        get
        {
            return mEndCount;
        }
    }
    private float mEndCount = 0;
    /// <summary>
    /// 计数的单位增加/减少速度
    /// </summary>
    public float Speed
    {
        set
        {
            mSpeed = value;
        }
        get
        {
            return mSpeed;
        }
    }
    private float mSpeed = 0;
    /*
     * 间隔(s)
     */
    private int Interval = 1;
    private bool mIsOver = false;
    private float mCurrCount = 0;
    private DateTime mPreTime = DateTime.Now;
    public String formatText = "";

    protected override void Start()
    {

        InvokeRepeating("CountDown", 0, Interval);

    }

    public void Stop()
    {
        CancelInvoke("CountDown");
    }
    /// <summary>
    /// 刷新内容
    /// </summary>
    protected virtual void RefreshText()
    {
        string count = Math.Ceiling(mCurrCount).ToString("N");
        int dotIndex = count.IndexOf('.');
        if (dotIndex > 0)
        {
            count = count.Substring(0, dotIndex);
        }
        if (formatText != String.Empty)
        {
            text = string.Format(formatText, count);
        }
        else
        {
            text = count;
        }
    }

    private void CountDown()
    {
        int interval = (int)((DateTime.Now - mPreTime).TotalSeconds + 0.5f);

        if (mSpeed > 0)
        {
            if (mCurrCount < mEndCount)
            {
                mCurrCount = (mCurrCount + mSpeed * interval);
                if (mCurrCount > mEndCount)
                    mCurrCount = mEndCount;
                RefreshText();
            }
        }
        else
        {
            if (mCurrCount > mEndCount)
            {
                mCurrCount = (mCurrCount + mSpeed * interval);
                if (mCurrCount < mEndCount)
                    mCurrCount = mEndCount;

                RefreshText();
            }
        }
        mPreTime = DateTime.Now;
    }
}

