using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using UnityEditor;

public class SpriteChecker 
{
    // 检查是否有同一图集，却打在了2个assetbundle里的图片
    [MenuItem("Tools/SpriteChecker")]
    static void CheckSpritesTagsAndBundles () 
    {
        // Get all the GUIDs (identifiers in project) of the Sprites in the Project
        string[] guids = AssetDatabase.FindAssets ("t:sprite");

        // Dictionary to store the tags and bundle names
        Dictionary<string,string> dict = new Dictionary<string, string>();
        foreach( string guid in guids) 
        {
            string path = AssetDatabase.GUIDToAssetPath(guid);
            TextureImporter ti = TextureImporter.GetAtPath( path) as TextureImporter;

            // If the tag is not in the dictionary, add it
            if (!dict.ContainsKey (ti.spritePackingTag ))
                dict.Add(ti.spritePackingTag, ti.assetBundleName);
            else
                // If the tag is associated with another bundle name, show an error
                if (dict[ti.spritePackingTag] != ti.assetBundleName)
                    Debug.LogWarning("Sprite : " + ti.assetPath + " should be packed in "+  dict[ti.spritePackingTag] );
        }
    }
}
