// Upgrade NOTE: replaced 'mul(UNITY_MATRIX_MVP,*)' with 'UnityObjectToClipPos(*)'

Shader "Custom/UI_Line" {
Properties {
	_TintColor ("Tint Color", Color) = ( 1,1,1,1 )
	_MainTex ("Particle Texture", 2D) = "white" {}
	
	//-------------------add----------------------
	  _MinX ("Min X", Float) = -10
      _MaxX ("Max X", Float) = 10
      _MinY ("Min Y", Float) = -10
      _MaxY ("Max Y", Float) = 10
      //-------------------add----------------------
	
}
 
Category {
	Tags { "Queue"="Transparent" "IgnoreProjector"="True" "RenderType"="Transparent" }
	Blend SrcAlpha OneMinusSrcAlpha
	Cull Back
	Lighting Off
	
	SubShader {
		Pass {
		
			CGPROGRAM
			#pragma vertex vert
			#pragma fragment frag
			#pragma multi_compile_particles
 
			#include "UnityCG.cginc"
 
			sampler2D _MainTex;
			fixed4 _TintColor;
			//-------------------add----------------------
			float _MinX;
            float _MaxX;
            float _MinY;
            float _MaxY;
            //-------------------add----------------------
			
			struct appdata_t {
				float4 vertex : POSITION;
				fixed4 color : COLOR;
				float2 texcoord : TEXCOORD0;
			};
 
			struct v2f {
				float4 vertex : SV_POSITION;
				fixed4 color : COLOR;
				float2 texcoord : TEXCOORD0;
				//-------------------add----------------------
				float3 vpos : TEXCOORD2;
				//-------------------add----------------------
			};
			
			float4 _MainTex_ST;
 
			v2f vert (appdata_t v)
			{
				v2f o;
				//-------------------add----------------------
				o.vpos = v.vertex.xyz;
				//-------------------add----------------------
				o.vertex = UnityObjectToClipPos(v.vertex);
				o.color = v.color;
				o.texcoord = TRANSFORM_TEX(v.texcoord,_MainTex);
				return o;
			}
 
			sampler2D_float _CameraDepthTexture;
			float _InvFade;
			
			fixed4 frag (v2f i) : SV_Target
			{				
				//-------------------add----------------------
				 fixed4 c =2.0f * i.color * _TintColor * tex2D(_MainTex, i.texcoord);
				 c.a *= (i.vpos.x >= _MinX);
	           	 c.a *= (i.vpos.x <= _MaxX);
	             c.a *= (i.vpos.y >= _MinY);
	             c.a *= (i.vpos.y <= _MaxY);
                 c.rgb *= c.a;
                return c;
                //-------------------add----------------------
			}
			ENDCG 
		}
	}	
}
}