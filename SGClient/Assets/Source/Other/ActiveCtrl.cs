using UnityEngine;
using System.Collections;

public class ActiveCtrl : MonoBehaviour
{    
    public GameObject[]   gameObjects;	

    public void Show( int index )
    {
        gameObjects[index].SetActive( true );
    }
    public void Hide( int index )
    {
        gameObjects[index].SetActive( false );
    }
}
