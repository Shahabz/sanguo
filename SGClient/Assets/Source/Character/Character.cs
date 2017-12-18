using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class Character : MonoBehaviour
{
    // 事件代理
    public delegate void OnEvent( Character character );
    public OnEvent              onMoveEnd;
    public OnEvent              onSP1End;
    public OnEvent              onSP2End;

    //
    public int                  shapeID = 0;
    public float                moveSpeed = 1.5f;
    public int                  defalutDirction = -1;       // 也是当前的数字方向
    public int                  defalutAction = 0;
    public bool                 hideOnDeath = true;
    public bool                 disableOnHide = false;
	public bool					attackOnce = true;
    //    
    public Vector3              targetDirection { get { return _TargetDirection; } }
    public Shape                currShape { get { return _CurrShape; } }
    public int                  currDirection { get { if( _CharAnime == null ) return 0; return _CharAnime.currDirection; } }

    //
    protected CharacterAnimator _CharAnime;
    protected Vector3           _TargetPosition;
    protected Vector3           _TargetDirection;
    protected Animator          _FadeAnime;
    protected short             _MovingState = 0;
    protected Shape             _CurrShape;
    protected SpriteGroup       _SpriteGroup;
    protected int               _CurrDirection;

    // 真实速度
    protected float             _MoveSpeed;

    protected virtual void Awake()
    {
        // 初始化shape表
        ShapeDataCheckInit();
        
        //
        _CharAnime = transform.Find( "Sprite" ).GetComponent<CharacterAnimator>();
        _MoveSpeed = moveSpeed;

        if( shapeID != 0 )
            SetShape( shapeID );
    }

    protected virtual void Start()
    {
        if ( _FadeAnime == null )
            _FadeAnime = GetComponent<Animator>();

        if( defalutDirction >= 0 )
        {
            TurnTo( defalutDirction );
        }
        if( defalutAction > 0 )
        {
            switch( defalutAction )
            {
                case 1:
                    Idle();
                    break;
                case 2:
                    Walk();
                    break;
                case 3:
                    Attack();
                    break;
                case 4:
                    Die();
                    break;
                case 5:
                    SP1();
                    break;
                case 6:
                    SP2();
                    break;
            }
        }
    }

    protected virtual void Update()
    {
        // Debug.DrawLine( transform.position, transform.position + _TargetDirection.normalized );
        if( _MovingState == 1 )
        {
            SetPosition( transform.position + ( _MoveSpeed * Time.deltaTime ) * _TargetDirection );
            Vector3 pos = transform.position;
            pos.z = 0;
            if ( Vector3.Angle( _TargetPosition - pos, _TargetDirection ) > 1f )
            {
                SetPosition( _TargetPosition );
                Stop();
                OnArrive();
            }
        }
        else if( _MovingState == 2 )
        {
            // 反向移动            
            SetPosition( transform.position - ( _MoveSpeed * Time.deltaTime ) * _TargetDirection );
            Vector3 pos = transform.position;
            pos.z = 0;
            if ( Vector3.Angle( pos - _TargetPosition, _TargetDirection ) > 1f )
            {
                SetPosition( _TargetPosition );
                Stop();
                OnArrive();
            }
        }
    }

    // 设置形象
    public void SetShape( int shapeID )
    {
        _CurrShape = GetShape( shapeID );
        _CharAnime.SetShape( _CurrShape );
        ResetForward();
    }

    // 调整朝向
    protected void ResetForward()
    {
        if( _CurrShape == null )
            return;

        if( _CharAnime == null )
            _CharAnime = transform.Find( "Sprite" ).GetComponent<CharacterAnimator>();

        int currAction = (int)_CharAnime.currAction;
        int dir = 0;
        // 根据动作不同数量，进行不同的方向判定
        if( _CurrShape.actionDirNum[currAction] == 0 )
            return;
        if( _CurrShape.actionDirNum[currAction] == 8 )        // 8 方向
        {
            // 先算出偏转角
            float angle = Vector3.Angle( Vector3.up, _TargetDirection );
            if( _TargetDirection.x < 0 )
                angle = 360f - angle;

            dir = Mathf.FloorToInt( ( angle + 45f / 2f ) / 45f );
            if( dir > 7 )
                dir = 0;
        }
        else if( _CurrShape.actionDirNum[currAction] == 4 )   // 4 方向
        {
            /*if( _TargetDirection.x >= 0 && _TargetDirection.y > 0 )
                dir = 1;
            else if( _TargetDirection.x > 0 && _TargetDirection.y <= 0 )
                dir = 3;
            else if( _TargetDirection.x <= 0 && _TargetDirection.y < 0 )
                dir = 5;
            else if( _TargetDirection.x < 0 && _TargetDirection.y >= 0 )
                dir = 7;
            else
                dir = 1;*/
			if( _TargetDirection.x >= 0 && _TargetDirection.y > 0 )
				dir = 0;
			else if( _TargetDirection.x > 0 && _TargetDirection.y <= 0 )
				dir = 2;
			else if( _TargetDirection.x <= 0 && _TargetDirection.y < 0 )
				dir = 4;
			else if( _TargetDirection.x < 0 && _TargetDirection.y >= 0 )
				dir = 6;
			else
				dir = 0;
        }
        else if( _CurrShape.actionDirNum[currAction] == 2 )   // 2 方向  去左右里找能用的
        {
            if( _TargetDirection.x < 0 )
            {
                for( int i = 0; i < 5; i++ )
                {
                    if( ( _CurrShape.action[currAction] & ( 1 << i ) ) != 0 )
                    {
                        dir = i;
                    }
                }
            }
            else
            {
                for( int i = 5; i < 8; i++ )
                {
                    if( ( _CurrShape.action[currAction] & ( 1 << i ) ) != 0 )
                    {
                        dir = i;
                    }
                }
            }
        }

        _CharAnime.Play( (short)dir );
        _CurrDirection = dir;
    }

    // 设置位置，动态调整z值。
    public void SetPosition( Vector3 pos )
    {
        pos.z = pos.y;
        transform.position = pos;
    }

    // 设置速度
    public void SetSpeed( float speed )
    {
        moveSpeed = speed;
        _MoveSpeed = speed;
    }

    // 设置移动动画播放速度
    public void SetWalkAnimeSpeed( float speed )
    {

    }

    /*****************************************************/
    // 角色操作
    /*****************************************************/
    // 移动至
    public virtual void MoveTo( Vector3 position )
    {
        position.z = 0;
        _TargetPosition = position;
        Vector3 pos = transform.position;
        pos.z = 0;
        _TargetDirection = ( _TargetPosition - pos ).normalized;
        _MovingState = 1;

        Walk();
    }

    // 移动向
    public virtual void MoveForward( Vector3 direction, float distance )
    {
        direction.z = 0;
        _TargetDirection = direction.normalized;

        MoveForward( distance );
    }

    // 以当前方向移动一段距离
    public virtual void MoveForward( float distance )
    {
        _TargetPosition = transform.position + _TargetDirection * distance;
        _TargetPosition.z = 0;
        _MovingState = 1;
        Walk();
        ResetForward();
    }

    // 以当前方向后退一段距离
    
    public virtual void MoveBackward( float distance )
    {
        _TargetPosition = transform.position - _TargetDirection * distance;
        _TargetPosition.z = 0;
        _MovingState = 2;
        Walk();
        ResetForward();
    }

    // 停止移动
    public virtual void Stop()
    {        
        _MovingState = 0;
        Idle();
    }

    // 转向
    public void TurnTo( Vector3 direction )
    {
        direction.z = 0;
        _TargetDirection = direction.normalized;
        ResetForward();
    }

    public void TurnTo( int direction )
    {        
        switch( direction )
        {
            case 0:
                TurnTo( new Vector3( 0, 3, 0 ) );
                break;
            case 1:
                TurnTo( new Vector3( 5, 3, 0 ) );
                break;
            case 2:
                TurnTo( new Vector3( 5, 0, 0 ) );
                break;
            case 3:
                TurnTo( new Vector3( 5, -3, 0 ) );
                break;
            case 4:
                TurnTo( new Vector3( 0, -3, 0 ) );
                break;
            case 5:
                TurnTo( new Vector3( -5, -3, 0 ) );
                break;
            case 6:
                TurnTo( new Vector3( -5, 0, 0 ) );
                break;
            case 7:
                TurnTo( new Vector3( -5, 3, 0 ) );
                break;
        }
    }

    // 隐藏
    public void Hide()
    {
        if( _FadeAnime == null )
            return;
        
        _FadeAnime.enabled = true;
        _FadeAnime.SetTrigger( "FadeIn" );
    }

    // 显示
    public void Show()
    {
        if ( _FadeAnime == null )
            return;

        gameObject.SetActive( true );

        _FadeAnime.enabled = true;
        _FadeAnime.SetTrigger( "FadeOut" );
    }

    public void Show( bool instantly )
    {
        if ( _FadeAnime == null )
            return;

        if( instantly )
        {
            _FadeAnime.enabled = false;
            _SpriteGroup.alpha = 1;
        }
        else
        {
            Show();
        }
    }

    public void Hide( bool instantly )
    {
        if ( _FadeAnime == null )
            return;

        if( instantly )
        {
            _FadeAnime.enabled = false;
            _SpriteGroup.alpha = 0;
        }
        else
        {
            Hide();
        }
    }

    /*****************************************************/
    // 原始动作
    /*****************************************************/
    public virtual void Walk()
    {
        _CharAnime.Play( CharacterAnimator.ACTION.WALK );
        ResetForward();
    }

    public virtual void Attack()
    {
        _CharAnime.Play( CharacterAnimator.ACTION.ATTACK );
        ResetForward();
    }

    public virtual void Idle()
    {
        _CharAnime.Play( CharacterAnimator.ACTION.IDLE );
        ResetForward();
    }

    public virtual void Die()
    {
        _CharAnime.Play( CharacterAnimator.ACTION.DIE );
        ResetForward();
    }

    public virtual void SP1()
    {
        _CharAnime.Play( CharacterAnimator.ACTION.SP1 );
        ResetForward();
    }
    public virtual void SP2()
    {
        _CharAnime.Play( CharacterAnimator.ACTION.SP2 );
        ResetForward();
    }

    /*****************************************************/
    // 动作事件
    /*****************************************************/
    // 移动完毕
    protected virtual void OnArrive()
    {
        if ( onMoveEnd != null )
            onMoveEnd( this );
    }

    // 播放完显示
    public virtual void OnShow()
    {
    }

    // 播放完隐藏
    public virtual void OnHide()
    {
        if( disableOnHide )
            gameObject.SetActive( false );
    }

    // 攻击动画触发
    public virtual void OnAttack()
    {
		// 如果只攻击一次
		if( attackOnce )
			Stop();
    }

    // 播放完死亡动画
    public virtual void OnDie()
    {
        if( hideOnDeath )
            Hide();
    }

    public virtual void OnSP1()
    {
        if ( onSP1End != null )
            onSP1End( this );
    }

    public virtual void OnSP2()
    {
        if ( onSP2End != null )
            onSP2End( this );
    }

    /*****************************************************/
    // 加载
    /*****************************************************/
    static GameObject s_Character;
    static Dictionary<int,GameObject>    s_CharactorList = new Dictionary<int,GameObject>();
    protected static GameObject GetCharacterObj( int shapeID )
    {
        Shape shape = GetShape( shapeID );
        if( shape.overridePre == "" )
        {
            if( s_Character == null )
                s_Character = ResourceManager.LoadPrefab( "Character" );

            return Instantiate( s_Character );
        }
        else
        {
            GameObject ch = null;
            if( !s_CharactorList.ContainsKey( shapeID ) )
            {
                ch = ResourceManager.LoadPrefab( shape.overridePre );
                s_CharactorList.Add( shapeID, ch );
            }
            else
                ch = s_CharactorList[shapeID];

            return Instantiate( ch );
        }
    }

    public static void ClearStatic()
    {
        s_Character = null;
        s_CharactorList.Clear();
    }

    public static Character Create( int shapeID )
    {
        return Create<Character>( shapeID );
    }

    public static T Create<T>( int shapeID ) where T : Character
    {
        T ch = GetCharacterObj( shapeID ).AddComponent<T>();
        ch.gameObject.name = "Shape_" + shapeID;
        ch.SetShape( shapeID );

        // 设定初始方向
        ch._TargetDirection = ( Quaternion.AngleAxis( -360f / 8f * ch._CharAnime.currDirection, Vector3.forward ) * Vector3.up ).normalized;
        return ch;
    }



    
    /*****************************************************/
    // Shape表
	// 上	右上		右	右 下	下	左下		左	左上
	// 0 	 0		 1      0             0     0              1        0
	// 01000100
    /*****************************************************/
    public class Shape
    {
        public string   shape;                      // 形象名称，通过这个加载动画文件
        public short[]  action = new short[6];      // 动作，二进制表示8个方向，从0点开始顺时针。
        public short[]  actionDirNum;               // 总共有几个方向
        public short    row;                        // 单位方阵内，一排多少个
        public short    col;                        // 单位方阵内，一列多少个
        public float    shadow;                     // 影子缩放比例
        public Color    color;                      // 染色
        public short    ai;                         // AI的ID
        public string   overridePre;                // 覆盖预制物体
		public string 	abname;						// 所在的abname				
    }

    static Dictionary<int,Shape>      s_ShapeData;
    // 默认形象配置
    static Shape                      s_ShapeDefault;

    // 如果s_ShapeData没有初始化，则去初始化他
    static void ShapeDataCheckInit()
    {        
        if( s_ShapeData == null )
        {
            s_ShapeData = new Dictionary<int,Shape>();
            TableUtil objectTable = new TableUtil();
            objectTable.OpenFromData( "shape.txt" );
            for ( int row = 0; row < objectTable.GetRecordsNum(); row++ )
            {
                Shape shape = new Shape();
                shape.shape = objectTable.GetValue( row, 1 );
                for( int i = 0; i < 6; i++ )
                {
                    shape.action[i] = short.Parse( objectTable.GetValue( row, 2 + i ) );
                }
                shape.row = short.Parse( objectTable.GetValue( row, 8 ) );
                shape.col = short.Parse( objectTable.GetValue( row, 9 ) );
                shape.shadow = float.Parse( objectTable.GetValue( row, 10 ) );
                ColorUtility.TryParseHtmlString( objectTable.GetValue( row, 11 ), out shape.color );
                shape.ai = short.Parse( objectTable.GetValue( row, 12 ) );
                shape.overridePre = objectTable.GetValue( row, 13 );
				shape.abname = objectTable.GetValue( row, 15 );
                s_ShapeData.Add( int.Parse( objectTable.GetValue( row, 0 ) ), shape );
            }
        }
    }

    // 直接获得形象数据
    public static Shape GetShape( int shapeID )
    {
        ShapeDataCheckInit();
        if( s_ShapeData.ContainsKey( shapeID ) )
            return s_ShapeData[shapeID];
        else
            return GetDefaultShape();
    }

    // 该形象一个方阵最多放几个？
    public static int GetShapeNumPerLegion( int shapeID )
    {
        ShapeDataCheckInit();
        
        if( s_ShapeData.ContainsKey( shapeID ) )
            return s_ShapeData[shapeID].row * s_ShapeData[shapeID].col;
        else
            return GetDefaultShape().row * GetDefaultShape().col;
    }

    static Shape GetDefaultShape()
    {
        if( s_ShapeDefault == null )
        {
            s_ShapeDefault = new Shape();
            s_ShapeDefault.shape = "Default";
            s_ShapeDefault.action = new short[6]{ 255, 255, 255, 255, 255, 255 };
            s_ShapeDefault.actionDirNum = new short[6]{ 8, 8, 8, 8, 8, 8 };
            s_ShapeDefault.row = 3;
            s_ShapeDefault.col = 3;
        }

        return s_ShapeDefault;
    }
}
