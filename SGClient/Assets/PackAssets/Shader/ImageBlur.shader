// Upgrade NOTE: replaced 'mul(UNITY_MATRIX_MVP,*)' with 'UnityObjectToClipPos(*)'

Shader "Custom/ImageBlur"
{
	Properties
	{
		[PerRendererData] _MainTex ("Sprite Texture", 2D) = "white" {}
		_Color ("Tint", Color) = (1,1,1,1)
		
		_StencilComp ("Stencil Comparison", Float) = 8
		_Stencil ("Stencil ID", Float) = 0
		_StencilOp ("Stencil Operation", Float) = 0
		_StencilWriteMask ("Stencil Write Mask", Float) = 255
		_StencilReadMask ("Stencil Read Mask", Float) = 255
 
		_ColorMask ("Color Mask", Float) = 15
        //--------------add------------------
        _Distance ("Distance", Float) = 0.015
        //--------------add------------------
	}
 
	SubShader
	{
		Tags
		{ 
			"Queue"="Transparent" 
			"IgnoreProjector"="True" 
			"RenderType"="Transparent" 
			"PreviewType"="Plane"
			"CanUseSpriteAtlas"="True"
		}
		
		Stencil
		{
			Ref [_Stencil]
			Comp [_StencilComp]
			Pass [_StencilOp] 
			ReadMask [_StencilReadMask]
			WriteMask [_StencilWriteMask]
		}
 
		Cull Off
		Lighting Off
		ZWrite Off
		ZTest [unity_GUIZTestMode]
		Fog { Mode Off }
		Blend SrcAlpha OneMinusSrcAlpha
		ColorMask [_ColorMask]
 
		Pass
		{
		CGPROGRAM
			#pragma vertex vert
			#pragma fragment frag
			#include "UnityCG.cginc"
			
			struct appdata_t
			{
				float4 vertex   : POSITION;
				float4 color    : COLOR;
				float2 texcoord : TEXCOORD0;
			};
 
			struct v2f
			{
				float4 vertex   : SV_POSITION;
				fixed4 color    : COLOR;
				half2 texcoord  : TEXCOORD0;
				half2 taps[8] : TEXCOORD1; 
			};
			
			fixed4 _Color;
 
			sampler2D _MainTex;
            //--------------add------------------
            float _Distance;
			half4 _MainTex_TexelSize;
            //--------------add------------------
 
			v2f vert(appdata_t IN)
			{
				v2f OUT;
				OUT.vertex = UnityObjectToClipPos(IN.vertex);
				OUT.texcoord = IN.texcoord;
#ifdef UNITY_HALF_TEXEL_OFFSET
				OUT.vertex.xy += (_ScreenParams.zw-1.0)*float2(-1,1);
#endif
				OUT.color = IN.color * _Color;

				OUT.taps[0] = IN.texcoord + _MainTex_TexelSize.xy * half2( 0, _Distance );
				OUT.taps[1] = IN.texcoord + _MainTex_TexelSize.xy * half2( _Distance, _Distance );
				OUT.taps[2] = IN.texcoord + _MainTex_TexelSize.xy * half2( _Distance, 0 );
				OUT.taps[3] = IN.texcoord + _MainTex_TexelSize.xy * half2( _Distance, -_Distance );
				OUT.taps[4] = IN.texcoord + _MainTex_TexelSize.xy * half2( 0, -_Distance );
				OUT.taps[5] = IN.texcoord + _MainTex_TexelSize.xy * half2( -_Distance, -_Distance );
				OUT.taps[6] = IN.texcoord + _MainTex_TexelSize.xy * half2( -_Distance, 0 );
				OUT.taps[7] = IN.texcoord + _MainTex_TexelSize.xy * half2( -_Distance, _Distance );

				return OUT;
			}

			fixed4 frag(v2f IN) : SV_Target
			{
//				//half4 color = tex2D(_MainTex, IN.texcoord) * IN.color;
//				//clip (color.a - 0.01);
//				//return color;
 
				// 平均以当前像素为中心的九宫格的颜色
                //--------------add------------------
    //            float distance = _Distance;
    //            fixed4 computedColor = tex2D(_MainTex, IN.texcoord) * IN.color;
    //            computedColor += tex2D(_MainTex, half2(IN.texcoord.x + distance , IN.texcoord.y + distance )) * IN.color;
    //            computedColor += tex2D(_MainTex, half2(IN.texcoord.x - distance , IN.texcoord.y - distance )) * IN.color;
    //            computedColor += tex2D(_MainTex, half2(IN.texcoord.x + distance , IN.texcoord.y - distance )) * IN.color;
    //            computedColor += tex2D(_MainTex, half2(IN.texcoord.x - distance , IN.texcoord.y + distance )) * IN.color;
    //            computedColor += tex2D(_MainTex, half2(IN.texcoord.x + distance , IN.texcoord.y)) * IN.color;
    //            computedColor += tex2D(_MainTex, half2(IN.texcoord.x , IN.texcoord.y + distance )) * IN.color;
    //            computedColor += tex2D(_MainTex, half2(IN.texcoord.x - distance , IN.texcoord.y)) * IN.color;
    //            computedColor += tex2D(_MainTex, half2(IN.texcoord.x , IN.texcoord.y - distance )) * IN.color;
				//computedColor = computedColor / 9;
                //computedColor = computedColor / 5;
				fixed4 computedColor = tex2D(_MainTex, IN.texcoord) * 0.147761;
				computedColor += tex2D(_MainTex, IN.taps[0]) * 0.118318;
				computedColor += tex2D(_MainTex, IN.taps[1]) * 0.0947416;
				computedColor += tex2D(_MainTex, IN.taps[2]) * 0.118318;
				computedColor += tex2D(_MainTex, IN.taps[3]) * 0.0947416;
				computedColor += tex2D(_MainTex, IN.taps[4]) * 0.118318;
				computedColor += tex2D(_MainTex, IN.taps[5]) * 0.0947416;
				computedColor += tex2D(_MainTex, IN.taps[6]) * 0.118318;
				computedColor += tex2D(_MainTex, IN.taps[7]) * 0.0947416;
				//computedColor = computedColor / 9 * IN.color;
                return computedColor * IN.color;
                //--------------add------------------
			}
		ENDCG
		}
	}
}