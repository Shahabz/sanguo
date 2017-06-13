using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LockStepManager : MonoBehaviour {

    public static readonly int FirstLockStepTurnID = 0;
    public static LockStepManager Instance;
    public int LockStepTurnID = FirstLockStepTurnID;

    // 帧同步回合：帧同步回合可以由多个游戏回合组成。玩家在一个帧同步回合执行一个动作。帧同步回合长度会根据性能调，比如为200ms。
    private int initialLockStepTurnLength = 200;

    // 游戏回合：游戏回合就是游戏逻辑的更新。每个帧同步回合拥有的游戏回合次数是由性能控制的。比如为50ms，也就是每次帧同步回合有4次游戏回合。也就是每秒有20次游戏回合。
    private int initialGameFrameTurnLength = 50;


    public int LockstepTurnLength = 200;
    public int GameFrameTurnLength = 50;

    // 当前帧同步中游戏的帧数
    private int GameFrame = 0;

    // 从上一次GameFrame调用以来已经过去的累积时间以毫秒为单位
    private int AccumilatedTime = 0;

    void Start ()
    {
        enabled = false;
        Instance = this;
    }
	
	void Update ()
    {
        AccumilatedTime = AccumilatedTime + (int)( Time.deltaTime * 1000 );

        // 如果FPS太慢，我们可能需要多次更新游戏
        while ( AccumilatedTime > GameFrameTurnLength )
        {
            GameFrameTurn();
            AccumilatedTime = AccumilatedTime - GameFrameTurnLength;
        }
    }
    
    // 执行一个游戏回合
    private void GameFrameTurn()
    {
        LuaFun.fightFrameTurn.Call();
    }

}
