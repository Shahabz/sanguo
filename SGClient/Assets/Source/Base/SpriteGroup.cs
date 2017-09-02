using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class SpriteGroup : MonoBehaviour
{
	public float alpha = 1f;
	SpriteRenderer[]	_Sprites;
	float				_OldAlpha = 1f;

	void Awake()
	{
		_Sprites = GetComponentsInChildren<SpriteRenderer>();
	}

	void LateUpdate()
	{
		if( _OldAlpha != alpha )
		{
			_OldAlpha = alpha;
			for( int i = 0; i < _Sprites.Length; i++ )
			{
				Color color = _Sprites[i].color;
				color.a = alpha;
				_Sprites[i].color = color;
			}
		}
	}
}
