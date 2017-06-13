using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System;

// iOS
/*
app程序包安装路径，app本身就在这里，此目录是只读的，Editor中就是Assets
Application.dataPath            /var/containers/Bundle/Application/app sandbox/xxx.app/Data

dataPath下的Raw目录 此目录是只读的 通过IO读取就行
Application.streamingAssetsPath /var/containers/Bundle/Application/app sandbox/xxx.app/Data/Raw

Library下有两个子目录，Caches和Preferences
Caches是一个相对临时的目录，适合存放下载缓存的临时文件，空间不足时可能会被系统清除，Application.temporaryCachePath返回的就是此路径。
Preferences用于应用存储偏好设置，用NSUserDefaults读取或设置。
Application.temporaryCachePath /var/mobile/Containers/Data/Application/app sandbox/Library/Caches

这个目录用于存储需要长期保存的数据，比如我们的热更新内容就写在这里，注意iCloud会自动备份此目录 此目录是可读可写
Application.persistentDataPath /var/mobile/Containers/Data/Application/app sandbox/Documents
*/


// Android
/*
apk的安装路径 此目录是只读的
Application.dataPath            /data/app/package name-1/xxx.apk

此目录是只读的 WWW类来读取
Application.streamingAssetsPath jar:file:///data/app/package name-1/xxx.apk!/assets 

cache 缓存目录，类似于iOS的Cache目录 此目录是可读可写
Application.temporaryCachePath /storage/emulated/0/Android/data/package name/cache

files 类似于iOS的Documents目录  此目录是可读可写
Application.persistentDataPath /storage/emulated/0/Android/data/package name/files
*/

// 路径
public static class PathUtil
{
    // 安装包路径（只读）
    public static string PackagePath()
    {
        string path = string.Empty;
        switch ( Application.platform )
        {
            case RuntimePlatform.Android:
                path = Application.dataPath + "!assets/";
                break;
            case RuntimePlatform.IPhonePlayer:
                path = Application.dataPath + "/Raw/";
                break;
            default:
                path = Application.dataPath + "/StreamingAssets/";
                break;
        }
        return path;
    }

    // 安装包路径（只读）
    public static string StreamingAssetsPath()
    {
        string path = string.Empty;
        switch ( Application.platform )
        {
            case RuntimePlatform.Android:
                path = Application.streamingAssetsPath + "/";
                break;
            case RuntimePlatform.IPhonePlayer:
                path = Application.streamingAssetsPath + "/";
                break;
            default:
                path = Application.streamingAssetsPath + "/";
                break;
        }
        return path;
    }

    // 程序资源数据路径，也就是资源解压出来的本地路径（可读可写）
    public static string LocalPath()
    {
        string path = string.Empty;
        switch ( Application.platform )
        {
            case RuntimePlatform.Android:
                path = Application.persistentDataPath + "/LocalResources/";
                break;
            case RuntimePlatform.IPhonePlayer:
                path = Application.persistentDataPath + "/LocalResources/";
                break;
            default:
                path = Application.persistentDataPath + "/LocalResources/";
                break;
        }
        return path;
    }

    /// <summary>
    /// 读文件时候所用的文件头
    /// </summary>
    public static string platformPathHead()
    {
        string path = string.Empty;
        switch ( Application.platform )
        {
            case RuntimePlatform.Android:
                path = "file://";
                break;
            case RuntimePlatform.IPhonePlayer:
                path = "file://";
                break;
            case RuntimePlatform.WindowsPlayer:
                path = "file:///";
                break;
            case RuntimePlatform.WindowsEditor:
                path = "file:///";
                break;
            case RuntimePlatform.OSXPlayer:
                path = "file://";
                break;
            case RuntimePlatform.OSXEditor:
                path = "file://";
                break;
        }
        return path;
    }

    /// <summary>
    /// 解压出来的Data路径
    /// </summary>
    public static string LocalDataPath()
    {
        if ( Const.ResourceMode == "assetbundle" || !Application.isEditor )
            return LocalPath() + "/Data/";
        else
            return Application.dataPath + "/Resources/PackAssets/Data/";
    }

    /// <summary>
    /// 安装包里的Data路径
    /// </summary>
    public static string PackageDataPath()
    {
        if ( Const.ResourceMode == "assetbundle" || !Application.isEditor )
            return Application.streamingAssetsPath + "/Data/";
        else
            return Application.dataPath + "/Resources/PackAssets/Data/";
    }

    /// <summary>
    /// 解压出来的Lua脚本路径
    /// </summary>
    public static string LocalLuaPath()
    {
        if ( Const.ResourceMode == "assetbundle" || !Application.isEditor )
            return LocalPath() + "/Lua/";
        else
            return Application.dataPath + "/Lua/";         
    }
    public static string LocalLuaPath( string name )
    {
        string lowerName = name.ToLower();
        if ( lowerName.EndsWith( ".lua" ) )
        {
            return LocalLuaPath() + name;
        }
        return LocalLuaPath() + name + ".lua";
    }
    public static string LocalToLuaPath( string name )
    {
        string lowerName = name.ToLower();
        if ( lowerName.EndsWith( ".lua" ) )
        {
            return LocalLuaPath()+ "tolua/" + name;
        }
        return LocalLuaPath() + "tolua/" + name + ".lua";
    }

    /// <summary>
    /// 安装包里的Lua路径
    /// </summary>
    public static string PackageLuaPath()
    {
        if ( Const.ResourceMode == "assetbundle" || !Application.isEditor )
            return StreamingAssetsPath() + "Lua/";
        else
            return Application.dataPath + "/Lua/";          
    }
    public static string PackageLuaPath( string name )
    {
        string lowerName = name.ToLower();
        if ( lowerName.EndsWith( ".lua" ) )
        {
            return PackageLuaPath() + name;
        }
        return PackageLuaPath() + name + ".lua";
    }
    public static string PackageToLuaPath( string name )
    {
        string lowerName = name.ToLower();
        if ( lowerName.EndsWith( ".lua" ) )
        {
            return PackageLuaPath() + "tolua/" + name;
        }
        return PackageLuaPath() + "tolua/" + name + ".lua";
    }

    /// <summary>
    /// 日志路径
    /// </summary>
    public static string LogPath()
    {
        if ( Const.ResourceMode == "assetbundle" || !Application.isEditor )
            return Application.temporaryCachePath + "/Log/";
        else
            return Application.dataPath + "/../Log/";
    }

    /// <summary>
    /// 录像路径
    /// </summary>
    public static string RecPath()
    {
        if ( Const.ResourceMode == "assetbundle" || !Application.isEditor )
            return Application.temporaryCachePath + "/Recode/";
        else
            return Application.dataPath + "/../Recode/";
    }

    /// <summary>
    /// 临时补丁文件路径
    /// </summary>
    public static string PatchDownloadPath()
    {
        if ( Const.ResourceMode == "assetbundle" || !Application.isEditor )
            return Application.temporaryCachePath + "/Patch/";
        else
            return Application.dataPath + "/../Patch/";
    }

    /// <summary>
    /// 头像路径
    /// </summary>
    public static string HeadPath()
    {
        if ( Const.ResourceMode == "assetbundle" || !Application.isEditor )
            return Application.temporaryCachePath + "/Head/";
        else
            return Application.dataPath + "/../Head/";
    }

    /// <summary>
    /// 临时下载图片路径
    /// </summary>
    public static string ImagePath()
    {
        if ( Const.ResourceMode == "assetbundle" || !Application.isEditor )
            return Application.persistentDataPath + "/Image/";
        else
            return Application.dataPath + "/../Image/";
    }

    /// <summary>
    /// 本地用户配置文件路径
    /// </summary>
    public static string ConfigPath()
    {
        // 配置文件读取要在所有逻辑开始之前
        if ( Const.ResourceMode == "assetbundle" || !Application.isEditor )
            return Application.persistentDataPath + "/Config/";
        else
            return Application.dataPath + "/../Config/";
    }

    public static bool localLuaExists( string name )
    {
        return false;
    }

    public static bool localDataExists( string name )
    {
        if ( File.Exists( LocalPath() + name ) )
        { // 本地可写目录存在
            return true;
        }
        return false;
    }
}
