using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine.EventSystems;
using LuaInterface;
using UnityEngine.UI;
using LKCamera;

/// <summary>
/// 地图Camara控制脚本
/// </summary>

public class WorldMapMiniCamera : Camera2D
{
    // 控制的世界地图
	public WorldMapMini m_pWorldMapMini;
    // 屏幕的边框
    private Rect m_ScreenMargin = new Rect( 100, 200, 100, 200 );

	//定义枚举类型  
	public enum HorizontalAlignment {left, center, right};  
	public enum VerticalAlignment{top, middle, bottom};  
	public enum ScreenDimensions{pixels, screen_percentage};  

	//定义枚举类型的变量  
	public HorizontalAlignment horizontalAlignment = HorizontalAlignment.left;  
	public VerticalAlignment verticalAlignment = VerticalAlignment.top;  
	public ScreenDimensions dimensions = ScreenDimensions.pixels;  

	public int width = 50;  
	public int height = 50;  
	public float xOffset = 0.0f;  
	public float yOffset = 0.0f;  
	public bool update = true;  

	private int hSize, vSize, hLoc, vLoc;  

    //初始化游戏信息设置
    protected override void Start()
    {
        base.Start();
		float w = Screen.width;
		float h = Screen.height;
		//_Camera.rect = new Rect ( (w-256)/w, (h-128)/h, 1, 1 );
    }

    protected override void OnDragStart( Vector2 pos )
    {
    }
    protected override void OnDrag( Vector2 pos )
    {
        //判断拖拽的对象是否已经到摄像机的边缘
        if ( _DragObj != null )
        {
            Vector3 objScreenPos = _Camera.WorldToScreenPoint( _DragObj.transform.position );
            Vector3 cameraTrans = new Vector3( 0, 0, 0 );
            if ( objScreenPos.x < m_ScreenMargin.x )
                cameraTrans.x = m_ScreenMargin.x - objScreenPos.x;
            if ( ( _Camera.pixelWidth - objScreenPos.x ) < m_ScreenMargin.width )
                cameraTrans.x = _Camera.pixelWidth - m_ScreenMargin.width - objScreenPos.x;
            if ( objScreenPos.y < m_ScreenMargin.y )
                cameraTrans.y = m_ScreenMargin.y - objScreenPos.y;
            if ( ( _Camera.pixelHeight - objScreenPos.y ) < m_ScreenMargin.height )
                cameraTrans.y = _Camera.pixelHeight - m_ScreenMargin.height - objScreenPos.y;
            Vector3 pos1 = _Camera.ScreenToWorldPoint( objScreenPos + cameraTrans );
            cameraTrans = -pos1 + _DragObj.transform.position;
            
            // 矫正摄像机边缘
            cameraTrans += _Camera.transform.position;
			m_pWorldMapMini.MapBorderLimit( ref cameraTrans );
            _Camera.transform.position = cameraTrans;
        }
    }
    protected override void OnDragEnd()
    {
    }
    protected override void OnPress( Transform obj )
    {
    }
    protected override void OnClick( Transform obj )
    {
    }

    // 移动
    protected override void Move( float offsetX, float offsetY )
    {
		if (m_pWorldMapMini) {
			// 反转
			offsetX = -offsetX;
			offsetY = -offsetY;

			//移动
			Vector3 pos = _Camera.transform.position;
			pos.x += offsetX;
			pos.y += offsetY;

			// 对边界进行控制，返回控制后的位置
			m_pWorldMapMini.MapBorderLimit (ref pos);
			_Camera.transform.position = pos;
		}
    }

	void FixedUpdate()  
	{ 
		AdjustCamera();  
	}  
	//游戏对象初始化时，调用此函数  
	void AdjustCamera()  
	{  
		if (dimensions == ScreenDimensions.screen_percentage)            //调节视图为指定百分比大小  
		{  
			hSize = (int)(width * 0.01f * Screen.width);  
			vSize = (int)(height * 0.01f * Screen.height);  
		}  
		else                                 //调节视图为指定像素大小  
		{  
			hSize = height;  
			vSize = width;  
		}  

		if (horizontalAlignment == HorizontalAlignment.left)               //水平方向上是左对齐  
		{  
			hLoc = (int)(xOffset * 0.01f * Screen.width);  
		}  
		else if(horizontalAlignment == HorizontalAlignment.right)         //水平方向上是右对齐  
		{  
			hLoc = (int)((Screen.width - hSize) - (xOffset * 0.01f * Screen.width));  
		}  
		else                                                             //水平方向上是居中  
		{  
			hLoc = (int)((Screen.width - hSize) * 0.5f - (xOffset * 0.01f * Screen.width));  
		}  

		if (verticalAlignment == VerticalAlignment.top)                     //垂直方向为顶端  
		{  
			vLoc = (int)((Screen.height - vSize) - (yOffset * 0.01f * Screen.height));  
		}  
		else if (verticalAlignment == VerticalAlignment.bottom)             //垂直方向为底端  
		{  
			vLoc = (int)(yOffset * 0.01f * Screen.height);  
		}  
		else                                  //垂直方向为居中  
		{  
			vLoc = (int)((Screen.height - vSize) * 0.5f - (yOffset * 0.01f * Screen.height));  
		}  

		_Camera.pixelRect = new Rect(hLoc, vLoc, hSize, vSize);  
	}  
}

