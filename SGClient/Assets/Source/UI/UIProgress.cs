using UnityEngine;
using UnityEngine.UI;
using System.Collections;

public class MyRangeAttribute : PropertyAttribute
{
    // 这3个变量是在Inspector面板中显示的  
    public float min;    // 定义一个float类型的最大  
    public float max;    // 定义一个float类型的最大  
    public string label; // 显示标签  
    public MyRangeAttribute( float min, float max, string label = "" )
    {
        this.min = min;
        this.max = max;
        this.label = label;
    }

}

public class UIProgress : MonoBehaviour 
{
	public enum ProgressMode
	{
		Scale,
		Fill,
        ExpandHorizon,
        Expandvertical
	}

    Vector2                 _OriginValue = Vector2.zero;
	public Image			progress;
	public Image			fullProgress;
	public ProgressMode		mode = ProgressMode.Scale;
	private	float 			_timeCounter = 0;
	private float 			_lastvalue = 0.0f;
	private float 			_beginvalue = 0.0f;
	private float 			_endvalue = 0.0f;
	private short			_playcount = 1;
	private float 			_speed = 1.0f;
	private bool			_playing = false;

	public delegate void LuaCallback( object param );
	public LuaCallback callback = null;

	// Use this for initialization
	void Start () 
	{

	}

    public void SetValue( float value )
	{
        if ( _OriginValue == Vector2.zero )
            _OriginValue = progress.rectTransform.sizeDelta;


        if( float.IsNaN( value ) )
            value = 0f;

		if (value > 1.0)
		{
			value = 1.0f;
		} 
		else if (value < 0.0) 
		{
			value = 0.0f;
		} 

		if (fullProgress != null) 
		{
			if (value >= 0.99999f) {
				fullProgress.gameObject.SetActive (true);
				progress.gameObject.SetActive (false);
			} else {
				fullProgress.gameObject.SetActive (false);
				progress.gameObject.SetActive (true);
			}
		}
		_lastvalue = value;

		switch( mode )
		{
		case ProgressMode.Scale:
			Vector3 scale = progress.transform.localScale;
			scale.x = value;
			progress.transform.localScale = scale;
			if ( fullProgress != null )
				fullProgress.transform.localScale = scale;
			break;
		case ProgressMode.Fill:
			progress.fillAmount = value;
			if ( fullProgress != null )
				fullProgress.fillAmount = value;
			break;
        case ProgressMode.ExpandHorizon:
            progress.rectTransform.sizeDelta = new Vector2( _OriginValue.x * value, _OriginValue.y );
			if ( fullProgress != null )
				fullProgress.rectTransform.sizeDelta = new Vector2( _OriginValue.x * value, _OriginValue.y );
            break;
        case ProgressMode.Expandvertical:
            progress.rectTransform.sizeDelta = new Vector2( _OriginValue.x, _OriginValue.y * value );
			if ( fullProgress != null )
				fullProgress.rectTransform.sizeDelta = new Vector2( _OriginValue.x, _OriginValue.y * value );
            break;
		}
	}

	/// <summary>
	/// 播放进度过程
	/// beginvalue 开始值
	/// endvalue 结束值
	/// speed 速度
	/// count 经过几个完整进度
	/// </summary>
	public void Play( float beginvalue, float endvalue, float speed, short playcount = 1 )
	{
		SetValue (beginvalue);
		_beginvalue = beginvalue;
		_endvalue = endvalue;
		_speed = speed;
		_playcount = playcount;
		_playing = true;
	}

	public void Stop()
	{
		_beginvalue = 0;
		_endvalue = 0;
		_speed = 0;
		_playcount = 0;
		_playing = false;
		callback = null;
	}

	/// <summary>
	/// 结束经验条播放动画
	/// </summary>
	public void OverPlay()
	{
		SetValue( _endvalue );
		if ( callback != null )
			callback (null);
	}

	public void Update()
	{
		if (_playing) {
			if (_playcount > 0) {
				if (_playcount == 1) {
					if (_lastvalue < _endvalue) {
						SetValue (_lastvalue + Time.deltaTime*_speed);
					} else {
						_playcount--;
					}
				} else {
					if (_lastvalue < 1) {
						SetValue (_lastvalue + Time.deltaTime*_speed);
					} else {
						_playcount--;
						SetValue (0);
						if ( callback != null )
							callback (null);
					}
				}
			} else { 
				_playing = false;
					OverPlay (); 
			}
		}
	}
}
