using System;
using System.Collections.Generic;
using System.Text;
using UnityEngine;
/// <summary>
/// 表格读取工具
/// </summary>
public class TableUtil
{
    /// <summary>
    /// 行数
    /// </summary>
    private int mRecordsNum;

    /// <summary>
    /// 列数
    /// </summary>
    private int mFieldsNum;

    /// <summary>
    /// 数据区
    /// </summary>
    List<string> mDataBuf;

    /// <summary>
    /// 表格的列
    /// </summary>
    string[] mColumns;

    private string mFileName = "";

    /// <summary>
    /// 构造
    /// </summary>
    public TableUtil()
    {
        mRecordsNum = 0;
        mFieldsNum = 0;
        mDataBuf = new List<string>();
    }

    /// <summary>
    /// 打开TXT文件
    /// </summary>
    public bool OpenFromData( string fileName )
    {
        mFileName = fileName;
        // 优先尝试从解压目录读取
        byte[] fileData = FileUtil.GetInstance().GetDataFromFile( PathUtil.LocalDataPath() + fileName );
        if ( fileData == null )
        {
            // 没有就从安装包里读取
            fileData = FileUtil.GetInstance().GetDataFromWWW( PathUtil.PackageDataPath() + fileName );
            if ( fileData == null )
                return false;
        }

        return OpenFromMemory( fileData );
    }

    /// <summary>
    /// 打开TXT文件
    /// </summary>
    public bool OpenFromTXT( string fileName )
    {
        mFileName = fileName;
        byte[] fileData = FileUtil.GetInstance().GetDataFromFile( mFileName );
        if ( fileData == null )
            return false;

        return OpenFromMemory( fileData );
    }

    /// <summary>
    /// 解析内存数据
    /// </summary>
    public bool OpenFromMemory( byte[] memory )
    {
        string content = StringUtil.ByteToString( memory );

        // 拆分得到每行的内容
        content = content.Replace( "\r\n", "\n" );
        string[] lineArray = content.Split( new char[] { '\n' }, StringSplitOptions.RemoveEmptyEntries );
        if ( lineArray.Length < 1 )
            return false;

        // 分解第一行
        string[] strArray = lineArray[0].Split( new char[] { '\t' } );
        int recordsNum = 0;
        int fieldsNum = strArray.Length;
        mColumns = new string[fieldsNum];
        Array.Copy( strArray, mColumns, fieldsNum );

        // 遍历余下行数
        for ( int i = 1; i < lineArray.Length; ++i )
        {
            if ( lineArray[i].Length == 0 )
                break;

            strArray = lineArray[i].Split( new char[] { '\t' } );

            // 是不是有内容
            if ( strArray.Length == 0 )
                break;

            if ( strArray[0].Length == 0 )
                break;

            if ( strArray[0][0] == '\0' )
                break;

            // 是不是注释行
            if ( strArray[0][0] == '#' )
                continue;
            // 如果列数不对,则补上
            //            if (strArray.Length != fieldsNum)
            //           {
            //                if (strArray.Length < fieldsNum)
            //                {
            //                    int subNum = fieldsNum - strArray.Length;
            //                        for (int n = 0; n < subNum; ++n)
            //                            strArray.Add(""); // 这种数组类型看样子没法补了
            //               }
            //           }
            // Debug.Log( "id:" + strArray[0] + "  text:" + strArray[1] );
            // 填充数据区
            for ( int n = 0; n < fieldsNum; ++n )
            {
                mDataBuf.Add( strArray[n] );
            }

            ++recordsNum;
        }

        // 记录相关信息
        mRecordsNum = recordsNum;
        mFieldsNum = fieldsNum;
        return true;
    }

    /// <summary>
    /// 取数据
    /// </summary>
    /// <param name="recordLine">从0开始</param>
    /// <param name="columNum">从0开始</param>
    /// <returns></returns>
    public string GetValue( int recordLine, int columNum )
    {
#if UNITY_EDITOR
        try
        {
            int position = recordLine * mFieldsNum + columNum;

            if ( position < 0 || position > mDataBuf.Count )
            {
                string error = string.Format( "Invalid search request : recordLine:{0} columNum:{1}", recordLine, columNum );
                System.Diagnostics.Debug.Assert( false, error );
                throw new Exception( error );
            }

            return mDataBuf[position];
        }
        catch ( Exception ex )
        {
            System.Diagnostics.Debug.Assert( false, string.Format( "文件:{0} 读取出现异常:{1}", mFileName, ex.Message ) );
            return "";
        }
#else
        int position = recordLine * mFieldsNum + columNum;

            if (position < 0 || position > mDataBuf.Count)
            {
                string error = string.Format("Invalid search request : recordLine:{0} columNum:{1}", recordLine, columNum);
                System.Diagnostics.Debug.Assert(false, error);
                throw new Exception(error);
            }

            return mDataBuf[position];
#endif


    }
    /// <summary>
    /// 获取列
    /// </summary>
    /// <param name="columnNum"></param>
    /// <returns></returns>
    public string GetColumn( int columnNum )
    {
        return mColumns[columnNum];
    }
    /// <summary>
    /// 获取记录数
    /// </summary>
    public int GetRecordsNum()
    {
        return mRecordsNum;
    }

    /// <summary>
    /// 获取列数
    /// </summary>
    public int GetFieldsNum()
    {
        return mFieldsNum;
    }

}