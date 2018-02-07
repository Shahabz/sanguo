// Upgrade NOTE: replaced 'mul(UNITY_MATRIX_MVP,*)' with 'UnityObjectToClipPos(*)'

Shader "Custom/Dissolution3D"
{
    Properties
	{
		_MainTex ( "main", 2D ) = "white" {}
        _Color ( "main_c", Color ) = (0,0,0,1)
        _RJL ( "RJL", Range( 0, 22 ) ) = 0
        _RJ ( "RJ", 2D ) = "white" {}
        _sq ("sq", Float ) = 1.1
        _node_9766 ( "node_9766", Range( 0, 22 ) ) = 1
        [HideInInspector]_Cutoff ( "Alpha cutoff", Range( 0, 1 ) ) = 0.5
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

		Cull Off
		Lighting Off
		ZWrite Off
		Fog { Mode Off }
		Blend SrcAlpha OneMinusSrcAlpha
 
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
			};
			
			sampler2D _MainTex;
			fixed4 _Color;
            uniform float _RJL;
            uniform sampler2D _RJ; uniform float4 _RJ_ST;
            uniform float _node_9766;
			uniform float4 _MainTex_ST;
 
			v2f vert(appdata_t IN)
			{
				v2f OUT;
				OUT.vertex = UnityObjectToClipPos(IN.vertex);
				OUT.texcoord = IN.texcoord;
				OUT.color = IN.color;
				return OUT;
			}
 
			fixed4 frag(v2f IN) : SV_Target
			{
/////// Vectors:
                float4 _RJ_var = tex2D( _RJ, TRANSFORM_TEX( IN.texcoord, _RJ ) );
                clip( ( _RJ_var.r * _node_9766 ) - 0.5 );
////// Lighting:
////// Emissive:
                float4 _main_var = tex2D( _MainTex, TRANSFORM_TEX( IN.texcoord, _MainTex ) );
                float4 emissive = lerp( _Color.rgba, _main_var.rgba, step( _RJL, _RJ_var.r ) );
                return emissive * IN.color;
			}
		ENDCG
		}
	}
}
