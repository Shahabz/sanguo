using System.Collections; 
using UnityEngine;
using UnityEngine.UI;  
using UnityEngine.EventSystems;  
using System.Collections.Generic;  
using System;  
 
public class UIScrollPage : MonoBehaviour, IBeginDragHandler, IEndDragHandler  
{  
	public int controlID = 0;
	public UIMod uiMod;
	public float smooting = 5;                          // 滑动速度  
	public int pageCount = 1;                           // 每页显示的项目  
	public float autoInterval = 5.0f;					// 自动翻页间隔

	List<GameObject> listItem =  new List<GameObject>();                	 // scrollview item   
	UIScrollRect srect;  
	float pageIndex;                                    //总页数  
	bool isDrag = false;                                //是否拖拽结束  
	List<float> listPageValue = new List<float> { 0 };  //总页数索引比列 0-1  
	float targetPos = 0;                                //滑动的目标位置  
	float nowindex = 0;                                 //当前位置索引  
	bool isplay = false;
	/// <summary>  
	/// 用于返回一个页码，页码从0开始  
	/// </summary>  
	public Action<int> OnPageChanged = null;
	public Action<int> OnCountChanged = null;
	void Awake()  
	{  
		srect = GetComponent<UIScrollRect>();  
		ListPageValueInit();  
	}  

	void Start()
	{
		// 挂载uiMod
		if (uiMod == null)
		{
			uiMod = gameObject.GetComponentInParent<UIMod>();
		}
	}

	void OnEnable()
	{
		srect.normalizedPosition = Vector2.zero;
		srect.horizontalNormalizedPosition = 0.0f;
		nowindex = 0;
		targetPos = 0;
		if (isplay)
			Play ();
	}

	void OnDisable()
	{
		if (isplay)
			Stop ();
	}

	//每页比例  
	void ListPageValueInit()  
	{  
		listPageValue.Clear ();
		pageIndex = (listItem.Count / pageCount)-1;  
		if (listItem != null && listItem.Count != 0)  
		{  
			for (float i = 0; i <= pageIndex; i++)  
			{  
				listPageValue.Add((i / pageIndex));  
			}  
		} 
		srect.horizontalNormalizedPosition = 0.0f;
	}  

	void Update()  
	{  
		if (!isDrag)  
			srect.horizontalNormalizedPosition = Mathf.Lerp(srect.horizontalNormalizedPosition, targetPos, Time.deltaTime * smooting);  
	}  
	/// <summary>  
	/// 拖动开始  
	/// </summary>  
	///<param name="eventData">  
	public void OnBeginDrag(PointerEventData eventData)  
	{  
		isDrag = true; 
		Stop ();
	
	}  
	/// <summary>  
	/// 拖拽结束  
	/// </summary>  
	///<param name="eventData">  
	public void OnEndDrag(PointerEventData eventData)  
	{  
		isDrag = false;  
		var tempPos = srect.horizontalNormalizedPosition; //获取拖动的值  
		var index = 0;  
		float offset = Mathf.Abs(listPageValue[index] - tempPos);    //拖动的绝对值  
		for (int i = 0; i < listPageValue.Count; i++)  
		{  
			float temp = Mathf.Abs(tempPos - listPageValue[i]);  
			if (temp < offset)  
			{  
				index = i;  
				offset = temp;  
			}  
		}  
		targetPos = listPageValue[index];  
		nowindex = index;  

		if (OnPageChanged != null)  
		{  
			OnPageChanged(Convert.ToInt32(nowindex));  
		}
		object[] param = { UIEVENT.SCROLLPAGE, controlID, Convert.ToInt32(nowindex) };
		uiMod.OnEvent(param);
	} 
		
	public void AddPage( GameObject obj )
	{
		obj.transform.SetParent (srect.content.transform);
		obj.transform.localPosition = new Vector3( 0, 0, 0 );
		obj.transform.localScale = new Vector3( 1, 1, 1 );
		listItem.Add (obj);
		ListPageValueInit (); 
	}

	public void AppendPage( GameObject obj )
	{
		obj.transform.SetParent (srect.content.transform);
		obj.transform.localPosition = new Vector3( 0, 0, 0 );
		obj.transform.localScale = new Vector3( 1, 1, 1 );
		listItem.Add (obj);
		ListPageValueInit (); 
		PageCountChanged ();
		Go (0);
	}

	public void DelPage( GameObject obj )
	{
		Destroy(obj.gameObject);
		listItem.Remove (obj);
		ListPageValueInit (); 
		PageCountChanged ();
		Go (0);
	}

	public void ClearPage()
	{
		Utils.ClearChild (srect.content.transform);
		srect.horizontalNormalizedPosition = 0.0f;
		listItem.Clear();
		ListPageValueInit (); 
	}

	public void PageCountChanged()
	{
		if (OnCountChanged != null)  
		{  
			OnCountChanged(listItem.Count);  
		}
	}

	public void Set( int index )
	{
		targetPos = listPageValue[Convert.ToInt32(nowindex)];  
		if (OnPageChanged != null)  
		{  
			OnPageChanged(Convert.ToInt32(nowindex));  
		}
		object[] param = { UIEVENT.SCROLLPAGE, controlID, Convert.ToInt32(nowindex) };
		uiMod.OnEvent(param);
	}

	public void Pre()  
	{  
		nowindex = Mathf.Clamp(nowindex - 1, 0, pageIndex);  
		Set(Convert.ToInt32 (nowindex));
	}  

	public void Next()
	{  
		nowindex = Mathf.Clamp(nowindex + 1, 0, pageIndex);  
		Set(Convert.ToInt32 (nowindex));
	}  

	public void Go( int index )
	{  
		nowindex = Mathf.Clamp(index, 0, pageIndex);  
		Set(Convert.ToInt32 (nowindex));
	} 

	public void Play()
	{
		isplay = true;
		InvokeRepeating( "OnAuto", 2.0f, autoInterval );
	}

	public void Stop()
	{
		isplay = false;
		CancelInvoke ("OnAuto");
	}


	public void OnAuto()
	{
		if (isDrag)
			return;
		srect.horizontalNormalizedPosition =  listPageValue[Convert.ToInt32(nowindex)];
		if (nowindex + 1 > pageIndex) {
			nowindex = 0;
		} else {
			nowindex += 1;
		}
		Set(Convert.ToInt32 (nowindex));
	}
}
