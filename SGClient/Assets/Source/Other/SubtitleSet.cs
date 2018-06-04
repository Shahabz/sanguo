using UnityEngine;
using System.Collections;

public class SubtitleSet : MonoBehaviour
{
    public UIText           text;
    public FXTypeWriter     typeWriter;

    public void SetText( int localizedID )
    {
        if( localizedID == 0 )
            text.text = "";
        else
        {
            text.text = Localization.text( localizedID );
            typeWriter.Play();
        }
    }
}
