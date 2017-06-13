using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

/// <summary>
/// 首字母放大
[Serializable]
public class UITextFirstUpper : UIText
{
    private string mOriginText = "";
    public int UpperSize = 50;

    public override string text
    {
        get
        {
            return base.text;
        }
        set
        {
            mOriginText = value;
            if (value.Length > 0)
            {
                base.text = string.Format("<size={0}>{1}</size>{2}", UpperSize, value[0], value.Substring(1));
            }
            else
            {
                base.text = value;
            }
        }
    }
}

