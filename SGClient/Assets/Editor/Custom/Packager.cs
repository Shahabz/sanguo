using UnityEditor;
using UnityEngine;
using System;
using System.IO;
using System.Text;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;

public class Packager
{
    public static string platform = string.Empty;
    static List<string> paths = new List<string>();
    static List<string> files = new List<string>();

    /// <summary>
    /// 打不同平台资源包入口
    /// </summary>
	[MenuItem( "Packager/切换为开发模式(Resource)" )]
    public static void Switch_Develop()
    {
        string signfilePath = Application.dataPath + "/" + ".develop.mode";
        if ( File.Exists( signfilePath ) )
        {
            EditorUtility.DisplayDialog( "警告", "已经是开发模式了，无需切换！", "确定" );
        }
        else
        {
            FileUtil.GetInstance().WriteFile( signfilePath, "develop" );
            EditorUtility.DisplayDialog( "注意", "已经切换到开发模式了！", "确定" );
        }
    }
    [MenuItem( "Packager/切换为发布模式(AssetBundle)" )]
    public static void Switch_Release()
    {
        string signfilePath = Application.dataPath + "/" + ".develop.mode";
        if ( File.Exists( signfilePath ) )
        {
            File.Delete( signfilePath );
            EditorUtility.DisplayDialog( "注意", "已经切换到发布模式了！", "确定" );
        }
        else
        {
            EditorUtility.DisplayDialog( "警告", "已经是发布模式了，无需切换！", "确定" );
        }
    }

    [MenuItem( "Packager/Build Assets - android" )]
    public static void BuildAssets_android()
    {
        BuildAssets( BuildTarget.Android );
    }

    [MenuItem( "Packager/Build Assets - ios" )]
    public static void BuildAssets_ios()
    {
        BuildAssets( BuildTarget.iOS );
    }

    [MenuItem( "Packager/Build Assets - windows" )]
    public static void BuildAssets_Windows()
    {
        BuildAssets( BuildTarget.StandaloneWindows );
    }

    [MenuItem( "Packager/Build Assets - OSX" )]
    public static void BuildAssets_Osx()
    {
        BuildAssets( BuildTarget.StandaloneOSXUniversal );
    }

    /// <summary>
    /// 打包
    /// </summary>
    public static void BuildAssets( BuildTarget target )
    {
        // StreamingAssets 目录
        string streamingAssetsPath = Application.dataPath + "/StreamingAssets/";
        string StreamingAssetsPathUpload = Application.dataPath + "/../StreamingAssetsUpload/";

        // 没有StreamingAssets就创建
        if ( !Directory.Exists( streamingAssetsPath ) )
            Directory.CreateDirectory( streamingAssetsPath );
        if ( !Directory.Exists( StreamingAssetsPathUpload ) )
            Directory.CreateDirectory( StreamingAssetsPathUpload );

        // 清空StreamingAssets目录
        clearDirectory( streamingAssetsPath );
        clearDirectory( StreamingAssetsPathUpload );

        //if ( Const.ResourceMode == "assetbundle" )
        //{
            BuildPipeline.BuildAssetBundles( StreamingAssetsPathUpload, BuildAssetBundleOptions.ChunkBasedCompression, target );
        //}
        //else
        //    UnityEngine.Debug.Log( "Const.ResourceMode = " + Const.ResourceMode );

        FileInfo fi = new FileInfo( StreamingAssetsPathUpload + "StreamingAssetsUpload" );
        fi.MoveTo( StreamingAssetsPathUpload + "StreamingAssets" );

        fi = new FileInfo( StreamingAssetsPathUpload + "StreamingAssetsUpload.manifest" );
        fi.MoveTo( StreamingAssetsPathUpload + "StreamingAssets.manifest" );

        // -------------------------------------- 资源版本文件部分 -------------------------------------------
        // 修改app配置的版本号，每次打包+1
        IniUtil resverIni = new IniUtil();
        resverIni.OpenFromTXT( Application.dataPath + "/../Config/resver.txt" );
        int new_resource_version = Convert.ToInt32( resverIni.ReadValue( "RESOURCE_VERSION", "0" ) ) + 1;
        resverIni.WriteValue( "RESOURCE_VERSION", new_resource_version );
        File.Copy( Application.dataPath + "/../Config/resver.txt", StreamingAssetsPathUpload + "resver.txt", true );
     
        // -------------------------------------- 配置文件部分 -------------------------------------------
        // 配置文件在StreamingAssets文件夹中的路径
        string targetTxtPath = StreamingAssetsPathUpload + "/Data/";
        if ( Directory.Exists( targetTxtPath ) )
            Directory.Delete( targetTxtPath, true );
        Directory.CreateDirectory( targetTxtPath );

        paths.Clear();
        files.Clear();

        // 配置文件源路径
        string sourceTxtPath = Application.dataPath + "/Resources/PackAssets/Data/";

        // 递归遍历所有文件文件夹
        Recursive( sourceTxtPath );

        // 遍历所有文件并复制
        foreach ( string f in files )
        {
            if ( f.EndsWith( ".meta" ) || f.EndsWith( ".DS_Store" ) )
                continue;
            string newfile = f.Replace( sourceTxtPath, "" );
            if ( newfile.Equals( "user.txt" ) )
                continue;
            string newpath = targetTxtPath + newfile;
            string path = Path.GetDirectoryName( newpath );
            if ( !Directory.Exists( path ) )
                Directory.CreateDirectory( path );
            File.Copy( f, newpath, true );
        }

        // -------------------------------------- lua脚本部分 -------------------------------------------
        // lua脚本在StreamingAssets文件夹中的路径
        string luaPath = StreamingAssetsPathUpload + "Lua/";
        if ( Directory.Exists( luaPath ) )
            Directory.Delete( luaPath, true );
        Directory.CreateDirectory( luaPath );

        paths.Clear();
        files.Clear();

        // lua脚本源路径
        string luaDataPath = Application.dataPath + "/Lua/";

        // 递归遍历所有文件文件夹
        Recursive( luaDataPath );

        // 遍历所有文件并复制
        foreach ( string f in files )
        {
            if ( !f.EndsWith( ".lua" ) )
                continue;
            string newfile = f.Replace( luaDataPath, "" );
            string newpath = luaPath + newfile;
            string path = Path.GetDirectoryName( newpath );
            if ( !Directory.Exists( path ) )
                Directory.CreateDirectory( path );
            File.Copy( f, newpath, true );
//            if ( target == BuildTarget.StandaloneWindows || target == BuildTarget.StandaloneOSXUniversal )
//                File.Copy( f, newpath, true );
//            else
//#if UNITY_STANDALONE_WIN
//                Execute( Application.dataPath + "/../../Public/ghlua.exe", f + " " + newpath, 0 );
//#else
//			Execute(Application.dataPath+"/../../Public/ghlua", f + " " + newpath, 0);
//#endif

            //Execute(Application.dataPath+"/../../Public/luajit/luajit.exe", " -b " + f + " " + newpath, 0);
            //Execute(Application.dataPath + "/../../Public/luac/Luac.exe", " -o " + newpath + " " + f, 0);
        }

        // -------------------------------------- 所有文件列表部分 -------------------------------------------
        // 文件列表
        string newFilePath = StreamingAssetsPathUpload + "files.txt";
        if ( target == BuildTarget.Android )
            newFilePath = StreamingAssetsPathUpload + "android_files.txt";
        else if ( target == BuildTarget.iOS )
            newFilePath = StreamingAssetsPathUpload + "ios_files.txt";
        else if ( target == BuildTarget.StandaloneWindows )
            newFilePath = StreamingAssetsPathUpload + "windows_files.txt";
        else if ( target == BuildTarget.StandaloneOSXUniversal )
            newFilePath = StreamingAssetsPathUpload + "osx_files.txt";

        // 如果文件存在删除文件
        if ( File.Exists( newFilePath ) )
            File.Delete( newFilePath );

        paths.Clear();
        files.Clear();

        // 递归遍历所有文件文件夹
        Recursive( StreamingAssetsPathUpload );

        // 创建文件列表
        FileStream fs = new FileStream( newFilePath, FileMode.CreateNew );
        StreamWriter sw = new StreamWriter( fs );
        for ( int i = 0; i < files.Count; i++ )
        {
            string file = files[i];
            string ext = Path.GetExtension( file );
            if ( ext.Equals( ".meta" ) || ext.Equals( ".manifest" ) || ext.Equals( ".DS_Store" ) )
                continue;

            // md5 值
            string md5 = Utils.md5file( file );
            // 文件大小
            FileInfo fileInfo = new FileInfo( file );
            long size = fileInfo.Length;
            // 文件相对路径
            string value = file.Replace( StreamingAssetsPathUpload, string.Empty );
            sw.WriteLine( value + "|" + md5 + "|" + size );
        }
        sw.Close();
        fs.Close();

        // ------------------------------------------------------------------------------------------------------------------------------------------------------------
        // -------------------------------------- 完毕以后，把StreamingAssetsUpload里的文件拷贝到StreamingAssets，刨去需要过滤的 ------------------------------------------- 
        if ( target == BuildTarget.Android || target == BuildTarget.StandaloneWindows )
        {
            foreach ( string f in files )
            {
                //string filename = Path.GetFileName( f );
                //if ( filename.Equals( "android_files.txt" )  )
                //    continue;

                //if ( filename.Equals( "ui_background" ) )
                //    continue;
                //if ( filename.Equals( "ui_background.manifest" ) )
                //    continue;

                //if ( filename.Equals( "ui_building" ) )
                //    continue;
                //if ( filename.Equals( "ui_building.manifest" ) )
                //    continue;

                //if ( filename.Equals( "ui_dialog" ) )
                //    continue;
                //if ( filename.Equals( "ui_dialog.manifest" ) )
                //    continue;

                //if ( filename.Equals( "ui_film" ) )
                //    continue;
                //if ( filename.Equals( "ui_film.manifest" ) )
                //    continue;

                //if ( filename.Equals( "ui_pre" ) )
                //    continue;
                //if ( filename.Equals( "ui_pre.manifest" ) )
                //    continue;

                //if ( filename.Equals( "tiled2unity" ) )
                //    continue;
                //if ( filename.Equals( "tiled2unity.manifest" ) )
                //    continue;

                //if ( filename.Equals( "mapthumb" ) )
                //    continue;
                //if ( filename.Equals( "mapthumb.manifest" ) )
                //    continue;

                //if ( filename.Equals( "mapcity" ) )
                //    continue;
                //if ( filename.Equals( "mapcity.manifest" ) )
                //    continue;

                //if ( filename.Equals( "building" ) )
                //    continue;
                //if ( filename.Equals( "building.manifest" ) )
                //    continue;

                //if ( filename.Equals( "char_1" ) )
                //    continue;
                //if ( filename.Equals( "char_1.manifest" ) )
                //    continue;

                //if ( filename.Equals( "city_0" ) )
                //    continue;
                //if ( filename.Equals( "city_0.manifest" ) )
                //    continue;

                //if ( filename.Equals( "effect_0" ) )
                //    continue;
                //if ( filename.Equals( "effect_0.manifest" ) )
                //    continue;

                //if ( filename.Equals( "effect_2" ) )
                //    continue;
                //if ( filename.Equals( "effect_2.manifest" ) )
                //    continue;

                string newfile = f.Replace( "../StreamingAssetsUpload", "StreamingAssets" );
                string path = Path.GetDirectoryName( newfile );
                if ( !Directory.Exists( path ) )
                    Directory.CreateDirectory( path );
                File.Copy( f, newfile, true );
            }
        }
        else if ( target == BuildTarget.iOS )
        {
            foreach ( string f in files )
            {
                string filename = Path.GetFileName( f );
                if ( filename.Equals( "ios_files.txt" ) )
                    continue;

                string newfile = f.Replace( "../StreamingAssetsUpload", "StreamingAssets" );
                string path = Path.GetDirectoryName( newfile );
                if ( !Directory.Exists( path ) )
                    Directory.CreateDirectory( path );
                File.Copy( f, newfile, true );
            }
        }

        newFilePath = newFilePath.Replace( "../StreamingAssetsUpload", "StreamingAssets" );
        // 如果文件存在删除文件
        if ( File.Exists( newFilePath ) )
            File.Delete( newFilePath );

        paths.Clear();
        files.Clear();

        // 递归遍历所有文件文件夹
        Recursive( streamingAssetsPath );

        // 从新创建文件列表
        fs = new FileStream( newFilePath, FileMode.CreateNew );
        sw = new StreamWriter( fs );
        for ( int i = 0; i < files.Count; i++ )
        {
            string file = files[i];
            string ext = Path.GetExtension( file );
            if ( ext.Equals( ".meta" ) || ext.Equals( ".manifest" ) || ext.Equals( ".DS_Store" ) )
                continue;
            // md5 值
            string md5 = Utils.md5file( file );
            // 文件大小
            FileInfo fileInfo = new FileInfo( file );
            long size = fileInfo.Length;
            // 文件相对路径
            string value = file.Replace( streamingAssetsPath, string.Empty );
            sw.WriteLine( value + "|" + md5 + "|" + size );
        }
        sw.Close();
        fs.Close();

        AssetDatabase.Refresh();
        //ZipProxy.compless(resPath, "z.zip");
    }

    /// <summary>
    /// 递归遍历所有文件文件夹
    /// </summary>
    static void Recursive( string path )
    {
        string[] names = Directory.GetFiles( path );
        string[] dirs = Directory.GetDirectories( path );
        foreach ( string filename in names )
        {
            string ext = Path.GetExtension( filename );
            if ( ext.Equals( ".meta" ) || ext.Equals( ".DS_Store" ) )
                continue;
            files.Add( filename.Replace( '\\', '/' ) );
        }
        foreach ( string dir in dirs )
        {
            paths.Add( dir.Replace( '\\', '/' ) );
            Recursive( dir );
        }
    }

    /// <summary>
    /// 清空目录
    /// </summary>
    public static void clearDirectory( string path )
    {
        DirectoryInfo info = new DirectoryInfo( path );
        try
        {
            foreach ( System.IO.DirectoryInfo d in info.GetDirectories() )
            {
                d.Delete( true );
            }
            foreach ( System.IO.FileInfo f in info.GetFiles() )
            {
                f.Delete();
            }
        }
        catch ( Exception e )
        {
            UnityEngine.Debug.LogError( e.Message );
        }
    }

    /// <summary>
    /// 执行程序
    /// </summary>
    public static string Execute( string FileName, string dosCommand, int milliseconds )
    {
        string output = "";     //输出字符串
        if ( dosCommand != null && dosCommand != "" )
        {
            Process process = new Process();     //创建进程对象
            ProcessStartInfo startInfo = new ProcessStartInfo();
            startInfo.FileName = FileName;      //设定需要执行的程序
            startInfo.Arguments = "" + dosCommand;   //设定参数，其中的“/C”表示执行完命令后马上退出
            startInfo.UseShellExecute = false;     //不使用系统外壳程序启动
            startInfo.RedirectStandardInput = false;   //不重定向输入
            startInfo.RedirectStandardOutput = true;   //重定向输出
            startInfo.CreateNoWindow = true;     //不创建窗口
            process.StartInfo = startInfo;
            try
            {
                if ( process.Start() )       //开始进程
                {
                    if ( milliseconds == 0 )
                        process.WaitForExit();     //这里无限等待进程结束
                    else
                        process.WaitForExit( milliseconds );  //这里等待进程结束，等待时间为指定的毫秒
                    output = process.StandardOutput.ReadToEnd();//读取进程的输出
                }
            }
            catch ( Exception e )
            {
                UnityEngine.Debug.LogError( e.Message );
            }
            finally
            {
                if ( process != null )
                    process.Close();
            }
        }
        return output;
    }

}