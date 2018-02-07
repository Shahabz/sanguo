// Upgrade NOTE: replaced 'mul(UNITY_MATRIX_MVP,*)' with 'UnityObjectToClipPos(*)'

Shader "Custom/SpriteGray"
{	
	Properties
	{
		[PerRendererData] _MainTex ("Sprite Texture", 2D) = "white" {}
		[MaterialToggle] PixelSnap ("Pixel snap", Float) = 0

		
        _Color ("主颜色", Color) = (1,1,1,1)                       // 主色
		_DissolveText ("溶解贴图", 2D) = "white" {}                 // 溶解贴图  
		_Tile("溶解贴图的平铺大小", Range (0, 1)) = 1                // 平铺值,设置溶解贴图大小
		_Amount ("溶解值", Range (0, 1)) = 0.5                     // 溶解度  
		_DissSize("溶解大小", Range (0, 1)) = 0.1                   // 溶解范围大小
		_DissColor ("溶解主色", Color) = (1,1,1,1)                  // 溶解颜色  
		_AddColor ("叠加色，与主色叠加为开始色[R|G|B>0表示启用]", Color) = (1,1,1,1) // 改色与溶解色融合形成开始色  
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
		Blend One OneMinusSrcAlpha

		Pass
		{
		CGPROGRAM
			#pragma vertex vert
			#pragma fragment frag
			#pragma multi_compile _ PIXELSNAP_ON
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

			v2f vert(appdata_t IN)
			{
				v2f OUT;
				OUT.vertex = UnityObjectToClipPos(IN.vertex);
				OUT.texcoord = IN.texcoord;
				OUT.color = IN.color;
				#ifdef PIXELSNAP_ON
				OUT.vertex = UnityPixelSnap (OUT.vertex);
				#endif

				return OUT;
			}

			sampler2D _MainTex;
			sampler2D _AlphaTex;
			float _AlphaSplitEnabled;
			sampler2D _DissolveText;  
			fixed4 _Color;          // 主色  
			half _Tile;             // 平铺值  
			half _Amount;           // 溶解度  
			half _DissSize;         // 溶解范围  
			half4 _DissColor;       // 溶解颜色  
			half4 _AddColor;        // 叠加色

			// 最终色  
			static half4 finalColor = float4( 1, 1, 1, 1 );  

			fixed4 frag( v2f IN ) : SV_Target
			{
				// 对主材质进行采样  
				fixed4 tex = tex2D( _MainTex, IN.texcoord ) * _Color;
				// 对裁剪材质进行采样，取R色值  
				float ClipTex = tex2D ( _DissolveText, IN.texcoord / _Tile).r;

				// 裁剪量 = 裁剪材质R - 外部设置量  
				float ClipAmount = ClipTex - _Amount;  
				if(_Amount > 0)  
				{  
					// 如果裁剪材质的R色值 < 设置的裁剪值  那么此点将被裁剪  
					if(ClipAmount < 0)  
					{  
						clip(-0.1);  
					}  
					// 然后处理没有被裁剪的值  
					else  
					{  
						// 针对没有被裁剪的点，【裁剪量】小于【裁剪大小】的做处理  
						// 如果设置了叠加色，那么该色为ClipAmount/_DissSize(这样会形成渐变效果)  
						if(ClipAmount < _DissSize)  
						{  
							if(_AddColor.x == 0)  
								finalColor.x = _DissColor.x;  
							else  
								finalColor.x = ClipAmount / _DissSize;  

							if (_AddColor.y == 0)  
								finalColor.y = _DissColor.y;  
							else  
								finalColor.y = ClipAmount / _DissSize;  

							if (_AddColor.z == 0)  
								finalColor.z = _DissColor.z;  
							else  
								finalColor.z = ClipAmount / _DissSize;  
							// 融合  
							tex = tex * finalColor * 2;  
						}  
					}  
				}
				return tex;
			}

		ENDCG
		}
	}
}
