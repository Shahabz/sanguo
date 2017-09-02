using UnityEngine;
using System.Collections;

public class CharController : MonoBehaviour
{
    public Character[]   character;

    public void CharTurnTo( string param )
    {
        string[] pa = param.Split( new char[]{ ':', ',' } );
        if( pa.Length < 2 )
            return;

        int index = int.Parse( pa[0] );
        if( index < 0 || index >= character.Length )
            return;

        int direction = int.Parse( pa[1] );
        character[index].TurnTo( direction );
    }

	public void CharWalk( string param )
    {
        string[] pa = param.Split( new char[]{ ':', ',' } );
        if( pa.Length < 1 )
            return;

        int index = int.Parse( pa[0] );
        if( index < 0 || index >= character.Length )
            return;

        character[index].Walk();
    }

	public void CharStop( string param )
    {
        string[] pa = param.Split( new char[]{ ':', ',' } );
        if( pa.Length < 1 )
            return;

        int index = int.Parse( pa[0] );
        if( index < 0 || index >= character.Length )
            return;

        character[index].Stop();
    }

	public void CharShow( string param )
    {
        string[] pa = param.Split( new char[]{ ':', ',' } );
        if( pa.Length < 1 )
            return;

        int index = int.Parse( pa[0] );
        if( index < 0 || index >= character.Length )
            return;

        character[index].Show();
    }

	public void CharHide( string param )
    {
        string[] pa = param.Split( new char[]{ ':', ',' } );
        if( pa.Length < 1 )
            return;

        int index = int.Parse( pa[0] );
        if( index < 0 || index >= character.Length )
            return;

        character[index].Hide();
    }

	public void CharAttack( string param )
    {
        string[] pa = param.Split( new char[]{ ':', ',' } );
        if( pa.Length < 1 )
            return;

        int index = int.Parse( pa[0] );
        if( index < 0 || index >= character.Length )
            return;

        //FightCharacter fightcharacter = character[index] as FightCharacter;
        //fightcharacter.Attack();
    }

	public void CharMoveTo( string param )
    {
        string[] pa = param.Split( new char[]{ ':', ',' } );
        if( pa.Length < 3 )
            return;

        int index = int.Parse( pa[0] );
        if( index < 0 || index >= character.Length )
            return;

        float x = float.Parse( pa[1] );
        float y = float.Parse( pa[2] );
        
        //FightCharacter fightcharacter = character[index] as FightCharacter;
        //fightcharacter.MoveTo( new Vector3( x, y, 0 ) );
    }

    public void CharAttackAt( string param )
    {
        string[] pa = param.Split( new char[]{ ':', ',' } );
        if( pa.Length < 3 )
            return;

        int index = int.Parse( pa[0] );
        if( index < 0 || index >= character.Length )
            return;

        float x = float.Parse( pa[1] );
        float y = float.Parse( pa[2] );
        
        //FightCharacter fightcharacter = character[index] as FightCharacter;
        //fightcharacter.AttackAt( new Vector3( x, y, 0 ) );
    }
}
