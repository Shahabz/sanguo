using UnityEngine;
using System.Collections;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using UnityEditor;
using UnityEditorInternal;

public class BuildCharAnimaiton : Editor
{    //美术给的原始图片路径
	private static string ImagePath = Application.dataPath + "/Sprites/Character";
	private static string AnimePath = Application.dataPath + "/Resources/PackAssets/Character";

	static string[] IgnoreList = { "Char_Shape_OgreMan0", "Char_Shape_OgreMan1", "Char_Shape_Mud", "Char_Shape_DragonIce", "Char_Shape_KingTower" };

    static bool CheckIgnoreList( string name )
    {
        for( int i = 0; i < IgnoreList.Length; i++ )
        {
            if( name == IgnoreList[i] )
                return true;
        }
        return false;
    }
	
	[MenuItem("Character/创建所有动画")]
	static void BuildAllAnimaition()
	{
		DirectoryInfo raw = new DirectoryInfo(ImagePath);
		foreach (DirectoryInfo dictorys in raw.GetDirectories())
		{
			string dictoryPath;
			int startIndex = dictorys.FullName.IndexOf ("Assets\\");
			if (startIndex == -1) {
				startIndex = dictorys.FullName.IndexOf ("Assets/");
				dictoryPath = dictorys.FullName.Substring( startIndex ).Replace("/", "/");
			}
			else{
				dictoryPath = dictorys.FullName.Substring( startIndex ).Replace("\\", "/");
			}
            if( CheckIgnoreList( dictorys.Name ) )
            {                
                Debug.Log( "Ignore:" + dictorys.Name );
                continue;
            }

			foreach( DirectoryInfo dictoryAnimations in dictorys.GetDirectories() )
			{
                string name = dictoryAnimations.Name;
                // 若果存在1方向或3方向的动画，则分别翻转生成7和5方向的动画
                if( name.Substring( name.Length - 1 ) == "1" )
                {
                    name = name.Substring( 0, name.Length - 1 ) + "7";
				    AssetDatabase.CreateAsset( BuildAnimationClip( dictoryAnimations, name ),
                        dictoryPath.Replace( "Sprites", "Resources/PackAssets" ) + "/" + name + ".anim" );
				    AssetDatabase.SaveAssets();
                }
                else if( name.Substring( name.Length - 1 ) == "2" )
                {
                    name = name.Substring( 0, name.Length - 1 ) + "6";
				    AssetDatabase.CreateAsset( BuildAnimationClip( dictoryAnimations, name ),
                        dictoryPath.Replace( "Sprites", "Resources/PackAssets" ) + "/" + name + ".anim" );
				    AssetDatabase.SaveAssets();
                }
                else if( name.Substring( name.Length - 1 ) == "3" )
                {
                    name = name.Substring( 0, name.Length - 1 ) + "5";
				    AssetDatabase.CreateAsset( BuildAnimationClip( dictoryAnimations, name ),
                        dictoryPath.Replace( "Sprites", "Resources/PackAssets" ) + "/" + name + ".anim" );
				    AssetDatabase.SaveAssets();
                }
				else if( name.Substring( name.Length - 1 ) == "6" )
				{
					name = name.Substring( 0, name.Length - 1 ) + "2";
					AssetDatabase.CreateAsset( BuildAnimationClip( dictoryAnimations, name ),
						dictoryPath.Replace( "Sprites", "Resources/PackAssets" ) + "/" + name + ".anim" );
					AssetDatabase.SaveAssets();
				}

				AssetDatabase.CreateAsset( BuildAnimationClip( dictoryAnimations, dictoryAnimations.Name ),
                    dictoryPath.Replace( "Sprites", "Resources/PackAssets" ) + "/" + dictoryAnimations.Name + ".anim" );
				AssetDatabase.SaveAssets();
			}
		}
	}
    
    [MenuItem("Character/清除全部动画")]
	static void ClearAllAnimation()
	{
		DirectoryInfo raw = new DirectoryInfo(AnimePath);
		foreach (DirectoryInfo dictorys in raw.GetDirectories())
		{
			FileInfo[] animes = dictorys.GetFiles().ToList().Where(x => x.Extension.ToLower() == ".anim" ).ToArray();
			for( int i = 0; i < animes.Length; i++ )
			{
				File.Delete( animes[i].FullName );
			}
		}
	}

	static int TurnNumToInt( string name )
	{
		string[] strs = name.Split( new char[]{ '_', '.' } );
		try
		{
			return int.Parse( strs[strs.Length-2] );
		}
		catch( UnityException e )
		{
			Debug.Log( name );
		}
		
		return 0;
	}
	
	static AnimationClip BuildAnimationClip( DirectoryInfo dictorys, string name )
    {
        string animationName = dictorys.Name;

        //查找所有图片，因为我找的测试动画是.jpg
		FileInfo[] images = dictorys.GetFiles().ToList().Where(x => (x.Extension.ToLower() == ".png" || x.Extension.ToLower() == ".tga")).OrderBy(x => ( TurnNumToInt( x.Name ) )).ToArray();
        AnimationClip clip = new AnimationClip();
        if( images.Length == 0 )
            return clip;

        EditorCurveBinding curveBinding = new EditorCurveBinding();
        curveBinding.type = typeof(SpriteRenderer);
        curveBinding.path = "";
        curveBinding.propertyName = "m_Sprite";

        ObjectReferenceKeyframe[] keyFrames;
        Sprite sprite;
        //动画长度是按秒为单位，1/10就表示1秒切10张图片，根据项目的情况可以自己调节
        float frameTime = 1f / 4f;
        
        // 最后补一个第一帧
        // 补帧一个第一帧
        if( !name.Contains( "DIE" ) && !name.Contains( "SP1" ) && !name.Contains( "SP2" ) )
        { 
            keyFrames = new ObjectReferenceKeyframe[images.Length+1];
            sprite = AssetDatabase.LoadAssetAtPath<Sprite>( DataPathToAssetPath( images[0].FullName ).Replace( "\\", "/" ) );
            keyFrames[images.Length] = new ObjectReferenceKeyframe();
            keyFrames[images.Length].time = frameTime * Mathf.Max( 0, ( images.Length + 1 - Mathf.Pow( 1.07f, images.Length ) ) );
            keyFrames[images.Length].value = sprite;
        }
        else
        {            
            keyFrames = new ObjectReferenceKeyframe[images.Length];
        }

        for (int i = 0; i < images.Length; i++)
        {
            sprite = AssetDatabase.LoadAssetAtPath<Sprite>(DataPathToAssetPath(images[i].FullName).Replace("\\", "/"));

            keyFrames[i] = new ObjectReferenceKeyframe();
            keyFrames[i].time = frameTime * Mathf.Max(0, (i +1- Mathf.Pow(1.07f, i)));
            keyFrames[i].value = sprite;
        }

        //动画帧率，30比较合适
        clip.frameRate = 30;

        //if( fullname.Contains( "Airman" ) )
        //{
        //    if( !name.Contains( "Dth" ) )
        //    {
        //        EditorCurveBinding curve = new EditorCurveBinding();
        //        curve.type = typeof( Transform );
        //        curve.path = "";
        //        curve.propertyName = "m_LocalPosition.y";

        //        AnimationCurve curveAnime = new AnimationCurve();
        //        curveAnime.AddKey( 0f, 2f );
        //        AnimationUtility.SetEditorCurve( clip, curve, curveAnime );
        //    }
        //    else
        //    {
        //        EditorCurveBinding curve = new EditorCurveBinding();
        //        curve.type = typeof( Transform );
        //        curve.path = "";
        //        curve.propertyName = "m_LocalPosition.y";

        //        float lastKey = keyFrames[images.Length - 1].time + frameTime * Mathf.Max( 0, ( 6 + 1 - Mathf.Pow( 1.07f, 6 ) ) );
        //        AnimationCurve curveAnime = new AnimationCurve();
        //        curveAnime.AddKey( 0f, 2f );
        //        curveAnime.AddKey( keyFrames[images.Length - 1].time/2f, 2.5f );
        //        curveAnime.AddKey( lastKey, 0f );
        //        AnimationUtility.SetEditorCurve( clip, curve, curveAnime );


        //        AnimationEvent evt = new AnimationEvent();
        //        evt.time = keyFrames[keyFrames.Length - 1].time;
        //        evt.functionName = "OnDie";
        //        AnimationUtility.SetAnimationEvents( clip, new AnimationEvent[] { evt } );
        //    }
        //}

        // 方向，5,7，翻转
		if( name.Substring( name.Length - 1 ) == "5" || name.Substring( name.Length - 1 ) == "2" || name.Substring( name.Length - 1 ) == "7" )
        {
            EditorCurveBinding curve = new EditorCurveBinding();
            curve.type = typeof( Transform );
            curve.path = "";
            curve.propertyName = "m_LocalScale.x";

            AnimationCurve curveAnime = new AnimationCurve();
            curveAnime.AddKey( 0f, -1f );
            AnimationUtility.SetEditorCurve( clip, curve, curveAnime );

            curve = new EditorCurveBinding();
            curve.type = typeof( Transform );
            curve.path = "";
            curve.propertyName = "m_LocalScale.y";

            curveAnime = new AnimationCurve();
            curveAnime.AddKey( 0f, 1f );
            AnimationUtility.SetEditorCurve( clip, curve, curveAnime );

            curve = new EditorCurveBinding();
            curve.type = typeof( Transform );
            curve.path = "";
            curve.propertyName = "m_LocalScale.z";

            curveAnime = new AnimationCurve();
            curveAnime.AddKey( 0f, 1f );
            AnimationUtility.SetEditorCurve( clip, curve, curveAnime );
        }
        else
        {
            EditorCurveBinding curve = new EditorCurveBinding();
            curve.type = typeof( Transform );
            curve.path = "";
            curve.propertyName = "m_LocalScale.x";

            AnimationCurve curveAnime = new AnimationCurve();
            curveAnime.AddKey( 0f, 1f );
            AnimationUtility.SetEditorCurve( clip, curve, curveAnime );

            curve = new EditorCurveBinding();
            curve.type = typeof( Transform );
            curve.path = "";
            curve.propertyName = "m_LocalScale.y";

            curveAnime = new AnimationCurve();
            curveAnime.AddKey( 0f, 1f );
            AnimationUtility.SetEditorCurve( clip, curve, curveAnime );

            curve = new EditorCurveBinding();
            curve.type = typeof( Transform );
            curve.path = "";
            curve.propertyName = "m_LocalScale.z";

            curveAnime = new AnimationCurve();
            curveAnime.AddKey( 0f, 1f );
            AnimationUtility.SetEditorCurve( clip, curve, curveAnime );
        }


        SerializedObject serializedClip = new SerializedObject(clip);
        AnimationClipSettings clipSettings = new AnimationClipSettings(serializedClip.FindProperty("m_AnimationClipSettings"));

        if( name.Contains( "ATTACK" ) )
		{
            clipSettings.loopTime = true;
            serializedClip.ApplyModifiedProperties();

            // 龙的攻击从第一帧开始
			AnimationEvent evt = new AnimationEvent();
            if( name.Contains( "Monster11" ) )
                evt.time = 0;
            else
                evt.time = keyFrames[images.Length - 1].time - frameTime * Mathf.Max( 0, ( 2 + 1 - Mathf.Pow( 1.07f, 2 ) ) );
			evt.functionName = "OnAttack";

			AnimationUtility.SetAnimationEvents( clip, new AnimationEvent[]{ evt } );
		}
        else if( name.Contains( "DIE" ) )
        {
            clipSettings.loopTime = false;
            serializedClip.ApplyModifiedProperties();

			AnimationEvent evt = new AnimationEvent();
			evt.time = keyFrames[keyFrames.Length-1].time;
			evt.functionName = "OnDie";
			
			AnimationUtility.SetAnimationEvents( clip, new AnimationEvent[]{ evt } );
        }
        else if( name.Contains( "SP1" ) )
        {
            clipSettings.loopTime = true;
            serializedClip.ApplyModifiedProperties();

			AnimationEvent evt = new AnimationEvent();
			evt.time = keyFrames[keyFrames.Length-1].time;
			evt.functionName = "OnSP1";
			
			AnimationUtility.SetAnimationEvents( clip, new AnimationEvent[]{ evt } );
        }
        else if( name.Contains( "SP2" ) )
        {
            clipSettings.loopTime = true;
            serializedClip.ApplyModifiedProperties();

			AnimationEvent evt = new AnimationEvent();
			evt.time = keyFrames[keyFrames.Length-1].time;
			evt.functionName = "OnSP2";
			
			AnimationUtility.SetAnimationEvents( clip, new AnimationEvent[]{ evt } );
        }
        else
        {
            clipSettings.loopTime = true;
            serializedClip.ApplyModifiedProperties();
        }
        
        AnimationUtility.SetObjectReferenceCurve(clip, curveBinding, keyFrames);

        return clip;
    }

    public static string DataPathToAssetPath(string path)
    {
        if (Application.platform == RuntimePlatform.WindowsEditor)
            return path.Substring(path.IndexOf("Assets\\"));
        else
            return path.Substring(path.IndexOf("Assets/"));
    }


    class AnimationClipSettings
    {
        SerializedProperty m_Property;

        private SerializedProperty Get(string property) { return m_Property.FindPropertyRelative(property); }

        public AnimationClipSettings(SerializedProperty prop) { m_Property = prop; }

        public float startTime { get { return Get("m_StartTime").floatValue; } set { Get("m_StartTime").floatValue = value; } }
        public float stopTime { get { return Get("m_StopTime").floatValue; } set { Get("m_StopTime").floatValue = value; } }
        public float orientationOffsetY { get { return Get("m_OrientationOffsetY").floatValue; } set { Get("m_OrientationOffsetY").floatValue = value; } }
        public float level { get { return Get("m_Level").floatValue; } set { Get("m_Level").floatValue = value; } }
        public float cycleOffset { get { return Get("m_CycleOffset").floatValue; } set { Get("m_CycleOffset").floatValue = value; } }

        public bool loopTime { get { return Get("m_LoopTime").boolValue; } set { Get("m_LoopTime").boolValue = value; } }
        public bool loopBlend { get { return Get("m_LoopBlend").boolValue; } set { Get("m_LoopBlend").boolValue = value; } }
        public bool loopBlendOrientation { get { return Get("m_LoopBlendOrientation").boolValue; } set { Get("m_LoopBlendOrientation").boolValue = value; } }
        public bool loopBlendPositionY { get { return Get("m_LoopBlendPositionY").boolValue; } set { Get("m_LoopBlendPositionY").boolValue = value; } }
        public bool loopBlendPositionXZ { get { return Get("m_LoopBlendPositionXZ").boolValue; } set { Get("m_LoopBlendPositionXZ").boolValue = value; } }
        public bool keepOriginalOrientation { get { return Get("m_KeepOriginalOrientation").boolValue; } set { Get("m_KeepOriginalOrientation").boolValue = value; } }
        public bool keepOriginalPositionY { get { return Get("m_KeepOriginalPositionY").boolValue; } set { Get("m_KeepOriginalPositionY").boolValue = value; } }
        public bool keepOriginalPositionXZ { get { return Get("m_KeepOriginalPositionXZ").boolValue; } set { Get("m_KeepOriginalPositionXZ").boolValue = value; } }
        public bool heightFromFeet { get { return Get("m_HeightFromFeet").boolValue; } set { Get("m_HeightFromFeet").boolValue = value; } }
        public bool mirror { get { return Get("m_Mirror").boolValue; } set { Get("m_Mirror").boolValue = value; } }
    }

}