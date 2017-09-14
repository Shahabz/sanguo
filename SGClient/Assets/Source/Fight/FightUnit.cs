using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public enum FightUnitState
{
    None = 0,
    Walk = 1,
    Attack = 2
}

public class FightUnit : MonoBehaviour {

    public Vector3 toPosition;              // 目的坐标
    public float speed = 0.01f;             // 速度

    void Start () {
		
	}
	
	void Update () {
		
	}
}
