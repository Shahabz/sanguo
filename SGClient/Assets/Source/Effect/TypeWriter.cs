using System;
using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using UnityEngine.UI;
using UnityEngine.Events;

[AddComponentMenu( "UI/Effects/TypeWriter" )]
[RequireComponent(typeof(UIText))]
public class TypeWriter : BaseMeshEffect
{
    public bool playOnStart = false;
    public int charsPerSecond = 0;
    public float delay = 0;
    public float speedRatio = 1;
    public UnityEvent myEvent; 
    //public LuaFunction onFinish;

    VertexHelper    _Mesh;
    private float   _Timer;
    private bool    _Active = false;
    private bool    _Playing = false;
    private int     _LastWordCount = 0;
    private List<int> _Vtx;

    private bool    _Rebuild = false;

    void Start()
    {
        if ( myEvent == null )
            myEvent = new UnityEvent();

        if( playOnStart )
            Play();
    }

    public override void ModifyMesh( VertexHelper mesh )
    {
        if ( !IsActive() || Localization.currentLanguage == "ar" )
        {
            return;
        }

        _Mesh = mesh;

        if( _Rebuild || _Vtx == null )
        {
            _Vtx = new List<int>();
            for( int i = 0; i < _Mesh.currentVertCount; i++ )
            {
                bool ok = true;
                UIVertex uv1 = new UIVertex();
                _Mesh.PopulateUIVertex( ref uv1, i );
                if( i < _Mesh.currentVertCount - 1 )
                {
                    UIVertex uv2 = new UIVertex();
                    _Mesh.PopulateUIVertex( ref uv2, i+1 );

                    if( uv1.position == uv2.position )
                    {
                        ok = false;
                    }
                }
                if ( ok && i > 0 )
                {
                    UIVertex uv2 = new UIVertex();
                    _Mesh.PopulateUIVertex( ref uv2, i-1 );

                    if( uv1.position == uv2.position )
                    {
                        ok = false;
                    }
                }

                if( ok )
                    _Vtx.Add( i );
            }
        }

        int index = 0;
        for( int i = 0; i < _Vtx.Count; i++ )
        {            
            UIVertex uv = new UIVertex();
            _Mesh.PopulateUIVertex( ref uv, _Vtx[i] );

            Color32 color = uv.color;

            if( index < (int)( charsPerSecond * ( _Timer - delay ) ) * 4 )
            {
                color.a = 255;
            }
            else
            {
                color.a = 0;
            }

            uv.color = color;

            _Mesh.SetUIVertex( uv, _Vtx[i] );

            index++;
        }
    }

    public void Play()
    {
        _Timer = 0;
        _Active = true;
        _LastWordCount = 0;

        _Rebuild = true;

        // 如是阿拉伯语，立即显示所有
        if( Localization.currentLanguage == "ar" )
        {
            _Vtx = new List<int>();
            OnFinish();
        }
    }

    void Update()
    {
        if( _Mesh == null || _Active == false )
            return;

        if( _Timer < delay )
        {
            _Timer += Time.deltaTime * speedRatio;
            return;
        }
        else            
            _Timer += Time.deltaTime * speedRatio;

        if( (int)( charsPerSecond * ( _Timer - delay ) ) > _LastWordCount )
        {
            _Playing = true;
            _LastWordCount = (int)( charsPerSecond * ( _Timer - delay ) );

            gameObject.SetActive( false );
            gameObject.SetActive( true );

            if( (int)( charsPerSecond * ( _Timer - delay ) ) * 4 >= _Vtx.Count )
            {
                OnFinish();
            }
        }
    }
    public void OnFinish()
    {
        if( _Active == false )
            return;
        
        _Playing = false;
        _Active = false;

        if( _Vtx == null )
            return;
        
        _Timer = _Vtx.Count / 4 / charsPerSecond + delay + 1;

        gameObject.SetActive( false );
        gameObject.SetActive( true );

        //if( onFinish != null )
        //    onFinish.Execute();

        try
        {
            myEvent.Invoke();
        }
        catch ( Exception )
        {
            Debug.Log( "Error!" );
        }
    }
}