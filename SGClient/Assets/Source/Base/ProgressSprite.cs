using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ProgressSprite : MonoBehaviour {

    public enum ProgressMode
    {
        Horizon,
        vertical
    }

    Vector2 _spriteSize = Vector2.zero;
    public GameObject _progress;
    SpriteRenderer _spriteRenderer;
    public ProgressMode _mode = ProgressMode.Horizon;

    void Awake()
    {
        _spriteRenderer = _progress.GetComponent<SpriteRenderer>();
        _spriteSize = _spriteRenderer.size;
    }

    public void SetValue( float value )
    {
        if ( float.IsNaN( value ) )
            value = 0f;

        if ( value > 1.0 )
            value = 1.0f;
        else if ( value < 0.0 )
            value = 0.0f;

        switch ( _mode )
        {
            case ProgressMode.Horizon:
                _spriteRenderer.size = new Vector2( _spriteSize.x * value, _spriteSize.y );
                break;
            case ProgressMode.vertical:
                _spriteRenderer.size = new Vector2( _spriteSize.x, _spriteSize.y * value );
                break;
        }
    }
}
