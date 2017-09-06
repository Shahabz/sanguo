using UnityEngine;
using System.Collections;
using UnityEngine.UI;
using UnityEngine.EventSystems;
using DG.Tweening;

public class UIScrollRect : ScrollRect
{
    public int controlID = 0;
    public UIMod uiMod;
    public bool enableOnDragEvent = false;
    public bool enableDrag = true;

    protected GameObject m_LoadingItem = null;
    protected bool m_IsLoadingTop = false;
    protected override void Start()
    {
        base.Start();

        // 挂载uiMod
        if (uiMod == null)
        {
            uiMod = gameObject.GetComponentInParent<UIMod>();
        }
    }

    public override void OnBeginDrag(PointerEventData eventData)
    {
        base.OnBeginDrag(eventData);
        object[] param = { UIEVENT.SCROLLDRAG, controlID, 0 };
        uiMod.OnEvent(param);
    }


    public override void OnDrag(PointerEventData eventData)
    {
        if (enableDrag)
            base.OnDrag(eventData);
        if (enableOnDragEvent)
        {
            object[] param = { UIEVENT.SCROLLDRAG, controlID, 1 };
            uiMod.OnEvent(param);
        }
    }


    public override void OnEndDrag(PointerEventData eventData)
    {
        base.OnEndDrag(eventData);
        object[] param = { UIEVENT.SCROLLDRAG, controlID, 2 };
        uiMod.OnEvent(param);

        if(m_LoadingItem!=null)
        {
            m_LoadingItem.GetComponent<LayoutElement>().preferredHeight = 100;
            if (m_IsLoadingTop)
            {
                RectTransform rectTrans = m_LoadingItem.transform.Find("Content").GetComponent<RectTransform>();
                rectTrans.localPosition = new Vector3(0, -0, 0);
                
            }
        }
    }

    public void ResetScroll()
    {
        SetContentAnchoredPosition(Vector2.zero);
        onValueChanged.Invoke(normalizedPosition);
    }

    public void SetContentPosition(Vector2 position)
    {
        SetContentAnchoredPosition(position);
        onValueChanged.Invoke(normalizedPosition);
    }
    public virtual void ScrollToBottom()
    {
        StartCoroutine(ScrollToBottomAsyn());
    }

    protected virtual IEnumerator ScrollToBottomAsyn()
    {
        enabled = false;
        enabled = true;
        yield return new WaitForEndOfFrame();
        this.normalizedPosition = Vector2.zero;
        yield return null;
    }
    /// <summary>
    /// 滚动内容里面的某个子对象 到底部
    /// </summary>
    public virtual void ScrollToBottom(int contentChildIndex)
    {
        StartCoroutine(ScrollToBottomAsyn(contentChildIndex));
    }
    protected virtual IEnumerator ScrollToBottomAsyn(int contentChildIndex)
    {
        if (content == null || content.childCount <= contentChildIndex)
        {

        }
        else
        {
            yield return new WaitForEndOfFrame();

            var childTrans = content.GetChild(contentChildIndex) as RectTransform;
            float childPosY = -childTrans.localPosition.y - content.offsetMax.y; //子对象在滚动框里面的位置
            float scrollHeight = (this.transform as RectTransform).rect.height;
            float posY = childPosY + childTrans.rect.height - scrollHeight;
            if (posY > 0)
            {
                Vector2 preOffset = content.offsetMax;
                float perY = (posY) / 10;
                for (int tmpi = 0; tmpi < 10; tmpi++)
                {
                    preOffset.y = preOffset.y + perY;
                    this.content.offsetMax = preOffset;
                    yield return new WaitForEndOfFrame();
                }
            }
        }
        yield return null;
    }

	public virtual void ScrollToTop()
	{
		StartCoroutine(ScrollToTopAsyn());
	}

	protected virtual IEnumerator ScrollToTopAsyn()
	{
		enabled = false;
		enabled = true;
		yield return new WaitForEndOfFrame();
		this.normalizedPosition = new Vector2( 0, 1 );
		yield return null;
	}
	/// <summary>
	/// 滚动内容里面的某个子对象 到顶部
	/// </summary>
	public virtual void ScrollToTop(int contentChildIndex)
	{
		StartCoroutine(ScrollToTopAsyn(contentChildIndex));
	}
	protected virtual IEnumerator ScrollToTopAsyn(int contentChildIndex)
	{
		if (content == null || content.childCount <= contentChildIndex)
		{

		}
		else
		{
			yield return new WaitForEndOfFrame();

			var childTrans = content.GetChild(contentChildIndex) as RectTransform;
			float childPosY = -childTrans.localPosition.y - content.offsetMax.y; //子对象在滚动框里面的位置
			float scrollHeight = (this.transform as RectTransform).rect.height;
			float posY = childPosY + childTrans.rect.height - scrollHeight;
			if (posY > 0)
			{
				Vector2 preOffset = content.offsetMax;
				float perY = (posY) / 10;
				for (int tmpi = 0; tmpi < 10; tmpi++)
				{
					preOffset.y = preOffset.y - perY;
					this.content.offsetMax = preOffset;
					yield return new WaitForEndOfFrame();
				}
			}
		}
		yield return null;
	}

	/// <summary>
	/// 指定一个 item让其定位到ScrollRect中间
	/// </summary>
	/// <param name="target">需要定位到的目标</param>
	public void CenterOnItem(RectTransform target)
	{
		// Item is here
		//var itemCenterPositionInScroll = GetWorldPointInWidget( scrollRect.GetComponent<RectTransform>(), GetWidgetWorldPoint( target));
		//var itemCenterPositionInScroll = GetWorldPointInWidget( this.GetComponent<RectTransform>(), GetWidgetWorldPoint( target));
		//Debug .Log( "Item Anchor Pos In Scroll: " + itemCenterPositionInScroll);
		// But must be here
		//var targetPositionInScroll = GetWorldPointInWidget( scrollRect.GetComponent <RectTransform>(), GetWidgetWorldPoint( viewPointTransform));
		//var targetPositionInScroll = GetWorldPointInWidget( this.GetComponent <RectTransform>(), GetWidgetWorldPoint( viewPointTransform));
		//Debug .Log( "Target Anchor Pos In Scroll: " + targetPositionInScroll);
		// So it has to move this distance
		//var difference = targetPositionInScroll - itemCenterPositionInScroll;
		//difference .z = 0f ;

		//var newNormalizedPosition = new Vector2(difference .x / (contentTransform.rect.width -viewPointTransform.rect.width ), difference .y / (contentTransform.rect .height - viewPointTransform. rect.height ));
		
		//newNormalizedPosition = scrollRect.normalizedPosition - newNormalizedPosition;

		//newNormalizedPosition .x = Mathf.Clamp01(newNormalizedPosition.x );
		//newNormalizedPosition .y = Mathf.Clamp01(newNormalizedPosition.y );

		//DOTween .To(() => scrollRect.normalizedPosition , x=>scrollRect.normalizedPosition = x ,newNormalizedPosition, 3);
	}

	Vector3 GetWidgetWorldPoint (RectTransform target)
	{
		//pivot position + item size has to be included
		var pivotOffset = new Vector3(
			(0.5f - target .pivot. x) * target.rect.size.x ,
			(0.5f - target .pivot. y) * target.rect.size.y ,
			0f);
		var localPosition = target.localPosition + pivotOffset ;
		return target.parent.TransformPoint (localPosition);
	}

	Vector3 GetWorldPointInWidget (RectTransform target, Vector3 worldPoint)
	{
		return target.InverseTransformPoint(worldPoint );
	}

    /// <summary>
    /// 显示加载更多
    /// </summary>
    public virtual void ShowLoading(bool isTop, float waitTime)
    {
        if(m_LoadingItem == null)
        {
            StartCoroutine(ShowLoadingAsync(isTop, waitTime));
        }
        
    }
    protected IEnumerator ShowLoadingAsync(bool isTop, float waitTime)
    {
        m_IsLoadingTop = isTop;
        m_LoadingItem = GameObject.Instantiate( ResourceManager.LoadPrefab("UIP_Loading"));
        m_LoadingItem.transform.SetParent(this.content.transform);
        m_LoadingItem.transform.localScale = Vector3.one;
        //UIText text = m_LoadingItem.GetComponentInChildren<UIText>();
        //text.text = "Loading";
        RectTransform rectTrans = m_LoadingItem.transform.Find("Content").GetComponent<RectTransform>();
        if (isTop == true)
        {
            m_LoadingItem.GetComponent<LayoutElement>().preferredHeight = 100;
            rectTrans.localPosition = new Vector3(0, -50, 0);
            m_LoadingItem.transform.SetAsFirstSibling();
        }
        else
        {
            rectTrans.localPosition = new Vector3(0, -50, 0);
            m_LoadingItem.transform.SetAsLastSibling();
        }
        //enableDrag = false;
        //enabled = false;
        yield return null;
        yield return new WaitForSeconds(waitTime);
        if (m_LoadingItem != null)
        {
            HideLoading();
        }
    }
    /// <summary>
    /// 隐藏加载更多
    /// </summary>
    public virtual void HideLoading()
    {
        if (m_LoadingItem != null)
        {
            GameObject.Destroy(m_LoadingItem);

            m_LoadingItem = null;
            //enableDrag = true;
            //enabled = true;
        }
    }
    /// <summary>
    /// 是否正在显示loading
    /// </summary>
    /// <returns></returns>
    public virtual bool IsShowLoading()
    {
        return m_LoadingItem != null;
    }
}
