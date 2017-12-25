using UnityEngine;
using System;
using System.Collections;
using LuaInterface;
using System.Collections.Generic;
public class WorldMapMini : MonoBehaviour
{
    // 地图摄像机
	protected WorldMapMiniCamera m_pWorldMapMiniCamera;

    // 世界地图大小（格子数）
    public float m_nMaxWidth;
    public float m_nMaxHeight;

    // 格子大小（像素）
    public float MAP_TILEWIDTH;
    public float MAP_TILEHEIGHT;

    // 四条边 先左右，后下上
    Vector4     _Limit;
    private Transform m_MapUnitRoot;

	// Use this for initialization
	void Start () {
        // 获取脚本函数
        object[] param = { this };
        object[] ret = LuaFun.worldMapMiniStart.Call( param );
        m_nMaxWidth = float.Parse(ret[0].ToString());
        m_nMaxHeight = float.Parse(ret[1].ToString());
        MAP_TILEWIDTH = float.Parse(ret[2].ToString());
        MAP_TILEHEIGHT = float.Parse(ret[3].ToString());

        // 把自己的赋给摄像机
		m_pWorldMapMiniCamera = transform.Find("MapMiniCamera").GetComponent<WorldMapMiniCamera>();
		m_pWorldMapMiniCamera.m_pWorldMapMini = this;

        // 菱形顶点
        _Limit = new Vector4( -m_nMaxWidth * MAP_TILEWIDTH / 200f + 0.5f, m_nMaxWidth * MAP_TILEWIDTH / 200f - 0.5f,
            -m_nMaxHeight * MAP_TILEHEIGHT / 200f + 0.5f, m_nMaxHeight * MAP_TILEHEIGHT / 200f - 0.5f );
        m_MapUnitRoot = this.transform.Find("MapUnitRoot");
	}
	
	// Update is called once per frame
	void Update () {
	     LuaFun.worldMapUpdate.Call();
	}

    // 转换到合法坐标
    public void MapBorderLimit( ref Vector3 pos )
    {
        // 偏移坐标
        pos -= new Vector3( m_nMaxWidth * MAP_TILEWIDTH / 200f, -m_nMaxHeight * MAP_TILEHEIGHT / 200f, 0f );

        // 先约束到矩形内
        if( pos.x < _Limit.x )
            pos.x = _Limit.x;
        else if( pos.x > _Limit.y )
            pos.x = _Limit.y;
        if( pos.y < _Limit.z )
            pos.y = _Limit.z;
        else if( pos.y > _Limit.w )
            pos.y = _Limit.w;

        // 然后以x为基准求y的值
        float x = ( m_nMaxWidth * MAP_TILEWIDTH / 200f - 0.5f - Math.Abs( pos.x ) );
        float y = x * MAP_TILEHEIGHT / MAP_TILEWIDTH * ( pos.y / Math.Abs( pos.y ) );

        if ( pos.y > 0 && pos.y > y )
            pos.y = y;
        else if ( pos.y < 0 && pos.y < y )
            pos.y = y;

        // 还原
        pos += new Vector3( m_nMaxWidth * MAP_TILEWIDTH / 200f, -m_nMaxHeight * MAP_TILEHEIGHT / 200f, 0f );
    }

     // 游戏坐标(菱形格)=》摄像机坐标
    public Vector2 ConvertGameToCamera( int gameCoorX, int gameCoorY )
    {
        float px  = ( MAP_TILEWIDTH/2/100 ) * ( gameCoorX - gameCoorY );
        float py  = -( MAP_TILEHEIGHT/2/100 ) * ( gameCoorX + gameCoorY );
        return new Vector2(px + m_nMaxWidth * MAP_TILEWIDTH/2/100, py );
    }
}
