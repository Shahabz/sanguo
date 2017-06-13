using System;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.Events;

[RequireComponent( typeof( Text ) )]
public class FXTypeWriter : MonoBehaviour
{
    public enum Mode
    {
        PERWORD,
        ALPHA
    }

    public Mode mode = Mode.PERWORD;
    public bool playOnStart = false;
    public UnityEvent myEvent; 
    public int charsPerSecond = 0;
    public int totalTime = 0;       // 注意不为0时，charsPerSecond失效
    public float delay = 0;

    private bool isActive = false;
    private float timer;
    private string words;
    private Text mText;

    void Awake()
    {
        mText = GetComponent<Text>();
    }

    void Start()
    {
        if ( myEvent == null )
            myEvent = new UnityEvent();

        charsPerSecond = Mathf.Max( 1, charsPerSecond );

        if ( playOnStart )
            Play();
    }

    public void Play()
    {        
        // 如是阿拉伯语，立即显示所有
        if( Localization.currentLanguage == "ar" )
        {
            OnFinish();
            return;
        }

        words = mText.text;
        mText.text = string.Empty;
        isActive = true;
        timer = 0;

        if( totalTime != 0 )
        {
            charsPerSecond = Mathf.CeilToInt( (float)words.Length / totalTime ); 
        }
    }

    void OnWriterPerWord()
    {
        if ( isActive )
        {
            if( timer < delay )
            {
                timer += Time.deltaTime;
                return;
            }

            try
            {
                mText.text = words.Substring( 0, (int)( charsPerSecond * ( timer - delay ) ) );
                timer += Time.deltaTime;
            }
            catch ( Exception )
            {
                OnFinish();
            }
        }
    }

    void OnWriterAlpha()
    {
        if ( isActive )
        {
            if( timer < delay )
            {
                timer += Time.deltaTime;
                mText.text = words.Insert( 0, "<color=#00000000>" ) + "</color>";
                return;
            }

            try
            {
                mText.text = words.Insert( (int)( charsPerSecond * ( timer - delay ) ), "<color=#00000000>" ) + "</color>";
                timer += Time.deltaTime;
            }
            catch ( Exception )
            {
                OnFinish();
            }
        }
    }

    public void OnFinish()
    {
        if( isActive == false )
            return;

        isActive = false;
        mText.text = words;
        try
        {
            myEvent.Invoke();
        }
        catch ( Exception )
        {
            Debug.Log( "Error!" );
        }
    }

    void Update()
    {
        if( mode == Mode.PERWORD )
            OnWriterPerWord();
        else if( mode == Mode.ALPHA )
            OnWriterAlpha();
    }
}
