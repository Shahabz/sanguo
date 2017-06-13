using UnityEngine;
using UnityEngine.UI;

// 注意不能被遮挡。上层图片文字的raycast target必须为false
[RequireComponent(typeof(Graphic))]
public class UIDropper : MonoBehaviour
{
    public int controlID = 0;

    public void OnDrop( UIDragger dragger )
    {
        if( dragger != null && dragger.uiMod != null )
        {
            object[] param = { UIEVENT.UIDROP, controlID, dragger.controlID };
		    dragger.uiMod.OnEvent( param );
        }
    }
}
