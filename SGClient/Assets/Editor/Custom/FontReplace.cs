using UnityEngine;  
using System.Collections;  
using System.Collections.Generic;  
using UnityEditor;  
public class FontReplese  
{
	// 批量修改字体
    public static void ChangeLabelFont ( List<UIText> labelList )  
    {  
		Font mFont = Resources.Load<Font> ("Fonts/pglh" );// 地方是要替换成的字体的路径  
        if ( mFont == null )  
        {  
            Debug.LogError (" Font not found ! " );  
            return;  
        }  
        foreach (var label in labelList )  
        {
			if ( label !=null && label.font != null && ( label.font.name == "Arial" || label.font.name == "arial") )//这个地方的name可以改为原来的字体的名称  
            {  
                label.font = mFont;  
            }
        }  
        Debug.Log ( labelList.Count );  
    }  

	// 字体还原默认
	public static void ChangeLabelFontArial ( List<UIText> labelList )  
	{  
		Font mFont = Resources.Load<Font> ("Fonts/arial" );// 地方是要替换成的字体的路径  
		if ( mFont == null )  
		{  
			Debug.LogError (" Font not found ! " );  
			return;  
		}  
		foreach (var label in labelList )  
		{
			if ( label !=null && label.font != null && ( label.font.name == "pglh") )//这个地方的name可以改为原来的字体的名称  
			{  
				label.font = mFont;  
			}
		}  
		Debug.Log ( labelList.Count );  
	}  

	// 批量修改类型，粗体，斜体等
    public static void ChangeLabelStyle ( List<UIText> labelList )  
    {
        foreach ( var label in labelList )  
        {
            if ( label != null )//这个地方的name可以改为原来的字体的名称  
            {  
                label.fontStyle = FontStyle.Normal;
            }
        }
    }  
  

    //改变选中的预设上边的字体  
	[MenuItem ("Tools/ChangeUITextFont" )]  
	public static void ChangeUITextFont ()  
    {
        GetTypeList();
    }
  
    public static List<UIText> GetTypeList ()
    {  
        Object[] objList = Selection.GetFiltered ( typeof ( Object ), SelectionMode.DeepAssets );  
        //Debug.LogError (Selection.activeObject.name);  
        Debug.Log ( objList.Length );
        List<UIText> tmpList = new List<UIText> ();  
        foreach (var tmp in objList )  
        {  
            if ( ( tmp as GameObject ) !=null )  
            {  
                string assetstr =AssetDatabase.GetAssetPath ( tmp );  
                GameObject obj =PrefabUtility.InstantiatePrefab ( tmp ) as GameObject;  
  
                List<UIText> tmpLabel = GetTypeIncludeChildren ( obj );  
                if ( tmpLabel.Count >0 )  
                {  
					// 修改
                    //ChangeLabelStyle ( tmpLabel );
					ChangeLabelFont ( tmpLabel );  
					//ChangeLabelFontArial( tmpLabel );  

                    try  
                    {  
                        PrefabUtility.ReplacePrefab ( obj, tmp );  
                    }  
                    catch ( System.Exception ex )  
                    {  
                        Debug.LogError ( ex.ToString () + tmp.name );  
                    }  
                    //Debug.LogError ( AssetDatabase.RenameAsset ( assetstr, name ) );  
                }
                GameObject.DestroyImmediate ( obj );  
            }  
        }  
        AssetDatabase.SaveAssets ();  
        AssetDatabase.Refresh ();  
        return tmpList;  
    }  
  
    public static List<UIText> GetTypeIncludeChildren ( Object obj )  
    {  
        GameObject tmp = obj as GameObject;  
        if ( tmp ==null )  
        {  
            //return new List<UIText> ();  
        }  
        List<UIText> tmpList = new List<UIText> ();  
        UIText tmpLabel = tmp.GetComponent<UIText> ();  
        if ( tmpLabel !=null )  
        {  
            tmpList.Add ( tmpLabel );  
        }
        foreach ( Transform child in tmp.transform )
        {
            tmpList.AddRange( GetTypeIncludeChildren( child.gameObject ) );
        }  
        return tmpList;  
    }


	//改变选中的预设上边的字体大小
	[MenuItem ("Tools/ChangeUITextFontSzie" )]  
	public static void ChangeUITextFontSzie ()  
	{
		GetTypeList_FontSzie();
	}
	public static List<UIText> GetTypeList_FontSzie ()
	{  
		Object[] objList = Selection.GetFiltered ( typeof ( Object ), SelectionMode.DeepAssets );  
		//Debug.LogError (Selection.activeObject.name);  
		Debug.Log ( objList.Length );
		List<UIText> tmpList = new List<UIText> ();  
		foreach (var tmp in objList )  
		{  
			if ( ( tmp as GameObject ) !=null )  
			{  
				string assetstr =AssetDatabase.GetAssetPath ( tmp );  
				GameObject obj =PrefabUtility.InstantiatePrefab ( tmp ) as GameObject;  

				List<UIText> tmpLabel = GetTypeIncludeChildren ( obj );  
				if ( tmpLabel.Count > 0 )  
				{  
					// 修改
					RectTransform rect = obj.GetComponent<RectTransform>();
					ChangeLabelFontSize( tmpLabel, rect, assetstr ); 

					try  
					{  
						PrefabUtility.ReplacePrefab ( obj, tmp );  
					}  
					catch ( System.Exception ex )  
					{  
						Debug.LogError ( ex.ToString () + tmp.name );  
					}  
					//Debug.LogError ( AssetDatabase.RenameAsset ( assetstr, name ) );  
				}
				GameObject.DestroyImmediate ( obj );  
			}  
		}  
		AssetDatabase.SaveAssets ();  
		AssetDatabase.Refresh ();  
		return tmpList;  
	}  

	// 批量修改类型，粗体，斜体等
	public static void ChangeLabelFontSize ( List<UIText> labelList, RectTransform rect, string name )  
	{
		foreach ( var label in labelList )
		{
			if ( label !=null && label.font != null && ( label.font.name == "pglh") )//这个地方的name可以改为原来的字体的名称  
			{  
				//label.fontSize += 2;
				if (rect.rect.height <= 32) {
					Debug.LogError ( name );  
				}
			}
		}
	}  
  
}  