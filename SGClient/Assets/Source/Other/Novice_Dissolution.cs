using UnityEngine;
using System.Collections;
using UnityEngine.UI;

public class Novice_Dissolution : MonoBehaviour
{	
	public Image    image;
	public float	value;	
	float _LastValue = 0;	
	
	void Update()
	{
		if ( _LastValue != value )
		{
			_LastValue = value;
			image.material.SetFloat ( "_node_9766", value );
		}
	}
}
