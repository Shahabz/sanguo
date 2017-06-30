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

-- 玩家头像
function PlayerHeadSprite( shape )
	local name = string.format("playerhead_%02d", shape+1 );
	local sprite = LoadSprite( name );
	if sprite == nil then
		sprite = LoadSprite( "Char_Default" );
	end
	return sprite;
end

-- 玩家半身像
function PlayerFaceSprite( shape )
	local name = string.format("playerhead_%02d_face", shape+1 );
	local sprite = LoadSprite( name );
	if sprite == nil then
		sprite = LoadSprite( "Char_Default" );
	end
	return sprite;
end

-- 
function BuildingSprite( kind )
	local name = string.format("building_kind%d", kind );
	local sprite = LoadSprite( name );
	if sprite == nil then
		sprite = LoadSprite( "Char_Default" );
	end
	return sprite;
end
