using System.Collections;
using UnityEngine;

public class DisableOnDisable : MonoBehaviour
{
    public GameObject[] objs;
    void OnDisable()
    {
        for( int i = 0; i < objs.Length; i++ )
        {
            if( objs[i] != null )
                objs[i].SetActive( false );
        }
    }
}
