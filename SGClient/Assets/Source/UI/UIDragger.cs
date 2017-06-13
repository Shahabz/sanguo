using UnityEngine;
using System.Collections;
using UnityEngine.EventSystems;
using UnityEngine.UI;

[RequireComponent(typeof(Graphic))]
public class UIDragger : UIBehaviour, IEventSystemHandler, IBeginDragHandler, IDragHandler, IEndDragHandler
{
    public UIMod        uiMod;
    public int          controlID;

    public float        scale = 1f;
    public Image        sourceIcon;
    public Image        dragIcon;

    public bool         hideSourceOnDragging = true;

    protected override void Start()
    {
        if( dragIcon == null )
        {
            Debug.LogWarning( "No Drag Target!!" );
            Destroy( this );
        }
        dragIcon.gameObject.SetActive( false );
    }


    public void OnBeginDrag(PointerEventData eventData)
    {
        if( sourceIcon != null )
        {
            dragIcon.sprite = sourceIcon.sprite;
            dragIcon.SetNativeSize();
            dragIcon.transform.localScale = Vector3.one * scale;

            if( hideSourceOnDragging )
                sourceIcon.enabled = false;
        }

        if( uiMod != null )
        {
            object[] param = { UIEVENT.UIDRAG, controlID, 0 };
		    uiMod.OnEvent( param );
        }

        dragIcon.gameObject.SetActive( true );
    }

    public void OnDrag(PointerEventData eventData)
    {
        Vector3 pos = eventData.pressEventCamera.ScreenToWorldPoint( eventData.position );
        pos.z = 0;
        dragIcon.transform.position = pos;
    }

    public void OnEndDrag(PointerEventData eventData)
    {
        if( uiMod != null )
        {
            object[] param = { UIEVENT.UIDRAG, controlID, 1 };
		    uiMod.OnEvent( param );
        }

         if( sourceIcon != null && hideSourceOnDragging )
                sourceIcon.enabled = true;

        dragIcon.gameObject.SetActive( false );

        UIDropper dropper = eventData.pointerCurrentRaycast.gameObject.GetComponent<UIDropper>();
        if( dropper != null )
        {
            dropper.OnDrop( this );
        }
    }
}

