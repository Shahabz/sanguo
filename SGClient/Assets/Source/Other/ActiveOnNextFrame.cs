using UnityEngine;
using System.Collections;

public class ActiveOnNextFrame : MonoBehaviour
{
    public GameObject acitveGO;

    void Start()
    {        
        acitveGO.SetActive( false );
    }

    void OnEnable()
    {
        if( acitveGO != null )
            StartCoroutine( Active() );
    }

    IEnumerator Active()
    {
        yield return new WaitForEndOfFrame();
        yield return new WaitForFixedUpdate();

        acitveGO.SetActive( true );
    }

    void OnDisable()
    {
        acitveGO.SetActive( false );
    }
}
