using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using LuaInterface;
using UnityEngine.UI;

public class UIManager : MonoBehaviour
{
	public const int	UI_MAXCOUNT = 16;

    Transform			_UIRoot; 
	Transform[]			_Layers = new Transform[4];
    Camera				_UICamera;

    // 顺序查找
	List<UISystem>		            _UIList = new List<UISystem>();
    // 名字查找
    Dictionary<string,UISystem>     _UIListByName = new Dictionary<string,UISystem>();
    // childUI栈
    List<UISystem>                  _ChildUIStack = new List<UISystem>();


    void Awake()
    {
        GameObject uiRoot = GameObject.Find( "UIRoot" );
        if ( uiRoot == null )
        {
            Debug.LogError( "Can't Find UIRoot!" );
            return;
        }
        _UIRoot = uiRoot.transform;

        Transform uicamera = _UIRoot.Find( "UICamera" );
        if ( uicamera == null )
        {
            Debug.LogError( "Can't Find UICamera!" );
            return;
        }
        _UICamera = uicamera.GetComponent<Camera>();
        
		for( int i = 0; i < _Layers.Length; i++ )
		{
			Transform layer = _UIRoot.Find( "Layer" + i );
			if( layer == null )
			{
				Debug.LogError( "Can't Find Layer" + i );
				return;
			}
			_Layers[i] = layer;
		}
    }

    // 打开界面，每次都调
    public GameObject Open( string uiName )
    {
        // 是否存在，交给存在的界面处理
        UISystem uiSystem = null;
        if( _UIListByName.TryGetValue( uiName, out uiSystem ) )
        {
            OnOpen( uiSystem );
            return uiSystem.gameObject;
        }

		// 超过最大数量，析构最老的那个
        if( _UIList.Count >= UI_MAXCOUNT )
        {
            for( int i = 0; i < _UIList.Count; i++ )
            {
                // 子界面堆栈中的界面不删除
                if( _UIList[i].autoDestroy && !_ChildUIStack.Contains( _UIList[i] )  )
                {
                    GameObject.Destroy( _UIList[i].gameObject );
                    break;
                }
            }
        }

		// 读资源
        GameObject obj = ResourceManager.LoadPrefab( uiName );
        //GameObject obj = res.load(uiName);
        if ( obj == null )
        {
            Debug.LogError( "Can't Find UI:" + uiName );
            return null;
        }
        obj.SetActive( false );
		GameObject go = GameObject.Instantiate<GameObject>( obj );
        obj.SetActive( true );
        go.name = uiName;
		uiSystem = go.GetComponent<UISystem>();
		if( uiSystem == null )
		{
			Destroy( obj );
			Debug.LogError( uiName + "is not a UI!" );
			return null;
		}

		go.transform.SetParent( _Layers[ (int)uiSystem.layer ] );
        go.transform.localScale = Vector3.one;
        go.GetComponent<RectTransform>().anchoredPosition = Vector2.zero;
        go.GetComponent<RectTransform>().sizeDelta = Vector2.zero;

		_UIList.Add( uiSystem );
        _UIListByName.Add( uiName, uiSystem );
        //uiSystem.gameObject.SetActive( false );
        OnOpen( uiSystem );
        return go;
    }
    
    // 界面打开时调用
    public void OnOpen( UISystem uiSystem )
    {
        // 如果这个界面刚刚已经打开则返回
        if( _UIList.Count > 0 && uiSystem.gameObject.activeSelf == true )
        {            
            ResetToFront( uiSystem );
            return;
        }

        ResetToFront( uiSystem );

        // 处理childUI
        if( uiSystem.childUI )
        {
            // 新打开，非返回，返回采用简单打开
            if( _ChildUIStack.Count == 0 )
            {
                // 找到最后一个打开的界面
                for( int i = _UIList.Count - 1; i >= 0; i-- )
                {
                    if( _UIList[i].layer == UILayer.LAYER1 && _UIList[i] != uiSystem && _UIList[i].gameObject.activeSelf )
                    {
                        // 简单关闭
                        Close_End( _UIList[i] );

                        // 根入栈
                        _ChildUIStack.Add( _UIList[i] );
                        break;
                    }
                }
            }
            else
            {
                // 简单关闭
                Close_End( _ChildUIStack[ _ChildUIStack.Count - 1 ] );
            }
            
            _ChildUIStack.Add( uiSystem );
        }

        Open_End( uiSystem );
    }

    // 移至队列最后
    void ResetToFront( UISystem uiSystem )
    {
        if( _UIList.Contains( uiSystem ) == false || _UIList[ _UIList.Count - 1 ] == uiSystem )
        {
            return;
        }

        _UIList.Remove( uiSystem );
        _UIList.Add( uiSystem );

        // 设到最前
        uiSystem.transform.SetAsLastSibling();
    }

    // 打开或关闭时，最后调用
    void Open_End( UISystem uiSystem )
    {
        uiSystem.gameObject.SetActive( true );
        ResetToFront( uiSystem );
    }
    void Close_End( UISystem uiSystem )
    {
        uiSystem.gameObject.SetActive( false );
        
        // 如果最后一个界面关闭(除Layer0的界面)，发事件
        if( IsAllUIClose() )
        {
            LuaFun.allUIClose.Call();
        }

    }

    // 是否有其它界面打开
    public bool IsAllUIClose()
    {
        // 从后往前遍历会更快些
        for( int i = _UIList.Count - 1; i >= 0; i-- )
        {
            if( _UIList[i].gameObject.activeSelf && _UIList[i].layer == UILayer.LAYER1 )
            {
                return false;
            }
        }

        return true;
    }

    // 除Layer0界面打开的个数
    public int OpenCount()
    {
        int count = 0;
        for( int i = _UIList.Count - 1; i >= 0; i-- )
        {
            if( _UIList[i].gameObject.activeSelf && _UIList[i].layer == UILayer.LAYER1 )
            {
                count++;
            }
        }

        return count;
    }

    // 界面关闭时调用
    public void Close( string uiName )
    {
        UISystem uiSystem = null;
        if( _UIListByName.TryGetValue( uiName, out uiSystem ) )
        {            
            OnClose( uiSystem );
        }
    }

    public void OnClose( UISystem uiSystem )
    {
        if( uiSystem.childUI )
        {
            // 出栈
            _ChildUIStack.Remove( uiSystem );
            
            // 简单打开
            if( _ChildUIStack.Count > 0 )
            {
                Open_End( _ChildUIStack[ _ChildUIStack.Count - 1 ] );       
            }
        }
        else
        {
            if( _ChildUIStack.Count == 1 && _ChildUIStack[0] == uiSystem )
                _ChildUIStack.Clear();
        }

        Close_End( uiSystem );
    }
    
    public void OnClose( UISystem uiSystem, bool force )
    {
        if( force )
        {
            if( uiSystem.childUI && uiSystem.gameObject.activeSelf )
                _ChildUIStack.Clear();
            Close_End( uiSystem );
        }
        else
            OnClose( uiSystem );
    }

    // 关闭最后打开的界面
    public bool CloseLast()
    {
        for( int i = _UIList.Count - 1; i >= 0; i-- )
        {
            if( _UIList[i].gameObject.activeSelf && _UIList[i].layer != UILayer.LAYER0 )
            {
                _UIList[i].Close();
                return true;
            }
        }
        return false;
    }

    // 关闭所有界面Layer1&Layer2
    // 关闭最后打开的界面
    public void CloseAll()
    {
        for( int i = _UIList.Count - 1; i >= 0; i-- )
        {
            if( _UIList[i].gameObject.activeSelf && _UIList[i].layer != UILayer.LAYER0 )
            {
                OnClose( _UIList[i], true );
            }
        }
    }

    // 关闭所有带词缀的界面
    public void CloseAll( string prefix )
    {
        for( int i = _UIList.Count - 1; i >= 0; i-- )
        {
            if( _UIList[i].gameObject.activeSelf && _UIList[i].gameObject.name.Contains( prefix ) )
            {
                OnClose( _UIList[i], true );
            }
        }
    }

    // UI 根节点
    public Transform GetUIRoot()
    {
        return _UIRoot;
    }

    // UI摄影机
    public Camera GetUICamera()
    {
        return _UICamera;
    }

    // 获得画布
    public Canvas GetCanvas()
    {
        return _UIRoot.GetComponent<Canvas>();
    }

    // 获得对应层
	public Transform GetLayer( int index )
	{
		if( index < 0 || index > _Layers.Length )
		{
			return null;
		}
		return _Layers[ index ];
	}

    // 界面析构时调用
    public void Unload( UISystem ui )
    {
		_UIList.Remove( ui );
        _UIListByName.Remove( ui.uiName );
    }

    // 清空UI
    public void Clear()
    {
        for( int i = 0; i < _Layers.Length; i++ )
        {
            for( int j = 0; j < _Layers[i].childCount; j++ )
            {
                if( _Layers[i] != null )
                    Destroy( _Layers[i].GetChild( j ).gameObject );
            }
        }

        _UIList.Clear();
        _UIListByName.Clear();
        _ChildUIStack.Clear();
    }

    // 开启全屏遮挡
    public void ShowMask( bool show )
    {
        GameObject.Find( "UIRoot" ).transform.Find( "Mask" ).gameObject.SetActive( show );
    }
}
