// Upgrade NOTE: replaced 'mul(UNITY_MATRIX_MVP,*)' with 'UnityObjectToClipPos(*)'

Shader "Custom/WaterAnimation"
{
	Properties
	{
		_Color ( "Tint", Color) = ( 1, 1, 1, 1 )
		_MainTex ( "Texture：", 2D ) = "white" {}
		_BumpMap ( "NoiseMap", 2D ) = "bump" {}
		_XSpeed( "X Speed：",Range( -10, 10 ) ) = 1
		_YSpeed( "Y Speed：",Range( -10, 10 ) ) = 1
		_BumpAmt  ("Distortion", range (0,128)) = 10
		_AddTex ( "Addtive：", 2D ) = "white" {}
		_XAddSpeed( "X Speed：",Range( -10, 10 ) ) = 1
		_YAddSpeed( "Y Speed：",Range( -10, 10 ) ) = 1
		_AddColor( "Add Color：", Color) = ( 1, 1, 1, 1 )
	}
	SubShader
	{
		Tags
		{
			"Queue" = "Transparent" 
			"IgnoreProjector" = "True" 
			"RenderType"="Transparent" 
			"PreviewType"="Plane"
		}

		Cull Off
		Lighting Off
		//ZWrite Off

		Pass
		{
			CGPROGRAM
			#pragma vertex vert
			#pragma fragment frag
			#pragma multi_compile _ PIXELSNAP_ON
			#include "UnityCG.cginc"

			sampler2D _MainTex : register( s0 );
			sampler2D _BumpMap : register( s1 );
			sampler2D _AddTex : register( s2 );
			sampler2D_float _CameraDepthTexture;
            float4 _MainTex_ST;
            float4 _BumpMap_ST;
            float4 _AddTex_ST;
			float4 _MainTex_TexelSize;
			fixed _XSpeed;
			fixed _YSpeed;
			float _BumpAmt;
			fixed _XAddSpeed;
			fixed _YAddSpeed;
			float4 _AddColor;

			struct appdata_t
			{
				float4 vertex   : POSITION;
				float4 color    : COLOR;
				float2 texcoord : TEXCOORD0;
			};

			struct v2f
			{
				float4 vertex   : SV_POSITION;
				float4 color    : COLOR;
				float2 texcoord  : TEXCOORD0;
				float2 addcoord  : TEXCOORD1;
				float2 uv : TEXCOORD2;
			};
			
			fixed4 _Color;

			v2f vert( appdata_t IN )
			{
				v2f OUT;
				OUT.vertex = UnityObjectToClipPos( IN.vertex );
				OUT.uv = IN.texcoord;
				OUT.color = IN.color * _Color;				
		    
				fixed xValue = _XSpeed * _Time;		//改变u坐标值 _Time为内置变量
				fixed yValue = _YSpeed * _Time;		//改变V坐标值
				fixed xAdd = _XAddSpeed * _Time;
				fixed yAdd = _YAddSpeed * _Time;
				OUT.texcoord = IN.texcoord + fixed2( xValue, yValue );
				OUT.addcoord = IN.texcoord + fixed2( xAdd, yAdd );

				return OUT;
			}

			fixed4 frag( v2f IN ) : COLOR
			{
				//half2 bump = tex2D( _BumpMap, IN.texcoord ).rb; 
				float2 bump = UnpackNormal( tex2D( _BumpMap, TRANSFORM_TEX( IN.uv, _BumpMap ) ) ).gb;
				float2 UV = IN.texcoord + bump * _BumpAmt * _MainTex_TexelSize.xy;
				UV = TRANSFORM_TEX( UV, _MainTex ); 
				fixed4 c = tex2D( _MainTex, UV );

				float2 addUV = TRANSFORM_TEX( IN.addcoord, _AddTex );
				fixed4 add = tex2D( _AddTex, addUV );
				
				return c * _Color + add * _AddColor * _AddColor.a;
			}

			ENDCG
		}
	}
	FallBack "Diffuse"
}
