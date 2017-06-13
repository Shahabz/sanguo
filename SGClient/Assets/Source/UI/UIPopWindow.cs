using UnityEngine;
using System.Collections;
using System.Collections.Generic;

// 最多显示objPool.Length个，必须在Lua层做缓冲队列
// GameObject*必须*嵌套3层，并用UITween做动画，参考NotifyDlg
// 第一层为-位移+淡入层。第二层为-展开层。第三层为-收缩层。
public class UIPopWindow : MonoBehaviour
{
    public UIMod        uiMod;
    public int          controlID = 0;
    public float        lifeTime = 3f;      // 每个窗口的持续时间
    public float        space = 168;        // 位移间隔

    public int          currCount { get{ return _Queue.Count; } }

    // 物体池，第一个为模板物体
    [SerializeField]
    GameObject[]        objPool;

    // 当前正在显示的队列
    List<Transform>    _Queue = new List<Transform>();
    Dictionary<Transform,Coroutine>    _QueueDelay = new Dictionary<Transform,Coroutine>();

    // 隐藏时还原所有
    void OnDisable()
    {
        for( int i = 0; i < objPool.Length; i++ )
        {
            // 如果是空的，则实例化一个，然后返回
            if( objPool[i] != null )
            {
                objPool[i].gameObject.SetActive( false );
            }
        }

        _QueueDelay.Clear();
        _Queue.Clear();
    }

    public Transform Pop()
    {
        UITween tween;
        // 如果队列，当前是满的状态，则加速队列末尾消失速度。Lua层将数据插入缓冲队列
        if( _Queue.Count == objPool.Length )
        {
            tween = _Queue[0].transform.GetChild( 0 ).GetChild( 0 ).GetComponent<UITween>();
            
            // 如果已在收缩中直接返回，否则收缩
            if( tween.isPlaying )
                return null;

            StopCoroutine( _QueueDelay[tween.transform] );
            tween.Play( true );

            return null;
        }

        GameObject obj = objPool[0];

        // 找一个闲置的
        for( int i = 0; i < objPool.Length; i++ )
        {
            // 如果是空的，则实例化一个，然后返回
            if( objPool[i] == null )
            {
                objPool[i] = GameObject.Instantiate( objPool[0] );
                obj = objPool[i];

                obj.transform.SetParent( objPool[0].transform.parent );
                obj.transform.localScale = Vector3.one;
                obj.GetComponent<RectTransform>().offsetMax = objPool[0].GetComponent<RectTransform>().offsetMax;
                obj.GetComponent<RectTransform>().offsetMin = objPool[0].GetComponent<RectTransform>().offsetMin;
                break;
            }

            // 找一个隐藏的
            if( objPool[i].gameObject.activeSelf == false )
            {
                obj = objPool[i];
                break;
            }
        }
        obj.gameObject.SetActive( true );
        // 还原
        obj.transform.localPosition = Vector3.zero;
        obj.transform.localScale = Vector3.one;
        obj.GetComponent<CanvasGroup>().alpha = 1;

        // 播放展开动画
        tween = obj.transform.GetChild( 0 ).GetComponent<UITween>();
        tween.Play( true );
        
        // 设定收缩时间，回调，并播放
        tween = obj.transform.GetChild( 0 ).GetChild( 0 ).GetComponent<UITween>();
        tween.ToInit();
        tween.onFinish = OnFinish;
        _QueueDelay.Add( tween.transform, StartCoroutine( DelayShrink( tween ) ) );

        // 其他现存窗口播放位移动画
        for( int i = 0; i < _Queue.Count; i++ )
        {
            UITweenLocalPosition tweenPos = _Queue[i].GetComponent<UITweenLocalPosition>();
            tweenPos.from = _Queue[i].localPosition;
            tweenPos.to = new Vector3( 0, space * ( _Queue.Count - i  ), 0 );
            tweenPos.ToInit();
            tweenPos.Play( true );
            
            UITweenFade tweenFade = _Queue[i].GetComponent<UITweenFade>();
            tweenFade.from = _Queue[i].GetComponent<CanvasGroup>().alpha;
            tweenFade.to = 0.75f + ( 1f - (float)( _Queue.Count - i ) / (float)( objPool.Length - 1 ) ) * 0.25f;
            tweenFade.ToInit();
            tweenFade.Play( true );

            UITweenScale tweenScale = _Queue[i].GetComponent<UITweenScale>();
            tweenScale.from = _Queue[i].transform.localScale;
            tweenScale.to = ( 0.75f + ( 1f - (float)( _Queue.Count - i ) / (float)( objPool.Length - 1 ) ) * 0.25f ) * Vector3.one;
            tweenScale.ToInit();
            tweenScale.Play( true );
        }

        // 返回最内层
        _Queue.Add( obj.transform );
        return obj.transform.GetChild( 0 ).GetChild( 0 );
    }

    // 收缩完毕
    void OnFinish( GameObject obj, int value )
    {
        Transform trans = obj.transform.parent.parent;
        // 删除
        _Queue.Remove( trans );
        _QueueDelay.Remove( obj.transform );
        trans.gameObject.SetActive( false );

        if( uiMod != null )
        {
            object[] param = { UIEVENT.PUSHEVENT, controlID, 0 };
		    uiMod.OnEvent( param );
        }
    }

    // 延时收缩
    IEnumerator DelayShrink( UITween tween )
    {
        yield return new WaitForSeconds( lifeTime );

        tween.Play( true );
    }
}
