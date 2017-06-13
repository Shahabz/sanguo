using UnityEngine;
using System.Collections;
using UnityEngine.EventSystems;

public class UIDragSensor : UIBehaviour, IEventSystemHandler, IBeginDragHandler, IDragHandler, IEndDragHandler
{
    public int controlID;
    public UIMod uiMod;
    public UIScrollRect uiScrollRect;
    protected bool m_IsSideslip = false;
    // Use this for initialization
    protected override void Start()
    {
        // 挂载uiMod
        if (uiMod == null)
        {
            uiMod = gameObject.GetComponentInParent<UIMod>();
            if (uiMod == null)
            {
                Debug.LogError("Can't Find UIMod:" + gameObject.name);
                Destroy(gameObject);
                return;
            }
        }
    }

    public void OnBeginDrag(PointerEventData eventData)
    {
        Vector3 delta = new Vector3(eventData.delta.x, eventData.delta.y, 0);

        if (Mathf.Abs(eventData.delta.y) > Mathf.Abs(eventData.delta.x))
        {
            m_IsSideslip = false;
            uiScrollRect.OnBeginDrag(eventData);
        }
        else
        {
            m_IsSideslip = true;
            object[] param = { UIEVENT.DRAGSENSOR, controlID, delta };
            uiMod.OnEvent(param);
        }
    }

    public void OnDrag(PointerEventData eventData)
    {
        Vector3 delta = new Vector3(eventData.delta.x, eventData.delta.y, 1);

        if (m_IsSideslip == false)
            uiScrollRect.OnDrag(eventData);
        else
        {
            object[] param = { UIEVENT.DRAGSENSOR, controlID, delta };
            uiMod.OnEvent(param);
        }
    }
    public void OnEndDrag(PointerEventData eventData)
    {
        Vector3 delta = new Vector3(eventData.delta.x, eventData.delta.y, 2);
        object[] param = { UIEVENT.DRAGSENSOR, controlID, delta };
        uiScrollRect.OnEndDrag(eventData);
        uiMod.OnEvent(param);
        m_IsSideslip = false;
    }
}
