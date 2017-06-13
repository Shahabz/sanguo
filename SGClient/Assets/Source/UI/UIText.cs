using UnityEngine;
using System.Collections;
using UnityEngine.UI;
using ArabicSupport;

public class UIText : Text 
{
	public int localizeID = 0;
    public bool rtlAlignment = false;
    public string rtlReverseSign = "";
    public override string text
    { 
        get{ return base.text; }
        set
        {
            if( Localization.currentLanguage == "ar" )
            {
                // 删除富文本
                string str = RTLService.RTL.RemoveRichText( value );
                // 如果是多行，处理多行和翻转
                if( horizontalOverflow == HorizontalWrapMode.Wrap )
                    str = RTLService.RTL.ConvertWordWrap( ArabicFixer.Fix( str, true, false ), rectTransform.rect.width, this );
                else
                    str = ArabicFixer.Fix( str, true, false );
                // 处理符号翻转
                if( rtlReverseSign != "" )
                    str = RTLService.RTL.ReverseBySign( str, rtlReverseSign[0] );

                base.text = str;
            }
            else
                base.text = value;
        }
    }
    protected void SetBase( string str )
    {
        base.text = str;
    }

	protected override void Start ()
	{
		base.Start ();

		if( localizeID != 0 )
		{
			this.text = Localization.text( localizeID );
        }

        if( Localization.currentLanguage == "ar" && rtlAlignment )
        {
            switch( alignment )
            {
                case TextAnchor.UpperLeft:
                    alignment = TextAnchor.UpperRight;
                    break;
                case TextAnchor.MiddleLeft:
                    alignment = TextAnchor.MiddleRight;
                    break;
                case TextAnchor.LowerLeft:
                    alignment = TextAnchor.LowerRight;
                    break;
            }
        }


#if UNITY_EDITOR
        SetDefaultFont();
#endif
    }

    void SetDefaultFont()
    {
        if ( font == null || font.name == "Arial" )//这个地方的name可以改为原来的字体的名称  
        {
            Font mFont = Resources.Load<Font>( "Fonts/arial" );//注意这个地方是要替换成的字体的路径  
            if ( mFont == null )
            {
                Debug.LogError( " Font not found ! " );
                return;
            }
            font = mFont;
        }
    }
}
