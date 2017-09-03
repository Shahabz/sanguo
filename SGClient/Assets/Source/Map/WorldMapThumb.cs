using UnityEngine;
using System;
using System.Collections;
using LuaInterface;
using System.Collections.Generic;
public class WorldMapThumb : MonoBehaviour {
    // 地图摄像机
    protected WorldMapThumbCamera m_pWorldMapThumbCamera;

    // Use this for initialization
	void Start () {

        // 把自己的赋给摄像机
        m_pWorldMapThumbCamera = transform.Find("ThumbCamera").GetComponent<WorldMapThumbCamera>();
        m_pWorldMapThumbCamera.m_pWorldMapThumb = this;

        // 获取脚本函数
        object[] param = { this };
        LuaFun.worldMapThumbStart.Call( param );
	}
	
	// Update is called once per frame
	void Update () {   
	}
}
