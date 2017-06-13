using UnityEngine;
using System.Collections;

[RequireComponent(typeof(UIText))]
public class Subtitle : MonoBehaviour
{
    public int[]    localizedID;
    public float[]  timeStamp;
    public string[] text;

    FXTypeWriter      _TypeWriter;
    UIText          _Text;
    float[]         _TimeDelay;

    void Awake()
    {
        _TypeWriter = GetComponent<FXTypeWriter>();
        _Text = GetComponent<UIText>();
    }

    void Start()
    {
        _TimeDelay = new float[ timeStamp.Length ];
        _TimeDelay[0] = timeStamp[0];
        for( int i = 1; i < _TimeDelay.Length; i++ )
        {
            _TimeDelay[i] = timeStamp[i] - timeStamp[ i - 1 ];
        }

        if( localizedID.Length != 0 )
        {
            StartCoroutine( Play() );
        }
        else
            return;
    }

    IEnumerator Play()
    {
        int index = 0;
        while( index < localizedID.Length )
        {
            yield return new WaitForSeconds( _TimeDelay[index] );

            if( index < text.Length )
                _Text.text = text[index];
            else
                _Text.text = Localization.text( localizedID[index] )[0].ToString();
            _TypeWriter.Play();
            index++;
        }
    }
}
