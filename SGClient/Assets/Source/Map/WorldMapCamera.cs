using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine.EventSystems;
using LuaInterface;
using UnityEngine.UI;

/// <summary>
/// 地图Camara控制脚本
/// </summary>

public class WorldMapCamera : Camera2D
{
    // 控制的世界地图
    public WorldMap m_pWorldMap;
    // 屏幕的边框
    private Rect m_ScreenMargin = new Rect( 100, 200, 100, 200 );
	// 每多少帧通知位置变化
	public float m_viewChangeSec = 1.0f;
	public float m_viewChangeFrame = 1 / 30.0f;
    //是否可以移动摄像机
    public bool IsCanMoved = true;
		
    //初始化游戏信息设置
    protected override void Start()
    {
        base.Start();
        // 每N秒通知1次位置变化了
		InvokeRepeating( "ViewChangeSec", 0, m_viewChangeSec );
        // 每N秒通知1次位置变化了
		InvokeRepeating( "ViewChangeFrame", 0, m_viewChangeFrame );
    }

    protected override void OnDragStart( Vector2 pos )
    {
    }
    protected override void OnDrag( Vector2 pos )
    {
        object[] param = { pos };
        LuaFun.worldMapOnDrag.Call( param );

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
            m_pWorldMap.MapBorderLimit( ref cameraTrans );
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
        Vector2 touchpos;

#if UNITY_STANDALONE || UNITY_EDITOR
        touchpos = Input.mousePosition;
#else
           touchpos =  Input.GetTouch(0).position;
#endif
        int unit_index = -1;
        if ( obj != null )
        {
            ShareData sd = obj.GetComponent<ShareData>();
            if ( sd != null )
            {
                unit_index = Convert.ToInt32( sd.GetValue( "unit_index" ) );
            }
        }
        object[] param = { obj, touchpos, unit_index };
        LuaFun.worldMapOnClick.Call( param );
    }

    // 移动
    protected override void Move( float offsetX, float offsetY )
    {
        if ( IsCanMoved )
        {
            // 反转
            offsetX = -offsetX;
            offsetY = -offsetY;

            //移动
            Vector3 pos = _Camera.transform.position;
            pos.x += offsetX;
            pos.y += offsetY;

            // 对边界进行控制，返回控制后的位置
            m_pWorldMap.MapBorderLimit( ref pos );
            _Camera.transform.position = pos;
        }
    }

    // 每秒通知3次位置变化了
    void ViewChangeSec()
    {
        LuaFun.worldMapViewChangeSec.Call();
    }

    // 每秒通知30次位置变化了
    void ViewChangeFrame()
    {
        LuaFun.worldMapViewChangeFrame.Call();
    }

    void OnGUI()
    {
        //int screen_x = (int)(_Camera.transform.position.x * 100) - 120 * 256;
        //int screen_y = (int)(_Camera.transform.position.y * 100) + 120 * 128;

        //int logic_ptx = (int)((screen_x-256*screen_y/128));
        //int logic_pty = (int)(-((screen_x+256*screen_y/128)));

        //if(logic_ptx<0)
        //    logic_ptx -= 256;
        //if(logic_pty<0)
        //    logic_pty -= 256;
        //int logic_x = (int)(logic_ptx/256)+120;
        //int logic_y = (int)(logic_pty/256)+120;

        //GUI.Label(new Rect(0, 0, 300, 20), "Screen:(" + screen_x.ToString() + "," + screen_y.ToString() + ")");
        //GUI.Label(new Rect(0, 20, 300, 20), "Logic:(" + logic_x.ToString() + "," + logic_y.ToString() + ")");
        //GUI.Box( new Rect( Screen.width / 2 - 5, Screen.height / 2 - 5, 10, 10 ), "" );
    }
}

