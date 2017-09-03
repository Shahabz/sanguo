using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine.EventSystems;
using LuaInterface;
using UnityEngine.UI;

/// <summary>
/// 地图缩略图Camara控制脚本
/// </summary>

public class WorldMapThumbCamera : Camera2D
{
    // 控制的世界地图缩略图
    public WorldMapThumb m_pWorldMapThumb;

    //是否可以移动摄像机
    public bool IsCanMoved = true;

    //初始化游戏信息设置
    protected override void Start()
    {
        base.Start();
    }

    protected override void OnDragStart( Vector2 pos )
    {
    }

    protected override void OnDrag( Vector2 pos )
    {
        object[] param = { pos };
        LuaFun.worldMapThumbOnDrag.Call( param );
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

        object[] param = { obj, touchpos };
        LuaFun.worldMapThumbOnClick.Call( param );
    }
}

