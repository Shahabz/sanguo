using UnityEngine;
using System.Collections;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using UnityEditor;
using UnityEditorInternal;

public class BuildAnimaiton : Editor
{	
	[MenuItem("Tools/创建选中目录所有动画")]
	static void BuildSelectAnimaition()
	{
		ClearSelectAnimation();
		string dictoryPath = AssetDatabase.GetAssetPath(Selection.activeObject);
		DirectoryInfo raw = new DirectoryInfo(dictoryPath);
		foreach (DirectoryInfo dictoryAnimations in raw.GetDirectories())
		{
			//每个文件夹就是一组帧动画，这里把每个文件夹下的所有图片生成出一个动画文件
			string name = dictoryAnimations.Name;
			AnimationClip clip = BuildAnimationClip(dictoryAnimations, name);
			AssetDatabase.CreateAsset(clip, dictoryPath + "/" + name + ".anim");
			AssetDatabase.SaveAssets();
		}
	}

	[MenuItem("Tools/创建选中目录当前动画")]
	static void BuildSelectCurrentAnimaition()
	{
		ClearSelectAnimation();
		string dictoryPath = AssetDatabase.GetAssetPath(Selection.activeObject);
		DirectoryInfo raw = new DirectoryInfo(dictoryPath);
        
		string name = raw.Name;
		AnimationClip clip = BuildAnimationClip(raw, name);
		AssetDatabase.CreateAsset(clip, dictoryPath + "/" + name + ".anim");
		AssetDatabase.SaveAssets();
    }
	
	//[MenuItem("Fight/ClearSelectAnimation--选中")]
	static void ClearSelectAnimation()
	{
		string dictoryPath = AssetDatabase.GetAssetPath(Selection.activeObject);
		DirectoryInfo dictorys = new DirectoryInfo(dictoryPath);
		FileInfo[] animes = dictorys.GetFiles().ToList().Where(x => x.Extension.ToLower() == ".anim" ).ToArray();
		for( int i = 0; i < animes.Length; i++ )
		{
			File.Delete( animes[i].FullName );
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
	
	static AnimationClip BuildAnimationClip(DirectoryInfo dictorys, string name)
    {
        string animationName = dictorys.Name;

        //查找所有图片，因为我找的测试动画是.jpg
		FileInfo[] images = dictorys.GetFiles().ToList().Where(x => (x.Extension.ToLower() == ".png" || x.Extension.ToLower() == ".tga")).OrderBy(x => ( TurnNumToInt( x.Name ) )).ToArray();
        AnimationClip clip = new AnimationClip();

        EditorCurveBinding curveBinding = new EditorCurveBinding();
        curveBinding.type = typeof(SpriteRenderer);
        curveBinding.path = "";
        curveBinding.propertyName = "m_Sprite";
        ObjectReferenceKeyframe[] keyFrames = new ObjectReferenceKeyframe[images.Length];
        //动画长度是按秒为单位，1/10就表示1秒切10张图片，根据项目的情况可以自己调节
        float frameTime = 1f / 12f;
        for (int i = 0; i < images.Length; i++)
        {
            Sprite sprite = AssetDatabase.LoadAssetAtPath<Sprite>(DataPathToAssetPath(images[i].FullName).Replace("\\", "/"));

            keyFrames[i] = new ObjectReferenceKeyframe();
            keyFrames[i].time = frameTime * Mathf.Max(0, (i +1- Mathf.Pow(1.07f, i)));
            keyFrames[i].value = sprite;
        }
        //动画帧率，30比较合适
        clip.frameRate = 30;

        SerializedObject serializedClip = new SerializedObject( clip );
        AnimationClipSettings clipSettings = new AnimationClipSettings( serializedClip.FindProperty( "m_AnimationClipSettings" ) );
        clipSettings.loopTime = true;
        serializedClip.ApplyModifiedProperties();

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