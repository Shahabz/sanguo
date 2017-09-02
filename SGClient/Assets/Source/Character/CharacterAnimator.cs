using UnityEngine;
using System.Collections;
using System.Collections.Generic;

// 对角色动画进行控制，角色形象的数据也在其内
public class CharacterAnimator : MonoBehaviour
{	
	public enum ACTION
	{
		IDLE = 0,
		WALK = 1,
		ATTACK = 2,
		DIE = 3,
		SP1 = 4,
        SP2 = 5,
	};
	
    //
	public short     		currDirection{ get{ return _CurrDirection; } }
	public ACTION			currAction{ get{ return _CurrAction; } }

    //
	Animator		        _Animator;
	short    		        _CurrDirection;
	ACTION			        _CurrAction = ACTION.IDLE;
	static int		        _nDirectionID = -1;
	static int[]	        _nActionID = { 0, 0, 0, 0, 0, 0 };
	Character	            _Character;

    Coroutine               _PlayOnStart;

        
    void Init()
    {
        if ( _Animator == null )
            _Animator = transform.GetComponent<Animator>();

        if( _nDirectionID == -1 )
        {
		    _nDirectionID = Animator.StringToHash( "Direction" );
		    _nActionID[0] = Animator.StringToHash( "IDLE" );
		    _nActionID[1] = Animator.StringToHash( "WALK" );
		    _nActionID[2] = Animator.StringToHash( "ATTACK" );
		    _nActionID[3] = Animator.StringToHash( "DIE" );
		    _nActionID[4] = Animator.StringToHash( "SP1" );
		    _nActionID[5] = Animator.StringToHash( "SP2" );
        }
    }

    IEnumerator PlayOnStart()
    {
        yield return new WaitForFixedUpdate();

        _Animator.SetInteger( _nDirectionID, (int)_CurrDirection );
		_Animator.SetTrigger( _nActionID[ (int)_CurrAction ] );
    }

    void Start()
    {
        _Character = GetComponentInParent<Character>();

        StartCoroutine( PlayOnStart() );
    }

    void OnEnable()
    {
        if( _Animator != null )
        {
            _Animator.SetInteger( _nDirectionID, (int)_CurrDirection );
		    _Animator.SetTrigger( _nActionID[ (int)_CurrAction ] );
        }
    }

    // 设置移动的播放速度
    public void SetMoveAnimeSpeed( float speed )
    {

    }

    // 设置形象
    public void SetShape( Character.Shape shape )
    {
        Init();

        shape.actionDirNum = new short[6];

        AnimatorOverrideController overrideController = new AnimatorOverrideController();
        overrideController.runtimeAnimatorController = _Animator.runtimeAnimatorController;
        
        // 先看是否有此动作，然后加载
        for( int i = 0; i < 8; i++ )
        {
            for( int j = 0; j < 6; j++ )
            {
                // 枚举转成字符串，当做动作名
                string actionName = ( (ACTION)j ).ToString();

                // 读取动画文件，并累计动作数量
                if( ( shape.action[j] & ( 1 << i ) ) != 0 )
                {
                    AnimationClip clip = ResourceManager.LoadCharactorAnime( shape.shape + "_" + actionName + "_" + i, shape.shape );
                    if( clip == null )
                        continue;

                    clip.name = actionName + "_" + i;
                    overrideController[ actionName + "_" + i ] = clip;
                    shape.actionDirNum[j] += 1;
                }
            }
        }

        _Animator.runtimeAnimatorController = overrideController;

        // 应用染色
        _Animator.GetComponent<SpriteRenderer>().color = shape.color;
    }

    public void Play( short direction )
    {
        Play( direction, _CurrAction );
    }

    public void Play( ACTION action )
    {
        Play( _CurrDirection, action );
    }

	public void Play( short direction, ACTION action )
	{
        // 如果动画系统都没有加载，则等到下帧播放
        if( _Character == null )
        {            
            _CurrDirection = direction;
            _CurrAction = action;
            return;
        }

		if( direction == _CurrDirection && action == _CurrAction )
			return;
        
        _CurrDirection = direction;
        _CurrAction = action;

		_Animator.SetInteger( _nDirectionID, (int)direction );
        for( int i = 0; i < _nActionID.Length; i++ )
        {
            if( i == (int)action )
                _Animator.SetTrigger( _nActionID[i] );
            else
                _Animator.ResetTrigger( _nActionID[i] );
        }
		
	}
	
	public void OnAttack()
	{
		_Character.OnAttack();
	}

	public void OnDie()
	{
		_Character.OnDie();
	}

    public void OnSP1()
    {
		_Character.OnSP1();
    }

    public void OnSP2()
    {
		_Character.OnSP2();
    }
}
