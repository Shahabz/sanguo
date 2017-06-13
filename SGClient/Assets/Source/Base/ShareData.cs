using UnityEngine;
using System.Collections;
using System.Collections.Generic;

// 扔到对象上 可通过这个类让 C#和lua 通用数据
public class ShareData : MonoBehaviour
{
	public int[]		intValue;
	public float[]		floatValue;
	public bool[]		boolValue;
	public string[]		stringValue;
    public Hashtable	_vars = new Hashtable();

    /// <summary>
    /// 添加变量
    /// </summary>
    public void AddValue(string name, string value) {
        _vars[name]= value;
    }

    /// <summary>
    /// 获取变量
    /// </summary>
    public object GetValue(string name) {
        return _vars[name];
    }

    /// <summary>
    /// 移除变量
    /// </summary>
    public void RemoveValue(string name) {
        _vars.Remove(name);
    }

    /// 设置变量
    public void SetInt( int index, int value )
    {        
        if( index < 0 )
            return;

        if( intValue == null )
        {
            intValue = new int[index+1];
            intValue[index] = value;
        }
        else if( index >= intValue.Length )
        {
            int[] newInt = new int[index+1];
            System.Array.Copy( intValue, newInt, intValue.Length );
            newInt[index] = value;
            intValue = newInt;
        }
        else
            intValue[index] = value;
    }

    /// <summary>
    /// 清除变量
    /// </summary>
    public void ClearShareVars() {
        _vars.Clear();
    }
}
