using UnityEngine;
using System.Collections;

public class ActiveDelay : MonoBehaviour
{
    public GameObject[] gameObjects;
    public float[]      delays;

	void OnEnable ()
    {
        for( int i = 0; i < gameObjects.Length; i++ )
        {
            gameObjects[i].SetActive( false );
            StartCoroutine( DelayActive( delays[i], gameObjects[i] ) );
        }
    }

    IEnumerator DelayActive( float delay, GameObject obj )
    {
        yield return new WaitForSeconds( delay );

        obj.SetActive( true );
    }
}
