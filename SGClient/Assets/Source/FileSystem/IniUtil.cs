using System;
using System.Collections.Generic;
using System.Text;
using UnityEngine;
/// <summary>
/// 表格读取工具
/// </summary>
public class IniUtil
{
	/// <summary>
	/// 节点值
	/// </summary>
	private Dictionary<string, string> m_dict;
	private string m_fileName;

    /// <summary>
    /// 构造
    /// </summary>
    public IniUtil()
    {
		m_fileName = "";
		m_dict = new Dictionary<string, string>();
    }

    /// <summary>
    /// 打开TXT文件
    /// </summary>
    public bool OpenFromData( string fileName )
    {
        m_fileName = fileName;
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
    /// 打开TXT文件， 配置文件一般在Config里
    /// </summary>
    public bool OpenFromTXT( string fileName )
    {
        m_dict.Clear();
		m_fileName = fileName;
        byte[] fileData = FileUtil.GetInstance().GetDataFromFile( fileName );
        if (fileData == null)
            return false;
        return OpenFromMemory(fileData);
    }

    /// <summary>
    /// 打开TXT文件从StreamingAssets
    /// </summary>
    public bool OpenFromStreamingAssets( string fileName )
    {
        m_dict.Clear();
        m_fileName = fileName;
        byte[] fileData = FileUtil.GetInstance().GetDataFromWWW( PathUtil.StreamingAssetsPath() + fileName );
        if ( fileData == null )
            return false;
        return OpenFromMemory( fileData );
    }

    /// <summary>
    /// 解析内存数据
    /// </summary>
    public bool OpenFromMemory(byte[] memory)
    {
		string content = StringUtil.ByteToString(memory);
		string st = null;
		int equalSignPos;
		string key, value;

		// 拆分得到每行的内容
        content = content.Replace( "\r\n", "\n" );
		string[] lineArray = content.Split(new char[] { '\n' }, StringSplitOptions.RemoveEmptyEntries);
		if (lineArray.Length < 1)
			return false;

		for ( int tmpi=0; tmpi < lineArray.Length; tmpi++ )
		{
			st = lineArray[tmpi].Trim();
			if (st == "")
				continue;
			if ( st[0] == '#' )
			{
				
			}
			else
			{                    
				//开始解析         
				equalSignPos = st.IndexOf('=');
				if (equalSignPos != 0)
				{
					key = st.Substring(0, equalSignPos);
					value = st.Substring(equalSignPos + 1, st.Length - equalSignPos - 1);
					if (m_dict.ContainsKey(key))
						m_dict[key] = value;
					else
						m_dict.Add(key, value);
				}
				else
				{
					if (m_dict.ContainsKey(st))
						m_dict[st] = "";
					else
						m_dict.Add(st, "");
				}              
			}                                
		}

        return true;
    }

	public void Clear()
	{
		m_dict.Clear();
	}        

	// 写入一个值
	public void WriteValue(string key, object value)
	{
		if (m_dict.ContainsKey(key))
			m_dict[key] = value.ToString();
		else
			m_dict.Add(key, value.ToString());

		string IniText="";
		foreach ( var item in m_dict )
		{

			IniText = IniText + item.Key + "=" + item.Value + "\r\n";
			
		}
		FileUtil.GetInstance().WriteFile( m_fileName, IniText );
	}

	// 读取一个值
	public object ReadValue(string key, string defaultv)
	{
		if (m_dict.ContainsKey(key))
			return m_dict[key];
		else
			return defaultv;
	}

    // 关联到global里
    public void SetGlobal()
    {
       foreach ( var item in m_dict )
       {
           Global.AddValue(item.Key, item.Value);
       }
    }
}

