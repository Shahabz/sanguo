using UnityEngine;
using System.Collections;
using LuaInterface;

public class CityCamera : Camera2D
{
    public Transform        trail;

    // 分层拖动
    public Transform[]      layers;
    public Vector2[]        layersMoveRatios;
    public Vector2[]        layersScaleRatios;

	bool					_DragGain;
    LuaFunction             _OnCameraTween;

	protected override void OnDragStart( Vector2 pos ){ if( trail != null && _DragGain ) trail.transform.position = pos; }

	protected override void OnDrag( Vector2 pos )
	{
		if( _DragGain )
		{
			RaycastHit2D hit = Physics2D.Raycast( new Vector2( pos.x, pos.y ), Vector2.zero );
			//if( hit.collider != null )
			//	ResourceGain( hit.collider.transform );
            
            if( trail != null )
                trail.transform.position = pos;
		}
	}

	protected override void OnDragEnd()
	{
		_DragGain = false;
	}

    public void OnCityClick(Transform obj)
	{
        if( _DragGain )
            return;

        if ( obj != null && obj.tag == "Building" )
        {
            TweenPosToInBound( obj.transform.localPosition, 0.2f );
            //TweenSizeTo( defaultSize *0.8f, 0.2f );
            LuaFun.buildingselect.Call( obj );
        }
        else
        {
            //TweenSizeTo( defaultSize, 0.2f );
        }
	}
    protected override void OnClick(Transform obj)
    {
        OnCityClick(obj);
    }

	protected override void OnPress( Transform obj )
	{
	}

    protected override void Move( float offsetX, float offsetY )
    {
        base.Move( offsetX, offsetY );
        RefreshLayersMove();
    }

    void RefreshLayersMove()
    {
        Vector3 offset = _Camera.transform.position;
        for( int i = 0; i < layers.Length; i++ )
        {
            float z = layers[i].localPosition.z;
            layers[i].localPosition = new Vector3( layersMoveRatios[i].x * offset.x, layersMoveRatios[i].y * offset.y, z );
        }
    }

    protected override void  Zoom( float length, Vector3 focus )
    {
        if ( zoomLock )
            return;
        if ( length == 0 )
            return;

        base.Zoom( length, focus );
        RefreshLayersSize();
    }
    
    protected override void  ZoomMove( float length, Vector3 focus, Vector3 direction )
    {
        if ( zoomLock )
            return;
        if ( length == 0 )
            return;

        base.ZoomMove( length, focus, direction );
        RefreshLayersSize();
    }

    void RefreshLayersSize()
    {
        //if( _OnCameraTween == null )
        //    _OnCameraTween = eye.gameManager.uluaMgr.lua.GetFunction( "BuildingCameraOnTween" );
        //_OnCameraTween.Call( _Camera.orthographicSize );
        if ( zoomLock )
            return;
        for ( int i = 0; i < layers.Length; i++ )
        {
            layers[i].localScale = new Vector3( 1 + ( _Camera.orthographicSize / defaultSize - 1 ) * layersScaleRatios[i].x, 1 + ( _Camera.orthographicSize / defaultSize - 1 ) * layersScaleRatios[i].y, 1 );
        }
    }

    protected override void OnTween()
    {
        base.OnTween();
        RefreshLayersMove();
        RefreshLayersSize();
    }
}
