using UnityEngine;
using System.Collections;

public class AnimeGrouprParam : MonoBehaviour
{
    public Animator[]      animatorGroup;
    public void Set( string param )
    {
        string[] pa = param.Split( new char[]{ ':',',' } );

        int type = int.Parse( pa[0] );
        if( type < 0 || type > 3 )
            return;
		
        switch( type )
        {
            case 0:
                {
                    int par = int.Parse( pa[2] );
                    for( int i = 0; i < animatorGroup.Length; i++ )
                    {
                        animatorGroup[i].SetInteger( pa[1], par );
                    }
                    break;
                }
            case 1:
                {
                    float par = float.Parse( pa[2] );
                    for( int i = 0; i < animatorGroup.Length; i++ )
                    {
                        animatorGroup[i].SetFloat( pa[1], par );
                    }
                    break;
                }
            case 2:
                {
                    bool par = int.Parse( pa[2] ) == 0 ? false : true;
                    for( int i = 0; i < animatorGroup.Length; i++ )
                    {
                        animatorGroup[i].SetBool( pa[1], par );
                    }
                    break;
                }
			case 3:
				{
					for( int i = 0; i < animatorGroup.Length; i++ )
					{
						animatorGroup[i].SetTrigger( pa[1] );
					}
					break;
				}
        }

    }
}
