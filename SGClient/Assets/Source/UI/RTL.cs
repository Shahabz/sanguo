namespace RTLService
{
    using System;
    using System.Collections;
    using System.Collections.Generic;
    using System.Globalization;
    using System.Text;
    using UnityEngine;

    public class RTL
    {
        // 计算换行
        public static string ConvertWordWrap( string originalText, float rtlWidth, UIText label = null )
        {
            try
            {
                if ( originalText  == "")
                {
                    return "";
                }

                if ( rtlWidth < 4f )
                {
                    return originalText;
                }


                string[] strArray = SplitIntoLine(originalText);
                StringBuilder builder = new StringBuilder();
                string[] strArray2 = {};
                for ( int i = 0; i < strArray.Length; i++ )
                {
                    if ( label == null )
                    {
                        strArray2 = new string[] { strArray[i] };
                    }
                    else
                    {
                        strArray2 = BreakAtWordWrap( strArray[i], label, rtlWidth );
                    }
                    // 交换下行
                    for( int j = strArray2.Length - 1; j >= 0; j-- )
                    {
                        builder.AppendLine( strArray2[j] );
                    }
                }

                // 
                string str = builder.ToString();
                if ( !originalText.EndsWith( "\n" ) && str.EndsWith( "\n" ) )
                {
                    str = str.Substring( 0, str.Length - 1 );
                }

                // 把符号反一下
                char[] ch = str.ToCharArray();
                for( int i = 0; i < ch.Length; i ++  )
                {
                    ch[i] = ReverseBlockSign( ch[i] );
                }
                str = new string( ch );

                // 冒号左右翻转
                // str = ReverseByColon( str );

                return str;
            }
            catch
            {
                return "";
            }
        }

        // 先行分行
        private static string[] SplitIntoLine(string input)
        {
            return input.Split(new string[] { "\r\n", "\n" }, StringSplitOptions.None);
        }
        
        // 按照宽度，分行
        private static string[] BreakAtWordWrap( string aText, UIText label, float width )
        {
            List<int> list = new List<int>();
            string text = aText;
            float fontSize = label.font.fontSize;
            float fnalLineHeight = label.preferredHeight;
            int finalSize = label.fontSize;
            
            int lastIndex = text.Length - 1;
            float totalWidth = 0;
            // 从后往前断行
            for ( int i = lastIndex; i >= 0; i-- )
            {
                // 获取每个字符的宽度
                totalWidth += GetCharWidth( text[i], label.font, finalSize, label.fontStyle ) + 0.1f;//GetTextLength( text[j].ToString(), label, width, finalSpacingX, fnalLineHeight, label.font, finalSize, label.fontStyle );
                // 如果宽度超过了最大行宽，则向后找一个空格处，并换行
                if( totalWidth > width )
                {
                    int index = i;
                    while ( index <= lastIndex )
                    {
                        if ( text[index].Equals( ' ' ) )
                        {
                            list.Add( index );
                            break;
                        }
                        index++;
                    }

                    if ( index > lastIndex )
                    {
                        list.Add( i );
                        lastIndex = i;
                    }
                    else
                    {
                        lastIndex = index - 1;
                        i = index - 1;
                    }

                    totalWidth = 0;
                }
            }

            // 翻一下
            list.Reverse();
            //Debug.Log("finalSpacingX:" + finalSpacingX + ",fnalLineHeight:" + fnalLineHeight + ",finalSize:" + finalSize + ",fontScale:" + fontScale + ",pixelDensity:" + pixelDensity + "list" + list.Count);
            return GetStrArray( aText, list );
        }

        // 获取字符大小
        static float GetCharWidth( char ch, Font dynamicFont, int finalSize, FontStyle fontStyle )
        {  
		    if ( dynamicFont != null )
		    {
			    dynamicFont.RequestCharactersInTexture( ch.ToString(), finalSize, fontStyle );

                CharacterInfo info;
                if( dynamicFont.GetCharacterInfo( ch, out info, finalSize, fontStyle ) )
				    return info.advance;
            }
            return 0f;
        }
        
        // 根据断点，拆分字符串
        private static string[] GetStrArray(string aText, List<int> list)
        {
            if ( list == null || list.Count == 0 )
            {
                return new string[] { aText };
            }

            string[] strArray = new string[ list.Count + 1 ];
            strArray[0] = aText.Substring( 0, list[0] );//.Replace( "\n", "" );
            for( int j = 0; j < list.Count; j++ )
            {
                if ( j < ( list.Count - 1 ) )
                {
                    strArray[ j + 1 ] = aText.Substring( list[j], list[ j + 1 ] - list[j] );//Replace("\n", "");
                }
                else
                {
                    strArray[ j + 1 ] = aText.Substring( list[j] );//.Replace("\n", "");
                }
            }
            return strArray;
        }

        // 冒号翻转
        public static string ReverseBySign( string str, char sign )
        {
            string[] sub = str.Split( sign );
            if ( sub.Length == 0 )
                return str;
            
            StringBuilder rlz = new StringBuilder();
            if ( sub.Length == 1 )
            {
                if ( str[0] == sign )
                {
                    rlz.Append( sub[0] );
                    rlz.Append( sign );
                }
                else
                {
                    rlz.Append( sign );
                    rlz.Append( sub[0] );
                }
            }
            else
            {
                for( int i = sub.Length - 1; i >= 0; i-- )
                {
                    rlz.Append( sub[i] );
                    if( i != 0 )
                        rlz.Append( sign );
                }
            }
            
            return rlz.ToString();
        }
        
        // 符号反向
        private static char ReverseBlockSign( char ch )
        {
            switch ( ch )
            {
                case '(':
                    ch = ')';
                    return ch;

                case ')':
                    ch = '(';
                    return ch;

                case '<':
                    ch = '>';
                    return ch;

                case '>':
                    ch = '<';
                    return ch;

                case '[':
                    ch = ']';
                    return ch;

                case ']':
                    ch = '[';
                    return ch;
            }
            return ch;
        }
                  
        // 清除富文本
        public static string RemoveRichText( string text )
        {
            int index = 0;
            do
            {
                index = text.IndexOf( '<', index );
                if ( index >= 0 )
                {
                    int index2 = text.IndexOf( '>' );
                    if ( index2 > 0 )
                    {
                        // 是否包含标签
                        string tab = text.Substring( index + 1, index2 - index );
                        if( tab.Contains( "color" ) || tab.Contains( "size" ) )
                        {
                            // 删除内容
                            text = text.Remove( index, index2 - index + 1 );
                        }
                        else
                            index = index2 + 1;
                    }
                    else
                        index = -1;
                }
            }
            while( index != -1 );

            return text;
        }

        //static float GetTextLength( string text, UIText label, float width, float finalSpacingX, float finalLineHeight, Font dynamicFont, int finalSize, FontStyle fontStyle )
        //{
        //    Vector2 CalculatedSize = CalculatePrintedSize( text, width, finalSpacingX, finalLineHeight, dynamicFont, finalSize, fontStyle );
        //    return CalculatedSize.x;
        //}

        //static public Vector2 CalculatePrintedSize( string text, float width, float finalSpacingX, float finalLineHeight, Font dynamicFont, int finalSize, FontStyle fontStyle )
        //{
        //    Vector2 v = Vector2.zero;

        //    if ( !string.IsNullOrEmpty( text ) )
        //    {
        //        // Ensure we have characters to work with
        //        // Prepare(text, dynamicFont, finalSize, fontStyle);

        //        float x = 0f, y = 0f, maxX = 0f;
        //        int textLength = text.Length, ch = 0, prev = 0;

        //        for ( int i = 0; i < textLength; i++ )
        //        {
        //            ch = text[i];

        //            // Start a new line
        //            if ( ch == '\n' )
        //            {
        //                if ( x > maxX ) maxX = x;
        //                x = 0f;
        //                y += finalLineHeight;
        //                continue;
        //            }
        //            // Skip invalid characters
        //            if ( ch < ' ' ) continue;
        //            float w = GetGlyphWidth( text[i], prev, dynamicFont, finalSize, fontStyle );
        //            if ( w != 0f )
        //            {
        //                w += finalSpacingX;

        //                if ( Mathf.RoundToInt( x + w ) > width )
        //                {
        //                    if ( x > maxX ) maxX = x - finalSpacingX;
        //                    y += finalLineHeight;
        //                }
        //                x += w;

        //                prev = ch;
        //            }
                   
        //        }

        //        v.x = (( x > maxX ) ? x - finalSpacingX : maxX );
        //        v.y = ( y + finalLineHeight );
        //    }
        //    return v;
        //} 
    }
}

