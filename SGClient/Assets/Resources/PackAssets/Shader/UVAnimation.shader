// Upgrade NOTE: replaced 'mul(UNITY_MATRIX_MVP,*)' with 'UnityObjectToClipPos(*)'

Shader "Custom/UVAnimation"
{
	Properties
	{
		_MainTex ( "Texture：", 2D ) = "white" {}
		_Color ( "Tint", Color) = ( 1, 1, 1, 1 )
		_XSpeed( "X Speed：",Range( -10, 10 ) ) = 1
		_YSpeed( "Y Speed：",Range( -10, 10 ) ) = 1
	}
	SubShader
	{
		Tags { "RenderType" = "Opaque" }
		Cull Off
		Lighting Off
		ZWrite Off
		Blend One OneMinusSrcAlpha

		Pass
		{
			CGPROGRAM
			#pragma vertex vert
			#pragma fragment frag
			#pragma multi_compile _ PIXELSNAP_ON
			#include "UnityCG.cginc"

			sampler2D _MainTex;
			fixed _XSpeed;
			fixed _YSpeed;

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
			
			fixed4 _Color;

			v2f vert( appdata_t IN )
			{
				v2f OUT;
				OUT.vertex = UnityObjectToClipPos( IN.vertex );
				OUT.texcoord = IN.texcoord;
				OUT.color = IN.color * _Color;
				#ifdef PIXELSNAP_ON
				OUT.vertex = UnityPixelSnap ( OUT.vertex );
				#endif

				return OUT;
			}

			fixed4 frag( v2f IN ) : SV_Target
			{		
				fixed2 UV = IN.texcoord;
		    
				fixed xValue = _XSpeed * _Time;		//改变u坐标值 _Time为内置变量
				fixed yValue = _YSpeed * _Time;		//改变V坐标值
		    
				UV += fixed2( xValue, yValue );
		    
				fixed4 c = tex2D( _MainTex, UV );
				return c;
			}

			ENDCG
		}
	}
	FallBack "Diffuse"
}
