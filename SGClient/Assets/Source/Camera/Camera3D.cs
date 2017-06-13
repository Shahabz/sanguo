using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using UnityEngine.EventSystems;

/// <summary>
/// 城市专用Camara控制脚本
/// </summary>

public abstract class Camera3D : MonoBehaviour
{
	// UICamera
	public Camera uiCamera;
	// 速度衰减（乘）
	public float moveDecay = 0.075f;
	// 缩放限制
	public float zoomInLimit = 4f;
	public float zoomOutLimit = 10f;
	// 缩放速度
	public float zoomSpeed = 0.02f;
	// 缩放回弹阈值
	public float zoomRestrict = 1f;
	// 缩放时移动速度
	public float zoomOffsetSpeed = 0.1f;
	// 地图拖动限制，顺序为：left, top, right, bottom
	public Vector4 mapLimit;

	// 记录上一次点击地面的位置
	Vector3				_OldHit;
	// 记录上一次点击屏幕位置
	Vector2				_OldPosition1;
	Vector2				_OldPosition2;
	// 上次的Focus;
	Vector3				_LastFocus;
	// 摄像机
	protected Camera    _Camera;
	protected Vector4	_CameraLimit;
	// 触摸时焦点位置
	Vector2				_TargetPos;
	// 动量
	Vector3				_Momentum;
	// 是否发生过移动或缩放
	bool				_Moved = false;
	// 锁死触控
	bool				_Lock = false;
	// 拖动
	Camera3dDragable	_Draging;
	
	
	//初始化游戏信息设置
	protected virtual void Start()
	{
		_Camera = GetComponent<Camera>();
		if( _Camera == null )
		{
			Debug.LogError( "Camera3D is not binding to a Camera!" );
			return;
		}

		ResetCameraLimit();
	}
	
	void Update ()
	{
#if UNITY_STANDALONE || UNITY_EDITOR
		// 鼠标版
		if( Input.GetMouseButtonUp(0) )
		{
			if( _Lock )
			{
				// 解锁
				_Lock = false;
			}
			else
			{
				// 鼠标抬起时再进行事件检测，如果发生移动则Pass
				if( _Moved == false )
				{
					Ray ray = _Camera.ScreenPointToRay( Input.mousePosition );
					RaycastHit hit;
					if( Physics.Raycast( ray, out hit ) )
					{
						OnClick( hit.collider.transform );
					}
				}
				
				_Moved = false;
				_OldHit = Vector3.zero;
				_Draging = null;
			}
			
		}
		// 锁死则不接受鼠标事件
		else if( _Lock == false )
		{
			if( Input.GetMouseButtonDown(0) )
			{
				// 2D UI碰撞
				PointerEventData eventDataCurrentPosition = new PointerEventData( EventSystem.current );
				eventDataCurrentPosition.position = new Vector2( Input.mousePosition.x, Input.mousePosition.y );
				
				List<RaycastResult> results = new List<RaycastResult>();
				EventSystem.current.RaycastAll( eventDataCurrentPosition, results );
				
				if( results.Count > 0 )
				{
					for( int i = 0; i < results.Count; i++ )
					{
						if( results[i].gameObject.tag == "UI" )
						{
							_Lock = true;
							break;
						}
					}
				}
				if( _Lock == false )
				{
					// 射线
					Ray ray = _Camera.ScreenPointToRay( Input.mousePosition );
					RaycastHit[] hits = Physics.RaycastAll( ray );
					for( int i = 0; i < hits.Length; i++ )
					{
						if( hits[i].collider != null )
						{
							if( hits[i].collider.tag == "MapDragger" )
								_OldHit = hits[i].point;
							else
							{
								OnPress( hits[i].collider.transform );
								_Draging = hits[i].collider.GetComponent<Camera3dDragable>();
							}
						}
					}
				}
			}
			else if( Input.GetMouseButton(0) )
			{
				// 处理拖动
				if( _OldHit != Vector3.zero )
				{
					// 射线
					Ray ray = _Camera.ScreenPointToRay( Input.mousePosition );
					RaycastHit[] hits = Physics.RaycastAll( ray );
					for( int i = 0; i < hits.Length; i++ )
					{
						if( hits[i].collider != null )
						{
							if( _Draging == null && hits[i].collider.tag == "MapDragger" && hits[i].point != _OldHit )
							{
								_Momentum = hits[i].point - _OldHit;
								_Moved = true;
							}
							else if( _Draging != null )
								OnDrag( _Draging.transform, Input.mousePosition, hits[i].transform );
						}
					}
				}				
			}
			
			// 缩放
			Zoom( Input.GetAxis("Mouse ScrollWheel") * 200f );
		}
#else
		// 触摸版
		if( Input.touchCount == 0 )
		{
			// 缩放回弹
			if( _Camera.fieldOfView < zoomInLimit )
				Zoom( 0.1f, _LastFocus );
			if( _Camera.fieldOfView > zoomOutLimit )
				Zoom( -0.1f, _LastFocus );

			_Lock = false;
			_Draging = null;
		}
		// 锁死则不检测触摸
		else if( _Lock == false )
		{
			// 单点触摸
			if( Input.touchCount == 1 )
			{				
				// 缩放回弹
				if( _Camera.fieldOfView < zoomInLimit )
					_Camera.fieldOfView += 0.1f;
				if( _Camera.fieldOfView > zoomOutLimit )
					_Camera.fieldOfView -= 0.1f;

				// 如果点到UI上了直接锁死
				if ( Input.GetTouch(0).phase == TouchPhase.Began )
				{
					// 2D UI碰撞
					PointerEventData eventDataCurrentPosition = new PointerEventData( EventSystem.current );
					eventDataCurrentPosition.position = new Vector2( Input.GetTouch(0).position.x, Input.GetTouch(0).position.y );
					
					List<RaycastResult> results = new List<RaycastResult>();
					EventSystem.current.RaycastAll( eventDataCurrentPosition, results );
					
					if( results.Count > 0 )
					{
						for( int i = 0; i < results.Count; i++ )
						{
							if( results[i].gameObject.tag == "UI" )
							{
								_Lock = true;
								break;
							}
						}
					}
					if( _Lock == false )
					{
						// 射线
						Ray ray = _Camera.ScreenPointToRay( Input.GetTouch(0).position );
						RaycastHit[] hits = Physics.RaycastAll( ray );
						for( int i = 0; i < hits.Length; i++ )
						{
							if( hits[i].collider != null )
							{
								if( hits[i].collider.tag == "MapDragger" )
									_OldHit = hits[i].point;
								else
								{
									OnPress( hits[i].collider.transform );
									_Draging = hits[i].collider.GetComponent<Camera3dDragable>();
								}
							}
						}
					}
				}
				// 移动
				else if( Input.GetTouch(0).phase == TouchPhase.Moved )
				{
					if( _OldHit != Vector3.zero )
					{
						// 射线
						Ray ray = _Camera.ScreenPointToRay( Input.GetTouch(0).position );
						RaycastHit[] hits = Physics.RaycastAll( ray );
						for( int i = 0; i < hits.Length; i++ )
						{
							if( hits[i].collider != null )
							{
								if( _Draging == null && hits[i].collider.tag == "MapDragger" && hits[i].point != _OldHit )
								{
									_Momentum = hits[i].point - _OldHit;
									_Moved = true;
									break;
								}
								else if( _Draging != null )
									OnDrag( _Draging.transform, Input.mousePosition, hits[i].transform );
							}
						}
					}
				}			
				// 释放清空
				else if( Input.GetTouch(0).phase == TouchPhase.Ended )
				{					
					// 鼠标抬起时再进行事件检测，如果发生移动则Pass
					if( _Moved == false )
					{
						Ray ray = _Camera.ScreenPointToRay( Input.GetTouch(0).position );
						RaycastHit[] hits = Physics.RaycastAll( ray );
						bool isHit = false;
						for( int i = 0; i < hits.Length; i++ )
						{
							if( hits[i].collider != null )
							{
								if( hits[i].collider.tag == "MapDragger" )
									OnClick( null );
								else
								{
									OnClick( hits[i].collider.transform );
									isHit = true;
								}
							}
						}
						if( isHit == false )
							OnClick( null );
					}
					
					_Moved = false;
					
					_TargetPos = Vector2.zero;
					_OldHit = Vector3.zero;
					_OldPosition1 = Vector2.zero;
					_OldPosition2 = Vector2.zero;
				}
			}
			// 多点触摸
			else if( Input.touchCount == 2 )
			{			
				// 初值
				if( _OldPosition1 == Vector2.zero )
				{
					_OldHit = Vector3.zero;
					_OldPosition1 = Input.GetTouch(0).position;
				}
				if( _OldPosition2 == Vector2.zero )
				{
					_OldPosition2 = Input.GetTouch(1).position;
				}
				
				// 两只手指移动 - 缩放
				if( Input.GetTouch(0).phase == TouchPhase.Moved || Input.GetTouch(1).phase == TouchPhase.Moved )
				{
					// 当前触摸点位置
					Vector2 tempPosition1 = Input.GetTouch(0).position;
					Vector2 tempPosition2 = Input.GetTouch(1).position;
					
					if( _TargetPos == Vector2.zero )
					{
						_TargetPos = ( tempPosition1 + tempPosition2 ) / 2;
					}
					
					// 两点触摸之间的距离
					float leng1 = Vector3.Distance( _OldPosition1, _OldPosition2 );
					float leng2 = Vector3.Distance( tempPosition1, tempPosition2 );
					// 移动缩放
					Zoom( leng2 - leng1, _TargetPos );
					
					// 上一次触摸点的位置，用于对比
					_OldPosition1 = tempPosition1;
					_OldPosition2 = tempPosition2;
				}
				
				// 释放清空
				if( Input.GetTouch(0).phase == TouchPhase.Ended )
				{
					_OldPosition1 = Vector2.zero;
				}
				if( Input.GetTouch(1).phase == TouchPhase.Ended )
				{
					// 射线
					Ray ray = _Camera.ScreenPointToRay( Input.GetTouch(0).position );
					RaycastHit[] hits = Physics.RaycastAll( ray );
					for( int i = 0; i < hits.Length; i++ )
					{
						if( hits[i].collider != null )
						{
							if( hits[i].collider.tag == "MapDragger" )
								_OldHit = hits[i].point;
						}
					}
					_OldPosition2 = Vector2.zero;
				}
			}
			// 超过两点时调整
			else if( Input.touchCount > 2 )
			{
				for( int i = 0; i < Input.touchCount; i++ )
				{
					if( Input.GetTouch(i).phase == TouchPhase.Ended )
					{
						_OldPosition1 = Vector2.zero;
						_OldPosition2 = Vector2.zero;
						break;
					}
				}
			}
		}
#endif
		
		// 移动动量
		if( _Momentum != Vector3.zero )
		{
			Move( _Momentum.x, _Momentum.z );
			
			_Momentum -= _Momentum * moveDecay;
			if( _Momentum.magnitude < 0.1f )
			{
				_Momentum = Vector3.zero;
			}
		}
	}

	// 计算可拖动范围
	void ResetCameraLimit()
	{
		// 分别从屏幕两个对角(左下，右上)发射射线。
		Vector3[] corner = { new Vector3( 0, 0, 0 ), new Vector3( 1, 1, 0 ) };
		Vector3[] points = new Vector3[2];
		for( int i = 0; i < 2; i++ )
		{
			Ray ray = _Camera.ViewportPointToRay( corner[i] );
			RaycastHit[] hits = Physics.RaycastAll( ray );
			bool isHit = false;
			for( int j = 0; j < hits.Length; j++ )
			{
				if( hits[j].collider != null )
				{
					if( hits[j].collider.tag == "MapDragger" )
					{
						points[i] = hits[j].point;
						isHit = true;
						break;
					}
				}
			}
		}

		// 左
		_CameraLimit.x = _Camera.transform.localPosition.x - ( points[0].x - mapLimit.x );
		// 上
		_CameraLimit.y = _Camera.transform.localPosition.z + ( mapLimit.y - points[1].z );
		// 右
		_CameraLimit.z = _Camera.transform.localPosition.x + ( mapLimit.z - points[1].x );
		// 下
		_CameraLimit.w = _Camera.transform.localPosition.z - ( points[0].z - mapLimit.w );
	}

	// 只用于PC
	void Zoom( float length )
	{
		Zoom( length, Input.mousePosition );
	}
	
	// 焦点缩放
	void Zoom( float length, Vector3 focus )
	{
		if( length == 0 )
			return;

		_LastFocus = focus;
		
		length = -length;
		
		// 缩放前焦点位置
		Vector3 org = Vector3.zero;
		Ray ray = _Camera.ScreenPointToRay( focus );
		RaycastHit[] hits = Physics.RaycastAll( ray );
		for( int i = 0; i < hits.Length; i++ )
		{
			if( hits[i].collider != null )
			{
				if( hits[i].collider.tag == "MapDragger"  )
				{
					org = hits[i].point;
					break;
				}
			}
		}
		
		// 缩放
		_Camera.fieldOfView += zoomSpeed * length;
		if( _Camera.fieldOfView < zoomInLimit - zoomRestrict )
			_Camera.fieldOfView = zoomInLimit - zoomRestrict;
		if( _Camera.fieldOfView > zoomOutLimit + zoomRestrict )
			_Camera.fieldOfView = zoomOutLimit + zoomRestrict;
		
		// 对齐焦点
		Vector3 nw = Vector3.zero;
		ray = _Camera.ScreenPointToRay( focus );
		hits = Physics.RaycastAll( ray );
		for( int i = 0; i < hits.Length; i++ )
		{
			if( hits[i].collider != null )
			{
				if( hits[i].collider.tag == "MapDragger"  )
				{
					nw = hits[i].point;
					break;
				}
			}
		}
		Vector3 offset = nw - org;

		// UI
		if( uiCamera != null )
			uiCamera.fieldOfView = _Camera.fieldOfView;

		// 重新计算可拖动范围
		ResetCameraLimit();

		Move( offset.x, offset.z );
	}

	// 移动
	protected virtual void Move( float offsetX, float offsetZ )
	{
		// 反转
		offsetX = -offsetX;
		offsetZ = -offsetZ;

		//移动
		Vector3 oldPos = _Camera.transform.position;
		Vector3 pos = _Camera.transform.position;
		pos.x += offsetX;
		pos.z += offsetZ;

		if( pos.x <  _CameraLimit.x )
			pos.x = _CameraLimit.x;
		else if( pos.x > _CameraLimit.z )
			pos.x = _CameraLimit.z;
		
		if( pos.z < _CameraLimit.w )
			pos.z = _CameraLimit.w;
		else if( pos.z > _CameraLimit.y )
			pos.z = _CameraLimit.y;
		
		_Camera.transform.position = pos;
		
		if( uiCamera != null )
			uiCamera.transform.position = pos;
	}
	
	IEnumerator TweenPos( Vector3 pos, float duration )
	{
		Vector2 speed = ( pos - _Camera.transform.position ) / duration;
		while( pos != _Camera.transform.position )
		{
			Vector2 distance = speed * Time.deltaTime;
			Move( distance.x, distance.y );
			yield return null;
		}
	}
	
	IEnumerator TweenSize( float size, float duration )
	{
		float speed = ( size - _Camera.orthographicSize ) / duration;
		while( size != _Camera.orthographicSize )
		{
			_Camera.orthographicSize += speed * Time.deltaTime;
			yield return null;
		}
	}
	
	public void TweenPosTo( Vector3 pos, float duration )
	{
		StartCoroutine( TweenPos( pos, duration ) );
	}
	
	public void TweenSizeTo( float size, float duration )
	{
		StartCoroutine( TweenSize( size, duration ) );
	}

	protected abstract void OnClick( Transform obj );
	protected abstract void OnPress( Transform obj );
	protected abstract void OnDrag( Transform obj, Vector3 pos, Transform hit );
}
