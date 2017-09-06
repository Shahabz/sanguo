﻿using UnityEngine;
using System.Collections;

namespace UnityEngine.UI
{
    [System.Serializable]
    public class LoopScrollPrefabSource 
    {
        public string prefabName;
		public GameObject prefab;
        public int poolSize = 5;

        private bool inited = false;
        public virtual GameObject GetObject()
        {
            if(!inited)
            {
				SG.ResourceManager.Instance.InitPool(prefabName, prefab, poolSize);
                inited = true;
            }
			return SG.ResourceManager.Instance.GetObjectFromPool(prefabName, prefab);
        }
    }
}
