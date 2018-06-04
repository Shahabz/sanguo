using UnityEngine;
using System.Collections;

public class ActiveChildrenRandomDelay : MonoBehaviour
{
    public Vector2 range;

	void Start ()
    {
        for( int i = 0; i < transform.childCount; i++ )
        {
            StartCoroutine( DelayActive( Random.Range( range.x, range.y ), transform.GetChild( i ).gameObject ) );
        }
    }

    IEnumerator DelayActive( float delay, GameObject obj )
    {
        yield return new WaitForSeconds( delay );

        obj.SetActive( true );
    }
}
