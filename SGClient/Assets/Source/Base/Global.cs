using UnityEngine;
using System.Collections;
using System.Collections.Generic;

// 通过这个类可让 C#和lua 共享变量
public static class Global {
    public static Hashtable ShareVars = new Hashtable();        //Lua共享变量
 
    /// <summary>
    /// 添加变量
    /// </summary>
    public static void AddValue(string name, string value) {
        if ( ShareVars.ContainsKey(name) )
            ShareVars[name] = value;
        else
            ShareVars.Add(name, value);
    }

    /// <summary>
    /// 获取变量
    /// </summary>
    public static object GetValue(string name) {
        if ( ShareVars.ContainsKey( name ) )
            return ShareVars[name];
        else
            return "";
    }

    /// <summary>
    /// 移除变量
    /// </summary>
    public static void RemoveValue(string name) {
        ShareVars.Remove(name);
    }

    /// <summary>
    /// 清除变量
    /// </summary>
    public static void ClearShareVars() {
        ShareVars.Clear();
    }
}
