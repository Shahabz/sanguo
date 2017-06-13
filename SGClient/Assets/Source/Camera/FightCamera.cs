using UnityEngine;
using System.Collections;
using LuaInterface;

public class FightCamera : Camera2D
{
    // 分层拖动
    public Transform[]      layers;
    public Vector2[]        layersMoveRatios;

    protected override void OnDragStart( Vector2 pos )
    {
    }

    protected override void OnDrag( Vector2 pos )
    {
    }

    protected override void OnDragEnd()
    {
    }

    public void OnFightClick( Transform obj )
	{
       
	}
    protected override void OnClick( Transform obj )
    {
        OnFightClick(obj);
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
}
