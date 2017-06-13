using UnityEngine;
using System.Collections;
using UnityEditor;  

public class LocalizeReload
{     
    [MenuItem ("Localize/LocalizeReload" )]  
    public static void ChangeSelectStyle ()  
    {
        Localization.init();  
        
             
    }

}
