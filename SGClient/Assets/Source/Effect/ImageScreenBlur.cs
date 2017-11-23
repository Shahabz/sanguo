using UnityEngine;
using System.Collections;
using UnityEngine.UI;

[RequireComponent(typeof(Image))]
public class ImageScreenBlur : MonoBehaviour
{
    Image   _Image;
    static Texture2D   _ScreenShot;
    static Sprite      _Sprite;

	void Awake ()
    {
	    _Image = GetComponent<Image>();
        if( _Image == null )
            Destroy( this );
        
        // 先创建一个的空纹理，大小可根据实现需要来设置 
        if( _ScreenShot == null )
            _ScreenShot = new Texture2D( Screen.width, Screen.height, TextureFormat.RGB24, false );
        if( _Sprite == null )
            _Sprite = Sprite.Create( _ScreenShot, new Rect( 0, 0, Screen.width, Screen.height ), new Vector2( 0.5f, 0.5f ) );
        
        _Image.sprite = _Sprite;
	}

    void OnEnable()
    {
        Refresh();
    }

    public void Refresh()
    {
        _Image.color = Color.clear;
        StartCoroutine( RefreshOnRender() );
    }

    IEnumerator RefreshOnRender()
    {
        yield return new WaitForEndOfFrame();
        CaptureScreenshot();
        _Image.color = Color.white;
    }

    Texture2D CaptureScreenshot()   
    {  
        // 读取屏幕像素信息并存储为纹理数据，  
        _ScreenShot.ReadPixels( new Rect( 0, 0, Screen.width, Screen.height ), 0, 0 );  
        _ScreenShot.Apply();
        // 缩小贴图
  
        // 最后，我返回这个Texture2d对象，这样我们直接，所这个截图图示在游戏中，当然这个根据自己的需求的。  
        // return ScaleTextureBilinear( _ScreenShot, 0.5f );  
        return _ScreenShot;
    }

    //Texture2D ScaleTextureBilinear(Texture2D originalTexture, float scaleFactor)  
    //{  
    //    Texture2D newTexture = new Texture2D(Mathf.CeilToInt (originalTexture.width * scaleFactor), Mathf.CeilToInt (originalTexture.height * scaleFactor));  
    //    float scale = 1.0f / scaleFactor;  
    //    int maxX = originalTexture.width - 1;  
    //    int maxY = originalTexture.height - 1;  
    //    for (int y = 0; y < newTexture.height; y++)  
    //    {  
    //        for (int x = 0; x < newTexture.width; x++)  
    //        {  
    //            // Bilinear Interpolation  
    //            float targetX = x * scale;  
    //            float targetY = y * scale;  
    //            int x1 = Mathf.Min(maxX, Mathf.FloorToInt(targetX));  
    //            int y1 = Mathf.Min(maxY, Mathf.FloorToInt(targetY));  
    //            int x2 = Mathf.Min(maxX, x1 + 1);  
    //            int y2 = Mathf.Min(maxY, y1 + 1);  
  
    //            float u = targetX - x1;  
    //            float v = targetY - y1 ;  
    //            float w1 = (1 - u) * (1 - v);  
    //            float w2 = u * (1 - v);  
    //            float w3 = (1 - u) * v;  
    //            float w4 = u * v;  
    //            Color color1 = originalTexture.GetPixel(x1, y1);  
    //            Color color2 = originalTexture.GetPixel(x2, y1);  
    //            Color color3 = originalTexture.GetPixel(x1, y2);  
    //            Color color4 = originalTexture.GetPixel(x2,  y2);  
    //            Color color = new Color(Mathf.Clamp01(color1.r * w1 + color2.r * w2 + color3.r * w3+ color4.r * w4),  
    //                Mathf.Clamp01(color1.g * w1 + color2.g * w2 + color3.g * w3 + color4.g * w4),  
    //                Mathf.Clamp01(color1.b * w1 + color2.b * w2 + color3.b * w3 + color4.b * w4),  
    //                Mathf.Clamp01(color1.a * w1 + color2.a * w2 + color3.a * w3 + color4.a * w4)  
    //                );  
    //            newTexture.SetPixel(x, y, color);  
    //        }  
    //    }  
  
    //    newTexture.Apply();
    //    return newTexture;  
    //}  
}
