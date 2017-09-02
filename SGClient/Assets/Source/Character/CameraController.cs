using UnityEngine;
using System.Collections;

public class CameraController : MonoBehaviour 
{
    Camera2D _Camera;

    void Start()
    {
		if(  GetComponentInParent<City>() != null )
			_Camera = GetComponentInParent<City>().transform.GetComponentInChildren<Camera2D>();
        else
            _Camera = GetComponentInParent<WorldMap>().transform.GetComponentInChildren<Camera2D>();
    }

    public void Zoom( string param )
    {
        string[] pa = param.Split( new char[]{ ',' } );
        if( pa.Length < 2 )
            return;

        _Camera.TweenSizeTo( float.Parse( pa[0] ), float.Parse( pa[1] ) );
    }

    public void Move( string param )
    {
        string[] pa = param.Split( new char[]{ ',' } );
        if( pa.Length < 3 )
            return;

        _Camera.TweenPosTo( new Vector3( float.Parse( pa[0] ), float.Parse( pa[1] ), 0 ), float.Parse( pa[2] ) );
    }

	public void Shake( string param )
	{
		string[] pa = param.Split( new char[]{ ',' } );
		if( pa.Length < 3 )
			return;

		_Camera.Shake( float.Parse( pa[0] ), float.Parse( pa[1] ), float.Parse( pa[2] ) );
	}
}
