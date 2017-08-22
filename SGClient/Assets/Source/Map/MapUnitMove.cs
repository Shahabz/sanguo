using UnityEngine;
using System;
using System.Collections;
using LuaInterface;
using DG.Tweening;

// 世界地图显示单元移动脚本
public class MapUnitMove : MonoBehaviour
{
    public int stat = 0;                    // 1开始行走
    public Vector3 toPosition;              // 目的坐标
    public float speed = 0.01f;             // 速度
    // Use this for initialization
    void Start()
    {
    }

    // Update is called once per frame
    void Update()
    {
        if ( stat == 1 )
        {
            transform.localPosition = Vector3.MoveTowards( transform.localPosition, toPosition, speed*Time.deltaTime );
        }
        // 总距离
        //float m = (float)Math.Pow(Math.Pow(( to_pos.x - from_pos.x ), 2) + Math.Pow(( to_pos.y - from_pos.y ), 2), 0.5);
        //n += 0.01f;
        //cur_pos.x = ( ( to_pos.x - from_pos.x ) * n ) / m + from_pos.x;
        //cur_pos.y = ( ( to_pos.y - from_pos.y ) ) * n / m + from_pos.y;
        //gameObject.GetComponent<Transform>().localPosition = new Vector3(cur_pos.x, cur_pos.y, 0);
    }


}
