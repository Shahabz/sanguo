using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;
using LuaInterface;
using DG.Tweening;

// 部队移动脚本
public class ArmyMove : MonoBehaviour
{
	public short stat = 0;                  // 1开始行走
	public Vector3 fromPosition;            // 出发坐标
	public Vector3 toPosition;              // 目的坐标
	public short reback;
	public float speed = 0.01f;             // 速度
	public GameObject[] 	heroGameObject; // 武将
	public short heroCount = 1;				// 需要显示的武将数量
	public short invokeShow = 0;			// 是否开启延迟显示
	public float direction = 0.8f;			// 超过这个距离就显示一个
	public float direction1 = 0.5f;			// 超过这个距离就显示一个
	public float direction2 = 0.2f;			// 超过这个距离就显示一个
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
			if ( invokeShow == 1 )
			{ // 有延迟显示
				if (reback == 0 ) 
				{
					float d = (transform.localPosition - fromPosition).magnitude;
					if (heroCount == 4) 
					{
						if (d > direction * 3) 
						{
							invokeShow = 0;
							for (int tmpi = 0; tmpi < heroCount; tmpi++) {
								heroGameObject [tmpi].SetActive (true);
							}
						} else if (d > direction * 2) {
							for (int tmpi = 0; tmpi < heroCount - 1; tmpi++) {
								heroGameObject [tmpi].SetActive (true);
							}
						} else if (d > direction) {
							for (int tmpi = 0; tmpi < heroCount - 2; tmpi++) {
								heroGameObject [tmpi].SetActive (true);
							}
						}
					} 
					else if (heroCount == 3) 
					{
						if (d > direction1 * 3) {
							invokeShow = 0;
							for (int tmpi = 0; tmpi < heroCount; tmpi++) {
								heroGameObject [tmpi].SetActive (true);
							}
						} else if (d > direction1 * 2) {
							for (int tmpi = 0; tmpi < heroCount - 1; tmpi++) {
								heroGameObject [tmpi].SetActive (true);
							}
						} 
					} 
					else if (heroCount == 2) 
					{
						if (d > direction2 * 3) {
							invokeShow = 0;
							for (int tmpi = 0; tmpi < heroCount; tmpi++) {
								heroGameObject [tmpi].SetActive (true);
							}
						}
					}
				} 	
			}
		}

	}


}
