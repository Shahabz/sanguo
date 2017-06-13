using UnityEngine;
using System.Collections;
using UnityEngine.UI;
using UnityEngine.EventSystems;

public class UIInputField : InputField
{
    public int controlID = 0;
    public UIMod uiMod;
    // 是否自动关闭
    public bool IsAutoClose = true;
    // 是否过滤emoji表情
    public bool IsFilterEmoji = true;

    protected override void Start()
    {
        base.Start();
        // 挂载uiMod
        if (uiMod == null)
        {
            uiMod = gameObject.GetComponentInParent<UIMod>();
            if (uiMod == null)
            {
                Debug.LogError("Can't Find uiMod:" + gameObject.name);
                Destroy(gameObject);
                return;
            }
        }

        onValueChanged.AddListener(ValueChange);
        onValidateInput += ValidateInput;
        onEndEdit.AddListener(EndEdit);
    }
    /// <summary>
    /// 值修改
    /// </summary>
    /// <param name="v"></param>
    public virtual void ValueChange(string v)
    {
        object[] param = { UIEVENT.INPUTVALUECHANGED, controlID, this.text };
        uiMod.OnEvent(param);
    }
    /// <summary>
    /// 验证输入
    /// </summary>
    /// <param name="text"></param>
    /// <param name="charIndex"></param>
    /// <param name="addedChar"></param>
    /// <returns></returns>
    public virtual char ValidateInput(string text, int charIndex, char addedChar)
    {
        if (IsFilterEmoji && Utils.IsEmojiCharacter(addedChar))
        {
            return (char)0;
        }
        else
        {
            if (addedChar == '\n')
            {
                if (lineType == LineType.MultiLineSubmit && m_Keyboard != null)
                {
                    if (uiMod != null)
                    {
                        string val = m_Keyboard.text;
                        string con = "";
                        for (int i = 0; i < val.Length; ++i)
                        {
                            char c = val[i];
                            if (c == '\n')
                                c = (char)0;
                            if (Utils.IsEmojiCharacter(c))
                                c = (char)0;
                            if (c != 0)
                                con += c;
                        }
                        object[] param = { UIEVENT.INPUTSUBMITEX, controlID, con };
                        uiMod.OnEvent(param);
                    }
                }
            }
            return addedChar;
        }
    }

    public virtual void EndEdit(string v)
    {
        object[] param = { UIEVENT.INPUTSUBMIT, controlID, this.text };
        uiMod.OnEvent(param);
    }
}
