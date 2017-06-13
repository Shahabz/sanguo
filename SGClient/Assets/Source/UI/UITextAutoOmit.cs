using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;

/// <summary>
/// 文本 
/// 文本超过长度后自动添加省略号
/// 确保文本内容初始为空
/// </summary>
public class UITextAutoOmit : UIText
{
    public override string text
    {
        get
        {
            return base.text;
        }
        set
        {

            Refresh(value);
        }
    }
    private StringBuilder mOriginText = new StringBuilder();
    private float mPerLineHeight;

    protected override void Awake()
    {
        this.rectTransform.pivot = new Vector2(0, 1);
        mPerLineHeight = this.preferredHeight;
    }

    /// <summary>
    /// 刷新内容
    /// </summary>
    public virtual void Refresh(string content)
    {
        mOriginText = new StringBuilder();
        mOriginText.Append(content);
        base.text = content;
        if (this.IsActive() && Localization.currentLanguage != "ar")
            StartCoroutine(SetContent());
    }

    /// <summary>
    /// 设置内容
    /// </summary>
    /// <returns></returns>
    protected virtual IEnumerator SetContent()
    {
        yield return new WaitForEndOfFrame();
        if (this.preferredHeight > this.rectTransform.rect.height)
        {
            //先确定可以允许有多少行
            int maxLineCount = (int)(this.rectTransform.rect.height / mPerLineHeight);
            if (maxLineCount <= 0)
            {
            }
            else
            {
                //结束的显示内容索引
                int endIndex = this.cachedTextGeneratorForLayout.lines[maxLineCount].startCharIdx;
                int replaceCount = 0;
                int startIndex = endIndex;
                for (int tmpi = endIndex - 1; tmpi >= 0; tmpi--)
                {
                    var chat = this.cachedTextGeneratorForLayout.characters[tmpi];
                    if (chat.charWidth > 0.1)
                    {
                        //避免出现richtext 的情况
                        mOriginText.Replace(mOriginText[tmpi], '.', tmpi, 1);
                        startIndex = tmpi;
                        replaceCount++;
                    }
                    if (replaceCount >= 3)
                    {
                        break;
                    }
                }
                for (int tmpi = endIndex; tmpi < mOriginText.Length; tmpi++)
                {
                    var chat = this.cachedTextGeneratorForLayout.characters[tmpi];
                    if (chat.charWidth > 0)
                    {
                        mOriginText.Replace(mOriginText[tmpi], ' ', tmpi, 1);
                    }
                }
                base.text = mOriginText.ToString();
            }
        }
        yield return null;
    }
}

