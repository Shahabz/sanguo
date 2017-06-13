// Upgrade NOTE: replaced 'mul(UNITY_MATRIX_MVP,*)' with 'UnityObjectToClipPos(*)'

Shader "Custom/AnimationAdditive_Cull" {
Properties {
	_Color ( "Tint", Color) = ( 1, 1, 1, 1 )
	_MainTex ( "Base (RGB)", 2D ) = "white" {}
	_AddTex ( "Base (RGB)", 2D ) = "white" {}
	_XSpeed( "X Speed：", Range( -20, 20 ) ) = 1
	_YSpeed( "Y Speed：", Range( -20, 20 ) ) = 1
	_DiscardLine( "DiscardLine：", Float ) = 0
}

SubShader {
	Tags { "RenderType"="Opaque" }
	LOD 100
	
	Pass {  
		CGPROGRAM
			#pragma vertex vert
			#pragma fragment frag
			
			#include "UnityCG.cginc"

			struct appdata_t {
				float4 vertex : POSITION;
				float2 texcoord : TEXCOORD0;
			};

			struct v2f {
				float4 pos : TEXCOORD2;
				float4 vertex : SV_POSITION;
				half2 texcoord : TEXCOORD0;
				half2 animeuv : TEXCOORD1;
			};
			
			fixed4 _Color;
			sampler2D _MainTex;
			sampler2D _AddTex;
			float4 _MainTex_ST;
			fixed _XSpeed;
			fixed _YSpeed;
			float _DiscardLine;
			
			v2f vert ( appdata_t v )
			{
				v2f o;
				o.pos = mul( UNITY_MATRIX_MV, v.vertex );
				o.vertex = UnityObjectToClipPos( v.vertex );
				o.texcoord = TRANSFORM_TEX(v.texcoord, _MainTex);

				fixed xValue = _XSpeed * _Time;		//改变u坐标值 _Time为内置变量
				fixed yValue = _YSpeed * _Time;		//改变V坐标值
				o.animeuv = v.texcoord + fixed2( xValue, yValue );

				return o;
			}
			
			fixed4 frag( v2f i ) : SV_Target
			{
				if( i.pos.y <= _DiscardLine )
				{
					discard;
				}

				fixed4 col = tex2D( _MainTex, i.texcoord );
				fixed4 add = tex2D( _AddTex, i.animeuv ) * _Color;
				col = col + add;
				return col;
			}
		ENDCG
	}
}

}
