using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Fight : MonoBehaviour {

	// 每多少帧通知位置变化
	public float m_logicTimer = 0.1f;

	// Use this for initialization
	void Start () {
		InvokeRepeating( "Onlogic", 0, m_logicTimer );
	}
	
	// Update is called once per frame
	void Update () {
		
	}
		
	void OnDestroy() 
	{
		CancelInvoke ("Onlogic");
	}

	void Onlogic(){
		LuaFun.fightLogic.Call();
	}
}
