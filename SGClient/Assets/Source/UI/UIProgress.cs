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
}
