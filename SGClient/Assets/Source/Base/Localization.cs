using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using LuaInterface;
using System.Reflection;
using System.IO;
using System.Text;
using System;

// 本地化功能文件
public static class Localization
{
    // 当前选择国家
    public static string currentCountry = "";

    // 当前使用语言
    public static string currentLanguage = "";

    // 国家信息英文编码作key
    public static Dictionary<string, string[]> countryCodeTable;

    // 国家信息数字编码作key
    public static Dictionary<int, string[]> countryIdTable;

    // 语言信息英文编码作key
    public static Dictionary<string, string[]> languageCodeTable;

    // 语言信息数字编码作key
    public static Dictionary<int, string[]> languageIdTable;

    // 基础文字表
    static Dictionary<int, string> localize;

    // 道具文字表
    static Dictionary<int, string> localize_item;

    // 任务文字表
    static Dictionary<int, string> localize_quest;

    // 基础文字表-没找到对应文字，则使用英文的
    static Dictionary<int, string> localize_en_us;

    // 道具文字表-没找到对应文字，则使用英文的
    static Dictionary<int, string> localize_item_en_us;

    // 任务文字表-没找到对应文字，则使用英文的
    static Dictionary<int, string> localize_quest_en_us;

    public static bool bInited = false;

    // 根据选择语言进行初始化
    public static int init()
    {
        destroy();

        countryCodeTable = new Dictionary<string, string[]>();
        countryIdTable = new Dictionary<int, string[]>();
        languageCodeTable = new Dictionary<string, string[]>();
        languageIdTable = new Dictionary<int, string[]>();

        // 读取国家信息
        TableUtil _countryTxt = new TableUtil();
        if ( _countryTxt.OpenFromData( "localization_country.txt" ) == false )
            return -1;
        for ( int row = 0; row < _countryTxt.GetRecordsNum(); row++ )
        {
            string countryCode      = _countryTxt.GetValue( row, 0 );  //  国家英文代码（国际标准）
            string CountryID        = _countryTxt.GetValue( row, 1 );  //  国家数字代码（国际标准）
            string CountryNameID    = _countryTxt.GetValue( row, 2 );  //  国家名称ID
            string CountryFlag      = _countryTxt.GetValue( row, 3 );  //  国旗资源名
            string CountryMoney     = _countryTxt.GetValue( row, 4 );  //  货币简称
    
            // 英文编码作key
            if ( countryCodeTable.ContainsKey( countryCode ) == false )
            {
                string[] ls = new string[4];
                ls[0] = CountryID;
                ls[1] = CountryNameID;
                ls[2] = CountryFlag;
                ls[3] = CountryMoney;
                countryCodeTable.Add( countryCode, ls );
            }
            // 数字编码作key
            if ( countryIdTable.ContainsKey( Convert.ToInt32(CountryID) ) == false )
            {
                string[] ls = new string[4];
                ls[0] = countryCode;
                ls[1] = CountryNameID;
                ls[2] = CountryFlag;
                ls[3] = CountryMoney;
                countryIdTable.Add( Convert.ToInt32(CountryID), ls );
            }
        }

        // 读取语言信息
        TableUtil _languageTxt = new TableUtil();
        if ( _languageTxt.OpenFromData( "localization_language.txt" ) == false )
            return -2;
        for ( int row = 0; row < _languageTxt.GetRecordsNum(); row++ )
        {
            string LanguageCode         = _languageTxt.GetValue( row, 0 );  //  语言英文代码（国际标准）
            string LanguageCustomID     = _languageTxt.GetValue( row, 1 );  //  语言自定义ID（无标准，暂时跟u3d里定义的语言ID走）
            string LanguageNameID       = _languageTxt.GetValue( row, 2 );  //  语言名称ID
            string LanguageDirectory    = _languageTxt.GetValue( row, 3 );  //  语言存在目录
            string TranslationCode      = _languageTxt.GetValue( row, 4 );  //  翻译Code

            // 英文编码作key
            if ( languageCodeTable.ContainsKey( LanguageCode ) == false )
            {
                string[] ls = new string[4];
                ls[0] = LanguageCustomID;
                ls[1] = LanguageNameID;
                ls[2] = LanguageDirectory;
                ls[3] = TranslationCode;
                languageCodeTable.Add( LanguageCode, ls );
            }
            // 数字编码作key
            if ( languageIdTable.ContainsKey( Convert.ToInt32(LanguageCustomID) ) == false )
            {
                string[] ls = new string[4];
                ls[0] = LanguageCode;
                ls[1] = LanguageNameID;
                ls[2] = LanguageDirectory;
                ls[3] = TranslationCode;
                languageIdTable.Add( Convert.ToInt32( LanguageCustomID ), ls );
            }
        }

        // 用户本地配置文件里获取玩家已经选择的国家语言
        IniUtil userIni = new IniUtil();
        userIni.OpenFromTXT( PathUtil.ConfigPath() + "user.txt" );

        // 读取使用语言
        currentLanguage = userIni.ReadValue( "LANGUAGE", "" ).ToString();
        if ( currentLanguage == "" )
        {
            // 没有存档的，使用设备当前的
            currentLanguage = DeviceHelper.getLanguage();
            userIni.WriteValue("LANGUAGE", currentLanguage );
        }

        // 读取使用国家
        currentCountry = userIni.ReadValue( "COUNTRY", "" ).ToString();
        if ( currentCountry == "" )
        {
            // 没有存档的，使用设备当前的
            currentCountry = DeviceHelper.getCountry();
            userIni.WriteValue( "COUNTRY", currentCountry );
        }

        // 先去语言信息里找一下，语言-地区的目录（如zh-cn），没有则直接使用语言(如zh，ru，en等)，还是没有则使用英文的
        string localizeDir = languageDirectory( currentLanguage + "-" + currentCountry );
        if ( localizeDir == "" )
        {
            localizeDir = languageDirectory( currentLanguage );
            if ( localizeDir == "" )
            {
                localizeDir = languageDirectory( "en" );
                if ( localizeDir == "" )
                {
                    return -3;
                }
            }
        }
        localizeDir = localizeDir +  "/";

        // 读取基础文字表
        TableUtil _localizetxt = new TableUtil();
        localize = new Dictionary<int, string>();
        if ( _localizetxt.OpenFromData( localizeDir + "localize.txt" ) == false )
        {
            // 没有对应的语言文件夹，用英语的
            localizeDir = languageDirectory( "en" ) + "/";
            if ( _localizetxt.OpenFromData( localizeDir + "localize.txt" ) == false )
            {
                return -4;
            }
        }

        for ( int row = 0; row < _localizetxt.GetRecordsNum(); row++ )
        {
            int key = Convert.ToInt32( _localizetxt.GetValue( row, 0 ) );
            string text = _localizetxt.GetValue( row, 1 );
            text = text.Replace( "\\n", "\n" );
            localize.Add( key, text );
        }

        // 读取道具文字表
        TableUtil _localizeItemTxt = new TableUtil();
        localize_item = new Dictionary<int, string>();
        if ( _localizeItemTxt.OpenFromData( localizeDir + "localize_item.txt" ) == false )
        {
            // 没有对应的语言文件夹，用英语的
            localizeDir = languageDirectory( "en" ) + "/";
            if ( _localizeItemTxt.OpenFromData( localizeDir + "localize_item.txt" ) == false )
            {
                return -5;
            }
        }

        for ( int row = 0; row < _localizeItemTxt.GetRecordsNum(); row++ )
        {
            int key = Convert.ToInt32( _localizeItemTxt.GetValue( row, 0 ) );
            string text = _localizeItemTxt.GetValue( row, 1 );
            text = text.Replace( "\\n", "\n" );
            localize_item.Add( key, text );
        }

        // 读取任务文字表
        TableUtil _localizeQuestTxt = new TableUtil();
        localize_quest = new Dictionary<int, string>();
        if ( _localizeQuestTxt.OpenFromData( localizeDir + "localize_quest.txt" ) == false )
        {
            // 没有对应的语言文件夹，用英语的
            localizeDir = languageDirectory( "en" ) + "/";
            if ( _localizeQuestTxt.OpenFromData( localizeDir + "localize_quest.txt" ) == false )
            {
                return -6;
            }
        }

        for ( int row = 0; row < _localizeQuestTxt.GetRecordsNum(); row++ )
        {
            int key = Convert.ToInt32( _localizeQuestTxt.GetValue( row, 0 ) );
            string text = _localizeQuestTxt.GetValue( row, 1 );
            text = text.Replace( "\\n", "\n" );
            localize_quest.Add( key, text );
        }

        // 容错处理，所有找不到的文字使用英文
        localizeDir = languageDirectory( "en" ) + "/";

        // 读取基础文字表
        TableUtil localizetxt_en_us = new TableUtil();
        localize_en_us = new Dictionary<int, string>();
        localizetxt_en_us.OpenFromData( localizeDir + "localize.txt" );
        for ( int row = 0; row < localizetxt_en_us.GetRecordsNum(); row++ )
        {
            int key = Convert.ToInt32( localizetxt_en_us.GetValue( row, 0 ) );
            string text = localizetxt_en_us.GetValue( row, 1 );
            text = text.Replace( "\\n", "\n" );
            localize_en_us.Add( key, text );
        }

        // 读取道具文字表
        TableUtil localizeitemtxt_en_us = new TableUtil();
        localize_item_en_us = new Dictionary<int, string>();
        localizeitemtxt_en_us.OpenFromData( localizeDir + "localize_item.txt" );
        for ( int row = 0; row < localizeitemtxt_en_us.GetRecordsNum(); row++ )
        {
            int key = Convert.ToInt32( localizeitemtxt_en_us.GetValue( row, 0 ) );
            string text = localizeitemtxt_en_us.GetValue( row, 1 );
            text = text.Replace( "\\n", "\n" );
            localize_item_en_us.Add( key, text );
        }

        // 读取任务文字表
        TableUtil localizequesttxt_en_us = new TableUtil();
        localize_quest_en_us = new Dictionary<int, string>();
        localizequesttxt_en_us.OpenFromData( localizeDir + "localize_quest.txt" );
        for ( int row = 0; row < localizequesttxt_en_us.GetRecordsNum(); row++ )
        {
            int key = Convert.ToInt32( localizequesttxt_en_us.GetValue( row, 0 ) );
            string text = localizequesttxt_en_us.GetValue( row, 1 );
            text = text.Replace( "\\n", "\n" );
            localize_quest_en_us.Add( key, text );
        }

        bInited = true;
        return 0;
    }

    public static void destroy()
    {
        bInited = false;
        countryCodeTable = null;
        countryIdTable = null;
        languageCodeTable = null;
        languageIdTable = null;
        localize = null;
        localize_item = null;
        localize_quest = null;
    }

    // 获取国家数字代码
    public static int countryID( string code )
    {
        if ( countryCodeTable.ContainsKey( code ) == false )
        {
            return 0;
        }
        return Convert.ToInt32( countryCodeTable[code][0] );
    }

    // 获取国家英文代码
    public static string countryCode( int id )
    {
        if ( countryIdTable.ContainsKey( id ) == false )
        {
            return "";
        }
        return countryIdTable[id][0];
    }

    // 获取国家名称
    public static string countryName( string code )
    {
        if ( countryCodeTable.ContainsKey( code ) == false )
        {
            return "";
        }
        return text( Convert.ToInt32( countryCodeTable[code][1] ) );
    }

    // 获取国家名称
    public static string countryName( int id )
    {
        if ( countryIdTable.ContainsKey( id ) == false )
        {
            return "";
        }
        return text( Convert.ToInt32( countryIdTable[id][1] ) );
    }

    // 获取国家旗帜
    public static string countryFlag( string code )
    {
        if ( countryCodeTable.ContainsKey( code ) == false )
        {
            return "";
        }
        return  countryCodeTable[code][2];
    }

    // 获取国家旗帜
    public static string countryFlag( int id )
    {
        if ( countryIdTable.ContainsKey( id ) == false )
        {
            return "";
        }
        return countryIdTable[id][2];
    }

    // 获取国家货币简称
    public static string countryMoney( string code )
    {
        if ( countryCodeTable.ContainsKey( code ) == false )
        {
            return "";
        }
        return countryCodeTable[code][3];
    }


    // 获取语言自定义数字编号
    public static int languageCustomID( string code )
    {
        if ( languageCodeTable.ContainsKey( code ) == false )
        {
            return 0;
        }
        return Convert.ToInt32( languageCodeTable[code][0] );
    }

    // 获取语言英文编码
    public static string languageCode( int customID )
    {
        if ( languageIdTable.ContainsKey( customID ) == false )
        {
            return "";
        }
        return languageIdTable[customID][0];
    }

    // 获取语言名称
    public static string languageName( string code )
    {
        if ( languageCodeTable.ContainsKey( code ) == false )
        {
            return "";
        }
        return text( Convert.ToInt32( languageCodeTable[code][1] ) );
    }

    // 获取语言名称
    public static string languageName( int customID )
    {
        if ( languageIdTable.ContainsKey( customID ) == false )
        {
            return "";
        }
        return text( Convert.ToInt32( languageIdTable[customID][1] ) );
    }

    // 获取语言目录
    public static string languageDirectory( string code )
    {
        if ( languageCodeTable.ContainsKey( code ) == false )
        {
            return "";
        }
        return languageCodeTable[code][2];
    }

    // 获取翻译Code
    public static string translationCode( string code )
    {
        if ( languageCodeTable.ContainsKey( code ) == false )
        {
            return "en";
        }
        return languageCodeTable[code][3];
    }

    public static string text( int id )
    {
        //Debug.Log( "id:" + id );
        if ( localize.ContainsKey( id ) == false )
        {
            if ( localize_en_us.ContainsKey( id )  == false )
                return "";
            return localize_en_us[id];
        }
        return localize[id];
    }

    public static string text_item( int id )
    {
        if ( localize_item.ContainsKey( id ) == false )
        {
            if ( localize_item_en_us.ContainsKey( id ) == false )
                return "";
            return localize_item_en_us[id];
        }
        return localize_item[id];
    }

    public static string text_quest( int id )
    {
        if ( localize_quest.ContainsKey( id ) == false )
        {
            if ( localize_quest_en_us.ContainsKey( id ) == false )
                return "";
            return localize_quest_en_us[id];
        }
        return localize_quest[id];
    }
}

