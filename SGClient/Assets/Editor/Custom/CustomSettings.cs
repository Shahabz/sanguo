using UnityEngine;
using System;
using System.Collections.Generic;
using LuaInterface;

using BindType = ToLuaMenu.BindType;
using System.Reflection;
using UnityEngine.UI;
using UnityEngine.EventSystems;

public static class CustomSettings
{
    public static string saveDir = Application.dataPath + "/Source/Generate/";    
    public static string toluaBaseType = Application.dataPath + "/ToLua/BaseType/";    

    //导出时强制做为静态类的类型(注意customTypeList 还要添加这个类型才能导出)
    //unity 有些类作为sealed class, 其实完全等价于静态类
    public static List<Type> staticClassTypes = new List<Type>
    {        
        typeof(UnityEngine.Application),
        typeof(UnityEngine.Time),
        typeof(UnityEngine.Screen),
        typeof(UnityEngine.SleepTimeout),
        typeof(UnityEngine.Input),
        typeof(UnityEngine.Resources),
        typeof(UnityEngine.Physics),
        typeof(UnityEngine.RenderSettings),
        typeof(UnityEngine.QualitySettings),
        typeof(UnityEngine.GL),
    };

    //附加导出委托类型(在导出委托时, customTypeList 中牵扯的委托类型都会导出， 无需写在这里)
    public static DelegateType[] customDelegateList = 
    {        
        _DT(typeof(Action)),                
        _DT(typeof(UnityEngine.Events.UnityAction)),
        _DT(typeof(System.Predicate<int>)),
        _DT(typeof(System.Action<int>)),
        _DT(typeof(System.Comparison<int>)),
    };

    //在这里添加你要导出注册到lua的类型列表
    public static BindType[] customTypeList =
    {                
        //------------------------为例子导出--------------------------------
        //_GT(typeof(TestEventListener)),
        //_GT(typeof(TestProtol)),
        //_GT(typeof(TestAccount)),
        //_GT(typeof(Dictionary<int, TestAccount>)).SetLibName("AccountMap"),
        //_GT(typeof(KeyValuePair<int, TestAccount>)),    
        //_GT(typeof(TestExport)),
        //_GT(typeof(TestExport.Space)),
        //-------------------------------------------------------------------        
                
        _GT(typeof(Debugger)).SetNameSpace(null),        

#if USING_DOTWEENING
        _GT(typeof(DG.Tweening.DOTween)),
        _GT(typeof(DG.Tweening.Tween)).SetBaseType(typeof(System.Object)).AddExtendType(typeof(DG.Tweening.TweenExtensions)),
        _GT(typeof(DG.Tweening.Sequence)).AddExtendType(typeof(DG.Tweening.TweenSettingsExtensions)),
        _GT(typeof(DG.Tweening.Tweener)).AddExtendType(typeof(DG.Tweening.TweenSettingsExtensions)),
        _GT(typeof(DG.Tweening.LoopType)),
        _GT(typeof(DG.Tweening.PathMode)),
        _GT(typeof(DG.Tweening.PathType)),
        _GT(typeof(DG.Tweening.RotateMode)),
        _GT(typeof(Component)).AddExtendType(typeof(DG.Tweening.ShortcutExtensions)),
        _GT(typeof(Transform)).AddExtendType(typeof(DG.Tweening.ShortcutExtensions)),
        _GT(typeof(Light)).AddExtendType(typeof(DG.Tweening.ShortcutExtensions)),
        _GT(typeof(Material)).AddExtendType(typeof(DG.Tweening.ShortcutExtensions)),
        _GT(typeof(Rigidbody)).AddExtendType(typeof(DG.Tweening.ShortcutExtensions)),
        _GT(typeof(Camera)).AddExtendType(typeof(DG.Tweening.ShortcutExtensions)),
        _GT(typeof(AudioSource)).AddExtendType(typeof(DG.Tweening.ShortcutExtensions)),
        //_GT(typeof(LineRenderer)).AddExtendType(typeof(DG.Tweening.ShortcutExtensions)),
        //_GT(typeof(TrailRenderer)).AddExtendType(typeof(DG.Tweening.ShortcutExtensions)),    
#else
                                         
        _GT(typeof(Component)),
        _GT(typeof(Transform)),
        _GT(typeof(Material)),
        //_GT(typeof(Light)),
        _GT(typeof(Rigidbody)),
        _GT(typeof(Camera)),
        _GT(typeof(AudioSource)),
        //_GT(typeof(LineRenderer))
        //_GT(typeof(TrailRenderer))
#endif
      
        _GT(typeof(Behaviour)),
        _GT(typeof(MonoBehaviour)),        
        _GT(typeof(GameObject)),
        _GT(typeof(TrackedReference)),
        _GT(typeof(Application)),
        _GT(typeof(Physics)),
        _GT(typeof(Collider)),
        _GT(typeof(Time)),        
        _GT(typeof(Texture)),
        _GT(typeof(Texture2D)),
        _GT(typeof(Shader)),        
        _GT(typeof(Renderer)),
        _GT(typeof(WWW)),
        _GT(typeof(Screen)),        
        _GT(typeof(CameraClearFlags)),
        _GT(typeof(AudioClip)),        
        _GT(typeof(AssetBundle)),
        _GT(typeof(ParticleSystem)),
        _GT(typeof(AsyncOperation)).SetBaseType(typeof(System.Object)),        
        _GT(typeof(LightType)),
        _GT(typeof(SleepTimeout)),
#if UNITY_5_3_OR_NEWER
		_GT(typeof(UnityEngine.Experimental.Director.Playable)),
#endif
        _GT(typeof(Animator)),
        _GT(typeof(Input)),
        _GT(typeof(KeyCode)),
        _GT(typeof(SkinnedMeshRenderer)),
        _GT(typeof(Space)),      
       

        _GT(typeof(MeshRenderer)),
#if !UNITY_5_4_OR_NEWER
        _GT(typeof(ParticleEmitter)),
        _GT(typeof(ParticleRenderer)),
        _GT(typeof(ParticleAnimator)), 
#endif
                              
        _GT(typeof(BoxCollider)),
        _GT(typeof(MeshCollider)),
        _GT(typeof(SphereCollider)),        
        _GT(typeof(CharacterController)),
        _GT(typeof(CapsuleCollider)),
        
        _GT(typeof(Animation)),        
        _GT(typeof(AnimationClip)).SetBaseType(typeof(UnityEngine.Object)),        
        _GT(typeof(AnimationState)),
        _GT(typeof(AnimationBlendMode)),
        _GT(typeof(QueueMode)),  
        _GT(typeof(PlayMode)),
        _GT(typeof(WrapMode)),

        _GT(typeof(QualitySettings)),
        _GT(typeof(RenderSettings)),                                                   
        _GT(typeof(BlendWeights)),           
        _GT(typeof(RenderTexture)),
        _GT(typeof(Resources)),
        _GT(typeof(UnityEngine.SceneManagement.SceneManager)),

        // unity2d
        _GT(typeof(SpriteRenderer)),
        _GT(typeof(Motion)),
        _GT(typeof(YieldInstruction)),
        _GT(typeof(LayoutGroup)),
        _GT(typeof(Image)),
        _GT(typeof(Sprite)),

        // BEGIN 自定义
        _GT(typeof(eye)),
        _GT(typeof(Utils)),
        _GT(typeof(Const)),
        _GT(typeof(Global)),
        _GT(typeof(ShareData)),
        _GT(typeof(ByteBuffer)),
         _GT(typeof(FileUtil)),
        _GT(typeof(StringUtil)),
        _GT(typeof(TableUtil)),
        _GT(typeof(LogUtil)),
        _GT(typeof(IniUtil)),
        _GT(typeof(PathUtil)),

        _GT(typeof(GameManager)),
        _GT(typeof(ResourceManager)),
        _GT(typeof(ObjectPoolManager)),
        _GT(typeof(NetworkManager)),
        _GT(typeof(Localization)),
        _GT(typeof(DeviceHelper)),
        _GT(typeof(OpenUDIDPhone.OpenUDID)),
		_GT(typeof(UnitMove)),
        _GT(typeof(ProgressSprite)),

        // UI
        _GT(typeof(UIMod)),
        _GT(typeof(UISystem)),
        _GT(typeof(UIManager)),
        _GT(typeof(Selectable)),
        _GT(typeof(Button)),
        _GT(typeof(UIButton)),
        _GT(typeof(UIButtonAgent)),
        _GT(typeof(InputField)),
        _GT(typeof(UIInputField)),
        _GT(typeof(UIInputFieldSubmit)),
        _GT(typeof(RectTransform)),
        _GT(typeof(Slider)),
        _GT(typeof(UISlider)),
        _GT(typeof(ScrollRect)),
        _GT(typeof(UIScrollRect)),
        _GT(typeof(UIScrollBase)),
        _GT(typeof(UIScrollRing)),
        _GT(typeof(UIScrollView)),
        _GT(typeof(UIDragSensor)),
        _GT(typeof(Text)),
        _GT(typeof(UIText)),
        _GT(typeof(GridLayoutGroup)),
        _GT(typeof(LayoutElement)),
        _GT(typeof(UITween)),
        _GT(typeof(UITweenRectPosition)),
        _GT(typeof(UITweenFade)),
        _GT(typeof(UITweenColor)),
        _GT(typeof(UITweenLocalPosition)),
        _GT(typeof(UITweenLocalPositionBetween)),
        _GT(typeof(UITweenScale)),
        _GT(typeof(UITweenPosition)),
        _GT(typeof(UITweenSizeDelta)),
        _GT(typeof(UITweenCityBuilding)),
        _GT(typeof(MaskableGraphic)),
        _GT(typeof(Graphic)),
        _GT(typeof(TextGenerator)),
        _GT(typeof(UICharInfo)),
        _GT(typeof(TextGenerationSettings)),
        _GT(typeof(UITextTimeCountdown)),
        _GT(typeof(UITextTimeCountdown.DirectType)),
        _GT(typeof(UITextNumCountdown)),
        _GT(typeof(UITextUnderline)),
        _GT(typeof(UIProgress)),
        _GT(typeof(FXTypeWriter)),
        _GT(typeof(TypeWriter)),
        _GT(typeof(UITextAutoOmit)),
        _GT(typeof(UIPopWindow)),
        _GT(typeof(GUIUtility)),
        _GT(typeof(UIGradient)),
        _GT(typeof(Color32)),
        _GT(typeof(Shake)),
        _GT(typeof(UIHighLightMask)),

        // Gizmo
		_GT(typeof(GizmoPlane)),
        _GT(typeof(GizmoBezierLine)),
        _GT(typeof(Bezier)),
        _GT(typeof(BezierMove)),

        // DragonBones
        _GT(typeof(DragonBones.UnityFactory)),
        _GT(typeof(DragonBones.AnimationTimelineState)),
        _GT(typeof(DragonBones.AnimationFrameData)),
        _GT(typeof(DragonBones.AnimationState)),
        _GT(typeof(DragonBones.AnimationFadeOutMode)),
        _GT(typeof(DragonBones.Animation)),
        _GT(typeof(DragonBones.AnimationData)),
        _GT(typeof(DragonBones.AnimationConfig)),
        _GT(typeof(DragonBones.ActionData)),
        _GT(typeof(DragonBones.ActionType)),
        _GT(typeof(DragonBones.Armature)),
        _GT(typeof(DragonBones.ArmatureData)),
        _GT(typeof(DragonBones.ArmatureType)),
        _GT(typeof(DragonBones.Bone)),
        _GT(typeof(DragonBones.BoneData)),
        _GT(typeof(DragonBones.BoneFrameData)),
        _GT(typeof(DragonBones.BoneTimelineData)),
        _GT(typeof(DragonBones.BoneTimelineState)),
        _GT(typeof(DragonBones.DragonBones)),
        _GT(typeof(DragonBones.DragonBonesData)),
        _GT(typeof(DragonBones.Transform)),
        _GT(typeof(DragonBones.TransformObject)),
        _GT(typeof(DragonBones.BaseObject)),
        _GT(typeof(DragonBones.EventObject)),
        _GT(typeof(DragonBones.UnityArmatureComponent)),
        // END

        _GT(typeof(CityCamera)),
    };

    public static List<Type> dynamicList = new List<Type>()
    {
        typeof(MeshRenderer),
#if !UNITY_5_4_OR_NEWER
        typeof(ParticleEmitter),
        typeof(ParticleRenderer),
        typeof(ParticleAnimator),
#endif

        typeof(BoxCollider),
        typeof(MeshCollider),
        typeof(SphereCollider),
        typeof(CharacterController),
        typeof(CapsuleCollider),

        typeof(Animation),
        typeof(AnimationClip),
        typeof(AnimationState),

        typeof(BlendWeights),
        typeof(RenderTexture),
        typeof(Rigidbody),
    };

    //重载函数，相同参数个数，相同位置out参数匹配出问题时, 需要强制匹配解决
    //使用方法参见例子14
    public static List<Type> outList = new List<Type>()
    {
        
    };

    public static BindType _GT(Type t)
    {
        return new BindType(t);
    }

    public static DelegateType _DT(Type t)
    {
        return new DelegateType(t);
    }    
}
