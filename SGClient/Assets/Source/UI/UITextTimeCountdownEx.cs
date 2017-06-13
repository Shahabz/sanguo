using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;


public class UITextTimeCountdownEx : UITextTimeCountdown
{
    protected override void ShowText(int leftTime)
    {
        if (leftTime < 60)
        {
            if (!string.IsNullOrEmpty(formatText))
                text = string.Format(formatText, "<1m");
            else
                text = "<1m";
        }
        else
        {
            leftTime = leftTime - (leftTime % 60);
            base.ShowText(leftTime);
        }
    }
}

