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
	public ProgressMode		mode = ProgressMode.Scale;

    public void SetValue( float value )
	{
        if ( _OriginValue == Vector2.zero )
            _OriginValue = progress.rectTransform.sizeDelta;


        if( float.IsNaN( value ) )
            value = 0f;

        if ( value > 1.0 )
            value = 1.0f;
        else if ( value < 0.0 )
            value = 0.0f;

		switch( mode )
		{
		case ProgressMode.Scale:
			Vector3 scale = progress.transform.localScale;
			scale.x = value;
			progress.transform.localScale = scale;
			break;
		case ProgressMode.Fill:
			progress.fillAmount = value;
			break;
        case ProgressMode.ExpandHorizon:
            progress.rectTransform.sizeDelta = new Vector2( _OriginValue.x * value, _OriginValue.y );
            break;
        case ProgressMode.Expandvertical:
            progress.rectTransform.sizeDelta = new Vector2( _OriginValue.x, _OriginValue.y * value );
            break;
		}
	}
}
