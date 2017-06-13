-- 资源管理函数
-- 载入Sprite
function LoadSprite( name )
    return ResourceManager.LoadSprite( name );
end

-- 载入Prefab
function LoadPrefab( name )
    return ResourceManager.LoadPrefab( name );
end

-- 异步载入Sprite
function LoadSpriteAsyn( name, callback )
	eye.resourceManager:LoadSpriteAsyn( name, callback );
end

-- 异步载入Prefab
function LoadPrefabAsyn( name, callback )
	eye.resourceManager:LoadPrefabAsyn( name, callback );
end
