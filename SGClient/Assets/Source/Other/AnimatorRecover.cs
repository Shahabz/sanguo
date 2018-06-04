using UnityEngine;
using System.Collections;

// 注意* 一定要放到Animator前面，一定，一定，一定！！！
[RequireComponent(typeof(Animator))]
public class AnimatorRecover : MonoBehaviour
{
    public enum ParamType
    {
        BOOL,
        INT,
        FLOAT
    }
    public ParamType    paramType;
    public string       paramName;

    Animator    _Animator;
    bool        _CurrBool;
    int         _CurrInt;
    float       _CurrFloat;
    
    void Start()
    {
        _Animator = GetComponent<Animator>();
    }

    void OnDisable()
    {
        if( _Animator == null )
            return;

        switch( paramType )
        {
            case ParamType.BOOL:
                _CurrBool = _Animator.GetBool( paramName );
                break;
                
            case ParamType.INT:
                _CurrInt = _Animator.GetInteger( paramName );
                break;

            case ParamType.FLOAT:
                _CurrFloat = _Animator.GetFloat( paramName );
                break;
        }
    }

    void OnEnable()
    {
        if( _Animator == null )
            return;

        switch( paramType )
        {
            case ParamType.BOOL:
                _Animator.SetBool( paramName, _CurrBool );
                break;
                
            case ParamType.INT:
                _Animator.SetInteger( paramName, _CurrInt );
                break;

            case ParamType.FLOAT:
                _Animator.SetFloat( paramName, _CurrFloat );
                break;
        }
    }
}
