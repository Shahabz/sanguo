using UnityEngine;
using System.Collections;

public class ActiveOP : MonoBehaviour
{
    public void Disable()
    {
        gameObject.SetActive( false );
    }
    public void Enable()
    {
        gameObject.SetActive( true );
    }
}
