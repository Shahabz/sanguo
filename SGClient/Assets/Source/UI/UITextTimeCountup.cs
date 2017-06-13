using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine.UI;


public class UITextTimeCountup :UIText
{
    public UIMod uiMod;
    public int controlID = 0;
    private bool mIsStart = false;
    public int Total
    {
        get
        {
            return m_Total;
        }
    }
    private int m_Total = -1;
    protected override void Start()
    {
        base.Start();
        InvokeRepeating("Countup", 0, 1);
    }

    public void ReStart()
    {
        InvokeRepeating("Countup", 0, 1);
        mIsStart = true;
        m_Total = -1;
    }

    public void Stop()
    {
        CancelInvoke("Countup");
        mIsStart = false;
    }

    private void Countup()
    {
        if (mIsStart == false)
            return;
        m_Total = m_Total + 1;
        ShowText(m_Total);
        if (uiMod != null)
        {
            object[] param = { UIEVENT.TIMECOUNTCHANGED, controlID, m_Total };
            uiMod.OnEvent(param);
        }
    }

    protected virtual void ShowText(int total)
    {
        text = Utils.GetTimeFormat(total);
    }
}

