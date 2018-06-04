using UnityEngine;
using System.Collections;
using UnityEngine.UI;

public class Novice_ScreenCut : MonoBehaviour
{
    public Image    image;
    public GameObject[]   gameObjects;

    Texture2D _ScreenShot;

    void Awake()
    {
        _ScreenShot = new Texture2D( Screen.width, Screen.height, TextureFormat.RGB24, false );        
        image.sprite =  Sprite.Create( _ScreenShot, new Rect( 0, 0, Screen.width, Screen.height ), new Vector2( 0.5f, 0.5f ) );
    }    

    public void Refresh()
    {
        StartCoroutine( RefreshOnRender() );
    }

    IEnumerator RefreshOnRender()
    {
        yield return new WaitForEndOfFrame();

        _ScreenShot.ReadPixels( new Rect( 0, 0, Screen.width, Screen.height ), 0, 0 );  
        _ScreenShot.Apply();
    }

    public void Show( int index )
    {
        gameObjects[index].SetActive( true );
    }
    public void Hide( int index )
    {
        gameObjects[index].SetActive( false );
    }
}
