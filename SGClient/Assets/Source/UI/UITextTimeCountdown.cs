using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;
using UnityEngine.UI;

/*
 * UI 倒计时功能
 * 
 * 确保 挂载了 UITEXT对象
 */
public class UITextTimeCountdown : UIText
{
    public UIMod uiMod;
    public int controlID = 0;
    public UIProgress uiProgress = null;
    public UISlider uiSlider = null;
    public String formatText = "";
    public String overText = "";
    public bool playOnEable = true;

    public enum DirectType
    {
        LeftToRight = 0,
        RightToLeft = 1,
    }

    public DirectType Direct = DirectType.LeftToRight;

    /*
     * 时长 (s)
     */
    public virtual int Duration
    {
        set
        {
            mStartTime = DateTime.Now;
            if (mDuration != value)
            {
                mDuration = value;
                if (mIsOver)
                {
                    ReStart();
                }
                //mPassTime = 0;
                OnValueChanged();
            }
        }
        get
        {
            return mDuration;
        }
    }
    private int mDuration = 0;

    public virtual int PassTime
    {
        get
        {
            return mPassTime;
        }
        set
        {
            mStartPassTime = value;
            if (value != mPassTime)
            {
                mPassTime = value;
                if (mIsOver)
                {
                    ReStart();
                }
                OnValueChanged();
            }
        }
    }

    public int LeftTime
    {
        get
        {
            return Duration - PassTime;
        }
    }

    /*
     * 间隔(s)
     */
    public int Interval = 1;

    private bool mIsOver = false;
    private int mPassTime = 0;
    private int mStartPassTime = 0;
    //private int mDay = 60 * 60 * 24;
    //private int mHour = 60 * 60;
    //private int mMin = 60;
    private DateTime mStartTime = DateTime.Now;

    protected override void Start()
    {
        if (playOnEable)
            InvokeRepeating("CountDown", 0, Interval);

        // 挂载uiMod
        if (uiMod == null)
        {
            uiMod = gameObject.GetComponentInParent<UIMod>();
        }
    }
    /// <summary>
    /// 停止
    /// </summary>
    public void Stop()
    {
        mIsOver = true;
        CancelInvoke("CountDown");
    }

    /// <summary>
    /// 重新启动
    /// </summary>
    public void ReStart()
    {
        mIsOver = false;
        //CancelInvoke("CountDown");
        InvokeRepeating("CountDown", 0, Interval);
    }
    public void Update()
    {
    }
    /// <summary>
    /// 设置显示内容
    /// </summary>
    /// <param name="leftTime"></param>
    protected virtual void ShowText(int leftTime)
    {
        if (formatText != String.Empty)
        {
            text = string.Format(formatText, Utils.GetTimeFormat(leftTime));
        }
        else
        {
            text = Utils.GetTimeFormat(leftTime);
        }
    }
    /// <summary>
    /// 值修改
    /// </summary>
    protected void OnValueChanged()
    {

        int leftTime = mDuration - mPassTime;
        if (leftTime < 0)
            leftTime = 0;
        ShowText(leftTime);

        if (Direct == DirectType.LeftToRight)
        {
            if (uiProgress)
            {
                if (mDuration <= 0)
                {
                    uiProgress.SetValue(1.0f);
                }
                else
                    uiProgress.SetValue(mPassTime / (float)mDuration);
            }

            if (uiSlider)
            {
                if (mDuration <= 0)
                {
                    uiSlider.value = 1.0f;
                }
                else
                    uiSlider.value = mPassTime / (float)mDuration;
            }
        }
        else if (Direct == DirectType.RightToLeft)
        {
            if (uiProgress)
            {
                if (leftTime <= 0)
                {
                    uiProgress.SetValue(0.0f);
                }
                else
                    uiProgress.SetValue(leftTime / (float)mDuration);
            }

            if (uiSlider)
            {
                if (leftTime <= 0)
                {
                    uiSlider.value = 0.0f;
                }
                else
                    uiSlider.value = leftTime / (float)mDuration;
            }
        }



        if (mPassTime >= mDuration)
        {
            Stop();
            OnTimeEnd();
        }
    }
    /// <summary>
    /// 计算
    /// </summary>
    private void CountDown()
    {
        if (mIsOver)
            return;
        double interval = (DateTime.Now - mStartTime).TotalSeconds;
        //mPassTime = mStartPassTime + (int)(interval + 0.5f);
        if (mPassTime <= mDuration)
        {
            mPassTime = mStartPassTime + (int)(interval + 0.5f);

            OnValueChanged();
        }
    }
    // 计时结束
    protected virtual void OnTimeEnd()
    {
        if (overText != String.Empty)
        {
            text = overText;
        }
        if (this.uiMod != null)
        {
            object[] param = { UIEVENT.TIMECOUNTEND, controlID, 0 };
            uiMod.OnEvent(param);
        }
    }
    /// <summary>
    /// 设置时间
    /// </summary>
    /// <param name="duration"></param>
    /// <param name="passTime"></param>
    public virtual void SetTime(int duration, int passTime)
    {
        mStartTime = DateTime.Now;
        mDuration = duration;
        mPassTime = passTime;
        mStartPassTime = passTime;

        if (mIsOver)
        {
            ReStart();
        }
        OnValueChanged();
    }
}

