using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ActiveDelayFrame : MonoBehaviour {

	public Transform[]              objList;
	public int                      numPreFrame = 1;
	public bool allchild = false;
	List<GameObject> _List = new List<GameObject>();

	void Start ()
	{
		GetObjs();

		// 如果没有指定List则自动加入所有孩子
		if( objList.Length == 0 )
		{
			InitList( transform );
		}
		StartCoroutine( Load() );
	}

	void InitList( Transform trans )
	{
		foreach ( Transform obj in trans )
		{
			_List.Add( obj.gameObject );
			if (allchild == true) {
				InitList( obj );
			}
		}
	}

	void GetObjs()
	{        
		foreach ( Transform obj in objList )
		{
			_List.Add( obj.gameObject );
			foreach( Transform trans in obj )
			{
				_List.Add( trans.gameObject );
			}
		}
	}

	IEnumerator Load()
	{
		int count = 0;
		foreach ( var obj in _List )
		{
			obj.gameObject.SetActive( true );
			count++;
			if( count == numPreFrame )
			{
				count = 0;
				yield return 0;
			}
		}
	}
}
