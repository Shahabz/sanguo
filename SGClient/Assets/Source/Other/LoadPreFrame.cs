using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class LoadPreFrame : MonoBehaviour
{
    public Transform[]              loadList;
    public GameManager.LuaExecute   onLoading;
    public int                      numPreFrame = 2;

    List<GameObject> _List = new List<GameObject>();
    int _Curr = 0;
    int _Last = 0;
	void Start ()
    {
	    GetObjs();

        // 如果没有指定List则自动加入所有孩子
        if( loadList.Length == 0 )
        {
            InitList( transform );
        }


        // 编辑器模式下直接显示全部
//#if UNITY_STANDALONE || UNITY_EDITOR
//        foreach ( var obj in _List )
//        {
//            obj.gameObject.SetActive( true );
//            _Curr++;
//        }
//        return;
//#endif

        StartCoroutine( Load() );
	}

    void InitList( Transform trans )
    {
        foreach ( Transform obj in trans )
        {
            _List.Add( obj.gameObject );
            InitList( obj );
        }
    }

    void GetObjs()
    {        
        foreach ( Transform obj in loadList )
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
            _Curr++;
            count++;
            if( count == numPreFrame )
            {
                count = 0;
                yield return 0;
            }
        }
    }

    void Update()
    {
        if( _Curr != _Last )
        {
            if( onLoading != null )
                onLoading( new object[]{ _Curr, _List.Count } );

            if( _Curr == _List.Count )
                Destroy( this );
        }
    }
}
