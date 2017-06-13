using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using System.Collections.Generic;
using System.Text;

public class UITextUnderline : UIText
{
    bool _ShowLine = true;
    public bool showLine
    {
        get
        {
            return _ShowLine;
        }
        set
        {
            if( value )
                RefreshLine();

            _ShowLine = value;
            if( _UnderLine != null )
                _UnderLine.gameObject.SetActive( value );
        }
    }

    public override string text
    {
        get
        {
            return base.text;
        }
        set
        {
            base.text = value;
            RefreshLine();
        }
    }
    
    UITextUnderline         _UnderLine;
    float                   _Width;
    bool                    _Clone = false;

    protected override void Start()
    {
        base.Start();
        RefreshLine();
    }

    void CreateLine()
    {
        if( _Clone || _UnderLine != null )
            return;
        //克隆Text，获得相同的属性
        GameObject cloneObj = new GameObject("Underline");
        cloneObj.AddComponent<RectTransform>();
        _UnderLine = cloneObj.AddComponent<UITextUnderline>();
        //_UnderLine = Instantiate( this ) as UITextUnderline;

        //_UnderLine.name = "Underline";
        _UnderLine._Clone = true;
        _UnderLine.font = this.font;
        _UnderLine.fontSize = this.fontSize;
        _UnderLine.fontStyle = this.fontStyle;
        _UnderLine.lineSpacing = this.lineSpacing;
        _UnderLine.alignment = this.alignment;

        cloneObj.transform.SetParent(transform);
        RectTransform rt = cloneObj.GetComponent < RectTransform >();
  
        //设置下划线坐标和位置
        rt.localScale = Vector3.one;
        rt.anchoredPosition3D = Vector3.zero;  
        rt.offsetMax = Vector2.zero;  
        rt.offsetMin = Vector2.zero;  
        rt.anchorMax = Vector2.one;  
        rt.anchorMin = Vector2.zero;

        _UnderLine.SetBase( "_" );
        _Width = _UnderLine.preferredWidth;      //单个下划线宽度
        
        if( !_ShowLine )
            _UnderLine.gameObject.SetActive( false );
    }

    //public override void SetVerticesDirty()
    //{
    //    base.SetVerticesDirty();
    //    if( _Clone )
    //        return;
        
    //    if( IsActive() && showLine )
    //       RefreshLine();
    //}

    void RefreshLine()
    {
        if ( Application.isPlaying == false )
            return;
        CreateLine();
        if( _UnderLine == null )
            return;

        supportRichText = false;

        Font font = this.font;
        
        _UnderLine.color = this.color;

		font.RequestCharactersInTexture( text, this.fontSize, FontStyle.Normal );
		CharacterInfo characterInfo;
		float width = 0f;
		for( int i = 0; i < text.Length; i++ )
        {			
			font.GetCharacterInfo( text[i], out characterInfo, this.fontSize );
			//width+=characterInfo.width; unity5.x提示此方法将来要废弃
			width += characterInfo.advance;
		}

        int lineCount = Mathf.CeilToInt( width / _Width ); 
        _UnderLine.SetBase( "" );
        StringBuilder line = new StringBuilder(); ;
        for(int i = 0;i < lineCount - 1;i++)  
        {
            line.Append("_");
            
        }
        _UnderLine.SetBase(line.ToString());
    }
}
