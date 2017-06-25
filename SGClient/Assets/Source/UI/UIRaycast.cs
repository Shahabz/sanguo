using UnityEngine;
using System.Collections;

namespace UnityEngine.UI
{
	public class UIRaycast : MaskableGraphic
	{
		protected UIRaycast()
		{
			useLegacyMeshGeneration = false;
		}

		protected override void OnPopulateMesh(VertexHelper toFill)
		{
			toFill.Clear();
		}
	}
}