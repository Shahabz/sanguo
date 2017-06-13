using UnityEngine;
using System.Collections;
using UnityEngine.UI;
using UnityEngine.EventSystems;

public class UIButton : Button
{
	public int 			controlID = 0;
    public int          clickInterval = 0;
	public UIMod 		uiMod;
	public int	        audioID = 0;

    Coroutine           _ClickInterval;
    int                 _ClickEnableTime;

	protected override void Start()
	{
		base.Start();
		// 挂载uiMod
		if( uiMod == null )
		{
			uiMod = gameObject.GetComponentInParent<UIMod>();
		}
	}

	public override void OnPointerClick( PointerEventData eventData )
	{
		base.OnPointerClick( eventData );
        if( !interactable || uiMod == null )
            return;

        if ( clickInterval != 0 )
        {
            _ClickEnableTime = System.DateTime.Now.Second + clickInterval;
            _ClickInterval = StartCoroutine( ClickInterval() );
            interactable = false;
        }
        LogUtil.GetInstance().WriteGame( "UIEVENT.CLICK["+uiMod.uiName+"]" + controlID );
		object[] param = { UIEVENT.CLICK, controlID, 0 };
		uiMod.OnEvent( param );
        if (audioID == 0)
        {
            //fruit.audioManager.Play(45);
        }
        else
        {
            //fruit.audioManager.Play(audioID);
        }
        

        //if( clickSound != null )
        //{
        //    fruit.audioManager.PlayAudio(AudioManager.AudioType.UI,  clickSound );
        //}

	}
	
	public override void OnPointerDown( PointerEventData eventData )
	{
        base.OnPointerDown( eventData );
        if ( !interactable || uiMod == null )
            return;

		object[] param = { UIEVENT.PRESS, controlID, 0 };
		uiMod.OnEvent( param );
	}
	
	
	public override void OnPointerUp( PointerEventData eventData )
	{
		base.OnPointerUp( eventData );
        if ( !interactable || uiMod == null )
            return;

		object[] param = { UIEVENT.PRESS, controlID, 1 };
		uiMod.OnEvent( param );
	}

    IEnumerator ClickInterval()
    {
        yield return new WaitForSeconds( _ClickEnableTime - System.DateTime.Now.Second );

        _ClickEnableTime = 0;
        interactable = true;
    }

    protected override void OnEnable()
    {
        base.OnEnable();

        if( _ClickEnableTime != 0 )
        {
            if( System.DateTime.Now.Second > _ClickEnableTime )
            {
                interactable = true;
                _ClickEnableTime = 0;
            }
            else
                _ClickInterval = StartCoroutine( ClickInterval() );
        }
    }

    protected override void OnDisable()
    {
        base.OnDisable();

        if( _ClickInterval != null )
            StopCoroutine( _ClickInterval );
    }
}
