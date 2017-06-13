using UnityEngine;
using System.Collections;
using UnityEngine.UI;
using System.Collections.Generic;



public class UITextUnderlineEx : UIText
{
    public bool ShowLine
    {
        set
        {
            m_ShowLine = value;
            if (m_Line != null)
            {
                m_Line.SetActive(value);
            }
        }
        get
        {
            return m_ShowLine;
        }
    }
    protected bool m_ShowLine = true;

    //线对象
    protected GameObject m_Line = null;
    //是否有修改
    private bool m_IsChanged = false;

    protected override void Awake()
    {
        if (m_Line == null && Application.isPlaying == true)
        {
            m_Line = new GameObject("line");
            m_Line.layer = 5;
            m_Line.tag = "UI";
            m_Line.AddComponent<Image>();
            RectTransform trans = m_Line.GetComponent<RectTransform>();
            trans.SetParent(this.transform);
            trans.sizeDelta = Vector2.zero;
            trans.pivot = new Vector2(0, 0.5f);
            trans.localScale = Vector3.one;
            m_Line.SetActive(false);
        }

    }

    public void Update()
    {
        if (m_IsChanged)
        {
            m_IsChanged = false;

            for (int tmpi = 0; tmpi < this.cachedTextGenerator.lineCount; tmpi++)
            {
                int charIndex = this.cachedTextGenerator.lines[tmpi].startCharIdx;
                if (cachedTextGenerator.characters.Count > charIndex)
                {
                    float posX = cachedTextGenerator.characters[charIndex].cursorPos.x * pixelsPerUnit;
                    float posY = this.cachedTextGenerator.lines[tmpi].topY - cachedTextGenerator.lines[tmpi].height * pixelsPerUnit;
                    int count = cachedTextGenerator.characters.Count;
                    float width = Mathf.Abs(cachedTextGenerator.characters[count - 1].cursorPos.x - posX) * pixelsPerUnit;
                    SetLine(m_Line, posX, posY, width, 1.5f);
                }
            }
        }
    }

    /// <summary>
    /// 设置线
    /// </summary>
    /// <param name="line"></param>
    /// <param name="posX"></param>
    /// <param name="posY"></param>
    /// <param name="width"></param>
    /// <param name="height"></param>
    private void SetLine(GameObject line, float posX, float posY, float width, float height)
    {
        if (m_Line == null)
            return;
        var lineImage = line.GetComponent<Image>();
        RectTransform trans = m_Line.GetComponent<RectTransform>();
        lineImage.color = this.color;
        trans.anchorMin = new Vector2(0.5f, 0.5f);
        trans.anchorMax = new Vector2(0.5f, 0.5f);
        trans.sizeDelta = new Vector2(width, height);
        trans.localPosition = new Vector3(posX, posY - 1, 0);
        line.SetActive(m_ShowLine);
    }

    protected override void OnPopulateMesh(VertexHelper toFill)
    {
        base.OnPopulateMesh(toFill);
        //记录显示文字有修改
        m_IsChanged = true;
    }
}
